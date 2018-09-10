/*  Arduino Progarm
*/
#include <Stepper.h>
#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knobLeft(2, 3);
Encoder knobRight(11, 12);
//   avoid using pins with LEDs attached

/* TURN OFF STEPPER
// change this to the number of steps on your motor
#define STEPS 200
// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 4, 5, 6, 7);
*/

// DC motor settings
const int MOT_A1_PIN = 4;
const int MOT_A2_PIN = 5;
const int MOT_B1_PIN = 6;
const int MOT_B2_PIN = 7;

// defines pins numbers for ultrasonic sensor
const int UltrasonicTrigPin = 9;
const int UltrasonicEchoPin = 10;
// defines variables for ultrasonic sensor
long duration1;
int distance1;

int Switch_inPin = 26;        // the number of the input pin
int Switch_state = HIGH;      // the current state of the output pin
int Switch_reading;           // the current reading from the input pin
int Switch_previous = LOW;    // the previous reading from the input pin
// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long Switch_time = 0;         // the last time the output pin was toggled
long Switch_debounce = 200;   // the debounce time, increase if the output flickers
int Switch_directionPin = 22; // the number of the input pin

// Timer
unsigned long time;

// Extra items
int counter1;
int encoder_down;
int motor_off_encoder;

// LED pin
int LED_pin = 13;        

void setup()
{
  Serial.begin(9600); // Starts the serial communication

  /* TURN OFF STEPPER
  // set the speed of the motor to 60 RPMs
  stepper.setSpeed(60);
  */

  // DC motor setup
  // Initialize the stepper driver control pins to output drive mode.
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);
  pinMode(MOT_B1_PIN, OUTPUT);
  pinMode(MOT_B2_PIN, OUTPUT);
  // Start with drivers off, motors coasting.
  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);
  digitalWrite(MOT_B1_PIN, LOW);
  digitalWrite(MOT_B2_PIN, LOW);
  
  // Ultrasonic Sensor
  pinMode(UltrasonicTrigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(UltrasonicEchoPin, INPUT); // Sets the echoPin as an Input

  // Select switch input
  pinMode(Switch_inPin, INPUT);
  pinMode(Switch_directionPin, INPUT);

  // Turn off on-board LED
  pinMode(13, OUTPUT);

  // Initialize digital pin LED_BUILTIN as an output.  
  pinMode(LED_pin, OUTPUT);     // alternatively can use "LED_BUILTIN"

  // extra initialize
  counter1 = 0;
  encoder_down = 1085;
  motor_off_encoder = 0;
}

// Encoder setting
long positionLeft  = -999;
long positionRight = -999;

void loop()
{
  // Set left and right encoders
  long newLeft, newRight;
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  if (newLeft != positionLeft || newRight != positionRight) {
    Serial.print("Left = ");
    Serial.print(newLeft);
    Serial.print(", Right = ");
    Serial.print(newRight);
    Serial.println();
    positionLeft = newLeft;
    positionRight = newRight;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    knobLeft.write(0);
    knobRight.write(0);
  }
  
  if (digitalRead(Switch_inPin) == LOW && motor_off_encoder == 0){
    counter1 = 0;
    time = millis(); 
    if (digitalRead(Switch_directionPin) == HIGH) {
      Serial.println("Motor Forward");
      digitalWrite(MOT_A1_PIN, HIGH);      
      digitalWrite(MOT_A2_PIN, LOW);
      while ((digitalRead(Switch_directionPin) == HIGH) && (digitalRead(Switch_inPin) == LOW) && (motor_off_encoder == 0)) {
        if (knobLeft.read() <= -1 || millis()-time>9000) {
          motor_off_encoder = 1;  // Automatically turns motor off once conditions are met
        }
      }
      Serial.println(millis()-time);
    }
    if (digitalRead(Switch_directionPin) == LOW) {
      Serial.println("Motor backwards");
      digitalWrite(MOT_A1_PIN, LOW);      
      digitalWrite(MOT_A2_PIN, HIGH);
      while ((digitalRead(Switch_directionPin) == LOW) && (digitalRead(Switch_inPin) == LOW) && (motor_off_encoder == 0)) {
        if (knobLeft.read() >= encoder_down || millis()-time>4000) {
          motor_off_encoder = 1;  // Automatically turns motor off once conditions are met
        }
      }
      Serial.println(millis()-time);
    }
      
    /*
    // Ultrasonic sensor
    // Clears the UltrasonicTrigPin wait 2 micro seconds
    digitalWrite(UltrasonicTrigPin, LOW);  
    delayMicroseconds(5);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(UltrasonicTrigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(UltrasonicTrigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration1 = pulseIn(UltrasonicEchoPin, HIGH);
    // Calculating the distance
    distance1 = duration1 * 0.034 / 2 / 2.54;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance1);
    delay(250);
    */
    
    /*
    if (distance1<12 && distance1>1){
      // Turn motor on until encoder is over 12 then stop and reverse until encoder is less than or equal to 0
      //set_motor_pwm(50,MOT_A1_PIN,MOT_A2_PIN);
      //analogWrite(MOT_A1_PIN, 255);
      digitalWrite(MOT_A1_PIN, HIGH);      
      digitalWrite(MOT_A2_PIN, LOW);
      time = millis();           
      while ((knobLeft.read() < 12) && (millis() < time+1000)) {
      }
      //set_motor_pwm(0,MOT_A1_PIN,MOT_A2_PIN);
      digitalWrite(MOT_A1_PIN, LOW);
      digitalWrite(MOT_A2_PIN, LOW);     
      delay(1000);   
      //set_motor_pwm(-50,MOT_A1_PIN,MOT_A2_PIN);
      digitalWrite(MOT_A1_PIN, LOW);
      digitalWrite(MOT_A2_PIN, HIGH); 
      //analogWrite(MOT_A2_PIN, 255);
      time = millis();           
      while ((knobLeft.read() > -2) && (millis() < time+1000)) {
      }
      //set_motor_pwm(0,MOT_A1_PIN,MOT_A2_PIN);
      digitalWrite(MOT_A1_PIN, LOW);
      digitalWrite(MOT_A2_PIN, LOW);            
      
      delay(100);   // Noise in the system was causing the ultrasonic sensor to read a low value?? so adding delay to avoid false positive
    }
    */
  }
  else
  {
    if (counter1 == 0) {
      Serial.println("Motor Off");    // Turns off motor if the door was stopped manually or automatically
      digitalWrite(MOT_A1_PIN, LOW);
      digitalWrite(MOT_A2_PIN, LOW); 
      counter1=1;
      Serial.println("Wait until switch is released");    
      while (digitalRead(Switch_inPin) == LOW) {
        // Waits in this loop until the switch is turned off to change the direction
      }
      if ((digitalRead(Switch_inPin) == HIGH) && (motor_off_encoder == 1)) {
        Serial.println("Reset encoder shutoff");    
        motor_off_encoder = 0;    // Bring encoder shutoff variable back to 0
        delay(100);               // Prevents any switch bounce from briefly turning on motors
      }
    }
    /*
    // Blink LED on and off
    digitalWrite(LED_pin, HIGH);      // turn the LED on (HIGH is the voltage level)
    delay(500);                       // wait 
    digitalWrite(LED_pin, LOW);       // turn the LED off by making the voltage LOW
    delay(500);                       // wait 
    */
  }  
  
  
}


/* Unused
   
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (Switch_reading == HIGH && Switch_previous == LOW && millis() - Switch_time > Switch_debounce) {
    if (Switch_state == HIGH)
      Switch_state = LOW;
    else
      Switch_state = HIGH;

    Switch_time = millis();
  }
  Switch_previous = Switch_reading;
   
*/
/* switch
   Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
   press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
   a minimum delay between toggles to debounce the circuit (i.e. to ignore
   noise).
   David A. Mellis (21 November 2006)
*/
/*
  int inPin = 2;         // the number of the input pin
  int outPin = 13;       // the number of the output pin

  int state = HIGH;      // the current state of the output pin
  int reading;           // the current reading from the input pin
  int previous = LOW;    // the previous reading from the input pin

  // the follow variables are long's because the time, measured in miliseconds,
  // will quickly become a bigger number than can be stored in an int.
  long time = 0;         // the last time the output pin was toggled
  long debounce = 200;   // the debounce time, increase if the output flickers

  void setup()
  {
  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);
  }

  void loop()
  {
  reading = digitalRead(inPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;

    time = millis();
  }

  digitalWrite(outPin, state);

  previous = reading;
  }


  if (condition1)
  {
  // do Thing A
  }
  else if (condition2)
  {
  // do Thing B
  }
  else
  {
  // do Thing C
  }
*/
