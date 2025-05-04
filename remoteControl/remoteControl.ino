#include <esp_now.h>
#include <WiFi.h>

#define INPUT_IGNORE_LOWER_THRESHOLD 1800
#define INPUT_IGNORE_UPPER_THRESHOLD 2050
#define INPUT_MAXIMUM_VALUE 4095
#define GAS_JOYSTICK_PIN 18
#define STEERING_JOYSTICK_PIN 20
#define CAR_GAS_DIODE_PIN 7
#define CAR_STEERING_DIODE_PIN 8
#define SIGNAL_SENT_DIODE_PIN 9

uint8_t broadcastAddress[] = {0x14, 0x2B, 0x2F, 0xD7, 0x45, 0x0C};
int previouslyCalculatedGasInput = 100;
int previouslyCalculatedSteeringInput = 0;

struct remoteControlMessage{
  int gi;
  int si;
} remoteControlMessage;

struct remoteControlMessage controlInputs;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(SIGNAL_SENT_DIODE_PIN, OUTPUT);
  pinMode(CAR_STEERING_DIODE_PIN, OUTPUT);
  pinMode(CAR_GAS_DIODE_PIN, OUTPUT);
}

bool isValueInRange(int value, int lowerRange, int upperRange) {
  return value >= lowerRange && value <= upperRange;
}

bool isValueAboveThreshold(int value, int threshold) {
  return value > threshold;
}

bool isValueBelowThreshold(int value, int threshold) {
  return value < threshold;
}

int getNormalizedGasInput(int gasInput) {
  if(isValueBelowThreshold(gasInput, INPUT_IGNORE_LOWER_THRESHOLD)) {
    digitalWrite(CAR_GAS_DIODE_PIN, HIGH);
    return 1;
  } else if(isValueAboveThreshold(gasInput, INPUT_IGNORE_UPPER_THRESHOLD)){
    digitalWrite(CAR_GAS_DIODE_PIN, HIGH);
    return -1;
  }

  digitalWrite(CAR_GAS_DIODE_PIN, LOW);
  return 0;
}

int getNormalizedSteeringInput(int steeringInput) {
  float percentage = 0;
  if(isValueBelowThreshold(steeringInput, INPUT_IGNORE_LOWER_THRESHOLD)) {
    percentage = -((float)(INPUT_IGNORE_LOWER_THRESHOLD-steeringInput))/((float)INPUT_IGNORE_LOWER_THRESHOLD);
  } else if(isValueAboveThreshold(steeringInput, INPUT_IGNORE_UPPER_THRESHOLD)) {
    percentage = ((float)(steeringInput-INPUT_IGNORE_UPPER_THRESHOLD))/((float)(INPUT_MAXIMUM_VALUE-INPUT_IGNORE_UPPER_THRESHOLD));
  }

  if(percentage != 0) {
    digitalWrite(CAR_STEERING_DIODE_PIN, HIGH);
  } else {
    digitalWrite(CAR_STEERING_DIODE_PIN, LOW);
  }
  return (int)(percentage * 45.0f);
}

void loop() {
  int gasInput = analogRead(GAS_JOYSTICK_PIN);
  int steeringInput = analogRead(STEERING_JOYSTICK_PIN);

  int normalizedGasInput = getNormalizedGasInput(gasInput);
  int normalizedSteeringInput = getNormalizedSteeringInput(steeringInput);

  if(normalizedGasInput == previouslyCalculatedGasInput && normalizedSteeringInput == previouslyCalculatedSteeringInput) {
    digitalWrite(SIGNAL_SENT_DIODE_PIN, LOW);
    return;
  }
  digitalWrite(SIGNAL_SENT_DIODE_PIN, HIGH);
  previouslyCalculatedGasInput = normalizedGasInput;
  previouslyCalculatedSteeringInput = normalizedSteeringInput;
  Serial.println("Sending with gasInput and steeringInput --------------");
  Serial.println(gasInput);
  Serial.println(normalizedSteeringInput);
  controlInputs.gi = normalizedGasInput;
  controlInputs.si = normalizedSteeringInput;
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &controlInputs, sizeof(controlInputs));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(50);
}