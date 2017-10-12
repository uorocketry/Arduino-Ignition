#include <Servo.h>
#include <Arduino.h>

Servo servo;

int close_servo = 25;
int servoPin = 9;
const int led = 6; // place mat for igniter
unsigned long previous_millis = 0;

/* Assign states to input commands */ 
typedef enum State{
STOP = '1',
IGNITE = '2',
OPEN = '3',
WAIT = '4',
CLOSE = '5'
};

State state;
/* Function Prototypes */
State stateMachine(State state);
State serialDelay(int numofdelays, int delay_length);
State valveOpen();
State valveClose();

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
    // TO DO: some check statement if serial input is not a defined enum value
  }
  state = stateMachine(state);
  Serial.println(state);
}

/*State machine function */
State stateMachine(State state)
{
  switch (state){
   case STOP:
     servo.attach(servoPin);
     digitalWrite(led, LOW);
     servo.write(close_servo);
     servo.detach();
     Serial.println("END");
     state = WAIT;
     break;
   case IGNITE:
     digitalWrite(led, HIGH);
     state = WAIT;
     break;
   case OPEN:
     state = valveOpen();
     break;
   case CLOSE:
     state = valveClose();
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
State valveOpen(){
  State state = OPEN;
  servo.attach(servoPin);
  servo.write(70);
  state = serialDelay(20, 100);
  
  for (int pos = 70; pos <= 110; pos++) {
    servo.write(pos);
    state = serialDelay(4, 10);
  }
  
  for (int pos = 110; pos <= 180; pos++) {
    servo.write(pos);
    state = serialDelay(3, 5);
  }
  state = serialDelay(100,100);
  return state;
}

/* Valve Closed */
State valveClose(){
  State state = CLOSE;
  servo.write(close_servo);
  state = serialDelay(5, 10);
  servo.detach();
  return state;
}

/* delay Call back function */
State serialDelay(int numofdelays, int delay_length){
  State state = CLOSE;
  for(int i=0;i<numofdelays;i++){
    delay(delay_length);
    if (Serial.available()){
      state = (State)Serial.read();
      if(state == STOP){
        break;  
      }
    }
  }
  return state;
}
