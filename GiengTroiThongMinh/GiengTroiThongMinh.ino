#include <Stepper.h>

// Số bước để hoàn thành 1 chu kỳ quay (1 vòng quay)
int steps_1_cycle = 200;
Stepper stepper_motor(steps_1_cycle, 8, 9, 10, 11);   

// Chân digital cảm biến
int dig = 13;

// Chế độ tự động - dùng cảm biến (true:mở, false:đóng)
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
    Serial.println(isAutoMode);
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
  stepper_motor.setSpeed(120);
}

int lastSensorValue = 1;
void loop() 
{
  // Khi không có mưa thì giá trị digital là 1
  int sensorValue = digitalRead(dig);
  //Serial.println(sensorValue);

  // Chế độ tự động - Điều khiển bằng cảm biến
  if(isAutoMode)
  {
      if(sensorValue==1 && lastSensorValue==0)
      {
        openSkyLight();
      }
      else if(sensorValue==0 && lastSensorValue==1)
      {
        closeSkyLight();
      }
      else{  }
  }
  else
  {
      if(isButtonPressed)
        openSkyLight();
  }
  
  lastSensorValue = sensorValue;
  isButtonPressed = false;
  delay(300);
}
