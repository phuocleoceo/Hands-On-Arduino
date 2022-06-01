#include <Stepper.h>

// Số bước để hoàn thành 1 chu kỳ quay (1 vòng quay)
int steps_1_cycle = 200;
Stepper stepper_motor(steps_1_cycle, 8, 9, 10, 11);   

// Chân digital cảm biến
int dig = 13;

// Trạng thái của giếng trời (true:mở, false:đóng)
bool SkyLightStatus = true;
// Số vòng mà ta muốn động cơ bước quay
int rotation = 3*steps_1_cycle;

void closeSkyLight() {
  SkyLightStatus = false;
  stepper_motor.step(rotation);
}
void openSkyLight() {
  SkyLightStatus = true;
  stepper_motor.step(-1*rotation);
}

void setup() 
{
  Serial.begin(9600);
  // Chân cảm biến
  pinMode(dig, INPUT);
  // Toc do dong co buoc
  stepper_motor.setSpeed(120);
}

int lastSensorValue = 1;
void loop() 
{
  // Khi không có mưa thì giá trị digital là 1
  int sensorValue = digitalRead(dig);
  Serial.println(sensorValue);
  if(sensorValue==1 && lastSensorValue==0)
  {
    openSkyLight();
  }
  else if(sensorValue==0 && lastSensorValue==1)
  {
    closeSkyLight();
  }
  else{  }
  lastSensorValue = sensorValue;
  delay(300);
}
