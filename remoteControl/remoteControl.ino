#include <esp_now.h>
#include <WiFi.h>

#define INPUT_IGNORE_LOWER_THRESHOLD 2900
#define INPUT_IGNORE_UPPER_THRESHOLD 3100
#define INPUT_MAXIMUM_VALUE 4095
#define GAS_JOYSTICK_PIN 18
#define STEERING_JOYSTICK_PIN 19

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

  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
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
  if(isValueAboveThreshold(gasInput, INPUT_IGNORE_LOWER_THRESHOLD)) {
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    return 0;
  } else {
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
    return 1;
  }
}

int getNormalizedSteeringInput(int steeringInput) {
  float percentage = 0;
  if(isValueBelowThreshold(steeringInput, INPUT_IGNORE_LOWER_THRESHOLD)) {
    percentage = -((float)(INPUT_IGNORE_LOWER_THRESHOLD-steeringInput))/((float)INPUT_IGNORE_LOWER_THRESHOLD);
  } else if(isValueAboveThreshold(steeringInput, INPUT_IGNORE_UPPER_THRESHOLD)) {
    percentage = ((float)(steeringInput-INPUT_IGNORE_UPPER_THRESHOLD))/((float)(INPUT_MAXIMUM_VALUE-INPUT_IGNORE_UPPER_THRESHOLD));
  }

  return (int)(percentage * 45.0f);
}

void loop() {
  int gasInput = analogRead(GAS_JOYSTICK_PIN);
  int steeringInput = analogRead(STEERING_JOYSTICK_PIN);

  int normalizedGasInput = getNormalizedGasInput(gasInput);
  int normalizedSteeringInput = getNormalizedSteeringInput(steeringInput);

  if(normalizedGasInput == previouslyCalculatedGasInput && normalizedSteeringInput == previouslyCalculatedSteeringInput) {
    digitalWrite(9, LOW);
    return;
  }
  digitalWrite(9, HIGH);
  previouslyCalculatedGasInput = normalizedGasInput;
  previouslyCalculatedSteeringInput = normalizedSteeringInput;
  Serial.println("Sending with steeringInput --------------");
  Serial.println(normalizedSteeringInput);
  controlInputs.gi = normalizedGasInput;
  controlInputs.si = normalizedSteeringInput;
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &controlInputs, sizeof(controlInputs));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
    Serial.println("Sending error");
  }
  delay(50);
}