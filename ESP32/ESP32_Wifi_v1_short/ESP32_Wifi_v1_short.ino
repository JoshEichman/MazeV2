/*  ESP32 Testing
 *   
 *   Need to instal VCP drivers for USB to UART
 */

#include <WiFi.h>
//#include <Encoder.h>

// defines pins numbers for ultrasonic sensor
const int  UltrasonicTrigPin = 34;  
const int  UltrasonicEchoPin = 23;  
const int  UltrasonicTrigPin2 = 35; 
const int  UltrasonicEchoPin2 = 22; 
// defines variables for ultrasonic sensor
long duration1;
byte distance1;
long duration2;
byte distance2;

// DC motor settings
const int MOT_A1_PIN = 32;
const int MOT_A2_PIN = 33;
const int MOT_B1_PIN = 25;
const int MOT_B2_PIN = 26;
const int MOT_C1_PIN = 27;
const int MOT_C2_PIN = 14;
const int MOT_D1_PIN = 12;
const int MOT_D2_PIN = 13;

/*
// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knobA(21,19);
Encoder knobB(18, 5);
Encoder knobC(17,16);
Encoder knobD( 4,15);
//   avoid using pins with LEDs attached
*/

char ssid[] = "Nuria's Hizzie 2";           // SSID of your home WiFi
char pass[] = "roundwind557";            // password of your home WiFi
IPAddress server(192,168,1,9);       // the fix IP address of the server
WiFiClient client;

// Timer
unsigned long time1;
  
// Extra items
int counter1;



void setup() {
  Serial.begin(115200);               // only for debug

  // Ultrasonic Sensor
  pinMode(UltrasonicTrigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(UltrasonicEchoPin, INPUT); // Sets the echoPin as an Input
  pinMode(UltrasonicTrigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(UltrasonicEchoPin2, INPUT); // Sets the echoPin as an Input

  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP32 to the computer   

  // Initialize
  counter1 = 0;
}

/*
// Encoder setting
long positionA = -999;
long positionB = -999;
long positionC = -999;
long positionD = -999;
*/

void loop () {

  if (counter1 == 0) {
    time1 = millis();
    counter1 = 1;
  }
  
  /*
  // Set left and right encoders
  long newA, newB, newC, newD;
  newA = knobA.read();
  newB = knobB.read();
  newC = knobC.read();
  newD = knobD.read();      
  if (newA != positionA) {
    Serial.print("A = ");
    Serial.println(newA);
    positionA = newA;
  }
  if (newB != positionB) {
    Serial.print("B = ");
    Serial.println(newB);
    positionB = newB;
  }
  if (newC != positionC) {
    Serial.print("C = ");
    Serial.println(newC);
    positionC = newC;
  }
  if (newD != positionD) {
    Serial.print("D = ");
    Serial.println(newD);
    positionD = newD;
  }
  */
  
  // Ultrasonic sensor
  // ToDo: Keep track of every value and find average/median/stdev then check for change

  digitalWrite(UltrasonicTrigPin2, LOW);    
  delay(2);
  digitalWrite(UltrasonicTrigPin2, HIGH); 
  delay(10); 
  digitalWrite(UltrasonicTrigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(UltrasonicEchoPin2, HIGH);
 
  /*
  delayMicroseconds(2);
  digitalWrite(UltrasonicTrigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(UltrasonicTrigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(UltrasonicEchoPin2, HIGH);
  */
  
  delay(50);
  
  // Clears the UltrasonicTrigPin wait 2 micro seconds
  digitalWrite(UltrasonicTrigPin, LOW);  
  delay(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(UltrasonicTrigPin, HIGH); 
  delay(10); 
  digitalWrite(UltrasonicTrigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(UltrasonicEchoPin, HIGH);
 
  
  // Calculating the distance 
  distance1 = duration1 * 0.034 / 2 / 2.54;     // value in inches
  distance2 = duration2 * 0.034 / 2 / 2.54;     // value in inches

  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance1: ");
  Serial.print(distance1);
  Serial.print("   Distance2: ");
  Serial.println(distance2);

 
  client.connect(server, 21);   // Connection to the server
  client.print("Distance: ");
  client.print(distance1);
  client.print(" and ");
  client.println(distance2);
  //client.println("Hello!");
  //client.println("Hello server! Are you sleeping?\r");  // sends the message to the server
  //String answer = client.readStringUntil('\r');   // receives the answer from the sever
  //Serial.println("from server: " + answer);
  client.flush();
  
  delay(50);

  if (millis()-time1 >= 2000) {
    counter1 = 0;    
    digitalWrite(MOT_A1_PIN, HIGH);      
    digitalWrite(MOT_A2_PIN, LOW);
    delay(500);
    digitalWrite(MOT_A1_PIN, LOW);      
    digitalWrite(MOT_A2_PIN, LOW);
    delay(10);
    digitalWrite(MOT_B1_PIN, HIGH);      
    digitalWrite(MOT_B2_PIN, LOW);
    delay(500);
    digitalWrite(MOT_B1_PIN, LOW);      
    digitalWrite(MOT_B2_PIN, LOW);
    delay(10);
    digitalWrite(MOT_C1_PIN, HIGH);      
    digitalWrite(MOT_C2_PIN, LOW);
    delay(500);
    digitalWrite(MOT_C1_PIN, LOW);      
    digitalWrite(MOT_C2_PIN, LOW);
    delay(10);
    digitalWrite(MOT_D1_PIN, HIGH);      
    digitalWrite(MOT_D2_PIN, LOW);
    delay(500);
    digitalWrite(MOT_D1_PIN, LOW);      
    digitalWrite(MOT_D2_PIN, LOW);
    delay(10);
  }

  
}
