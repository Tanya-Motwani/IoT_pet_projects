

/*
 * 05/02/2019 - 9:00
 * IOT Lab 4 - MQTT Publish and Subscribe
 * Author - Qasim Burhan Bhatia 16BCE059
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = "Q's iPhone";
const char* password = "misaqmisaq";
const char* mqtt_server = "m16.cloudmqtt.com";
const char* user = "omykikcj";
const char* pass = "QWbTfh9RvHYr";
const int port = 16677;
WiFiClient Wificlient;
PubSubClient client(Wificlient);


DHT dht(2, DHT11);
int DHTSignal = 2;

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (client.connect("ESP8266",user,pass)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("16bce059/iot", "");
      // ... and resubscribe
      client.subscribe("16bce059/iot/data");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  char msg[40];
  snprintf (msg, 40, "Temperature: %f\nHumidity: %f",dht.readTemperature(),dht.readHumidity());
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("16bce059/iot", msg);

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(DHTSignal,INPUT);
  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected! IP is - ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, port);
  client.setCallback(callback);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
