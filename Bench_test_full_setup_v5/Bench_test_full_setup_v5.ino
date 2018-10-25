/*  
 *   Works as is. Raises until -1 on encoder, raises until 500
 *   Still have memory error for chip. M+aybe the chip was killed by an encoder on the same transmission port
 *     It sent garbled data to the terminal before failing.
 *     Check this https://github.com/esp8266/Arduino/issues/770
 *     Ran chip for a while and now it is having same issue. Could be encoder position on D0? or lack of additional 5v power? 
 *     Caused by trying to call the knobA.read() value too quickly. Added a 5ms delay and works great
 *   D4, GPIO2 also turns on LED when connected to encoder  
 *   Error while trying to run ((knobA.read() <= -1) || (millis()-time1>9000))
 *     Could be connection pins for knobA??
 *     Switching to knobB didn't help
 *     Removing knobA.read() didn't help
 *   Got same memory error when only encoders were plugged in (D1-D4 and grounds)
 *     Switched
 *   Board wouldn't start from memory unless D4 was an output and not an input.
 *    Switched motor output for encoder input
*/
#include <Encoder.h>

/*
 NodeMCU V1.0 
 Board pin names to IOs
 D0 - GPIO16
 D1 - GPIO5 
 D2 - GPIO4
 D3 - GPIO0
 D4 - GPIO2 (PWM)
 3V3
 GND
 D5 - GPIO14 (PWM)
 D6 - GPIO12 (PWM)
 D7 - GPIO13 
 D8 - GPIO15 (PWM)
 RX - GPIO3
 TX - GPIO1
 GND
 3V3
 A0 - ADC0
 SD3 - GPIO9
 SD2 - GPIO10
*/

 
// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knobA(4, 5);      // D1, D2
Encoder knobB(12, 14);    // D5, D6   (on NodeMCU v1, D3,D4,D5,D6 work for encoder (but D4 won't allow for booting when plugged in))
//   avoid using pins with LEDs attached

// DC motor settings
const int MOT_A1_PIN = 0;  // D3
const int MOT_A2_PIN = 2;  // D4
const int MOT_B1_PIN = 13; // D7
const int MOT_B2_PIN = 15; // D8

// defines pins numbers for ultrasonic sensor
const int UltrasonicTrigPin = 3; //RX   16(worked); //D0                                      3(worked even plugged in loading);  //RX  14(worked); //D5     // 9(not working);    //SD3   3(not working);    // RX
const int UltrasonicEchoPin = 16; //D0  3(worked but can't be plugged in while loading); //RX 16(worked, wLEDon); //D0                  12(worked); //D6     // 10(not working);   //SD2   16(not working);   // D0       
// defines variables for ultrasonic sensor
long duration1;
int distance1;

// Timer
unsigned long time1;
unsigned long time2;
unsigned long time3;

// Extra items
int encoder_down;
int doorA_position;
int doorB_position;
int doorA_previous_position;
int doorB_previous_position;
int door_timer;
int initialize_counter;

void setup()
{
  Serial.begin(115200); // Starts the serial communication

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
  pinMode(UltrasonicEchoPin, INPUT);  // Sets the echoPin as an Input

  // Initialize digital pin LED_BUILTIN as an output.  
  //pinMode(LED_BUILTIN, OUTPUT);
  
  //(FIX THIS TO AVOID A POWER OUTAGE TEARING MOTOR APART)
  doorA_position = 0;     // 1=up, 0=down
  doorB_position = 0;     // 1=up, 0=down  
  
  // extra initialize
  doorA_previous_position = abs(doorA_position-1);  // Tracks if the door position has changed and Turn motor off (must be different than initial door position)
  doorB_previous_position = abs(doorB_position-1);  // Tracks if the door position has changed and Turn motor off (must be different than initial door position)
  encoder_down = 800;                               // Encoder reading when door is down (Previous test value = 1085)
  door_timer = 10000;                               // Time to wait between door changes (ms) 
}


// Encoder setting
long positionA  = -999;
long positionB = -999;


void loop()
{
  //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off (for some reason LOW doesn't work)

  // Set A and B encoders
  long newA, newB;
  newA = knobA.read();
  newB = knobB.read();
  if (newA != positionA || newB != positionB) {
    Serial.print("A = ");
    Serial.print(newA);
    Serial.print(", B = ");
    Serial.print(newB);
    Serial.println();
    positionA = newA;
    positionB = newB;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    knobA.write(0);
    knobB.write(0);
  }

  // Set timer after door has been triggered
  if (doorA_previous_position!=doorA_position) {
    time2 = millis();   
    doorA_previous_position=doorA_position;     // Denotes that countdown has started for next door to move
  } 
  // Set timer after door has been triggered
  if (doorB_previous_position!=doorB_position) {
    time3 = millis();   
    doorB_previous_position=doorB_position;     // Denotes that countdown has started for next door to move
  } 
  
  // Check Ultrasonic sensor
  // Clears the UltrasonicTrigPin wait 5 micro seconds
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


  // If time has been reached to open door A
  if ((millis()-time2)>=door_timer) {
    Serial.println("Time reached"); 
    time1 = millis(); 
    
    // Door Controls
    if (doorA_position == 0) {        // If its down then take it up
      Serial.println("Motor up");
      digitalWrite(MOT_A1_PIN, HIGH);      
      digitalWrite(MOT_A2_PIN, LOW);
      while (doorA_position == 0) {
        delay(2);
        if ((knobA.read() <= -encoder_down)) {    // Removed for testing    || (millis()-time2)>=door_timer+9000
          doorA_position = 1;             // Change to show new door position          
        }
      }
      Serial.println("Motor Off");    // Turns off motor if the door was stopped manually or automatically
      digitalWrite(MOT_A1_PIN, LOW);
      digitalWrite(MOT_A2_PIN, LOW); 
      Serial.println(millis()-time1);
    }    
    else if (doorA_position == 1) {        // If its up then take it down
      Serial.println("Motor down");
      digitalWrite(MOT_A1_PIN, LOW);      
      digitalWrite(MOT_A2_PIN, HIGH);
      while (doorA_position == 1) {
        delay(2);
        if ((knobA.read() >= 0)) {   // Removed for testing  || (millis()-time2)>=door_timer+4000
          doorA_position = 0;             // Change to show new door position
        }
      }
      Serial.println("Motor Off");    // Turns off motor if the door was stopped manually or automatically
      digitalWrite(MOT_A1_PIN, LOW);
      digitalWrite(MOT_A2_PIN, LOW); 
      Serial.println(millis()-time1);
    }          
  }
  // If time has been reached to open door B
  if ((millis()-time3)>=door_timer) {
    Serial.println("Time reached"); 
    time1 = millis(); 
    
    // Door Controls
    if (doorB_position == 0) {        // If its down then take it up
      Serial.println("Motor up");
      digitalWrite(MOT_B1_PIN, HIGH);      
      digitalWrite(MOT_B2_PIN, LOW);
      while (doorB_position == 0) {
        delay(2);
        if ((knobB.read() <= -encoder_down)) {      //  Removed for testing     || (millis()-time3)>=door_timer+9000
          doorB_position = 1;             // Change to show new door position          
        }
      }
      Serial.println("Motor Off");    // Turns off motor if the door was stopped manually or automatically
      digitalWrite(MOT_B1_PIN, LOW);
      digitalWrite(MOT_B2_PIN, LOW); 
      Serial.println(millis()-time1);
    }    
    else if (doorB_position == 1) {        // If its up then take it down
      Serial.println("Motor down");
      digitalWrite(MOT_B1_PIN, LOW);      
      digitalWrite(MOT_B2_PIN, HIGH);
      while (doorB_position == 1) {
        delay(2);
        if ((knobB.read() >= 0)) {     // Removed for testing  || (millis()-time3)>=door_timer+4000
          doorB_position = 0;             // Change to show new door position
        }
      }
      Serial.println("Motor Off");    // Turns off motor if the door was stopped manually or automatically
      digitalWrite(MOT_B1_PIN, LOW);
      digitalWrite(MOT_B2_PIN, LOW); 
      Serial.println(millis()-time1);
    }          
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
