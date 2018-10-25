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
*/
#include <Encoder.h>

/*
 NodeMCU V1.0 
 Board pin names to IOs
 D0 - GPIO16
 D1 - GPIO5 
 D2 - GPIO4 (PWM)
 D3 - GPIO0
 D4 - GPIO2
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
Encoder knobA(4, 5);      // checking on D1,D3 to see if it works     // D1, D2
Encoder knobB(14, 12);    // D5, D6   (on NodeMCU v1, D3,D4,D5,D6 work for encoder (but D4 won't allow for booting when plugged in))
//   avoid using pins with LEDs attached

/* 
Testing interrupt pins
WORKED

DIDN"T WORK
Using 0,2,15 as Inputs is tricky (https://www.forward.com.au/pfod/ESP8266/GPIOpins/index.html)

    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
0      N  Y*    1* 1*                                      
1         N  N                                        
2            N                                        
3                                          1*           
4                  Y                                  
5                                             N       
6                                                     
7                                                     
8                                                     
9                                                     
10                                                     
11                                                     
12                                         Y  Y  N  N*      
13                                                     
14                                                     
15                                                     
16                                                     

* 0,2 Works but has been tempermental (particularly at startup, doesn't allow normal non-USB connected boot)
* 12,16 turns on LED based on input values
* Using TX causes text errors 
* 1* triggers but doesn't change value (stays -1 or 0, or 1 and 2 )
*/

void setup()
{
  Serial.begin(115200); // Starts the serial communication  
}

// Encoder setting
long positionA  = -999;
long positionB = -999;


void loop()
{
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
  /*
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    knobA.write(0);
    knobB.write(0);
  }
  */
}
