// 引脚定义
#define TOUCH_PIN 4   // 触摸焊盘引脚
#define LED_PIN 2     // LED正极引脚

bool ledState = false;
int lastTouchValue = 999;
unsigned long timeNow = 0;
unsigned long timeLast = 0;
// 防抖间隔
#define DELAY_DEBOUNCE 50

void setup() {
  Serial.begin(115200);    // 串口调试
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 上电默认熄灭
}

void loop() {
  timeNow = millis();
  int val = touchRead(TOUCH_PIN);
  Serial.print("触摸数值：");
  Serial.println(val); // 实时打印触摸值

  // 防抖判断
  if ((timeNow - timeLast) > DELAY_DEBOUNCE) {
    if (val < 400 && lastTouchValue >= 400) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      if(ledState){
        Serial.println("灯点亮");
      }else{
        Serial.println("灯熄灭");
      }
      timeLast = timeNow;
    }
  }
  lastTouchValue = val;
  delay(20);
}