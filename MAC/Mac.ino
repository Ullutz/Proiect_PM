#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  // Get and print the MAC address
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
}