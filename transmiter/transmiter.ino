#include "esp_now.h"
#include "WiFi.h"
#include "Wire.h"
#include "MPU6050_light.h"

uint8_t broadcastAddress[] = {0xD4, 0x8AFC, 0xA3, 0xF3, 0xE0};

typedef struct{
  bool f;
  bool b;
  bool l;
  bool r;
} message;
message data;

MPU6050 mpu(Wire);


unsigned long timer = 0;
float x, y;
bool front = false, back = false, left = false, right = false;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);


  if (esp_now_init() != 0){
    Serial.println("Error Initializing ESP NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != 0){
    Serial.println("Failed to add peer");
    return;
  }

  Wire.setPins(21, 22);
  Wire.begin();

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } 
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); 
  Serial.println("Done!\n");
}

void loop() {
  mpu.update();

  if((millis()-timer)>10){ ]
	x = mpu.getAngleX();
	y = mpu.getAngleY();

	if(x >= 30)
    front = true;
  else
    front = false;
  if(x <= -30)
    back = true;
  else
    back = false;
  if(y >= 30)
    right = true;
  else
    right = false;
  if(y <= -30)
    left = true;
  else
    left = false;
	timer = millis();  

  data.l = front;
  data.r = back;
  data.f = left;
  data.b = right;

  esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));
  } 
}