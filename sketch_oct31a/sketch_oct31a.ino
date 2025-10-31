#include <WiFi.h>
#include <HTTPClient.h>

#define DIR_PIN 16
#define SPEED_PIN 4

const char* ssid = "Hinet";    // your network SSID
const char* password = "12345678";    // your network password
// Domain Name with full URL Path for HTTP POST Request
const char* serverName = "http://api.thingspeak.com/update";
// write API Key provided by thingspeak
String apiKey = "8JP35ZK5XJ6G5BID";    // your write API key

bool Power = false;
int Speed = 0;
bool is_clockwise = true;

void setup() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(SPEED_PIN, OUTPUT);

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
  digitalWrite(DIR_PIN, HIGH);
  analogWrite(SPEED_PIN, 200);
  post(Power, Speed, is_clockwise);
  Power = true;
  Speed = 200;
  is_clockwise = true;
  delay(2000);

  analogWrite(SPEED_PIN, 0);
  post(Power, Speed, is_clockwise);
  Power = false;
  Speed = 0;
  delay(1000);

  digitalWrite(DIR_PIN, LOW);
  analogWrite(SPEED_PIN, 150);
  post(Power, Speed, is_clockwise);
  Power = true;
  Speed = 150;
  is_clockwise = false;
  delay(2000);
  
  analogWrite(SPEED_PIN, 0);
  post(Power, Speed, is_clockwise);
  Power = false;
  Speed = 0;
  delay(1000);
}

void post(bool Power, int Speed, bool is_clockwise)
{
    if (WiFi.status() == WL_CONNECTED) {  //Check the current connection status
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Data to send with HTTP POST
    String httpRequestData = "api_key=" + apiKey + "&field1=" + String(Power) + "&field2=" + String(is_clockwise) + "&field3=" + String(Speed);
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
