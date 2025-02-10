#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Định nghĩa chân NRF24L01
#define CE_PIN  D8
#define CSN_PIN D4

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = {'0', '0', '0', '0', '1'}; // Địa chỉ giao tiếp

// Cấu trúc dữ liệu gửi đi
struct DataPackage {
  int joystickValue;  // Giá trị Joystick (trục VRx)
  bool button1;       // Trạng thái nút 1
  bool button2;       // Trạng thái nút 2
  bool button3;       // Trạng thái nút 3
  bool button4;       // Trạng thái nút 4
};

DataPackage data; // Biến chứa dữ liệu gửi đi

// Định nghĩa chân cho Joystick và nút nhấn
#define JOYSTICK_PIN A0
#define BUTTON1_PIN D0 // GPIO 5
#define BUTTON2_PIN D2 // GPIO 4
#define BUTTON3_PIN D3 // GPIO 0
#define BUTTON4_PIN D9 // GPIO 3

void setup() {
  Serial.begin(115200);
  radio.begin();
  
  // Cấu hình NRF24L01
  radio.setPALevel(RF24_PA_LOW);       // Giảm công suất để ổn định
  radio.setDataRate(RF24_1MBPS);       // Đồng bộ tốc độ truyền
  radio.setRetries(3, 5);              // Thiết lập retries (3 lần, mỗi lần 5ms)

  radio.openWritingPipe(address);
  radio.stopListening(); // Chuyển sang chế độ gửi
  radio.printDetails();
  Serial.println("Sender ready");


  // Cấu hình chân Joystick và nút nhấn
  pinMode(JOYSTICK_PIN, INPUT);
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  pinMode(BUTTON4_PIN, INPUT_PULLUP);

}

void loop() {

  // Đọc giá trị từ joystick
  data.joystickValue = analogRead(JOYSTICK_PIN);

  // Đọc trạng thái nút nhấn
  data.button1 = digitalRead(BUTTON1_PIN);
  data.button2 = digitalRead(BUTTON2_PIN);
  data.button3 = digitalRead(BUTTON3_PIN);
  data.button4 = digitalRead(BUTTON4_PIN);

    Serial.print("Joystick Value: ");
    Serial.print(data.joystickValue);
    Serial.print(" | Button 1: ");
    Serial.print(data.button1);
    Serial.print(" | Button 2: ");
    Serial.print(data.button2);
    Serial.print(" | Button 3: ");
    Serial.print(data.button3);
    Serial.print(" | Button 4: ");
    Serial.println(data.button4);

  const char text[] = "Hello World";
  
  // Gửi dữ liệu
  bool success = radio.write(&text, sizeof(text));
  
  if (success) {
    Serial.println("Data sent: Hello World");
  } else {
    Serial.println("Sending failed");
  }

  delay(1000); // Gửi mỗi 1 giây
}