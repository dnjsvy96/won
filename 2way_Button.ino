/*
D1mini PINMAP IoTSwitch2wayFB.ino
Tx  1
Rx  3
D1  5 (SCL)LED
D2  4 (SDA)button
D3  0 FLASH 
D4  2 <builtin LED>  
A0  ADC0
D0  16 wake 
D5  14 SCK  
D6  12 MISO 
D7  13 MOSI 
D8  15 SS 
firebase esp8266 client library
 */
//Firebase IoT Switch  

#include <FirebaseESP8266.h>
#include  <ESP8266WiFi.h>
#define WIFI_SSID "KT_GiGA_2G_wons"//"<Your Wifi SSID>"
#define WIFI_PASS "Dnjsvy0221"//"<Your WiFi Password>"

#define FIREBASE_HOST "home-b1262-default-rtdb.firebaseio.com" //Firebase Project URL:Remove "https:", "/"
#define FIREBASE_AUTH "PKM52IzlheH6MvLvRpPii72sHfHRDRApWWBe3Lmr"      //Firebase Auth Token

FirebaseData fbData;

//If using Relay Module
int LED = D4; //initialize 2 Pin
int SW1 = D2;
String state = "0";
String swState = "0";
bool X = LOW;

void setup() {
  
 Serial.begin(9600);
 
   WiFi.begin (WIFI_SSID, WIFI_PASS);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println ("");
  Serial.println ("WiFi Connected!");
  
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
    pinMode(LED,OUTPUT);// 
    digitalWrite(LED,HIGH); 
    pinMode(SW1,INPUT_PULLUP);// 

}

void loop() {

  if (Firebase.getString(fbData,"/Voice/LED")) {
    state = fbData.stringData();
    if (state=="1"){
      digitalWrite(LED,LOW); //Device1 is ON
    } else if (state=="0"){
      digitalWrite(LED,HIGH);//Device1 if OFF
    }
  }

  if(digitalRead(SW1) == HIGH){
    while(digitalRead(SW1) != LOW){
      delay(5);
    }
    swState = (state=="1")? "0" : "1";
    fbWrite(swState);
  }
}

void fbWrite(String s){
  if (Firebase.setString(fbData,"/Voice/LED", s))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + fbData.dataPath());
    Serial.println("TYPE: " + fbData.dataType());
    Serial.println("ETag: " + fbData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  } 
}
/*
 * void loop() {
  if (Firebase.get(fbData,"/Voice/LED")) {
    if (fbData.dataType() == "string") {
      String state1 = fbData.stringData();
      if (state1=="1"){
        digitalWrite(LED,LOW); //Device1 is ON
      } else if (state1=="0"){
        digitalWrite(LED,HIGH);//Device1 if OFF
    }}
  }
}
*/