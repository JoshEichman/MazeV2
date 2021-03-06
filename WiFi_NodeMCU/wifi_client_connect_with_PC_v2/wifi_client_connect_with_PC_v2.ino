/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 *  GPIO Pins
 *  D0    16
 *  D1    5
 *  D2    4
 *  D3    0
 *  D4    2
 *  D5    14
 *  D6    12
 *  D7    13
 *  D8    15
 *  D9    3
 *  D10   1
 *  SD3?  10  Posibly as digital input
 *  SD2?  9   Posibly as digital input
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Encoder.h>


char ssid[] = "Nuria's Hizzie 2";           // SSID of your home WiFi
char pass[] = "roundwind557";            // password of your home WiFi
IPAddress server(192,168,1,9);       // the fix IP address of the server
WiFiClient client;


byte ledPin = 16;

// defines pins numbers for ultrasonic sensor
const int  UltrasonicTrigPin = 12;    // D6
const int  UltrasonicEchoPin = 13;    // D7
// defines variables for ultrasonic sensor
long duration1;
byte distance1;

// Timer
unsigned long time1;

// DC motor settings
const int MOT_A1_PIN = 15;  // D8
const int MOT_A2_PIN = 14;  // D5
const int MOT_B1_PIN = 16;  // D0
const int MOT_B2_PIN = 3;   // D9

Encoder knobA(0,2);         // D3,D4
Encoder knobB(4,5);         // D1,D2

// Extra items
int counter1;

void setup() {
  Serial.begin(9600);               // only for debug

  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer    
  
  /*  
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  */
  pinMode(ledPin, OUTPUT);

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

  // Initialize
  counter1 = 0;
}

// Encoder setting
long positionA = -999;
long positionB = -999;


void loop () {
  // Capture time at the start
  if (counter1 == 0) {
    time1 = millis();
    counter1 = 1;
  }
  // Set encoders
  long newA, newB;
  newA = knobA.read();
  newB = knobB.read();
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
  
  //digitalWrite(ledPin, HIGH);
  
  // Ultrasonic sensor
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

  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance1);
  
  //digitalWrite(ledPin, LOW);
  
  client.connect(server, 21);   // Connection to the server
  client.print("Distance: ");
  client.println(distance1);
    
  //client.println("Hello!");
  //client.println("Hello server! Are you sleeping?\r");  // sends the message to the server
  //String answer = client.readStringUntil('\r');   // receives the answer from the sever
  //Serial.println("from server: " + answer);
  client.flush();
  //digitalWrite(ledPin, HIGH);
  //delay(2000);                  // client will trigger the communication after two seconds
  //digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
  //delay(500);                  
  
  // Makes sure that loops do not occur faster than 250 ms
  if (millis()-time1<250) {
    delay(250+time1-millis());
  }

  //if (millis()-time1 >= 10000) {
  if (knobB.read()>96) {
    counter1 = 0;    
    Serial.println("Run Motor");
    
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
    /*
    digitalWrite(MOT_C1_PIN, HIGH);      
    digitalWrite(MOT_C2_PIN, LOW);
    delay(500);
    digitalWrite(MOT_C1_PIN, LOW);      
    digitalWrite(MOT_C2_PIN, LOW);
    delay(10);
    */
    /*
    digitalWrite(MOT_D1_PIN, HIGH);      
    digitalWrite(MOT_D2_PIN, LOW);
    delay(500);
    digitalWrite(MOT_D1_PIN, LOW);      
    digitalWrite(MOT_D2_PIN, LOW);
    delay(10);    
    */
  }  

}
