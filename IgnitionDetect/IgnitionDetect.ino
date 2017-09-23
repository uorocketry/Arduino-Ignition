#include <Servo.h>
#include <Arduino.h>

  Servo servo;


int inPin = 2;         // the number of the input pin
int interruptPin = 3; // pin that triggers interrupt
int close_servo = 25;
int servoPin = 9;

boolean interrupted = false;
boolean ran = false;
boolean switch_state = false;      // the current state of the output pin
boolean switch_reading;           // the current reading from the input pin
boolean switch_prev = true;    // the previous reading from the input pin

long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void setup()
{
  pinMode(inPin, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(interruptPin), kill_test, LOW);
      servo.attach(servoPin);

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
  if (switch_state && !ran) {
    Serial.println("on");

    servo.write(70);
    delay(2000);

    for (int pos = 70; pos <= 110; pos++) {
      servo.write(pos);
      delay(40);
    }

    for (int pos = 110; pos <= 180; pos++) {
      servo.write(pos);
      delay(15);
    }
    delay(10000);
    servo.write(close_servo);
    delay(500);
    servo.detach();
    ran = true;
  }
  else {
    Serial.println("off");
  }
  //*************************************************************************************************

  switch_prev = switch_reading;
}

void kill_test() {
  servo.attach(servoPin);
  Serial.println("END");
  servo.write(close_servo);
  servo.detach();

  }
