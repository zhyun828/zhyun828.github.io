# STM32 学习知识点整理

> 这份笔记根据本次对话里提到的问题整理，按知识结构重新排序，方便背诵和查阅。
> 说明：原来的“本对话问题整理_新版.md”中的问题已经按题型并入本文，统一保留在文中的补充问题索引里，后续继续在这一份总文档上迭代即可。

# 1. 基础概念

## 1.1 PC13 是什么
在 STM32 中，`PC13` 表示：
- `P` = Port（端口）
- `C` = GPIOC 端口
- `13` = 该端口的第 13 号引脚
所以 `PC13` 就是 **GPIOC 端口的第 13 个引脚**。
它本质上是一个 GPIO 引脚，但也可能复用成其他功能，具体要看芯片型号的数据手册和参考手册。
很多 STM32 开发板上，板载 LED 常接在 `PC13`。

## 1.2 Port 和 GPIO 的区别
### GPIO
GPIO 是 **通用输入输出引脚**（General Purpose Input/Output）。

### Port
Port 是 **一组 GPIO 引脚的集合**。
例如：
- `GPIOA` 包含 `PA0 ~ PA15`
- `GPIOB` 包含 `PB0 ~ PB15`
- `GPIOC` 包含 `PC0 ~ PC15`

### 一句话记忆
- **GPIO**：单个引脚的概念
- **Port**：一组 GPIO 的概念

## 1.3 OLED 里的 O 是什么
`OLED` = **Organic Light Emitting Diode**
其中：
- `O` = **Organic**（有机）
所以 OLED 的中文一般叫：**有机发光二极管**

## 1.4 AO 口和 DO 口是什么
这两个说法更常见于模块，而不是 STM32 寄存器命名。

### AO
`AO` = **Analog Output** / 模拟输出
表示模块输出的是模拟电压。

### DO
`DO` = **Digital Output** / 数字输出
表示模块输出的是数字高低电平。

### 注意
很多传感器模块同时带：
- `AO`：模拟量输出，可接 STM32 的 ADC
- `DO`：数字量输出，可接 STM32 的普通 GPIO 输入

# 2. ARM 位宽与地址

## 2.1 32 位 ARM 架构中的“32 位”指什么
32 位主要表示处理器的基础数据宽度是 32 位，通常体现在：
1. **通用寄存器宽度**通常为 32 位
2. **一次运算最自然处理 32 位数据**
3. **地址空间通常按 32 位地址表示**

### 一句话记忆
**32 位 = 寄存器和数据通路通常以 32 位为基本单位。**

## 2.2 STM32 地址为什么经常写成 `0x00000000` 这种 8 位十六进制形式
不是巧合，而是因为 STM32 是 32 位架构，地址通常按 **32 位地址值**表示。

### 为什么是 8 位十六进制
因为：
- 1 位十六进制 = 4 位二进制
- 8 位十六进制 = 32 位二进制

例如：
- `0x08000000`：Flash 起始地址
- `0x20000000`：SRAM 起始地址
- `0x40000000`：外设寄存器区起始地址

### 一句话记忆
**8 位十六进制地址 = 32 位二进制地址。**

# 3. 总线与时钟

## 3.1 AHB 和 APB 的区别

### AHB
`AHB` = **Advanced High-performance Bus**
特点：
- 高性能总线
- 速度快
- 带宽大
- 常连接内核、DMA、存储器等高速模块

### APB
`APB` = **Advanced Peripheral Bus**
特点：
- 外设总线
- 结构更简单
- 功耗更低
- 常连接串口、定时器、I2C、SPI 等外设

### 一句话记忆
**AHB 管高速主干，APB 管普通外设。**

## 3.2 GPIO 在 APB 里吗
### 对 STM32F1（如 STM32F103）来说
GPIO 一般挂在 **APB2** 上。
例如：
```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
```
这就说明 `GPIOA` 属于 `APB2`。

### 但要注意
不是所有 STM32 系列都一样。
- **STM32F1**：GPIO 常在 `APB2`
- **很多新系列**：GPIO 常在 `AHB` / `AHB1`
所以最终要看具体芯片参考手册。

# 4. GPIO 输入输出模式

## 4.1 GPIO 模式总览（以 STM32F1 为主）
| 模式 | 标准库名称 | 作用 |
|---|---|---|
| 浮空输入 | `GPIO_Mode_IN_FLOATING` | 输入，不上拉不下拉 |
| 上拉输入 | `GPIO_Mode_IPU` | 输入，默认高电平 |
| 下拉输入 | `GPIO_Mode_IPD` | 输入，默认低电平 |
| 模拟输入 | `GPIO_Mode_AIN` | 给 ADC 用 |
| 推挽输出 | `GPIO_Mode_Out_PP` | 普通数字输出 |
| 开漏输出 | `GPIO_Mode_Out_OD` | 只能主动拉低 |
| 复用推挽输出 | `GPIO_Mode_AF_PP` | USART/PWM/SPI 等外设输出 |
| 复用开漏输出 | `GPIO_Mode_AF_OD` | I2C 等外设输出 |

## 4.2 输入模式说明
### 浮空输入
- 没有内部上拉/下拉
- 外部不驱动时电平可能飘

### 上拉输入
- 内部接上拉电阻
- 默认高电平
- 按键最常用

### 下拉输入
- 内部接下拉电阻
- 默认低电平

### 模拟输入
- 给 ADC 采样使用
- 输入的是连续模拟电压

## 4.3 输出模式说明
### 推挽输出
- 能主动输出高电平
- 能主动输出低电平
- 驱动能力强
- 点 LED、控制模块最常用

### 开漏输出
- 能主动拉低
- 不能主动拉高
- 高电平要靠外部上拉
- I2C 常用

### 一句话记忆
- **推挽：高低都能主动输出**
- **开漏：只能主动拉低**

## 4.4 点 LED 建议用什么输出
一般建议用：
```c
GPIO_Mode_Out_PP
```
也就是 **推挽输出**。
原因：
- 控制简单
- 高低电平都明确
- 驱动普通 LED 最稳妥

## 4.5 LED 是 `SetBits` 点亮还是 `ResetBits` 点亮
这**不是由 GPIO 模式决定的**，而是由 **LED 的硬件接法决定的**。

### 情况 1：高电平点亮
如果接法是：
```
GPIO --- 电阻 --- LED --- GND
```
那么：
- `SetBits`：亮
- `ResetBits`：灭

### 情况 2：低电平点亮
如果接法是：
```
VCC --- 电阻 --- LED --- GPIO
```
那么：
- `ResetBits`：亮
- `SetBits`：灭

### 实战判断方法
直接试：
- `GPIO_SetBits(...)` 看灯亮不亮
- `GPIO_ResetBits(...)` 看灯亮不亮
很多开发板板载 LED 是 **低电平点亮**。

# 5. GPIO 常用寄存器（STM32F1）

## 5.1 GPIOA 寄存器截图里的项目是什么意思
| 寄存器 | 英文全称 | 作用 |
|---|---|---|
| `CRL` | Configuration Register Low | 配置 0~7 号引脚 |
| `CRH` | Configuration Register High | 配置 8~15 号引脚 |
| `IDR` | Input Data Register | 读取输入电平 |
| `ODR` | Output Data Register | 保存输出电平状态 |
| `BSRR` | Bit Set/Reset Register | 位置位/复位寄存器 |
| `BRR` | Bit Reset Register | 位复位寄存器 |
| `LCKR` | Lock Register | 锁定 GPIO 配置 |

## 5.2 ODR、BSRR、BRR 的区别
### ODR
`ODR` = Output Data Register
- 保存整个端口的输出状态
- 改某一位通常是“读-改-写”

### BSRR
`BSRR` = Bit Set/Reset Register
- 低 16 位写 1：对应位输出高电平
- 高 16 位写 1：对应位输出低电平
- 原子性更好，更推荐用来控制单个引脚

### BRR
`BRR` = Bit Reset Register
- 写 1 到某位：对应引脚输出低电平

### 一句话记忆
- **ODR：存输出状态**
- **BSRR：既能置 1 又能清 0**
- **BRR：专门清 0**

# 6. STM32 外设总览（常见模块）
下面是你截图里那些常见外设的简要说明：

| 缩写 | 英文 | 中文/作用 |
|---|---|---|
| ADC | Analog-to-Digital Converter | 模数转换器 |
| AFIO | Alternate Function I/O | 复用功能与外部中断映射 |
| BKP | Backup Registers | 备份寄存器/备份域 |
| CAN | Controller Area Network | CAN 总线通信 |
| CRC | Cyclic Redundancy Check | CRC 校验 |
| DAC | Digital-to-Analog Converter | 数模转换器 |
| DBG | Debug | 调试相关 |
| DMA | Direct Memory Access | 直接存储器访问 |
| EXTI | External Interrupt | 外部中断控制 |
| FLASH | Flash Memory Interface | Flash 控制 |
| FSMC | Flexible Static Memory Controller | 外部静态存储器控制 |
| GPIO | General Purpose I/O | 通用输入输出 |
| I2C | Inter-Integrated Circuit | I2C 总线 |
| IWDG | Independent Watchdog | 独立看门狗 |
| NVIC | Nested Vectored Interrupt Controller | 中断控制器 |
| PWR | Power Control | 电源控制 |
| RCC | Reset and Clock Control | 复位与时钟控制 |
| RTC | Real-Time Clock | 实时时钟 |
| SDIO | SD Input/Output | SD 卡接口 |
| SPI | Serial Peripheral Interface | SPI 总线 |
| TIM | Timer | 定时器 |
| USART | Universal Synchronous/Asynchronous Receiver Transmitter | 串口 |
| USB | Universal Serial Bus | USB 接口 |
| WWDG | Window Watchdog | 窗口看门狗 |

# 7. 中断系统：IRQ、NVIC、优先级

## 7.1 IRQ 是什么
`IRQ` = **Interrupt Request** / 中断请求
含义：
- 外设或事件向 CPU 发出“需要处理我”的请求
- CPU 收到后会跳去执行对应的中断服务函数（ISR）
例如：
- 定时器更新中断
- 串口接收中断
- 外部中断 EXTI

## 7.2 抢占优先级和响应优先级
STM32 NVIC 优先级通常分两部分：

### 抢占优先级（Preemption Priority）
决定：**一个中断能不能打断另一个正在执行的中断**
数值越小，优先级越高。

### 响应优先级 / 子优先级（Subpriority）
决定：**多个不能互相抢占的中断同时挂起时，谁先被响应**
数值越小，优先级越高。

### 一句话记忆
- **抢占优先级：能不能插队**
- **响应优先级：大家排队时谁先来**

## 7.3 为什么 `NVIC_InitStructure` 可以用两次
因为它只是一个普通结构体变量。
你每次：
1. 重新给结构体成员赋值
2. 再调用 `NVIC_Init()`
就是在配置一个新的中断。
例如先配：
- `EXTI0_IRQn`
再改成员后配：
- `EXTI1_IRQn`
这完全正常。

## 7.4 什么是 `ITConfig`
`IT` = **Interrupt**
所以 `xxx_ITConfig()` 通常表示：**给某个外设配置中断使能或中断相关功能**
例如：
```c
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
```
表示：**使能 TIM2 的更新中断**。

## 7.5 为什么没有主动调用 `TIM2_IRQHandler()`，但它仍然会执行
因为中断服务函数不是靠你手动调用的，而是：
- 外设产生中断事件
- NVIC 检测到这个中断请求
- CPU 自动跳转到对应 ISR 执行
所以 `TIM2_IRQHandler()` 是由 **硬件中断机制自动调用** 的。

# 8. 编码器外部中断代码要点

## 8.1 为什么编码器例子里必须调用 `Encoder_Init()`
如果 `main()` 里没有调用：
```c
Encoder_Init();
```
那么：
- GPIO 不会初始化
- EXTI 不会配置
- NVIC 不会使能
- 中断函数虽然写了，但永远不会真正触发

### 结论
**中断服务函数写出来不等于就能工作，前提是对应外设和 NVIC 必须先初始化。**

# 9. 定时器核心概念

## 9.1 什么是定时器更新事件（Update Event）
更新事件通常表示：
- 定时器计数器到达自动重装值 `ARR`
- 发生溢出/更新
- 于是产生一次“更新事件”
如果更新中断开着，就会触发定时器更新中断。

### 一句话理解
**更新事件就是“这一轮计数结束了”。**

## 9.2 为什么要写 `TIM_ClearFlag(TIM2, TIM_FLAG_Update)`
定时器一旦发生更新事件，会置位对应标志位。
如果不清除：
- 中断可能持续触发
- 后续更新事件处理会受影响
所以中断服务中通常要清除：
```c
TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
```
而初始化时有时也会先清一次旧标志，避免“刚开中断就立刻进入”。

## 9.3 `TIM_ETRClockMode2Config(...)` 是什么
这个函数用于配置：**定时器使用外部触发输入 ETR 作为时钟源（外部时钟模式 2）**。
主要可配置：
- 外部触发信号预分频
- 极性
- 输入滤波
常用于：
- 外部脉冲计数
- 外部时钟驱动定时器计数

## 9.4 `TI1FP1` 是什么
这是定时器输入捕获/从模式控制里常见的命名。
可以拆成：
- `TI1`：定时器输入通道 1
- `FP1`：经过滤波/极性处理后的输入信号 1
更准确理解：**TI1FP1 表示“定时器通道 1 的输入信号，经输入选择、极性和滤波后形成的内部信号”。**
这个信号常被用于：
- 输入捕获
- 从模式触发
- 编码器接口

### 关于 `FP`
这里的 `FP` 更接近 **Filtered Path / Filtered input** 的意思，可以理解成“经过处理后的输入信号”。

## 9.5 定时器里的主模式和从模式
这里更推荐按**定时器同步**来理解，而不是按串口/I2C 那套主从理解。

### 主模式（Master）
一个定时器把自己的某个事件（如更新事件）通过 `TRGO` 输出出去，作为“触发源”。

### 从模式（Slave）
另一个定时器把外部/内部触发输入 `TRGI` 当作控制信号，决定：
- 何时启动
- 何时复位
- 何时门控计数
- 何时外部时钟驱动

### 一句话记忆
- **主模式：我发触发**
- **从模式：我听触发**

# 10. ADC 基础知识

## 10.1 EOC 是什么
`EOC` = **End of Conversion** / 转换结束标志
意思是：**ADC 一次转换完成了，结果已经准备好了。**
通常这时可以去读：
```c
ADC1->DR
```

## 10.2 `ADC1->DR` 是什么
`DR` = **Data Register** / 数据寄存器
作用：**存放 ADC 转换结果**
例如 12 位 ADC：
- 结果范围通常为 `0 ~ 4095`
- 每完成一次采样，结果都会写入 `ADC1->DR`

### 一句话记忆
**ADC1->DR 就是 ADC1 的转换结果存放处。**

# 11. DMA 触发方式

## 11.1 DMA 的硬件触发和软件触发区别

### 硬件触发
由外设事件自动触发 DMA 请求。
例如：
- ADC 转换完成
- USART 收到数据
- SPI 发送/接收事件
- 定时器事件

### 软件触发
由程序手动开启或手动触发 DMA 传输。
例如：
```c
DMA_Cmd(DMA1_Channel1, ENABLE);
```

### 一句话记忆
- **硬件触发：外设一有事，DMA 自动搬**
- **软件触发：程序手动让 DMA 开始搬**

# 12. ADC + DMA 工作原理（你的 `AD_Init()` 代码）
这段代码的整体作用是：**让 ADC1 按顺序不断采样 PA0、PA1、PA2、PA3 四个通道，并用 DMA 自动把结果搬到内存数组里。**

## 12.1 工作流程总览
```
PA0/PA1/PA2/PA3 模拟电压
        ↓
ADC1 按顺序扫描采样
        ↓
每个结果写入 ADC1->DR
        ↓
ADC 发 DMA 请求
        ↓
DMA 把 DR 中数据搬到 ADC_Value[]
        ↓
采完 4 个通道后继续循环
```

## 12.2 关键配置点

### 1）GPIO 配为模拟输入
```c
GPIO_Mode_AIN
```

### 2）规则组顺序配置
```c
ADC_RegularChannelConfig(...)
```
表示按顺序采：
1. `ADC_Channel_0`
2. `ADC_Channel_1`
3. `ADC_Channel_2`
4. `ADC_Channel_3`

### 3）扫描模式
```c
ADC_ScanConvMode = ENABLE;
```
表示多个通道按顺序采样。

### 4）连续转换模式
```c
ADC_ContinuousConvMode = ENABLE;
```
表示采完一轮后继续下一轮，不停。

### 5）DMA 方向
```c
DMA_DIR = DMA_DIR_PeripheralSRC;
```
表示数据从外设（ADC）搬到内存。

### 6）DMA 循环模式
```c
DMA_Mode = DMA_Mode_Circular;
```
表示数组写满后又从头继续覆盖。

### 7）ADC 软件启动
```c
ADC_SoftwareStartConvCmd(ADC1, ENABLE);
```
表示第一次转换由软件启动。

### 8）DMA 触发方式
这里是：
- **ADC 启动是软件触发**
- **DMA 搬运是 ADC 转换完成后的硬件触发**

# 13. 串口 USART / Serial 知识点

## 13.1 什么是硬件流控制
硬件流控制用于避免串口双方速度不匹配导致丢数据。
通常用两根握手线：
- `RTS`
- `CTS`

### 作用
- `RTS`：我告诉对方“你能不能给我发”
- `CTS`：对方告诉我“你能不能发”

### 一句话记忆
**硬件流控制 = 用 RTS/CTS 硬件线控制串口什么时候发、什么时候停。**

## 13.2 “9600 波特率、8 位字长、无校验、1 位停止位、无流控、只有发送模式”是什么意思
表示串口参数为：

| 参数 | 含义 |
|---|---|
| 9600 波特率 | 每秒 9600 bit |
| 8 位字长 | 1 帧中数据部分 8 位 |
| 无校验 | 不加奇偶校验位 |
| 1 位停止位 | 1 帧结束时 1 位停止位 |
| 无流控 | 不用 RTS/CTS |
| 只有发送模式 | 只开 TX，不开 RX |

这类配置常简写为：**9600，8N1，TX only**

## 13.3 `while (*String)` 为什么可以这么写
例如：
```c
void Serial_SendString(char* String)
{
    while (*String) {
        Serial_SendByte(*String++);
    }
}
```
原因：
- `String` 是指针
- `*String` 是当前字符
- C 语言里：
  - `0` 为假
  - 非 `0` 为真
字符串末尾有结束符：
```c
'\0'
```
它的值就是 `0`。
所以：
```c
while (*String)
```
本质上等价于：
```c
while (*String != '\0')
```

## 13.4 `Serial_Printf()` 的工作原理
例如：
```c
void Serial_Printf(const char* Format, ...)
{
    char Buffer[128];
    va_list Args;
    va_start(Args, Format);
    vsnprintf(Buffer, sizeof(Buffer), Format, Args);
    va_end(Args);
    Serial_SendString(Buffer);
}
```
作用：**做一个串口版的 printf**。

### 流程
1. 准备缓冲区 `Buffer`
2. 用 `va_list` 接收可变参数
3. `vsnprintf()` 按格式字符串把参数拼成普通字符串
4. `Serial_SendString()` 发出去

### 一句话记忆
**先格式化到字符串，再通过串口发送。**

# 14. C 语言数组、字符串与 `sizeof`

## 14.1 `sizeof(Array)` 得到的是什么
对数组来说：
```c
sizeof(Array)
```
得到的是：**整个数组占用的总字节数**
不是直接返回元素个数。

## 14.2 为什么 `uint8_t Array[] = "Hello, World!";` 里 `sizeof(Array)` 看起来像数组长度
因为：
- `uint8_t` 每个元素占 1 字节
- 所以总字节数 = 元素个数
例如：
```c
uint8_t Array[] = "Hello, World!";
```
实际包含结尾 `\0`，所以总字节数是 14。

### 注意
```c
sizeof(Array) - 1
```
常用于：**发送字符串内容，但不发送结尾的 `\0`**。

## 14.3 任何情况下数组大小都等于所占字节数吗
不一定。
要区分：
1. **数组元素个数**
2. **数组总字节数**
只有当元素类型大小为 1 字节时，它们才数值相等。
例如：
```c
char s[] = "abc";   // 4 个字节：a b c \0
uint16_t a[10];     // 元素个数 10，总字节数 20
```

### 一句话记忆
**`sizeof(数组)` 返回总字节数，不是直接返回元素个数。**

# 15. 数据类型与溢出

## 15.1 为什么 `uint8_t KeyNum = 666;` 打印出来是 154
因为 `uint8_t` 范围是：
```
0 ~ 255
```
而 `666` 超出了范围，所以只保留低 8 位，相当于：
```
666 mod 256 = 154
```
所以最终存进去的是 `154`。

### 一句话记忆
**不是 printf 打错了，而是 `uint8_t` 装不下 666。**

# 16. STM32 标准库命名：`Init`、`StructInit`、`DeInit`

## 16.1 `StructInit`
例如：
```c
GPIO_StructInit(&GPIO_InitStructure);
```
作用：**给初始化结构体填默认值**。
目的是防止你忘记初始化某些字段。

## 16.2 `DeInit`
例如：
```c
USART_DeInit(USART1);
```
作用：**把外设恢复到复位后的默认状态**。
是对外设寄存器本身操作，不是对结构体操作。

### 一句话记忆
- **StructInit：初始化结构体默认值**
- **DeInit：复位外设配置**

# 17. 常见寄存器/字段缩写总表
下面整理常见的寄存器或字段缩写，方便记忆。

## 17.1 通用寄存器缩写
| 缩写 | 英文 | 含义 |
|---|---|---|
| `CR` | Control Register | 控制寄存器 |
| `SR` | Status Register | 状态寄存器 |
| `DR` | Data Register | 数据寄存器 |
| `BRR` | Bit Reset Register / Baud Rate Register | GPIO 中常表示位复位；USART 中常表示波特率寄存器 |
| `BSRR` | Bit Set/Reset Register | 位置位/复位寄存器 |
| `IDR` | Input Data Register | 输入数据寄存器 |
| `ODR` | Output Data Register | 输出数据寄存器 |
| `CNT` | Counter | 计数器 |
| `PSC` | Prescaler | 预分频器 |
| `ARR` | Auto Reload Register | 自动重装寄存器 |
| `CCR` | Capture/Compare Register | 捕获比较寄存器 |
| `CCMR` | Capture/Compare Mode Register | 捕获比较模式寄存器 |
| `CCER` | Capture/Compare Enable Register | 捕获比较使能寄存器 |
| `EGR` | Event Generation Register | 事件产生寄存器 |
| `DIER` | DMA/Interrupt Enable Register | DMA/中断使能寄存器 |
| `PR` | Pending Register | 挂起标志寄存器 |
| `IMR` | Interrupt Mask Register | 中断屏蔽寄存器 |
| `EMR` | Event Mask Register | 事件屏蔽寄存器 |
| `RTSR` | Rising Trigger Selection Register | 上升沿触发选择寄存器 |
| `FTSR` | Falling Trigger Selection Register | 下降沿触发选择寄存器 |
| `RCR` | Repetition Counter Register | 重复计数寄存器 |
| `BDTR` | Break and Dead-Time Register | 断路与死区时间寄存器 |

## 17.2 常见字段/标志缩写
| 缩写 | 英文 | 含义 |
|---|---|---|
| `EOC` | End of Conversion | ADC 转换结束 |
| `TXE` | Transmit Data Register Empty | 发送数据寄存器空 |
| `RXNE` | Receive Data Register Not Empty | 接收数据寄存器非空 |
| `TC` | Transmission Complete | 发送完成 |
| `DTG` | Dead-Time Generator | 死区时间相关字段 |
| `REP` | Repetition Counter | 重复计数概念（常对应 `RCR`） |
| `RDP` | Read Protection | 读保护 |
| `WRP` | Write Protection | 写保护 |

## 17.3 关于 `RC_W0` 是什么（重要更正）
在 STM32 参考手册里，寄存器位的访问属性常见有：
- `rw`：可读可写
- `r`：只读
- `w`：只写
- `rc_w0`：**read / clear by writing 0**
也就是：**这个位可以读，软件写 0 可以清除它。**
这是寄存器位访问类型说明，不是某个具体寄存器名字。

### 一句话记忆
**`rc_w0` = 可读，写 0 清除。**

# 18. 下载、BootLoader、选项字节与保护

## 18.1 DTR / RTS 自动复位、自动进 BootLoader 是什么
串口下载工具里常有 DTR / RTS 选项，用来控制：
- 自动复位
- 自动进入 BootLoader
因为很多板子会把：
- `DTR`
- `RTS`
接到板子的：
- `RESET`
- `BOOT0`
但不同板子接法不同，所以软件里要让你选“高电平有效还是低电平有效、哪个脚负责复位、哪个脚负责进 BootLoader”。

### 一句话记忆
**这些选项就是在配置串口工具如何用 DTR/RTS 自动控制复位和下载模式。**

## 18.2 什么是选项字节（Option Bytes）
选项字节是 STM32 Flash 里的特殊区域，用来保存芯片级配置。
常见配置包括：
- 读保护
- 写保护
- 看门狗方式
- Boot 相关配置
- BOR 等特殊设置

### 一句话记忆
**选项字节 = 芯片的“系统配置区”。**

## 18.3 什么是读保护（Read Protection）
读保护用于：**防止别人把你芯片里的程序读出来。**
常见等级：
- `Level 0`：无保护
- `Level 1`：开启保护，若要解除通常会整片擦除
- `Level 2`：最高保护，很多系列不可逆

### 一句话记忆
**读保护防“读走你的代码”。**

## 18.4 什么是写保护（Write Protection）
写保护用于：**防止某些 Flash 页被擦除或改写。**
常用于保护：
- Bootloader
- 关键参数区
- 固件核心区

### 一句话记忆
**写保护防“把关键区写坏”。**

# 19. 常见工程错误与排查

## 19.1 `Flash Download failed - Could not load file ... Project.axf`
如果终端里出现类似：
```
null -b "...Project.uvprojx"
```
并提示：
```
'null' 不是内部或外部命令
```
这通常不是代码问题，而是：**VSCode/任务配置没有找到 Keil 的 `UV4.exe` 路径。**

### 排查思路
1. 确认已安装 Keil
2. 找到 `UV4.exe` 路径
3. 检查 `.vscode/tasks.json`
4. 把 `command` 改成真实 Keil 路径

## 19.2 `function declared implicitly` 警告
例如：
```
function "Key_Init" declared implicitly
```
说明：**在使用这个函数时，编译器还没见过它的函数声明。**
通常原因：
- 少写了 `#include "Key.h"`
- `Key.h` 里没写函数原型
- `Key.c` 没加入工程

### 一句话记忆
**隐式声明警告 = 用了函数，但没提前声明。**

## 19.3 `identifier "ADO" is undefined` / `too many arguments in function call`
这类错误通常说明：
1. 变量名写错了（如 `ADO` 其实应是 `AD0`）
2. 函数声明和调用参数个数不匹配
3. 头文件和源文件里的函数原型不一致

### 典型排查
- 检查变量拼写
- 检查函数原型
- 检查头文件是否包含正确

# 20. 一些高频实战结论速记

## 20.1 点 LED
- 建议模式：`GPIO_Mode_Out_PP`
- 是否 `SetBits` 点亮取决于硬件接法

## 20.2 按键
- 常用模式：`GPIO_Mode_IPU`

## 20.3 ADC
- 模拟输入：`GPIO_Mode_AIN`
- 结果寄存器：`ADCx->DR`
- 转换完成标志：`EOC`

## 20.4 串口
- 最常见参数：`9600 8N1`
- TX 引脚常用：`GPIO_Mode_AF_PP`
- I2C 常用：`GPIO_Mode_AF_OD`

## 20.5 中断
- ISR 不需要手动调用
- 必须先配置外设 + NVIC
- 抢占优先级决定能不能打断
- 子优先级决定排队顺序

## 20.6 DMA
- 硬件触发：外设事件自动搬
- 软件触发：程序手动开始搬

# 21. 按问题类型归并的补充问题索引

这一节把原来单独整理的对话问题，按题型并入当前 STM32 总笔记中，便于后续继续补充答案或回查。

## 21.1 C / C++ 基础与标准库

- 内联函数和宏定义的区别与使用场景
- `void MyI2C_W_SCL(uint8_t Bitvalue){ GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)Bitvalue); Delay_us(10); }` 为什么可以写成 `(BitAction)Bitvalue`
- `time.h` 大概是做什么的
- `struct tm` 为什么要写成 `struct tm time_data;`，不能直接写 `tm time_data;`
- 对于 `uint32` 来说能表示的最长时间是多少，到什么日期
- 那么对于 `int32` 呢

## 21.2 I2C / SPI / UART / 通信协议基础

### I2C 相关

- `MyI2C_W_SDA(Byte&0x80);` 这样调用时会发生什么
- 为什么 MPU6050 默认地址是 `0x68`，代码里却写 `MyI2C_SendByte(0xD0);`
- `uint8_t MyI2C_ReceiveAck(void){...}` 这个函数有返回值，为什么别的地方可以直接写 `MyI2C_ReceiveAck();` 而不接收返回值
- 读寄存器时最后的 `MyI2C_SendAck(1);` 为什么给的是非应答
- I2C 为什么用开漏
- I2C 的开漏为什么是弱上拉，为什么不能是强上拉
- 弱上拉会有哪些缺点
- 为什么 IIC 的时钟线叫 `SCL`
- 软件实现一个 I2C 通信协议时，是不是两个设备随便找两个 IO 口就可以，因为从始至终是自己设计时序、数据和格式
- STM32 的 I2C 引脚为什么要用“复用开漏”而不是普通“开漏”
- 读寄存器流程里，为什么读的操作没有再次发送寄存器地址
- 仲裁是如何执行的

### SPI 相关

- SPI 的时钟线为什么叫 `SCK`
- 为什么 SPI 输入通常配置成上拉或者浮空
- 什么是高阻态
- 在 I2C 和 SPI 中，设备不传输时会把数据线设为高阻态；那任何输入模式都有可能被配置成高阻态吗
- 对于 MISO，从机既要高阻态输出，又要配置成浮空输入或者上拉输入，对吗
- 也就是说 SPI 主机的配置是输出开漏、输入上拉或者浮空；而从机的两根数据线在接收数据前都是高阻态吗
- 高阻态是怎么做到的，它和从设备把 MISO 配置成输出（如开漏输出）不冲突吗
- `while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01) { ... }` 和 `while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) && timeout--){ break; }` 为什么第二种会出问题
- 主模式全双工连续传输和非连续传输这两种 SPI 传输模式的原理和区别
- `TDR`、`RDR` 和移位寄存器是不是同一个东西
- 连续模式是如何协调输入和输出之间的时序的，会不会出现发送了很多数据但接收很少的情况
- 非连续写法是：

  ```c
  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
  SPI_I2S_SendData(SPI1,ByteSend);

  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
  SPI_I2S_ReceiveData(SPI1);
  ```

  那连续传输应该怎么写

### UART / 术语

- UART 发送时的 `TX` 里，`X` 是什么意思

### 通信协议实现方式

- 通信协议的软件实现和硬件实现有什么区别
- 软件和硬件实现分别实现的是哪一部分，是指协议时序吗，它们“长什么样”

## 21.3 GPIO / 复用 / 电气特性

- 复用电路应该如何配置
- 在 I2C 和 SPI 场景里，输入、开漏、复用开漏、浮空、上拉这些模式分别该怎么理解和选择

## 21.4 存储器 / Flash / 地址 / W25Q64

### Flash 与存储介质

- 常用的内存卡或者 U 盘是不是 Flash
- NAND 是什么意思，为什么它掉电不丢失，为什么其他存储不行
- 为什么可以做到掉电不丢失，掉电丢失又是因为什么
- Flash 只能从 1 写 0，这个说法对吗
- 为什么写入的时候必须先擦除
- NAND 闪存和 NOR 闪存的区别
- NAND 擦写次数较少，那岂不是 U 盘或者 SSD 用不久就坏了

### 地址、页、扇区、块

- 某个芯片的寻址信号是六位的十六进制数，这个芯片的大小是多少，能算吗
- 为什么常说每个地址单元代表 1 字节；有没有可能代表 2 字节或者更多字节；地址单元是不是指寄存器地址；我也见过 16 位寄存器
- 扇区和页是什么
- 块、扇区、页的区别

### W25Q64 / 代码细节

- `void W25Q64_SectorErase(uint32_t Address){ ... MySPI_SwapByte((Address >> 16) & 0xFF); ... }` 这里为什么要 `& 0xFF`

### 传输位宽

- `dual` 和 `quad` 是什么

## 21.5 显示与接口

- TN 屏是什么
- DSI 是什么

## 21.6 VS Code / 补全 / 插件

- 截图里 `Completions – Completions limit reached` 等提示是什么意思
- VS Code 的代码补全是不是只有 Copilot
- ChatGPT 没有代码补全功能吗
- 截图里的这个 `Codex` 好像不是官方的 Codex，这是什么

## 21.7 RTC / BKP / 时间系统

- RTC 和 BKP 是什么
- Unix、GMT、UTC 分别是什么
- `PWR_CR` 寄存器的 `DBP` 位是什么
- `MyRTC_Init / MyRTC_SetTime / MyRTC_ReadTime` 这段代码为什么一直格式报错

## 21.8 PWR / 低功耗 / 电源相关

- `VBT` 是什么
- `VBT` 引脚可以给芯片供电吗
- `PWR` 是什么
- STM32 的睡眠、停止和待机模式分别有什么作用，怎么使用
- 低功耗模式一览图（睡眠 / 停机 / 待机）该怎么理解

## 21.9 中断与事件

- 中断和事件有什么区别，分别是什么
- 如果配置了中断源但是不配置 NVIC，会不会产生事件

## 21.10 代码流程理解

- 这一段硬件 I2C 代码的大概流程是什么：

  ```c
  I2C_GenerateSTART(I2C2, ENABLE);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(I2C2, reg);
  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_GenerateSTART(I2C2, ENABLE);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  I2C_AcknowledgeConfig(I2C2, DISABLE);
  I2C_GenerateSTOP(I2C2, ENABLE);
  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
  data = I2C_ReceiveData(I2C2);

  I2C_AcknowledgeConfig(I2C2, ENABLE);
  return data;
  ```

## 21.11 元问题记录

- 帮我把在这个对话中的所有问题收集起来生成一个 `md` 文件，要求排版简洁，顺序合理
- 帮我把在这个对话中的所有问题收集起来生成一个 `md` 文件，要求排版简洁，顺序合理（再次提出）

# 22. 最后总结（超短背诵版）
- `PC13`：GPIOC 第 13 号引脚
- `Port` 是一组 GPIO，`GPIO` 是单个引脚
- `OLED` 的 `O` = Organic
- `AHB` 管高速主干，`APB` 管普通外设
- STM32F1 里 GPIO 一般在 `APB2`
- 点 LED 常用推挽输出 `GPIO_Mode_Out_PP`
- `ODR` 存输出状态，`BSRR` 更适合单独置位/复位
- `IRQ` 是中断请求，ISR 由硬件自动调用
- 抢占优先级决定能不能打断，子优先级决定谁先响应
- 定时器更新事件 = 一轮计数结束
- `EOC` = ADC 转换完成
- `ADC1->DR` = ADC 结果寄存器
- DMA 硬件触发靠外设事件，软件触发靠程序手动
- `StructInit` 初始化结构体默认值，`DeInit` 复位外设
- `rc_w0` = 可读，写 0 清除
- 串口 `9600 8N1` 是最常见基础配置
- `sizeof(数组)` 返回总字节数
- `uint8_t` 装不下 666，结果会溢出成 154
- 选项字节存芯片配置；读保护防读代码；写保护防改 Flash
