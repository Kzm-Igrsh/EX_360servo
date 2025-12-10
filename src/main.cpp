#include <M5Unified.h>
#include <ESP32Servo.h>

// 3つのサーボ
Servo servoLeft;
Servo servoCenter;
Servo servoRight;

// サーボピン
const int SERVO_LEFT_PIN = 5;    // G5 - Left
const int SERVO_CENTER_PIN = 6;  // G6 - Center
const int SERVO_RIGHT_PIN = 7;   // G7 - Right

// 360°サーボの速度設定（マイクロ秒）
const int SPEED_STOP = 1500;   // Stop
const int SPEED_WEAK = 1650;   // Weak
const int SPEED_STRONG = 3000; // Strong

// テスト設定
const int ROTATE_TIME = 3000;  // 各速度で3秒回転

// 長押し判定時間（ミリ秒）
const int LONG_PRESS_TIME = 1000;  // 1秒以上で長押し

// 10パターンの動作時間（1000-3000ms）とインターバル（0-500ms）
const int patternRotateTimes[10] = {2500, 1500, 2000, 1200, 2800, 1800, 2200, 1000, 2600, 1400};  // ms
const int patternIntervals[10] = {400, 250, 150, 350, 50, 450, 200, 300, 0, 100};  // ms (0-500ms)

// ========================================
// シリアル通信用ヘルパー関数
// PC側のPython GUIが期待する厳密なフォーマットで出力
// ========================================
void sendStimMessage(const char* position, const char* strength) {
  Serial.printf("%s,%s\n", position, strength);
}

void stopAllServos() {
  servoLeft.writeMicroseconds(SPEED_STOP);
  servoCenter.writeMicroseconds(SPEED_STOP);
  servoRight.writeMicroseconds(SPEED_STOP);
}

void testServoSpeed(Servo &servo, const char* position, int pin) {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.printf("%s\n", position);
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.printf("Pin: G%d\n", pin);
  M5.Display.println("");
  
  // Stop
  servo.writeMicroseconds(SPEED_STOP);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(2);
  M5.Display.println("Stop");
  M5.Display.setTextSize(1);
  M5.Display.printf("%dus", SPEED_STOP);
  delay(2000);
  
  // Weak - 刺激開始メッセージ送信
  sendStimMessage(position, "Weak");
  servo.writeMicroseconds(SPEED_WEAK);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(2);
  M5.Display.println("Weak");
  M5.Display.setTextSize(1);
  M5.Display.printf("%dus", SPEED_WEAK);
  delay(ROTATE_TIME);
  
  // Weak終了 - インターバル開始
  servo.writeMicroseconds(SPEED_STOP);
  sendStimMessage("none", "none");
  delay(1000);
  
  // Strong - 刺激開始メッセージ送信
  sendStimMessage(position, "Strong");
  servo.writeMicroseconds(SPEED_STRONG);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(2);
  M5.Display.println("Strong");
  M5.Display.setTextSize(1);
  M5.Display.printf("%dus", SPEED_STRONG);
  delay(ROTATE_TIME);
  
  // Strong終了 - Stop
  servo.writeMicroseconds(SPEED_STOP);
  sendStimMessage("none", "none");
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(2);
  M5.Display.println("Stop");
  M5.Display.setTextSize(1);
  M5.Display.printf("%dus", SPEED_STOP);
  delay(1000);
}

void runAllTests() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("Full Test");
  delay(1000);
  
  // Left (G5)
  testServoSpeed(servoLeft, "Left", SERVO_LEFT_PIN);
  delay(500);
  
  // Center (G6)
  testServoSpeed(servoCenter, "Center", SERVO_CENTER_PIN);
  delay(500);
  
  // Right (G7)
  testServoSpeed(servoRight, "Right", SERVO_RIGHT_PIN);
  delay(500);
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Full Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 20x pattern");
  
  delay(2000);
}

void executePattern(const char* position, int speed, int moveNum, int rotateTime, int intervalTime) {
  stopAllServos();
  delay(100);
  
  Servo* targetServo;
  int pin;
  
  if (strcmp(position, "Left") == 0) {
    targetServo = &servoLeft;
    pin = SERVO_LEFT_PIN;
  } else if (strcmp(position, "Center") == 0) {
    targetServo = &servoCenter;
    pin = SERVO_CENTER_PIN;
  } else {
    targetServo = &servoRight;
    pin = SERVO_RIGHT_PIN;
  }
  
  const char* strengthName = (speed == SPEED_WEAK) ? "Weak" : "Strong";
  
  // 画面表示
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.printf("Move %d/20\n", moveNum);
  M5.Display.println("==============");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.printf("%s\n", position);
  M5.Display.printf("%s\n", strengthName);
  M5.Display.setTextSize(1);
  M5.Display.printf("G%d:%dus\n", pin, speed);
  M5.Display.printf("Time:%dms\n", rotateTime);
  M5.Display.printf("Wait:%dms", intervalTime);
  
  // ========================================
  // 刺激開始 - PC側へメッセージ送信
  // ========================================
  sendStimMessage(position, strengthName);
  
  // モーター駆動
  targetServo->writeMicroseconds(speed);
  delay(rotateTime);
  targetServo->writeMicroseconds(SPEED_STOP);
  
  // ========================================
  // インターバル開始 - "none,none" を送信
  // ========================================
  if (intervalTime > 0) {
    sendStimMessage("none", "none");
    
    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.setTextSize(1);
    M5.Display.printf("Move %d/20\n", moveNum);
    M5.Display.println("==============");
    M5.Display.println("");
    M5.Display.setTextSize(2);
    M5.Display.println("None");
    M5.Display.println("");
    M5.Display.setTextSize(1);
    M5.Display.printf("Wait:%dms", intervalTime);
    
    delay(intervalTime);
  } else {
    // インターバルが0msの場合も "none,none" を送信
    sendStimMessage("none", "none");
  }
}

void run20Pattern() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("20x Pattern");
  delay(1000);
  
  // 1~10回目
  executePattern("Center", SPEED_WEAK, 1, patternRotateTimes[0], patternIntervals[0]);   // Center Weak 2500ms / 400ms
  executePattern("Left", SPEED_STRONG, 2, patternRotateTimes[1], patternIntervals[1]);   // Left Strong 1500ms / 250ms
  executePattern("Right", SPEED_WEAK, 3, patternRotateTimes[2], patternIntervals[2]);    // Right Weak 2000ms / 150ms
  executePattern("Left", SPEED_WEAK, 4, patternRotateTimes[3], patternIntervals[3]);     // Left Weak 1200ms / 350ms
  executePattern("Center", SPEED_STRONG, 5, patternRotateTimes[4], patternIntervals[4]); // Center Strong 2800ms / 50ms
  executePattern("Right", SPEED_STRONG, 6, patternRotateTimes[5], patternIntervals[5]);  // Right Strong 1800ms / 450ms
  executePattern("Left", SPEED_STRONG, 7, patternRotateTimes[6], patternIntervals[6]);   // Left Strong 2200ms / 200ms
  executePattern("Right", SPEED_WEAK, 8, patternRotateTimes[7], patternIntervals[7]);    // Right Weak 1000ms / 300ms
  executePattern("Center", SPEED_WEAK, 9, patternRotateTimes[8], patternIntervals[8]);   // Center Weak 2600ms / 0ms
  executePattern("Center", SPEED_STRONG, 10, patternRotateTimes[9], patternIntervals[9]);// Center Strong 1400ms / 100ms
  
  // 11~20回目（配列の順番をめちゃくちゃに、位置と強度も変更）
  executePattern("Right", SPEED_STRONG, 11, patternRotateTimes[8], patternIntervals[5]); // Right Strong 2600ms / 450ms
  executePattern("Left", SPEED_WEAK, 12, patternRotateTimes[3], patternIntervals[9]);    // Left Weak 1200ms / 100ms
  executePattern("Center", SPEED_STRONG, 13, patternRotateTimes[6], patternIntervals[2]); // Center Strong 2200ms / 150ms
  executePattern("Right", SPEED_WEAK, 14, patternRotateTimes[1], patternIntervals[7]);   // Right Weak 1500ms / 300ms
  executePattern("Left", SPEED_STRONG, 15, patternRotateTimes[9], patternIntervals[4]);  // Left Strong 1400ms / 50ms
  executePattern("Center", SPEED_WEAK, 16, patternRotateTimes[5], patternIntervals[8]);  // Center Weak 1800ms / 0ms
  executePattern("Right", SPEED_STRONG, 17, patternRotateTimes[0], patternIntervals[3]); // Right Strong 2500ms / 350ms
  executePattern("Left", SPEED_WEAK, 18, patternRotateTimes[7], patternIntervals[1]);    // Left Weak 1000ms / 250ms
  executePattern("Center", SPEED_STRONG, 19, patternRotateTimes[4], patternIntervals[6]); // Center Strong 2800ms / 200ms
  executePattern("Left", SPEED_STRONG, 20, patternRotateTimes[2], patternIntervals[0]);  // Left Strong 2000ms / 400ms
  
  stopAllServos();
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("20x Pattern");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 20x pattern");
  
  delay(2000);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // シリアル通信初期化（115200bps）
  Serial.begin(115200);
  
  // サーボ初期化
  servoLeft.attach(SERVO_LEFT_PIN);
  servoLeft.writeMicroseconds(SPEED_STOP);
  
  servoCenter.attach(SERVO_CENTER_PIN);
  servoCenter.writeMicroseconds(SPEED_STOP);
  
  servoRight.attach(SERVO_RIGHT_PIN);
  servoRight.writeMicroseconds(SPEED_STOP);
  
  // 初期画面表示
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("3x360 Servo Test");
  M5.Display.println("================");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.println("None");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 20x pattern");
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
      // 長押し：20パターン実行
      run20Pattern();
    } else {
      // 短押し：フルテスト実行
      runAllTests();
    }
    
    // テスト完了後、Noneを表示
    M5.Display.clear();
    M5.Display.setTextSize(1);
    M5.Display.setCursor(0, 0);
    M5.Display.println("3x360 Servo Test");
    M5.Display.println("================");
    M5.Display.println("");
    M5.Display.setTextSize(2);
    M5.Display.println("None");
    M5.Display.println("");
    M5.Display.setTextSize(1);
    M5.Display.println("Short: Full test");
    M5.Display.println("Long: 20x pattern");
  }
  
  delay(10);
}