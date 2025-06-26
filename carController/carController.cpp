#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define INITIAL_SERVO_ROTATION 90
#define FRONT_LEFT_SERVO_PIN 2
#define FRONT_RIGHT_SERVO_PIN 4

#define FORWARD_VOLTAGE_PIN 5
#define FORWARD_GROUND_PIN 18
#define BACKWARD_VOLTAGE_PIN 19
#define BACKWARD_GROUND_PIN 21

Servo frontLeftWheel;
Servo frontRightWheel;

struct RemoteControlMessage {
  int gi;
  int si;
};
struct RemoteControlMessage remoteData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&remoteData, incomingData, sizeof(remoteData));
  frontLeftWheel.write(INITIAL_SERVO_ROTATION+remoteData.si);
  frontRightWheel.write(INITIAL_SERVO_ROTATION+remoteData.si);

  int moveForwardVoltageState;
  int moveForwardGroundState;
  int moveBackwardVoltageState;
  int moveBackwardGroundState;

  if(remoteData.gi == 1) {
    moveForwardVoltageState = LOW;
    moveForwardGroundState = HIGH;

    moveBackwardVoltageState = HIGH;
    moveBackwardGroundState = LOW;
  } else if(remoteData.gi == -1) {
    moveForwardVoltageState = HIGH;
    moveForwardGroundState = LOW;

    moveBackwardVoltageState = LOW;
    moveBackwardGroundState = HIGH;
  } else {
    moveForwardVoltageState = HIGH;
    moveForwardGroundState = LOW;

    moveBackwardVoltageState = HIGH;
    moveBackwardGroundState = LOW;
  }

  digitalWrite(FORWARD_VOLTAGE_PIN, moveForwardVoltageState);
  digitalWrite(FORWARD_GROUND_PIN, moveForwardGroundState);

  digitalWrite(BACKWARD_VOLTAGE_PIN, moveBackwardVoltageState);
  digitalWrite(BACKWARD_GROUND_PIN, moveBackwardGroundState);
}

void setup() {
  Serial.begin(115200);

  frontLeftWheel.attach(FRONT_LEFT_SERVO_PIN);
  frontLeftWheel.write(INITIAL_SERVO_ROTATION);

  frontRightWheel.attach(FRONT_RIGHT_SERVO_PIN);
  frontRightWheel.write(INITIAL_SERVO_ROTATION);
  
  pinMode(FORWARD_VOLTAGE_PIN, OUTPUT);
  pinMode(FORWARD_GROUND_PIN, OUTPUT);
  pinMode(BACKWARD_VOLTAGE_PIN, OUTPUT);
  pinMode(BACKWARD_GROUND_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP NOW ERROR");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);  
}
 
void loop() {

}