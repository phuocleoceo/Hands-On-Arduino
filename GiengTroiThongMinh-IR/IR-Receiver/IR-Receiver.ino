#include <IRremote.h>
#include <IRremoteInt.h>

int RECV_PIN = 3;

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() 
{
  if (irrecv.decode(&results)) 
  {     
//    Serial.println(results.value, HEX);  
  switch (results.value) {
    case 0xa90:
      Serial.println("Chuyen che do");
      break;
    case 0xa91:
      Serial.println("Dieu khien bang tay");
      break;
    case 0xa92:
      Serial.println("Giam toc do");
      break;
    case 0xa93:
      Serial.println("Tang toc do");
      break;
    default:
      break;
  }
    irrecv.resume(); // Receive the next value
  }
}
