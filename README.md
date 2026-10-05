# Early Warning System: Deteksi Kebakaran Dini

Sistem **Early Warning System (EWS) Deteksi Kebakaran Dini** berbasis **Arduino Mega 2560** yang dirancang untuk mendeteksi kondisi suhu dan asap secara bersamaan. Sistem memanfaatkan **NTC Temperature Sensor** untuk membaca suhu dan **MQ-2 Gas Sensor** untuk mendeteksi tingkat asap/gas. Hasil pembacaan kedua sensor digunakan untuk menentukan kondisi sistem dan mengendalikan buzzer, LED merah, relay, serta LCD 16×2 I2C.

Proyek ini dibuat sebagai bagian dari praktikum mata kuliah **Internet of Things (IoT)**.

## Tujuan

- Memahami penerapan Early Warning System menggunakan Arduino Mega 2560.
- Membaca perubahan suhu menggunakan NTC Temperature Sensor.
- Mendeteksi tingkat asap/gas menggunakan MQ-2 Gas Sensor.
- Menggabungkan pembacaan suhu dan asap untuk menentukan kondisi sistem.
- Memberikan peringatan melalui buzzer dan LED merah.
- Menggunakan relay sebagai simulasi pengaktifan sprinkler pada kondisi bahaya.
- Menampilkan identitas praktikan dan kondisi sistem melalui LCD 16×2 I2C.

## Komponen

| Komponen | Fungsi | Pin Arduino |
|---|---|---|
| Arduino Mega 2560 | Mikrokontroler utama | — |
| NTC Temperature Sensor | Mendeteksi perubahan suhu | A0 |
| MQ-2 Gas Sensor | Mendeteksi asap/gas | A1 |
| Buzzer | Peringatan suara | D7 |
| LED Merah | Indikator visual | D6 |
| Relay Module | Simulasi sprinkler | D8 |
| LCD 16×2 I2C | Menampilkan informasi sistem | SDA D20, SCL D21 |
| Resistor 220Ω | Pembatas arus LED | Seri dengan LED |

VCC dan GND komponen didistribusikan melalui jalur VCC 5V dan GND pada breadboard.

## Skema Wiring

Hubungan pin utama:

```text
NTC Temperature Sensor
OUT  → A0
VCC  → VCC 5V
GND  → GND

MQ-2 Gas Sensor
AOUT → A1
VCC  → VCC 5V
GND  → GND

Buzzer
Signal → D7
GND    → GND

Relay Module
IN  → D8
VCC → VCC 5V
GND → GND

LED Merah
Anode    → D6
Cathode  → Resistor 220Ω → GND

LCD 16×2 I2C
SDA → D20
SCL → D21
VCC → VCC 5V
GND → GND
```

## Logika Deteksi

Sistem menggunakan ambang batas suhu dan asap untuk menentukan kondisi.

| Kondisi | Suhu | Asap | Respons Sistem |
|---|---:|---:|---|
| Normal | ≤ 35°C | ≤ 1500 ppm | LCD menampilkan status aman, buzzer OFF, LED OFF, relay OFF |
| Suhu Tinggi | > 35°C dan < 50°C | ≤ 1500 ppm | LCD peringatan suhu tinggi, buzzer 800 Hz, LED berkedip, relay OFF |
| Suhu Sangat Tinggi | ≥ 50°C | ≤ 1500 ppm | LCD peringatan suhu sangat tinggi, buzzer 800 Hz, LED berkedip, relay OFF |
| Asap Tinggi | ≤ 35°C | > 1500 ppm | LCD peringatan asap tinggi, buzzer 800 Hz, LED berkedip, relay OFF |
| Bahaya | > 35°C | > 1500 ppm | LCD menampilkan bahaya dan sprinkler aktif, buzzer 1200 Hz, LED berkedip lebih cepat, relay ON |

Pada kondisi bahaya, relay digunakan sebagai simulasi pengaktifan sprinkler. Batas dan respons tersebut mengikuti implementasi yang diuji pada praktikum.

## Alur Sistem

```text
NTC Sensor ──┐
             ├──> Arduino Mega 2560 ──> Tentukan kondisi
MQ-2 Sensor ─┘                             │
                                          ├──> LCD
                                          ├──> Buzzer
                                          ├──> LED Merah
                                          └──> Relay / Sprinkler
```

Saat startup, LCD menampilkan identitas praktikan sebelum sistem masuk ke proses monitoring. Setelah itu, Arduino membaca sensor secara terus-menerus dan menentukan respons sesuai kondisi suhu dan asap.

## Identitas Startup

Pada saat sistem pertama kali dijalankan, LCD menampilkan:

```text
M Rafie Firman R
NIM: 24051204065
```

## Implementasi

Program Arduino menggunakan:

- pembacaan analog untuk NTC dan MQ-2;
- perhitungan suhu dari pembacaan NTC;
- pemetaan nilai MQ-2 ke rentang 0–5000 ppm;
- logika kondisi berbasis ambang suhu dan asap;
- `tone()` untuk menghasilkan pola buzzer;
- kontrol digital LED dan relay;
- `LiquidCrystal_I2C` untuk LCD 16×2.

Konfigurasi utama pada program:

```cpp
#define PIN_NTC A0
#define PIN_MQ2 A1
#define PIN_BUZZER 7
#define PIN_RELAY 8
#define PIN_LED 6

const float SUHU_NORMAL_MAX = 35.0;
const float SUHU_SANGAT_TINGGI = 50.0;
const float ASAP_RENDAH_MAX = 500.0;
const float ASAP_TINGGI_MIN = 1500.0;
```

## Simulasi

Proyek diuji melalui simulasi rangkaian Arduino. Laporan praktikum mendokumentasikan beberapa kondisi pengujian:

1. Kondisi normal.
2. Suhu tinggi.
3. Suhu sangat tinggi.
4. Asap tinggi.
5. Suhu dan asap tinggi secara bersamaan.

Hasil simulasi menunjukkan bahwa perubahan pembacaan suhu dan asap menghasilkan respons sistem yang berbeda sesuai kondisi yang telah diprogram. fileciteturn25file0L123-L160

Dokumentasi pada laporan juga memperlihatkan rangkaian Arduino Mega, sensor, buzzer, LED, relay, dan LCD pada simulasi, termasuk wiring komponen melalui breadboard. fileciteturn25file0L91-L120

## Hasil Pengujian

Berdasarkan laporan praktikum:

- Pada kondisi normal, suhu ≤35°C dan asap ≤1500 ppm, buzzer, LED, dan relay dalam keadaan mati serta LCD menampilkan status aman. fileciteturn25file0L250-L263
- Pada kondisi suhu tinggi, suhu >35°C dengan asap belum tinggi, buzzer dan LED aktif sedangkan relay tetap mati. Pada suhu ≥50°C, LCD menampilkan status suhu sangat tinggi. fileciteturn25file0L182-L204
- Pada kondisi asap tinggi, nilai asap >1500 ppm dengan suhu ≤35°C menyebabkan buzzer dan LED aktif, sedangkan relay tetap mati. fileciteturn25file0L210-L228
- Pada kondisi bahaya, suhu >35°C dan asap >1500 ppm secara bersamaan menyebabkan relay aktif sebagai simulasi sprinkler, buzzer bekerja pada 1200 Hz, dan LED berkedip lebih cepat. fileciteturn25file0L229-L248

## Sumber Kode Simulasi

Proyek simulasi Wokwi:

https://wokwi.com/projects/476899317361810433

## Dokumentasi

Dokumentasi lengkap mengenai teori, wiring, kode, dan hasil pengujian tersedia pada laporan praktikum.
