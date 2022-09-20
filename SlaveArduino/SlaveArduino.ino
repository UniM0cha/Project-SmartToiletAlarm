/*
isBeing = 0 : 사용가능
isBeing = 1 : 사용중
isBeing = 2 : 비상
*/

#include <Wire.h>

#define SLAVEID 1

int ledGreen = 6;
int ledRed = 13;
int piezo = 8;
int PIR = 12;
int sw = 2;
int val;

bool check = false;   //스위치가 눌렸는지 확인하는 변수
unsigned long past = 0;
unsigned long now = 0;
int ledState = LOW;
char isBeing = '0'; //master에 전송하는 변수

void setup()
{
  Wire.begin(SLAVEID);
  Wire.onRequest(sendToMaster);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(piezo, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(sw), warning, FALLING);
}

void sendToMaster(){
  Wire.write(isBeing);
}

void warning(){
  if(check == false){
    check = true;
  }
  else{
    check = false;
  }
}

void loop() {
  
  if(!check){
    //경보가 울리지 않는 상태 (스위치 안눌림)
    digitalWrite(piezo, LOW);  //부저 off
    val = digitalRead(PIR);   //pir로부터 값 받아옴
    if(val == HIGH){
      //pir센서가 감지되었을 때
      isBeing = '1';
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, LOW);
    }
    else{
      //pir센서가 감지되지 않았을 때
      isBeing = '0';
      digitalWrite(ledGreen, HIGH);
      digitalWrite(ledRed, LOW);
    }
  }
  else{
    //경보가 울리는 상태 (스위치 눌림)
    isBeing = '2';
    digitalWrite(piezo, HIGH); //부저 on
    
    //led 깜박깜박
    now = millis();
    digitalWrite(ledGreen, LOW);
    if(now - past >= 100){
      past = now;

      if(ledState == LOW){
        ledState = HIGH;
      }
      else{
        ledState = LOW;
      }
      digitalWrite(ledRed, ledState);
    }
  }
}
