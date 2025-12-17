#include <Wire.h>
#include <MPU6050.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

MPU6050 mpu;

// Receiver MAC Address (CHANGE THIS)
uint8_t receiverMAC[] = {0x4C, 0xEB, 0xD6, 0x1F, 0x89, 0x34};

// ---------------- ESP-NOW CALLBACK ----------------
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Send Status: ");
  if (sendStatus == 0) Serial.println("Success");
  else Serial.println("Fail");
}

// ------------- SEND FUNCTION -------------
void sendCommand(const char* cmd) {
  esp_now_send(receiverMAC, (uint8_t*)cmd, strlen(cmd) + 1);
  Serial.print("Sent: ");
  Serial.println(cmd);
}

// --------------------- SETUP -----------------------
void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  // ---------- ESP-NOW INIT ----------
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  if (esp_now_init() == 0) {
    Serial.println("ESP-NOW Initialized.");
  } else {
    Serial.println("ESP-NOW Init Failed!");
    ESP.restart();
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // Register send callback
  esp_now_register_send_cb(OnDataSent);

  // Add receiver peer
  if (esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) == 0) {
    Serial.println("Receiver added successfully");
  } else {
    Serial.println("Failed to add receiver");
  }

  Serial.println("MPU6050 + ESP-NOW Ready");
}

// -------------------- LOOP -------------------------
void loop() {
  int16_t ax, ay, az, gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  Serial.print("Accel -> ");
  Serial.print("X: "); Serial.print(ax);
  Serial.print(" | Y: "); Serial.print(ay);
  Serial.print(" | Z: "); Serial.print(az);
  Serial.println();

  // --- YOUR GESTURE CONDITIONS (UNCHANGED) ---

  if (ax > 6000) {
    Serial.println("right");
    sendCommand("right");
  }
  else if (ax < -6000) {
    Serial.println("left");
    sendCommand("left");
  }
  else if (ay < -6000) {
    Serial.println("backward");
    sendCommand("backward");
  }
  else if (ay > 6000) {
    Serial.println("forward");
    sendCommand("forward");
  }

  //delay(300);
}
