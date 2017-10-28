#include <Servo.h>
#include <Arduino.h>

Servo servo;

int close_servo = 17;
int servoPin = 9;

const int relay = 7; // place mat for igniter relay
const int led_green = 10;
const int led_red = 11;

/* Assign states to input commands */
typedef enum State {
        STOP = '1',
        IGNITE = '2',
        OPEN = '3',
        WAIT = '4',
        CLOSE = '5',
        PING = '6'
};

State state;
State temp;
/* Function Prototypes */
State stateMachine(State state);
State serialDelay(int numofdelays, int delay_length, State next_state);
State valveOpen();
State valveClose();

void setup()
{
        pinMode(relay, OUTPUT);
        pinMode(led_green, OUTPUT);
        pinMode(led_red, OUTPUT);

        digitalWrite(relay, LOW);
        digitalWrite(led_green, LOW);
        digitalWrite(led_red, LOW);

        Serial.begin(9600);
        servo.attach(servoPin);
        // initial state is close
        state = (State)CLOSE;
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
}

/*State machine function */
State stateMachine(State state)
{
        switch (state) {
        case STOP:
                servo.attach(servoPin);
                digitalWrite(relay, LOW);
                digitalWrite(led_green, HIGH);
                digitalWrite(led_red, LOW);

                servo.write(close_servo);
                servo.detach();
                //Serial.println("END");
                state = WAIT;
                break;
        case IGNITE:
                digitalWrite(relay, HIGH);  
                digitalWrite(led_green, LOW);
                digitalWrite(led_red, HIGH);                           
                state = serialDelay(20,100,OPEN);
                break;
        case OPEN:
                Serial.println("opening");
                digitalWrite(relay, HIGH);
                digitalWrite(led_green, HIGH);
                digitalWrite(led_red, HIGH); 
                state = valveOpen();
                Serial.println("done");
                break;
        case CLOSE:
                Serial.println("closing");
                digitalWrite(relay, LOW);
                digitalWrite(led_green, HIGH);
              digitalWrite(led_red, LOW); 
                state = valveClose();
                state = WAIT;
                break;
        case WAIT:
                // delay until some command is given
                digitalWrite(relay, LOW);
                digitalWrite(led_green, HIGH);
                digitalWrite(led_red, LOW); 
                break;

        case PING:
              Serial.println("I am ALIVE!");
              state = WAIT;
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
        state = serialDelay(20, 100, WAIT); //wait 2 seconds
        servo.write(158); //open all the way
        state = serialDelay(100,100, CLOSE); //wait
        return state;
}

/* Valve Closed */
State valveClose(){
        State state = CLOSE;
        servo.attach(servoPin); //mount servo
        servo.write(70);
        state = serialDelay(5, 10, state);
        servo.detach();
        return state;
}

/* delay Call back function */
State serialDelay(int numofdelays, int delay_length, State next_state){
        State state = next_state;
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
