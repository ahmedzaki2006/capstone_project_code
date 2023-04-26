#include <dht.h> // Include DHT library
#include <LiquidCrystal.h> // Include LCD library

#define DHTPIN A1 // DHT11 data pin
dht DHT; // Initialize DHT sensor

int relay1 = 6; // Relay 1 control pin
int relay2 = 7; // Relay 2 control pin
int relay3 = 8; // Relay 3 control pin

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Initialize LCD monitor

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(relay1, OUTPUT); // Set relay 1 pin as output
  pinMode(relay2, OUTPUT); // Set relay 2 pin as output
  pinMode(relay3, OUTPUT); // Set relay 3 pin as output
  lcd.begin(16, 2); // Initialize LCD monitor
  //dht.begin(); // Initialize DHT sensor
}

void loop() {
  DHT.read22(DHTPIN); // Read temperature and humidity values
  float temperature = DHT.temperature; // Get temperature value
  float humidity = DHT.humidity; // Get humidity value

  // Control relay 1 based on temperature
  if (temperature > 21) {
    digitalWrite(relay1, HIGH); // Turn on relay 1
  } else {
    digitalWrite(relay1, LOW); // Turn off relay 1
  }

  // Control relay 2 based on humidity
  if (humidity > 75) {
    digitalWrite(relay2, HIGH); // Turn on relay 2
  } else {
    digitalWrite(relay2, LOW); // Turn off relay 2
  }

  // Control relay 3 based on humidity
  if (humidity < 65) {
    digitalWrite(relay3, HIGH); // Turn on relay 3
  } else {
    digitalWrite(relay3, LOW); // Turn off relay 3
  }

  // Display temperature and humidity on LCD monitor
  lcd.clear();
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");

  // Print temperature and humidity values to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(2000); // Wait for 2 seconds before next reading
}
