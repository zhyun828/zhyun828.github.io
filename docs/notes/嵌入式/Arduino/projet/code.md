#include <Wire.h>
#include <Encoder.h>

/*
  ====================== 硬件连接说明（Arduino Mega2560）======================
  I2C SDA -> D20
  I2C SCL -> D21
  电机方向 DIR -> D8
  电机使能 EN(PWM) -> D9   （PWM 控制电机速度/力矩，0~255）
  编码器 A -> D2
  编码器 B -> D3
  ===========================================================================

  本程序目标：
  1) 读取温度传感器（MPL3115A2）的温度 T
  2) 把温度映射到“指针轴预期角度 expectedDeg”
  3) 通过编码器读数得到“指针轴实际角度 actualDeg”
  4) GO 模式下：电机自动转动，使 actualDeg 逼近 expectedDeg
  5) STOP 模式下：电机不动（即使温度变化也不跟随）
  6) 串口命令：
     s：STOP（停止跟随、停止电机）
     g：GO（开始跟随温度）
     r / l：驱动轮转 1 圈（±12 count）
     r5 / l5：驱动轮转 5 圈（±60 count）
     sync：校准（把当前指针所指位置当成“正确位置”，修正 offset）
*/

// ---------------- 引脚定义 ----------------
const uint8_t PIN_DIR   = 8;   // 电机方向
const uint8_t PIN_EN    = 9;   // 电机使能（PWM）
const uint8_t PIN_ENC_A = 2;   // 编码器 A
const uint8_t PIN_ENC_B = 3;   // 编码器 B

// ---------------- 方向反转选项 ----------------
// 如果你发现：
// - r 指令实际向左转，或 GO 跟随方向相反
// 可以通过修改这两个布尔值快速修正方向，不用重新接线
const bool INVERT_MOTOR_DIR = true;  // 反转电机方向信号
const bool INVERT_ENCODER   = true;  // 反转编码器计数方向（count 正负）

// ---------------- 机械参数（很关键） ----------------
// 实测：驱动轮转 1 圈，编码器 count 变化为 12
const long  ONE_REV_COUNTS = 12;

// 减速比：驱动轮转 GEAR_RATIO 圈，指针轴转 1 圈
// drive : shaft = 52.734 : 1
const float GEAR_RATIO = 52.734f;

/*
  degPerDriveCountOnShaft():
  计算“驱动轮编码器每增加 1 count，指针轴转过多少度”
  推导：
  - 驱动轮 1 圈 = 12 count
  - 驱动轮转 52.734 圈，指针轴转 360°
  因此：
  1 count 对应的指针轴角度 = 360 / (12 * 52.734)
*/
static inline float degPerDriveCountOnShaft() {
  return 360.0f / (float)ONE_REV_COUNTS / GEAR_RATIO;
}

/*
  wrap180(a):
  把任意角度归一化到 (-180, 180] 区间
  角度是周期量，例如：
  190° 等价于 -170°
  540° 等价于 180°
  归一化的目的：
  - 避免角度计算越界
  - 避免控制器走“绕大圈”的路径
*/
static inline float wrap180(float a) {
  while (a > 180.0f) a -= 360.0f;
  while (a <= -180.0f) a += 360.0f;
  return a;
}

/*
  shaftDegToDriveCount(deg):
  把“指针轴角度(°)”换算成“驱动轮编码器 count”
  注意：这里只做纯比例换算，不包含校准 offset（offset 在别处处理）
*/
static inline long shaftDegToDriveCount(float deg) {
  return lroundf(deg / degPerDriveCountOnShaft());
}

/*
  driveCountToShaftDeg(countDrive, calibOffsetDeg):
  把“驱动轮编码器 count”换算成“指针轴角度(°)”，并加上校准偏移 calibOffsetDeg
  模型：
  actualDeg = count * (degPerCount) + calibOffsetDeg
  再 wrap180 折回到 (-180,180] 区间
*/
static inline float driveCountToShaftDeg(long countDrive, float calibOffsetDeg) {
  return wrap180((float)countDrive * degPerDriveCountOnShaft() + calibOffsetDeg);
}

// ---------------- 温度传感器（MPL3115A2）I2C 部分 ----------------
// MPL3115A2 常用 I2C 地址 0x60
const uint8_t MPL3115A2_ADDR = 0x60;

/*
  i2cWrite8(addr, reg, val):
  向 I2C 设备的 reg 寄存器写入 1 个字节 val
  返回 true 表示写入成功
*/
bool i2cWrite8(uint8_t addr, uint8_t reg, uint8_t val) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  return (Wire.endTransmission() == 0);
}

/*
  i2cReadBytes(addr, reg, buf, len):
  从 I2C 设备的 reg 寄存器开始，连续读取 len 个字节，放入 buf
  返回 true 表示读取成功
*/
bool i2cReadBytes(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  // endTransmission(false) = 不释放总线（重复启动），适合“先写寄存器地址再读数据”
  if (Wire.endTransmission(false) != 0) return false;

  uint8_t got = Wire.requestFrom(addr, len);
  if (got != len) return false;

  for (uint8_t i = 0; i < len; i++) buf[i] = Wire.read();
  return true;
}

/*
  mpl3115a2Init():
  初始化 MPL3115A2 使其进入温度测量模式
  - PT_DATA_CFG(0x13) 写 0x07：使能事件标志等功能
  - CTRL_REG1(0x26) 写 0xB9：设置过采样并置为 Active（开始测量）
*/
bool mpl3115a2Init() {
  if (!i2cWrite8(MPL3115A2_ADDR, 0x13, 0x07)) return false;
  if (!i2cWrite8(MPL3115A2_ADDR, 0x26, 0xB9)) return false;
  delay(10);
  return true;
}

/*
  readTemperatureC(tempC):
  读取温度，返回摄氏度 float
  MPL3115A2 温度寄存器从 0x04 开始：
  - buf[0] = 温度整数部分（有符号）
  - buf[1] 高 4 位为小数（单位 1/16°C）
*/
bool readTemperatureC(float &tempC) {
  uint8_t buf[2];
  if (!i2cReadBytes(MPL3115A2_ADDR, 0x04, buf, 2)) return false;

  int8_t msb = (int8_t)buf[0];
  uint8_t lsb = buf[1];

  float frac = (float)(lsb >> 4) / 16.0f;      // 4bit 小数
  tempC = (float)msb + ((msb >= 0) ? frac : -frac);
  return true;
}

// ---------------- 温度 -> 预期角度 映射 ----------------
// 温度量程：20~30 °C
const float TEMP_MIN = 20.0f;
const float TEMP_MAX = 30.0f;

// 对应角度量程：-120~+120 °（指针轴角度）
const float ANGLE_MIN = -120.0f;
const float ANGLE_MAX =  120.0f;

/*
  tempToAngleDeg(t):
  把温度 t（°C）线性映射到指针轴角度（°）
  - 温度低于 TEMP_MIN：夹到 TEMP_MIN
  - 温度高于 TEMP_MAX：夹到 TEMP_MAX
  映射公式：
  a = ANGLE_MIN + (t - TEMP_MIN) * (ANGLE_MAX - ANGLE_MIN)/(TEMP_MAX - TEMP_MIN)
*/
static inline float tempToAngleDeg(float t) {
  if (t < TEMP_MIN) t = TEMP_MIN;
  if (t > TEMP_MAX) t = TEMP_MAX;

  float a = ANGLE_MIN + (t - TEMP_MIN) * (ANGLE_MAX - ANGLE_MIN) / (TEMP_MAX - TEMP_MIN);
  return wrap180(a);
}

// ---------------- 电机控制函数 ----------------

/*
  stopMotor():
  让 PWM=0，电机停止（断电）
  注意：这是“滑行停”，不是刹车
*/
void stopMotor() { analogWrite(PIN_EN, 0); }

/*
  setMotor(dirHigh, pwm):
  设置电机方向和 PWM 输出
  - dirHigh: 逻辑方向（true/false）
  - pwm: 0~255
  INVERT_MOTOR_DIR 可以用来反转方向
*/
void setMotor(bool dirHigh, int pwm) {
  if (INVERT_MOTOR_DIR) dirHigh = !dirHigh;
  digitalWrite(PIN_DIR, dirHigh ? HIGH : LOW);
  analogWrite(PIN_EN, pwm);
}

// ---------------- 编码器对象与读取 ----------------
Encoder enc(PIN_ENC_A, PIN_ENC_B);

/*
  readCountDrive():
  读取编码器计数（驱动轮侧）
  INVERT_ENCODER 用于反转 count 正负
*/
long readCountDrive() {
  long c = enc.read();
  if (INVERT_ENCODER) c = -c;
  return c;
}

// ---------------- 运动控制器（你验证“十分完美”的部分） ----------------
/*
  控制思想：分两段
  1) COARSE（粗调）：用比例控制 P，把误差快速拉近（连续输出 PWM）
  2) NUDGE（微调）：靠短脉冲一点点推，尽量落到 err=0（并在脉冲间等待停稳）

  zones:
  FAR_ZONE: 误差很大 -> 快档粗调
  FINE_ZONE: 误差很小 -> 进入微调（nudge）
*/
const long FAR_ZONE  = 8;
const long NEAR_ZONE = 3; // 此版本中主要用于概念，实际判断用 FAR_ZONE/FINE_ZONE
const long FINE_ZONE = 2;

// 稳定判定：
// err==0 连续 500ms -> 认为到位
// err==±1 连续 800ms -> 可选兜底（允许少量误差）
const long TOL_DONE = 0;
const unsigned long STABLE_MS_ZERO = 500;
const unsigned long STABLE_MS_ONE  = 800;
const bool ALLOW_FALLBACK_PLUSMINUS1 = true;

// 粗调比例系数（P 控制）
const float KP_COARSE = 7.0f;

// PWM 限幅：保证“能动 + 不过猛”
const int PWM_COARSE_MIN_FAST = 60;
const int PWM_COARSE_MAX_FAST = 130;

const int PWM_COARSE_MIN_SLOW = 55;
const int PWM_COARSE_MAX_SLOW = 100;

// 微调 nudge：靠短脉冲推一下，然后等待
const int PWM_NUDGE = 55;
const unsigned long NUDGE_OFF_MS = 160;     // 脉冲之间等待停稳（很关键）
const unsigned long NUDGE_ON_MS_COARSE = 35; // err=2 时脉冲稍长
const unsigned long NUDGE_ON_MS_FINE   = 15; // err=1 时脉冲更短，争取落到 0

// 调度周期/打印周期/超时保护
const unsigned long CTRL_MS   = 10;   // 控制周期 10ms
const unsigned long PRINT_MS  = 200;  // 串口打印周期 200ms
const unsigned long TIMEOUT_MS = 7000;

// 运动状态机
enum MotionState { M_IDLE, M_COARSE_FAST, M_COARSE_SLOW, M_NUDGE_OFF, M_NUDGE_ON };
MotionState mstate = M_IDLE;

// 运动目标（驱动轮编码器 count）
long targetCountDrive = 0;

// 运动开始时间，用于超时
unsigned long moveStartMs = 0;

// nudge 相位起始时间
unsigned long phaseT0 = 0;

// 用于稳定判定的计时器
unsigned long stableZeroT0 = 0;
unsigned long stableOneT0  = 0;

/*
  motionStartTo(newTargetCount):
  启动一次运动，让编码器 count 走到 newTargetCount
*/
void motionStartTo(long newTargetCount) {
  targetCountDrive = newTargetCount;
  moveStartMs = millis();
  stableZeroT0 = 0;
  stableOneT0  = 0;
  mstate = M_COARSE_FAST;
}

/*
  motionMoveBy(delta):
  相对运动：在当前 count 基础上 +delta
  用于 r/l/r5/l5 命令
*/
void motionMoveBy(long delta) {
  long c = readCountDrive();
  motionStartTo(c + delta);
}

/*
  motionUpdate():
  运动控制器核心状态机
  每 CTRL_MS 调用一次：
  - 计算 err
  - 粗调 / 微调
  - 输出 PWM
  - 到位后进入 IDLE
*/
void motionUpdate() {
  unsigned long now = millis();

  long c = readCountDrive();
  long err = targetCountDrive - c;
  long aerr = labs(err);

  // 1) 超时保护：防止编码器失效一直转
  if (mstate != M_IDLE && now - moveStartMs > TIMEOUT_MS) {
    mstate = M_IDLE;
    stopMotor();
  }

  // 2) 稳定判定：err==0 或 err==±1 持续一定时间，认为到位
  if (mstate != M_IDLE) {
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

    if (TOL_DONE == 0) {
      if (stableZeroT0 != 0 && now - stableZeroT0 >= STABLE_MS_ZERO) {
        mstate = M_IDLE; stopMotor();
      } else if (ALLOW_FALLBACK_PLUSMINUS1 && stableOneT0 != 0 && now - stableOneT0 >= STABLE_MS_ONE) {
        mstate = M_IDLE; stopMotor();
      }
    }
  }

  // 3) 状态机输出
  if (mstate == M_IDLE) {
    stopMotor();
    return;
  }

  // ---- 粗调阶段：连续 PWM，快速逼近 ----
  if (mstate == M_COARSE_FAST || mstate == M_COARSE_SLOW) {

    // 误差很小 -> 进入微调
    if (aerr <= FINE_ZONE) {
      mstate = M_NUDGE_OFF;
      phaseT0 = now;
      stopMotor();
    }
    // 误差中等 -> 慢档粗调
    else if (aerr < FAR_ZONE) {
      mstate = M_COARSE_SLOW;
    }
    // 误差很大 -> 快档粗调
    else {
      mstate = M_COARSE_FAST;
    }

    // 在粗调状态下：计算 P 控制输出 u=Kp*err，并转成 PWM
    if (mstate == M_COARSE_FAST || mstate == M_COARSE_SLOW) {
      float u = KP_COARSE * (float)err;
      bool dirHigh = (u >= 0);
      int pwm = (int)fabs(u);

      // PWM 限幅：确保能动、避免过猛
      if (mstate == M_COARSE_FAST) {
        if (pwm > PWM_COARSE_MAX_FAST) pwm = PWM_COARSE_MAX_FAST;
        if (pwm > 0 && pwm < PWM_COARSE_MIN_FAST) pwm = PWM_COARSE_MIN_FAST;
      } else {
        if (pwm > PWM_COARSE_MAX_SLOW) pwm = PWM_COARSE_MAX_SLOW;
        if (pwm > 0 && pwm < PWM_COARSE_MIN_SLOW) pwm = PWM_COARSE_MIN_SLOW;
      }

      setMotor(dirHigh, pwm);
    }
  }

  // ---- 微调阶段：OFF 等待 ----
  else if (mstate == M_NUDGE_OFF) {
    stopMotor();
    // 等待一段时间让机械停稳，再打下一次脉冲
    if (now - phaseT0 >= NUDGE_OFF_MS) {
      mstate = M_NUDGE_ON;
      phaseT0 = now;
    }
  }

  // ---- 微调阶段：ON 脉冲 ----
  else if (mstate == M_NUDGE_ON) {
    // err=1 时脉冲更短，避免过冲，争取落到 err=0
    unsigned long onMs = (aerr <= 1) ? NUDGE_ON_MS_FINE : NUDGE_ON_MS_COARSE;

    bool dirHigh = (err > 0);
    setMotor(dirHigh, PWM_NUDGE);

    // 脉冲时间到 -> 回到 OFF 等待
    if (now - phaseT0 >= onMs) {
      mstate = M_NUDGE_OFF;
      phaseT0 = now;
    }
  }
}

// ---------------- GO/STOP 模式与串口命令 ----------------
enum Mode { MODE_STOP, MODE_GO };
Mode mode = MODE_STOP;  // 上电默认 STOP：不跟随温度

String cmdLine;

// 校准偏移：解决“电机没有绝对0角度”的问题
// sync 时会更新该值，让当前指针对应当前温度的 expectedDeg
float calibOffsetDeg = 0.0f;

// 预期角度（由温度映射得到）与实际角度（由编码器换算得到）
float expectedDeg = 0.0f;
float actualDeg   = 0.0f;

// GO 模式下把 expectedDeg 换算成目标 count
long expectedCountDrive = 0;

/*
  expectedDegToTargetCount(expDeg):
  将预期角度 expDeg 转成目标 count（驱动轮编码器）
  注意：必须考虑 calibOffsetDeg
  模型：
  actualDeg = count*degPer + offset
  => count = (expDeg - offset)/degPer
*/
long expectedDegToTargetCount(float expDeg) {
  float pure = wrap180(expDeg - calibOffsetDeg);
  return lroundf(pure / degPerDriveCountOnShaft());
}

/*
  handleCommand(s):
  处理串口命令：
  - s：STOP
  - g：GO
  - r/l/r5/l5：相对运动（用于微调）
  - sync：校准 offset（把当前指针位置认为是正确位置）
*/
void handleCommand(String s) {
  s.trim();
  s.toLowerCase();

  if (s == "s") {
    mode = MODE_STOP;
    mstate = M_IDLE;   // 取消正在执行的运动
    stopMotor();
    return;
  }
  if (s == "g") {
    mode = MODE_GO;
    return;
  }

  // 手动调节：驱动轮转动固定圈数（以 count 精准控制）
  if (s == "r")  { motionMoveBy(+ONE_REV_COUNTS); return; }
  if (s == "l")  { motionMoveBy(-ONE_REV_COUNTS); return; }
  if (s == "r5") { motionMoveBy(+5 * ONE_REV_COUNTS); return; }
  if (s == "l5") { motionMoveBy(-5 * ONE_REV_COUNTS); return; }

  if (s == "sync") {
    // 校准逻辑：
    // 若 expectedDeg 是“应该指向的位置”，actualDeg 是“当前实际指向的位置”
    // 那么它们之间差值 err = expected - actual
    // 把这个差值加到 offset 上，就能让“当前实际”被当成“正确”
    float err = wrap180(expectedDeg - actualDeg);
    calibOffsetDeg = wrap180(calibOffsetDeg + err);
    return;
  }
}

// ---------------- Arduino setup/loop ----------------
void setup() {
  Serial.begin(9600);

  // 引脚模式配置
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_EN, OUTPUT);
  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);

  stopMotor();

  // I2C 初始化 + 传感器初始化
  Wire.begin();
  (void)mpl3115a2Init();

  // 上电时先读一次编码器，计算初始实际角度
  long c = readCountDrive();
  actualDeg = driveCountToShaftDeg(c, calibOffsetDeg);

  // 串口提示信息（便于调试/报告）
  Serial.println(F("Temp Pointer Ready. Default STOP."));
  Serial.println(F("Commands: s(stop) g(go) r l r5 l5 sync"));
  Serial.print(F("deg/count(shaft)="));
  Serial.println(degPerDriveCountOnShaft(), 6);
}

void loop() {
  // 1) 串口行读取：接收命令（以回车/换行结束）
  while (Serial.available()) {
    char ch = (char)Serial.read();
    if (ch == '\n' || ch == '\r') {
      if (cmdLine.length()) {
        handleCommand(cmdLine);
        cmdLine = "";
      }
    } else {
      cmdLine += ch;
      if (cmdLine.length() > 64) cmdLine.remove(0, 32); // 防止字符串过长
    }
  }

  // 2) 每 CTRL_MS 执行一次控制逻辑（非阻塞）
  static unsigned long lastCtrl = 0;
  unsigned long now = millis();
  if (now - lastCtrl >= CTRL_MS) {
    lastCtrl = now;

    // 2.1 读取温度，得到 expectedDeg（预期角度）
    float tempC = 0.0f;
    bool tOk = readTemperatureC(tempC);
    if (tOk) expectedDeg = tempToAngleDeg(tempC);

    // 2.2 读取编码器，得到 actualDeg（实际角度）
    long countDrive = readCountDrive();
    actualDeg = driveCountToShaftDeg(countDrive, calibOffsetDeg);

    // 2.3 GO 模式：跟随温度
    if (mode == MODE_GO) {
      expectedCountDrive = expectedDegToTargetCount(expectedDeg);

      // 只有目标变化时才启动一次新的运动任务
      // 目的：避免每个控制周期都重置状态机造成抖动
      if (expectedCountDrive != targetCountDrive) {
        motionStartTo(expectedCountDrive);
      }
    }

    // 2.4 STOP 模式：如果没有运动任务，确保电机断电
    // 注意：STOP 模式下仍允许 r/l/r5/l5 手动触发 motionStartTo()
    if (mode == MODE_STOP && mstate == M_IDLE) {
      stopMotor();
    }

    // 2.5 执行运动控制状态机（若有目标则驱动电机）
    motionUpdate();

    // 3) 每 PRINT_MS 打印一次状态（用于调试/报告记录）
    static unsigned long lastPrint = 0;
    if (now - lastPrint >= PRINT_MS) {
      lastPrint = now;

      Serial.print(F("T="));
      if (tOk) Serial.print(tempC, 2);
      else Serial.print(F("NaN"));

      Serial.print(F("; expected="));
      Serial.print(expectedDeg, 1);

      Serial.print(F("; actual="));
      Serial.print(actualDeg, 1);

      Serial.print(F("; count="));
      Serial.print(countDrive);

      Serial.print(F("; mode="));
      Serial.println(mode == MODE_GO ? F("GO") : F("STOP"));
    }
  }
}

下面按“函数/模块”为单位，把你这份代码里**每一个函数**（含 inline 小函数）都解释清楚。你写报告时可以直接用这些描述（我用的是偏工程报告风格：目的、输入输出、实现要点）。

---
# 大纲
## 1. 机械与数学辅助函数

### `degPerDriveCountOnShaft()`

**作用**：计算“驱动轮编码器 1 个 count 对应指针轴转过的角度（°）”。
**原理**：驱动轮 1 圈 = 12 count；驱动轮转 52.734 圈，指针轴转 1 圈（360°）。
[
\text{deg/count} = \frac{360}{12 \times 52.734}
]
**返回值**：`float`，单位 °/count。
**用途**：把 `count` 换算成轴角度，或把轴角度换算成 count。

---

### `wrap180(float a)`

**作用**：把任意角度归一化到区间 (-180, 180]（或等价区间）。
**输入**：`a` 任意角度（可能超过 360 或小于 -360）。
**输出**：归一化后的角度。
**用途**：避免角度越界，且处理“20°C 与 30°C 对应同一个角度”的环绕关系（±180 等价）。

---

### `shaftDegToDriveCount(float deg)`

**作用**：把“指针轴角度（°）”换算成“驱动轮编码器 count”。
**输入**：`deg`（不包含校准 offset 的纯角度映射）。
**输出**：`long` 类型 count（四舍五入）。
**用途**：理论上可以用来直接指定某个轴角对应的 count（在你这版里实际用得不多，你主要用 `expectedDegToTargetCount()`）。

---

### `driveCountToShaftDeg(long countDrive, float calibOffsetDeg)`

**作用**：把编码器 `countDrive` 转换成“指针轴实际角度 actualDeg”，并叠加校准偏移 `calibOffsetDeg`。
**输入**：

* `countDrive`：驱动轮编码器计数（每圈约 12）
* `calibOffsetDeg`：校准偏移（用 `sync` 调整）
  **输出**：`actualDeg`（单位 °，范围 wrap 到 (-180, 180]）。
  **用途**：报告里可以写：用编码器估算输出轴角度，实现“实际角度反馈”。

---

## 2. I2C 读写与温度传感器相关函数（MPL3115A2）

### `i2cWrite8(uint8_t addr, uint8_t reg, uint8_t val)`

**作用**：向 I2C 设备的某个寄存器写 1 字节。
**输入**：

* `addr`：I2C 设备地址（MPL3115A2 是 `0x60`）
* `reg`：寄存器地址
* `val`：要写入的值
  **输出**：`bool`，返回写入是否成功（`Wire.endTransmission()==0` 代表成功）。
  **用途**：给传感器配置寄存器。

---

### `i2cReadBytes(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len)`

**作用**：从 I2C 设备的某个寄存器开始，连续读取 `len` 个字节。
**输入**：

* `addr`：设备地址
* `reg`：起始寄存器地址
* `buf`：缓冲区指针，用来存放读取的数据
* `len`：读取长度
  **输出**：`bool`，是否读取成功。
  **实现要点**：先写寄存器地址（但不释放总线 `endTransmission(false)`），再 `requestFrom()` 读数据。
  **用途**：读取温度寄存器的数据。

---

### `mpl3115a2Init()`

**作用**：初始化 MPL3115A2，使其进入温度测量工作状态。
**核心配置**：

* 向 `PT_DATA_CFG(0x13)` 写 `0x07`：启用事件/数据就绪等功能（课程里常用写法）
* 向 `CTRL_REG1(0x26)` 写 `0xB9`：设置输出采样/过采样并置为 Active（开始测量）
  **输出**：`bool`，是否初始化成功。
  **用途**：在 `setup()` 中调用，保证传感器可以正常输出温度。

---

### `readTemperatureC(float &tempC)`

**作用**：读取传感器温度并换算成摄氏度浮点数。
**输入/输出**：

* 参数 `tempC` 为引用输出：返回测得温度（°C）
* 返回值 `bool`：读取成功与否
  **实现要点**：
* 从寄存器 `0x04` 读 2 字节：MSB 是整数部分（有符号），LSB 的高 4 位是 1/16°C 的小数部分
* 组合得到温度：`temp = msb + (lsb>>4)/16`
  **用途**：在主循环中实时获取当前温度 T。

---

## 3. 温度到角度映射函数

### `tempToAngleDeg(float t)`

**作用**：把温度范围 20–30°C 映射到指针轴角度 (-180°, 180°]，并让 20°C 与 30°C 映射到同一角度（-180°）。
**输入**：`t` 温度（°C）。
**输出**：对应角度（°）。
**映射关系**：

* `t=20 → -180°`
* `t=25 → 0°`
* `t=30 → +180°`，但你要求 30 和 20 同角，所以把 `+180°` 折回 `-180°`
  **用途**：生成“预期角度 expectedDeg”。

---

## 4. 电机驱动输出函数

### `stopMotor()`

**作用**：停止电机输出。
**实现**：`analogWrite(PIN_EN, 0)`，PWM=0。
**用途**：STOP 模式、运动结束、等待阶段等都用它让电机断电。

---

### `setMotor(bool dirHigh, int pwm)`

**作用**：设置电机转向与 PWM，占空比控制速度/扭矩。
**输入**：

* `dirHigh`：逻辑方向（正/反）
* `pwm`：PWM 值 0–255
  **实现要点**：
* 根据 `INVERT_MOTOR_DIR` 反转方向（用于修正接线导致的方向反）
* `digitalWrite(PIN_DIR, ...)` 设置方向
* `analogWrite(PIN_EN, pwm)` 输出 PWM
  **用途**：运动控制中真正驱动电机的唯一出口。

---

## 5. 编码器读取函数

### `readCountDrive()`

**作用**：读取 Encoder 库的计数值，并根据 `INVERT_ENCODER` 修正计数方向。
**输出**：`long` 编码器计数（驱动轮每圈约 12）。
**用途**：作为反馈量 `countDrive`，既用于 report 的“count”，也用于计算 actualDeg。

---

## 6. 运动控制模块（你最核心的“完美”部分）

### `motionStartTo(long newTargetCount)`

**作用**：启动一次运动任务：让驱动轮 count 走到 `newTargetCount`。
**输入**：目标计数 `newTargetCount`。
**实现要点**：

* 设置 `targetCountDrive`
* 记录开始时间 `moveStartMs` 用于超时保护
* 清零稳定计时 `stableZeroT0 / stableOneT0`
* 把状态机置为 `M_COARSE_FAST`（先粗调再精调）
  **用途**：
* GO 模式温度目标变化时调用
* 手动 r/l/r5/l5 也会间接调用它

---

### `motionMoveBy(long delta)`

**作用**：相对运动：以当前 count 为起点，目标变为 `当前 + delta`。
**输入**：`delta`（例如 `+12`、`-12`、`+60`、`-60`）。
**用途**：实现 r/l/r5/l5。

---

### `motionUpdate()`

**作用**：运动控制核心状态机，每 10ms 调用一次，负责：

* 读取当前 count
* 计算误差 `err = targetCountDrive - count`
* 根据误差大小决定处于粗调还是微调
* 输出电机 PWM，直到满足稳定结束条件
  **主要流程**：

1. **超时保护**
   如果运动时间超过 `TIMEOUT_MS`，强制停止，避免编码器失效导致一直转。

2. **稳定判定（结束条件）**

* 若 `err==0` 持续 `STABLE_MS_ZERO(500ms)` → 完成
* 若 `err==±1` 持续 `STABLE_MS_ONE(800ms)` → 作为兜底完成
  （这就是你“多数 0，偶尔 ±1”的策略来源）

3. **状态机控制**

* `M_COARSE_FAST / M_COARSE_SLOW`：比例控制 `u = KP_COARSE * err`，并用 PWM 上下限保证能动且不过冲过大

  * 若误差进入 `FINE_ZONE`（≤2）→ 转入 `M_NUDGE_OFF`
* `M_NUDGE_OFF`：断电等待 `NUDGE_OFF_MS` 让机械停稳/消除回弹
* `M_NUDGE_ON`：输出短脉冲 `NUDGE_ON_MS_FINE/COARSE` 微调 1 count，尽量落在 0

**用途**：这是你实现“精确 count”的关键，因此写报告时建议重点描述：粗调+微调两阶段控制思想。

---

## 7. GO/STOP 与校准（sync）相关函数

### `expectedDegToTargetCount(float expDeg)`

**作用**：把“预期轴角 expectedDeg”转换成“目标驱动轮 count”。
**输入**：`expDeg`（来自温度映射）。
**输出**：对应 `count`（long）。
**实现要点**：

* 你有一个校准偏移 `calibOffsetDeg`：
  轴角模型：`actualDeg = count*degPer + calibOffsetDeg`
* 所以要得到某个轴角对应的 count，反解：
  [
  count = \frac{expDeg - calibOffsetDeg}{degPerCount}
  ]
* 代码里先 `pure = wrap180(expDeg - calibOffsetDeg)` 再除以 degPerCount
  **用途**：GO 模式把温度→角度→count，用于运动控制目标。

---

### `handleCommand(String s)`

**作用**：解析串口指令，实现你要求的交互逻辑。
**支持命令**：

* `s`：进入 STOP，立即停止电机，取消当前运动（`mstate=M_IDLE`）
* `g`：进入 GO，开始跟随温度
* `r/l/r5/l5`：调用 `motionMoveBy()` 做相对运动
* `sync`：校准：让当前指针“实际角度”被视作“预期角度”

  * 计算 `err = expectedDeg - actualDeg`
  * 更新 `calibOffsetDeg += err`
    **用途**：这是系统的“人机接口”，报告里可以单独一节写“串口命令与校准流程”。

---

## 8. Arduino 主结构函数

### `setup()`

**作用**：系统初始化。
**主要内容**：

* `Serial.begin(9600)`：设置波特率
* 设置电机引脚/编码器引脚模式，初始停止电机
* `Wire.begin()` 初始化 I2C
* `mpl3115a2Init()` 初始化温度传感器
* 初始读取 `count` 并计算 `actualDeg`
* 串口打印提示与 deg/count 便于调试/写报告
  **用途**：上电准备工作。

---

### `loop()`

**作用**：主循环，包含三件事：串口输入、周期控制、串口输出。
**结构**：

1. **串口命令接收**
   把一行命令存进 `cmdLine`，遇到 `\n/\r` 就调用 `handleCommand()`。

2. **按 CTRL_MS 周期执行控制**（你设置 10ms）

* 读温度 → `expectedDeg`
* 读编码器 → `countDrive`
* 计算 `actualDeg`
* 若 `mode == GO`：

  * 把 `expectedDeg → expectedCountDrive`
  * 如果目标改变：`motionStartTo(expectedCountDrive)`（启动/刷新运动目标）
* STOP 模式下若没有运动：确保电机断电
* 调用 `motionUpdate()` 执行运动状态机

3. **按 PRINT_MS 周期打印状态**（你设置 200ms）
   输出：

* `T=...`
* `expected=...`（预期轴角）
* `actual=...`（实际轴角）
* `count=...`（驱动轮 count）
* `mode=GO/STOP`

---

## 9. 变量/状态（报告常用解释点）


* `mode`：STOP/GO 工作模式（上电默认 STOP）
* `mstate`：运动控制状态机（粗调快/粗调慢/等待/脉冲/空闲）
* `calibOffsetDeg`：校准偏移（sync 后更新，用于修正“电机无绝对零位”）
* `expectedDeg`：温度映射得到的预期指针轴角度
* `actualDeg`：编码器反馈换算得到的实际轴角度
* `targetCountDrive`：运动控制的目标计数（驱动轮 count）
* `expectedCountDrive`：GO 模式下由 expectedDeg 计算出的目标 count

---

# 第 1 大段

## 第 1 段：头文件（库）
#include <Wire.h>
#include <Encoder.h>


#include <Wire.h>：✅ Arduino 官方自带库，用来操作 I2C 总线（你 Mega 的 D20/D21）。

你会用到的函数：Wire.begin()、Wire.beginTransmission()、Wire.write()、Wire.endTransmission()、Wire.requestFrom()、Wire.read() 等。

#include <Encoder.h>：✅ 第三方 Encoder 库（PJRC 常用那个），用于读取增量编码器 A/B 相。

你会用到：Encoder enc(pinA,pinB)、enc.read()、enc.write()（你这版没用 write）。

结论：这两行都是“库提供”，不是你自己写的功能。

## 第 2 段：硬件连接注释（只是注释，不参与编译）
/*
  Mega2560 Pin map:
  I2C SDA -> D20
  I2C SCL -> D21
  Motor DIR -> D8
  Motor EN(PWM) -> D9
  Encoder A -> D2
  Encoder B -> D3
*/


只是说明接线，不会影响程序。

其中 “I2C SDA/SCL” 这两个引脚是 Mega 固定的 I2C 引脚：D20/D21。

## 第 3 段：引脚常量（自己定义的）

const uint8_t PIN_DIR   = 8;
const uint8_t PIN_EN    = 9;
const uint8_t PIN_ENC_A = 2;
const uint8_t PIN_ENC_B = 3;


这些是✅ 你自己定义的常量（只是为了代码可读性）。

PIN_DIR：电机方向控制脚（D8）

PIN_EN：电机 PWM 使能脚（D9）

PIN_ENC_A/B：编码器 A/B 相（D2/D3）

uint8_t 是 C/C++ 的标准类型（Arduino 也有），表示 0~255 的无符号整数。

## 第 4 段：方向反转开关（自己定义的“调试开关”）
const bool INVERT_MOTOR_DIR = true;
const bool INVERT_ENCODER   = true;


这也是✅ 你自己定义的常量（不是库自带）。

为什么要有它？

你接线/安装方向可能导致：

电机正反方向跟你想的相反

编码器计数正负号相反
所以我们用这两个开关“软件纠正”，不用改接线。

它们在哪用？

INVERT_MOTOR_DIR：在 setMotor() 里决定是否 dirHigh = !dirHigh;

INVERT_ENCODER：在 readCountDrive() 里决定是否 c = -c;

## 第 5 段：机械参数（自己定义，来自你的实测与结构）
const long  ONE_REV_COUNTS = 12;
const float GEAR_RATIO = 52.734f;


这两个是✅ 你自己定义的常量，不是库自带。

ONE_REV_COUNTS = 12：你实测“驱动轮转 1 圈，Encoder 输出变化 12 count”。

GEAR_RATIO = 52.734：你的减速比（驱动轮 : 指针轴 = 52.734 : 1）

注意：这两个是整个“角度换算”正确与否的根。

## 第 6 段：每个 count 对应指针轴多少度（自己写的 inline 函数）
static inline float degPerDriveCountOnShaft() {
  return 360.0f / (float)ONE_REV_COUNTS / GEAR_RATIO;
}


这是✅ 你自己（我们）写的函数，不是库自带。

static inline 的意思：

inline：建议编译器把函数展开（更快）

static：限制作用域在当前文件（避免链接冲突）

它做的事：建立“count → 指针轴角度”的比例尺
推导：

驱动轮 1 圈 = 12 count

驱动轮转 52.734 圈 → 指针轴转 1 圈 = 360°

所以 1 count → 指针轴角度：这个函数会被两个关键地方用到：

driveCountToShaftDeg()：count → 实际角度 actualDeg

expectedDegToTargetCount()：预期角度 expectedDeg → 目标 count

## 第 7 段：角度归一化（自己写的函数）
static inline float wrap180(float a) {
  while (a > 180.0f) a -= 360.0f;
  while (a <= -180.0f) a += 360.0f;
  return a;
}


这也是✅ 你自己写的函数。

它做的事：把角度限制在 (-180, 180]，因为角度是周期的。

为什么必要？

避免角度跑到 540°、-900° 这种数值

让误差计算总是走“最短方向”

让 sync 校准不会累积偏移到非常大

## 第 8 段：轴角 → count（自己写的辅助换算）
static inline long shaftDegToDriveCount(float deg) {
  return lroundf(deg / degPerDriveCountOnShaft());
}


✅ 自己写的函数

lroundf()：C/C++ 标准数学函数（Arduino 支持），把 float 四舍五入到 long

它做的事：给定“指针轴转了多少度”，换算出需要多少个 count。

你当前程序里主要用的是 expectedDegToTargetCount()，这个 shaftDegToDriveCount() 现在相当于备用工具函数。

## 第 9 段：count → 实际轴角（自己写的关键换算）
static inline float driveCountToShaftDeg(long countDrive, float calibOffsetDeg) {
  return wrap180((float)countDrive * degPerDriveCountOnShaft() + calibOffsetDeg);
}


这是✅ 自己写的函数，也是非常关键的“测量模型”。

countDrive * degPerDriveCountOnShaft()：把编码器 count 变成轴角

+ calibOffsetDeg：加上校准偏移（因为电机没有绝对零位）

wrap180(...)：把角度折回 (-180,180]，防止越界

这行最终生成：actualDeg（实际角度），用于：

串口打印

sync 校准（expectedDeg - actualDeg）

# 第 2 大段

## 2.1 传感器 I2C 地址（自己定义）
const uint8_t MPL3115A2_ADDR = 0x60;


✅ 你自己定义的常量

0x60 是 MPL3115A2 芯片在 I2C 总线上的地址

后面所有 I2C 读写都会用到这个地址

如果你换成别的温度传感器，这一段几乎一定要改

## 2.2 I2C 写 1 个寄存器（自己写的“封装函数”）
bool i2cWrite8(uint8_t addr, uint8_t reg, uint8_t val) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  return (Wire.endTransmission() == 0);
}

这是什么？

✅ 你自己写的函数

但里面用到的 Wire.xxx 都是 Arduino Wire 库自带

一行一行讲：
Wire.beginTransmission(addr);


⛔ 不是你写的

✅ Wire 库函数

意思：开始向 I2C 设备 addr 发送数据

Wire.write(reg);


向设备发送“寄存器地址”

Wire.write(val);


向这个寄存器写入数据

return (Wire.endTransmission() == 0);


结束传输

endTransmission() 返回 0 表示成功

函数整体返回 true / false

在系统中的作用

👉 初始化传感器寄存器时用
👉 比如“开启温度测量”“配置模式”

## 2.3 I2C 读多个字节（自己写的封装）
bool i2cReadBytes(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false;}

这几行在干嘛？
Wire.beginTransmission(addr);
Wire.write(reg);


告诉设备：“我接下来要从 reg 这个寄存器开始读”

Wire.endTransmission(false)


false 的意思是：不要释放 I2C 总线

这是 I2C 的“重复起始（repeated start）”

是标准做法，不是你发明的

if (Wire.endTransmission(false) != 0) return false;


如果通信失败，直接返回 false

接着往下
uint8_t got = Wire.requestFrom(addr, len);
if (got != len) return false;


Wire.requestFrom()：

✅ Wire 库自带

从设备读取 len 个字节

如果没读够，判定失败

把数据读出来
for (uint8_t i = 0; i < len; i++) buf[i] = Wire.read();
return true;


Wire.read()：Wire 库函数

把字节存进你提供的数组 buf

在系统中的作用

👉 所有传感器数据读取的基础工具函数
## 2.4 初始化 MPL3115A2（自己写的逻辑）
bool mpl3115a2Init() {
  if (!i2cWrite8(MPL3115A2_ADDR, 0x13, 0x07)) return false;

这行什么意思？

0x13：PT_DATA_CFG 寄存器

0x07：打开数据就绪中断（温度/气压）

这是 MPL3115A2 数据手册规定的

👉 你不是“随便写”，而是照 datasheet 配的

  if (!i2cWrite8(MPL3115A2_ADDR, 0x26, 0xB9)) return false;


0x26：CTRL_REG1

0xB9 含义（重要，写报告可以用）：

ALT=0 → 温度/气压模式

OSR → 过采样

ACTIVE=1 → 开始测量

  delay(10);
  return true;
}


给芯片一点启动时间

返回初始化成功

结论

✅ mpl3115a2Init() 是你自己写的

⚠️ 但寄存器地址 & 数值来自官方手册

## 2.5 读取温度（核心函数）
bool readTemperatureC(float &tempC) {
  uint8_t buf[2];
  if (!i2cReadBytes(MPL3115A2_ADDR, 0x04, buf, 2)) return false;

解释

0x04：温度寄存器起始地址

连续读 2 个字节（MSB + LSB）

如果失败直接返回 false

  int8_t msb = (int8_t)buf[0];
  uint8_t lsb = buf[1];


MSB：温度整数部分（有符号）

LSB：小数部分（高 4 位有效）

  float frac = (float)(lsb >> 4) / 16.0f;


MPL3115A2 的温度精度是 1/16 °C

右移 4 位后再除以 16

  tempC = (float)msb + ((msb >= 0) ? frac : -frac);
  return true;
}


正温：加小数

负温：减小数（这是 datasheet 推荐写法）

在整个系统里的位置（非常重要）

👉 这是唯一一个“真实世界 → 数字世界”的入口

传感器 → tempC

后面：

tempC → expectedDeg

expectedDeg → expectedCountDrive

expectedCountDrive → 电机动作


# 第 3 大段：温度 → 期望指针角度（建模核心）

---

## 3.1 温度范围参数（你自己定义的“设计指标”）

```cpp
const float TEMP_MIN = 20.0f;
const float TEMP_MAX = 30.0f;
```

* ✅ **完全由你定义**
* 这是系统的**工作温度量程**
* 物理含义：

  * 温度 ≤ 20°C → 当作 20°C
  * 温度 ≥ 30°C → 当作 30°C

📌 在报告中可以写成：

> 系统对温度进行限幅处理，仅在 [20°C, 30°C] 区间内进行角度映射。

---

## 3.2 角度范围参数（你自己定义的“机械指标”）

```cpp
const float ANGLE_MIN = -120.0f;
const float ANGLE_MAX =  120.0f;
```

* ✅ **你自己定义**
* 表示**指针轴的允许旋转范围**
* 物理含义：

  * 最低温度 → 指针最左（-120°）
  * 最高温度 → 指针最右（+120°）

📌 这是“温度表盘刻度”的直接数字化体现。

---

## 3.3 温度 → 角度映射函数（你自己写的核心模型）

```cpp
static inline float tempToAngleDeg(float t) {
```

* ✅ **你自己写的函数**
* 输入：温度（°C）
* 输出：期望指针轴角度（°）

---

### 第一步：温度限幅（防止异常）

```cpp
  if (t < TEMP_MIN) t = TEMP_MIN;
  if (t > TEMP_MAX) t = TEMP_MAX;
```

* 防止：

  * 传感器异常
  * 超出量程导致指针乱转
* 这是**工程中非常标准的做法**

📌 报告术语：

> 为提高系统鲁棒性，对温度输入进行饱和处理（clamping）。

---

### 第二步：线性映射（核心公式）

```cpp
  float a = ANGLE_MIN 
          + (t - TEMP_MIN) 
          * (ANGLE_MAX - ANGLE_MIN) 
          / (TEMP_MAX - TEMP_MIN);
```

这是**整套系统最重要的一行之一**，我们慢慢拆。

#### 数学形式：

[
a = a_{min} + \frac{t - t_{min}}{t_{max} - t_{min}} (a_{max} - a_{min})
]

#### 物理含义：

* 温度在 `[TEMP_MIN, TEMP_MAX]` 内
* 按比例映射到 `[ANGLE_MIN, ANGLE_MAX]`

#### 你的具体数值：

* 温度跨度：30 − 20 = 10°C
* 角度跨度：120 − (−120) = 240°
* 灵敏度：
  [
  24^\circ / ^\circ C
  ]

📌 报告可写：

> 本系统采用线性比例映射，将温度变化等比例转换为指针角度变化。

---

### 第三步：角度归一化（安全措施）

```cpp
  return wrap180(a);
}
```

* 使用你前面定义的 `wrap180()`
* 保证角度永远在 (-180°, 180°] 内
* 防止：

  * 偏移叠加
  * 同步校准后角度越界

📌 注意：

> 即使你现在角度范围是 ±120°，
> 这个函数依然有意义，是“通用安全保护”。

---

## 3.4 这一段在系统中的位置（强烈建议写）

整个数据链路是：

```
真实温度 (°C)
   ↓  readTemperatureC()
tempC
   ↓  tempToAngleDeg()
expectedDeg
   ↓  expectedDegToTargetCount()
expectedCountDrive
   ↓  motion controller
电机 → 指针轴
```

这一步**不涉及电机、不涉及 PID、不涉及编码器**，
它是**纯数学建模层**。

---

## 3.5 如果以后要改量程，需要改哪里？

例如你老师问：

> “如果温度范围改成 -20~60°C，角度改成 -90~90°，要改什么？”

你可以非常自信地回答：

✅ **只需要改 4 个参数**：

```cpp
const float TEMP_MIN  = -20.0f;
const float TEMP_MAX  =  60.0f;
const float ANGLE_MIN = -90.0f;
const float ANGLE_MAX =  90.0f;
```

❌ **不需要改**：

* 电机控制参数
* 编码器参数
* 运动状态机
* PI / P / nudge 逻辑

📌 这是你程序结构设计得非常好的地方。

---



---

如果你回复 **“继续”**，
下一段我会讲：

👉 **电机控制与编码器接口（setMotor / readCountDrive）**
这是“软件如何真正驱动物理世界”的部分，也是你震荡问题当初的根源之一。


好，我们继续 👍
这一段是**“软件如何真正驱动物理世界”**：
👉 **电机 + 编码器接口层**。
这是你之前出现**震动、方向反、count 飘**等问题的根源层，非常关键。

我还是**几行几行讲**，并且明确告诉你：

* 哪些是 **你自己定义的**
* 哪些是 **库/Arduino 已有的**

---

# 第 4 大段：电机输出 & 编码器输入（硬件接口层）

这一层的特点是：
👉 **不做“智能决策”**
👉 只做**最底层的 I/O 封装**

---

## 4.1 电机停止函数（你自己写的）

```cpp
void stopMotor() { 
  analogWrite(PIN_EN, 0); 
}
```

* ✅ **你自己写的函数**
* `analogWrite()`：Arduino 自带 API

### 作用

* 让 PWM = 0
* 电机**不通电**
* 没有保持力（不是刹车）

📌 在你的系统里：

* STOP 模式
* NUDGE_OFF 阶段
* 运动完成后
  都会调用它

---

## 4.2 电机驱动函数（你自己写的关键封装）

```cpp
void setMotor(bool dirHigh, int pwm) {
```

* ✅ **你自己写的函数**
* 输入两个量：

  * `dirHigh`：逻辑方向（正 / 反）
  * `pwm`：速度大小（0~255）

---

### 方向反转处理（非常重要）

```cpp
  if (INVERT_MOTOR_DIR) dirHigh = !dirHigh;
```

* `INVERT_MOTOR_DIR`：你自己定义的调试参数
* 解决问题：

  * “代码里算的是正方向，但电机实际反着转”

📌 这是**工程中极常见做法**，而且非常专业。

---

### 输出方向信号

```cpp
  digitalWrite(PIN_DIR, dirHigh ? HIGH : LOW);
```

* `digitalWrite()`：Arduino 自带
* 控制电机驱动器 DIR 引脚

---

### 输出 PWM

```cpp
  analogWrite(PIN_EN, pwm);
```

* `analogWrite()`：Arduino 自带
* 本质：

  * 控制电机**转速**
  * 不是控制角度
  * 不是控制时间

📌 非常重要一句话（你报告里可以写）：

> 本系统中电机为速度控制对象，角度控制通过编码器反馈在上层软件中实现。

---

## 4.3 编码器对象（库提供）

```cpp
Encoder enc(PIN_ENC_A, PIN_ENC_B);
```

* ⛔ 不是你写的
* ✅ `Encoder` 类来自 `<Encoder.h>` 库

作用：

* 自动处理中断 / 轮询
* 自动判断 A/B 相方向
* 给你一个“累计计数值”

---

## 4.4 读取编码器 count（你自己写的封装）

```cpp
long readCountDrive() {
  long c = enc.read();
```

* `enc.read()`：Encoder 库自带
* 返回一个 **long 类型累计计数**
* 可能是正，也可能是负

---

### 编码器方向反转

```cpp
  if (INVERT_ENCODER) c = -c;
  return c;
}
```

* `INVERT_ENCODER`：你自己定义
* 用来修正：

  * A/B 相接反
  * 或安装方向反了

📌 非常重要：

> **编码器只负责“数数”，方向的物理含义是软件赋予的**

---

## 4.5 小结：这一层“只干三件事”

你可以在报告里总结为：

1. **stopMotor()**

   * 关闭 PWM
   * 电机断电

2. **setMotor(dir, pwm)**

   * 输出方向
   * 输出速度（PWM）

3. **readCountDrive()**

   * 从编码器读取累计 count
   * 统一方向符号

⚠️ 这一层 **不关心**：

* 温度
* 角度
* r / l / r5
* 稳不稳定

👉 所有“聪明的东西”都在**下一层：运动控制状态机**

---

## 4.6 你之前所有问题，其实都来自这里 + 上一层

你之前遇到的：

* “r 一下转很多圈”
* “count 有时正有时负”
* “不加手阻尼就抖”
* “方向反了”

本质原因总结一句话：

> **电机是速度执行器，而你一开始试图把它当成角度执行器来用**

现在你这套结构已经完全正确：

* 底层：速度 + count
* 上层：目标 count + 状态机
* 最上层：温度 → 角度 → count


# 第 5 大段：运动控制状态机（按 count 精确到位）

> 核心目标：让编码器 count 从当前值走到 `targetCountDrive`，并且尽量做到 `err=0`（或很小）。

---

## 5.1 区域划分参数（你自己定义的控制策略参数）

```cpp
const long FAR_ZONE = 8;
const long NEAR_ZONE = 3;
const long FINE_ZONE = 2;
```

* ✅ 全是你自己定义的控制参数（不是库自带）
* 它们用于把误差 `err = target - count` 分成不同“工作区间”：

  * **误差大**：快速粗调（COARSE_FAST）
  * **误差中等**：慢速粗调（COARSE_SLOW）
  * **误差很小**：进入微调（NUDGE）

你现在实际使用的主要是：

* `FAR_ZONE`：决定 fast/slow
* `FINE_ZONE`：决定何时进入 nudge

`NEAR_ZONE` 在你这版逻辑里其实没真正用到（你只用 FAR_ZONE/FINE_ZONE），它保留也没问题。

---

## 5.2 “到位判定”参数（你自己定义，决定精度/时间）

```cpp
const long TOL_DONE = 0;
const unsigned long STABLE_MS_ZERO = 500;
const unsigned long STABLE_MS_ONE  = 800;
const bool ALLOW_FALLBACK_PLUSMINUS1 = true;
```

* ✅ 都是你自己定义

意义：

* `TOL_DONE = 0`：你要求“尽量多数 0 误差”，所以把完成目标设为 **err 必须=0**
* `STABLE_MS_ZERO = 500`：`err==0` 必须持续 500ms 才算“真的到位”

  * 防止刚好扫到 0 又弹回去
* `STABLE_MS_ONE = 800` + `ALLOW_FALLBACK_PLUSMINUS1=true`

  * 如果一直很难做到 0，就允许 err=±1 稳定 800ms 当成完成（兜底）
  * 这让系统不会无限抖动在 0 附近

---

## 5.3 粗调控制参数（你自己定义，决定“拉近速度”）

```cpp
const float KP_COARSE = 7.0f;
const int PWM_COARSE_MIN_FAST = 60;
const int PWM_COARSE_MAX_FAST = 130;

const int PWM_COARSE_MIN_SLOW = 55;
const int PWM_COARSE_MAX_SLOW = 100;
```

* ✅ 都是你自己定义

解释：

* `KP_COARSE`：粗调阶段的 P 控制增益

  * 输出：`u = KP_COARSE * err`
  * `err` 以 count 为单位，所以 u 也是“与 count 误差成比例”的控制量
* `PWM_COARSE_MIN_*`：最小 PWM（克服静摩擦，“一定要动起来”）
* `PWM_COARSE_MAX_*`：最大 PWM（避免过冲、避免太猛）

**FAST vs SLOW**

* FAST：误差很大，用更强的输出，快速接近
* SLOW：误差较小，用更温柔的输出，降低过冲，准备进入 nudge

---

## 5.4 微调 nudge 参数（你自己定义，决定“最后能不能落在 0”）

```cpp
const int PWM_NUDGE = 55;
const unsigned long NUDGE_OFF_MS = 160;
const unsigned long NUDGE_ON_MS_COARSE = 35;
const unsigned long NUDGE_ON_MS_FINE   = 15;
```

* ✅ 都是你自己定义

作用非常重要：

* `PWM_NUDGE`：微调脉冲的力度（一般接近“能动门槛”）
* `NUDGE_ON_MS_*`：脉冲持续时间

  * err=2 时用长一点（推进更多）
  * err=1 时用短一点（只推一点点，尽量落到 0）
* `NUDGE_OFF_MS`：脉冲之间的等待时间

  * 这就是你之前发现“加手指摩擦就更准”的原因之一
  * OFF 等待相当于让系统自然阻尼、齿隙回弹结束再测一次 count

---

## 5.5 控制调度参数（你自己定义）

```cpp
const unsigned long CTRL_MS  = 10;
const unsigned long PRINT_MS = 200;
const unsigned long TIMEOUT_MS = 7000;
```

* ✅ 自己定义

* `CTRL_MS=10ms`：运动控制更新频率 100Hz

* `PRINT_MS=200ms`：串口输出频率

* `TIMEOUT_MS=7000ms`：7秒到不了就强制停止（防止编码器失效一直转）

---

## 5.6 状态机枚举（你自己定义）

```cpp
enum MotionState { M_IDLE, M_COARSE_FAST, M_COARSE_SLOW, M_NUDGE_OFF, M_NUDGE_ON };
MotionState mstate = M_IDLE;
```

* ✅ 自己定义

含义：

* `M_IDLE`：空闲，电机必须停
* `M_COARSE_FAST`：快档粗调
* `M_COARSE_SLOW`：慢档粗调
* `M_NUDGE_OFF`：微调等待（电机关）
* `M_NUDGE_ON`：微调脉冲（电机开一小段时间）

---

## 5.7 目标与计时变量（你自己定义）

```cpp
long targetCountDrive = 0;
unsigned long moveStartMs = 0;

unsigned long phaseT0 = 0;
unsigned long stableZeroT0 = 0;
unsigned long stableOneT0  = 0;
```

* ✅ 自己定义

解释：

* `targetCountDrive`：目标 count（你希望 count 变成多少）
* `moveStartMs`：开始移动的时间，用于 timeout
* `phaseT0`：nudge on/off 阶段起始时间
* `stableZeroT0`：第一次进入 err==0 的时间（用来算持续 500ms）
* `stableOneT0`：第一次进入 |err|==1 的时间（用来算持续 800ms）

---

## 5.8 启动运动：`motionStartTo()`

```cpp
void motionStartTo(long newTargetCount) {
  targetCountDrive = newTargetCount;
  moveStartMs = millis();
  stableZeroT0 = 0;
  stableOneT0  = 0;
  mstate = M_COARSE_FAST;
}
```

* ✅ 你自己写的函数
* `millis()`：Arduino 自带（返回系统运行毫秒数）

逐行解释：

1. `targetCountDrive = newTargetCount;`
   设置目标

2. `moveStartMs = millis();`
   记录开始时间

3. `stableZeroT0 = 0; stableOneT0 = 0;`
   清空稳定计时器（因为这是一次新的运动）

4. `mstate = M_COARSE_FAST;`
   新运动默认从“快档粗调”开始

---

## 5.9 相对移动：`motionMoveBy()`

```cpp
void motionMoveBy(long delta) {
  long c = readCountDrive();
  motionStartTo(c + delta);
}
```

* ✅ 你自己写的函数
* `readCountDrive()` 是你封装的编码器读取

解释：

* 读取当前 count = c
* 目标变成 c + delta
* 然后用 `motionStartTo()` 启动状态机

这就是为什么：

* `r` 对应 `+12`
* `l` 对应 `-12`

而且每次都是“相对移动”

## 5.10 `motionUpdate()` 第 1 段：读数 + 误差

```cpp
void motionUpdate() {
  unsigned long now = millis();

  long c = readCountDrive();
  long err = targetCountDrive - c;
  long aerr = labs(err);
```

* `millis()`：✅ Arduino 自带，当前运行时间（ms）
* `readCountDrive()`：✅ 你自己写的封装（内部用 `enc.read()`）
* `targetCountDrive`：✅ 你自己维护的目标 count
* `err = target - current`：✅ 控制误差（count单位）
* `labs(err)`：✅ C 标准库函数（long absolute），得到误差绝对值

> 到这里，你已经得到了“我离目标还差多少 count”。

---

## 5.11 第 2 段：超时保护（防止一直转）

```cpp
  if (mstate != M_IDLE && now - moveStartMs > TIMEOUT_MS) {
    mstate = M_IDLE;
    stopMotor();
  }
```

* `TIMEOUT_MS`：✅ 你定义的 7000ms
* `moveStartMs`：✅ 你在 `motionStartTo()` 里记录的开始时间
* 逻辑：
  **只要当前不是空闲状态**，并且运动时间超过 7 秒 → 直接停机

为什么必须要有？

* 编码器断线/松动
* 电机卡住
* 齿轮打滑
  这些情况如果没有 timeout，会导致电机一直输出 PWM，风险很大。

---

## 5.12 第 3 段：稳定到位判定（“0误差要持续”）

这一段是你系统为什么“看起来很稳”的关键。

```cpp
  if (mstate != M_IDLE) {
    if (err == 0) {
      if (stableZeroT0 == 0) stableZeroT0 = now;
    } else {
      stableZeroT0 = 0;
    }
```

逐行解释：

* 只有在“运动进行中”（非 IDLE）才计算稳定性
* 如果 `err==0`：

  * 第一次达到 0 时，记录时间 `stableZeroT0 = now`
  * 后面如果一直保持 0，就不再改这个时间
* 如果 `err != 0`：

  * 清零 `stableZeroT0`，意味着“稳定计时中断，得重新来”

---

接着是 |err|==1 的兜底计时：

```cpp
    if (labs(err) == 1) {
      if (stableOneT0 == 0) stableOneT0 = now;
    } else {
      stableOneT0 = 0;
    }
```

* 逻辑完全同上，只是条件从 `err==0` 换成 `|err|==1`
* 这是为了避免“永远在 0 附近震荡但就是碰不到 0”导致一直不结束

---

然后是真正的“完成判定”：

```cpp
    if (TOL_DONE == 0) {
      if (stableZeroT0 != 0 && now - stableZeroT0 >= STABLE_MS_ZERO) {
        mstate = M_IDLE; stopMotor();
      } else if (ALLOW_FALLBACK_PLUSMINUS1 && stableOneT0 != 0 && now - stableOneT0 >= STABLE_MS_ONE) {
        mstate = M_IDLE; stopMotor();
      }
    }
  }
```

* `TOL_DONE==0`：表示你想“目标多数为 0”
* 第一优先级：
  `err==0` 连续保持 `STABLE_MS_ZERO=500ms` → 完成
* 第二优先级（兜底）：
  如果允许 `±1`，并且 `|err|==1` 连续 `800ms` → 完成

为什么要“持续时间”而不是“一瞬间到 0 就结束”？

* 机械有惯性/齿隙/回弹
* 一瞬间读到 0 可能只是“路过”，马上又变成 ±1
  持续时间能显著减少抖动。

---

## 5.13 第 4 段：状态机输出（真正控制电机）

先看入口：

```cpp
  if (mstate == M_IDLE) {
    stopMotor();
    return;
  }
```

* 如果空闲：电机必须停
* 直接 return，不进入任何输出逻辑
  这保证了“空闲状态绝对不会发力”。

---

### 5.13.1 粗调（COARSE）分支

```cpp
  if (mstate == M_COARSE_FAST || mstate == M_COARSE_SLOW) {
```

说明当前处于粗调：输出连续 PWM，让它快速接近目标。

### (1) 判断是否要切到 nudge

```cpp
    if (aerr <= FINE_ZONE) {
      mstate = M_NUDGE_OFF;
      phaseT0 = now;
      stopMotor();
    }
```

* 当 |err| ≤ 2（你的 FINE_ZONE=2）
* 切到 `M_NUDGE_OFF`（先断电等待）
* 记录 `phaseT0`，用于 nudge 的计时

为什么先 OFF？

* 让系统停稳，避免惯性导致下一次脉冲判断不准

---

### (2) 决定 FAST 还是 SLOW

```cpp
    else if (aerr < FAR_ZONE) {
      mstate = M_COARSE_SLOW;
    } else {
      mstate = M_COARSE_FAST;
    }
```

* |err| < 8 → SLOW（更温柔）
* |err| ≥ 8 → FAST（更猛）

---

### (3) 在 COARSE 状态下计算 PWM 输出

```cpp
    if (mstate == M_COARSE_FAST || mstate == M_COARSE_SLOW) {
      float u = KP_COARSE * (float)err;
      bool dirHigh = (u >= 0);
      int pwm = (int)fabs(u);
```

* `u = Kp * err`：典型比例控制
* `dirHigh`：误差正 → 往正方向转；误差负 → 往反方向转
* `pwm = |u|`：输出强度与误差成正比

> 注意：这里是 **P 控制**，没有 I/D。你用 nudge 解决静摩擦和最后的精度。

---

### (4) PWM 限幅（“能动”+“别太猛”）

FAST 档：

```cpp
      if (mstate == M_COARSE_FAST) {
        if (pwm > PWM_COARSE_MAX_FAST) pwm = PWM_COARSE_MAX_FAST;
        if (pwm > 0 && pwm < PWM_COARSE_MIN_FAST) pwm = PWM_COARSE_MIN_FAST;
      }
```

* 上限：不让它过猛
* 下限：不让它卡住不动（尤其快档）

SLOW 档：

```cpp
      else {
        if (pwm > PWM_COARSE_MAX_SLOW) pwm = PWM_COARSE_MAX_SLOW;
        if (pwm > 0 && pwm < PWM_COARSE_MIN_SLOW) pwm = PWM_COARSE_MIN_SLOW;
      }
```

* 同理，只是 max/min 更低

最后真正输出：

```cpp
      setMotor(dirHigh, pwm);
```

---

## 5.13.2 微调 OFF（NUDGE_OFF）

```cpp
  else if (mstate == M_NUDGE_OFF) {
    stopMotor();
    if (now - phaseT0 >= NUDGE_OFF_MS) {
      mstate = M_NUDGE_ON;
      phaseT0 = now;
    }
  }
```

* OFF 阶段：电机断电
* 等待 `NUDGE_OFF_MS=160ms`
* 时间到了进入 ON 阶段，并更新 phaseT0

为什么 OFF 要这么久？

* 让齿隙回弹结束
* 让编码器读数稳定
* 让惯性停止
  你之前“加手摩擦更准”，本质就是给系统加了阻尼，相当于缩短了“停稳时间”。

---

## 5.13.3 微调 ON（NUDGE_ON）

```cpp
  else if (mstate == M_NUDGE_ON) {
    unsigned long onMs = (aerr <= 1) ? NUDGE_ON_MS_FINE : NUDGE_ON_MS_COARSE;
```

* 如果 |err|==1：用更短的脉冲 15ms（避免过冲）
* 如果 |err|==2：用稍长的脉冲 35ms（推动更明显）

---

方向与输出：

```cpp
    bool dirHigh = (err > 0);
    setMotor(dirHigh, PWM_NUDGE);
```

* err>0 → 往正方向推一下
* 输出 PWM_NUDGE（接近能动门槛，不猛）

---

脉冲结束就回到 OFF：

```cpp
    if (now - phaseT0 >= onMs) {
      mstate = M_NUDGE_OFF;
      phaseT0 = now;
    }
  }
}
```

---


继续 👍
接下来讲你代码的后半段：**模式（STOP/GO）、串口命令、sync 校准、以及 loop() 调度结构**。我还是按你代码顺序几行几行讲，并说明哪些是你定义的、哪些是 Arduino/库自带的。

---

# 第 6 大段：模式系统（STOP / GO）与串口命令

---

## 6.1 模式枚举（你自己定义）

```cpp
enum Mode { MODE_STOP, MODE_GO };
Mode mode = MODE_STOP;  // default power-on STOP
```

* ✅ `enum Mode` 是你自己定义的类型
* `MODE_STOP / MODE_GO` 是你自己定义的两个模式
* `mode = MODE_STOP`：上电默认 STOP（符合你最初需求：默认停机）

模式含义：

* **STOP**：不跟随温度（电机默认停），但仍允许手动 r/l/r5/l5 做校准微调
* **GO**：自动跟随温度（温度变 → expectedDeg 变 → 目标 count 变 → 电机动作）

---

## 6.2 串口缓存（你自己定义）

```cpp
String cmdLine;
```

* ✅ 你自己定义的变量
* 类型 `String` 是 Arduino 提供的字符串类（Arduino 核心库自带）
* 用途：把用户输入的一行命令（例如 `"r5"`、`"sync"`）缓冲起来，直到遇到回车

---

## 6.3 校准偏移 offset（你自己定义，解决“无绝对零位”）

```cpp
float calibOffsetDeg = 0.0f;
```

* ✅ 你自己定义的变量
* 物理意义：**指针轴角度的“零点偏移”**
* 原因：你的电机/编码器没有绝对零点，上电 count=0 不代表指针=0°

你定义了角度测量模型：

[
actualDeg = count \cdot degPerCount + calibOffsetDeg
]

`sync` 的目的就是调整 `calibOffsetDeg`，让“当前指针位置=当前温度应指向的位置”。

---

## 6.4 expected / actual 角度（你自己定义）

```cpp
float expectedDeg = 0.0f;
float actualDeg   = 0.0f;
```

* ✅ 你自己定义的变量
* expectedDeg：温度映射出来的“应该指向的角度”
* actualDeg：编码器换算出来的“当前实际角度”

---

## 6.5 GO 模式下的目标 count（你自己定义）

```cpp
long expectedCountDrive = 0;
```

* ✅ 你自己定义
* GO 模式下我们最终要把 expectedDeg 转换成“目标 count”，让运动控制器去追它

---

## 6.6 expectedDeg → 目标 count 的换算函数（你自己写）

```cpp
long expectedDegToTargetCount(float expDeg) {
  float pure = wrap180(expDeg - calibOffsetDeg);
  return lroundf(pure / degPerDriveCountOnShaft());
}
```

### 逐行解释

* ✅ 这是你自己写的函数
* `wrap180()`：你自己写的工具函数
* `degPerDriveCountOnShaft()`：你自己写的比例尺函数
* `lroundf()`：C 标准数学函数（Arduino 支持）

核心推导来自模型：

[
actualDeg = count \cdot degPerCount + offset
]

想让 `actualDeg = expDeg`，则：

[
count = \frac{expDeg - offset}{degPerCount}
]

对应代码就是：

* `pure = expDeg - calibOffsetDeg`
* `count = pure / degPerCount`

这一步很关键：**offset 不参与温度映射，它只参与“角度↔count”的换算**。

---

## 6.7 命令处理函数 handleCommand（你自己写）

```cpp
void handleCommand(String s) {
  s.trim();
  s.toLowerCase();
```

* `trim()` / `toLowerCase()`：Arduino String 自带方法（不是你写的）
* 目的：

  * 去掉空格/回车残留
  * 统一大小写，避免用户输入 “R5” 无效

---

### STOP 命令 s

```cpp
  if (s == "s") {
    mode = MODE_STOP;
    mstate = M_IDLE;
    stopMotor();
    return;
  }
```

* `mode=STOP`：停止自动跟随
* `mstate=M_IDLE`：强制取消任何正在执行的运动任务
* `stopMotor()`：确保电机断电
* `return`：结束处理

---

### GO 命令 g

```cpp
  if (s == "g") {
    mode = MODE_GO;
    return;
  }
```

* 切换到自动跟随
* 不直接 motionStartTo，因为 GO 逻辑在 loop 的周期更新中统一处理（结构更干净）

---

### 手动微调命令 r/l/r5/l5

```cpp
  if (s == "r")  { motionMoveBy(+ONE_REV_COUNTS); return; }
  if (s == "l")  { motionMoveBy(-ONE_REV_COUNTS); return; }
  if (s == "r5") { motionMoveBy(+5 * ONE_REV_COUNTS); return; }
  if (s == "l5") { motionMoveBy(-5 * ONE_REV_COUNTS); return; }
```

* `ONE_REV_COUNTS=12`：你定义的机械参数
* 这些命令直接调用“你完美的 motion 状态机”
* 注意：即便在 STOP 模式，也能手动微调——这符合你最早的需求（校准时不随温度动）

---

### sync 校准命令（你自己写的关键功能）

```cpp
  if (s == "sync") {
    float err = wrap180(expectedDeg - actualDeg);
    calibOffsetDeg = wrap180(calibOffsetDeg + err);
    return;
  }
```

这是最值得写报告的部分之一。

#### 为什么是 “offset += (expected - actual)”？

你希望 sync 之后，当前 count 下算出来的角度变成 expectedDeg。

原本：
[
actualDeg = count\cdot degPer + offset
]

你要新的 offset' 使得：
[
expectedDeg = count\cdot degPer + offset'
]

所以：
[
offset' = expectedDeg - count\cdot degPer
]

而你当前 offset 已经满足：
[
actualDeg = count\cdot degPer + offset
\Rightarrow count\cdot degPer = actualDeg - offset
]

代入：
[
offset' = expectedDeg - (actualDeg - offset) = offset + (expectedDeg - actualDeg)
]

所以正确更新就是：

```cpp
offset = offset + (expected - actual)
```

你代码完全正确。

`wrap180()` 是为了防止偏移累积越界。

---

# 第 7 大段：setup() 初始化（系统上电行为）

---

```cpp
void setup() {
  Serial.begin(9600);
```

* `Serial.begin(9600)`：Arduino 自带串口初始化
* 波特率 9600（你要求的）

---

```cpp
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_EN, OUTPUT);
  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);
  stopMotor();
```

* `pinMode()` / `INPUT_PULLUP`：Arduino 自带
* 编码器用上拉输入：常见做法，减少悬空噪声
* `stopMotor()`：上电先确保电机不动（安全）

---

```cpp
  Wire.begin();
  (void)mpl3115a2Init();
```

* `Wire.begin()`：Wire 库自带，启动 I2C
* `mpl3115a2Init()`：你写的初始化
* `(void)`：表示“我不处理返回值”（不关心失败与否），为了消除编译警告

---

```cpp
  long c = readCountDrive();
  actualDeg = driveCountToShaftDeg(c, calibOffsetDeg);
```

* 上电读一次编码器，得到当前“实际角度”
* 因为 offset 初始为 0，所以这一步更多是为了让打印的 actualDeg 有意义

---

```cpp
  Serial.println(...);
  Serial.print(...);
```

* 打印提示信息（调试/实验报告很好用）

---

# 第 8 大段：loop() 主循环调度（非阻塞结构）

你 loop 里主要分三块：

1. 读串口命令（随时响应）
2. 每 CTRL_MS 做一次控制更新（定时执行）
3. 每 PRINT_MS 打印一次状态（定时执行）

---

## 8.1 读串口命令（你写的缓冲逻辑）

```cpp
while (Serial.available()) {
  char ch = (char)Serial.read();
```

* `Serial.available()` / `Serial.read()`：Arduino 自带
* 逐字节读入

遇到换行/回车，认为一条命令结束：

```cpp
  if (ch == '\n' || ch == '\r') {
    if (cmdLine.length()) {
      handleCommand(cmdLine);
      cmdLine = "";
    }
  }
```

否则追加到字符串：

```cpp
  else {
    cmdLine += ch;
    if (cmdLine.length() > 64) cmdLine.remove(0, 32);
  }
```

* `remove()` 是 String 自带方法
* 防止用户乱输入导致 cmdLine 无限长

---

## 8.2 每 CTRL_MS 更新一次控制（你写的“定时器式 loop”）

```cpp
static unsigned long lastCtrl = 0;
unsigned long now = millis();
if (now - lastCtrl >= CTRL_MS) {
  lastCtrl = now;
```

* 这是典型“非阻塞定时”写法
* 好处：不用 delay，系统能及时响应串口

---

### 8.2.1 读温度 → expectedDeg

```cpp
  float tempC = 0.0f;
  bool tOk = readTemperatureC(tempC);
  if (tOk) expectedDeg = tempToAngleDeg(tempC);
```

* `readTemperatureC()`：你写的 I2C 读取
* `tempToAngleDeg()`：你写的线性映射
* tOk 失败就不更新 expectedDeg（保持上一次值）

---

### 8.2.2 读编码器 → actualDeg

```cpp
  long countDrive = readCountDrive();
  actualDeg = driveCountToShaftDeg(countDrive, calibOffsetDeg);
```

* `readCountDrive()`：你写的封装（内部 enc.read）
* `driveCountToShaftDeg()`：你写的换算（count→角度 + offset）

---

### 8.2.3 GO 模式：目标变化才启动运动

```cpp
  if (mode == MODE_GO) {
    expectedCountDrive = expectedDegToTargetCount(expectedDeg);

    if (expectedCountDrive != targetCountDrive) {
      motionStartTo(expectedCountDrive);
    }
  }
```

这一段非常关键：**只在目标 count 改变时才启动一次运动**。

为什么这么做？

* 如果你每个 CTRL_MS 都强行 motionStartTo，会一直重置状态机：

  * 稳定计时清零
  * 状态永远在 COARSE_FAST
  * 会导致抖动/震荡
* 现在这样写：状态机可以自己完整跑完一次运动任务

---

### 8.2.4 STOP 模式：没有运动任务就停机

```cpp
  if (mode == MODE_STOP && mstate == M_IDLE) {
    stopMotor();
  }
```

* STOP 模式默认电机停
* 但如果你手动输入 r/l 触发了 motionMoveBy，此时 mstate != IDLE，就不会强制 stop（保证手动动作能完成）

---

### 8.2.5 运行运动状态机（只要目标存在就会动）

```cpp
  motionUpdate();
```

* 这是你最核心的控制器，我们上一段讲过

---

## 8.3 定时打印（你写的调试输出）

```cpp
static unsigned long lastPrint = 0;
if (now - lastPrint >= PRINT_MS) {
  lastPrint = now;
  ...
}
```

打印内容：

* 温度 `T=`
* expectedDeg
* actualDeg
* countDrive
* mode

这对实验报告也很重要：你可以截图作为实验结果。

---

## 这一段的“谁定义的”总结

✅ 你自己定义/实现：

* `Mode`、`mode`
* `calibOffsetDeg`、`expectedDeg`、`actualDeg`
* `expectedDegToTargetCount()`
* `handleCommand()`
* loop 内部调度逻辑（lastCtrl/lastPrint）

✅ Arduino/库提供：

* `Serial.begin/available/read/print/println`
* `millis()`
* `pinMode()`
* `Wire.begin()`
* `Encoder enc` 的 `enc.read()`

---

如果你愿意，我还可以把你整段“sync 校准推导”写成报告形式（含公式推导），以及把状态机画成一个简单的流程图描述（文字版/伪代码版）。

