// 定义LED引脚GPIO2
const int ledPin = 2;
// 记录LED当前亮灭状态
int ledState = LOW;
// 记录上一次切换灯的时间
unsigned long previousMillis = 0;
// 完整闪烁周期1000ms，1Hz频率
const long interval = 1000;

void setup() {
  // 设置LED引脚为输出模式
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 获取当前系统运行毫秒数
  unsigned long currentMillis = millis();
  // 判断是否到达切换灯光的时间
  if (currentMillis - previousMillis >= interval/2) {
    // 更新计时基准
    previousMillis = currentMillis;
    // 翻转LED状态
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}