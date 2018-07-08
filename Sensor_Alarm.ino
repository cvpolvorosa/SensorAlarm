#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
DS3231  rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int interval = 150, intervalClock = 1000;
Time  t;
unsigned long prevMillis = 0, prevMillis2 = 0, lastDebounceTimeButton1 = 0;
boolean ledBlink = false;
byte ledState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  lcd.init();
  lcd.noBacklight();
  rtc.begin();

}

void loop() {
  int reading = analogRead(0);
  int buttonPress = analogRead(A2);
  //Serial.println(reading);
  if (reading < 900) {
    ledBlink = true;
    lcdGetTime();
  }
  blinkWithoutDelay(ledBlink);
  button(buttonPress);

  Serial.print(reading);
  Serial.print(" ");
  Serial.println(buttonPress);
  delay(20);
}


void lcdGetTime() {
  lcd.backlight();
  unsigned long currentMillis = millis();
  if (currentMillis - prevMillis2 >= intervalClock) {
    t = rtc.getTime();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(rtc.getMonthStr());
    lcd.print(" ");
    lcd.print(t.date, DEC);
    lcd.print(" ");
    lcd.print(t.year, DEC);

    lcd.setCursor(0, 1);
    lcd.print(t.hour, DEC);
    lcd.print(":");
    lcd.print(t.min, DEC);
    lcd.print(":");
    lcd.print(t.sec, DEC);
  }
}


void blinkWithoutDelay(boolean ledBlink) {
  unsigned long currentMillis = millis();
  if (ledBlink == true) {
    if (currentMillis - prevMillis >= interval) {
      prevMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
    }
  }
  else {
    ledState = LOW;
  }
  digitalWrite(5, ledState);
}

void button(int value) {
  int reading = value;
  unsigned long currentMillis = millis();
  if (reading >= 500) {
    if ((currentMillis - lastDebounceTimeButton1) >= interval) {
      lastDebounceTimeButton1 = currentMillis;
      ledBlink = false;
    }
  }
}

