#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

// Thông số wifi
const char* ssid = "TripleTruong";
const char* password = "vidieukhien";

// Server cổng 80
AsyncWebServer server(80);

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

// Giá trị cảm biến
int sensorValue = 1;
// Chạy hay dừng ?
bool isRunning = false;
// Tốc độ
int current_speed = 128;
// Hướng quay động cơ
boolean dir[] = {LOW, HIGH};

// Hàm thay dổi tốc độ
void changeSpeed()
{
  if(current_speed == 128) 
    current_speed = 255;
  else 
    current_speed = 128;
}

// Hàm thay đổi hướng
void changeDirection()
{
  dir[0] = !dir[0];
  dir[1] = !dir[1];
}

void setup() 
{
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

  // Check xem SPIFFS có nạp được file html, css hay không
  if(!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  // Tạo kết nối
  Serial.print("Setting connect...");
  WiFi.softAP(ssid, password);
  
  // Server dùng file html để hiển thị
  server.on("/",HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html");
  });

  // Server dùng file css để tạo style
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Server lắng nghe sự kiện
  server.on("/update",HTTP_GET, [](AsyncWebServerRequest *request)
  {
    String requestMessage;
    if (request->hasParam("method")) 
    {
      requestMessage = request->getParam("method")->value();
      switch(requestMessage)
      {
        case "On":
        {
          isRunning = true;
          break;
        }
        case "Off":
        {
          isRunning = false;
          break;
        }
        case "Reverse":
        {
          changeDirection();
          break;
        }
        case "Speed":
        {
          changeSpeed();
          break;
        }
        default: { }
      }
    }
    else
      requestMessage = "No request";

    Serial.println(requestMessage);
    request->send(200, "text/plain", "OK");
  });

  // Server trả về giá trị cảm biến cho Client
  server.on("/sensorValue",HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", sensorValue);
  });

  server.begin();
  Serial.println("Server started !");
  
  Serial.print("Sorf-AP IP Address : ");
  Serial.println(WiFi.softAPIP());
}

void loop() 
{
  // Khi không có vật cản thì giá trị digital là 1
  sensorValue = digitalRead(dig);

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
