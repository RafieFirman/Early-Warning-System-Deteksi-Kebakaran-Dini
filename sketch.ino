#include <LiquidCrystal_I2C.h>

#define PIN_NTC     A0
#define PIN_MQ2     A1
#define PIN_BUZZER  7
#define PIN_RELAY   8
#define PIN_LED     6

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Suhu
const float SUHU_NORMAL_MAX = 35.0;
const float SUHU_SANGAT_TINGGI = 50.0;

// Asap
const float ASAP_RENDAH_MAX = 500.0;
const float ASAP_TINGGI_MIN = 1500.0;

// Konstanta sensor suhu
const float BETA = 3950.0;
const float T0 = 298.15;

// Konfig sensor asap
const float MQ2_PPM_MIN = 0.0;
const float MQ2_PPM_MAX = 5000.0;


void tampilLCD(String baris1, String baris2) {

  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print(baris1);

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(baris2);
}


void setup() {
  Serial.begin(9600);

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

// Akuator Startup
  digitalWrite(PIN_RELAY, LOW);
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);

// LCD
  lcd.init();
  lcd.backlight();
  tampilLCD("M Rafie Firman R", "NIM: 24051204065");

  delay(3000);
  lcd.clear();
}


void loop() {

  int analogNTC = analogRead(PIN_NTC);
  if (analogNTC <= 0) {
    analogNTC = 1;
  }

  if (analogNTC >= 1023) {
    analogNTC = 1022;
  }

  float suhuKelvin = 1.0 / (log(1.0 / (1023.0 / analogNTC - 1.0)) / BETA + 1.0 / T0);
  float suhu = suhuKelvin - 273.15;

  int analogMQ2 = analogRead(PIN_MQ2);
  float asapPPM = map(analogMQ2, 0, 1023, MQ2_PPM_MIN, MQ2_PPM_MAX);

  bool suhuTinggi = suhu > SUHU_NORMAL_MAX;
  bool asapTinggi = asapPPM > ASAP_TINGGI_MIN;

  // KONDISI 4  SUHU TINGGI + ASAP TINGGI
  if (suhuTinggi && asapTinggi) {
    tampilLCD( "BAHAYA !!!", "SPRINKLER AKTIF");
    digitalWrite(PIN_RELAY, HIGH);

    tone(PIN_BUZZER, 1200);
    digitalWrite(PIN_LED, HIGH);
    delay(100);

    noTone(PIN_BUZZER);
    digitalWrite(PIN_LED, LOW);
    delay(100);
  }

// KONDISI 3 SUHU TINGGI + ASAP RENDAH/SEDANG
  else if (suhuTinggi && !asapTinggi) {
    digitalWrite(PIN_RELAY, LOW);

    if (suhu >= SUHU_SANGAT_TINGGI) {
      tampilLCD("PERINGATAN SUHU", "SANGAT TINGGI !!!");
    } else {
      tampilLCD("PERINGATAN", "SUHU TINGGI !!!");
    }

    tone(PIN_BUZZER, 800);
    digitalWrite(PIN_LED, HIGH);
    delay(250);

    noTone(PIN_BUZZER);
    digitalWrite(PIN_LED, LOW);
    delay(250);
  }

// KONDISI 2 SUHU NORMAL + ASAP TINGGI
  else if (!suhuTinggi && asapTinggi) {
    digitalWrite(PIN_RELAY, LOW);
    tampilLCD("PERINGATAN", "ASAP TINGGI !!!");

    tone(PIN_BUZZER, 800);
    digitalWrite(PIN_LED, HIGH);
    delay(250);

    noTone(PIN_BUZZER);
    digitalWrite(PIN_LED, LOW);
    delay(250);
  }

// KONDISI 1 SUHU NORMAL + ASAP RENDAH
  else {
    digitalWrite(PIN_RELAY, LOW);
    digitalWrite(PIN_LED, LOW);
    noTone(PIN_BUZZER);
    tampilLCD("STATUS: AMAN", "NORMAL");

    delay(300);
  }
}