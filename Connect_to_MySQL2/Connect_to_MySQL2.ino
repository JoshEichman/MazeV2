/*
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
#include <ESP8266WiFi.h>       //#include <WiFi.h>   // Use this for WiFi instead of Ethernet.h
#include <WiFiClient.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

//WiFiServer server(80);
//IPAddress ip(192, 168, 1, 10);
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 255, 0);

//* 
// For MAC
IPAddress server_addr(192,168,1,9);  // IP of the MySQL *server* here (Mac: 192,168,1,5)
// Must create database, then create user with full priveledges (including grant?) and leave the hostname as wildcard '%'
// Windows   Had to change the authentication to standard (ALTER USER 'user1'@'%' IDENTIFIED WITH mysql_native_password BY 'pass1'; FLUSH PRIVILEGES;)
char user[] = "MazeDoor";           // MySQL user login username@hostname
char password[] = "rooDezaM*2";     // MySQL user login password
// MAC
// char user[] = "user1";           // MySQL user login username@hostname
// char password[] = "password1";   // MySQL user login password
//*/

// Random number
long randNumber;

/*
// For Windows (need to fix see error below)
// Error: 109 = Client does not support authentication protocol requested by server; consider upgrading MySQL client
IPAddress server_addr(192,168,1,9); // IP of the MySQL *server* here
// Must create database, then create a user with full priveledges (including grant?) and leave the hostname as wildcard '%'
char user[] = "MazeDoor";           // MySQL user login username@hostname
char password[] = "rooDezaM*2";     // MySQL user login password
*/

// Sample query
//char INSERT_SQL[] = "INSERT INTO MazeV2mac.table1 (Cell,distance,Door_set) VALUES (145, 1, 1)";   // MAC
char INSERT_SQL[] = "INSERT INTO MazeV2.data1 (Cell,Distance,Door_set) VALUES (145, 1, 1)";   // Windows
// char UPDATE_SQL[30];   //char UPDATE_SQL[] = "UPDATE MazeV2.data1 SET Distance=10 WHERE Cell=1";
char UPDATE_SQL_TEXT1[] = "UPDATE MazeV2.data1 SET Distance=";
char UPDATE_SQL_TEXT2[] = " WHERE Cell=1";
char SELECT_SQL[] = "SELECT Distance FROM MazeV2.data1 WHERE Cell=1";
char cstr[16];

// WiFi card example
char ssid[] = "Nuria's Hizzie 2"; // SSID of your home WiFi
char pass[] = "roundwind557";     // password of your home WiFi

WiFiClient client;            // Use this for WiFi instead of EthernetClient
MySQL_Connection conn((Client *)&client);
  
void setup() {
  Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network
  //ESP.wdtDisable();
  Serial.println();
  Serial.println();
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

  delay(2000);
  Serial.print("Connecting...");
  while (conn.connect(server_addr, 3306, user, password) != true) {
     delay(2000);
     Serial.print ( "." );
  }
  Serial.println("");
  
  // To close connection
  // conn.close();
}
  
void loop() {
  Serial.println("Recording data.");
   
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
  
  delay(round(random(1000,6000)));
}
