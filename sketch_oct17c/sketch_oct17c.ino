#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "DHT.h"

#define DHTPIN 4
#define LEDPIN 16
#define BUTTONPIN 17
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Hinet";    // your network SSID
const char* password = "12345678";    // your network password
// Domain Name with full URL Path for HTTP POST Request
const char* serverName = "http://api.thingspeak.com/update";
// write API Key provided by thingspeak
String apiKey = "7FCUS7TUMKAKL5WE";    // your write API key
int prev;
int buttonPrev;
int LEDState;
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

  pinMode(LEDPIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT);
  prev = millis();
  buttonPrev = millis();
  LEDState = true;
}


void loop() 
{
  //Check the current connection status
  if (WiFi.status() == WL_CONNECTED)
  {  
    WiFiClient client;
    HTTPClient http;

    int buttonState = digitalRead(BUTTONPIN);
    int buttonNow = millis();
    if(buttonState == LOW && buttonNow-buttonPrev>100)
    {
      buttonPrev = buttonNow;
      LEDState = !LEDState;
      http.begin("https://api.thingspeak.com/channels/3121225/feeds/last.json?api_key=FHXLAVUUQWXK7TJH"); //Specify the URL
      int httpResponseCode = http.GET();  //Make the request

      if(httpResponseCode > 0)
      { 
        //Check for the returning code
        String payload = http.getString();
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.println(payload);  // Print JSON response

        // Parse JSON response using Arduino_JSON library
        JSONVar jsonData = JSON.parse(payload);
        // Check if parsing was successful
        if (JSON.typeof(jsonData) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }
        // Extract the value of field1
        String field1Value = jsonData["field1"];
        Serial.print("Field1 value: ");
        Serial.println(field1Value);  // Print the value of field1

        String field2Value = jsonData["field2"];
        Serial.print("Field1 value: ");
        Serial.println(field2Value);  // Print the value of field1

        String field3Value = jsonData["field3"];
        Serial.print("Field1 value: ");
        Serial.println(field3Value);  // Print the value of field1
      }  
      else
      {
        Serial.println("Error on HTTP request");
      }
      delay(5000);
    }
    Serial.print("buttonState=");
    Serial.print(buttonState);
    Serial.print(" %\t");
    Serial.print("LEDState=");
    Serial.println(LEDState);

    //delay(1000); // wait for 10 seconds
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    // Check if any reads failed
    if(isnan(h) || isnan(t))
    {
      Serial.println("Failed to read from DHT sensor");
      return;
    }
    bool ALARM = false;

    int now = millis();
    if(now-prev>10000)
    {
      prev = now;
      Serial.print("Humidity : ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature : ");
      Serial.print(t);
      Serial.println(" *C");

      if((t>30||h<40)&&LEDState)
      {
        digitalWrite(LEDPIN, HIGH);
        ALARM = true;
      }
      else
      {
        digitalWrite(LEDPIN, LOW);
        ALARM = false;
      }
      Serial.print("Alarm : ");
      Serial.println(ALARM);

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h) + "&field3=" + String(ALARM);
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);

      if (httpResponseCode > 0)
      { 
        //Check for the returning code
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else
      {
        Serial.println("Error on HTTP request");
      }
    }
    
    http.end();  //Free the resources
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}
