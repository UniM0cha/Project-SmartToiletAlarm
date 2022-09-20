#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//SSID와 패스워드 설정
//#define STASSID "Atomy5G"
//#define STAPSK  "ks123456789"
#define STASSID "U+Net2493"
#define STAPSK  "k1025423s."
//#define STASSID "iptime"
//#define STAPSK  ""

const char *ssid = STASSID;
const char *password = STAPSK;

// 웹 서버 선언
ESP8266WebServer server(80);

//웹 페이지 띄우기
void handleRoot() {
  char data[400];

  if(Serial.available() > 0) { //수신되었는지 상태 확인
    int state = Serial.read();

    if(state == '1') {
      snprintf(data, 400, "데이터 1");
      server.send(200, "text/html", data);
      Serial.println("1을 서버로 전송");
    }
    else if(state == '2'){
      snprintf(data, 400, "데이터 2");
      server.send(200, "text/html", data);
      Serial.println("2를 서버로 전송");
    }

  }
  else{
      snprintf(data, 400, "입력되지 않음");
      server.send(200, "text/html", data);
      Serial.println("else");
  }
}

// 페이지가 없을 때
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // WIFI 연결될 때까지 대기
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  //서버 시작
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
