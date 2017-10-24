#include <Servo.h>
#include <Arduino.h>

Servo servo;

int close_servo = 17;
int servoPin = 9;
const int led = LED_BUILTIN; // place mat for igniter
const int led2 = 12;

/* Assign states to input commands */
typedef enum State {
        STOP = '1',
        IGNITE = '2',
        OPEN = '3',
        WAIT = '4',
        CLOSE = '5'
};

State state;
State temp;
/* Function Prototypes */
State stateMachine(State state);
State serialDelay(int numofdelays, int delay_length);
State valveOpen();
State valveClose();

void setup()
{
        pinMode(led, OUTPUT);
        pinMode(led2, OUTPUT);
        digitalWrite(led, LOW);
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
                temp = (State)Serial.read();
                Serial.println(temp);
                state = temp;
                // TO DO: some check statement if serial input is not a defined enum value
        }
        state = stateMachine(state);
        //Serial.println(state);
}

/*State machine function */
State stateMachine(State state)
{
        switch (state) {
        case STOP:
                servo.attach(servoPin);
                digitalWrite(led, HIGH);
                digitalWrite(led2, HIGH);
                servo.write(close_servo);
                servo.detach();
                //Serial.println("END");
                state = WAIT;
                break;
        case IGNITE:
                digitalWrite(led, LOW);                             
                digitalWrite(led2, LOW);

                state = WAIT;
                break;
        case OPEN:
                Serial.println("opening");
                digitalWrite(led, LOW);
                digitalWrite(led2, HIGH);
                state = valveOpen();
                Serial.println("done");
                break;
        case CLOSE:
                Serial.println("closing");
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
        servo.attach(servoPin); //mount servo
        servo.write(70); //test servo mounting
        state = serialDelay(20, 100); //wait 2 seconds
        servo.write(158); //open all the way
        state = serialDelay(100,100); //wait
        return state;
}

/* Valve Closed */
State valveClose(){
        State state = CLOSE;
        servo.attach(servoPin); //mount servo
        servo.write(70);
        state = serialDelay(5, 10);
        servo.detach();
        return state;
}

/* delay Call back function */
State serialDelay(int numofdelays, int delay_length){
        State state = CLOSE;
        for(int i=0; i<numofdelays; i++) {
                delay(delay_length);
                if (Serial.available()) {
                        state = (State)Serial.read();
                        if(state == STOP) {
                                Serial.println("STOPPED");
                                break;
                        }
                }
        }
        return state;
}
