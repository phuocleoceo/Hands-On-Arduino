// Các chân kết nối của 4 nút nhấn
int buttonMode = 8;
int buttonHandControl = 9;
int buttonDecreaseSpeed = 10;
int buttonIncreaseSpeed = 11;

int debounceDelay = 500; 
// Thời gian gần nhất nút nhấn được sử dụng
int lastModePress = 0;
int lastHandControlPress = 0;
int lastDecreaseSpeedPress = 0;
int lastIncreaseSpeedPress = 0;

// Lưu lại giá trị gần nhất của nút nhấn
int lastModeStatus = HIGH;
int lastHandControlStatus = HIGH;
int lastDecreaseSpeedStatus = HIGH;
int lastIncreaseSpeedStatus = HIGH;

void setup() 
{
  Serial.begin(9600);
  // Cài đặt chế độ cho 4 nút nhấn luôn ở mức điện áp cao
  pinMode(buttonMode, INPUT_PULLUP);
  pinMode(buttonHandControl, INPUT_PULLUP);
  pinMode(buttonDecreaseSpeed, INPUT_PULLUP);
  pinMode(buttonIncreaseSpeed, INPUT_PULLUP);
}

void loop() 
{
  int modeStatus = digitalRead(buttonMode);
  int handControlStatus = digitalRead(buttonHandControl);
  int decreaseSpeedStatus = digitalRead(buttonDecreaseSpeed);
  int increaseSpeedStatus = digitalRead(buttonIncreaseSpeed);

  // Nếu trạng thái của nút nhấn giữa giá trị vừa đọc được 
  // với giá trị gần nhất khác nhau thì thực hiện câu lệnh

  if ((millis()-lastModePress > debounceDelay) && (modeStatus != lastModeStatus)) 
  {
    Serial.println("Chuyen che do");
    lastModePress = millis();
  }

  if ((millis()-lastHandControlPress > debounceDelay) && (handControlStatus != lastHandControlStatus)) 
  {
    Serial.println("Dieu khien bang tay");
    lastHandControlPress = millis();
  }

  if ((millis()-lastDecreaseSpeedPress > debounceDelay) && (decreaseSpeedStatus != lastDecreaseSpeedStatus)) 
  {
    Serial.println("Giam toc do");
    lastDecreaseSpeedPress = millis();
  }

  if ((millis()-lastIncreaseSpeedPress > debounceDelay) && (increaseSpeedStatus != lastIncreaseSpeedStatus)) 
  {
    Serial.println("Tang toc do");
    lastIncreaseSpeedPress = millis();
  }

  // Cập nhật lại giá trị mới nhất của nút nhấn
  lastModeStatus = modeStatus;
  lastHandControlStatus = lastHandControlStatus;
  lastHandControlStatus = lastDecreaseSpeedStatus;
  lastIncreaseSpeedStatus = lastIncreaseSpeedStatus;
}
