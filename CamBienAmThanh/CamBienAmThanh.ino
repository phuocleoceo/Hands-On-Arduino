#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

// Cong analog va digital gan voi cam bien
int ana = A0;
int dig = 13;
// Cac cong digital noi voi Led
int led[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
int quantity = 10;
// Gia tri max-min analog
int maxAna = 0;
int minAna = 9999;
// Che do LCD
int LCDMode = 0;

// Fix loi ham ngat chay nhieu lan khi nhan Button
int lastPress = 0;
void changeLCD()
{
  if(millis()-lastPress > 500)
  {    
    if(LCDMode == 0) 
      LCDMode = 1;
    else 
      LCDMode = 0;
  }
  lastPress = millis();
}

// minAnalog...value...maxAnalog   =>  minExpect...x...maxExpect 
// (x-minExpect)/(maxExpect-minExpect) = (value-minAnalog)/(maxAnalog-minAnalog)           
int mappingValue(int value, int minAnalog, int maxAnalog, int minExpect, int maxExpect) 
{
  return (value - minAnalog) * (maxExpect - minExpect) / (maxAnalog - minAnalog) + minExpect;
}

int getLed(int value)
{
  return mappingValue(value, 520, 820, 0, quantity);
}

int getDecibel(int value)
{
  return mappingValue(value, 0, 1023, 48, 66);
}

void setup() 
{
  Serial.begin(9600);
  // Ngat tai chan so 2
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, changeLCD, RISING);
  
  // Chan cam bien, chan analog mac dinh la INPUT
  // pinMode(ana, INPUT);
  pinMode(dig, INPUT);
  
  // Chan digital Led dat la OUTPUT
  for (int i = 0; i < quantity ; i++)
  {
    pinMode(led[i], OUTPUT);
  }
  
  // Khoi tao LCD
  lcd.init();  
  lcd.backlight(); 
}

void loop() {
  // Doc gia tri Analog va Digital cua cam bien
  int anaValue = analogRead(ana);
  int digValue = digitalRead(dig);  
  Serial.println(anaValue);  

  if (anaValue > maxAna) 
    maxAna = anaValue;
  if (anaValue < minAna) 
    minAna = anaValue;

  // Anh xa gia tri analog sang so luong den led
  int n = getLed(anaValue);
  for (int i = 0 ; i < quantity ; i++)
  {
    if(i < n)
      digitalWrite(led[i], HIGH);
    else
      digitalWrite(led[i], LOW);
  } 
  
  if (LCDMode == 0)
  {
    // Su dung gia tri Digital
    lcd.setCursor(0,0);
    if(digValue == 0) 
      lcd.print("Khong am thanh   ");
    else
      lcd.print("Co am thanh      "); 
    // Su dung gia tri Analog
    lcd.setCursor(0,1);
    lcd.print("Cuong do: ");
    lcd.print(getDecibel(anaValue));
    lcd.print("  ");
    lcd.setCursor(14,1);
    lcd.print("dB");
  } 
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Thap nhat: ");
    lcd.print(getDecibel(minAna));
    lcd.print("   ");
    lcd.setCursor(14,0);
    lcd.print("dB");
    
    lcd.setCursor(0,1);
    lcd.print("Cao nhat : ");
    lcd.print(getDecibel(maxAna));
    lcd.print("   ");
    lcd.setCursor(14,1);
    lcd.print("dB");
  }      
  
  delay(50);
}
