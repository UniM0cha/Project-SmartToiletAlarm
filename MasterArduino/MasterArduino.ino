/*
isBeing = 0 : 사용가능
isBeing = 1 : 사용중
isBeing = 2 : 비상
*/

#include <Wire.h>

int rPin=13;
int piezo=12;
unsigned long past = 0;
int ledState = LOW;

//화장실의 상태를 저장하는 배열 변수
char isBeing[2];
//서버로 전송하기 위한 문자열을 저장하는 문자열 변수
String state;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  pinMode(rPin, OUTPUT);
  pinMode(piezo, OUTPUT);
}

void loop()
{ 
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

  state = "";
  for(int i=0 ; i<2 ; i++){
    state += isBeing[i];
  }
  Serial.println(state);
  
}
