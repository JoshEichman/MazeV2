/*
  Complete Maze Code
  1) connect to Wifi 
       (assign MAC address to specific IP in router settings)
       (Navigate to http://192.168.1.1/, go to Advanced > LAN Setup > click "Add" on the bottom) 
  2) connect to MySQL (connect to table1 on MAC)
  3) Wait for change to the door assigned to the IP address of this device then open/close

 *   NOTES: 
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

  
  MySQL Connector/Arduino Example : connect by wifi

  This example demonstrates how to connect to a MySQL server from an
  Arduino using an Arduino-compatible Wifi shield. Note that "compatible"
  means it must conform to the Ethernet class library or be a derivative
  thereof. See the documentation located in the /docs folder for more
  details.
  INSTRUCTIONS FOR USE
  1) Change the address of the server to the IP address of the MySQL server
  2) Change the user and password to a valid MySQL user and password
  3) Change the SSID and pass to match your WiFi network
  4) Connect a USB cable to your Arduino
  5) Select the correct board and port
  6) Compile and upload the sketch to your Arduino
  7) Once uploaded, open Serial Monitor (use 115200 speed) and observe

  If you do not see messages indicating you have a connection, refer to the
  manual for troubleshooting tips. The most common issues are the server is
  not accessible from the network or the user name and password is incorrect.

  Note: The MAC address can be anything so long as it is unique on your network.

  Created by: Dr. Charles A. Bell
*/
/*
#include <ESP8266WiFi.h>       //#include <WiFi.h>   // Use this for WiFi instead of Ethernet.h
#include <WiFiClient.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
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

/*
// For MAC
// Notes: fix laptop IP
// set each ESP IP from 100+
// If ESP can;t connect go into random. try again after X times/seconds
// Save time of ESP last access (add column)
// Add column for setpoint and actual
IPAddress server_addr(192,168,1,5);  // IP of the MySQL *server* here (Mac: 192,168,1,5)
// Must create database, then create user with full priveledges (including grant?) and leave the hostname as wildcard '%'

// Windows   Had to change the authentication to standard (ALTER USER 'user1'@'%' IDENTIFIED WITH mysql_native_password BY 'pass1'; FLUSH PRIVILEGES;)
// char user[] = "MazeDoor";           // MySQL user login username@hostname
// char password[] = "rooDezaM*2";     // MySQL user login password

// MAC
char user[] = "user1";           // MySQL user login username@hostname
char password[] = "password1";   // MySQL user login password

// Random number
long randNumber;

// Sample query
// Windows
//char INSERT_SQL[] = "INSERT INTO MazeV2.data1 (Cell,Distance,Door_set) VALUES (145, 1, 1)";   // Windows
//// char UPDATE_SQL[30];   //char UPDATE_SQL[] = "UPDATE MazeV2.data1 SET Distance=10 WHERE Cell=1";
//char UPDATE_SQL_TEXT1[] = "UPDATE MazeV2.data1 SET Distance=";
//char UPDATE_SQL_TEXT2[] = " WHERE Cell=1";
//char SELECT_SQL[] = "SELECT Distance FROM MazeV2.data1 WHERE Cell=1";
//char cstr[16];

//*
// MAC
char INSERT_SQL[] = "INSERT INTO MazeV2mac.table1 (Cell,distance,Door_set) VALUES (4, 1, 1)";   // MAC
char UPDATE_SQL_TEXT1[] = "UPDATE MazeV2mac.table1 SET Distance=";
char UPDATE_SQL_TEXT2[] = " WHERE Cell=1";
char SELECT_SQL[] = "SELECT Distance FROM MazeV2mac.table1 WHERE Cell=1";
char cstr[16];
*/


// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knobA(4, 5);      // D1, D2
Encoder knobB(12, 14);    // D5, D6   (on NodeMCU v1, D1,D2,D3,D4,D5,D6 work for encoders (but D4 won't allow for booting when plugged in))
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
int doorA_init_desired;
int doorB_init_desired;
int door_timer;
int encoder_timer;
int encoder_timer_counter;
int reset_door_frequency;
int reset_doorA_counter;
int reset_doorB_counter;
int initialize_counter;

/*
// WiFi card example
char ssid[] = "Nuria's Hizzie 2"; // SSID of your home WiFi
char pass[] = "roundwind557";     // password of your home WiFi

WiFiClient client;            // Use this for WiFi instead of EthernetClient
MySQL_Connection conn((Client *)&client);
*/
  
void setup()
{
  Serial.begin(115200); // Starts the serial communication
/*
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid, pass);
  // Begin WiFi section
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
*/
 
  /*
  int status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    while(true);
  }
  // print out info about the connection:
  else {
    Serial.println("Connected to network");
    IPAddress ip = WiFi.localIP();
    Serial.print("My IP address is: ");
    Serial.println(ip);
  }
  // End WiFi section
  */
/*
  delay(2000);
  Serial.print("Connecting...");
  while (conn.connect(server_addr, 3306, user, password) != true) {
     delay(2000);
     Serial.print ( "." );
  }
  Serial.println("");  
  // To close connection
  // conn.close();
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
  pinMode(UltrasonicEchoPin, INPUT);  // Sets the echoPin as an Input

  // Initialize digital pin LED_BUILTIN as an output.  
  //pinMode(LED_BUILTIN, OUTPUT);
  
  //(FIX THIS TO AVOID A POWER OUTAGE TEARING MOTOR APART)
  doorA_position = 1;     // 1=up, 0=down
  doorB_position = 0;     // 1=up, 0=down  
  doorA_init_desired = 1; // 1=up, 0=down
  doorB_init_desired = 0; // 1=up, 0=down
  
  // extra initialize
  doorA_previous_position = abs(doorA_position-1);  // Tracks if the door position has changed and Turn motor off (must be different than initial door position)
  doorB_previous_position = abs(doorB_position-1);  // Tracks if the door position has changed and Turn motor off (must be different than initial door position)
  encoder_down = 2580;                              // Encoder reading when door is down (Previous test value = 1085)
  door_timer = 20000;                               // Time to wait between door changes (ms) 
  encoder_timer = 100;                              // Check to see if the encoder is moving within this time (ms)
  encoder_timer_counter = 1;                        // Used in conjunction with encoder_timer to periodically check that the encoder is still moving
  reset_door_frequency = 3;                         // Reset door by running to max position and resetting encoder after this many door lifting events
  reset_doorA_counter = 1;                          // Used in conjunction with reset_door_frequency to reset door encoder
  reset_doorB_counter = 1;                          // Used in conjunction with reset_door_frequency to reset door encoder
  initialize_counter = 0;                           // Used to run code on the first iteration of loop()

} 


// Encoder setting
long positionA = -999;
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
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    knobA.write(0);
    knobB.write(0);
  } 
  
  if (initialize_counter==0) {
    if (doorA_position==0) { knobA.write(encoder_down); }
    if (doorB_position==0) { knobB.write(encoder_down); }
      // Set timer after door has been triggered
      
    delay(1000);
    // If time has been reached to open door A
    if (doorA_init_desired != doorA_position) {
      Serial.println("Set initial doorA position"); 
      time1 = millis(); 
      encoder_timer_counter = 1;
      newA = knobA.read(); 
      
      // Door Controls
      if (doorA_position == 0) {        // If its down then take it up
        Serial.println("Motor up");
        digitalWrite(MOT_A1_PIN, HIGH);      
        digitalWrite(MOT_A2_PIN, LOW);
        while (doorA_position == 0) {
          if (millis()-time1>=encoder_timer_counter*encoder_timer) {
            if (newA==knobA.read()) {
              doorA_position = 1;             // Change to show new door position before exiting loop 
              break;    // If the encoder value is identical from the last encoder_timer interval exit loop and stop motor
            }
            newA = knobA.read(); 
            encoder_timer_counter++;    // Increment encoder
          }        
          delay(2);
          if (knobA.read() <= -1) {
            if (reset_door_frequency != reset_doorA_counter) {   // If statement used to delay motor off every "reset_door_frequency" to reset encoders. Relies on the stall check above to turn off motors 
              doorA_position = 1;             // Change to show new door position  
            }
            if (millis()-time1>=16000) {      // Just in case the string has broken or something else that allows the pulley to continue spinning during an encoder reset, default time 
              Serial.println("DoorA Time out"); 
              doorA_position = 1;             // Change to show new door position  
            }
          }
        }
        if (reset_door_frequency <= reset_doorA_counter) {    // Because the break doesn't allow for reset_doorA_counter to increment an additional time on the final iteration this inequality is correct as written 
          reset_doorA_counter = 1;          // reset counter        
          Serial.println("DoorA encoder reset");
          knobA.write(0);
        } 
        reset_doorA_counter++;          // Increment counter       
  
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
          if (millis()-time1>=encoder_timer_counter*encoder_timer) {
            if (newA==knobA.read()) {
              doorA_position = 0;             // Change to show new door position before exiting loop
              break;    // If the encoder value is identical from the last encoder_timer interval exit loop and stop motor
            }
            newA = knobA.read(); 
            encoder_timer_counter++;    // Increment encoder
          }        
          delay(2);
          if ((knobA.read() >= encoder_down || (millis()-time1)>=16000)) { 
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
    if ((doorB_init_desired != doorB_position)) {
      Serial.println("Set initial doorB position"); 
      time1 = millis(); 
      encoder_timer_counter = 1;
      newB = knobB.read(); 
      
      // Door Controls
      if (doorB_position == 0) {        // If its down then take it up
        Serial.println("Motor up");
        digitalWrite(MOT_B1_PIN, HIGH);      
        digitalWrite(MOT_B2_PIN, LOW);
        while (doorB_position == 0) {
          if (millis()-time1>=encoder_timer_counter*encoder_timer) {
            if (newB==knobB.read()) {
              Serial.println("DoorB jam");
              doorB_position = 1;             // Change to show new door position before exiting loop
              break;    // If the encoder value is identical from the last encoder_timer interval exit loop and stop motor
            }
            Serial.print("EncoderBprevious=");
            Serial.print(newB);
            Serial.print(" EncoderBnew=");
            Serial.println(knobB.read());
            newB = knobB.read(); 
            encoder_timer_counter++;    // Increment encoder
          }        
          delay(2);
          if (knobB.read() <= -1) {      
            if (reset_door_frequency != reset_doorB_counter) {   // If statement used to delay motor off every "reset_door_frequency" to reset encoders. Relies on the stall check above to turn off motors
              Serial.println("DoorB normal exit");
              doorB_position = 1;             // Change to show new door position  
            }
            if (millis()-time1>=16000) {      // Just in case the string has broken or something else that allows the pulley to continue spinning during an encoder reset, default time 
              Serial.println("DoorB Time out"); 
              doorB_position = 1;             // Change to show new door position  
            }
          }
        }
        if (reset_door_frequency <= reset_doorB_counter) {    // Because the break doesn't allow for reset_doorB_counter to increment an additional time on the final iteration this inequality is correct as written 
          reset_doorB_counter = 0;      // reset counter        
          Serial.println("DoorB encoder reset");
          knobB.write(0);
        }
        reset_doorB_counter++;          // Increment counter
  
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
          if (millis()-time1>=encoder_timer_counter*encoder_timer) {
            if (newB==knobB.read()) {
              doorB_position = 0;             // Change to show new door position before exiting loop
              break;    // If the encoder value is identical from the last encoder_timer interval exit loop and stop motor
            }
            newB = knobB.read(); 
            encoder_timer_counter++;    // Increment encoder
          }        
          delay(2);
          if ((knobB.read() >= encoder_down) || (millis()-time1)>=10000) { 
            doorB_position = 0;             // Change to show new door position
          }
        }
        Serial.println("Motor Off");    // Turns off motor if the door was stopped manually or automatically
        digitalWrite(MOT_B1_PIN, LOW);
        digitalWrite(MOT_B2_PIN, LOW); 
        Serial.println(millis()-time1);
      }          
    }
    doorA_previous_position = abs(doorA_position-1);  // Tracks if the door position has changed and Turn motor off (must be different than initial door position)
    doorB_previous_position = abs(doorB_position-1);  // Tracks if the door position has changed and Turn motor off (must be different than initial door position)
    initialize_counter=1;
  }
  
/*
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
*/

/*
 // Initiate the query class instance
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  // Execute the query   
  cur_mem->execute(SELECT_SQL);
  column_names *cols = cur_mem->get_columns();
  row_values *row = NULL;
  // Prints single row value (good reference for SELECT: https://github.com/ChuckBell/MySQL_Connector_Arduino/issues/19)
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      randNumber = atoi(row->values[0]);            
    }
  } while (row != NULL);  
  Serial.print("Random Number = ");
  Serial.println(randNumber);
  randNumber = round(random(0,100));   // Create new random number and load to SQL

  // Write update SQL string and apply
  char UPDATE_SQL[30];
  strcpy(UPDATE_SQL,UPDATE_SQL_TEXT1);
  strcat(UPDATE_SQL,itoa(randNumber, cstr, 10));  
  strcat(UPDATE_SQL,UPDATE_SQL_TEXT2); //Serial.println(UPDATE_SQL);
  cur_mem->execute(UPDATE_SQL);
  // Note: since there are no results, we do not need to read any data
  // Deleting the cursor also frees up memory used
  delete cur_mem; 
*/

  
  // Set timer after door has been triggered
  if (doorA_previous_position!=doorA_position) {
    time2 = millis();   
    doorA_previous_position=doorA_position;     // Denotes that countdown has started for next door to move
  }     
  // If time has been reached to open door A
  if ((millis()-time2)>=door_timer) {
    Serial.println("Time reached"); 
    time1 = millis(); 
    encoder_timer_counter = 1;
    newA = knobA.read(); 
    
    // Door Controls
    if (doorA_position == 0) {        // If its down then take it up
      Serial.println("Motor up");
      digitalWrite(MOT_A1_PIN, HIGH);      
      digitalWrite(MOT_A2_PIN, LOW);
      while (doorA_position == 0) {
        if (millis()-time1>=encoder_timer_counter*encoder_timer) {
          if (newA==knobA.read()) {
            doorA_position = 1;             // Change to show new door position before exiting loop 
            break;    // If the encoder value is identical from the last encoder_timer interval exit loop and stop motor
          }
          newA = knobA.read(); 
          encoder_timer_counter++;    // Increment encoder
        }        
        delay(2);
        if (knobA.read() <= -1) {
          if (reset_door_frequency != reset_doorA_counter) {   // If statement used to delay motor off every "reset_door_frequency" to reset encoders. Relies on the stall check above to turn off motors 
            doorA_position = 1;             // Change to show new door position  
          }
          if (millis()-time1>=16000) {      // Just in case the string has broken or something else that allows the pulley to continue spinning during an encoder reset, default time 
            Serial.println("DoorA Time out"); 
            doorA_position = 1;             // Change to show new door position  
          }
        }
      }
      if (reset_door_frequency <= reset_doorA_counter) {    // Because the break doesn't allow for reset_doorA_counter to increment an additional time on the final iteration this inequality is correct as written 
        reset_doorA_counter = 1;          // reset counter        
        Serial.println("DoorA encoder reset");
        knobA.write(0);
      } 
      reset_doorA_counter++;          // Increment counter       

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
        if (millis()-time1>=encoder_timer_counter*encoder_timer) {
          if (newA==knobA.read()) {
            doorA_position = 0;             // Change to show new door position before exiting loop
            break;    // If the encoder value is identical from the last encoder_timer interval exit loop and stop motor
          }
          newA = knobA.read(); 
          encoder_timer_counter++;    // Increment encoder
        }        
        delay(2);
        if ((knobA.read() >= encoder_down || (millis()-time1)>=16000)) { 
          doorA_position = 0;             // Change to show new door position
        }
      }
      Serial.println("Motor Off");    // Turns off motor if the door was stopped manually or automatically
      digitalWrite(MOT_A1_PIN, LOW);
      digitalWrite(MOT_A2_PIN, LOW); 
      Serial.println(millis()-time1);
    }         
  }


  // Set timer after door has been triggered
  if (doorB_previous_position!=doorB_position) {
    time3 = millis();   
    doorB_previous_position=doorB_position;     // Denotes that countdown has started for next door to move
  } 
  // If time has been reached to open door B
  if ((millis()-time3)>=door_timer) {
    Serial.println("Time reached"); 
    time1 = millis(); 
    encoder_timer_counter = 1;
    newB = knobB.read(); 
    
    // Door Controls
    if (doorB_position == 0) {        // If its down then take it up
      Serial.println("Motor up");
      digitalWrite(MOT_B1_PIN, HIGH);      
      digitalWrite(MOT_B2_PIN, LOW);
      while (doorB_position == 0) {
        if (millis()-time1>=encoder_timer_counter*encoder_timer) {
          if (newB==knobB.read()) {
            Serial.println("DoorB jam");
            doorB_position = 1;             // Change to show new door position before exiting loop
            break;    // If the encoder value is identical from the last encoder_timer interval exit loop and stop motor
          }
          Serial.print("EncoderBprevious=");
          Serial.print(newB);
          Serial.print(" EncoderBnew=");
          Serial.println(knobB.read());
          newB = knobB.read(); 
          encoder_timer_counter++;    // Increment encoder
        }        
        delay(2);
        if (knobB.read() <= -1) {      
          if (reset_door_frequency != reset_doorB_counter) {   // If statement used to delay motor off every "reset_door_frequency" to reset encoders. Relies on the stall check above to turn off motors
            Serial.println("DoorB normal exit");
            doorB_position = 1;             // Change to show new door position  
          }
          if (millis()-time1>=16000) {      // Just in case the string has broken or something else that allows the pulley to continue spinning during an encoder reset, default time 
            Serial.println("DoorB Time out"); 
            doorB_position = 1;             // Change to show new door position  
          }
        }
      }
      if (reset_door_frequency <= reset_doorB_counter) {    // Because the break doesn't allow for reset_doorB_counter to increment an additional time on the final iteration this inequality is correct as written 
        reset_doorB_counter = 0;      // reset counter        
        Serial.println("DoorB encoder reset");
        knobB.write(0);
      }
      reset_doorB_counter++;          // Increment counter

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
        if (millis()-time1>=encoder_timer_counter*encoder_timer) {
          if (newB==knobB.read()) {
            doorB_position = 0;             // Change to show new door position before exiting loop
            break;    // If the encoder value is identical from the last encoder_timer interval exit loop and stop motor
          }
          newB = knobB.read(); 
          encoder_timer_counter++;    // Increment encoder
        }        
        delay(2);
        if ((knobB.read() >= encoder_down) || (millis()-time1)>=10000) { 
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
