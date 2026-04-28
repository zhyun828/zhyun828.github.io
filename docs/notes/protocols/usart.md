# USART

USART, Universal Synchronous and Asynchronous Receiver Transmitter, 是 MCU 里常见的串行通信外设。它可以工作在异步模式，也可以工作在同步模式。实际项目里最常用的是异步串口，也就是平时常说的 UART。

## 一句话理解

USART/UART 像两根点对点串行线：发送端和接收端不共享时钟，双方提前约好波特率、数据位、校验位和停止位，然后按固定节拍发送一帧一帧的数据。

## USART 和 UART 的关系

- UART: Universal Asynchronous Receiver Transmitter，只支持异步串行通信。
- USART: Universal Synchronous and Asynchronous Receiver Transmitter，既支持异步，也支持同步。
- 很多 MCU 手册里外设叫 USART，但你配置成无外部时钟、只用 `TXD/RXD` 时，本质上就是 UART 用法。

## 基本连接

异步串口最常见的连接只需要两根信号线和共地：

- `TXD`: Transmit Data，发送数据。
- `RXD`: Receive Data，接收数据。
- `GND`: 公共参考地。

连接时要交叉：

```text
MCU_A TXD  ->  MCU_B RXD
MCU_A RXD  <-  MCU_B TXD
MCU_A GND  --  MCU_B GND
```

如果使用同步 USART，还会额外出现 `XCK` 或类似名字的时钟线；如果使用硬件流控，还会出现 `RTS/CTS`。

<figure markdown="span">
  <img src="https://developerhelp.microchip.com/xwiki/bin/download/products/mcu-mpu/32bit-mcu/sam/l10-l11/peripherals/sercom/usart-uart/WebHome/saml10-sercom-usart-uart.png?height=261&rev=1.1&width=600" alt="Microchip SAM L10 SERCOM USART UART block diagram" />
  <figcaption>图 1：Microchip SAM L10/L11 SERCOM USART/UART 模块框图。来源：Microchip Developer Help。</figcaption>
</figure>

从框图可以看到，USART 外设内部通常包含：

- 发送缓冲区和发送移位寄存器。
- 接收移位寄存器和接收缓冲区。
- 波特率发生器。
- 帧格式控制和错误检测逻辑。
- 可选 DMA、中断、硬件流控、同步时钟等功能。

## 电平和接口标准

USART/UART 只描述串行帧怎么发，不等于规定了电气电平。常见电平有：

| 类型 | 电平特点 | 常见场景 |
| --- | --- | --- |
| TTL/CMOS UART | 0 V/3.3 V 或 0 V/5 V | MCU、传感器模块、USB-TTL |
| RS-232 | 正负电压，逻辑含义与 TTL 不同 | PC 串口、老设备 |
| RS-485 | 差分信号，适合长线和多节点 | 工业现场、Modbus RTU |

所以串口调不通时，要先确认“协议参数”和“电平标准”是不是都匹配。

## 异步帧格式

异步串口线路空闲时通常保持高电平。一帧数据从 start bit 开始，然后发送数据位，可选 parity bit，最后用 stop bit 回到空闲状态。

<figure markdown="span">
  <img src="https://developerhelp.microchip.com/xwiki/bin/download/products/mcu-mpu/32bit-mcu/sam/l10-l11/peripherals/sercom/usart-uart/WebHome/saml10-sercom-usart-uart_frame.png?height=172&rev=1.1&width=600" alt="Microchip SAM L10 SERCOM USART UART frame format diagram" />
  <figcaption>图 2：USART/UART 帧格式。来源：Microchip Developer Help。</figcaption>
</figure>

读这张图时按顺序看：

- `IDLE`: 空闲状态，信号线保持高电平。
- `St`: Start bit，固定为低电平，用来告诉接收端“一帧开始了”。
- `n`: 数据位，通常是 8 位，也可能是 5 到 9 位。
- `[P]`: 可选校验位，可以是奇校验或偶校验。
- `Sp`: Stop bit，固定为高电平，可以是 1 位或 2 位。

最常见的配置是 `8N1`：

```text
8 data bits, No parity, 1 stop bit
```

## 波特率

波特率表示每秒传输多少个符号。对普通 UART 来说，可以近似理解为每秒传多少 bit。

常见波特率：

| 波特率 | 常见用途 |
| ---: | --- |
| 9600 | 低速调试、老设备 |
| 115200 | MCU 串口日志、下载器、模块通信 |
| 1 Mbps | 板级高速串口、短线通信 |

发送端和接收端的波特率必须接近。如果偏差太大，接收端采样点会逐渐偏移，轻则乱码，重则帧错误。

## 接收采样

异步串口没有共享时钟，接收端通常这样工作：

1. 平时监测 `RXD` 是否从高电平变低。
2. 发现 start bit 后，等待半个 bit 时间，确认仍为低电平，避免误判毛刺。
3. 之后每隔一个 bit 时间在中间位置采样一次。
4. 采满数据位、校验位和停止位后，得到一个字符。

因此，UART 对波特率误差、边沿质量和噪声比较敏感。

## 发送和接收流程

发送流程：

1. 软件把一个字节写入发送数据寄存器。
2. 外设把数据装入移位寄存器。
3. 自动补 start bit、数据位、校验位和 stop bit。
4. 按波特率从 `TXD` 逐位输出。

接收流程：

1. 外设从 `RXD` 检测 start bit。
2. 按配置采样数据位和校验位。
3. 检查 stop bit 是否正确。
4. 把收到的数据放入接收寄存器或 FIFO。
5. 软件通过轮询、 interrupt 或 DMA 读取数据。

## 常见错误

| 错误 | 含义 | 常见原因 |
| --- | --- | --- |
| Framing Error | 停止位位置不符合预期 | 波特率不匹配、噪声、接错电平 |
| Parity Error | 校验位不匹配 | 双方校验配置不同、数据受干扰 |
| Overrun Error | 新数据到达时旧数据还没读走 | 中断处理慢、FIFO 太小、DMA 未开 |
| Break | 线路长时间保持低电平 | 对端发送 break、短路、接线异常 |

## 调试 checklist

1. `TXD` 是否接到对方 `RXD`，`RXD` 是否接到对方 `TXD`。
2. 双方是否共地。
3. 电平是否匹配：TTL、RS-232、RS-485 不能直接混接。
4. 波特率、数据位、校验位、停止位是否完全一致。
5. 逻辑分析仪解码是否能识别 start bit 和 stop bit。
6. 是否误用了反相串口电平。
7. 接收中断或 DMA 是否及时搬走数据，避免 overrun。
8. RS-485 半双工时，收发方向控制是否覆盖完整帧。

## 参考资料

- [Microchip Developer Help: SAM L10/L11 SERCOM USART](https://developerhelp.microchip.com/xwiki/bin/view/products/mcu-mpu/32bit-mcu/sam/l10-l11/peripherals/sercom/usart-uart/)
- [Microchip Developer Help: PIC32 USART Overview](https://developerhelp.microchip.com/xwiki/bin/view/products/mcu-mpu/32bit-mcu/PIC32/peripherals/usart-overview/)
- [Microchip TB3208: Basic Operation of UART with Protocol Support](https://www.microchip.com/en-us/application-notes/tb3208)
