#include <M5Unified.h>
#include <ESP32Servo.h>

Servo servo360;

// サーボピン（AtomS3のGrove互換ピン）
const int SERVO_PIN = 5;  // G5 (Grove Yellow)

// 360°サーボの速度設定（マイクロ秒）
const int SPEED_STOP = 1500;   // 停止（中立位置）
const int SPEED_SLOW = 1700;   // 遅い回転（時計回り）
const int SPEED_FAST = 3000;   // 速い回転（時計回り）

int speedMode = 0; // 0=停止, 1=遅い, 2=速い
int currentSpeed = SPEED_STOP;

void updateDisplay() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("360 Servo");
  M5.Display.println("");
  
  switch(speedMode) {
    case 0:
      M5.Display.println("STOP");
      M5.Display.println("");
      M5.Display.setTextSize(1);
      M5.Display.printf("PWM: %dus\n", SPEED_STOP);
      break;
    case 1:
      M5.Display.println("SLOW");
      M5.Display.println("");
      M5.Display.setTextSize(1);
      M5.Display.printf("PWM: %dus\n", SPEED_SLOW);
      break;
    case 2:
      M5.Display.println("FAST");
      M5.Display.println("");
      M5.Display.setTextSize(1);
      M5.Display.printf("PWM: %dus\n", SPEED_FAST);
      break;
  }
  
  M5.Display.println("");
  M5.Display.println("Press button");
  M5.Display.println("to change speed");
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // サーボ初期化
  servo360.attach(SERVO_PIN);
  servo360.writeMicroseconds(SPEED_STOP);
  
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("360 Servo Control");
  M5.Display.println("=================");
  M5.Display.println("");
  M5.Display.println("Press button");
  M5.Display.println("to change speed");
  M5.Display.println("");
  M5.Display.println("Stop -> Slow");
  M5.Display.println("-> Fast -> Stop");
  M5.Display.println("");
  M5.Display.println("Status: Stop");
  
  delay(2000);
  updateDisplay();
}

void loop() {
  M5.update();
  
  // ボタン押下：速度切り替え
  if (M5.BtnA.wasPressed()) {
    speedMode++;
    if (speedMode > 2) {
      speedMode = 0;
    }
    
    switch(speedMode) {
      case 0:
        currentSpeed = SPEED_STOP;
        break;
      case 1:
        currentSpeed = SPEED_SLOW;
        break;
      case 2:
        currentSpeed = SPEED_FAST;
        break;
    }
    
    servo360.writeMicroseconds(currentSpeed);
    updateDisplay();
  }
  
  delay(10);
}