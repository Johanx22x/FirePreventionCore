/**
 * Architecture IoT Project
 * Fire Prevention device 
 * This is the Arduino source code for the device
 *
 * @author Johan Rodriguez, Gabriel Alfaro, Christian Ferreto
 * @version 1.0
 */

#include "DHT.h" // Library for DHT11 sensor
#include <SoftwareSerial.h> // Library for bluetooth module

#define DHTPIN 7 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11
#define MQ2pin (0) // Define which analog input channel to be used for the MQ2 sensor
#define RX 5
#define TX 4

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino
SoftwareSerial hc06(TX,RX);

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
  dht.begin(); // initialize the DHT sensor
  hc06.begin(9600); //Initialize Bluetooth Serial Port
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

    if (i == 50) {
      hc06.print("A: Test alert$");  
    }

    if (data.MQ2 == isnan(data.MQ2) && !gasError) {
      hc06.print("E: Gas sensor is not working$");
      gasError = true;
      continue;
    }

    if (data.temperature == isnan(data.temperature) && !temperatureError) {
      hc06.print("E: Temperature sensor is not working$");
      temperatureError = true;
      continue;
    }

    if (data.humidity == isnan(data.humidity) && !humidityError) {
      hc06.print("E: Humidity sensor is not working$");
      humidityError = true;
      continue;
    }

    if (!temperatureAlert && data.temperature > 30) {
      hc06.print("A: Temperature is too high$");
      temperatureAlert = true;
    }

    if (!humidityAlert && data.humidity < 45) {
      hc06.print("A: Humidity is too low$");
      humidityAlert = true;
    }

    if (!gasAlert && data.MQ2 > 150) {
      hc06.print("A: Gas is too high$");
      gasAlert = true;
    }

    delay(100);
  }

  // Send data 
  hc06.print("T: " + String(data.temperature) + ";");
  hc06.print("H: " + String(data.humidity) + ";");
  hc06.print("G: " + String(data.MQ2) + "$");

  temperatureAlert = false;
  humidityAlert = false;
  gasAlert = false;

  temperatureError = false;
  humidityError = false;
  gasError = false;
}
