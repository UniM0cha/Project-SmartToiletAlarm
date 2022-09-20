/*
isBeing = 0 : 사용가능
isBeing = 1 : 사용중
isBeing = 2 : 비상
*/
#include <ESP8266WiFi.h>        //ESP8266 라이브러리
#include <WiFiClient.h>         //와이파이 연결 라이브러리
#include <ESP8266WebServer.h>   //서버 생성 라이브러리
#include <SoftwareSerial.h>
#include <Wire.h>

//SSID와 패스워드 설정
#define STASSID "Atomy"
#define STAPSK  "ks123456789"
//#define STASSID "U+Net2493"
//#define STAPSK  "k1025423s."
//#define STASSID "iptime"
//#define STAPSK  ""

#define PORT 80

ESP8266WebServer server(PORT);

int rPin=13;
int piezo=12;
unsigned long past = 0;
int ledState = LOW;
const char *ssid = STASSID;
const char *password = STAPSK;

//화장실의 상태를 저장하는 배열 변수
char isBeing[2];
//서버로 전송하기 위한 문자열을 저장하는 문자열 변수
String state = "";

void setup()
{
  Wire.begin();
  pinMode(rPin, OUTPUT);
  pinMode(piezo, OUTPUT);
  
  //서버 세팅 함수 호출
  server_setting();
}

void loop()
{ 
  //지속적으로 서버 통신
  server.handleClient();

  //I2C 통신
  Wire.requestFrom(1,1);
  if(Wire.available() > 0){
    isBeing[0] = Wire.read();
  }
  
  Wire.requestFrom(2,1);
  if(Wire.available() > 0){
    isBeing[1] = Wire.read();
  }
  
  if (isBeing[0] == '2' || isBeing[1] == '2'){
    //비상상황
    unsigned long now = millis();
    
    digitalWrite(piezo, HIGH);
    if(now - past >= 100){
      past = now;

      if(ledState == LOW){
        ledState = HIGH;
      }
      else{
        ledState = LOW;
      }
      digitalWrite(rPin, ledState);
    }
  }
  else{
    //평상시
    digitalWrite(piezo, LOW);
    digitalWrite(rPin, LOW);
  }

  //isBeing으로 state 문자열 생성
  state = "";
  for(int i=0 ; i<2 ; i++){
    state += isBeing[i];
  }
  Serial.println(state + "정보 들어옴");
  
}

////////////////
//서버 관련 함수들//
////////////////

//서버 초기 세팅
void server_setting() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // WIFI 연결될 때까지 대기
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //WIFI의 정보와 아이피 출력
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //클라이언트가 정상경로를 요청할 때 handleRoot함수를 실행하도록 설정
  server.on("/", handleRoot);
  //클라이언트가 비정상 경로를 요청할 때 handleNotFound함수를 실행하도록 설정
  server.onNotFound(handleNotFound);

  //서버 시작
  server.begin();
  Serial.println("Server started");
}

//정상경로
void handleRoot() {
  server.send(200, "text/plain", state);
  Serial.println(state + "를 서버로 전송");
}

//비정상경로
void handleNotFound() {
  String message = "File Not Found";
  server.send(404, "text/plain", message);
}
/////////////////
//서버 관련 함수 끝//
/////////////////
