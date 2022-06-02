#include <Stepper.h>

int stepper_speed = 120;
// Số bước để hoàn thành 1 chu kỳ quay (1 vòng quay)
int steps_1_cycle = 200;
Stepper stepper_motor(steps_1_cycle, 8, 9, 10, 11);   

// Chân digital cảm biến
int dig = 13;

// Chế độ tự động - dùng cảm biến
bool isAutoMode = true;

// Nút nhấn ở ngắt số 2
bool isButtonPressed = false;

// Số vòng mà ta muốn động cơ bước quay
int rotation = steps_1_cycle/2;

// Hàm thay đổi chế độ tự động - thủ công
int lastPressMode = 0;
void changeMode()
{
  if(millis()-lastPressMode > 1000)
  {    
    isAutoMode = !isAutoMode;
    Serial.print(">> Che do : ");
    if(isAutoMode)
      Serial.println("Tu dong");
    else
      Serial.println("Thu cong");
  }
  lastPressMode = millis();
}

// Hàm mở-đóng giếng trời thủ công
int lastPressOnOff = 0;
void changeOnOff()
{
  if(millis()-lastPressOnOff > 1000)
  {    
    // Chỉ điều khiển thủ công được khi đang ở chế độ thủ công
    if(!isAutoMode)
    {
     isButtonPressed = true; 
    }
  }
  lastPressOnOff = millis();
}

void closeSkyLight() 
{
  stepper_motor.step(rotation);
}

void openSkyLight() 
{
  stepper_motor.step(-1*rotation);
}

void setup() 
{
  Serial.begin(9600);
  // Chân cảm biến
  pinMode(dig, INPUT);
  // Chân ngắt
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, changeOnOff, RISING);
  
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, changeMode, RISING);
  
  // Tốc độ động cơ bước
  stepper_motor.setSpeed(stepper_speed);
}

int lastSensorValue = 1;
void loop() 
{
  // Khi không có mưa thì giá trị digital là 1
  int sensorValue = digitalRead(dig);

  // Chế độ tự động - Điều khiển bằng cảm biến
  if(isAutoMode)
  {
      Serial.print(">> Mua : ");
      if(sensorValue==1)
        Serial.println("Khong");
      else
        Serial.println("Co");
        
      if(sensorValue==1 && lastSensorValue==0)
      {
        Serial.print(">> Mo gieng troi, Toc do : ");
        Serial.println(stepper_speed);
        openSkyLight();
      }
      else if(sensorValue==0 && lastSensorValue==1)
      {
        Serial.print(">> Dong gieng troi, Toc do : ");
        Serial.println(stepper_speed);
        closeSkyLight();
      }
      else{  }
  }
  else
  {
      if(isButtonPressed)
      {
        Serial.print(">> Dong mo gieng troi thu cong, Toc do :");
        Serial.println(stepper_speed);
        openSkyLight();
      }
  }
  
  lastSensorValue = sensorValue;
  isButtonPressed = false;
  delay(300);
}
