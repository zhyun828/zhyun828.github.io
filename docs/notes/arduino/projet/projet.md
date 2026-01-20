| 功能        | 模块引脚 | Arduino Mega |
| --------- | ---- | ------------ |
| I2C SCL   | SCL  | **D21**      |
| I2C SDA   | SDA  | **D20**      |
| 电机方向      | DIR  | **D8**       |
| 电机使能(PWM) | EN   | **D9**       |
| 编码器A      | SA   | **D2**       |
| 编码器B      | SB   | **D3**       |



使用encoder.h库做一个感应温度的机器，其原理是，当温度变化时，检测温度，然后驱动轮转动传动轴通过指针指向该温度。温度范围20-30摄氏度，指针角度-180到180度（即20摄氏度和30摄氏度指针在同一个角度）。其次由于电机没有默认0角度我需要几个功能，首先是s，输入s使机器停止运行，即使温度改变电机也不随着变化，并默认上电进入s模式，g，输入g代表go，使机器正常运行。r，输入r使指针往右偏转一点，用于微调，l，输入l使指针往左偏转一点，r5、l5，输入r5、l5使指针多转动一点用于粗调。当前温度，该温度所对应的角度（预期角度），实际角度（电机转动后的实际角度），count用于计数，sync，输入sync使当前的实际角度=预期角度。比如此时是22.5摄氏度应该对应-90度，go之后实际角度也为-90度，而指针却指向25摄氏度，则需要输入l、r让实际角度偏转，比如偏转到-180度，然后输入sync使这个-180变为-90.波特率为9600，使用pi控制（不用d），串口输出为：温度传感器测得的温度；当前温度对应的预期旋转角度、实际旋转角度，计数器count次数。电机减速比为1：52.734



STOP 是：
停机不动（电机完全不驱动），温度变也不动；
只有发 r/l/r5/l5 时才“动一下”，到位就马上停止驱动，不再保持。

所以要改成：STOP 下完全不输出，新增一个 JOG 模式：只在 jog 时做短暂闭环，到了就立刻回到 STOP 并关 PWM。

#include <Wire.h>
#include <Encoder.h>



这个段代码已经实现了r/l/r5/l5的功能
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
