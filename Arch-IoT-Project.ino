/**
 * Architecture IoT Project
 * Fire Prevention device 
 * This is the Arduino source code for the device
 *
 * @author Johan Rodriguez, Gabriel Alfaro, Christian Ferreto
 * @version 1.0
 */

#include "DHT.h" // Library for DHT11 sensor

#define DHTPIN 7 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11
#define MQ2pin (0) // Define which analog input channel to be used for the MQ2 sensor

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino

/**
 * Struct to store the data of the sensors
 */
struct sensorData {
  float temperature; // Temperature in Celsius
  float humidity; // Humidity in %
  float MQ2; // MQ2 sensor value
};

/**
 * This function is used to read the data from the sensors
 *
 * @return sensorData
 */
sensorData getSensorData() {
  sensorData data;
  data.temperature = dht.readTemperature();
  data.humidity = dht.readHumidity();
  data.MQ2 = analogRead(MQ2pin);
  return data;
}

void setup() {
  Serial.begin(9600); // Initialize serial communication

  dht.begin(); // initialize the DHT sensor
}

void loop() {
  sensorData data;

  // Alert messages
  bool temperatureAlert = false;
  bool humidityAlert = false;
  bool gasAlert = false;

  // Error messages
  bool temperatureError = false;
  bool humidityError = false;
  bool gasError = false;

  for (int i = 0; i < 100; i++) {
    data = getSensorData();

    if (data.MQ2 == isnan(data.MQ2) && !gasError) {
      Serial.println("Failed to read from MQ2 sensor!");
      gasError = true;
      continue;
    }

    if (data.temperature == isnan(data.temperature) && !temperatureError) {
      Serial.println("failed to read from DHT sensor!");
      temperatureError = true;
      continue;
    }

    if (data.humidity == isnan(data.humidity) && !humidityError) {
      Serial.println("Failed to read from DHT sensor!");
      humidityError = true;
      continue;
    }

    if (!temperatureAlert && data.temperature > 30) {
      Serial.println("Temperature is too high");
      temperatureAlert = true;
    }

    if (!humidityAlert && data.humidity > 70) {
      Serial.println("Humidity is too high");
      humidityAlert = true;
    }

    if (!gasAlert && data.MQ2 > 150) {
      Serial.println("Gas is too high");
      gasAlert = true;
    }

    delay(100);
  }

  Serial.println();
  Serial.println("Temperature: " + String(data.temperature) + " C");
  Serial.println("Humidity: " + String(data.humidity) + " %");
  Serial.println("MQ2: " + String(data.MQ2) + " ppm");

  // TODO: Send data

  temperatureAlert = false;
  humidityAlert = false;
  gasAlert = false;

  temperatureError = false;
  humidityError = false;
  gasError = false;
}
