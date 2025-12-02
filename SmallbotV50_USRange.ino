/*
--------------------------------------------------------------------------------
ダンゴムシモードのみ
--------------------------------------------------------------------------------

1.ダンゴムシメイズ
　超音波センサが反応すると、左右交互に曲がります。（ダンゴムシの交替性転向反応）
　スライダーで方向転換の回転時間が変更できます。
　障害物を配置してスタートからゴールまでうまく走らせてみましょう。

モータの接続はM1が右、M2が左になっています。
*/


#include <I2CLiquidCrystal.h>		// LCDを使う。I2C_SDA：2 I2C_SCL:3
I2CLiquidCrystal lcd(40, false);	// StrawberryLinux社扱い（I2Cアドレス:0x3e）

// MotorDriver Pin Assign on RDC.
// Mx_1 Mx_2　回転方向
//   H   L    　正
//   L   H    　逆
//   L   L      停止
//   H   H      ブレーキ

#define M1_1		4	// 右モータ
#define M1_2		9
#define M1_PWM		6
#define M2_1		7	// 左モータ
#define M2_2		8
#define M2_PWM		5
#define PING_PIN	11 // 超音波測距センサ
#define BUTTON_PIN	12
#define LED_PIN		13

int sliderValue = 0;
int duration = 0;
int cm = 0;;

// the setup routine runs once when you press reset:
void setup() {                
  lcd.begin(16,2);		// 16x2のLCDを利用
  // initialize serial communication:
  // initialize the digital pin.
  pinMode(M1_1, OUTPUT);
  pinMode(M1_2, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  pinMode(M2_1, OUTPUT);
  pinMode(M2_2, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //モータスピードは、机上での使用と電池の消耗（超音波センサの電圧がシビアなため）を考慮して低速で定速にする
  analogWrite(M1_PWM, 120);
  analogWrite(M2_PWM, 120);

  sliderValue = analogRead(A3); // スライダーの読み取り
  lcd.setCursor(0,0);		// X,Yで位置指定
  lcd.print("SLIDER=     ");
  lcd.setCursor(7,0);		// X,Yで位置指定
  lcd.print(sliderValue);
  lcd.setCursor(0,1);		// X,Yで位置指定
  lcd.print("DIST. =     ");
  lcd.setCursor(7,1);		// X,Yで位置指定
  lcd.print(cm);

}


//--------------------------------------------------------------------------------
// ロボット動作
//
//　　停止して距離測定、
//  　障害物がない＝＞直進
//  　障害物がある＝＞スライダ値読み込み（旋回量を決定）
//　　　旋回
//    　旋回を３回数繰り返したら逆向きに６回、以下６回ごとに反転
//
//--------------------------------------------------------------------------------

#define RSTATE_STOP          0  // 停止
#define	RSTATE_STOP_WAIT     1  // 停止後の時間稼ぎ
#define RSTATE_MEASURE       2  // 距離計測
#define RSTATE_FORWARD       3  // 前進
#define	RSTATE_FORWARD_WAIT  4  // 前進時間待ち
#define	RSTATE_ROT           5  // 回転
#define	RSTATE_ROT_WAIT      6  // 回転完了待ち

int RState = RSTATE_STOP;	// 最初は停止状態
int Timer = 0;              // 時間稼ぎ用のタイマ	


void loop() {
  if (RState == RSTATE_STOP) {                   // 停止状態
    digitalWrite(M1_1,LOW);
    digitalWrite(M1_2,LOW);
    digitalWrite(M2_1,LOW);
    digitalWrite(M2_2,LOW);
    analogWrite(M1_PWM,0);
    analogWrite(M2_PWM,0);
    Timer = 10;                                  // 10ms停止
    RState = RSTATE_STOP_WAIT;                   // 停止待ち
  } else if (RState == RSTATE_STOP_WAIT) {
    Timer = Timer-1;                             // カウントダウン
    if (Timer == 0) {                            // カウントダウン終了
      RState = RSTATE_MEASURE;                   // 計測ステートに遷移
    }
  } else if (RState == RSTATE_MEASURE) {         // 計測ステート
    sliderValue = analogRead(A3);                // 回転時間はスライダで設定する
    if (sliderValue == 0) {                      // もし0だったら1にしておく
      sliderValue = sliderValue+1;
    }
    pinMode(PING_PIN, OUTPUT);                   // 出力モードに設定して
    digitalWrite(PING_PIN, LOW);                 // L=>H=>Lのパルス生成
    delayMicroseconds(2);
    digitalWrite(PING_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(PING_PIN, LOW);

    pinMode(PING_PIN, INPUT);  // パルスが返ってくるのを待つ
    duration = pulseIn(PING_PIN, HIGH,10000);
    if (duration == 0) {      // タイムアウト（10000μs）すると０になるので
      duration = 10000;       // 最大値だったということにする
    }
    cm = duration / 29 / 2;                      // 1cmあたり約29μs
    lcd.setCursor(0,0);		// X,Yで位置指定
    lcd.print("SLIDER=     ");
    lcd.setCursor(7,0);		// X,Yで位置指定
    lcd.print(sliderValue);
    lcd.setCursor(0,1);		// X,Yで位置指定
    lcd.print("DIST. =     ");
    lcd.setCursor(7,1);		// X,Yで位置指定
    lcd.print(cm);

    if (cm < 20) {                               // 障害物まで20cm未満
      RState = RSTATE_ROT;                       // なら方向転換
    } else {                                     // 20cm以上ある
      RState = RSTATE_FORWARD;                   // なら方向転換
    }
  } else if (RState == RSTATE_FORWARD) {         // 前進
    digitalWrite(M1_1,HIGH);
    digitalWrite(M1_2,LOW);
    digitalWrite(M2_1,HIGH);
    digitalWrite(M2_2,LOW);
    analogWrite(M1_PWM,120);
    analogWrite(M2_PWM,120);
    Timer = 100;
    RState = RSTATE_FORWARD_WAIT;
  } else if (RState == RSTATE_FORWARD_WAIT) {
    Timer = Timer-1;
    if (Timer == 0) {
      RState = RSTATE_STOP;
    }
  } else if (RState == RSTATE_ROT) {
    digitalWrite(M1_1,LOW);                  // 右は逆転
    digitalWrite(M1_2,HIGH);
    digitalWrite(M2_1,HIGH);                 // 左は正転
    digitalWrite(M2_2,LOW);
    analogWrite(M1_PWM,80);
    analogWrite(M2_PWM,80);
    Timer = sliderValue;
    RState = RSTATE_ROT_WAIT;
  } else if (RState == RSTATE_ROT_WAIT) {
    Timer = Timer-1;                         // タイマカウントダウン
    if (Timer == 0) {                        // カウントダウン終了したら
      RState = RSTATE_STOP;
    }
  } else {
    Timer = 0;
    RState = RSTATE_STOP;
  }
  delay(1);
}

