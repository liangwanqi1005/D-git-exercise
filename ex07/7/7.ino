#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "fulushouximzs";
const char* password = "lwq200606";

// 引脚定义
#define LED_PIN 2
WebServer server(80);
int brightness = 0;  // 存储LED亮度0~255

// 网页页面，带滑动条range
String buildHtml(){
  String page = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>网页无极调光</title>
<style>
body{text-align:center;font-size:22px;margin-top:80px;}
input{width:80%;height:30px;}
</style>
</head>
<body>
<h2>LED无极调光滑块</h2>
<p>当前亮度值：<span id="val">0</span></p>
<input type="range" min="0" max="255" id="slider">

<script>
let slider = document.getElementById("slider");
let showVal = document.getElementById("val");
// 滑动条拖动触发
slider.oninput = function(){
  let v = this.value;
  showVal.innerText = v;
  // GET请求发送亮度数值给ESP32
  fetch("/set?b="+v);
}
</script>
</body>
</html>
)HTML";
  return page;
}

// 首页路由
void handleRoot(){
  server.send(200,"text/html;charset=utf-8",buildHtml());
}

// 接收滑块亮度参数路由
void handleSetBright(){
  if(server.hasArg("b")){
    brightness = server.arg("b").toInt();
    ledcWrite(LED_PIN,brightness);
  }
  // 刷新页面
  server.sendHeader("Location","/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN,OUTPUT);
  ledcAttach(LED_PIN,5000,8);
  ledcWrite(LED_PIN,0);

  // 连接WiFi
  Serial.print("正在连接WiFi:");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(400);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功！IP地址：");
  Serial.println(WiFi.localIP());

  // 绑定网页路由
  server.on("/",handleRoot);
  server.on("/set",handleSetBright);
  server.begin();
}

void loop() {
  server.handleClient();
}