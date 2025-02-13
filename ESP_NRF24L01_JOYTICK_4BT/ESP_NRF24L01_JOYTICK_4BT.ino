#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Định nghĩa chân NRF24L01
#define CE_PIN  D8
#define CSN_PIN D4

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = {'0', '0', '0', '0', '1'}; // Địa chỉ giao tiếp

#define JOYSTICK_PIN A0
#define BUTTON1_PIN D0
#define BUTTON2_PIN D2
#define BUTTON3_PIN D3
#define BUTTON4_PIN D9

void setup() {
  

  Serial.begin(115200);
  radio.begin();
  
  // Cấu hình NRF24L01
  radio.setPALevel(RF24_PA_MIN);       // Giảm công suất để ổn định
  // radio.setDataRate(RF24_1MBPS);       // Đồng bộ tốc độ truyền
  // radio.setRetries(3, 5);              // Thiết lập retries (3 lần, mỗi lần 5ms)

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

  int joystickValue = analogRead(JOYSTICK_PIN);
  int button1 = digitalRead(BUTTON1_PIN);
  int button2 = digitalRead(BUTTON2_PIN);
  int button3 = digitalRead(BUTTON3_PIN);
  int button4 = digitalRead(BUTTON4_PIN);

  // Tạo chuỗi dữ liệu định dạng
  char dataToSend[32];
  sprintf(dataToSend, "X:%d,F:%d,B:%d,A:%d,D:%d", joystickValue, button1, button2, button3, button4);

  // Gửi chuỗi dữ liệu
  bool success = radio.write(&dataToSend, sizeof(dataToSend));
  if (success) {
    Serial.print("Data sent: ");
    Serial.println(dataToSend);
  } else {
    Serial.println("Sending failed");
  }


  delay(500); // Gửi mỗi 1 giây
}