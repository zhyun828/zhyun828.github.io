# I2C

I2C, Inter-Integrated Circuit, 是一种常见的板级低速同步串行总线。它只需要两根信号线：

- `SCL`: Serial Clock, 串行时钟线
- `SDA`: Serial Data, 串行数据线

它适合把 MCU 与传感器、EEPROM、RTC、电源管理芯片、IO 扩展器等器件连接在同一块 PCB 上。

## 一句话理解

I2C 像一条共享的两线小总线：控制器先发起通信，目标器件根据地址响应，双方按时钟节拍一位一位传数据。

## 总线连接

I2C 的 `SCL` 和 `SDA` 都是开漏或开集电极结构，器件只能主动把线拉低，不能主动输出高电平。总线恢复高电平依靠上拉电阻。

<figure markdown="span">
  <img src="https://www.analog.com/en/_/media/analog/en/app-note-images/an-1159/fig1.png?la=en&rev=ce58b4f274a14605b1bacf89dce7f733" alt="I2C bus connection with SDA, SCL, pull-up resistors, controller, and target devices" />
  <figcaption>图 1：I2C 典型连接方式。来源：Analog Devices AN-1159。</figcaption>
</figure>

这个结构带来几个重要结论：

- 总线空闲时，`SCL` 和 `SDA` 都应为高电平。
- 上拉电阻负责把总线拉回高电平；器件只负责把线拉低。
- 任意器件拉低总线，其他器件都会看到低电平，因此可以实现 ACK、仲裁和时钟拉伸。
- 板上多个模块如果都带上拉电阻，会形成并联等效电阻，需要检查总上拉是否过小。

## 角色

新版官方文档更倾向使用 controller 和 target：

- Controller: 发起通信、产生时钟、发送 START 和 STOP 的一方。
- Target: 被地址选中的器件，按 controller 的节拍收发数据。

老资料里常见 master 和 slave，阅读芯片手册时两套说法经常同时出现。

## 信号规则

I2C 的数据有效性规则非常关键：

- `SCL` 为高电平时，`SDA` 必须保持稳定，此时接收方采样数据。
- `SCL` 为低电平时，`SDA` 才允许变化。
- 例外是 START 和 STOP 条件：它们都是在 `SCL` 为高电平时改变 `SDA`。

<figure markdown="span">
  <img src="https://www.analog.com/en/_/media/analog/en/app-note-images/an-1159/fig7.png?la=en&rev=7734fecefb9a4e4faecf8b7e0857e389" alt="I2C-compatible interface timing diagram" />
  <figcaption>图 2：I2C 兼容接口时序图。来源：Analog Devices AN-1159。</figcaption>
</figure>

看这张时序图时，重点抓三件事：

- `SDA` 的数据位在 `SCL` 高电平期间被认为有效，所以不要在 `SCL` 高电平时随意改变数据。
- `tLOW` 和 `tHIGH` 分别约束时钟低电平和高电平持续时间，决定某个速率模式下的最低时序要求。
- `tSU` 和 `tH` 分别表示建立时间和保持时间：数据要在采样前提前稳定，采样后也要保持一小段时间。

## START 和 STOP

I2C 总线空闲时，`SCL` 和 `SDA` 都为高电平。Controller 要开始一次传输时发送 START；要结束一次传输时发送 STOP。

<figure markdown="span">
  <img src="https://www.analog.com/en/_/media/analog/en/app-note-images/an-1159/fig3.png?la=en&rev=68da20c28259472bab23d0d2a13fa322" alt="I2C start condition timing diagram" />
  <figcaption>图 3：START 条件，SCL 为高时 SDA 从高变低。来源：Analog Devices AN-1159。</figcaption>
</figure>

START 条件的判定方式：

- `SCL = 1`
- `SDA` 从 `1` 跳变到 `0`
- 表示 controller 获取总线并开始一次传输

<figure markdown="span">
  <img src="https://www.analog.com/en/_/media/analog/en/app-note-images/an-1159/fig4.png?la=en&rev=a7f280ab2d61401e941adf2db816955d" alt="I2C stop condition timing diagram" />
  <figcaption>图 4：STOP 条件，SCL 为高时 SDA 从低变高。来源：Analog Devices AN-1159。</figcaption>
</figure>

STOP 条件的判定方式：

- `SCL = 1`
- `SDA` 从 `0` 跳变到 `1`
- 表示 controller 释放总线，本次传输结束

## 一帧数据长什么样

常见 7 位地址模式下，controller 首先发送：

```text
START + 7-bit address + R/W + ACK + data byte + ACK/NACK + STOP
```

其中：

- 地址后面的 `R/W` 位为 `0` 表示写，为 `1` 表示读。
- 每 8 位数据后面都有第 9 个时钟，用来传 ACK 或 NACK。
- ACK 是接收方把 `SDA` 拉低；NACK 是接收方释放 `SDA`，由上拉电阻保持高电平。

<figure markdown="span">
  <img src="https://www.analog.com/en/_/media/analog/en/app-note-images/an-1159/fig2.png?la=en&rev=77516e94d1314983b857cf45a414e6aa" alt="I2C transfer sequence diagram" />
  <figcaption>图 5：I2C 完整传输序列。来源：Analog Devices AN-1159。</figcaption>
</figure>

这张图可以按顺序读：

- 起点是 START，说明总线从空闲进入传输状态。
- Controller 先发目标地址和读写方向位。
- 目标器件如果识别到自己的地址，会在 ACK 位把 `SDA` 拉低。
- 后面每个数据字节都是 8 位数据加 1 位 ACK/NACK。
- 最后用 STOP 释放总线。

## ACK 和 NACK

ACK/NACK 是 I2C 调试时最重要的观察点之一。每发送 8 位后，第 9 个时钟周期留给接收方回应。

<figure markdown="span">
  <img src="https://www.analog.com/en/_/media/analog/en/app-note-images/an-1159/fig5.png?la=en&rev=c35dac0e94b74de4a6a310fcab3eb1dc" alt="I2C ACK and NACK timing diagram" />
  <figcaption>图 6：ACK 与 NACK 时序。来源：Analog Devices AN-1159。</figcaption>
</figure>

判断方法：

- ACK: 第 9 个时钟期间，接收方把 `SDA` 拉低。
- NACK: 第 9 个时钟期间，接收方释放 `SDA`，线上保持高电平。

常见 NACK 原因：

- 地址错误，目标器件没有响应。
- 器件没有上电或接线错误。
- 读写方向位不符合当前操作。
- 寄存器地址或命令不被器件接受。
- 读多字节时，controller 用 NACK 表示最后一个字节已经读完。

## 写操作

写寄存器通常分三步：

1. Controller 发送目标器件地址，`R/W=0`。
2. Controller 发送要写入的寄存器地址或命令字。
3. Controller 发送数据字节。

```text
START
  target_address + W
  register_address
  data
STOP
```

如果要连续写多个字节，很多器件会在内部自动递增寄存器地址，但是否支持要看具体芯片手册。

## 读操作与 Repeated START

读寄存器常见流程是“先写寄存器地址，再重新开始读数据”：

```text
START
  target_address + W
  register_address
REPEATED START
  target_address + R
  data
NACK
STOP
```

<figure markdown="span">
  <img src="https://www.analog.com/en/_/media/analog/en/app-note-images/an-1159/fig6.png?la=en&rev=89db3cda0c534f3f9821c15398541848" alt="I2C repeated start timing diagram" />
  <figcaption>图 7：Repeated START 条件。来源：Analog Devices AN-1159。</figcaption>
</figure>

Repeated START 的意义是：不发送 STOP、不释放总线，直接从一个传输阶段切换到另一个传输阶段。

典型寄存器读取为什么需要它：

- 第一个阶段是写：告诉 target “我要读哪个寄存器”。
- 第二个阶段是读：重新发送同一个 target 地址，但方向位改成 `R`。
- 如果中间发送 STOP，某些器件可能会把内部状态机复位，导致读取失败或读到错误位置。
- 最后一个字节通常由 controller 发送 NACK，表示“不再继续读”。

## 地址

常见 I2C 使用 7 位地址。很多芯片手册会把地址写成两种形式：

- 7 位地址：例如 `0x68`
- 8 位读写地址：例如写地址 `0xD0`、读地址 `0xD1`

两者关系是：

```text
write_address = (address_7bit << 1) | 0
read_address  = (address_7bit << 1) | 1
```

调试时要先确认驱动 API 需要的是 7 位地址还是 8 位地址，这是非常常见的踩坑点。

## 速率

常见速率模式：

| 模式 | 典型最大速率 |
| --- | ---: |
| Standard-mode | 100 kbit/s |
| Fast-mode | 400 kbit/s |
| Fast-mode Plus | 1 Mbit/s |
| High-speed mode | 3.4 Mbit/s |

实际能跑多快取决于器件支持、走线长度、总线电容、上拉电阻和波形质量。嵌入式项目里，如果没有特殊需求，100 kHz 或 400 kHz 通常更稳。

## 上拉电阻

上拉电阻太大，`SCL`/`SDA` 上升沿会太慢；上拉电阻太小，器件拉低电平时电流会太大。

可以用这两个方向理解：

```text
Rpullup 太大 -> 上升慢 -> 高速通信容易失败
Rpullup 太小 -> 拉低电流大 -> VOL 可能超标，功耗增加
```

TI 的 I2C 上拉电阻应用笔记给出了工程计算思路：

```text
Rp(min) 由电源电压、低电平输出电压和灌电流能力决定
Rp(max) 由总线电容和允许的上升时间决定
```

常见经验值：

- 短线、低速、少量器件：`4.7 kΩ` 常见。
- 3.3 V、400 kHz、器件较多：可能需要 `2.2 kΩ` 到 `3.3 kΩ`。
- 板上多个模块都带上拉时，要注意等效电阻会变小。

## 时钟拉伸

Target 如果来不及准备数据，可以把 `SCL` 拉低，让 controller 等待。这叫 clock stretching。

注意：

- 不是所有 controller 驱动都完整支持 clock stretching。
- 某些传感器在测量转换期间会用 clock stretching。
- 如果总线表现为 `SCL` 长时间低电平，需要检查是否有器件在拉伸时钟，或总线被异常拉死。

## 多 controller 与仲裁

I2C 支持多个 controller。由于总线是线与结构，controller 在发送高电平时如果读到低电平，说明别的 controller 正在拉低总线，它会判定自己丢失仲裁并退出。

普通 MCU 项目大多是单 controller 架构，所以初学阶段可以先把重点放在地址、ACK、读写流程和电气连接上。

## 调试 checklist

1. 确认 `SCL` 和 `SDA` 是否都有上拉。
2. 空闲时两根线是否都是高电平。
3. 地址使用的是 7 位还是 8 位形式。
4. 逻辑分析仪里是否能看到 START、地址、ACK 和 STOP。
5. 读寄存器时是否需要 repeated START。
6. 总线速率是否超过目标器件支持范围。
7. 多个模块并联时，上拉电阻等效值是否过小。
8. `SDA` 或 `SCL` 长时间为低时，逐个断开器件排查是谁拉住总线。

## 和 SPI / UART 的直观区别

| 对比项 | I2C | SPI | UART |
| --- | --- | --- | --- |
| 时钟 | 有 `SCL` | 有 `SCLK` | 无独立时钟 |
| 数据线 | 双向 `SDA` | 通常 MOSI/MISO 分开 | TX/RX 分开 |
| 器件选择 | 地址 | 片选线 | 点对点为主 |
| 引脚数量 | 少 | 多 | 少 |
| 典型用途 | 板级低速外设 | 高速外设、显示、Flash | 串口调试、模块通信 |

## 参考资料

- [Analog Devices AN-1159: I2C-Compatible Interface Timing Specifications and Communication Protocol](https://www.analog.com/en/resources/app-notes/an-1159.html)
- [NXP UM10204: I2C-bus specification and user manual](https://www.nxp.com/docs/en/user-guide/UM10204.pdf)
- [Texas Instruments SLVA689: I2C Bus Pullup Resistor Calculation](https://www.ti.com/lit/an/slva689/slva689.pdf)
- [Analog Devices: I2C Communication Protocol, Understanding I2C Primer, PMBus, and SMBus](https://www.analog.com/en/resources/analog-dialogue/articles/2021/10/28/13/25/i2c-communication-protocol-understanding-i2c-primer-pmbus-and-smbus.html)
