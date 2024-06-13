#include <esp_now.h>
#include <WiFi.h>

#define START_PIN 35

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct struct_message {
  char m;
} struct_message;

struct_message startMessage;
esp_now_peer_info_t peerInfo;

void start(){
  startMessage.m = 'S'; // S for Start
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &startMessage, sizeof(startMessage));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}

/*void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}*/

void setup() {
  pinMode(START_PIN, INPUT);
  attachInterrupt(START_PIN, start, RISING);

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }else{
    Serial.println("initializing ESP-NOW: OK");
  }

  //esp_now_register_send_cb(onDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }else{
    Serial.println("added peer: OK");
  }
}

void loop() {

}

