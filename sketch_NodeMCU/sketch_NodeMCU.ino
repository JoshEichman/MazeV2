#include <ESP8266WiFi.h>

int LedPin = 16;

void setup() {
  Serial.begin(115200);
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(2000);
  Serial.println("Setup done");

  pinMode(LedPin, OUTPUT);
}

void loop() {
  Serial.println("scan start");

  int n = WiFi.scanNetworks();// WiFi.scanNetworks will return the number of networks found
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);

  digitalWrite(LedPin, HIGH);       // turn the LED off by making the voltage LOW
  delay(500);                       // wait 
  digitalWrite(LedPin, LOW);        // turn the LED off by making the voltage LOW
  delay(500);                       // wait 
  digitalWrite(LedPin, HIGH);       // turn the LED off by making the voltage LOW
  delay(500);                       // wait 
  digitalWrite(LedPin, LOW);        // turn the LED off by making the voltage LOW

}
