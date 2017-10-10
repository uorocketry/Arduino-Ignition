#include <Servo.h>
#include <Arduino.h>

Servo servo;

int close_servo = 25;
int servoPin = 9;
const int led = 6; // place mat for igniter

/* Assign states to input commands */ 
typedef enum State{
STOP = '1',
IGNITE = '2',
OPEN = '3',
WAIT = '4',
CLOSE = '5'
};

State state;
enum State stateMachine(enum State state);

void setup()
{
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(9600);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), kill_test, LOW);
  servo.attach(servoPin);
  // initial state is stop
  state = (State)STOP;
}

void loop()
{
  /* Read input Commands */
  if (Serial.available()) {
    state = (State)Serial.read();
    Serial.write(state);
    // TO DO: some check statement if serial input is not a defined enum value
  }
  state = stateMachine(state);
  Serial.println(state);
}

/* Emergency Stop Function */
void kill_test() {
  servo.attach(servoPin);
  Serial.println("END");
  servo.write(close_servo);
  servo.detach();
}

/*State machine function */
enum State stateMachine(enum State state)
{
  switch (state){
   case STOP:
     digitalWrite(led, LOW);
     state = WAIT;
     break;
   case IGNITE:
     digitalWrite(led, HIGH);
     state = WAIT;
     break;
   case OPEN:
     valveOpen();
     state = CLOSE;
     break;
   case CLOSE:
     valveClose();
     state = WAIT;
     break;
   case WAIT:
     // delay until some command is given
     break;
   default:
     break;
  }
  return state;
}

/* Valve Open Sequence */
void valveOpen(){
  servo.attach(servoPin);
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
