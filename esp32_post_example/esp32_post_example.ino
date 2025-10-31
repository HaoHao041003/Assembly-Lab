#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Hinet";    // your network SSID
const char* password = "12345678";    // your network password
// Domain Name with full URL Path for HTTP POST Request
const char* serverName = "http://api.thingspeak.com/update";
// write API Key provided by thingspeak
String apiKey = "7FCUS7TUMKAKL5WE";    // your write API key
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {  //Check the current connection status
    WiFiClient client;
    HTTPClient http;
    delay(10000); // wait for 10 seconds
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    // Check if any reads failed
    if(isnan(h) || isnan(t)){
      Serial.println("Failed to read from DHT sensor");
      return;
    }

    Serial.print("Humidity : ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.println(" *C");

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Data to send with HTTP POST
    String httpRequestData = "api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h);
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) { //Check for the returning code
      
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }

    else{
      Serial.println("Error on HTTP request");
    }

    http.end();  //Free the resources
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}
