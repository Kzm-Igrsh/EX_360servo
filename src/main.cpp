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
const int PATTERN_ROTATE_TIME = 3000;  // 10パターンも3秒回転
const int PATTERN_INTERVAL = 3000;  // 10パターンのインターバル3秒

// 長押し判定時間（ミリ秒）
const int LONG_PRESS_TIME = 1000;  // 1秒以上で長押し

void stopAllServos() {
  servo1.writeMicroseconds(SPEED_STOP);
  servo2.writeMicroseconds(SPEED_STOP);
  servo3.writeMicroseconds(SPEED_STOP);
}

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
  M5.Display.println("Full Test");
  delay(1000);
  
  Serial.println("=== Starting Full Servo Test ===");
  
  // Servo 1 (G5)
  testServoSpeed(servo1, 1, SERVO1_PIN);
  delay(500);
  
  // Servo 2 (G6)
  testServoSpeed(servo2, 2, SERVO2_PIN);
  delay(500);
  
  // Servo 3 (G7)
  testServoSpeed(servo3, 3, SERVO3_PIN);
  delay(500);
  
  Serial.println("=== Full Test Complete ===");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Full Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 10x pattern");
  
  delay(2000);
}

void executePattern(int servoNum, int speed, int moveNum) {
  stopAllServos();
  
  Servo* targetServo;
  int pin;
  
  switch(servoNum) {
    case 1:
      targetServo = &servo1;
      pin = SERVO1_PIN;
      break;
    case 2:
      targetServo = &servo2;
      pin = SERVO2_PIN;
      break;
    case 3:
      targetServo = &servo3;
      pin = SERVO3_PIN;
      break;
  }
  
  const char* speedName = (speed == SPEED_SLOW) ? "SLOW" : "FAST";
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.printf("Move %d/10\n", moveNum);
  M5.Display.println("==============");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.printf("Servo %d\n", servoNum);
  M5.Display.println(speedName);
  M5.Display.setTextSize(1);
  M5.Display.printf("G%d:%dus", pin, speed);
  
  Serial.printf("Move %d/10: Servo%d G%d %s (%dus)\n", moveNum, servoNum, pin, speedName, speed);
  
  targetServo->writeMicroseconds(speed);
  delay(PATTERN_ROTATE_TIME);  // 3秒回転
  targetServo->writeMicroseconds(SPEED_STOP);
  delay(PATTERN_INTERVAL);  // 3秒インターバル
}

void run10Pattern() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("10x Pattern");
  delay(1000);
  
  Serial.println("\n=== 10 Pattern Fixed Sequence ===");
  
  // 固定の10パターン（コンパイル時に決定）
  // 全箇所×全速度を網羅 + 同じ場所で速度変更パターンあり
  
  executePattern(2, SPEED_SLOW, 1);   // Servo2 遅い
  executePattern(1, SPEED_FAST, 2);   // Servo1 速い
  executePattern(3, SPEED_SLOW, 3);   // Servo3 遅い
  executePattern(1, SPEED_SLOW, 4);   // Servo1 遅い（速度変更パターン1回目）
  executePattern(2, SPEED_FAST, 5);   // Servo2 速い
  executePattern(3, SPEED_FAST, 6);   // Servo3 速い
  executePattern(1, SPEED_FAST, 7);   // Servo1 速い（速度変更パターン2回目）
  executePattern(3, SPEED_SLOW, 8);   // Servo3 遅い
  executePattern(2, SPEED_SLOW, 9);   // Servo2 遅い
  executePattern(2, SPEED_FAST, 10);  // Servo2 速い
  
  stopAllServos();
  
  Serial.println("=== 10 Pattern Complete ===\n");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("10x Pattern");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 10x pattern");
  
  delay(2000);
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
  M5.Display.println("Short press:");
  M5.Display.println(" Full test");
  M5.Display.println("");
  M5.Display.println("Long press:");
  M5.Display.println(" 10x pattern");
  M5.Display.println("");
  M5.Display.println("Press to start");
}

void loop() {
  M5.update();
  
  // ボタンが押された瞬間の時刻を記録
  if (M5.BtnA.wasPressed()) {
    unsigned long pressStartTime = millis();
    
    // ボタンが離されるまで待つ
    while (M5.BtnA.isPressed()) {
      M5.update();
      delay(10);
    }
    
    // 押していた時間を計算
    unsigned long pressDuration = millis() - pressStartTime;
    
    if (pressDuration >= LONG_PRESS_TIME) {
      // 長押し：10パターン実行
      Serial.printf("Long press detected (%lums)\n", pressDuration);
      run10Pattern();
    } else {
      // 短押し：フルテスト実行
      Serial.printf("Short press detected (%lums)\n", pressDuration);
      runAllTests();
    }
  }
  
  delay(10);
}