//663380193-6
//663380149-9
//663380147-3
//653380372-9
//663380492-6
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// กำหนดข้อมูลการเชื่อมต่อ WiFi
const char* ssid = "ssid";
const char* password = "pass";

// กำหนดข้อมูล MQTT Broker
const char* mqtt_server = "mqtt-dashboard.com";
const int mqtt_port = 1883;
const char* mqtt_username = "1936";
const char* mqtt_password = "";

// กำหนดชื่อ client
const char* client_id = "ESP8266Client";

// กำหนดหัวข้อ MQTT
const char* topic1 = "lab5";

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

  client.publish(topic1, "1936 \n 3729 \n 4926 \n 1499 \n 1473");
  Serial.println("Message send completly");

  delay(1000);
}

