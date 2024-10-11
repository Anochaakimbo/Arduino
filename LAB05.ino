#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// กำหนดข้อมูลการเชื่อมต่อ WiFi
const char* ssid = "ชื่อ WiFi";
const char* password = "รหัส WiFi";

// กำหนดข้อมูล MQTT Broker
const char* mqtt_server = "MQTT broker ip";
const int mqtt_port = 1883;
const char* mqtt_username = "ชื่อผู้ใช้ MQTT";
const char* mqtt_password = "รหัสผ่าน MQTT";

// กำหนดชื่อ client
const char* client_id = "ESP8266Client";

// กำหนดหัวข้อ MQTT
const char* topic1 = "หัวช้อ MQTT";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // ตั้งค่าโหมด Access Point
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to wifi...");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(client_id, mqtt_username, mqtt_password)) {
      client.subscribe(topic1);
    } else {
      Serial.println("Connecting to mqtt server...");
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.publish(topic1, "1058 สุดหล่อเท่ห์");
  Serial.println("Message send completly");

  delay(1000);
}