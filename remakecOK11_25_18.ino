#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


//declar ds3231 as rtc
DS3231 rtc(SDA, SCL);


// Set the LCD address to 0x27 in PCF8574 by NXP and Set to 0x3F in PCF8574A by Ti
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int upButton = 8;
int downButton = 9;
int selectButton = 10;
int backButton =  11;
int menu = 1; 
int pinBuzzer = 3;

byte DateIcon[] ={
  B00000,
  B01110,
  B11001,
  B10111,
  B11101,
  B10101,
  B01110,
  B00000
};

byte ClockIcon[] ={
  B00000,
  B01110,
  B10001,
  B10101,
  B11101,
  B10001,
  B01110,
  B00000
};

byte TempIcon[] ={
  B11000,
  B11100,
  B11100,
  B11001,
  B00100,
  B10010,
  B01001,
  B00100
};

byte CommaIcon[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01100,
  B00100,
  B01000
};

byte AlarmIcon[] ={
  B00100,
  B00000,
  B01110,
  B01010,
  B10001,
  B11111,
  B00000,
  B00000
};

byte ArrowIcon[] ={
  B10000,
  B11000,
  B11100,
  B11110,
  B11110,
  B11100,
  B11000,
  B10000
};

byte CircleIcon[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B01110,
  B00100,
  B00000,
  B00000
};

byte NothingIcon[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

Time t;
uint8_t hh = 0, mm = 0, ss = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.createChar(0, DateIcon);
  lcd.createChar(1, ClockIcon);
  lcd.createChar(2, TempIcon);
  lcd.createChar(3, CommaIcon);
  lcd.createChar(4, AlarmIcon);
  lcd.createChar(5, ArrowIcon);
  lcd.createChar(6, CircleIcon);
  lcd.createChar(7, NothingIcon);
  Serial.begin(115200);
  rtc.begin();
  pinMode(pinBuzzer, OUTPUT);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);
  Serial.begin(9600);
  digitalWrite(pinBuzzer, HIGH);
  updateMenu();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!digitalRead(downButton)){
    menu++;
    Serial.println(menu);
    updateMenu();
    delay(100);
    while (!digitalRead(downButton));
  }
  if (!digitalRead(upButton)){
    menu--;
    updateMenu();
    delay(100);
    while(!digitalRead(upButton));
  }
  if (!digitalRead(selectButton)){
    executeAction();
  }
  if (!digitalRead(backButton)){
    updateMenu();
    delay(100);
    while(!digitalRead(backButton));
  }

}

void printSerial(){
  Serial.print(rtc.getDOWStr());
  Serial.print("");

  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  Serial.println(rtc.getTimeStr());
  delay(1000);
  
}

void iconTemplateDashBoard(){
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  lcd.setCursor(11, 1);
  lcd.write(byte(2));
}

void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Time");
      lcd.setCursor(0, 1);
      lcd.print(" stopwatch");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Time");
      lcd.setCursor(0, 1);
      lcd.print(">stopwatch");
      break;
    case 3:
      menu = 3;
      break;
  }
}

void executeAction() {
  switch (menu) {
    case 1:
      TimeRtcLcd();
      break;
    case 2:
      stopwatch();
      break;
  }
}

void TimeRtcLcd(){
    lcd.clear();
    iconTemplateDashBoard();
    while(rtc.getTimeStr()){
      lcd.setCursor(1,1);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print(rtc.getDOWStr(1));
      lcd.setCursor(5, 0);
      lcd.write(byte(3));

      lcd.setCursor(6,0);
      lcd.print(rtc.getDateStr());

      lcd.setCursor(2,1);
      lcd.print(rtc.getTimeStr());
    
      lcd.setCursor(12, 1);
      lcd.print(rtc.getTemp());
      setAlarm(04, 30, 00);
      if (!digitalRead(backButton)){
        break;
        updateMenu();
      }
    }
   
}

void setAlarm(int h, int m, int s){
  digitalWrite(pinBuzzer, HIGH);
  
  t = rtc.getTime();
  int hours = h;
  int minute = m;
  int seconds = s;

  hh = t.hour,DEC;
  mm = t.min,DEC;
  ss = t.sec,DEC;

  
  if(hours == hh && minute == mm && seconds == ss){
      for(int i = 0; i <= 5 ; i++){
          lcd.setCursor(10, 1);
          lcd.write(byte(4));
          delay(500);
          lcd.setCursor(10, 1);
          lcd.write(byte(7));
          Serial.println("alarm ring");
          digitalWrite(pinBuzzer, HIGH);
          delay(500);
          digitalWrite(pinBuzzer, LOW);
          delay(500);
        }
    }
    
}

void stopwatch() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("stopwatch");
  lcd.setCursor(0,1);
  lcd.print("00:0");
  int hour = 0;
  int minute = 0;
  int seccond = 0;
  lcd.setCursor(2, 1);
  lcd.print(":");
  lcd.setCursor(5, 1);
  lcd.print(":");
  for(int i = 0 ; i < 60 ; i++ ){
      lcd.setCursor(6,1);
      lcd.print(i);
      delay(1000);
      if(i == 59 ){
          minute = minute + 1 ;
          lcd.setCursor(3,1);
          lcd.print(minute);
          i = 0;
          lcd.setCursor(7,1);
          lcd.print(" ");
          if(minute == 59 ){
            hour += 1;
            lcd.setCursor(2,1);
            lcd.print(hour);
            minute = 0;
            
           }
        }
        if (!digitalRead(backButton)){
          break;
          updateMenu();
        }
    }  
  
}


