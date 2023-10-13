/*
スモールロボット最初の一歩
*/

// MotorDriver Pin Assign on RDC.
//   Mx_2      Mx_1   動作
//  0(LOW)    0(LOW)  停止
//  0(LOW)    1(HIGH) 正転（前進する方向に回転） 
//  1(HIGH)   0(LOW)  逆転（後退する方向に回転）
//  1(HIGH)   1(HIGH) ブレーキ（停止）

// 右モータ
#define M1_1  	4
#define M1_2 		9
#define M1_PWM	6

// 左モータ
#define M2_1		7
#define M2_2		8
#define M2_PWM	5

#define LED_PIN		  13

//----------------------------------------
// setup()は起動時最初に一回だけ実行される
// 初期化などはここで行う
//----------------------------------------
//
void setup() {                
  // モータ関係の出力ピン
  pinMode(4, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(5, OUTPUT);

  // デジタル出力ピン（LED)
  pinMode(13, OUTPUT);

  // モータスピード設定
  // 右モーターの速度（0～255)
  analogWrite(6, 80);
  // 左モーターの速度（0～255）
  analogWrite(5, 120);

  // モーター回転方向設定
  // 右モーター正転
  digitalWrite(4, 1);
  digitalWrite(9, 0);
  // 右モーター正転
  digitalWrite(7, 1);
  digitalWrite(8, 0);
  
  // LED点灯
  digitalWrite(13, 1);
}

//----------------------------------------
// loop()は動作中連続して繰り返し
// 実行される
// 今回は走りっぱなしなので、何もすることはない
//----------------------------------------
//
void loop() {

}
