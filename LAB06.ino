//663380558-2
//663380572-8
//663380363-7
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// กำหนดข้อมูลการเชื่อมต่อ WiFi
const char* ssid = "Asuka";
const char* password = "11111111";

// กำหนดข้อมูล MQTT Broker
const char* mqtt_server = "mqtt-dashboard.com";
const int mqtt_port = 1883;
const char* mqtt_username = "dada";
const char* mqtt_password = "";

// กำหนดชื่อ client
const char* client_id = "3637"; 

// กำหนดหัวข้อ MQTT
const char* topic1 = "lab6_200924";

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
    if (client.connect(client_id, mqtt_username, mqtt_password)) {
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
  
  // ส่งสัญญาณจาก Trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // รับค่าสัญญาณสะท้อนกลับ
  duration = pulseIn(echoPin, HIGH, 10000); // กำหนด timeout เป็น 10ms

  // คำนวณระยะทาง
  distance = (duration / 2) / 29.1;

  if (distance > 0 && distance < 400) { // ตรวจสอบระยะทางในช่วงที่สมเหตุสมผล
    Serial.print("ระยะทาง: ");
    Serial.println(distance);
    
    char msg[10];
    sprintf(msg, "%ld", distance);
    client.publish(topic1, msg);
  } else {
    Serial.println("Error");
    client.publish(topic1, "Error");
  }
  
  delay(1000); 
}

void callback(char* topic1, byte* payload, unsigned int length) {
  // การทำงานใน callback
}
