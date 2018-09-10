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

// change this to the number of steps on your motor
#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 4, 5, 6, 7);

// defines pins numbers for ultrasonic sensor
const int UltrasonicTrigPin = 9;
const int UltrasonicEchoPin = 10;
// defines variables for ultrasonic sensor
long duration1;
int distance1;

int Switch_inPin = 22;        // the number of the input pin
int Switch_state = HIGH;      // the current state of the output pin
int Switch_reading;           // the current reading from the input pin
int Switch_previous = LOW;    // the previous reading from the input pin
// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long Switch_time = 0;         // the last time the output pin was toggled
long Switch_debounce = 200;   // the debounce time, increase if the output flickers


void setup()
{
  Serial.begin(9600); // Starts the serial communication
  // set the speed of the motor to 60 RPMs
  stepper.setSpeed(60);

  pinMode(UltrasonicTrigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(UltrasonicEchoPin, INPUT); // Sets the echoPin as an Input

  // Select switch input
  pinMode(Switch_inPin, INPUT);

  // Turn off LED
  pinMode(13, OUTPUT);
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
  
  if (digitalRead(Switch_inPin) == HIGH){

    // Ultrasonic sensor
    // Clears the UltrasonicTrigPin wait 2 micro seconds
    digitalWrite(UltrasonicTrigPin, LOW);  
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(UltrasonicTrigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(UltrasonicTrigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration1 = pulseIn(UltrasonicEchoPin, HIGH);
    // Calculating the distance
    distance1 = duration1 * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance1);

    if (distance1<150){
      // Step forward until the encoder has moved one rotation
      while (knobLeft.read() < 12) {
        stepper.step(1);
      }
      // Step reverse until the encoder has reversed to 0 rotation
      while (knobLeft.read() > 0) {
        stepper.step(-1);
      }      
      //Serial.println("Forward");
      //stepper.step(STEPS);
      //Serial.println("Backward");
      //stepper.step(-STEPS);
      delay(100);   // Noise in the system was causing the ultrasonic sensor to read a low value?? so adding delay to avoid false positive
    }
  }
  else
  {
    // Do nothing
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
