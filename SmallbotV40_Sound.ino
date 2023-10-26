/*
ダンス
　音センサが反応すると、回転、直進が交互に変わります。
　スライダーで音センサのしきい値を変更できます。
　音を出してダンスするように走らせてみましょう。
　また、タイミングよく動きを変えてスタートからゴールまで動かしてみましょう。
  モータの接続はM1が右、M2が左になっています。
*/

#include <I2CLiquidCrystal.h>		// LCDを使う。I2C_SDA：2 I2C_SCL:3
I2CLiquidCrystal lcd(40, false);	// StrawberryLinux社扱い（I2Cアドレス:0x3e）

// MotorDriver Pin Assign on RDC.
#define M1_1		4	// 右モータ
#define M1_2		9
#define M1_PWM		6
#define M2_1		7	// 左モータ
#define M2_2		8
#define M2_PWM  	5
#define PING_PIN	11	// 超音波測距センサ
#define BUTTON_PIN	12
#define LED_PIN		13

// ロボットの動作ステート
#define	RBT_STOP         0
#define RBT_STOP_WAIT    1
#define RBT_MEASURE      2
#define RBT_ROT          3
#define RBT_ROT_WAIT     4
#define RBT_FORWARD      5
#define RBT_FORWARD_WAIT 6

int sliderValue = 0;  // スライダの値を取り込む変数
unsigned int SoundLevel = 0;     // 周囲の騒音レベルは0でスタート

// the setup routine runs once when you press reset:
void setup() {                
  lcd.begin(16,2);		// 16x2のLCDを利用
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
  sliderValue = analogRead(A3)/3; // スライダーの読み取り
  lcd.setCursor(0,0);		// X,Yで位置指定
  lcd.print("SLIDER=     ");
  lcd.setCursor(7,0);		// X,Yで位置指定
  lcd.print(sliderValue);
  lcd.setCursor(0,1);		// X,Yで位置指定
  lcd.print("SOUND =     ");
  lcd.setCursor(7,1);		// X,Yで位置指定
  lcd.print(SoundLevel);
}


//----------------------------------------
// 繰り返し処理
//----------------------------------------
//

int RState = RBT_STOP;           // 停止状態からスタート
unsigned int Timer = 0;          // とりあえずタイマはクリア状態

void loop() {
  unsigned int insound;

  insound = analogRead(A4);            // 音声レベルを取得
  if (SoundLevel < insound) {          // 音が大きかったら
    lcd.setCursor(0,0);		// X,Yで位置指定
    lcd.print("SLIDER=     ");
    lcd.setCursor(7,0);    // X,Yで位置指定
    lcd.print(sliderValue);
    lcd.setCursor(0,1);		// X,Yで位置指定
    lcd.print("SOUND =     ");
    lcd.setCursor(7,1);    // X,Yで位置指定
    lcd.print(SoundLevel);
    SoundLevel = insound;              // レベル更新（最大値を見つける）
  }

  if (RState == RBT_STOP) {            // 停止ステート
    digitalWrite(M1_1, LOW);           // 右モーター停止
    digitalWrite(M1_2, LOW);
	digitalWrite(M2_1, LOW);           // 左モーター停止
	digitalWrite(M2_2, LOW);
	analogWrite(M1_PWM, 0);            // モーター回転速度
	analogWrite(M2_PWM, 0);
    Timer = 10;                         // 10ms待つ
    RState = RBT_STOP_WAIT;             // 停止完了待ち
  } else if (RState == RBT_STOP_WAIT) { // 停止待ち
    Timer = Timer-1;                    // タイマをカウントダウン
    if (Timer == 0)                     // カウントダウン終了したら
    RState = RBT_MEASURE;               // 音声レベル判定
  } else if (RState == RBT_MEASURE) {   // 音声を計測
    sliderValue = analogRead(A3)/3;    // スライダ値を取得して
    if (sliderValue < SoundLevel) {     // マイクの音声が大きい
      RState = RBT_ROT;                 //  回転
    } else {                            // マイクの音声が小さい
      RState = RBT_FORWARD;             //  前進
    }
    SoundLevel = 0;                     // 最大音量をクリア
  } else if (RState == RBT_ROT) {       // 右回転開始ステート
    digitalWrite(M1_1, LOW);            // 右モーター反転
    digitalWrite(M1_2, HIGH);
    digitalWrite(M2_1, HIGH);           // 左モーター正転
    digitalWrite(M2_2, LOW);
	analogWrite(M1_PWM, 80);            // モーター回転速度
	analogWrite(M2_PWM, 80);

    Timer = 600;                        // 600ms(0.6秒）回転
    RState = RBT_ROT_WAIT;              // 回転完了待ち
  } else if (RState == RBT_ROT_WAIT) {  // 右回転完了待ちステート
    Timer = Timer-1;                    // タイマカウントダウン
    if (Timer == 0) {                   // カウントダウン終了したら
      RState = RBT_STOP;                // 停止ステートに移行
    }
  } else if (RState == RBT_FORWARD) {   // 前進ステート
    digitalWrite(M1_1, HIGH);           // 右モーター正転
    digitalWrite(M1_2, LOW);
    digitalWrite(M2_1, HIGH);           // 左モーター正転
    digitalWrite(M2_2, LOW);
	analogWrite(M1_PWM, 120);           // モーター回転速度
	analogWrite(M2_PWM, 120);

    Timer = 100;                        // 100ms前進させる
    RState = RBT_FORWARD_WAIT;          // 前進完了待ち
  } else if (RState == RBT_FORWARD_WAIT) {
    Timer = Timer-1;                    // タイマカウントダウン
    if (Timer == 0) {                   // カウントダウン終了
      RState = RBT_STOP;                // 停止ステートに移行
    }
  } else {                              // ありえないはずだけど
    Timer = 0;                          // とりあえずタイマはクリアしておく
    RState = RBT_STOP;                  // 停止ステートに遷移
  }
  delay(1);
}
