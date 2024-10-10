#include <Wire.h>
#include <U8g2lib.h>
#include "DHT.h"

// กำหนดขาเซ็นเซอร์วัดความชื้นดินและรีเลย์
const int soilMoisturePin = A0;  // ขาที่เชื่อมต่อกับเซ็นเซอร์วัดความชื้นดิน
const int pumpRelayPin = 9;      // ขาที่เชื่อมต่อกับขา IN ของรีเลย์สำหรับปั๊มน้ำ
int moistureThreshold = 500;     // กำหนดค่า threshold สำหรับความชื้นดิน

// กำหนดขาเซ็นเซอร์ DHT11 และพัดลม
#define DHTPIN 11        // ขาที่เชื่อมต่อกับ DHT11 สำหรับเซ็นเซอร์อุณหภูมิ
#define FAN_RELAY_PIN 8   // ขาที่เชื่อมต่อกับรีเลย์ที่ควบคุมพัดลม
#define DHTTYPE DHT11     // ใช้เซ็นเซอร์ประเภท DHT11
DHT dht(DHTPIN, DHTTYPE);

// กำหนดขาสำหรับไฟ LED ทั้ง 4 ดวง
const int ledError = 2;      // ไฟดวงที่ 1 สำหรับแสดงข้อผิดพลาด
const int ledPump = 4;       // ไฟดวงที่ 2 สำหรับปั๊มน้ำทำงาน
const int ledFan = 5;        // ไฟดวงที่ 3 สำหรับพัดลมทำงาน
const int ledIdle = 6;       // ไฟดวงที่ 4 เมื่อไม่มีการทำงานใดๆ

// สร้างอ็อบเจกต์ U8g2 สำหรับหน้าจอ OLED ขนาด 64x32 ที่ใช้ I2C
U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
const int ldrPin = A1;         // ขาที่เชื่อมต่อกับเซ็นเซอร์ LDR
const int lightPin = 1;        // ขาที่เชื่อมต่อกับหลอดไฟ
int lightThreshold = 700; 
float temperatureThreshold = 30.0;  // กำหนดค่าอุณหภูมิที่พัดลมจะเปิด (30°C)
unsigned long previousMillis = 0;
const long interval = 5000;  // หน่วงเวลาทุก 5 วินาที

// เพิ่มตัวแปร pumpOn และ fanOn เป็น global
bool pumpOn = false;
bool fanOn = false;

void setup() {
  setupPins();
  setupSensors();
  setupDisplay();

  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, HIGH);  // เริ่มต้นด้วยการปิดหลอดไฟ
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    int soilMoistureValue = readSoilMoisture();
    float temperature = readTemperature();
    int lightValue = readLight();

    if (isnan(temperature)) {
      handleSensorError();
    } else {
      Serial.print("Light Level: ");
      Serial.println(lightValue);
      displayData(soilMoistureValue, temperature);
      controlPump(soilMoistureValue);  // อัปเดตค่า pumpOn
      controlFan(temperature);         // อัปเดตค่า fanOn
      controlLEDs(pumpOn, fanOn);      // ใช้ pumpOn และ fanOn
      controlLight(lightValue);
      pinMode(lightPin, OUTPUT);
      digitalWrite(lightPin, LOW);  // เริ่มต้นด้วยการปิดหลอดไฟ
    }
  }
}

// แก้ไขฟังก์ชันควบคุมปั๊มน้ำและพัดลม
void controlPump(int soilMoistureValue) {
  if (soilMoistureValue > moistureThreshold) {
    digitalWrite(pumpRelayPin, LOW);  // เปิดปั๊มน้ำ
    pumpOn = true;
  } else {
    digitalWrite(pumpRelayPin, HIGH);  // ปิดปั๊มน้ำ
    pumpOn = false;
  }
}

void controlFan(float temperature) {
  if (temperature > temperatureThreshold) {
    digitalWrite(FAN_RELAY_PIN, LOW);  // เปิดพัดลม
    fanOn = true;
  } else {
    digitalWrite(FAN_RELAY_PIN, HIGH);  // ปิดพัดลม
    fanOn = false;
  }
}
int readLight() {
  return analogRead(ldrPin);
}

void controlLight(int lightValue) {
  if (lightValue > lightThreshold) {
    digitalWrite(lightPin, HIGH);  // เปิดหลอดไฟเมื่อค่าความเข้มแสงน้อย
  } else {
    digitalWrite(lightPin, LOW);  // ปิดหลอดไฟเมื่อค่าความเข้มแสงมาก
  }
}

// แยกการตั้งค่าพินต่างๆ ออกเป็นฟังก์ชัน
void setupPins() {
  pinMode(pumpRelayPin, OUTPUT);
  pinMode(FAN_RELAY_PIN, OUTPUT);
  pinMode(ledError, OUTPUT);
  pinMode(ledPump, OUTPUT);
  pinMode(ledFan, OUTPUT);
  pinMode(ledIdle, OUTPUT);
  
  // เริ่มต้นด้วยการปิดอุปกรณ์
  digitalWrite(pumpRelayPin, HIGH);
  digitalWrite(FAN_RELAY_PIN, HIGH);
  digitalWrite(ledError, HIGH);
  digitalWrite(ledPump, HIGH);
  digitalWrite(ledFan, HIGH);
  digitalWrite(ledIdle, LOW); 
}

void setupSensors() {
  Serial.begin(9600);
  dht.begin();
}

void setupDisplay() {
  u8g2.begin();
}

int readSoilMoisture() {
  return analogRead(soilMoisturePin);
}

float readTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    delay(2000);
    temp = dht.readTemperature();
  }
  return temp;
}

void displayData(int soilMoistureValue, float temperature) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 10);
  u8g2.print("Soil:");
  u8g2.setCursor(40, 10);
  u8g2.print(soilMoistureValue);

  u8g2.setCursor(0, 25);
  u8g2.print("Temp:");
  u8g2.setCursor(40, 25);
  u8g2.print(temperature);
  u8g2.print(" C");

  u8g2.sendBuffer();
}

void controlLEDs(bool pumpOn, bool fanOn) {
  if (pumpOn && fanOn) {
    // ถ้าปั๊มน้ำและพัดลมทำงานพร้อมกัน ไฟดวงที่ 2 และ 3 จะติด
    digitalWrite(ledPump, LOW);
    digitalWrite(ledFan, LOW);
    digitalWrite(ledIdle, HIGH); // ปิดไฟ idle
  } else if (pumpOn) {
    // ถ้าปั๊มน้ำทำงาน ไฟดวงที่ 2 จะติด
    digitalWrite(ledPump, LOW);
    digitalWrite(ledFan, HIGH);  // ปิดไฟพัดลม
    digitalWrite(ledIdle, HIGH); // ปิดไฟ idle
  } else if (fanOn) {
    // ถ้าพัดลมทำงาน ไฟดวงที่ 3 จะติด และไฟดวงที่ 4 จะดับ
    digitalWrite(ledFan, LOW);
    digitalWrite(ledPump, HIGH); // ปิดไฟปั๊มน้ำ
    digitalWrite(ledIdle, HIGH); // ปิดไฟ idle
  } else {
    // ถ้าไม่มีอะไรทำงาน ไฟดวงที่ 4 จะติด
    digitalWrite(ledIdle, LOW);  // เปิดไฟ idle
    digitalWrite(ledPump, HIGH); // ปิดไฟปั๊มน้ำ
    digitalWrite(ledFan, HIGH);  // ปิดไฟพัดลม
  }
}

void handleSensorError() {
  Serial.println("Failed to read from DHT sensor!");
  digitalWrite(ledError, LOW); 
  digitalWrite(ledIdle, HIGH);  
}