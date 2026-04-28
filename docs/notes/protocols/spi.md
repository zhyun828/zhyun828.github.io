# SPI

SPI 是 Serial Peripheral Interface 的缩写，是 Motorola 开发的一种同步串行总线。它常用于 MCU 与 Flash、OLED、ADC、DAC、传感器、无线模块等板级外设通信。

## 1. 协议定位

| 协议 | 引脚 | 双工 | 时钟 | 电平 | 设备关系 |
| --- | --- | --- | --- | --- | --- |
| USART | `TX`、`RX` | 全双工 | 异步 | 单端 | 点对点 |
| I2C | `SCL`、`SDA` | 半双工 | 同步 | 单端 | 多设备 |
| SPI | `SCK`、`MOSI`、`MISO`、`SS` | 全双工 | 同步 | 单端 | 多设备 |
| CAN | `CAN_H`、`CAN_L` | 半双工 | 异步 | 差分 | 多设备 |

SPI 的特点：

- 四根通信线，典型全双工。
- 同步通信，时钟由主机输出。
- 支持一主多从。
- 没有统一地址字段，从设备由 `SS/CS` 片选线选择。
- 协议本身没有 ACK，读写是否成功要靠器件状态寄存器或上层逻辑判断。

## 2. 基本信号

| 信号 | 含义 | 方向 |
| --- | --- | --- |
| `SCK` / `SCLK` | 串行时钟 | 主机输出 |
| `MOSI` | Master Output Slave Input | 主机输出，从机输入 |
| `MISO` | Master Input Slave Output | 从机输出，主机输入 |
| `SS` / `CS` / `NSS` | 片选 | 主机输出，通常低有效 |

<figure markdown="span">
  <img src="../assets/spi/spi-single.svg" alt="SPI single master single slave connection" />
  <figcaption>图 1：SPI 单主单从连接。根据 STM32 入门课件硬件连接重绘。</figcaption>
</figure>

课件中的硬件连接要点：

- 所有 SPI 设备的 `SCK`、`MOSI`、`MISO` 分别连在一起。
- 主机另外引出多条 `SS` 控制线，分别接到各从机的 `SS` 引脚。
- 输出引脚配置为推挽输出。
- 输入引脚配置为浮空输入或上拉输入。

## 3. 起始和终止

SPI 的起始和终止通常由片选线控制：

- 起始条件：`SS` 从高电平切换到低电平。
- 终止条件：`SS` 从低电平切换到高电平。

一次完整传输中，`SS` 应覆盖命令、地址、数据等完整阶段。很多 SPI 器件会在 `SS` 拉高时结束内部状态机，如果中途抬高 `SS`，后续字节可能被当成新命令。

## 4. 移位交换

SPI 的核心是交换一个字节：

1. 主机拉低目标从机 `SS`。
2. 主机产生 `SCK`。
3. 主机在 `MOSI` 上逐 bit 输出。
4. 从机在 `MISO` 上逐 bit 输出。
5. 每个时钟边沿完成移入或移出。
6. 8 个时钟后，双方完成一个字节交换。

即使主机只是读从机，也必须继续输出时钟。很多时候主机会发送 dummy byte，例如 `0xFF` 或 `0x00`，只是为了产生时钟。

## 5. CPOL 和 CPHA

SPI 有四种模式，取决于 `CPOL` 和 `CPHA`：

- `CPOL`：Clock Polarity，时钟空闲电平。
- `CPHA`：Clock Phase，第一个边沿采样还是第二个边沿采样。

<figure markdown="span">
  <img src="../assets/spi/spi-modes.svg" alt="SPI four modes" />
  <figcaption>图 2：SPI 四种时钟模式。根据 STM32 入门课件 Mode 0 到 Mode 3 时序重绘。</figcaption>
</figure>

| Mode | CPOL | CPHA | 空闲 SCK | 数据行为 |
| ---: | ---: | ---: | --- | --- |
| 0 | 0 | 0 | 低电平 | 第一个边沿移入，第二个边沿移出 |
| 1 | 0 | 1 | 低电平 | 第一个边沿移出，第二个边沿移入 |
| 2 | 1 | 0 | 高电平 | 第一个边沿移入，第二个边沿移出 |
| 3 | 1 | 1 | 高电平 | 第一个边沿移出，第二个边沿移入 |

如果模式配错，常见现象是读到的数据错位、固定为 `0xFF/0x00`，或者逻辑分析仪换一种模式才能解出正确数据。

图 2 中红色虚线是采样边沿，灰色虚线是移位边沿。调试时如果看到波形但数据错误，优先核对从设备手册要求的 SPI Mode。

## 6. 多从机连接

<figure markdown="span">
  <img src="../assets/spi/spi-multi.svg" alt="SPI multiple slaves" />
  <figcaption>图 3：SPI 多从设备连接方式。根据 STM32 入门课件硬件连接重绘。</figcaption>
</figure>

注意：

- `SCK/MOSI/MISO` 可以共用。
- 每个从机需要独立 `SS/CS`。
- 任意时刻一般只允许一个从机被选中。
- 未选中的从机应释放 `MISO`，否则会总线冲突。
- 从设备越多，主机占用 GPIO 越多。

## 7. SPI Flash 典型时序

课件使用 W25Q64 讲 SPI Flash。常见命令包括：

- 写使能：`0x06`
- 指定地址写：`0x02 + Address[23:0] + Data`
- 指定地址读：`0x03 + Address[23:0] + Data`

<figure markdown="span">
  <img src="../assets/spi/spi-flash-sequences.svg" alt="SPI flash command address read and write sequences" />
  <figcaption>图 4：SPI Flash 常见命令序列。根据 STM32 入门课件重绘。</figcaption>
</figure>

### 发送指令

```text
CS low
MOSI: 0x06
CS high
```

用于 W25Qxx 的写使能。没有地址和数据阶段。

### 指定地址写

```text
CS low
MOSI: 0x02
MOSI: Address[23:0]
MOSI: Data...
CS high
```

Flash 写入通常还需要先擦除，且页写不能跨页随意写。

### 指定地址读

```text
CS low
MOSI: 0x03
MOSI: Address[23:0]
MISO: Data...
CS high
```

读阶段主机仍要持续产生时钟；主机可能在 `MOSI` 上发送 dummy byte。

## 8. W25Q64 课件要点

W25Qxx 系列是低成本、小型化、使用简单的非易失性存储器。

用途：

- 数据存储。
- 字库存储。
- 固件程序存储。

课件列出的参数：

- 存储介质：Nor Flash。
- W25Q64 容量：`64 Mbit / 8 MByte`。
- 地址宽度：24 位。
- 普通 SPI 时钟频率可到 `80 MHz`。
- Dual SPI 可到 `160 MHz`。
- Quad SPI 可到 `320 MHz`。

W25Q64 常见引脚：

| 引脚 | 功能 |
| --- | --- |
| `VCC/GND` | 电源，常见 2.7 到 3.6 V |
| `CS` | SPI 片选 |
| `CLK` | SPI 时钟 |
| `DI` | MOSI，主机输出从机输入 |
| `DO` | MISO，从机输出主机输入 |
| `WP` | 写保护 |
| `HOLD` | 数据保持 |

## 9. STM32 SPI 外设

课件对 STM32 硬件 SPI 的描述：

- 硬件自动执行时钟生成和数据收发。
- 可配置 8 位或 16 位数据帧。
- 可配置高位先行或低位先行。
- 时钟频率可由 `fPCLK / (2, 4, 8, 16, 32, 64, 128, 256)` 分频得到。
- 支持主模式或从模式。
- 可精简为半双工或单工通信。
- 支持 DMA。
- 兼容 I2S 协议。
- STM32F103C8T6 硬件 SPI 资源：`SPI1`、`SPI2`。

基本结构：

```text
PCLK -> 波特率发生器 -> SCK
发送数据寄存器 TDR -> 移位寄存器 -> MOSI
MISO -> 移位寄存器 -> 接收数据寄存器 RDR
GPIO 开关控制 -> SCK/MOSI/MISO/SS
```

## 10. 常见状态标志

| 标志 | 含义 | 调试用途 |
| --- | --- | --- |
| `TXE` | 发送缓冲区空 | 可以写入下一个数据 |
| `RXNE` | 接收缓冲区非空 | 可以读取收到的数据 |
| `BSY` | SPI 忙 | 判断最后一个 bit 是否真正发完 |
| `OVR` | 溢出 | 接收数据未及时读取 |

SPI 是全双工移位：发送一个字节的同时通常也会收到一个字节。因此只写不读也可能造成接收溢出。

## 11. 调试 checklist

1. SPI Mode 是否和从设备手册一致。
2. `SS/CS` 是否低有效，是否覆盖完整命令。
3. `SCK/MOSI/MISO` 是否接错。
4. 数据位宽是 8 位还是 16 位。
5. 高位先行还是低位先行。
6. 分频后的 `SCK` 是否超过从设备最大时钟。
7. 读数据时是否继续发送 dummy byte 以产生时钟。
8. 多从机时是否有多个设备同时驱动 `MISO`。
9. Flash 写入前是否发送写使能，写前是否擦除。
10. 是否等待 `BSY=0` 后再拉高 `CS`。

## 参考资料

- `STM32入门教程.pptx`：SPI 通信、硬件电路、Mode 0 到 Mode 3、W25Q64、STM32 SPI 外设相关页。
- [Analog Devices: Introduction to SPI Interface](https://www.analog.com/en/resources/analog-dialogue/articles/2018/08/13/15/31/introduction-to-spi-interface.html)
- [Analog Devices AN-1248: SPI Interface](https://www.analog.com/media/en/technical-documentation/application-notes/AN-1248.pdf)
