#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <INA219.h>
#include "RTClib.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
INA219 ina1(0x40);
INA219 ina2(0x44);
RTC_DS3231 rtc;

float volt1, volt2;
int tanggal, bulan, tahun, jam, menit, detik;

const int relay1 = 2;
const int relay2 = 3;

void setup() {
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("SOLAR CHARGE");
  lcd.setCursor(3,1);
  lcd.print("CONTROLLER");
  delay(2000);
  if (! rtc.begin()) {
    Serial.println("RTC Tidak Ditemukan");
    Serial.flush();
    abort();
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  ina1.begin();
  ina2.begin();
  
  lcd.clear();
  lcd.setCursor(7,0);
  lcd.print("GET");
  lcd.setCursor(5,1);
  lcd.print("STARTED");
  delay(2000);
}

void loop() {
  lcd.clear();
  readDateTime();
  readVolt();
  delay(2000);
  lcd.clear();
  cekStatus();
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Volt1 : ");
  lcd.print(volt1);
  lcd.setCursor(0,1);
  lcd.print("Volt2 : ");
  lcd.print(volt2);
  delay(2000);
}

void readVolt(){
  volt1 = ina1.busVoltage();
  volt2 = ina2.busVoltage();
}

void readDateTime(){
  DateTime now = rtc.now();
  tanggal = now.day();
  bulan   = now.month();
  tahun   = now.year();
  jam     = now.hour();
  menit   = now.minute();
  detik   = now.second();
  lcd.setCursor(3,0);
  lcd.print(String()+tanggal+"/"+bulan+"/"+tahun);
  lcd.setCursor(4,1);
  lcd.print(String()+jam+":"+menit+":"+detik);
}

void cekStatus(){
  lcd.print("STATUS:");
  if ((jam >= 6 && jam <= 18)){
    r1("OFF");
    digitalWrite(relay1,HIGH);
  }
  else{
    r1("ON");
    digitalWrite(relay1,LOW);
  }
  if(volt1 < 8.0){
    r2("OFF");
    digitalWrite(relay2,HIGH);
  }
  else{
    r2("ON");
    digitalWrite(relay2,LOW);
  }
}

void r1(String statusR1){
  lcd.setCursor(8,0);
  lcd.print("R1: ");
  lcd.print(statusR1);
}

void r2(String statusR2){
  lcd.setCursor(8,1);
  lcd.print("R2: ");
  lcd.print(statusR2);
}
