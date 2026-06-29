// 定义两路LED引脚
#define LED_A 4
#define LED_B 5

void setup() {
  Serial.begin(115200);
  // 两路独立PWM，5000Hz 8位分辨率
  ledcAttach(LED_A, 5000, 8);
  ledcAttach(LED_B, 5000, 8);
}

void loop() {
  // 同步渐变：A渐亮，B同步渐暗
  for(int bright = 0; bright <= 255; bright++){
    ledcWrite(LED_A, bright);
    ledcWrite(LED_B, 255 - bright);
    delay(8); // 延时统一，过渡柔和不急促
  }
  // 同步渐变：A渐暗，B同步渐亮
  for(int bright = 255; bright >= 0; bright--){
    ledcWrite(LED_A, bright);
    ledcWrite(LED_B, 255 - bright);
    delay(8);
  }
}