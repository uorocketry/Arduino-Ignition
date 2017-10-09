#include <Servo.h>
#include <Arduino.h>

Servo servo;

int close_servo = 25;
int servoPin = 9;
const int led = 6; // place mat for igniter

/* Assign states to input commands */ 
typedef enum States{
STOP = '1',
IGNITE = '2',
OPEN = '3',
WAIT = '4',
CLOSE = '5'
};

States state;
void stateMachine(enum States state);

void setup()
{
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), kill_test, LOW);
  servo.attach(servoPin);
  // initial state is stop
  state = (States)STOP;
}

void loop()
{
  /* Read input Commands */
  if (Serial.available()) {
    state = (States)Serial.read();
    Serial.write(state);
    // TO DO: some check statement if serial input is not a defined enum value
  }
  stateMachine(state);
}

/* Emergency Stop Function */
void kill_test() {
  servo.attach(servoPin);
  Serial.println("END");
  servo.write(close_servo);
  servo.detach();
}

/*State machine function */
void stateMachine(enum States state)
{
  switch (state){
   case STOP:
   // Go to either Ignite, wait
     digitalWrite(led, LOW);
     state = WAIT;
     break;
   case IGNITE:
     digitalWrite(led, HIGH);
     state = WAIT;
     break;
   case OPEN:
     valveOpen();
   // Go to CLOSE
   state = CLOSE;
     break;
   case CLOSE:
     valveClose();
     // Go to OPEN or RESET
     state = WAIT;
     break;
   case WAIT:
     // delay until some command is given
     break;
   default:
     break;
  }
}

/* Valve Open Sequence */
void valveOpen(){
  
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
}

/* Valve Closed */
void valveClose(){
  servo.write(close_servo);
  delay(500);
  servo.detach();
}
