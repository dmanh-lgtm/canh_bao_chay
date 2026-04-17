#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long preTimeQ = 0;
unsigned long preTimeB = 0;
const long interval = 500;

const int relayQ = 2;
const int relayB = 7;
const int flame_sensor = 3;
const int gas_sensor = 4;
const int button1 = 5;
const int button2 = 6;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(relayQ, OUTPUT);
  pinMode(relayB, OUTPUT);
  digitalWrite(relayQ, LOW);
  digitalWrite(relayB, LOW);
  pinMode(flame_sensor, INPUT);
  pinMode(gas_sensor, INPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  
  lcd.print("OK");
  delay(1000);
  lcd.clear();
}

void loop() {
  unsigned long currentTime = millis();
  
  bool coLua = (digitalRead(flame_sensor) == HIGH || digitalRead(button1) == LOW);
  bool coKhoi = (digitalRead(gas_sensor) == HIGH || digitalRead(button2) == LOW);

  if (coLua) {
    digitalWrite(relayQ, HIGH);
    preTimeQ = currentTime;
  } else {
    if (currentTime - preTimeQ >= interval) {
      digitalWrite(relayQ, LOW);
    }
  }

  if (coKhoi) {
    digitalWrite(relayB, HIGH);
    preTimeB = currentTime;
  } else {
    if (currentTime - preTimeB >= interval) {
      digitalWrite(relayB, LOW);
    }
  }
lcd.setCursor(0, 0);
  if (coLua && coKhoi) {
    lcd.print("LUA, KHOI      ");
  } 
  else if (coLua) {
    lcd.print("CANH BAO: CHAY ");
  } 
  else if (coKhoi) {
    lcd.print("CANH BAO: KHOI ");
  } 
  else {
    lcd.print("HE THONG: OK   ");
  }

  lcd.setCursor(0, 1);
  if (digitalRead(relayQ)) {
    lcd.print("QUAT:ON  ");
  } 
  else {
    lcd.print("QUAT:OFF ");
  }

  if (digitalRead(relayB)) {
    lcd.print("BOM:ON ");
  } 
  else {
    lcd.print("BOM:OFF");
  }
  delay(50);
}