/*  ESP32 Testing
 */

// defines pins numbers for ultrasonic sensor
const int  UltrasonicTrigPin = 19;
const int  UltrasonicEchoPin = 18;
const int  UltrasonicTrigPin2 = 23;
const int  UltrasonicEchoPin2 = 22;
// defines variables for ultrasonic sensor
long duration1;
byte distance1;
long duration2;
byte distance2;

void setup() {
  Serial.begin(115200);               // only for debug

  // Ultrasonic Sensor
  pinMode(UltrasonicTrigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(UltrasonicEchoPin, INPUT); // Sets the echoPin as an Input
  pinMode(UltrasonicTrigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(UltrasonicEchoPin2, INPUT); // Sets the echoPin as an Input
}


void loop () {
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

  delay(50);
}
