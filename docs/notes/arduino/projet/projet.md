# 温度指示系统项目总览

## 1. 硬件连接

| 功能 | 模块引脚 | Arduino Mega |
| ---- | ---- | ------------ |
| I2C SCL | SCL | **D21** |
| I2C SDA | SDA | **D20** |
| 电机方向 | DIR | **D8** |
| 电机使能（PWM） | EN | **D9** |
| 编码器 A | SA | **D2** |
| 编码器 B | SB | **D3** |

## 2. 项目目标

使用 `Encoder.h` 库做一个“感应温度并驱动指针显示”的系统。

整体原理是：

1. 温度变化时，先读取温度。
2. 再把温度映射成目标角度。
3. 驱动轮转动，通过传动轴带动指针指向对应温度。

项目约束与需求如下：

- 温度范围：`20 ~ 30` 摄氏度
- 指针角度范围：`-180 ~ 180` 度
  `20` 摄氏度和 `30` 摄氏度在同一个角度位置
- 电机没有默认零位，因此必须支持手动校准
- 串口波特率：`9600`
- 控制方式：PI（不用 D）
- 电机减速比：`1 : 52.734`

## 3. 串口命令与输出需求

需要支持以下命令：

- `s`：停止运行；即使温度改变，电机也不跟着变化；默认上电进入 `s` 模式
- `g`：`go`，机器正常运行
- `r`：指针向右偏转一点，用于微调
- `l`：指针向左偏转一点，用于微调
- `r5 / l5`：多转动一点，用于粗调
- `sync`：把当前实际角度校准为当前预期角度

串口输出信息包括：

- 温度传感器测得的温度
- 当前温度对应的预期旋转角度
- 实际旋转角度
- 计数器 `count`

校准示例：

假设当前是 `22.5` 摄氏度，理论上应对应 `-90` 度；系统进入 `go` 后，内部的“实际角度”也认为自己是 `-90` 度，但物理指针却指向了 `25` 摄氏度。那么就需要通过 `l / r` 调整，让指针偏转到正确位置，例如偏到 `-180` 度，再执行 `sync`，把这个 `-180` 映射成当前应该对应的 `-90`。

## 4. STOP / JOG 的思路

对 `STOP` 的理解是：

- 停机不动，电机完全不驱动
- 温度变化时也不跟随
- 只有收到 `r / l / r5 / l5` 时才“动一下”
- 到位后马上停止驱动，不持续保持

因此这里的思路是：

- `STOP` 下完全不输出
- 新增一个 `JOG` 模式
- 只在 jog 时做短暂闭环控制
- 到位后立刻回到 `STOP` 并关闭 PWM

## 5. 相关头文件

```cpp
#include <Wire.h>
#include <Encoder.h>
```

## 6. 已实现的 `r / l / r5 / l5` 代码片段

下面这段代码已经实现了 `r / l / r5 / l5` 的功能：

```cpp
#include <Encoder.h>

const uint8_t PIN_DIR = 8;
const uint8_t PIN_EN  = 9;
const uint8_t PIN_ENC_A = 2;
const uint8_t PIN_ENC_B = 3;

const bool INVERT_MOTOR_DIR = true;
const bool INVERT_ENCODER   = true;

const long ONE_REV_COUNTS = 12;

// zones
const long FAR_ZONE = 8;      // |err| >= 8 -> coarse
const long NEAR_ZONE = 3;     // 3 <= |err| < 8 -> slow coarse
const long FINE_ZONE = 2;     // |err| <= 2 -> fine nudge (try to land on 0)

// tolerance & stability goals
const long TOL_DONE = 0;              // 你要“多数=0”：完成条件设为0
const unsigned long STABLE_MS_ZERO = 500; // 连续500ms保持err==0才结束
const unsigned long STABLE_MS_ONE  = 800; // 如果一直做不到0，允许err=±1稳定800ms也结束（可选兜底）
const bool ALLOW_FALLBACK_PLUSMINUS1 = true;

// coarse P
const float KP_COARSE = 7.0f;
const int PWM_COARSE_MIN_FAST = 60;
const int PWM_COARSE_MAX_FAST = 130;

const int PWM_COARSE_MIN_SLOW = 55;
const int PWM_COARSE_MAX_SLOW = 100;

// nudge
const int PWM_NUDGE = 55;                 // 贴近“能动门槛”
const unsigned long NUDGE_OFF_MS = 160;   // 等待停稳（很关键）

const unsigned long NUDGE_ON_MS_COARSE = 35; // err=2时用
const unsigned long NUDGE_ON_MS_FINE   = 15; // err=1时用（更短，争取落到0）

// control timing
const unsigned long CTRL_MS  = 10;
const unsigned long PRINT_MS = 100;
const unsigned long TIMEOUT_MS = 7000;

Encoder enc(PIN_ENC_A, PIN_ENC_B);

enum State { IDLE, COARSE_FAST, COARSE_SLOW, NUDGE_OFF, NUDGE_ON };
State st = IDLE;

long target = 0;
unsigned long startMs = 0;

unsigned long phaseT0 = 0;
unsigned long stableZeroT0 = 0;
unsigned long stableOneT0  = 0;

String line;

long readCount() {
  long c = enc.read();
  if (INVERT_ENCODER) c = -c;
  return c;
}

void stopMotor() { analogWrite(PIN_EN, 0); }

void setMotor(bool dirHigh, int pwm) {
  if (INVERT_MOTOR_DIR) dirHigh = !dirHigh;
  digitalWrite(PIN_DIR, dirHigh ? HIGH : LOW);
  analogWrite(PIN_EN, pwm);
}

void startMoveBy(long delta) {
  long c = readCount();
  target = c + delta;
  startMs = millis();
  stableZeroT0 = 0;
  stableOneT0 = 0;
  st = COARSE_FAST;
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_EN, OUTPUT);
  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);
  stopMotor();
  Serial.println(F("Hybrid+Fine (aim 0 error) ready. Commands: r l r5 l5 s z"));
}

void loop() {
  while (Serial.available()) {
    char ch = (char)Serial.read();
    if (ch == '\n' || ch == '\r') {
      if (line.length()) {
        line.trim(); line.toLowerCase();
        if (line == "r")  startMoveBy(+ONE_REV_COUNTS);
        if (line == "l")  startMoveBy(-ONE_REV_COUNTS);
        if (line == "r5") startMoveBy(+5 * ONE_REV_COUNTS);
        if (line == "l5") startMoveBy(-5 * ONE_REV_COUNTS);
        if (line == "s") { st = IDLE; stopMotor(); }
        if (line == "z") { enc.write(0); st = IDLE; stopMotor(); target = 0; }
        line = "";
      }
    } else {
      line += ch;
      if (line.length() > 16) line.remove(0, 8);
    }
  }

  static unsigned long lastCtrl = 0;
  unsigned long now = millis();
  if (now - lastCtrl >= CTRL_MS) {
    lastCtrl = now;

    long c = readCount();
    long err = target - c;
    long aerr = labs(err);

    // timeout
    if (st != IDLE && now - startMs > TIMEOUT_MS) {
      st = IDLE;
      stopMotor();
    }

    // stability tracking
    if (st != IDLE) {
      if (err == 0) {
        if (stableZeroT0 == 0) stableZeroT0 = now;
      } else {
        stableZeroT0 = 0;
      }

      if (labs(err) == 1) {
        if (stableOneT0 == 0) stableOneT0 = now;
      } else {
        stableOneT0 = 0;
      }

      // finish condition: prefer true zero
      if (TOL_DONE == 0) {
        if (stableZeroT0 != 0 && now - stableZeroT0 >= STABLE_MS_ZERO) {
          st = IDLE; stopMotor();
        } else if (ALLOW_FALLBACK_PLUSMINUS1 && stableOneT0 != 0 && now - stableOneT0 >= STABLE_MS_ONE) {
          st = IDLE; stopMotor();
        }
      }
    }

    // state machine
    if (st == IDLE) {
      stopMotor();
    }
    else if (st == COARSE_FAST || st == COARSE_SLOW) {
      if (aerr <= FINE_ZONE) {
        st = NUDGE_OFF;
        phaseT0 = now;
        stopMotor();
      } else if (aerr < NEAR_ZONE) {
        st = COARSE_SLOW;
      } else if (aerr < FAR_ZONE) {
        st = COARSE_SLOW;
      } else {
        st = COARSE_FAST;
      }

      if (st == COARSE_FAST || st == COARSE_SLOW) {
        float u = KP_COARSE * (float)err;
        bool dirHigh = (u >= 0);
        int pwm = (int)fabs(u);

        if (st == COARSE_FAST) {
          if (pwm > PWM_COARSE_MAX_FAST) pwm = PWM_COARSE_MAX_FAST;
          if (pwm > 0 && pwm < PWM_COARSE_MIN_FAST) pwm = PWM_COARSE_MIN_FAST;
        } else {
          if (pwm > PWM_COARSE_MAX_SLOW) pwm = PWM_COARSE_MAX_SLOW;
          if (pwm > 0 && pwm < PWM_COARSE_MIN_SLOW) pwm = PWM_COARSE_MIN_SLOW;
        }
        setMotor(dirHigh, pwm);
      }
    }
    else if (st == NUDGE_OFF) {
      stopMotor();
      if (now - phaseT0 >= NUDGE_OFF_MS) {
        st = NUDGE_ON;
        phaseT0 = now;
      }
    }
    else if (st == NUDGE_ON) {
      // choose shorter pulse when |err|==1 to land at 0
      unsigned long onMs = (aerr <= 1) ? NUDGE_ON_MS_FINE : NUDGE_ON_MS_COARSE;

      bool dirHigh = (err > 0);
      setMotor(dirHigh, PWM_NUDGE);

      if (now - phaseT0 >= onMs) {
        st = NUDGE_OFF;
        phaseT0 = now;
      }
    }

    // print
    static unsigned long lastP = 0;
    if (now - lastP >= PRINT_MS) {
      lastP = now;
      Serial.print(F("count=")); Serial.print(c);
      Serial.print(F(" target=")); Serial.print(target);
      Serial.print(F(" err=")); Serial.print(err);
      Serial.print(F(" state="));
      switch (st) {
        case IDLE: Serial.println(F("IDLE")); break;
        case COARSE_FAST: Serial.println(F("COARSE_FAST")); break;
        case COARSE_SLOW: Serial.println(F("COARSE_SLOW")); break;
        case NUDGE_OFF: Serial.println(F("NUDGE_OFF")); break;
        case NUDGE_ON: Serial.println(F("NUDGE_ON")); break;
      }
    }
  }
}
```
