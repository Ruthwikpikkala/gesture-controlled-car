#include <ESP8266WiFi.h>
#include <espnow.h>

// Direction pins
#define FORWARD_PIN   D1
#define BACKWARD_PIN  D2
#define LEFT_PIN      D3
#define RIGHT_PIN     D4
#define IDLE_PIN      D5   // HIGH when no data received

String command = "";
unsigned long lastReceivedTime = 0;
const unsigned long timeout = 1000; // 1 second timeout

void resetPins() {
  digitalWrite(FORWARD_PIN, LOW);
  digitalWrite(BACKWARD_PIN, LOW);
  digitalWrite(LEFT_PIN, LOW);
  digitalWrite(RIGHT_PIN, LOW);
  digitalWrite(IDLE_PIN, LOW);
}

// ESP-NOW receive callback
void onDataReceive(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  command = String((char*)incomingData);
  lastReceivedTime = millis();

  Serial.print("Received command: ");
  Serial.println(command);

  resetPins();

  if (command == "forward") {
    digitalWrite(FORWARD_PIN, HIGH);
    Serial.println("FORWARD pin HIGH");
  }
  else if (command == "backward") {
    digitalWrite(BACKWARD_PIN, HIGH);
    Serial.println("BACKWARD pin HIGH");
  }
  else if (command == "left") {
    digitalWrite(LEFT_PIN, HIGH);
    Serial.println("LEFT pin HIGH");
  }
  else if (command == "right") {
    digitalWrite(RIGHT_PIN, HIGH);
    Serial.println("RIGHT pin HIGH");
  }
  else {
    Serial.println("Unknown command");
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(FORWARD_PIN, OUTPUT);
  pinMode(BACKWARD_PIN, OUTPUT);
  pinMode(LEFT_PIN, OUTPUT);
  pinMode(RIGHT_PIN, OUTPUT);
  pinMode(IDLE_PIN, OUTPUT);

  resetPins();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() == 0) {
    Serial.println("ESP-NOW Initialized");
  } else {
    Serial.println("ESP-NOW Init Failed, restarting...");
    ESP.restart();
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataReceive);

  Serial.println("Receiver Ready");
}

void loop() {
  // If no data received for 1 second, activate IDLE pin
  if (millis() - lastReceivedTime > timeout) {
    resetPins();
    digitalWrite(IDLE_PIN, HIGH);
    Serial.println("NO SIGNAL → IDLE PIN HIGH");
  }

  //delay(100);
}
