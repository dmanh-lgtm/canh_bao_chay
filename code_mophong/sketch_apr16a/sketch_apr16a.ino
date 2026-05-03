#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int relayQ = 2;
const int relayB = 7;
const int flame_sensor = 3;
const int gas_sensor = 4;
const int button1 = 5;
const int button2 = 6;
const int buzzer = 10;

const unsigned long runTime  = 3000;
const unsigned long restTime = 3000;

unsigned long fanStartTime = 0;
bool fanIsRunning = false;
bool fanInRest    = false;

unsigned long pumpStartTime = 0;
bool pumpIsRunning = false;
bool pumpInRest    = false;

unsigned long lastLcdUpdate = 0;

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
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  lcd.print("OK");
  delay(1000);
  lcd.clear();
}

void loop() {
  unsigned long currentTime = millis();

  bool coLua  = (digitalRead(flame_sensor) == HIGH || digitalRead(button1) == LOW);
  bool coKhoi = (digitalRead(gas_sensor)   == HIGH || digitalRead(button2) == LOW);

  bool fanTrigger = coKhoi || coLua;

  //logic bật tắt quạt
  if (!fanIsRunning && !fanInRest) {
    if (fanTrigger) {
      digitalWrite(relayQ, HIGH);
      fanIsRunning = true;
      fanStartTime = currentTime;
    }
  }
  else if (fanIsRunning) {
    if (currentTime - fanStartTime >= runTime) {
      digitalWrite(relayQ, LOW);
      fanIsRunning = false;
      fanInRest    = true;
      fanStartTime = currentTime;
    }
  }
  else if (fanInRest) {
    if (currentTime - fanStartTime >= restTime) {
      fanInRest = false;
    }
  }

  //logic bật tắt bơm
  if (!pumpIsRunning && !pumpInRest) {
    if (coLua) {
      digitalWrite(relayB, HIGH);
      pumpIsRunning = true;
      pumpStartTime = currentTime;
    }
  }
  else if (pumpIsRunning) {
    if (currentTime - pumpStartTime >= runTime) {
      digitalWrite(relayB, LOW);
      pumpIsRunning = false;
      pumpInRest    = true;
      pumpStartTime = currentTime;
    }
  }
  else if (pumpInRest) {
    if (currentTime - pumpStartTime >= restTime) {
      pumpInRest = false;
    }
  }

  //logic còi
  if (fanIsRunning || pumpIsRunning) {
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
  }

  if (currentTime - lastLcdUpdate >= 500) {
    lastLcdUpdate = currentTime;

    lcd.setCursor(0, 0);
    if (coLua && coKhoi) {
      lcd.print("LUA, KHOI      ");
    } else if (coLua) {
      lcd.print("CANH BAO: CHAY ");
    } else if (coKhoi) {
      lcd.print("CANH BAO: KHOI ");
    } else if (fanInRest || pumpInRest) {
      lcd.print("DANG NGHI...   ");
    } else {
      lcd.print("HE THONG: OK   ");
    }

    lcd.setCursor(0, 1);
    lcd.print(fanIsRunning  ? "QUAT:ON  " : "QUAT:OFF ");
    lcd.print(pumpIsRunning ? "BOM:ON " : "BOM:OFF");
  }
}