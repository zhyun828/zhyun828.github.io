# SPI

SPI, Serial Peripheral Interface, 是一种同步、全双工、常用于板级外设连接的串行总线。它常用于 Flash、屏幕、ADC、DAC、传感器、无线模块等器件。

## 一句话理解

SPI 像一组由主设备控制节拍的移位寄存器：主设备拉低片选，输出时钟，MOSI 和 MISO 两条数据线可以同时一位一位移出和移入。

## 基本信号

标准 4 线 SPI 常见信号：

| 信号 | 方向 | 作用 |
| --- | --- | --- |
| `SCLK` / `CLK` | 主设备输出 | 串行时钟 |
| `MOSI` / `SDO` | 主到从 | 主设备发送，从设备接收 |
| `MISO` / `SDI` | 从到主 | 从设备发送，主设备接收 |
| `CS` / `SS` / `NSS` | 主设备输出 | 片选，通常低有效 |

<figure markdown="span">
  <img src="https://www.analog.com/en/_/media/images/analog-dialogue/en/volume-52/number-3/articles/introduction-to-spi-interface/205973_fig_01.svg?la=en&rev=f03bda0b77f94822a05ed2e7e8f8070d&sc_lang=en" alt="SPI configuration with main and subnode" />
  <figcaption>图 1：SPI 单主单从连接。来源：Analog Devices。</figcaption>
</figure>

这张图体现了 SPI 的几个特点：

- 主设备负责产生 `SCLK`。
- 主设备通过 `CS` 选择当前通信的从设备。
- `MOSI` 和 `MISO` 是两条独立数据线，因此可以全双工。
- 从设备未被选中时，通常需要释放或高阻其 `MISO`，避免多个从设备争用总线。

## 一次传输怎么开始

典型 SPI 传输流程：

1. 主设备把目标从设备的 `CS` 拉低。
2. 主设备输出 `SCLK`。
3. 每个时钟周期，主设备在 `MOSI` 上移出一位，同时从设备在 `MISO` 上移出一位。
4. 传完约定的 bit 数或字节数后，主设备停止时钟。
5. 主设备把 `CS` 拉高，结束本次传输。

SPI 没有地址字段，也没有标准 ACK。能不能读到正确数据，取决于片选、时钟模式、命令格式和时序是否符合从设备手册。

## CPOL 和 CPHA

SPI 最容易出错的是时钟模式。时钟模式由两个参数决定：

- `CPOL`: Clock Polarity，时钟空闲电平。
- `CPHA`: Clock Phase，在哪个边沿采样、哪个边沿移位。

四种常见模式：

| SPI Mode | CPOL | CPHA | 空闲时钟 | 常见描述 |
| ---: | ---: | ---: | --- | --- |
| 0 | 0 | 0 | 低电平 | 上升沿采样，下降沿移位 |
| 1 | 0 | 1 | 低电平 | 下降沿采样，上升沿移位 |
| 2 | 1 | 0 | 高电平 | 下降沿采样，上升沿移位 |
| 3 | 1 | 1 | 高电平 | 上升沿采样，下降沿移位 |

## Mode 0 时序

<figure markdown="span">
  <img src="https://www.analog.com/en/_/media/images/analog-dialogue/en/volume-52/number-3/articles/introduction-to-spi-interface/205973_fig_02.png?la=en&rev=c19f52f7fc014bbda34df6bf7c2a18fe&sc_lang=en" alt="SPI Mode 0 timing diagram" />
  <figcaption>图 2：SPI Mode 0，CPOL=0，CPHA=0。来源：Analog Devices。</figcaption>
</figure>

读这张图时重点看：

- `nCS` 低电平期间，本次 SPI 传输有效。
- `CLK` 空闲为低电平。
- 橙色虚线表示采样边沿，蓝色虚线表示移位边沿。
- Mode 0 中通常在上升沿采样，在下降沿准备下一位数据。
- `MOSI` 和 `MISO` 同时工作，主设备发送 `0xA5` 的同时也能收到 `0xBA`。

## Mode 3 时序

<figure markdown="span">
  <img src="https://www.analog.com/en/_/media/images/analog-dialogue/en/volume-52/number-3/articles/introduction-to-spi-interface/205973_fig_05.jpg?la=en&rev=2f0a41bece7d4bb481a4f647ea1e2caa&sc_lang=en" alt="SPI Mode 3 timing diagram" />
  <figcaption>图 3：SPI Mode 3，CPOL=1，CPHA=1。来源：Analog Devices。</figcaption>
</figure>

Mode 3 与 Mode 0 的区别：

- `CLK` 空闲为高电平。
- 数据采样仍发生在图中橙色虚线标出的边沿。
- 如果 MCU 配成 Mode 0，而从设备要求 Mode 3，逻辑分析仪可能能看到波形，但读出的数据会错位或完全错误。

## 多从设备连接

多个 SPI 从设备通常共享 `SCLK/MOSI/MISO`，但每个从设备有独立 `CS`。

<figure markdown="span">
  <img src="https://www.analog.com/en/_/media/images/analog-dialogue/en/volume-52/number-3/articles/introduction-to-spi-interface/205973_fig_06.svg?la=en&rev=5cb89587a840475fb17a3f64aae449e6&sc_lang=en" alt="Multi-subnode SPI configuration" />
  <figcaption>图 4：SPI 多从设备常规连接方式。来源：Analog Devices。</figcaption>
</figure>

注意点：

- 任意时刻通常只拉低一个从设备的 `CS`。
- 如果两个从设备同时驱动 `MISO`，数据会冲突。
- 从设备越多，主设备需要的片选 GPIO 越多。
- 有些器件支持 daisy-chain，但不是所有 SPI 器件都支持。

## 和 I2C / USART 的直观区别

| 对比项 | SPI | I2C | USART/UART |
| --- | --- | --- | --- |
| 时钟 | 有，由主设备输出 | 有，由 controller 输出 | 异步模式无共享时钟 |
| 数据方向 | MOSI/MISO 分开，全双工 | SDA 双向，半双工语义 | TX/RX 分开，全双工 |
| 设备选择 | 片选线 | 地址 | 点对点为主 |
| 标准 ACK | 无 | 有 ACK/NACK | 无协议级 ACK |
| 典型速度 | 较高 | 中低速 | 中低速到高速 |

## 调试 checklist

1. 从设备手册要求的 SPI Mode 是多少。
2. `CS` 是否低有效，是否覆盖完整命令和数据阶段。
3. 字节顺序是 MSB first 还是 LSB first。
4. 命令、地址、dummy byte、读写方向是否符合手册。
5. `MISO` 是否被多个从设备同时驱动。
6. 时钟频率是否超过从设备最大值。
7. 线太长或频率太高时，上升沿/下降沿是否变差。
8. 逻辑分析仪解码设置是否与 CPOL/CPHA 一致。

## 参考资料

- [Analog Devices: Introduction to SPI Interface](https://www.analog.com/en/resources/analog-dialogue/articles/2018/08/13/15/31/introduction-to-spi-interface.html)
- [Analog Devices AN-1248: SPI Interface](https://www.analog.com/media/en/technical-documentation/application-notes/AN-1248.pdf)
