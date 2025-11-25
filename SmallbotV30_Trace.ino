/*
ライントレース（LED点灯）
　明るさセンサを使ったライントレースです。
　スライダーで明るさセンサのしきい値を変更できます。
　センサが1つですので、ラインの片側のエッジ（白と黒の境界）のみを辿ります。
　ライン上にスモールボットの中央を置くと、明るさセンサは進行方向でみてラインの左側になります。
　センサが白の上なら右旋回、センサが黒の上なら左旋回します。

       Forward
          ||
 M2 light ||       M1
          ||

モータの接続はM1が右、M2が左になっています。
明るさセンサの横にあるLED13を点灯することで、環境光の影響を減らします。
*/
#include <I2CLiquidCrystal.h>		// LCDを使う。I2C_SDA：2 I2C_SCL:3
I2CLiquidCrystal lcd(40, false);	// StrawberryLinux社扱い（I2Cアドレス:0x3e）


#define M1_1          4	// 右モータ回転方向
#define M1_2          9
#define M1_PWM        6	// 右モーター速度
#define M2_1          7	// 左モータ回転方向
#define M2_2          8
#define M2_PWM        5	// 左モーター速度
#define PING_PIN     11 	// 超音波測距センサ
#define BUTTON_PIN   12	// 押しボタンスイッチ
#define LED_PIN      13	// LED
#define PHOTO        A2	// フォトセンサ
#define SLIDER       A3	// スライダ

//----------------------------------------
// 初期化
//----------------------------------------
//
void setup() {
  lcd.begin(16,2);		// 16x2のLCDを利用

  pinMode(M1_1, OUTPUT);
  pinMode(M1_2, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  pinMode(M2_1, OUTPUT);
  pinMode(M2_2, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(M1_1,1);	// 右モータ正転
  digitalWrite(M1_2,0);
  digitalWrite(M2_1,1);	// 右モータ正転
  digitalWrite(M2_2,0);

  analogWrite(M1_PWM, 80); // 右モータ速度
  analogWrite(M2_PWM, 120); // 左モータ速度

  digitalWrite(LED_PIN, HIGH); // LED点灯

}

//----------------------------------------
// 繰り返し実行
//----------------------------------------
//
int sliderval = 0;
int photoval = 0;
void loop() {
  sliderval = analogRead(SLIDER);
  photoval = analogRead(PHOTO);
  lcd.setCursor(0,0);		// X,Yで位置指定
  lcd.print("SLIDER=     ");
  lcd.setCursor(8,0);		// X,Yで位置指定
  lcd.print(sliderval);
  lcd.setCursor(0,1);		// X,Yで位置指定
  lcd.print("PHOTO =     ");
  lcd.setCursor(8,1);		// X,Yで位置指定
  lcd.print(photoval);
  if (photoval > sliderval) {
    // 右旋回
    analogWrite(M1_PWM,  80); // 右モータ速度
    analogWrite(M2_PWM, 120); // 左モータ速度
  } else {
    // 左旋回
    analogWrite(M1_PWM, 120); // 右モータ速度
    analogWrite(M2_PWM,  80); // 左モータ速度
  }
  delay(100);  // 0.1秒くらいはそのまま進む
}

