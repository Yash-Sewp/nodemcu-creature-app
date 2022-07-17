#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 0      //GPIO-0 D3 pin of nodemcu
/* Set these to your desired credentials. */
const char *ssid = "Naidoo_TN"; //Enter your WIFI ssid
const char *password = "dkrby87233!!"; //Enter your WIFI password

int bulbOne = 5; // GPIO5
int bulbTwo = 4; // GPIO4
int RelayTwoHeater = 16; // GPIO16
int RelayTwoMister = 2; // GPIO2
String id = "62cb0e6b6aee077248228a26";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;
DHT dht(dht_dpin, DHTTYPE);
ESP8266WebServer server(80);

// String backendUrl = "http://creature-keeper.herokuapp.com";
String backendUrl = "http://192.168.3.8:3000";

void setup() {
  pinMode(bulbOne, OUTPUT);
  pinMode(bulbTwo, OUTPUT);
  pinMode(RelayTwoHeater, OUTPUT);
  pinMode(RelayTwoMister, OUTPUT);
  delay(3000);
  dht.begin();
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop() {
  server.handleClient();
  getLightOneStatus();
  getLightTwoStatus();
  delay(5000);
  getHeaterPowerStatus();
  getMisterPowerStatus();
  postTempHumid();
  delay(1000);
}

String postTempHumid() {
  String postData = "";
  String petTankTemp = backendUrl + "/pet-tank-temp";
  WiFiClient client;
  HTTPClient http;

  float temp = 0.0;
  float humidity = 0.0; 
  humidity = dht.readHumidity();    
  temp = dht.readTemperature(); 

   delay(5000);

  http.begin(client, petTankTemp);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  postData = "id=" + id + "&temp=" + temp + "&humidity=" + humidity;

  int httpCode = http.POST(postData);  

  String payload = http.getString();    

  Serial.println(httpCode);

  http.end();

  return payload;

}

String getLightOneStatus() {
  String postData = "";
  String toggleBulbOne = backendUrl + "/bulb-one-power-status";
  WiFiClient client;
  HTTPClient http;

  http.begin(client, toggleBulbOne);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  postData = "id=" + id;

  int httpCode = http.POST(postData); 

  String payload = http.getString();

  Serial.println(httpCode);   
  Serial.println("Payload below");
  Serial.println(payload);
  if (httpCode > 0) {
    
    payload = http.getString();
    
    Serial.println(payload);
    if (payload == "true") {
      Serial.println("Should come on");
      digitalWrite(bulbOne, LOW);
    } else if (payload == "false") {
      Serial.println("Should go off");
      digitalWrite(bulbOne, HIGH);
    }
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpCode);
  }
  // Free resources
  http.end();

  return payload;
}

String getLightTwoStatus() {
  String postData = "";
  String toggleBulbTwo = backendUrl + "/bulb-two-power-status";
  WiFiClient client;
  HTTPClient http;

  http.begin(client, toggleBulbTwo);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  postData = "id=" + id;

  int httpCode = http.POST(postData); 

  String payload = http.getString();

  Serial.println(httpCode);   
  Serial.println("Payload below");
  Serial.println(payload);
  if (httpCode > 0) {
    
    payload = http.getString();
    
    Serial.println(payload);
    if (payload == "true") {
      Serial.println("Bulb two Should come on");
      digitalWrite(bulbTwo, LOW);
    } else if (payload == "false") {
      Serial.println("Bulb two Should go off");
      digitalWrite(bulbTwo, HIGH);
    }
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpCode);
  }
  // Free resources
  http.end();

  return payload;
}

String getHeaterPowerStatus() {
  String postData = "";
  String toggleHeaterStatus = backendUrl + "/heater-pad-power-status";
  WiFiClient client;
  HTTPClient http;

  http.begin(client, toggleHeaterStatus);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  postData = "id=" + id;

  int httpCode = http.POST(postData); 

  String payload = http.getString();

  Serial.println(httpCode);   
  Serial.println("Payload below");
  Serial.println(payload);
  if (httpCode > 0) {
    
    payload = http.getString();
    
    Serial.println(payload);
    if (payload == "true") {
      Serial.println("Heater should come on");
      digitalWrite(RelayTwoHeater, LOW);
    } else if (payload == "false") {
      Serial.println("Heater should go off");
      digitalWrite(RelayTwoHeater, HIGH);
    }
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpCode);
  }
  // Free resources
  http.end();

  return payload;
}

String getMisterPowerStatus() {
  String postData = "";
  String toggleMisterStatus = backendUrl + "/mister-power-status";
  WiFiClient client;
  HTTPClient http;

  http.begin(client, toggleMisterStatus);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  postData = "id=" + id;

  int httpCode = http.POST(postData); 

  String payload = http.getString();

  Serial.println(httpCode);   
  Serial.println("Payload below");
  Serial.println(payload);
  if (httpCode > 0) {
    
    payload = http.getString();
    
    Serial.println(payload);
    if (payload == "true") {
      Serial.println("Mister should come on");
      digitalWrite(RelayTwoMister, LOW);
    } else if (payload == "false") {
      Serial.println("Mister should go off");
      digitalWrite(RelayTwoMister, HIGH);
    }
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpCode);
  }
  // Free resources
  http.end();

  return payload;
}
