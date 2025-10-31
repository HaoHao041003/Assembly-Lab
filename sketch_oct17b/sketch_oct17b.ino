#include <DHT.h>
#define DHTPIN 4 // digital pin connected to sensor
#define DHTTYPE DHT11 // define DHT type

DHT dht(DHTPIN, DHTTYPE); // initialize DHT sensor

void setup() {
  Serial.begin(115200);
  Serial.println("DHT11 test !!");
  dht.begin();
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
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
}