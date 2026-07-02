#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int lm35Pin = A0;
const int relayPin = 8;

// GRANICE ZA HLAĐENJE (Histereza):
const float pragPali = 32.0; // Pali motor
const float pragGasi = 28.0; // Ugasi motor

bool motorRadi = false;
bool motorUgasenZbogZastite = false; 

void setup() {
  Serial.begin(9600);

  // KLJUČNO: Koristimo stabilnih 1.1V
  analogReference(INTERNAL);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Inicijalno ugašeno

  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(0, 1);
  lcd.print("Motor: OFF");
}

void loop() {
  // Čitanje sa filterom (30 uzoraka) za stabilnost
  long suma = 0;
  for (int i = 0; i < 30; i++) {
    suma += analogRead(lm35Pin);
    delay(1);
  }
  float prosecanAnalog = (float)suma / 30.0;

  // NOVA MATEMATIKA ZA 1.1V REFERENCU:
  // vout = prosecanAnalog * (1.1 / 1023.0)
  // tempC = vout * 100.0 -> što je isto što i:
  float tempC = prosecanAnalog * (110.0 / 1023.0);

  // Slanje za Octave
  Serial.println(tempC);

  // LCD prikaz temperature
  lcd.setCursor(6, 0);
  lcd.print(tempC, 1);
  lcd.print((char)223);
  lcd.print("C ");

  // LOGIKA ZAŠTITE
  if (tempC >= pragPali) {
    motorUgasenZbogZastite = true; // Pretoplo
  }
  else if (tempC <= pragGasi) {
    motorUgasenZbogZastite = false; // Ohlađeno
  }

  // Izvršavanje na releju (HIGH = MOTOR RADI)
  lcd.setCursor(7, 1);
  if (motorUgasenZbogZastite) {
    digitalWrite(relayPin, LOW); // KADA JE UGAŠEN
    lcd.print("OFF");
  } else {
    digitalWrite(relayPin, HIGH); // KADA JE UKLJUČEN
    lcd.print("ON ");
  }

  delay(500);
}