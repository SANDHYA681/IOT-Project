#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

// Server details
const char* SERVER_BASE = "https://192.168.100.100:5000";
String sensorDataUrl = String(SERVER_BASE) + "/sensor-data";


int prevMoisture = -1;
int prevTemperature = -1;
int prevHumidity = -1;


void sendSensorData(String type, int value) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String endpoint = String(SERVER_BASE) + "/sensor-data?type=" + type + "&value=" + String(value);


    http.begin(endpoint);
    int httpCode = http.GET();


    if (httpCode > 0) {
      Serial.println("Data sent successfully: " + type + " = " + String(value));
      Serial.println("HTTP Response code: " + String(httpCode));
    } else {
      Serial.println("Error in sending data: " + type + " = " + String(value));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected. Unable to send data.");
  }
}

void setup() {

  Serial.begin(9600);


  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println(sensorDataUrl);


  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" ");
  Serial.println("Connected to WiFi");
}

void loop() {

  if (Serial2.available()) {

    String receivedData = Serial2.readStringUntil('\n');


    Serial.println("Received: " + receivedData);


    int mIndex = receivedData.indexOf("m:");
    int tIndex = receivedData.indexOf("t:");
    int hIndex = receivedData.indexOf("h:");


    if (mIndex != -1) {
      int moisture = receivedData.substring(mIndex + 2, tIndex - 1).toInt();
      if (moisture != prevMoisture) {
        sendSensorData("moisture", moisture);
        prevMoisture = moisture;
        Serial.println("moisture: " + String(moisture));
      }
    } else {
      sendSensorData("moisture", -1);
    }
    if (tIndex != -1) {
      int temperature = receivedData.substring(tIndex + 2, hIndex - 1).toInt();
      if (temperature != 0 && temperature != prevTemperature) {
        sendSensorData("temperature", temperature);
        prevTemperature = temperature;
        Serial.println("temperature: " + String(temperature));
      }
    } else {
      sendSensorData("temperature", -1);
    }
    if (hIndex != -1) {
      int humidity = receivedData.substring(hIndex + 2).toInt();
      if (humidity != 0 && humidity != prevHumidity) {
        sendSensorData("humidity", humidity);
        prevHumidity = humidity;
        Serial.println("humidity: " + String(humidity));
      }
    } else {
      sendSensorData("humidity", -1);
    }
  }
}
