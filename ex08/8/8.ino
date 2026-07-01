#include <WiFi.h>
#include <WebServer.h>

// ========== AP热点配置 ==========
const char* ap_ssid = "ESP32-031";
const char* ap_pass = "12345678";

// ========== 硬件参数 ==========
const int LED_PIN = 2;
const int TOUCH_PIN = 4;
const int TOUCH_THRESHOLD = 400;

WebServer server(80);
bool isArm = false;    // 布防标志
bool isAlarm = false;  // 报警标志
unsigned long lastFlash = 0;
const unsigned long flashGap = 80;

// 生成网页页面
String getHtmlPage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>触摸安防系统</title>
<style>
body {font-family:Arial; text-align:center; margin-top:70px;}
h1 {font-size:30px; color:#222;}
.state-arm {font-size:26px; margin:25px 0;}
.state-alarm {font-size:32px; font-weight:bold; margin:30px 0;}
button {font-size:22px; width:260px; padding:14px; margin:12px; border:none; border-radius:6px; cursor:pointer;}
.btn-arm {background:#009473; color:white;}
.btn-disarm {background:#d63031; color:white;}
</style>
</head>
<body>
<h1>ESP32触摸安防控制系统</h1>
<div class="state-arm">布防状态：<span id="armText"></span></div>
<div class="state-alarm" id="alarmText"></div>
<br>
<button class="btn-arm" onclick="goArm()">一键布防</button>
<br>
<button class="btn-disarm" onclick="goDisarm()">一键撤防</button>

<script>
// 定时刷新状态，无刷新更新页面文字
function refreshStatus(){
  fetch("/status")
  .then(res=>res.json())
  .then(data=>{
    document.getElementById("armText").innerText = data.armTxt;
    document.getElementById("armText").style.color = data.armColor;
    document.getElementById("alarmText").innerText = data.alarmTxt;
    document.getElementById("alarmText").style.color = data.alarmColor;
  })
}
// 布防跳转
function goArm(){
  fetch("/arm").then(()=>refreshStatus());
}
// 撤防跳转
function goDisarm(){
  fetch("/disarm").then(()=>refreshStatus());
}
setInterval(refreshStatus, 500);
window.onload = refreshStatus;
</script>
</body>
</html>
)rawliteral";
  return html;
}

// 主页路由
void handleRoot() {
  server.send(200, "text/html; charset=utf-8", getHtmlPage());
}

// 获取系统实时状态接口（返回JSON）
void handleStatus() {
  String armTxt, armColor, alarmTxt, alarmColor;
  if(isArm){
    armTxt = "已布防（警戒开启）";
    armColor = "#009473";
    if(isAlarm){
      alarmTxt = "⚠️ 触摸触发报警！";
      alarmColor = "#d63031";
    }else{
      alarmTxt = "正常，无触摸触发";
      alarmColor = "#333333";
    }
  }else{
    armTxt = "未布防（警戒关闭）";
    armColor = "#d63031";
    alarmTxt = "系统未开启布防";
    alarmColor = "#666666";
  }
  String jsonData = "{\"armTxt\":\""+armTxt+"\",\"armColor\":\""+armColor+"\",\"alarmTxt\":\""+alarmTxt+"\",\"alarmColor\":\""+alarmColor+"\"}";
  server.send(200, "application/json", jsonData);
}

// 布防接口
void handleArm() {
  isArm = true;
  isAlarm = false;
  digitalWrite(LED_PIN, LOW);
  server.send(200);
}

// 撤防接口
void handleDisarm() {
  isArm = false;
  isAlarm = false;
  digitalWrite(LED_PIN, LOW);
  server.send(200);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // 【热点创建逻辑完全复制你调光代码写法，无改动】
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("===== 安防系统热点启动 =====");
  Serial.print("热点名称："); Serial.println(ap_ssid);
  Serial.print("访问地址：http://"); Serial.println(WiFi.softAPIP());

  // 绑定网页路由
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long nowTime = millis();
  int touchValue = touchRead(TOUCH_PIN);

  // 仅布防状态下检测触摸报警
  if(isArm && !isAlarm){
    if(touchValue < TOUCH_THRESHOLD){
      delay(150);
      if(touchRead(TOUCH_PIN) < TOUCH_THRESHOLD){
        isAlarm = true;
        Serial.println("触摸触发报警！");
      }
    }
  }

  // 报警时LED闪烁
  if(isAlarm){
    if(nowTime - lastFlash >= flashGap){
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      lastFlash = nowTime;
    }
  }else{
    digitalWrite(LED_PIN, LOW);
  }
}