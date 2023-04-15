#include <dht.h> // Include DHT library
#define DHTPIN A1 // DHT11 data pin
dht DHT; // Initialize DHT sensor
int relay1 = 3; // Relay 1 control pin
int relay2 = 4; // Relay 2 control pin
int relay3 = 5; // Relay 3 control pin

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(relay1, OUTPUT); // Set relay 1 pin as output
  pinMode(relay2, OUTPUT); // Set relay 2 pin as output
  pinMode(relay3, OUTPUT); // Set relay 3 pin as output
//  dht.begin(); // Initialize DHT sensor
}

void loop() {
  float temperature = DHT.temperature; // Read temperature value
  float humidity = DHT.humidity; // Read humidity value

  // If temperature is greater than 21°C, turn on relay 1
  if (temperature > 30) {
    digitalWrite(relay1, LOW); // Turn on relay 1
  } else {
    digitalWrite(relay1, HIGH); // Turn off relay 1
  }

  // If humidity is greater than 45%, turn on relay 2
  if (humidity > 75) {
    digitalWrite(relay2, LOW); // Turn on relay 2
  } else {
    digitalWrite(relay2, HIGH); // Turn off relay 2
  }

  // If humidity is less than 40%, turn on relay 3
  if (humidity < 65) {
    digitalWrite(relay3, HIGH); // Turn on relay 3
  } else {
    digitalWrite(relay3, LOW); // Turn off relay 3
  }

  // Print temperature and humidity values
  DHT.read22(DHTPIN);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  delay(2000); // Wait for 2 seconds before next reading
}
