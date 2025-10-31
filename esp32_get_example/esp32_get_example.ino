#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "Hinet";    // your network SSID
const char* password = "12345678";    // your network password

void setup() {
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    HTTPClient http;
    delay(10000); // wait for 10 seconds

    http.begin("https://api.thingspeak.com/channels/3121225/fields/1/last.json?api_key=FHXLAVUUQWXK7TJH"); //Specify the URL
    int httpResponseCode = http.GET();  //Make the request
  
    if (httpResponseCode > 0) { //Check for the returning code
  
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
      }
  
    else {
      Serial.println("Error on HTTP request");
    }
  
    http.end(); //Free the resources
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  
}
