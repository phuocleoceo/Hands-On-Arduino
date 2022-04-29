#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

// Chân digital cảm biến
int dig = 13;
// Chân standby
int STBY = 10; 

//MotorA
int PWMA = 5; // Tốc độ
int AIN1 = 9; // Hướng
int AIN2 = 8; // Hướng
//MotorB
int PWMB = 6; // Tốc độ
int BIN1 = 11; // Hướng
int BIN2 = 12; // Hướng

// Chạy hay dừng ?
int isRunning = false;
// Tốc độ
int current_speed = 128;
// Hướng quay động cơ
boolean dir[] = {LOW, HIGH};
// Đang quay theo chiều thuận
boolean forwardDirection = true;
// Digital Value trước đó
int dig_before = 1;

// Hàm cho ngắt 0 để thay đổi tốc độ
int lastPressSpeed = 0;
void changeSpeed()
{
  if(millis()-lastPressSpeed > 500)
  {    
    if(current_speed == 128) 
      current_speed = 255;
    else 
      current_speed = 128;
  }
  lastPressSpeed = millis();
}

// Hàm cho ngắt 1 để thay đổi hướng
int lastPressDirection = 0;
void changeDirection()
{
  if(millis()-lastPressDirection > 500)
  {    
    dir[0] = !dir[0];
    dir[1] = !dir[1];
    forwardDirection = !forwardDirection;
  }
  lastPressDirection = millis();
}


void setup() 
{  
  Serial.begin(9600);

  // Ngat tai chan so 2, 3
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(0, changeSpeed, RISING);
  attachInterrupt(1, changeDirection, RISING);
  // Chân cảm biến
  pinMode(dig, INPUT);
  // Chân standby
  pinMode(STBY, OUTPUT);
  // Chân Motor A
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  // Chân Motor B
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  // Khởi tạo LCD
  lcd.init();  
  lcd.backlight();
}

void loop() 
{
  int digValue = digitalRead(dig);  

  // Khi không có âm thanh thì giá trị digital là 1
  if(digValue == 0 && dig_before == 1)
  {
    Serial.println(digValue);
    isRunning = !isRunning;  
  }
  dig_before = digValue;

  if (isRunning)
  {
    move(1, current_speed, 0); //motor 1, current_speed, left
    move(2, current_speed, 1); //motor 2, current_speed, right
  }
  else
  {
    move(1, 0, 0); //motor 1, 0 speed, left
    move(2, 0, 1); //motor 2, 0 speed, right
  }
  // In ra LCD
  lcd.setCursor(0,0);
  lcd.print("Toc do : ");
  lcd.print(current_speed);
  lcd.setCursor(0,1);
  lcd.print("Chieu : ");
  if(forwardDirection) lcd.print("Thuan");
  else lcd.print("Nguoc");
}

void move(int motor, int speed, int direction)
{
// motor: 1 - motorA , 2 - motorB
// speed: tốc độ động cơ 0(tắt) -> 255
// direction: 0 chiều đồng hồ, 1 ngược chiều đồng hồ

  // Vô hiệu hóa chế độ standby
  digitalWrite(STBY, HIGH); 

  // Hướng động cơ
  boolean inPin1 = dir[0];
  boolean inPin2 = dir[1];
  // Hướng động cơ ngược lại
  if(direction == 1)
  {
    inPin1 = dir[1];
    inPin2 = dir[0];
  }
  // Xuất điện áp LOW/HIGH ra 2 chân để quay động cơ
  // Xuất tốc độ bằng chân PWM
  if(motor == 1)
  {
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }
  else
  {
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}
