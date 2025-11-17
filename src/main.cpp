#include <M5Unified.h>
#include <ESP32Servo.h>

// 3つのサーボ
Servo servo1;
Servo servo2;
Servo servo3;

// サーボピン
const int SERVO1_PIN = 5;  // G5
const int SERVO2_PIN = 6;  // G6
const int SERVO3_PIN = 7;  // G7

// 360°サーボの速度設定（マイクロ秒）
const int SPEED_STOP = 1500;   // 停止（中立位置）
const int SPEED_SLOW = 1650;   // 遅い回転（時計回り）
const int SPEED_FAST = 3000;   // 速い回転（時計回り）

// テスト設定
const int ROTATE_TIME = 3000;  // 各速度で3秒回転

void testServoSpeed(Servo &servo, int servoNum, int pin) {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.printf("Servo %d\n", servoNum);
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.printf("Pin: G%d\n", pin);
  M5.Display.println("");
  
  Serial.printf("=== Servo%d G%d Test ===\n", servoNum, pin);
  
  // 停止
  servo.writeMicroseconds(SPEED_STOP);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(2);
  M5.Display.println("STOP");
  M5.Display.setTextSize(1);
  M5.Display.printf("%dus", SPEED_STOP);
  Serial.printf("Speed: STOP (%dus)\n", SPEED_STOP);
  delay(2000);
  
  // 遅い回転
  servo.writeMicroseconds(SPEED_SLOW);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(2);
  M5.Display.println("SLOW");
  M5.Display.setTextSize(1);
  M5.Display.printf("%dus", SPEED_SLOW);
  Serial.printf("Speed: SLOW (%dus)\n", SPEED_SLOW);
  delay(ROTATE_TIME);
  
  // 速い回転
  servo.writeMicroseconds(SPEED_FAST);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(2);
  M5.Display.println("FAST");
  M5.Display.setTextSize(1);
  M5.Display.printf("%dus", SPEED_FAST);
  Serial.printf("Speed: FAST (%dus)\n", SPEED_FAST);
  delay(ROTATE_TIME);
  
  // 停止
  servo.writeMicroseconds(SPEED_STOP);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(2);
  M5.Display.println("STOP");
  M5.Display.setTextSize(1);
  M5.Display.printf("%dus", SPEED_STOP);
  Serial.printf("Speed: STOP (%dus)\n", SPEED_STOP);
  delay(1000);
}

void runAllTests() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("Test...");
  delay(1000);
  
  Serial.println("=== Starting 3x360 Servo Test ===");
  
  // Servo 1 (G5)
  testServoSpeed(servo1, 1, SERVO1_PIN);
  delay(500);
  
  // Servo 2 (G6)
  testServoSpeed(servo2, 2, SERVO2_PIN);
  delay(500);
  
  // Servo 3 (G7)
  testServoSpeed(servo3, 3, SERVO3_PIN);
  delay(500);
  
  Serial.println("=== All Tests Complete ===");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("All Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("");
  M5.Display.println("Press button");
  M5.Display.println("to test again");
  
  delay(2000);
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.println("3x360 Servo Test");
  M5.Display.println("================");
  M5.Display.println("");
  M5.Display.println("Pins: G5,G6,G7");
  M5.Display.println("");
  M5.Display.println("Each servo:");
  M5.Display.println(" Stop->Slow->Fast");
  M5.Display.println(" ->Stop");
  M5.Display.println(" 3sec rotation");
  M5.Display.println("");
  M5.Display.println("Press to start");
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  Serial.begin(115200);
  Serial.println("3x360 Servo Auto Test");
  
  // サーボ初期化
  Serial.printf("Init Servo1: Pin=%d\n", SERVO1_PIN);
  servo1.attach(SERVO1_PIN);
  servo1.writeMicroseconds(SPEED_STOP);
  
  Serial.printf("Init Servo2: Pin=%d\n", SERVO2_PIN);
  servo2.attach(SERVO2_PIN);
  servo2.writeMicroseconds(SPEED_STOP);
  
  Serial.printf("Init Servo3: Pin=%d\n", SERVO3_PIN);
  servo3.attach(SERVO3_PIN);
  servo3.writeMicroseconds(SPEED_STOP);
  
  Serial.println("Servo Init Complete");
  
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("3x360 Servo Test");
  M5.Display.println("================");
  M5.Display.println("");
  M5.Display.println("Pins: G5,G6,G7");
  M5.Display.println("");
  M5.Display.println("Each servo:");
  M5.Display.println(" Stop->Slow->Fast");
  M5.Display.println(" ->Stop");
  M5.Display.println(" 3sec rotation");
  M5.Display.println("");
  M5.Display.println("Press to start");
}

void loop() {
  M5.update();
  
  // ボタン押下：テスト開始
  if (M5.BtnA.wasPressed()) {
    Serial.println("Button pressed - Starting test");
    runAllTests();
  }
  
  delay(10);
}