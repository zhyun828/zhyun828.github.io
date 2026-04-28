# CAN

CAN, Controller Area Network, 是一种面向汽车和工业控制场景的多节点、实时、抗干扰串行通信协议。它常用于 ECU、传感器、执行器、电机控制器、电池管理系统和工业设备之间的通信。

## 一句话理解

CAN 像一条所有节点共享的差分总线：谁都可以尝试发送，但消息 ID 决定优先级；优先级高的消息会在仲裁中胜出，其他节点自动退让，不会破坏已经发送的数据。

## 总线结构

CAN 通常使用两根差分线：

- `CAN_H`
- `CAN_L`

每个节点一般由 MCU、CAN 控制器和 CAN 收发器组成。现代 MCU 可能内置 CAN 控制器，但仍然需要外部 CAN transceiver 接到物理总线上。

<figure markdown="span">
  <img src="https://developerhelp.microchip.com/xwiki/bin/download/applications/can/overview/WebHome/Picture1.jpg?height=334&rev=1.1&width=500" alt="Controller Area Network CAN overview graphic" />
  <figcaption>图 1：CAN 网络概念图。来源：Microchip Developer Help。</figcaption>
</figure>

硬件层面要特别注意：

- 总线两端通常需要终端电阻，典型值为 `120 Ω`。
- CAN 是差分通信，抗共模干扰能力比单端信号更好。
- 节点不是按地址通信，而是所有节点都接收总线上的帧，再根据 ID 过滤。

## 显性位和隐性位

CAN 总线有两个逻辑状态：

| CAN 术语 | 逻辑值 | 总线含义 |
| --- | ---: | --- |
| Dominant | 0 | 显性位，会覆盖隐性位 |
| Recessive | 1 | 隐性位，只有没人发送显性位时才保持 |

这和 I2C 的“线与”思想很像：如果一个节点发送隐性位 `1`，但读回总线是显性位 `0`，说明有更高优先级的节点正在发送。

## 仲裁

CAN 仲裁发生在消息 ID 阶段。多个节点可以同时开始发送，它们一边发送 ID，一边监测总线。

<figure markdown="span">
  <img src="https://developerhelp.microchip.com/xwiki/bin/download/applications/can/overview/mechanisms/WebHome/Picture14.jpg?rev=1.1" alt="CAN arbitration example" />
  <figcaption>图 2：CAN 仲裁示例。来源：Microchip Developer Help。</figcaption>
</figure>

读这张图时注意：

- ID 越小，优先级通常越高，因为更早出现 dominant `0`。
- 发送 recessive `1` 的节点如果读到 dominant `0`，说明自己输掉仲裁。
- 输掉仲裁的节点停止发送，等待下一次总线空闲再重试。
- 胜出的帧继续发送，不需要重传，因此 CAN 仲裁是非破坏性的。

## 数据帧结构

CAN 标准帧和扩展帧的 ID 长度不同：

- 标准帧：11-bit identifier。
- 扩展帧：29-bit identifier。

经典 CAN 的数据场最多 8 byte；CAN FD 支持更长数据场和更高数据阶段速率。

<figure markdown="span">
  <img src="https://developerhelp.microchip.com/xwiki/bin/download/applications/can/overview/mechanisms/WebHome/Picture11.jpg?rev=1.1" alt="CAN bit timing data frame diagram" />
  <figcaption>图 3：CAN 数据帧和 bit timing 示意。来源：Microchip Developer Help。</figcaption>
</figure>

从图里可以抓住三类信息：

- Identifier: 决定消息语义和仲裁优先级。
- Data: 实际载荷，经典 CAN 为 0 到 8 byte。
- ACK: 接收节点用于确认帧已经被至少一个节点正确接收。

## 位时序

CAN 的一个 bit 会被拆成多个时间段，用来补偿传播延迟和同步误差。

<figure markdown="span">
  <img src="https://developerhelp.microchip.com/xwiki/bin/download/applications/can/overview/mechanisms/WebHome/Picture12.png?rev=1.1" alt="CAN bit time programmable segments" />
  <figcaption>图 4：CAN bit time 可编程时间段。来源：Microchip Developer Help。</figcaption>
</figure>

常见时间段：

- Sync Seg: 用来同步边沿。
- Prop Seg: 补偿总线传播延迟。
- Phase Seg 1: 采样点前的相位缓冲。
- Phase Seg 2: 采样点后的相位缓冲。

调 CAN 波特率时，不能只看 “500 kbit/s” 这个数，还要看 sample point、SJW、TQ 分配是否和网络长度、收发器、其他节点兼容。

## ACK 与错误处理

CAN 有比较强的错误处理机制：

- 发送端会监测自己发出的位是否和总线一致。
- 接收端会检查 CRC、格式、位填充、ACK 等。
- 发现错误时，节点会发送 error frame。
- 错误计数过高的节点会进入 error passive，严重时进入 bus-off。

ACK 的含义：

- 发送节点在 ACK slot 发送 recessive。
- 任意正确接收该帧的节点会在 ACK slot 发送 dominant。
- 如果发送端没有看到 ACK，说明没有节点确认该帧，通常会重发。

## 位填充

为了保证总线上有足够边沿用于同步，CAN 使用 bit stuffing：

- 连续 5 个相同电平后，发送端插入 1 个相反电平。
- 接收端解析时会去掉这个 stuff bit。
- 如果接收端发现位填充规则被破坏，会报 stuffing error。

## 过滤器

CAN 是广播式总线：每个节点都会看到所有帧。为了减少 CPU 处理压力，CAN 控制器通常提供 ID filter 和 mask。

- Filter: 指定希望接收的 ID 模式。
- Mask: 指定哪些 ID bit 必须比较，哪些 bit 可以忽略。

这也是为什么 CAN 设计时要先规划 ID：ID 不只是“地址”，还包含优先级、消息类型和过滤策略。

## 调试 checklist

1. 总线两端是否各有 `120 Ω` 终端电阻。
2. `CAN_H` 和 `CAN_L` 是否接反。
3. 所有节点波特率、sample point、CAN FD 配置是否一致。
4. 收发器供电和待机/使能引脚是否正确。
5. 是否有节点一直 error frame 或 bus-off。
6. 是否只有一个节点在线导致没有 ACK。
7. ID filter 是否把目标帧过滤掉了。
8. 总线长度和分支长度是否适合当前速率。

## 和 USART / SPI / I2C 的直观区别

| 对比项 | CAN | USART/UART | SPI | I2C |
| --- | --- | --- | --- | --- |
| 拓扑 | 多节点共享总线 | 点对点为主 | 主从结构 | 多目标共享总线 |
| 物理层 | 差分 CAN_H/CAN_L | 取决于 TTL/RS-232/RS-485 | 单端板级信号 | 开漏上拉 |
| 仲裁 | 有，按 ID 优先级 | 无 | 无 | 有，按地址/位仲裁 |
| ACK | 有协议级 ACK | 无 | 无 | 有 ACK/NACK |
| 典型场景 | 汽车、工业控制 | 调试、模块通信 | 高速板级外设 | 低速板级外设 |

## 参考资料

- [Microchip Developer Help: Learn Controller Area Network (CAN) Protocol](https://developerhelp.microchip.com/xwiki/bin/view/applications/can/overview/)
- [Microchip Developer Help: Core Protocol Mechanisms of CAN](https://developerhelp.microchip.com/xwiki/bin/view/applications/can/overview/mechanisms/)
- [Texas Instruments: Introduction to the Controller Area Network (CAN)](https://www.ti.com/lit/an/sloa101b/sloa101b.pdf)
- [Texas Instruments: CAN Physical Layer and Termination Guide](https://www.ti.com/lit/an/slla270/slla270.pdf)
