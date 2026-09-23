# Zynq XC7Z020 硬件乘法加速器：从零理解 PS、PL 与 AXI

这篇笔记完整梳理一个最小但真实的 Zynq 硬件加速项目：ARM Cortex-A9 通过 AXI4-Lite 把两个操作数写入 PL 中的自定义 `mult_accel` IP，由 FPGA 里的 VHDL 乘法电路完成计算，再由 ARM 读回结果。

项目中间的软件安装、界面操作和排错过程不在本文范围内。本文只保留需要长期掌握的架构、硬件、软件和数据流知识。

## 1. 项目目标、结果与核心思想

最终实现的数据流是：

```text
ARM Cortex-A9
    │
    │ 写 A = 10
    │ 写 B = 20
    ▼
AXI4-Lite
    ▼
┌─────────────────────┐
│      PL / FPGA      │
│                     │
│      A × B          │
│       ↓             │
│      200            │
└─────────┬───────────┘
          │
          │ AXI 读取
          ▼
ARM 得到 result = 200
```

调试器中读到：

```text
result = 0x000000C8
```

因为：

```text
0xC8 = 200
10 × 20 = 200
```

这证明了完整链路已经工作：ARM 写入输入，PL 完成乘法，ARM 再读回结果。这里最关键的不是数值 200，而是职责分工：

```text
PS 负责控制和数据传输
PL 负责计算
AXI 负责 PS 与 PL 之间的通信
```

乘法不是 ARM 软件算完再写给 FPGA 的，而是由 PL 中综合出的真实硬件电路完成。ARM 程序只负责“写输入、读结果”。这就是最基础的 Zynq 硬件加速模型。

## 2. Zynq 架构与 FPGA 加速的本质

### 2.1 Zynq 与普通 FPGA 的区别

本项目使用 Zynq-7000 系列的 `XC7Z020`。Zynq 可以理解为在同一颗芯片中集成了处理器系统和可编程逻辑：

```text
┌────────────────────────────┐
│           Zynq             │
│                            │
│  PS                    PL  │
│ ┌─────────┐        ┌─────┐ │
│ │ ARM CPU │ ←AXI→  │FPGA │ │
│ └─────────┘        └─────┘ │
│                            │
└────────────────────────────┘
```

`PS` 是 Processing System，内部包括：

- ARM Cortex-A9 处理器；
- DDR Controller；
- UART、SPI、I2C、GPIO；
- Timer 等常用处理器外设。

`PL` 是 Programmable Logic，也就是 FPGA 部分，可以使用 VHDL、Verilog 或 HLS 描述和构造数字电路。

因此，Zynq 的核心特点是：

```text
CPU + FPGA
```

软件控制和专用硬件计算可以在同一颗芯片内配合工作。

### 2.2 CPU 执行指令，FPGA 构造电路

如果完全由 CPU 计算：

```c
result = a * b;
```

CPU 会按照取指、译码、执行和保存结果的过程运行指令。FPGA 的思路不同：我们直接在 PL 中构造乘法电路。

```text
A ───────┐
         × ────── Result
B ───────┘
```

VHDL 中的核心描述是：

```vhdl
mult_result <= unsigned(slv_reg0) * unsigned(slv_reg1);
```

综合后，这不是“保存在 FPGA 里等待执行的一条软件指令”，而是变成 FPGA 内部的硬件逻辑。可以先记住：

```text
CPU：执行程序
FPGA：构造电路
```

只实现一个乘法器时，性能优势并不明显。FPGA 的价值会在多个乘法器并行工作、MAC 阵列、流水线、矩阵乘法、卷积或 FIR 等场景中体现出来。

## 3. PS 与 PL 的通信：AXI、SmartConnect 与 MMIO

### 3.1 AXI4-Lite 与主从关系

本项目用 `AXI4-Lite` 连接 ARM 和自定义加速器。可以先把 AXI 理解为 ARM 与 FPGA 外设之间的一套标准通信协议。

```text
Cortex-A9
    │
    ▼
M_AXI_GP0
    │
    ▼
AXI SmartConnect
    │
    ├──── AXI GPIO
    │
    └──── mult_accel
```

这里需要理解 Master 和 Slave：

- PS 侧的 `M_AXI_GP0` 中，`M` 表示 Master，由 ARM 主动发起读写操作；
- `mult_accel` 的接口是 `S00_AXI`，`S` 表示 Slave，它等待 ARM 访问。

也就是：

```text
ARM         = AXI Master
mult_accel  = AXI Slave
```

### 3.2 AXI SmartConnect 的作用

ARM 的一个 `M_AXI_GP0` 可能要连接多个 PL 外设，例如 AXI GPIO、自定义加速器、Timer、DMA 或 UART Lite。`AXI SmartConnect` 可以理解成 AXI 交换机或路由器：

```text
                    ┌── AXI GPIO
ARM → SmartConnect ─┤
                    └── mult_accel
```

它根据 ARM 发出的地址，把访问请求转发给对应的外设。

### 3.3 Memory-Mapped I/O

CPU 不会发出“访问 FPGA 乘法器”这样的抽象命令，它只会访问地址。本项目为 `mult_accel` 分配的基地址是：

```text
Base Address = 0x43C00000
```

因此，CPU 执行：

```c
Xil_Out32(0x43C00000, 10);
```

实际数据流是：

```text
ARM
↓
写地址 0x43C00000，写数据 10
↓
AXI
↓
mult_accel
```

这种“外设像内存一样通过地址访问”的机制称为 Memory-Mapped I/O，简称 MMIO。

### 3.4 为什么先用 AXI GPIO 验证通信

在实现乘法器之前，先做了最简单的 AXI GPIO 实验：

```text
ARM → AXI → AXI GPIO
```

AXI GPIO 的基地址是 `0x41200000`。ARM 写入再读回：

```c
Xil_Out32(0x41200000, 0x12345678);
value = Xil_In32(0x41200000);
```

最终读到 `0x12345678`，说明下面这条往返通信路径正常：

```text
ARM → AXI → PL → ARM
```

这个实验本身不是硬件加速，而是把通信与计算拆开，先验证 AXI 基础链路。因此 AXI GPIO 是很合适的 AXI 入门实验。

## 4. 自定义 `mult_accel` IP 与寄存器映射

### 4.1 AXI4-Lite Slave Peripheral

`mult_accel` 是一个自定义的 AXI4-Lite Slave Peripheral。Vivado 模板为它生成了 4 个 32 位寄存器：

```vhdl
slv_reg0
slv_reg1
slv_reg2
slv_reg3
```

因为一个寄存器是 32 bit，也就是 4 Byte，所以相邻寄存器的地址偏移每次增加 4：

| 偏移 | 实际地址 | 功能 | 访问属性 |
| --- | --- | --- | --- |
| `0x00` | `0x43C00000` | A | CPU 写入 |
| `0x04` | `0x43C00004` | B | CPU 写入 |
| `0x08` | `0x43C00008` | Result | CPU 读取 |
| `0x0C` | `0x43C0000C` | Reserved | 备用 |

地址访问遵循统一形式：

```text
实际寄存器地址 = IP Base Address + Register Offset
```

### 4.2 IP 内部如何进行地址译码

AXI 地址进入 IP 后，需要判断当前访问的是哪个寄存器。模板中有：

```vhdl
constant ADDR_LSB : integer :=
    (C_S_AXI_DATA_WIDTH/32) + 1;
```

数据宽度为 32 bit 时，`ADDR_LSB = 2`。这是因为 AXI 地址按 Byte 编址，一个 32 位寄存器占 4 Byte，地址最低两位用于选择一个 32 位字中的字节，不用于选择寄存器。

```text
Offset 0x00 = 0000
Offset 0x04 = 0100
Offset 0x08 = 1000
Offset 0x0C = 1100
```

去掉最低两个 byte-address bit 后：

```text
00 → Reg0
01 → Reg1
10 → Reg2 / Result
11 → Reg3
```

这就是 VHDL 地址译码中以下分支的来源：

```vhdl
when b"00" =>
when b"01" =>
when b"10" =>
when b"11" =>
```

### 4.3 CPU 写 A 和 B

ARM 执行：

```c
Xil_Out32(MULT_BASE_ADDR + 0x00, 10);
```

IP 看到偏移 `0x00`，译码成 `"00"`，将 AXI 写数据 `S_AXI_WDATA` 写入 `slv_reg0`：

```text
slv_reg0 = 10
```

然后执行：

```c
Xil_Out32(MULT_BASE_ADDR + 0x04, 20);
```

偏移 `0x04` 被译码成 `"01"`，数据写入 `slv_reg1`：

```text
slv_reg1 = 20
```

至此，FPGA 内部保存了两个输入操作数。

## 5. PL 中的乘法、读写属性与结果位宽

### 5.1 真正的硬件乘法

32 位无符号数相乘，完整结果最多需要 64 位，因此定义：

```vhdl
signal mult_result :
    unsigned(2*C_S_AXI_DATA_WIDTH-1 downto 0);
```

当 `C_S_AXI_DATA_WIDTH = 32` 时，`mult_result` 宽度就是 64 bit。计算逻辑为：

```vhdl
mult_result <=
    unsigned(slv_reg0) * unsigned(slv_reg1);
```

对应的硬件关系是：

```text
             PL

slv_reg0
   10
    │
    ├────┐
         │
         × ─────── mult_result = 200
         │
    ├────┘
    │
   20
slv_reg1
```

这段逻辑才是整个项目真正的 Accelerator。

### 5.2 为什么第一版没有 START、DONE 和 BUSY

第一版乘法器使用组合逻辑：只要 A 或 B 变化，乘法结果就自动变化。

```text
A 或 B 变化
↓
组合乘法电路重新传播
↓
Result 自动变化
```

因此这一版不需要 `START`、`DONE` 或 `BUSY` 控制信号。CPU 写入 A 和 B 后，PL 就会形成结果。

矩阵乘法、向量点积、FIR 或 CNN 等需要多个周期的复杂加速器，通常才需要控制寄存器或握手信号，用于表达启动、忙碌和完成状态。

### 5.3 Result 为什么不能由 CPU 写

原始 AXI 模板允许 CPU 写 4 个寄存器，但 `Reg2` 已被定义为硬件计算结果，不应再允许软件覆盖。因此写地址译码的 `"10"` 分支改成：

```vhdl
when b"10" =>
    null;
```

最终访问属性是：

```text
Reg0 → CPU 写，作为输入 A
Reg1 → CPU 写，作为输入 B
Reg2 → CPU 不能写，读取时返回硬件结果
Reg3 → 备用
```

### 5.4 CPU 如何读 Result，以及为什么只返回低 32 位

读取偏移 `0x08` 时，返回：

```vhdl
std_logic_vector(
    mult_result(C_S_AXI_DATA_WIDTH-1 downto 0)
)
```

也就是 `mult_result[31:0]`。这是因为 AXI 数据宽度当前为 32 bit，而 `32 bit × 32 bit` 的完整结果可能是 64 bit。例如 `0xFFFFFFFF × 0xFFFFFFFF` 明显不能装进 32 位。

本实验的 `10 × 20 = 200` 不会溢出，读取低 32 位完全足够。如果以后需要完整的 64 位结果，可以重新安排寄存器：

```text
0x08 → Result Low
0x0C → Result High
```

软件再组合为：

```text
Result64 = (ResultHigh << 32) | ResultLow
```

## 6. Vivado Block Design、时钟与复位

最终的主要硬件结构是：

```text
                 ┌───────────────┐
                 │ Cortex-A9 PS  │
                 │               │
                 │ M_AXI_GP0     │
                 └───────┬───────┘
                         │
                         ▼
                 ┌───────────────┐
                 │ SmartConnect  │
                 └───┬───────┬───┘
                     │       │
                     ▼       ▼
                 AXI GPIO  mult_accel
                              │
                              ▼
                            A × B
```

其中 AXI GPIO 是前一步通信测试留下的外设，真正的计算加速器是 `mult_accel`。

### 6.1 Clock

PL 的 AXI 部分工作在：

```text
FCLK_CLK0 = 50 MHz
```

对应时钟周期：

```text
T = 1 / 50 MHz = 20 ns
```

这个时钟由 PS 产生，再通过 `processing_system7_0/FCLK_CLK0` 送到 PL。SmartConnect、AXI GPIO 和 `mult_accel` 都使用这一路时钟。

### 6.2 Reset

PS 输出 `FCLK_RESET0_N`，名称末尾的 `_N` 表示低电平有效。设计中再通过 Processor System Reset 模块产生适合 AXI 外设与互连使用的复位信号：

```text
PS Reset
   ↓
Processor System Reset
   ├── interconnect_aresetn → SmartConnect
   └── peripheral_aresetn   → mult_accel 等外设
```

时钟决定所有同步逻辑何时工作，复位负责让互连和外设从确定状态开始运行。二者都属于完整 AXI 系统的一部分，不只是“辅助连线”。

## 7. Vivado、Vitis、XSA、Platform 与产物

### 7.1 Vivado 和 Vitis 的职责

这个项目可以清楚分成硬件和软件两部分。

Vivado 负责硬件，包括：

- Zynq PS 配置；
- AXI SmartConnect 和 AXI GPIO；
- 自定义 `mult_accel` IP；
- AXI 地址分配；
- Clock 和 Reset；
- 综合、实现与 Bitstream。

Vitis 负责 ARM 软件，包括：

- `main.c`；
- BSP 和驱动；
- Standalone 域；
- ARM 程序编译与 ELF；
- JTAG 下载和调试。

可以概括为：

```text
Vivado → 搭硬件
Vitis  → 写 ARM 程序
```

### 7.2 XSA 是什么

Vivado 完成硬件设计后导出 XSA。XSA 可以理解为 Vivado 用来告诉 Vitis“硬件长什么样”的文件，其中包含：

- Processor 和 Memory 信息；
- AXI 外设及其地址；
- Bitstream；
- 其他硬件元数据。

只要硬件发生变化，例如新增 `mult_accel` 或修改地址，就需要更新整条链路：

```text
Generate Bitstream
↓
Export XSA
↓
Vitis 重新读取 XSA
```

否则软件平台仍然描述旧硬件。

### 7.3 Platform、Domain 与 Standalone

Vitis 读取 XSA 后建立 `zynq_hw_platform`。Platform 连接硬件设计和软件开发，大致包含：

```text
XSA
 ↓
Platform
 ├── CPU 信息
 ├── BSP
 ├── 驱动
 ├── Memory Map
 └── Hardware Information
```

本项目的 Domain 是：

```text
standalone_ps7_cortexa9_0
```

它表示：

```text
OS        = standalone
Processor = ps7_cortexa9_0
```

也就是程序以裸机方式运行在 Cortex-A9 Core 0 上。Standalone 没有 Linux 或复杂操作系统，程序基本沿着 `main() → BSP → 硬件` 的路径运行，特别适合学习 GPIO、AXI、中断、DMA、Timer 和硬件加速。

### 7.4 `.bit`、`.elf` 和 XSA 的区别

这三个产物不要混淆：

| 产物 | 作用 |
| --- | --- |
| `.bit` | 配置 PL，决定 FPGA 中有哪些硬件电路 |
| `.elf` | ARM 可执行程序，包含写 A、写 B、读结果等软件指令 |
| XSA | 把 Vivado 硬件信息交给 Vitis，连接硬件与软件开发 |

运行时需要先让 PL 中存在正确的加速器硬件，再让 ARM 运行与这份硬件地址映射匹配的软件。

## 8. ARM 裸机程序与完整读写链路

### 8.1 测试程序

最终测试程序的核心是：

```c
#include "xil_io.h"

#define MULT_BASE_ADDR 0x43C00000U

int main(void)
{
    u32 result;

    Xil_Out32(MULT_BASE_ADDR + 0x00, 10);
    Xil_Out32(MULT_BASE_ADDR + 0x04, 20);

    result = Xil_In32(MULT_BASE_ADDR + 0x08);

    while (1)
    {
    }

    return 0;
}
```

程序只做三件事：

1. 向 `Base + 0x00` 写入 A；
2. 向 `Base + 0x04` 写入 B；
3. 从 `Base + 0x08` 读取 Result。

### 8.2 `Xil_Out32` 和 `Xil_In32`

可以把它们简单理解为 32 位 MMIO 访问函数：

```c
Xil_Out32(address, value); // 向指定硬件地址写 32 位数据
Xil_In32(address);         // 从指定硬件地址读 32 位数据
```

它们不是普通 C 变量赋值，而是通过处理器的地址空间发起真实总线事务。BSP 提供这些接口，让软件不必手写底层的易失指针访问细节。

### 8.3 三步操作的内部过程

第一步：

```c
Xil_Out32(MULT_BASE_ADDR + 0x00, 10);
```

等价于向 `0x43C00000` 写 10，最终得到：

```text
slv_reg0 = 10
```

第二步：

```c
Xil_Out32(MULT_BASE_ADDR + 0x04, 20);
```

最终得到：

```text
slv_reg1 = 20
```

此时 PL 组合乘法电路自动形成：

```text
mult_result = 10 × 20 = 200
```

第三步：

```c
result = Xil_In32(MULT_BASE_ADDR + 0x08);
```

ARM 发起地址 `0x43C00008` 的读取。IP 将偏移 `0x08` 译码为 `"10"`，选择 `mult_result` 的低 32 位，并经 AXI 返回 200。

完整链路可以展开为：

```text
ARM 执行 Xil_Out32
↓
PS 的 M_AXI_GP0 发起 AXI 写事务
↓
SmartConnect 根据地址选择 mult_accel
↓
IP 地址译码，把数据写入 slv_reg0 / slv_reg1
↓
PL 组合乘法电路产生 mult_result
↓
ARM 执行 Xil_In32
↓
M_AXI_GP0 发起 AXI 读事务
↓
IP 返回 mult_result[31:0]
↓
ARM 变量 result 得到 200
```

## 9. 构建、下载、验证与当前内存限制

### 9.1 从硬件到软件的完整流程

不考虑具体界面操作，项目的逻辑流程是：

```text
设计并修改 mult_accel 的 VHDL
↓
综合自定义 IP 并重新打包
↓
把 mult_accel 加入 Vivado Block Design
↓
连接 AXI、Clock、Reset，并分配地址
↓
Validate Design
↓
Synthesis → Implementation → Generate Bitstream
↓
导出包含 Bitstream 的 XSA
↓
Vitis Platform 读取新 XSA 并重新构建
↓
编译 ARM 应用，得到 ELF
↓
通过 JTAG 配置 PL 并运行 ARM 程序
↓
在调试器中检查 result
```

硬件设计改变后，必须重新生成 Bitstream 和 XSA，并让 Vitis Platform 使用新 XSA；只重新编译 `main.c` 并不能把新硬件放进 PL。

### 9.2 如何验证成功

程序在 `while (1)` 处停住后，通过调试器查看 `result`：

```text
result = 0x000000C8
```

这同时验证了：

- ARM 软件正常运行；
- 基地址和偏移正确；
- PS 到 PL 的 AXI 写通路正常；
- `slv_reg0` 和 `slv_reg1` 正确接收数据；
- PL 中的乘法逻辑正常；
- AXI 读通路能把 Result 返回给 ARM。

因此这不是只验证一行乘法表达式，而是验证了一条完整的软硬件协同链路。

### 9.3 当前使用 OCM，以及 DDR 遗留问题

当前程序链接并运行在 OCM，即 `ps7_ram_0_memory_0`，而不是 DDR。原因是板子的 DDR/PS preset 还没有完全配置正确。

这不影响当前小规模 AXI 寄存器实验，因为程序和数据量都很小；但后续做大数组、DMA、向量或矩阵计算时，OCM 容量不够，必须修正 DDR 配置。

因此要区分两件事：

```text
当前乘法加速器与 AXI 链路：已经验证成功
DDR 配置：仍是后续进入大数据搬运前必须解决的问题
```

## 10. 性能认识、升级方向与知识索引

### 10.1 为什么它还只是“最小硬件加速器”

虽然这已经是真正的硬件加速结构，但性能上仍很初级。一次乘法需要：

```text
ARM 写 A
↓
一次 AXI transaction

ARM 写 B
↓
一次 AXI transaction

FPGA 乘法
↓

ARM 读 Result
↓
一次 AXI transaction
```

为了一个 `A × B` 产生三次 AXI 访问，而 ARM 自己执行 `a * b` 可能反而更快。因此必须记住：

> 把一个操作放进 FPGA，并不自动意味着获得加速。

硬件计算时间只是总时间的一部分。是否加速还取决于启动、数据搬运、总线事务和结果回传的开销，以及一次传输能让 FPGA 连续完成多少有效计算。

### 10.2 真正有意义的加速形式

下一步可以实现向量点积。例如：

```text
A = [1, 2, 3, 4]
B = [5, 6, 7, 8]
```

计算：

```text
1×5 + 2×6 + 3×7 + 4×8
```

CPU 通常使用循环：

```c
sum = 0;

for (...)
    sum += A[i] * B[i];
```

FPGA 可以构造多个并行乘法器和加法树：

```text
A0 × B0 ─┐
A1 × B1 ─┤
A2 × B2 ─┼── 加法树 → Result
A3 × B3 ─┘
```

再进一步加入 Pipeline、DSP48、AXI Stream、DMA 和 DDR，才能持续处理大量数据，逐渐进入真正意义上的 FPGA 加速设计。

### 10.3 推荐学习路线

```text
Level 0  PS / PL / AXI 基础                         ✅
   ↓
Level 1  ARM ↔ AXI GPIO                            ✅
   ↓
Level 2  ARM ↔ 自定义乘法 Accelerator              ✅
   ↓
Level 3  MAC 加速器：sum = sum + A × B
   ↓
Level 4  向量点积：Σ A[i] × B[i]
   ↓
Level 5  Pipeline + DSP
   ↓
Level 6  AXI Stream
   ↓
Level 7  DMA：DDR → DMA → FPGA → DMA → DDR
   ↓
Level 8  比较 CPU 计算时间、FPGA 计算时间和数据传输开销
```

到 DMA 加向量或矩阵计算阶段，就会真正理解硬件加速不是简单地“把 C 代码换成 VHDL”，而是重新设计数据流，让 FPGA 利用并行、流水线和专用硬件资源持续处理大量数据。

### 10.4 核心概念速查

| 概念 | 含义 |
| --- | --- |
| PS | ARM 处理器系统，负责控制与软件执行 |
| PL | FPGA 可编程逻辑，负责构造计算电路 |
| AXI4-Lite | 适合控制寄存器读写的内存映射总线 |
| AXI Master | 主动发起事务的一方，本项目中是 ARM |
| AXI Slave | 等待被访问的一方，本项目中是 `mult_accel` |
| AXI SmartConnect | AXI 路由与互连，根据地址选择外设 |
| MMIO | 用处理器地址空间访问硬件寄存器 |
| Base + Offset | 定位某个 IP 内部的具体寄存器 |
| `Xil_Out32` | ARM 通过 AXI 写 32 位寄存器 |
| `Xil_In32` | ARM 通过 AXI 读 32 位寄存器 |
| `slv_reg0` | 输入 A |
| `slv_reg1` | 输入 B |
| `mult_result` | PL 中乘法电路产生的 64 位结果 |
| `0x43C00008` | Result 低 32 位的读取地址 |
| `.bit` | 用来配置 FPGA 硬件 |
| `.elf` | ARM 运行的软件 |
| XSA | Vivado 向 Vitis 描述硬件的平台文件 |
| OCM | 当前程序使用的片上存储器 |
| DDR | 后续大数组和 DMA 场景需要使用的外部存储器 |

这个小项目最终建立的不是“会写一个乘法器”这一项孤立技能，而是一条完整认知链：

```text
ARM 软件
↓ MMIO
AXI 地址与事务
↓
自定义寄存器接口
↓
PL 硬件计算
↓
AXI 返回结果
```

这条链路是继续学习 MAC、向量点积、AXI Stream、DMA 和更大规模 FPGA 加速器的基础。
