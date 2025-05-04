#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define INITIAL_SERVO_ROTATION 90

Servo frontLeftWheel;
Servo frontRightWheel;

struct RemoteControlMessage {
  int gi;
  int si;
};
struct RemoteControlMessage remoteData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&remoteData, incomingData, sizeof(remoteData));
  Serial.print("Data received");
  Serial.print("Gas input: ");
  Serial.println(remoteData.gi);
  Serial.print("Steering input: ");
  Serial.println(remoteData.si);

  frontLeftWheel.write(INITIAL_SERVO_ROTATION+remoteData.si);

  if(remoteData.gi > 0) {
    digitalWrite(18, HIGH);
  } else {
    digitalWrite(18, LOW);
  }
}

void setup() {
  frontLeftWheel.attach(5);
  frontLeftWheel.write(INITIAL_SERVO_ROTATION);

  // Set up Serial Monitor
  Serial.begin(115200);
  pinMode(18, OUTPUT);
  
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}