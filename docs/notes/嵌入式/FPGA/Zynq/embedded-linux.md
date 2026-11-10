# Zynq 嵌入式 Linux 与软硬件协同

本教程沿着一套 Zynq-7000 实验展开：上电后启动 Linux，用处理器访问 FPGA 中的跑马灯和按键，让应用睡眠等待按键事件，再用定时器观察不同负载下的响应延迟。这里的处理系统（Processing System, PS）包括 Cortex-A9 处理器和片上控制器，可编程逻辑（Programmable Logic, PL）是用来搭建数字电路的区域。AXI 是两侧交换地址、数据和响应的一组总线接口规范。

示例采用 Zynq-7000、32 位 Arm、非安全启动和教学用的自定义 PL 外设。开发板型号、硬件设计文件和课程镜像尚未指定，示例地址、寄存器协议、工具配置和日志均为教学约定。实际工程以板卡原理图、硬件导出文件、IP 寄存器表及所选版本的文档为准。

```text
开发主机：硬件设计 + 交叉编译 → 启动镜像、内核、设备树、根文件系统
目标板：  上电 → 引导程序 → Linux → 驱动 → 用户程序
控制路径：用户程序 → 驱动 → PS 的 AXI 端口 → PL 外设
事件路径：PL 外设 → PS 中断控制器 → 驱动记录事件 → 用户程序恢复运行
```

## 嵌入式系统基础

### 嵌入式系统概述

嵌入式系统（Embedded System）是把计算能力放进一台设备中，完成设备规定的功能。比如电机控制器读取传感器、计算控制量、更新输出，并在传感器故障时进入安全状态。处理器、存储器、外围电路、固件和应用共同决定设备的行为。

| 约束 | 设计时具体考虑什么 |
| --- | --- |
| 功耗与能量 | 功率表示单位时间消耗的能量。设备以 2 W 连续运行 10 h，理想情况下消耗 20 Wh。电池设计还要考虑转换损耗、峰值电流和待机时间。 |
| 内存与计算 | 内核、驱动和应用需要代码、缓冲区与栈空间。采样和处理速度不匹配时，数据会在缓冲区累积。 |
| 成本 | 器件、板层、存储容量、供电、散热以及生产测试都会影响总成本。 |
| 实时性 | 结果要在规定时间内交付。偶尔超时对日志可能只影响体验，对控制回路则可能破坏功能。 |
| 可靠性 | 考虑掉电、总线超时、重启、异常输入以及升级失败。看门狗、错误检测和恢复路径都需要与实际故障模型对应。 |

主机（Host）运行 Vivado、Vitis、编译器和调试器。目标机（Target）是运行产物的板卡。比如在 x86-64 主机编译 Arm 程序，之后通过 SD 卡、网络或 JTAG 将结果送到目标机。主机能生成该程序，并不代表主机 CPU 能执行里面的 Arm 指令。

### 嵌入式软件运行模式

| 模式 | 谁管理执行 | 常见适用场景 |
| --- | --- | --- |
| 裸机（Bare Metal） | 启动代码初始化运行环境，主循环和中断处理函数直接协调工作 | 工作流简单、对寄存器或时序要求直接 |
| 实时操作系统（RTOS） | 调度器按优先级等规则运行任务，内核提供队列、定时器和同步对象 | 多个任务需要有可分析的响应时间 |
| 嵌入式 Linux | 内核管理进程、虚拟内存、文件系统、网络和驱动，应用通过系统调用访问资源 | 需要网络协议、进程隔离和丰富用户空间工具 |

同一个按钮在三种系统中的分工很直观。裸机可以在中断中设置标志，由主循环消费。RTOS 可以通知等待中的任务。Linux 驱动可以记录事件并唤醒阻塞在设备文件上的应用。需要多任务时，可继续阅读 [STM32 RTOS 与 FreeRTOS 笔记](../../stm32/rtos_freertos_notes_compact.md)。

## 编译与可执行文件

### 交叉编译

编译器把 C 源码翻译为目标机器指令，汇编器把汇编变成包含机器码、符号和重定位记录的目标文件（`.o`），链接器把多个目标文件及库组合，解析函数和变量引用，再按地址布局生成可执行文件。以 GCC 驱动程序为例，`-S` 停在汇编输出，`-c` 输出目标文件，省略两者通常继续链接。

假设 `main.o` 调用 `gpio_write`，编译它时只需知道函数声明。链接时再从 `gpio.o` 找到实现，把调用位置关联到最终地址。地址修正由重定位记录描述。

| 工具链前缀 | 用途与运行环境 |
| --- | --- |
| `arm-none-eabi-` | 适合 Arm 裸机目标，常配合板级支持包和链接脚本。应用负责与板上硬件或裸机运行库对接。 |
| `arm-linux-gnueabihf-` | 适合采用相应 Arm GNU/Linux 硬浮点 ABI 的目标。用户态应用所用的 libc、动态加载器和其他库应与目标根文件系统匹配。 |

指令集规定 CPU 能解码哪些指令。应用二进制接口（Application Binary Interface, ABI）规定已编译代码如何协作，例如参数通过哪些寄存器传递、栈如何对齐、浮点参数按什么规则传递。硬浮点 ABI 与 CPU 是否具备浮点执行单元是两个层面的约束。即便两个文件都显示为 Arm ELF，指令、ABI 或用户态库不兼容仍会导致运行失败。用户程序的 sysroot 是交叉工具链查找目标头文件和库的一棵目录树。内核和裸机程序不使用 Linux 用户态 libc。参见 [Arm AAPCS32](https://github.com/ARM-software/abi-aa/blob/main/aapcs32/aapcs32.rst)。

### 可执行文件

可执行与可链接格式（Executable and Linkable Format, ELF）用结构化方式保存代码和元数据。ELF Header 给出文件类别、字节序、机器类型和入口地址（Entry Point）。入口通常指向启动代码，例如裸机初始化函数或 Linux 程序的 `_start`。运行环境准备完成后才调用 C 的 `main`。

| ELF 视图 | 内容及用途 |
| --- | --- |
| 节（Section） | 链接器把用途相近的内容分组。常见的有指令 `.text`、只读常量 `.rodata`、有初值且可写的 `.data` 和零初始化区 `.bss`。符号表帮助定位名称，调试信息帮助调试器把地址对应到变量或源码行。 |
| 段（Segment） | 程序头表告诉加载器如何映射运行所需的区域。一个 `PT_LOAD` 可以包含多个节。文件中占用的字节数可以小于段在内存中占用的字节数，多出的部分按 ELF 规则清零。 |

裸机链接脚本（Linker Script，例如 `lscript.ld`）声明可用的存储区并决定输出节放到哪里。链接时给出的运行地址是 VMA，镜像中内容用于初始化的加载地址是 LMA。若设计让已初始化数据存放在非易失存储器，而运行时要修改它，脚本可把 `.data` 的 VMA 放在 RAM，把 LMA 放在镜像存储区。启动代码按链接符号复制初值并清零 `.bss`。例如 `.data` 有 16 字节，启动代码复制完整 16 字节；`.bss` 有 64 字节，则只需保留并清零 64 字节 RAM。

```ld
/* 教学片段。FLASH、RAM 需要在工程的 MEMORY 中先定义。 */
SECTIONS {
    .text : { *(.text*) *(.rodata*) } > FLASH
    .data : { __data_start = .; *(.data*) __data_end = .; } > RAM AT> FLASH
    __data_load = LOADADDR(.data);
    .bss (NOLOAD) : { __bss_start = .; *(.bss*) *(COMMON) __bss_end = .; } > RAM
}
```

这个布局用于理解 ROM 到 RAM 的初始化。Zynq 能否从 QSPI 原地执行取决于具体启动配置。实际工程必须让链接布局、引导程序的装载策略及启动代码一致。Linux 应用通常由内核和动态加载器按程序头建立进程映射，应用的虚拟地址、文件偏移、启动镜像在闪存中的位置各有不同含义。参见 [GNU ld 的输出节加载地址](https://sourceware.org/binutils/docs/ld/Output-Section-LMA.html)。

在主机上用 `readelf -h app.elf` 看机器类型和入口，`readelf -S app.elf` 看节，`readelf -l app.elf` 看段和节的归属，`readelf -A app.elf` 检查 Arm 属性。程序如何从静态文件变成进程可对照 [ELF Program Loading](https://gabi.xinuos.com/elf/07-pheader.html)。

## 存储器与硬件架构

### Zynq 存储器

上电后的关键问题是 CPU 最早能从哪里取指、能在哪里放栈，以及何时能使用容量更大的 DDR。

| 存储部件 | 掉电后内容 | 用途 |
| --- | --- | --- |
| ROM（Read-Only Memory） | 保留 | 片内 BootROM 提供芯片复位后的初始引导代码。 |
| OCM（On-Chip Memory） | 丢失 | 片上 RAM，在外部 DDR 可用前可放早期程序和栈。 |
| DDR（Double Data Rate SDRAM） | 丢失 | 板外大容量 RAM。PS 的 DDR 控制器需要按具体板卡配置时钟、时序和校准等参数。 |
| QSPI Flash（Quad Serial Peripheral Interface Flash） | 保留 | 持久保存启动镜像等数据，擦写粒度、容量和分区取决于器件与设计。 |

如果程序链接到 DDR，加载器把内容送入 DDR 前就要完成 DDR 初始化。否则即使 CPU 可以通过 JTAG 暂停，向该地址下载程序也可能失败。已有的 [Zynq 乘法加速实验](../zynq-hardware-acceleration-intro.md) 展示了用 OCM 验证最小裸机链路的做法。启动顺序参见 [AMD UG585 Basic Boot Sequence](https://docs.amd.com/r/en-US/ug585-zynq-7000-SoC-TRM/Basic-Boot-Sequence)。

### Zynq 地址空间

内存映射（Memory Map）把地址范围分配给 RAM、控制器或外设。处理器访问普通 RAM 时读写的是存储内容。访问内存映射 I/O（Memory-Mapped I/O, MMIO）地址时，总线把操作交给外设，写入可能改变引脚或启动一次硬件事务。

```text
Cortex-A9 → PS 地址译码 ──→ OCM 或 DDR
                       └──→ M_AXI_GP0 → AXI 互连 → PL 的 AXI4-Lite 外设
```

PS 到 PL 的通用 AXI 接口（General Purpose, GP）可用于读写控制寄存器。Vivado 的 AXI Address Editor 分配外设窗口。外设内部再按寄存器偏移译码。若窗口基址为 `0x41200000`，某寄存器偏移为 `0x04`，则物理地址为 `0x41200004`。在 AXI GPIO 的常见配置中，通道一数据和方向寄存器分别在 `0x00` 与 `0x04`，方向位 1 代表输入。地址与可用寄存器要分别查设计和 IP 文档。

裸机代码可用 BSP 的 `Xil_In32`、`Xil_Out32` 做指定宽度的访问。C 的 `volatile` 可约束编译器对所标记访问的省略与合并，但不会建立线程同步、设置设备内存属性或保证所有总线顺序。Linux 内核使用相应 MMIO 映射与访问器。参见 [AMD PG144 寄存器空间](https://docs.amd.com/r/en-US/pg144-axi-gpio/Register-Space) 和 [Linux 设备 I/O](https://docs.kernel.org/driver-api/device-io.html)。

## 系统启动

### 启动基础

上电复位（Power-On Reset, POR）让硬件进入已定义状态。处理器从复位向量所指定的启动路径开始取指。这个硬件起点、ELF Header 的软件入口地址、U-Boot 选择的内核加载地址分别属于不同阶段。

Zynq-7000 的启动模式由复位期间采样的绑带状态决定。开发板跳线或拨码可设置这些复用 I/O（Multiplexed I/O, MIO）引脚的启动电平。切换后按板卡说明重新上电或触发相应复位。JTAG 模式让开发主机通过调试接口初始化和下载程序。QSPI 和 SD 模式可从相应介质找到启动镜像，实现脱离主机上电启动。参见 [AMD UG585 Boot Mode Pin Settings](https://docs.amd.com/r/en-US/ug585-zynq-7000-SoC-TRM/Boot-Mode-Pin-Settings)。

### Zynq 启动阶段

| 阶段 | 输入与交接 |
| --- | --- |
| BootROM | 片内固化程序检查启动模式和镜像头。在典型的非安全存储介质启动中，把第一阶段引导程序（First Stage Boot Loader, FSBL）送到 OCM 后交出控制权。某些受支持的配置可从闪存原地执行。 |
| FSBL | 使用硬件设计生成的 PS 配置，例如 `ps7_init`，初始化时钟、MIO 和 DDR。按镜像配置装载 PL 位流和后续分区，再把控制权交给裸机应用或 U-Boot。 |
| U-Boot | 作为本实验的后续引导程序，从存储器或网络取内核、设备树以及可选的初始 RAM 文件系统，准备启动参数，进入内核入口。 |
| Linux 内核 | 建立内存管理、调度和中断环境，按设备树识别硬件并初始化驱动。随后挂载根文件系统或先使用 initramfs，启动用户空间的首个进程。 |
| 用户空间 | 进程号为 1 的 init 按系统配置启动服务。精简镜像可以用 BusyBox `/sbin/init`，并通过 `inittab` 调用 `/etc/init.d/rcS`。串口登录程序或启动脚本再提供 Shell。 |

FSBL 写入 PL 位流后，外设还要有正确的时钟、复位和地址配置。设备树的节点描述应与实际位流对应，否则内核可能创建一个逻辑设备，但访问不到预期电路。启动阶段可对照 [AMD Zynq-7000 Boot and Configuration](https://docs.amd.com/r/2024.1-English/ug1283-bootgen-user-guide/Zynq-7000-SoC-Boot-and-Configuration) 和 [Booting ARM Linux](https://docs.kernel.org/arch/arm/booting.html)。

### 启动文件和工具

| 产物 | 生成方式与消费方 |
| --- | --- |
| `.xsa` | Vivado 导出的硬件平台描述，供 Vitis 构建平台和 FSBL 等软件。是否包含位流取决于导出选项。 |
| `.bit` | Vivado 生成的 PL 位流，用于配置可编程逻辑。 |
| `fsbl.elf` | 从匹配 XSA 的 Vitis 平台生成的 Zynq-7000 FSBL。 |
| `.bif` 与 `BOOT.BIN` | BIF 描述分区顺序和属性，Bootgen 将分区打包成 BootROM 和 FSBL 可读的镜像。 |
| `uImage` | 旧式 U-Boot 内核镜像格式，带供 `bootm` 使用的头。项目也可能使用 `zImage` 或 FIT，相应启动命令不同。 |
| `.dtb` | 设备树二进制（Device Tree Blob, DTB），由 DTS 编译得到，供引导程序传给内核。 |

从 Vivado 导出 XSA 时，如果希望 Vitis 同时取得位流，需要在导出选项中包含它。之后以该 XSA 创建或更新 Vitis 平台，再生成 FSBL。更换 DDR 设置或 PL 硬件设计后，应核对 FSBL、位流和设备树是否来自同一硬件配置。参见 [AMD Zynq 硬件导出教程](https://docs.amd.com/r/en-US/xd131-zynq-embedded-design-tutorial/Step-8-Export-the-Design-to-the-Vitis-software-platform)。

```text
the_ROM_image:
{
    [bootloader] fsbl.elf
    system.bit
    u-boot.elf
}
```

把上面的分区配置保存为 `boot.bif`，由匹配工程版本的工具执行 `bootgen -arch zynq -image boot.bif -o BOOT.BIN`。此例的镜像包含 U-Boot，内核和 DTB 由 U-Boot 另行加载。QSPI 烧录时，按板卡的闪存类型和分区布局选择工具、偏移、写入范围及校验方式。先确认擦除范围和恢复路径，避免影响其他分区。

U-Boot 的环境变量 `bootcmd` 描述自动启动时执行的命令，`bootargs` 通常保存传给 Linux 的命令行参数。前者可能加载文件、设置变量并执行 `bootm`，后者说明控制台、根文件系统等。可在 U-Boot 用 `printenv bootcmd bootargs` 检查，启动后用 Linux 的 `cat /proc/cmdline` 核对生效内容。参见 [Bootgen BIF](https://docs.amd.com/r/2023.2-English/ug1283-bootgen-user-guide/Boot-Image-Format-BIF) 和 [U-Boot bootm](https://docs.u-boot.org/en/latest/usage/cmd/bootm.html)。

### 启动过程对比

| 路径 | 谁装载程序 | 适用验证 |
| --- | --- | --- |
| Vitis JTAG 下载 | 主机脚本可初始化 PS、配置 PL、下载 ELF 和启动 CPU，细节由调试配置决定 | 先验证硬件和程序，快速迭代。 |
| 裸机独立启动 | BootROM → FSBL → 裸机应用 | 验证无需主机时钟、DDR、位流及软件是否按顺序就绪。 |
| 嵌入式 Linux 启动 | BootROM → FSBL → U-Boot → 内核 → init | 验证从启动介质到驱动和用户空间的完整交接。 |

调试器曾初始化 DDR、加载位流或设置 CPU 状态时，JTAG 运行成功只验证了该调试流程。独立启动应冷启动验证，并用串口标记定位最后一个到达的阶段。

## 嵌入式 Linux 系统

### 操作系统基础

操作系统（Operating System, OS）协调 CPU、内存、存储、网络和外设。内核负责有特权的部分，例如切换进程、设置页表和处理 IRQ。一个可用的 Linux 系统还需要用户空间程序、库和配置，例如 init、Shell 和应用。硬件驱动把具体设备的细节收在一处，使应用可以通过文件接口使用不同设备。进程隔离依赖地址映射和访问权限，使一个进程对自身缓冲区的写入不会直接修改另一个进程的同名地址。

### Linux 内核架构

| 架构 | 服务放在哪里 | 对执行路径的影响 |
| --- | --- | --- |
| 单体内核（Monolithic Kernel） | 调度、文件系统和许多驱动在内核空间 | 服务之间可以直接调用，驱动缺陷也可能影响整个内核。 |
| 模块化单体内核 | 模块可按需装载，装载后仍在内核空间执行 | Linux 可以把驱动编入内核，也可以把它做成 `.ko` 动态加载。 |
| 微内核（Microkernel） | 内核提供少量基本机制，部分服务在独立进程中 | 服务间依赖进程间通信（Inter-Process Communication, IPC），可以按服务划分隔离边界。 |

内核态驱动能直接使用内核的中断和设备资源接口。用户态驱动则需要内核提供映射和事件通知接口，例如合适的 UIO 设备。内核与应用之间的系统调用，和应用进程之间用于交换数据的管道、共享内存或套接字，解决的是不同问题。管道与共享内存示例见 [Linux 基础笔记](../../linux/basic.md)。

### 根文件系统

根文件系统（Root Filesystem, Rootfs）是挂载在 `/` 的目录树。它给内核启动的用户程序提供可执行文件、库和配置。传统布局中，`/bin` 放基础命令，`/sbin` 放系统管理程序，`/etc` 放配置，`/lib` 放运行库，`/dev` 放设备节点。某些布局会把前两个目录做成指向 `/usr` 的链接。BusyBox 可用一个二进制提供多种命令，适合构建小型用户空间。它的 init 依据 `inittab` 启动脚本或终端，具体文件由镜像配置决定。

网络文件系统（Network File System, NFS）允许目标板挂载主机导出的目录。把该目录作为 Rootfs 后，更新目标程序可在主机上完成，重启或重新运行时无需重刷 QSPI。直接从 NFS 挂根时，内核需要在挂载前具备网卡、网络配置和 NFS 根挂载能力，相关支持应内建，或由先启动的 initramfs 提供所需组件。下面是静态网络示例，地址、网卡名、导出目录和串口参数均需匹配实验环境。

```text
console=ttyPS0,115200 root=/dev/nfs rw
nfsroot=192.0.2.10:/srv/target-root
ip=192.0.2.20:192.0.2.10::255.255.255.0:zynq:eth0:off
```

把这些字段按一行作为内核命令行传入。串口控制台使用通用异步收发器（Universal Asynchronous Receiver/Transmitter, UART）输出文字。主机可用 `tio -b 115200 /dev/ttyUSB0` 打开示例端口。上电后从串口日志区分 FSBL、U-Boot、内核和 init。若网络已通但 NFS 挂载失败，重点检查导出权限、根路径和内核参数。参见 [Linux NFS Root](https://docs.kernel.org/admin-guide/nfs/nfsroot.html)。

### 嵌入式 Linux 构建

| 产物 | 构建入口与主要配置 |
| --- | --- |
| Linux 内核与 DTB | 选定源码和板级配置后，以 `ARCH=arm` 和对应的 `CROSS_COMPILE` 构建目标内核与 `dtbs`。内核配置应满足串口、网络和预定根文件系统的早期启动需求。 |
| U-Boot | 使用该板的 U-Boot defconfig 交叉编译，核对串口、内存、启动介质和网络能力。生成物需适合 FSBL 的交接方式。 |
| BusyBox | 选取命令和 init 等功能，交叉编译并安装到 Rootfs 暂存目录。动态链接版本还需目标动态加载器和共享库。 |
| DTS → DTB | 设备树编译器（Device Tree Compiler, DTC）把文本描述编译为二进制。内核板级 DTS 往往引用头文件和其他 DTSI，由内核构建系统完成预处理与编译。 |

例如，在已选好板级配置的内核源码树中，可以用 `make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage dtbs` 生成 `zImage` 和板级 DTB。若启动脚本使用旧式 `uImage`，还需按项目约定用 `mkimage` 加头，并核对装载地址和入口。独立且无需 C 预处理的 `board.dts` 可以用 `dtc -I dts -O dtb -o board.dtb board.dts` 单独编译。交叉工具链前缀需换成实际安装的版本。U-Boot 的板级配置和编译方式见 [U-Boot Building with GCC](https://docs.u-boot.org/en/latest/build/gcc.html)。

Buildroot 可统一配置和生成工具链、Bootloader、内核与 Rootfs。Yocto Project 的构建体系通过层（layer）和配方（recipe）描述软件与板级配置，适合可维护的定制发行版。课程可以先手工构建关键部件，明确每个产物的职责，再用构建系统记录可复现配置。参见 [Buildroot 手册](https://buildroot.org/downloads/manual/manual.html) 和 [Yocto Project Concepts](https://docs.yoctoproject.org/overview-manual/concepts.html)。

## Linux 执行与权限模型

### 用户空间与内核空间

用户空间（User Space）和内核空间（Kernel Space）描述内存可见性与软件职责。用户态（User Mode）和特权态（Privileged Mode）描述 CPU 正以何种权限执行指令。硬件权限级别决定当前指令能否访问控制寄存器和受保护的映射。内核用这套机制保护自身代码、其他进程的数据和设备寄存器。

一个线程在用户态调用驱动时，进入内核处理请求后通常仍代表原线程执行。若请求能立即完成，内核可以直接返回用户态。只有需要调度等情况才会切换运行线程。模式切换与进程切换可以分开理解。

### 系统调用

系统调用（System Call）是应用请求内核服务的入口。C 库提供包装函数，按 ABI 放置系统调用号和参数。32 位 Arm Linux 常见的 EABI 路径通过 `svc`（Supervisor Call）触发同步异常，由内核分派到对应服务，再把结果交还给包装函数。包装函数的名称与最终调用的内核入口不一定一一对应，例如库实现可以用 `openat` 实现 `open`。

| 接口 | 对设备文件的意义 |
| --- | --- |
| `open()` | 按路径取得文件描述符（File Descriptor），即进程里引用已打开文件的整数。 |
| `read()`、`write()` | 通过描述符请求读写，返回实际处理的字节数，可能阻塞或部分完成。 |
| `close()` | 关闭当前描述符的引用。共享同一打开文件的其他引用可能仍有效。 |
| `mmap()` | 建立虚拟地址区域。后续普通内存指令访问该区域，具体语义由映射类型和驱动决定。 |
| `ioctl()` | 执行设备专用控制，命令号与参数布局由驱动的用户接口约定。 |

例如，应用对 `/dev/btns` 发出 `read(fd, &event, sizeof(event))`。内核先找到 `fd` 对应的打开文件，再调用该设备的读实现。没有事件时，驱动让线程等待。数据就绪后，驱动把记录拷回用户缓冲区，C 库向应用返回结果。

`man 2 read` 查看系统调用接口，`man 3 pthread_create` 查看库接口，`man 4 mem` 查看设备文件接口。系统调用成本包括模式切换和检查，实际延迟也可能含数据复制、等待设备、抢占或调度。批量读取能摊薄固定开销，但批量积累可能增加单个事件等待时间。基本文件调用示例见 [Linux 基础](../../linux/basic.md)。

## Linux 内存管理

### MMU 与地址转换

程序指针包含虚拟地址。内存管理单元（Memory Management Unit, MMU）借助操作系统建立的转换表，将虚拟地址转换为物理地址并检查权限。页是以固定粒度组织的一段连续地址。页表记录虚拟页对应的物理页及权限，CPU 可用转换后备缓冲（Translation Lookaside Buffer, TLB）缓存常用结果。映射也包含内存属性，例如适用于普通缓存内存或设备寄存器的访问属性。

假设页大小为 4096 字节，即 `0x1000`。虚拟地址 `0x20001234` 分成页基址 `0x20001000` 和页内偏移 `0x234`。如果这页映射到物理页 `0x01008000`，转换后的地址就是 `0x01008234`。页内偏移保持相同。恒等映射（Identity Mapping）是虚拟页与物理页基址相等的一种特殊配置，在引导代码切换地址环境时可能用到。

### 进程虚拟内存

进程的虚拟地址空间可以包含代码段、数据段、共享库、堆和线程栈。两个进程可以都使用 `0x20001234`，但页表把它们指向不同物理页。反过来，显式共享内存可以让两个不同的虚拟地址指向同一物理页。页权限控制可读、可写、可执行以及用户态能否访问。

页故障（Page Fault）是 CPU 在地址转换或权限检查时产生的异常。内核有时可以补齐映射，例如首次访问合法的按需分配区域，然后重试指令。若地址无有效映射或权限不允许，Linux 用户进程通常收到 `SIGSEGV`，默认终止。这类非法访问常显示为段错误（Segmentation Fault）。某些映射的后端访问错误也可能产生 `SIGBUS`。

### 用户空间硬件地址映射

`/dev/mem` 是 Linux 暴露物理地址访问的接口之一。受控板级调试时，可用 `open("/dev/mem", O_RDWR)` 打开，再尝试将目标物理窗口映射到用户空间。是否允许访问取决于内核配置、平台、权限及区域状态。不要和已经绑定的驱动同时访问同一设备，也不要用任意寄存器地址试写。

映射偏移需要页对齐。设目标物理地址为 `P`，实际页大小为 `S`。把 `P` 向下取整到页边界得到 `base = P - P % S`，页内偏移为 `delta = P - base`。映射必须至少覆盖 `delta + 访问字节数`。例如以 4 KiB 页访问 `0x43C00004` 的 32 位寄存器，映射物理偏移 `0x43C00000`，返回地址加 4 字节才是该寄存器的用户虚拟地址。

```c
/* 教学片段。假定本板确认物理页可访问，且设备尚未绑定驱动。 */
long page_size = sysconf(_SC_PAGESIZE);
if (page_size <= 0)
    return -1;
off_t phys = 0x43c00004;
off_t page = phys - phys % page_size;
size_t delta = (size_t)(phys - page);
size_t length = delta + sizeof(uint32_t);
int fd = open("/dev/mem", O_RDONLY);
if (fd < 0)
    return -1;
void *mapping = mmap(NULL, length, PROT_READ, MAP_SHARED, fd, page);
if (mapping == MAP_FAILED) {
    close(fd);
    return -1;
}
volatile const uint32_t *reg =
    (volatile const uint32_t *)((unsigned char *)mapping + delta);
uint32_t value = *reg;  /* 仅当硬件规定该寄存器可读、宽度和对齐正确时使用 */
printf("value = 0x%08" PRIx32 "\n", value);
munmap(mapping, length);
close(fd);
```

`PROT_READ`、`PROT_WRITE` 控制映射允许的操作，写映射还要匹配打开权限。`MAP_SHARED` 适用于希望访问共享后端的映射。`MAP_PRIVATE` 对普通文件的典型含义是写入走私有副本，不能用来表达设备寄存器写入。代码用 `MAP_FAILED` 检查失败，退出时用 `munmap()` 释放映射。适合长期使用的设备接口通常由驱动管理资源、权限、中断与并发。

## Linux 设备驱动

### 驱动基础

字符设备（Character Device）通过字节或约定的数据记录向用户空间提供接口，典型入口是 `/dev/xxx`。节点的主设备号、次设备号共同标识字符设备入口。打开节点还受所有权、用户组和权限位约束。一个跑马灯驱动可以只允许设定合法模式与速度，应用无须知道物理地址。相比让用户进程直接访问 `/dev/mem`，驱动可以在一处负责资源独占、数据校验和同步。

### 内核模块

内核模块（Kernel Module）是可装载到内核空间的 `.ko`。以外部模块为例，构建时使用与目标内核匹配的源码或构建目录、配置及交叉工具链。Makefile 可用 `obj-m += chenillard.o` 声明模块，随后执行 `make -C "$KDIR" M="$PWD" ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules`。如果启用了模块版本校验，还需要对应的符号版本信息。参见 [Linux 外部模块构建](https://docs.kernel.org/kbuild/modules.html)。

`module_init()` 注册初始化函数，`module_exit()` 注册卸载清理函数。早期字符设备示例可用 `register_chrdev()` 注册，使用 `/proc/devices` 查询主设备号。若系统没有自动创建设备节点，可用 `mknod /dev/chenillard c MAJOR MINOR`，将占位符替换为实际编号。现代小型设备也可用 `misc_register()` 管理节点。使用 `insmod` 加载、`lsmod` 查看、`rmmod` 卸载，借助 `dmesg` 查看 `printk()` 或 `dev_info()` 等输出。

模块使用内核 API，无法直接调用用户空间的 libc。初始化失败要回滚已经取得的资源，正常卸载要阻止新访问、使在途调用结束并释放资源。设备可解绑时，还要管理打开文件与设备对象的生命周期，不能仅依赖模块引用计数。参见 [Linux Driver Basics](https://docs.kernel.org/driver-api/basics.html)。

### 字符设备接口

`struct file_operations` 把文件操作分发到驱动函数。驱动可在 `.open` 中建立上下文并存入 `file->private_data`，在 `.read` 提供事件，在 `.write` 接收控制值，在 `.release` 清理此次打开的状态。`.release` 对应打开文件对象最后一次引用释放，多个描述符可以共享该对象。

用户缓冲区指针用 `__user` 标注。驱动用 `copy_from_user()` 读取、用 `copy_to_user()` 写回，并检查其返回的未复制字节数。复制可能睡眠，因此应在允许睡眠的上下文完成。读写回调用 `ssize_t` 返回字节数或负错误码。例如参数非法用 `-EINVAL`，设备已独占可返回 `-EBUSY`，访问用户缓冲区失败可返回 `-EFAULT`。文件位置用 `loff_t` 表示。寄存器命令或事件流通常设计成不可定位，应明确文件位置是否参与语义。

假设驱动要求 `write()` 一次提交 8 字节，其中前 4 字节是模式、后 4 字节是速度。驱动应先检查长度，再把整条记录拷进内核缓冲区，验证两个字段，最后写寄存器并返回 8。如果第二个字段无效，应在写任何寄存器之前返回错误，避免只改了一半状态。

### 内核访问硬件寄存器

驱动取得设备的物理地址与长度后，使用 `request_mem_region()` 声明占用，用 `ioremap()` 得到带 `__iomem` 标注的内核 I/O 地址。手工管理时，退出用 `iounmap()` 和 `release_mem_region()` 逆序释放。平台驱动常用 `devm_platform_ioremap_resource()` 合并资源获取与受管映射。可从 `/proc/iomem` 检查资源布局，权限与内核配置会影响可见信息。

根据寄存器要求选 `ioread8()`、`iowrite8()` 或 32 位版本。普通指针读写不能替代这些访问器。寄存器的访问宽度、字节序与顺序要按硬件约定。例如先写参数再写启动位，要确保设备按所需顺序观察写入。某些总线有延迟提交的写入，需要按平台文档从合适寄存器读回以确认写操作到达。参见 [Linux 设备 I/O](https://docs.kernel.org/driver-api/device-io.html)。

## 设备树与平台驱动

### 设备树基础

设备树（Device Tree）描述板上存在的设备、连接及资源。它的文本是 DTS（Device Tree Source），编译结果是 DTB。引导程序把 DTB 传给内核。驱动实现如何操作设备，设备树描述本板有哪些实例和资源。裸机的 `xparameters.h` 常在编译时提供硬件参数，而 Linux 平台驱动通常在绑定时从设备树或其他固件描述取得资源，因此同一驱动可用于基址不同的实例。

### 设备树属性

节点代表设备或总线，标签用于在 DTS 中引用节点。下面假定父总线已配置单个地址单元和单个大小单元，且地址无需额外转换。地址单元是一个 32 位数。示例不指定某块板的实际 IRQ。

```dts
chenillard0: led-controller@43c00000 {
    compatible = "example,chenillard-1.0";
    reg = <0x43c00000 0x1000>;
    status = "okay";
};
```

`chenillard0` 是标签，`compatible` 是匹配用的设备兼容标识，示例的 `reg` 指定从 `0x43C00000` 开始的 `0x1000` 字节窗口。一般情况下，`reg` 的单元个数由父总线的 `#address-cells` 和 `#size-cells` 决定，地址可能经由父总线 `ranges` 转换。`interrupts` 的格式由中断父节点的 binding 决定。使用 Arm GIC 时，常见描述由中断类型、相对编号和触发属性组成。必须从硬件连接和相应 binding 得到这些值。参见 [Devicetree Specification](https://devicetree-specification.readthedocs.io/en/stable/devicetree-basics.html)。

### 平台驱动

平台设备（Platform Device）携带板上设备的资源，平台驱动（Platform Driver）通过 `struct platform_driver` 注册。`struct of_device_id` 数组列出可匹配的设备类型，由 `.of_match_table` 指向。匹配成功后内核调用 `probe()`，每个实例应有自己的映射、中断和状态。

```c
static const struct of_device_id chenillard_match[] = {
    { .compatible = "example,chenillard-1.0" },
    { }
};
MODULE_DEVICE_TABLE(of, chenillard_match);

/* platform_driver 的 .driver.of_match_table 指向 chenillard_match。 */
```

在 `probe()` 中，可用 `platform_get_resource(pdev, IORESOURCE_MEM, 0)` 取得第一段 MMIO 资源，或直接用受管映射辅助函数。对于中断，用 `platform_get_irq(pdev, 0)` 取得 Linux IRQ 并处理负返回值。传统平台资源可包含 `IORESOURCE_IRQ`，设备树场景中应让平台与 IRQ 子系统完成中断描述的解析和映射。资源成功初始化后再注册用户接口。已有的 [平台总线设备模型和设备树](../../linux/linux驱动/04_平台总线设备模型和设备树.md) 给出了更多设备匹配示例。

## 硬件中断

### 中断架构

硬件中断请求（Interrupt Request, IRQ）是外设通知 CPU 有事件的机制。按键变化时，PL 中的 AXI GPIO 可记录通道事件并输出中断信号。Vivado 将该信号接到 PS 的 `IRQ_F2P` 路径。通用中断控制器（Generic Interrupt Controller, GIC）仲裁、屏蔽和分发 IRQ，CPU 再进入内核的异常处理代码。

```text
按键变化 → AXI GPIO 通道状态 → PL 到 PS 中断线
         → GIC → Cortex-A9 → Linux IRQ 子系统 → 驱动处理函数
```

GIC 的共享外设中断（Shared Peripheral Interrupt, SPI）可路由到相应 CPU 接口。这里的 SPI 与串行外设总线 SPI 含义不同。硬件线序号、GIC 硬件中断号、设备树编码和 Linux 分配的 IRQ 号要分别理解。驱动使用内核解析得到的 IRQ。边沿触发捕获信号跳变，电平触发则在信号持续有效时保持请求。AXI GPIO 可以对输入变化产生事件，同时向 GIC 输出有效高电平的中断信号。设备树描述应匹配后者。参见 [AMD UG585 System Interrupt](https://docs.amd.com/r/en-US/ug585-zynq-7000-SoC-TRM/System-Interrupt) 和 [AMD PG144 AXI GPIO Ports](https://docs.amd.com/r/en-US/pg144-axi-gpio/Port-Descriptions)。

### Linux 中断处理

`request_irq()` 把中断号关联到中断服务程序（Interrupt Service Routine, ISR）和设备上下文。平台驱动也可用 `devm_request_irq()` 管理资源。注册前先让外设的中断保持屏蔽并完成软件状态初始化。准备完毕后，根据 IP 的寄存器协议清理残留事件，再启用需要的通道及全局中断输出。

AXI GPIO 使能中断功能后，GIER 控制总输出，IPIER 控制通道，IPISR 记录通道事件。GIER 的全局使能位在 bit 31。IPISR 采用写 1 翻转（Toggle-On-Write），因此应确认哪些本设备状态位已经置位，只对准备确认的位写 1。例如读到 `0b11`，这次只确认通道一，就写 `0b01`。若把同样的写操作发给一个本来为 0 的位，它会变成 1。读取 GPIO 数据后按寄存器协议确认中断源，避免电平一直有效导致反复触发。硬件可合并近邻输入变化，单次通道状态不能代表无损的按键事件流。参见 [AMD PG144 Interrupts](https://docs.amd.com/r/en-US/pg144-axi-gpio/Interrupts)。

ISR 返回 `irqreturn_t`。确认并处理本设备事件后返回 `IRQ_HANDLED`，共享线上的事件若并非本设备产生则可返回 `IRQ_NONE`。硬中断上下文不能睡眠、获取睡眠型锁或直接执行用户缓冲区拷贝。较长的工作可交给线程化中断或工作队列。驱动负责确认外设事件，Linux IRQ 控制器层负责控制器侧的应答与分发。通过 `/proc/interrupts` 可查看各 CPU 上的 IRQ 计数。裸机也要确认外设和控制器，但需自行配置向量、中断控制器与异常环境。参见 [Linux Generic IRQ](https://docs.kernel.org/core-api/genericirq.html)。

### 中断驱动的等待机制

轮询（Polling）按周期读取状态。忙等待（Busy Waiting）持续检查并占用 CPU。如果每 10 ms 查一次，事件恰好在查询后发生时，单是等下一次查询就接近 10 ms。中断驱动的 I/O 允许无事件时把 CPU 交给别的工作，但仍要计入中断、唤醒与调度开销。

等待队列（Wait Queue）管理等待某个条件的线程。驱动先定义持久条件，例如「缓冲区非空，或设备已移除」。无数据时，阻塞的 `read()` 调用 `wait_event_interruptible()`。ISR 把数据写进队列后调用 `wake_up_interruptible()`。唤醒表示线程可以参与调度，线程真正运行时仍要重新检查条件。先记录数据再唤醒的顺序很重要，因为唤醒动作本身不保存事件。

```text
读线程                  驱动与 IRQ
检查队列 → 空 → 睡眠     按键变化 → 记录事件 → 唤醒读线程
重新获得 CPU → 重查队列 → 取记录 → 拷贝给应用
```

如果线程收到信号，等待函数可能返回负值，驱动应传递该结果。若文件以 `O_NONBLOCK` 打开且无数据，返回 `-EAGAIN`。用锁保护事件队列，取出事件后再在可睡眠上下文执行 `copy_to_user()`。设计还要定义缓冲区满时如何处理，以及设备卸载时如何唤醒等待者。参见 [Linux 等待队列接口](https://docs.kernel.org/driver-api/basics.html) 和站内的 [通用驱动框架](../../linux/linux驱动/02_通用的驱动框架.md)。

## 进程、线程与同步

### 进程与线程

进程提供地址空间与资源环境。线程是调度器安排执行的单位，同一进程的线程共享堆、全局变量和文件描述符表，但各自有寄存器状态、栈及线程局部数据。一个线程阻塞读按键时，另一个线程仍可读取键盘。

并发是多个活动在同一段时间内推进，单核可通过切换实现。并行是多个活动同时执行。Zynq-7000 的双核 Cortex-A9 在两个核均启用并由 Linux 管理时，可以同时运行两个可运行线程。共享数据的冲突既可能发生在双核，也可能发生在单核抢占时。

### POSIX 线程

可移植操作系统接口（Portable Operating System Interface, POSIX）规定了 Pthreads 等接口。引入 `pthread.h` 后，`pthread_create()` 启动签名为 `void *worker(void *arg)` 的函数，`pthread_join()` 等待一个可连接线程结束。线程参数所指对象要活到线程使用结束。阻塞在设备读取上的线程可能无法仅靠设置退出标志结束。应用可通过可唤醒的 I/O 等待设计，例如用 `poll()` 同时监听设备与退出事件，并在驱动中实现对应的可读通知。

用户程序编译和链接优先使用 `-pthread`，例如 `arm-linux-gnueabihf-gcc -Wall -pthread app.c -o app`。有些旧示例使用 `-lpthread`，它主要是链接选项。创建失败应检查 pthread 函数返回的错误码，并回收已经创建的线程。见 [Linux 基础笔记中的线程示例](../../linux/basic.md)。

### 并发同步

竞争条件（Race Condition）指结果依赖执行交错。例如速度初值为 10，两个线程分别执行一次加一。操作可拆为读、计算、写。若两个线程都读到 10，再分别写 11，最终得到 11，丢失一次更新。

互斥锁（Mutex）让同一时间只有一个线程进入临界区。应用用 `pthread_mutex_lock()` 与 `pthread_mutex_unlock()` 保护整段读改写。内核可睡眠上下文可用 `mutex_lock()` 与 `mutex_unlock()`，硬中断共享状态则需适用于中断上下文的同步方式。内核原子类型 `atomic_t` 可用于独立标志，`atomic_set()` 写标志，`atomic_xchg()` 原子地替换并返回旧值。比如用 `atomic_xchg(&opened, 1)` 争取独占打开，返回 1 表示已经被占用，成功路径结束后再释放状态。多个字段的一致更新仍需锁或明确的并发协议。

寄存器也有读改写冲突。两个线程分别改同一寄存器的不同位，若都先读旧值，再各自写全字，后一笔可能覆盖前一笔。驱动应统一管理这段事务，或在硬件支持时用独立置位、清位寄存器。带读或写副作用的寄存器还要遵守 IP 访问语义。

## 调度与实时系统

### 任务调度

调度器（CPU Scheduler）选择下一条要运行的线程。就绪（Ready）表示可运行但还没取得 CPU，运行（Running）表示正在占用 CPU，阻塞（Blocked）表示在等待数据、锁或其他事件。上下文切换要保存当前执行状态，再恢复另一个线程的状态。抢占调度允许内核按策略打断正在运行的任务。时间片是一些调度策略分配运行机会的方式，各策略规则不同。

单核同时只能执行一个线程。多核可以同时调度多个线程，还要考虑负载均衡、亲和性和共享资源竞争。按钮 IRQ 唤醒读线程后，它从阻塞转为可运行。什么时候获得 CPU 要看调度策略、其他任务和内核当时是否可以抢占。

### 实时系统

实时性关注能否在截止时间（Deadline）之前给出正确结果。硬实时要求在设计假设内满足时限，超时会使系统要求失效。软实时允许偶发超时，代价通常体现为性能或体验下降。平均延迟很低的系统也可能出现偶发长延迟，因此要关注可预测性与尾部数据。

最坏执行时间（Worst-Case Execution Time, WCET）关注任务自身最坏需要执行多久。最坏响应时间还要计入等待锁、被抢占和调度等影响。以按钮到输出更新为例，可将一次响应时间拆成中断到处理函数的延迟、驱动处理时间、唤醒到线程实际运行的延迟，以及应用和输出更新时间。假设一次测得四段分别为 8、12、60、20 µs，总计 100 µs。这是一次测量结果，不能据此推出所有情况下都满足 100 µs 截止时间。

### Linux 实时性

普通 Linux 的响应受中断、调度、锁、缺页和系统负载影响。启用 PREEMPT_RT 的配置可以让更多内核路径可抢占，并将许多中断处理交给可调度的线程。实时调度常见策略如下。

| 策略 | 基本行为 |
| --- | --- |
| `SCHED_FIFO` | 固定优先级。运行中的线程可因更高优先级任务抢占、主动让出或阻塞而停止运行。 |
| `SCHED_RR` | 固定优先级基础上，同优先级的可运行线程按时间片轮转。 |
| `SCHED_DEADLINE` | 按运行预算、相对截止时间和周期描述需求，内核进行准入及预算控制。 |

提高优先级需要相应权限，并要为日志、网络和其他线程保留运行机会。PREEMPT_RT 或实时策略可以改善可预测性。要证明硬实时，还需明确硬件、软件和负载条件，给出可信的时限分析。

需要进一步隔离时，可以采用非对称多处理（Asymmetric Multiprocessing, AMP），将 Linux 和 RTOS 分别部署到不同核。必须明确核的启动归属、共享内存、缓存一致性和中断分配。若目标是精确产生周期信号，可以由 PL 计数器直接生成边沿，通过 AXI Timer 中断让 Linux 观察事件。中断到 Linux 的到达时间和 PL 生成边沿的时刻是不同的测量量。参见 [PREEMPT_RT 理论](https://docs.kernel.org/core-api/real-time/theory.html) 和 [Linux Deadline 调度](https://docs.kernel.org/scheduler/sched-deadline.html)。

## Zynq Linux 综合实验

以下实验使用三个逻辑外设。跑马灯（Chenillard）按设定模式和速度移位，AXI GPIO 接收按键，AXI Timer 产生周期中断。先通过一次受控的寄存器读取确认硬件路径，再由驱动接管。最后让两个用户线程共同控制跑马灯，并测量中断和唤醒延迟。

### 系统构建与启动

| 步骤 | 产物与验证点 |
| --- | --- |
| Vivado 配置 PS、AXI 和 PL IP | 配置板级 DDR、串口、时钟和复位，给每个外设分配地址，连接 IRQ。生成位流并导出 XSA，保存地址和中断连接表。 |
| Vitis 生成 FSBL，构建 U-Boot | 用同一份硬件设计生成 Zynq-7000 FSBL。U-Boot 使用适配板卡的配置，确认 ELF 可作为 FSBL 后续分区。 |
| 构建 Linux、设备树与 Rootfs | 内核按板级配置交叉构建。DTB 的地址、大小和中断与 XSA/设计对应。Rootfs 内包含目标 ABI 的 BusyBox、库和应用。 |
| 打包与写入 | 依据前文 BIF 生成 `BOOT.BIN`，按已核对的 QSPI 分区写入并校验。内核与 DTB 可通过所选启动脚本从独立位置读取。 |
| 配置 NFS 和串口 | 主机导出目标 Rootfs，配置目标网卡和内核启动参数，用串口观察 FSBL、U-Boot、内核、NFS 挂载及 init。 |

最小的交接检查是：U-Boot 能看到可用 DDR，并能加载内核与 DTB。进入 Linux 后，`cat /proc/cmdline` 可核对根路径与控制台参数，`mount` 可验证 NFS 根是否挂载，`dmesg` 可查看驱动探测结果。若能看到 U-Boot 但内核没输出，先检查内核入口、装载地址之间是否冲突、DTB 与串口参数。若内核能启动但找不到 Rootfs，再检查网卡、NFS 和 init。

### FPGA 硬件控制

为跑马灯定义一个教学寄存器协议。窗口基址假设为 `0x43C00000`，模式位于偏移 `0x00`，速度位于 `0x04`。速度值在本例表示每次移位相隔的 PL 时钟周期数。若时钟为 50 MHz，即每秒 50,000,000 周期，写入 5,000,000 就得到约 0.1 s 的移位间隔。实际还要核对计数器宽度、重装语义和复位默认值。

先在无驱动占用的测试系统用前文 `/dev/mem` 与 `mmap()` 读回已知值。读值符合硬件协议后解除映射，再装载 `chenillard.ko`。驱动通过设备树取得窗口，注册 `/dev/chenillard`。设备约定每条写命令为两个 32 位字段，应用和驱动要使用一致的布局与字节序。下面是写回调的核心，省略设备结构、注册及解绑逻辑。这里约定在一条命令内先写速度再写模式，硬件若需要两者在同一拍生效，应提供影子寄存器和提交位。

```c
static ssize_t chen_write(struct file *file, const char __user *buf,
                          size_t len, loff_t *pos)
{
    struct chen_dev *dev = file->private_data;
    u32 command[2]; /* mode, ticks。教学接口约定使用同一目标机字节序 */

    if (len != sizeof(command))
        return -EINVAL;
    if (copy_from_user(command, buf, sizeof(command)))
        return -EFAULT;
    if (command[0] > 2 || command[1] == 0 || command[1] > dev->max_ticks)
        return -EINVAL;

    mutex_lock(&dev->lock);
    iowrite32(command[1], dev->base + 0x04);
    iowrite32(command[0], dev->base + 0x00);
    mutex_unlock(&dev->lock);
    return sizeof(command);
}
```

写入模式 1、速度 5,000,000 时，驱动先校验两个值，再按约定写两个寄存器。无效速度为 0 时直接返回错误，不应改变任何寄存器。用 LED 行为和允许读回的寄存器共同验证结果。

### 按钮中断驱动

`axi_btns.ko` 通过自定义兼容字符串匹配按键节点，创建 `/dev/btns`。同一个 AXI GPIO 实例应由一个驱动管理。如果原有 GPIO 驱动已经绑定，不应另一个驱动再占同一 MMIO 与 IRQ。教学驱动的步骤如下。

1. 在 `probe()` 获取并映射 MMIO，解析 IRQ，初始化事件队列及其锁。配置按键通道为输入，保持中断屏蔽。
2. 注册 ISR，清理待决状态，按通道和全局顺序启用中断输出。确认用户接口注册后可接受读取。
3. ISR 读取 IPISR。若本通道有事件，读取按钮位，按 IP 协议确认状态，向受保护的队列写入一个记录，然后唤醒读线程。
4. `read()` 等待队列非空。唤醒后在锁内取出一条记录，释放锁后复制到用户缓冲区。明确非阻塞、信号中断和队列满的返回或丢弃策略。
5. 解绑时停止接收新事件，屏蔽外设中断并等待在途 IRQ 结束，标记设备移除并唤醒读者。确保未结束的文件操作仍持有有效设备对象，再释放资源。

例如，以通道一、低电平表示按下为约定。若 IPISR 的通道一状态为 1，且读得 GPIO 数据的最低位为 0，ISR 确认该通道状态后，可把「按下」记录送入队列。读取端的关键逻辑如下。这里的 `ready` 由受保护的队列状态和移除标志计算，等待条件与取队列操作使用一致的同步规则。

```text
循环：
    持锁检查设备和队列
    已移除 → 解锁并返回设备不可用错误
    有记录 → 取一条记录、解锁、复制给用户并返回
    无记录 → 解锁
    若以非阻塞方式打开 → 返回 -EAGAIN
    wait_event_interruptible(queue, ready)
    若等待被信号打断 → 返回等待函数的负值
    回到循环，重新检查队列
```

调试时先确认空闲情况下 `read()` 阻塞。按下一次后观察 `/proc/interrupts` 计数和用户态接收到的值。如果中断计数持续增长而按键保持不变，优先检查清除时序和触发类型。如果一次按压产生多个变化，分别判断机械抖动、通道状态合并和软件队列溢出。需要稳定按键语义时，可以在硬件或可睡眠的延后处理路径实现消抖。

### 多线程应用

按钮线程从 `/dev/btns` 取事件，把事件转换成模式变化。键盘线程从控制台取输入，转换成速度变化。两者共享一份状态，例如 `mode=1, ticks=5000000`。如果按钮线程要改模式，键盘线程要改速度，可以让它们都走同一更新路径。

```text
等待输入（不持状态锁）
    ↓
持锁 → 从共享状态派生候选值 → 提交完整记录给 /dev/chenillard
    ↓
写入成功则保存新状态 → 解锁
```

例如按钮线程提交模式 2，随后键盘线程把速度改为 2,500,000。后者在锁内读到的模式应为 2，因此提交的是 `{2, 2500000}`。如果分别在锁外读取旧模式和旧速度，就可能用过期快照覆盖另一个线程的更新。这个方案假设控制写是短操作。若控制写可能长时间阻塞，可以让两个输入线程把事件交给单一输出线程，由它顺序更新并写设备。记录写入失败的原因，避免应用显示的状态与设备实际状态分离。

### 实时性测试

将 AXI Timer 配成固定周期中断，先确认计数器重装、触发类型和中断确认符合所用 IP。可把事件逐级打时间戳，分别测量硬件事件到 ISR、ISR 到读线程，以及读线程到 LED 更新。第一个量需要能观测硬件事件时刻，例如用 PL 捕获或逻辑分析仪同步观察硬件输出与测量标记。只有用户态读到两条事件的时间戳时，能测的是到达间隔及抖动。

例如周期设为 1 ms，五个连续事件的应用接收时间为 0、995、2006、3001、4019 µs。相邻间隔是 995、1011、995、1018 µs，与标称周期相减得到 −5、+11、−5、+18 µs。这批样本最大绝对间隔偏差是 18 µs。即使每次事件都恒定延后 100 µs，到达间隔也可能保持 1 ms，因此不能从间隔偏差推断绝对延迟。

旧实验可用 `gettimeofday()` 取得壁钟时间，但系统校时会影响时间差。Linux 用户态测时长时可优先使用 `clock_gettime(CLOCK_MONOTONIC, ...)`。内核与用户态时间戳用于直接相减前，需要确认时钟域和采样点一致。记录空闲、CPU 负载和 I/O 负载下的样本数、丢失事件数、中位数、P99 和最大值。P99 表示约 99% 样本不超过该值。把轮询、阻塞中断和所用调度配置放在相同条件下比较，并保留板卡、内核版本、计时方式与负载配置，方便复现。

最后沿着两条路径检查系统是否闭环。控制方向从输入线程、字符设备、寄存器到 LED 输出。事件方向从定时器或按键、PL 到 PS 中断、驱动缓冲区，再回到用户线程。两条路径都能逐段观测后，才能判断一个异常来自启动、地址配置、驱动状态还是调度延迟。
