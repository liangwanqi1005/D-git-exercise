#include <WiFi.h>
#include <WebServer.h>

// 修改为你自己的WiFi名称和密码
const char* ssid     = "fulushouximzs";
const char* password = "lwq200606";

#define LED_PIN 2
#define TOUCH_PIN 4
const int TOUCH_THRESHOLD = 400;

WebServer server(80);
bool isArm = false;
bool isAlarm = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.print("正在连接WiFi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功，访问IP：");
  Serial.println(WiFi.localIP());

  // 首页路由
  server.on("/", [](){
    String html;
    if(isArm){
      html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width'><style>body{text-align:center;margin-top:100px;font-size:26px;}button{font-size:24px;padding:18px 45px;margin:25px 15px;width:320px;}</style></head><body><h1>ESP32安防报警主机</h1><p>当前系统状态：<span style='color:green;font-weight:bold'>已布防</span></p><br><button onclick=\"location.href='/arm'\">布防 Arm</button><button onclick=\"location.href='/disarm'\">撤防 Disarm</button></body></html>";
    }else{
      html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width'><style>body{text-align:center;margin-top:100px;font-size:26px;}button{font-size:24px;padding:18px 45px;margin:25px 15px;width:320px;}</style></head><body><h1>ESP32安防报警主机</h1><p>当前系统状态：<span style='color:red;font-weight:bold'>未布防</span></p><br><button onclick=\"location.href='/arm'\">布防 Arm</button><button onclick=\"location.href='/disarm'\">撤防 Disarm</button></body></html>";
    }
    server.send(200, "text/html;charset=utf-8", html);
  });

  // 布防接口
  server.on("/arm", [](){
    isArm = true;
    isAlarm = false;
    server.sendHeader("Location","/");
    server.send(303);
  });

  // 撤防接口
  server.on("/disarm", [](){
    isArm = false;
    isAlarm = false;
    digitalWrite(LED_PIN, LOW);
    server.sendHeader("Location","/");
    server.send(303);
  });

  server.begin();
}

void loop() {
  server.handleClient();
  int touchVal = touchRead(TOUCH_PIN);

  // 仅布防状态触摸触发报警
  if(isArm && !isAlarm){
    if(touchVal < TOUCH_THRESHOLD){
      isAlarm = true;
    }
  }

  // 报警高频闪烁
  if(isAlarm){
    digitalWrite(LED_PIN, HIGH);
    delay(80);
    digitalWrite(LED_PIN, LOW);
    delay(80);
  }
}