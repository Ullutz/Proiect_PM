#include <esp_now.h>
#include <WiFi.h>

#define IN1 5
#define IN2 18
#define IN3 19
#define IN4 21

bool front = false;
bool back = false;
bool left = false;
bool right = false;

typedef struct {
  bool f;
  bool b;
  bool l;
  bool r;
} message;

message data;

void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  if (len == sizeof(message)) {
    memcpy(&data, incomingData, sizeof(data));

    front = data.f;
    back  = data.b;
    left  = data.l;
    right = data.r;

    Serial.print("Received Data - Front: ");
    Serial.print(front);
    Serial.print(", Back: ");
    Serial.print(back);
    Serial.print(", Left: ");
    Serial.print(left);
    Serial.print(", Right: ");
    Serial.println(right);
  } else {
    Serial.println("Received unexpected data length");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP-NOW Receiver Initialized");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  carStop();

  WiFi.mode(WIFI_STA);
  Serial.println("WiFi Mode set to STA");

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    ESP.restart();
  } else {
    Serial.println("ESP-NOW Initialized Successfully");
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receive Callback Registered");
}

void loop() {
  if (front) {
    carforward();
  } 
  else if (back) {
    carbackward();
  } 
  else if (left) {
    carturnleft();
  } 
  else if (right) {
    carturnright();
  } 
  else {
    carStop();
  }

  delay(100);
}

void carforward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Car Moving Forward");
}

void carbackward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Car Moving Backward");
}

void carturnleft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Car Turning Left");
}

void carturnright() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Car Turning Right");
}

void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println("Car Stopped");
}