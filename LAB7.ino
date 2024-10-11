//663380193-6
//653380372-9
//663380149-9
//663380147-3
//663380492-6
#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>

#define SSID        "A"        // WiFi SSID
#define PASSWORD    "11111111" // WiFi Password
#define LINE_TOKEN  "VekrQvazAvgbZ0okfpnUl6mw2TygEqDT2qOlM4ENbKc" // LINE Notify Token

#define TRIG_PIN D1  // Pin for the ultrasonic trigger
#define ECHO_PIN D2  // Pin for the ultrasonic echo

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(LINE.getVersion());

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP: ");
  Serial.println(WiFi.localIP());

  LINE.setToken(LINE_TOKEN);

  // Send member names
  LINE.notify("A 193-6");
  LINE.notify("B 372-9");
  LINE.notify("C 149-9");
  LINE.notify("D 147-3");
  LINE.notify("G 492-6");
  
  // Example of sending a picture
  LINE.notifyPicture("https://eldenring.wiki.fextralife.com/file/Elden-Ring/godfrey_first_elden_lord_bosses_elden_ring_wiki_600px.jpg");

  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  long duration, distance;

  // Clear the trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Set the trigger to HIGH for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo pin
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate distance in centimeters
  distance = duration * 0.034 / 2; // Speed of sound is 34300 cm/s

  // Print distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Send distance to LINE if necessary
  if (distance < 20) { // Adjust the threshold as needed
    LINE.notify("Warning: Object is too close! Distance: " + String(distance) + " cm");
  }

  delay(1000); // Delay before the next reading
}
