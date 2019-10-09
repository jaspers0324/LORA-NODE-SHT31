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

   //測試30-70%溼度補償值 
  float HA = 0.0; //1% SHT31 與 Rotronic 器差值
  float HB = 0.0; //5% SHT31 與 Rotronic 器差值
  float HC = 0.0; //10% SHT31 與 Rotronic 器差值
  float HD = 0.0; //20% SHT31 與 Rotronic 器差值
  float HE = 2.6; //30% SHT31 與 Rotronic 器差值 
  float HF = 2.4; //40% SHT31 與 Rotronic 器差值 
  float HG = 2.4; //50% SHT31 與 Rotronic 器差值 
  float HH = 3.1; //60% SHT31 與 Rotronic 器差值 
  float HI = 0.5; //70% SHT31 與 Rotronic 器差值
  
  float HZ; //濕度減掉器差值結果

  if (! isnan(t)) {  // check if 'is not a number'
    //Serial.print("Temp *C = "); 
    //Serial.print(t);
  } else { 
    //Serial.println("Failed to read temperature");
  }

  //Serial.print(',');
  
  if (! isnan(h)) {  // check if 'is not a number'
    //Serial.print("Hum. % = "); 
    //Serial.println(HZ);
  } else { 
    //Serial.println("Failed to read humidity");
  }
  //Serial.println();
  //delay(1000);



  // 校正區間30%
  // 精準度設定+-2%
  if (h>30 && h<39) {  
    //Serial.print("Hum. % = "); 
    HZ = h-HE;
    Serial.println(HZ,1); 
    
  } else { 
    //Serial.println(h,1);
  }


  // 校正區間40%
  // 精準度設定+-2%
  if (h>40 && h<49) {  
    //Serial.print("Hum. % = "); 
    HZ = h-HF;
    Serial.println(HZ,1); 
    
  } else { 
    //Serial.println(h,1);
  }

  // 校正區間50%
  // 精準度設定+-2%
  if (h>50 && h<59) {  
    //Serial.print("Hum. % = "); 
    HZ = h-HG;
    Serial.println(HZ,1); 
    
  } else { 
    //Serial.println(h,1);
  }

  // 校正區間60%
  // 精準度設定+-2%
  if (h>60 && h<66) {  
    //Serial.print("Hum. % = "); 
    HZ = h-HH;
    Serial.println(HZ,1); 
    
  } else { 
    //Serial.println(h,1);
  }

  // 校正區間70%
  // 精準度設定+-2%
  if (h>66 && h<71) {  
    //Serial.print("Hum. % = "); 
    HZ = h-HI;
    Serial.println(HZ,1); 
    
  } else { 
    //Serial.println(h,1);
  }

  

  //Serial.print("Sending packet: ");
  //Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("CAL1000");
  LoRa.print(",");
  LoRa.print(t,1); //顯示小數點後一位
  LoRa.print(",");
  LoRa.print(HZ,1); //顯示小數點後一位
  LoRa.print(",");
  LoRa.println();
  //LoRa.print(counter);
  LoRa.endPacket();
  //counter++;
  delay(1000);
}
