#include <ESP8266WiFi.h>
#include <coap_server.h>
#include <SimpleDHT.h>

SimpleDHT11 dht11;
#define DHT11_PIN D5

// CoAP server endpoint url callback
void callback_relay1(coapPacket &packet, IPAddress ip, int port, int obs);
coapServer coap;

//WiFi connection info
const char* ssid = "MotoG";
const char* password = "1234S678";

// LED STATE
bool LEDSTATE;

// CoAP server endpoint URL
void callback_relay1(coapPacket *packet, IPAddress ip, int port, int obs) {

  // send response
  char p[packet -> payloadlen + 1];
  memcpy(p, packet -> payload, packet -> payloadlen);
  p[packet->payloadlen] = NULL;
  String message(p);

  Serial.println("Callback Relay 1 Called");
  Serial.println(packet->payloadlen);
  Serial.println(p);
  Serial.println("IP address :"+ip);
  Serial.println("Port :"+port);
  Serial.println("Obs :"+obs);
  
  
  if (message.equals("0")) {
    digitalWrite(D1, LOW);
    //Serial.println("if loop");
  }
  else if (message.equals("1")) {
    digitalWrite(D1, HIGH);
    //Serial.println("else loop");
  }
 
  char *relay1 = (digitalRead(D1) > 0)? ((char *) "ON") :((char *) "OFF");
  
  //coap.sendResponse(packet, ip, port, light);
  if(obs == 1)
    coap.sendResponse(ip, port, relay1);
  else
    coap.sendResponse(ip, port, relay1);
}

void callback_dht11(coapPacket *packet, IPAddress ip, int port, int obs) {
  byte temperature;
  byte humidity;
  int chk = dht11.read(DHT11_PIN, &temperature, &humidity, NULL);
  String msg = "Real time temperature: ";
  msg = msg + temperature;
  msg = msg + " C; Real time Humidity: ";
  msg = msg + humidity;
  msg = msg + "%";
  char message[58];
  msg.toCharArray(message,58);
  coap.sendResponse(ip, port, message);
}


void setup() {
  yield();
  
  //serial begin
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("WiFi");
  
  // Connect to WiFi network
  Serial.println("");
  Serial.println("***************************************************");
  Serial.println("------------------- COAP DEMO by V.U.--------------");
  Serial.println("***************************************************");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    yield();
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
  // LED State
  pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH);
  LEDSTATE = true;
  
  // add server url endpoints.
  // can add multiple endpoint urls.
  coap.server(callback_relay1, "Relay1");
  coap.server(callback_dht11, "DHT11");
  coap.start(5683);
}

void loop() {
  coap.loop();
  delay(1000);
}
