#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include "Jasper_SHT31.h"

const int csPin = 10;
const int resetPin = 9;       // LoRa radio reset
const int irqPin = 2;         // LoRa radio Interrupt 

Jasper_SHT31 sht31 = Jasper_SHT31();

void setup() {
  Serial.begin(9600);

  LoRa.setPins(csPin, resetPin, irqPin);

  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    //Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  
  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    //Serial.println("Starting LoRa failed!");
    while (1);
  }
}


void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

   
//溼度補償值 
  float HA = 3.0; //30% SHT31 與 Rotronic 器差值
  float HB = 2.5; //40% SHT31 與 Rotronic 器差值
  float HC = 1.5; //50% SHT31 與 Rotronic 器差值
  float HD = 0.5; //60% SHT31 與 Rotronic 器差值

  float HZ = h; //濕度減掉器差值結果


  
  if (! isnan(t)) {  // check if 'is not a number'
    //Serial.print("Temp *C = "); 
    //Serial.println(t);
  } else { 
    //Serial.println("Failed to read temperature");
  }

  //Serial.print(',');
  
  if (! isnan(h)) {  // check if 'is not a number'
    //Serial.print("Hum. % = "); 
    //Serial.println(h);
  } else { 
    //Serial.println("Failed to read humidity");
  }
  //Serial.println();
  //delay(1000);
  

  // 校正區間30-40%
  // 精準度設定+-2%
  if (h>30 && h<40) {  
    HZ = h-HA;
    Serial.println(HZ,1); 
  }


  // 校正區間40-50%
  // 精準度設定+-2%
  if (h>40 && h<50) {  
    HZ = h-HB;
    Serial.println(HZ,1); 
  }


  // 校正區間50-60%
  // 精準度設定+-2%
  if (h>50 && h<60) {  
    HZ = h-HC;
    Serial.println(HZ,1); 
  }


  // 校正區間60-70%
  // 精準度設定+-2%
  if (h>60 && h<70) {  
    HZ = h+HD;
    Serial.println(HZ,1); 
  }


  // send packet
  LoRa.beginPacket();
  LoRa.print("CAL1000");
  LoRa.print(",");
  LoRa.print(t,1); //顯示小數點後一位
  LoRa.print(",");
  LoRa.print(HZ,1); //顯示小數點後一位
  LoRa.print(",");
  LoRa.println();
  LoRa.endPacket();
  delay(1000);
}
