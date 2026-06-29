#define TOUCH_PIN 4
#define LED_PIN 2

// 档位变量：1、2、3三档
int gear = 1;
// 步长，档位越大，呼吸速度越快
int step = 3;
// 触摸状态标记，保证必须松手才能再次触发
bool isPressed = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  // 兼容ESP32的PWM写法，不会报错
  ledcAttach(LED_PIN, 5000, 8);
}

void loop() {
  // 读取触摸值
  int touchValue = touchRead(TOUCH_PIN);

  // 触摸按下，且上一次是松开状态：只切换一次档位
  if (touchValue < 400 && isPressed == false)
  {
    gear = gear + 1;
    if (gear > 3)
    {
      gear = 1;
    }

    // 根据档位修改步长，三档速度区分明显
    if (gear == 1) step = 3;
    if (gear == 2) step = 8;
    if (gear == 3) step = 15;

    Serial.print("当前档位：");
    Serial.println(gear);
    isPressed = true;
  }

  // 手指松开，重置状态，允许下一次触摸
  if (touchValue >= 400)
  {
    isPressed = false;
  }

  // 呼吸灯渐亮
  for (int i = 0; i <= 255; i = i + step)
  {
    ledcWrite(LED_PIN, i);
    delay(10);
  }
  // 呼吸灯渐暗
  for (int i = 255; i >= 0; i = i - step)
  {
    ledcWrite(LED_PIN, i);
    delay(10);
  }
}