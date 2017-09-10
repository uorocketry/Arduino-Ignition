#include <Servo.h>
int inPin = 2;         // the number of the input pin

boolean switch_state = false;      // the current state of the output pin
boolean switch_reading;           // the current reading from the input pin
boolean switch_prev = true;    // the previous reading from the input pin

long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void setup()
{
  pinMode(inPin, INPUT);
  Serial.begin(9600);
}

void loop()
{
  switch_reading = digitalRead(inPin);
 
  if (switch_reading != switch_prev && millis() - time > debounce or switch_reading != switch_prev && millis() - time > debounce) {
    if (switch_state)
      switch_state = false;
    else
      switch_state = true;

    time = millis();    
  }
  
//**************************** insert code for valve in here***************************************  
      Servo servo;
      if(switch_state)
        Serial.println("on");
          servo.attach(9);

          servo.write(70);
           delay(2000);
    
          for(int pos = 70; pos<=110; pos++){
            servo.write(pos);
            delay(40);  
          }
        
          for(int pos=110; pos<=180; pos++){
            servo.write(pos);
            delay(15);
          }
          delay(10000);
          servo.write(25);
          delay(500);
          servo.detach();
      else
        Serial.println("off");
//*************************************************************************************************
        
  switch_prev = switch_reading;
}
