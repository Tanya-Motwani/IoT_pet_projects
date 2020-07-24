/*
 ESP8266 --> ThingSpeak Channel
 
 This sketch sends the Wi-Fi Signal Strength (RSSI) of an ESP8266 to a ThingSpeak
 channel using the ThingSpeak API (https://www.mathworks.com/help/thingspeak).
 
 Requirements:
 
   * ESP8266 Wi-Fi Device
   * Arduino 1.8.8+ IDE
   * Additional Boards URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json
   * Library: esp8266 by ESP8266 Community
   * Library: ThingSpeak by MathWorks
 
 ThingSpeak Setup:
 
   * Sign Up for New User Account - https://thingspeak.com/users/sign_up
   * Create a new Channel by selecting Channels, My Channels, and then New Channel
   * Enable one field
   * Enter SECRET_CH_ID in "secrets.h"
   * Enter SECRET_WRITE_APIKEY in "secrets.h"

 Setup Wi-Fi:
  * Enter SECRET_SSID in "secrets.h"
  * Enter SECRET_PASS in "secrets.h"
  
 Tutorial: http://nothans.com/measure-wi-fi-signal-levels-with-the-esp8266-and-thingspeak
   
 Created: Feb 1, 2017 by Hans Scharler (http://nothans.com)
*/

#include "ThingSpeak.h"

unsigned long myChannelNumber = 695182;
const char * myWriteAPIKey = "N3FEVECCZR0DQREE";

#include <ESP8266WiFi.h>

#define TRIGGER 2
#define ECHO 0

char ssid[] = "QComputer";   // your network SSID (name)
char pass[] = "12345678";   // your network password
int keyIndex = 0;    // your network key index number (needed only for WEP)
long duration,distance;

WiFiClient  client;

void setup() {
  Serial.begin(9600);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  
  WiFi.mode(WIFI_STA);
   // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println("QComputer");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  ThingSpeak.begin(client);
}

void loop() {

  digitalWrite(TRIGGER,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER,LOW);
  duration = pulseIn(ECHO,HIGH);
  distance = duration * 0.034 / 2;
  
  // Write value to Field 1 of a ThingSpeak Channel
  int httpCode = ThingSpeak.writeField(myChannelNumber, 1,distance, myWriteAPIKey);

  if (httpCode == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }
  // Wait 20 seconds to update the channel again
  delay(20000);
}
