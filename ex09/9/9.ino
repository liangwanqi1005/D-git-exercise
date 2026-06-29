#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "fulushouximzs";
const char* password = "lwq200606";

#define TOUCH_PIN 4
WebServer server(80);
int touchValue = 0;

// 首页HTML页面，JS自动轮询获取触摸值
String buildHtml(){
  String page = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>触摸实时数据仪表盘</title>
<style>
body{text-align:center;margin-top:100px;font-size:28px;}
#num{font-size:60px;color:#2266dd;font-weight:bold;}
</style>
</head>
<body>
<h2>ESP32触摸引脚实时数据</h2>
<p>当前触摸检测值：<span id="num">0</span></p>

<script>
// 每300毫秒自动读取一次数据，无刷新更新页面
function updateData(){
  fetch("/getTouch")
  .then(res=>res.text())
  .then(val=>{
    document.getElementById("num").innerText = val;
  })
}
setInterval(updateData,300);
</script>
</body>
</html>
)HTML";
  return page;
}

// 提供触摸数值接口
void handleGetTouch(){
  server.send(200,"text/plain",String(touchValue));
}

void handleRoot(){
  server.send(200,"text/html;charset=utf-8",buildHtml());
}

void setup() {
  Serial.begin(115200);

  // 连接WiFi
  Serial.print("连接WiFi：");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(400);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功，访问IP：");
  Serial.println(WiFi.localIP());

  // 绑定网页路由
  server.on("/",handleRoot);
  server.on("/getTouch",handleGetTouch);
  server.begin();
}

void loop() {
  server.handleClient();
  touchValue = touchRead(TOUCH_PIN);
}