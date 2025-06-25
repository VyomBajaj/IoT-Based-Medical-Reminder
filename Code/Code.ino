#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
 
LiquidCrystal lcd(2,3,4,5,6,7);
RTC_DS1307 RTC;
int temp, inc, hours1, minut,sec1, add = 11;
int next = A0;
int INC = A1;
int DECR = A2;  
int set_mad = A3;
const int trigPin = 1;
const int echoPin = 0;
long duration;
int distance;

#define box1 13
#define box2 12
#define box3 11
#define buzzer 8
 
int HOUR, MINUT, SECOND;
 
void setup() {
  Wire.begin();
  RTC.begin();
  lcd.begin(16, 2);
  pinMode(INC, INPUT_PULLUP);
  pinMode(next, INPUT_PULLUP);
  pinMode(DECR, INPUT_PULLUP);
  pinMode(set_mad, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(box1, OUTPUT);
  pinMode(box2, OUTPUT);
  pinMode(box3, OUTPUT);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(10, INPUT); // Sets the IR as an Input
  pinMode(9, OUTPUT); // Sets the Pin as an Output
 
  lcd.setCursor(0, 0);
  lcd.print("Medicine reminder");
  lcd.setCursor(0, 1);
  lcd.print(" Using Arduino  ");
  delay(100);
  
}
 
void loop() {
  int temp = 0, val = 1, temp4;
  DateTime now = RTC.now();
  if (digitalRead(set_mad) == 0)  
  {
    lcd.setCursor(0, 0);
    lcd.print("  Set Medicine  ");
    lcd.setCursor(0, 1);
    lcd.print("  Reminder time ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time 1");
    defualt();
    time(1);
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time 2");
    defualt();
    delay(500);
    time(2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time 3");
    defualt();
    time(3);
    lcd.setCursor(0, 0);
    lcd.print("Medicine reminder");
    lcd.setCursor(0, 1);
    lcd.print(" time has been set ");
    delay(500);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.setCursor(6, 0);
  lcd.print(HOUR = now.hour(), DEC);
  lcd.print(":");
  lcd.print(MINUT = now.minute(), DEC);
  lcd.print(":");
  lcd.print(SECOND = now.second(), DEC);
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.year(), DEC);
  match();
  delay(100);
}
 
void defualt() {
  lcd.setCursor(0, 1);
  lcd.print(HOUR);
  lcd.print(":");
  lcd.print(MINUT);
  lcd.print(":");
  lcd.print(0);
}
void time(int x) {
  int temp = 1, minuts = 0, hours = 0, seconds = 0;
  while (temp == 1) {
    if (digitalRead(INC) == 0) {
      HOUR++;
      if (HOUR == 24) {
        HOUR = 0;
      }
      while (digitalRead(INC) == 0)
        ;
    }
    if (digitalRead(DECR) == 0) {  
      HOUR--;
      if (HOUR == -1) {
        HOUR = 23;
      }
      while (digitalRead(DECR) == 0)
        ;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time ");
    lcd.print(x);
    lcd.setCursor(0, 1);
    lcd.print(HOUR);
    lcd.print(":");
    lcd.print(MINUT);
    lcd.print(":");
    lcd.print(SECOND);
    delay(100);
    if (digitalRead(next) == 0) {
      hours1 = HOUR;
      EEPROM.write(add++, hours1);
      temp = 2;
      while (digitalRead(next) == 0)
        ;
    }
  }
  while (temp == 2) {
    if (digitalRead(INC) == 0) {
      MINUT++;
      if (MINUT == 60) { MINUT = 0; }
      while (digitalRead(INC) == 0)
        ;
    }
    if (digitalRead(DECR) == 0) { 
      MINUT--;
      if (MINUT == -1) {
        MINUT = 59;
      }
      while (digitalRead(DECR) == 0)
        ;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Time ");
    lcd.print(x);
    lcd.setCursor(0, 1);
    lcd.print(HOUR);
    lcd.print(":");
    lcd.print(MINUT);
    lcd.print(":");
    lcd.print(SECOND);
    delay(100);
    if (digitalRead(next) == 0) {
      minut = MINUT;
      EEPROM.write(add++, minut);
      temp = 0;
      while (digitalRead(next) == 0)
        ;
    }
  }
 

  delay(500);
}
bool alarmActive = true;  // Global flag to track if alarm is active
int k=0;
int count=0;
int k2=0;
int count2=0;
int count3=0;
int k3=0;
void match() {
 

  int tem[17];
  for (int i = 11; i < 17; i++) {
    tem[i] = EEPROM.read(i);
  }
  if ((HOUR == tem[11] && MINUT == tem[12]) || 
      (HOUR == tem[13] && MINUT == tem[14]) ||
      (HOUR == tem[15] && MINUT == tem[16])) {
    
    while (alarmActive) {
      // Group One Alarm
      if (HOUR == tem[11] && MINUT == tem[12]) {
        if (digitalRead(set_mad) == 0 && count==0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Alarm turned off");
          digitalWrite(9,LOW);
          count=1;
          return;
        }
      
        if(count!=1){
          k=waterDispense();
        
          if (k==1){
            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Alarm turned off");
            count=1;
            return ;
          }
          beep();
          firstBox();
          lcd.setCursor(0, 0);
          lcd.print("  Take Group One  ");
          lcd.setCursor(0, 1);
          lcd.print("     Medicine     ");
        }
        if(count==1){
          return;
        }
        
      }
      
      // Group Two Alarm
      if (HOUR == tem[13] && MINUT == tem[14]) {
        if (digitalRead(set_mad) == 0 && count2==0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Alarm turned off");
          digitalWrite(9,LOW);
          count2=1;
          return;
        }
      
        if(count2!=1){
          k2=waterDispense();
        
          if (k2==1){
            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Alarm turned off");
            count2=1;
            return ;
          }
          beep();
          secondBox();
          lcd.setCursor(0, 0);
          lcd.print("  Take Group two  ");
          lcd.setCursor(0, 1);
          lcd.print("     Medicine     ");
        }
        if(count2==1){
          return;
        }
      }

      // Group Three Alarm
      if (HOUR == tem[15] && MINUT == tem[16]) {
        if (digitalRead(set_mad) == 0 && count3==0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Alarm turned off");
          digitalWrite(9,LOW);
          count3=1;
          return;
        }
      
        if(count3!=1){
          k=waterDispense();
        
          if (k3==1){
            
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Alarm turned off");
            count3=1;
            return ;
          }
          beep();
          thirdBox();
          lcd.setCursor(0, 0);
          lcd.print("  Take Group three  ");
          lcd.setCursor(0, 1);
          lcd.print("     Medicine     ");
        }
        if(count3==1){
          return;
        }
      }
      delay(100);  // Short delay for continuous loop
    }
  }
  
}

void firstBox(){
  digitalWrite(box1, HIGH);
  delay(500);
  digitalWrite(box1, LOW);
  delay(500);
}
void secondBox(){
  digitalWrite(box2, HIGH);
  delay(500);
  digitalWrite(box2, LOW);
  delay(500);
}
void thirdBox(){
  digitalWrite(box3, HIGH);
  delay(500);
  digitalWrite(box3, LOW);
  delay(500);
}

void beep() {
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(500);
}
int waterDispense(){
  int IR = digitalRead(10);
  
  if(IR == HIGH)
  {
    Serial.print("Glass Avaiable   ");
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance in cm
  distance = duration*0.034/2;
  
  int D = map(distance, 10, 1095, 0, 400); 
  
  if(D < 200)
  {
    Serial.println("Pump: Start");
    digitalWrite(9,HIGH);
    
  }
  else
  {
    Serial.println("Glass: Full ");
    digitalWrite(9,LOW);
    return 1;
  }
 }
 else
  {
    Serial.println("No Glass ");
    digitalWrite(9,LOW);
  }
  return 0;
}