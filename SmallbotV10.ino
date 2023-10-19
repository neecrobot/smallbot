/*
スモールロボット最初の一歩
モータの接続はM1が右、M2が左になっています。
明るさセンサの横にあるLED13を点灯することで、環境光の影響を減らします。
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
  // シリアルポートの初期化
  Serial.begin(9600);

  // モータ関係の出力ピン
  pinMode(M1_1, OUTPUT);
  pinMode(M1_2, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  pinMode(M2_1, OUTPUT);
  pinMode(M2_2, OUTPUT);
  pinMode(M2_PWM, OUTPUT);

  // デジタル出力ピン（LED)
  pinMode(LED_PIN, OUTPUT);

  // モータスピード設定
  analogWrite(M1_PWM, 80);	// 右モーターの速度（0～255)
  analogWrite(M2_PWM, 120);	// 左モーターの速度（0～255）

  // モーター回転方向設定
  digitalWrite(M1_1, 1);  // 右モーター正転
  digitalWrite(M1_2, 0);
  digitalWrite(M2_1, 1);  // 右モーター正転
  digitalWrite(M2_2, 0);
  
  // LED点灯
  digitalWrite(LED_PIN, HIGH);

}

//----------------------------------------
// loop()は動作中連続して繰り返し
// 実行される
// 今回は走りっぱなしなので、何もすることはない
//----------------------------------------
//
void loop() {

}
