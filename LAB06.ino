#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// กำหนดข้อมูลการเชื่อมต่อ WiFi
const char* ssid = "ssid";
const char* password = "pass";

// กำหนดข้อมูล MQTT Broker
const char* mqtt_server = "MQTT broker ip";
const int mqtt_port = 1883;
const char* mqtt_username = "1936";
const char* mqtt_password = "";

// กำหนดชื่อ client
const char* client_id = "ESP8266Client";

// กำหนดหัวข้อ MQTT
const char* topic1 = "หัวช้อ MQTT";
// รหัส4ตัวท้าย
const char* client_id = "1936";
const int triggerPin = D1; 
const int echoPin = D2;    

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("เชื่อมต่อ WiFi...");
  }
  Serial.println("เชื่อมต่อ WiFi สำเร็จ");
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("เชื่อมต่อ MQTT...");
    if (client.connect(client_id, mqtt_user, mqtt_password)) {
      Serial.println("เชื่อมต่อ MQTT สำเร็จ");
    } else {
      Serial.print("ล้มเหลว, rc=");
      Serial.print(client.state());
      Serial.println(" ลองใหม่ใน 5 วินาที");
      delay(5000);
    }
  }
}

void loop() {
  long duration, distance;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  Serial.print("ระยะทาง: ");
  Serial.println(distance);
  
  char msg[10];
  sprintf(msg, "%ld", distance);
  // รหัส4ตัวท้าย
  client.publish("1936", msg);
  delay(1000); 
}

void callback(char* topic, byte* payload, unsigned int length) {
}
