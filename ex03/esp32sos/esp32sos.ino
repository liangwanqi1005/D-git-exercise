const int ledPin = 2;
// 时间定义
const unsigned long dotTime = 200;    // 短亮200ms
const unsigned long dashTime = 600;   // 长亮600ms
const unsigned long gapTime = 200;    // 字符间隔
const unsigned long sosEndPause = 2000;// 一轮SOS结束停顿

unsigned long nowT;
unsigned long lastT;
int stage = 0;
bool ledOn = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  nowT = millis();
  switch(stage){
    case 0: blinkDot(); break;
    case 1: blinkDot(); break;
    case 2: blinkDot(); break;
    case 3: blinkDash(); break;
    case 4: blinkDash(); break;
    case 5: blinkDash(); break;
    case 6: blinkDot(); break;
    case 7: blinkDot(); break;
    case 8: blinkDot(); break;
    case 9:
      digitalWrite(ledPin,LOW);
      if(nowT - lastT > sosEndPause){
        stage = 0;
        lastT = nowT;
      }
      break;
  }
}

void blinkDot(){
  digitalWrite(ledPin,HIGH);
  if(nowT - lastT > dotTime){
    digitalWrite(ledPin,LOW);
    if(nowT - lastT > dotTime + gapTime){
      stage++;
      lastT = nowT;
    }
  }
}

void blinkDash(){
  digitalWrite(ledPin,HIGH);
  if(nowT - lastT > dashTime){
    digitalWrite(ledPin,LOW);
    if(nowT - lastT > dashTime + gapTime){
      stage++;
      lastT = nowT;
    }
  }
}
