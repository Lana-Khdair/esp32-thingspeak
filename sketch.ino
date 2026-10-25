
#include <WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"

// Student info 
const char* STUDENT_NAME = "Lana Khdair";
const char* UNIVERSITY_ID = "12325211";

// Wokwi's built-in WiFi
const char* WIFI_Name = "Wokwi-GUEST";
const char* WIFI_Pass = "";

const int channelNumber = 3550017;
const char* apiKey = "BRCIPCQ2JCCG5103";

// DHT22 data pin is connected to GPIO 15
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// to connect to the service
WiFiClient clientObject;

void setup() {

  Serial.begin(115200);
  // start the sensor
  dht.begin();

  // use the existing wifi
  WiFi.mode(WIFI_STA);
  // connect
  WiFi.begin(WIFI_Name, WIFI_Pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Status: Not Connected");
    delay(1000);
  }
  Serial.println("Status:  Connected");
  ThingSpeak.begin(clientObject);
}

void loop() {

  // read the data from the sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // check if the sensor reading failed
  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read from DHT22");
    delay(2000);
    return;
  }

  // print the data and the university ID in the log
  Serial.print("Name: ");
  Serial.print(STUDENT_NAME);
  Serial.print(" | University ID: ");
  Serial.println(UNIVERSITY_ID);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // which data to write
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  
  // send the university ID with the request as a status message
  ThingSpeak.setStatus(String("UniID: ") + UNIVERSITY_ID);

  int stat = ThingSpeak.writeFields(channelNumber, apiKey);
  if (stat == 200)
  {
    Serial.println("Data Sent to ThingSpeak");
  }
  else
  {
    Serial.println("Error sending data, code: " + String(stat));
  }

  // Send the data to ThingSpeak every 20 seconds
  delay(20000);
}