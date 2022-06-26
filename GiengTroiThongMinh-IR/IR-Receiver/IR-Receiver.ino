#include <IRremote.h>
#include <IRremoteInt.h>
#include <Stepper.h>

// Chân nhận tín hiệu IR
int RECV_PIN = 3;
// Đối tượng nhận tín hiệu và giải mã kết quả
IRrecv irrecv(RECV_PIN);
decode_results results;

// Tốc độ ban đầu
int stepper_speed = 100;
// Số bước để hoàn thành 1 chu kỳ quay (1 vòng quay)
int steps_1_cycle = 200;

// Đối tượng động cơ bước nối các chân 8 9 10 11
Stepper stepper_motor(steps_1_cycle, 8, 9, 10, 11);  

// Số vòng mà ta muốn động cơ bước quay
int rotation = steps_1_cycle/2; 

// Chân digital cảm biến
int dig = 13;

// Chế độ tự động - dùng cảm biến
bool isAutoMode = true;

// Hàm đóng giếng trời
void closeSkyLight() 
{
  stepper_motor.step(rotation);
}

// Hàm mở giếng trời
void openSkyLight() 
{
  stepper_motor.step(-1*rotation);
}

// Hàm thay đổi chế độ
void changeMode()
{
  isAutoMode = !isAutoMode;
  if(isAutoMode)
    Serial.println(">> Chuyen sang che do tu dong");
  else
    Serial.println(">> Chuyen sang che do thu cong");
}

// Hàm điều khiển giếng trời thủ công
void manualControl()
{
  if(isAutoMode)
    Serial.println("Chuyen sang che do thu cong de su dung");
  else
  {
    Serial.print(">> Dong mo gieng troi thu cong, Toc do :");
    Serial.println(stepper_speed);
    stepper_motor.step(rotation);
  }     
}

// Hàm giảm tốc độ
void decreaseSpeed()
{
  stepper_speed = stepper_speed - 20;
  Serial.print("Giam toc do : ");
  Serial.println(stepper_speed);
  stepper_motor.setSpeed(stepper_speed);  
}

// Hàm tăng tốc độ
void increaseSpeed()
{
  stepper_speed = stepper_speed + 20;
  Serial.print("Tang toc do : ");
  Serial.println(stepper_speed);
  stepper_motor.setSpeed(stepper_speed);
}

void setup()
{
  Serial.begin(9600);
  // Chân cảm biến
  pinMode(dig, INPUT);
  // Tốc độ động cơ bước
  stepper_motor.setSpeed(stepper_speed);
  // Nhận tín hiệu hồng ngoại
  irrecv.enableIRIn(); 
}

int lastSensorValue = 1;
void loop() 
{
  // Khi không có mưa thì giá trị digital là 1
  int sensorValue = digitalRead(dig);

  // Nhận tín hiệu hồng ngoại
  if (irrecv.decode(&results)) 
  {     
//  Serial.println(results.value, HEX);  
    switch (results.value) 
    {
      case 0xa90:
        changeMode();
        break;
      case 0xa91:
        manualControl();       
        break;
      case 0xa92:
        decreaseSpeed();
        break;
      case 0xa93:
        increaseSpeed();
        break;
      default:
        break;
    }
    // Nhận tín hiệu tiếp theo
    irrecv.resume(); 
  }

  // Chế độ tự động - Điều khiển bằng cảm biến
  if(isAutoMode)
  {
      Serial.print(">> Mua : ");
      if(sensorValue == 1)
        Serial.println("Khong");
      else
        Serial.println("Co");
        
      if(sensorValue == 1 && lastSensorValue == 0)
      {
        Serial.print(">> Mo gieng troi, Toc do : ");
        Serial.println(stepper_speed);
        openSkyLight();
      }
      else if(sensorValue == 0 && lastSensorValue == 1)
      {
        Serial.print(">> Dong gieng troi, Toc do : ");
        Serial.println(stepper_speed);
        closeSkyLight();
      }
      else{ }
  }
  else
  { }
  
  lastSensorValue = sensorValue;
  delay(300);
}
