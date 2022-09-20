#include <ESP8266WiFi.h>        //ESP8266 라이브러리
#include <WiFiClient.h>         //와이파이 연결 라이브러리
#include <ESP8266WebServer.h>   //서버 생성 라이브러리
#include <SoftwareSerial.h>

//SSID와 패스워드 설정
#define STASSID "Atomy"
#define STAPSK  "ks123456789"
//#define STASSID "U+Net2493"
//#define STAPSK  "k1025423s."
//#define STASSID "iptime"
//#define STAPSK  ""

// 웹 서버 선언
ESP8266WebServer server(80);
SoftwareSerial mySerial(10,11);

const char *ssid = STASSID;
const char *password = STAPSK;
int stallState[3];
String state = "00";
/*
 * 0 : 사용가능
 * 1 : 사용중
 * 2 : 비상
 */

/*
char state;
int rPin=13;
int piezo=12;
unsigned long past = 0;
int ledState = LOW;
*/

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

  //클라이언트가 루트를 요청할 때 handleRoot함수를 실행하도록 설정
  server.on("/", handleRoot);
  //클라이언트가 다른 주소를 요청할 때 handleNotFound함수를 실행하도록 설정
  server.onNotFound(handleNotFound);

  //서버 시작
  server.begin();
  Serial.println("Server started");
}

//정상경로
void handleRoot() {
  server.send(200, "text/plain", state);
  Serial.println(state);
}

//비정상경로
void handleNotFound() {
  String message = "File Not Found";
  server.send(404, "text/plain", message);
}
/////////////////
//서버 관련 함수 끝//
/////////////////

void setup(void) {
  //서버 세팅 함수 호출
  server_setting();

  /*
  mySerial.begin(2400);
  pinMode(rPin, OUTPUT);
  pinMode(piezo, OUTPUT);
  */
}

void loop(void) {
  //지속적으로 서버 통신
  server.handleClient();
  
  //시리얼 값을 받아서 서버로 정보 보냄
  if(Serial.available() > 0) { //수신되었는지 상태 확인
    state = Serial.readStringUntil('\n');
    if(state.length() > 3){
      Serial.println("다시 입력해주세요");
      state = "00";
    }
    else{
      Serial.print(state + "로 설정했습니다.");
    }
  }
  
  /*
  if(mySerial.available() > 0) { //수신되었는지 상태 확인
    state = mySerial.read();
    
    unsigned long now = millis();
    
    if(state == '1') {
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
      Serial.println(now);
    }
    
    else if(state == '2'){
      digitalWrite(piezo, LOW);
    }
  }
  */
}
