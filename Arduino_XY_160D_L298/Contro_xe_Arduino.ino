#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Chân CE và CSN của NRF24L01
#define CE_PIN 9
#define CSN_PIN 10

// Chân điều khiển động cơ
#define ENA 5  // PWM cho động cơ trái
#define IN1 6  // Chiều quay động cơ trái
#define IN2 7
#define ENB 3  // PWM cho động cơ phải
#define IN3 4  // Chiều quay động cơ phải
#define IN4 2

// Tạo đối tượng RF24
RF24 radio(CE_PIN, CSN_PIN);

// Địa chỉ pipe (kênh) để giao tiếp
const byte address[6] = {'0', '0', '0', '0', '1'};

char receivedData[32]; // Chuỗi để lưu dữ liệu nhận được
int speedValue = 100;  // Giá trị tốc độ ban đầu

void setup() {
    Serial.begin(115200);
    radio.begin();

    // Cấu hình NRF24L01
    radio.setPALevel(RF24_PA_MIN);
    radio.openReadingPipe(0, address);
    radio.startListening(); // Chuyển sang chế độ nhận
    Serial.println("Receiver ready");
    radio.printDetails();

    // Cấu hình chân động cơ
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void loop() {
    if (radio.available()) {
        radio.read(&receivedData, sizeof(receivedData));
        Serial.print("Received data: ");
        Serial.println(receivedData);

        int joystickValue, button1, button2, button3, button4;
        sscanf(receivedData, "X:%d,F:%d,B:%d,A:%d,D:%d", &joystickValue, &button1, &button2, &button3, &button4);

        if (button3 == 1) speedValue += 20; // Nút D - tăng tốc
        if (button2 == 1) speedValue -= 20; // Nút B - giảm tốc
        if (speedValue > 255) speedValue = 255;
        if (speedValue < 0) speedValue = 0;

        if (button4 == 1) { // Nút A - dừng động cơ
            stopMotors();
        } else {
            controlMotors(joystickValue, speedValue);
        }
    }
    delay(100);
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}

void controlMotors(int joystickValue, int speed) {
    int neutral = 512; // Giá trị trung tâm của joystick (0 - 1023)
    int turnThreshold = 100; // Ngưỡng để phân biệt rẽ trái, phải

    if (joystickValue < neutral - turnThreshold) { // Nghiêng trái, xe rẽ trái
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENA, speed);
        analogWrite(ENB, speed / 2); // Giảm tốc độ động cơ phải để rẽ trái
    } else if (joystickValue > neutral + turnThreshold) { // Nghiêng phải, xe rẽ phải
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENA, speed / 2); // Giảm tốc độ động cơ trái để rẽ phải
        analogWrite(ENB, speed);
    } else { // Đi thẳng
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENA, speed);
        analogWrite(ENB, speed);
    }
}
