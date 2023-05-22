#include <Arduino.h>
#include <WiFi.h>
#include <DHTesp.h>
#include <BH1750.h>
#include <ThingsBoard.h>
#define WIFI_SSID "iot2.4g"
#define WIFI_PASSWORD "iot2.4g123"

#define THINGSBOARD_SERVER "demo.thingsboard.io"
#define THINGSBOARD_ACCESS_TOKEN "jxkcbrmifget2elqyzpb"

// #define THINGSBOARD_MQTT_CLIENTID "got4vmffvq1u73h47cr1"
// #define THINGSBOARD_MQTT_USERNAME "phwkw0rhrj89tu8w2war"
// #define THINGSBOARD_MQTT_PASSWORD "uehmpqzn3nmfmjmm4k18"

#define LED_GREEN  4
#define LED_YELLOW 5
#define LED_RED    18
#define DHT_PIN 19

DHTesp dht;
BH1750 lightMeter;
WiFiClient wifiClient;
ThingsBoard tb(wifiClient);

void WifiConnect();

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BUILTIN, 0);
  dht.setup(DHT_PIN, DHTesp::DHT11);
  Wire.begin();
  lightMeter.begin(); 
  Serial.println("Booting...");
  WifiConnect();
  if (tb.connect(THINGSBOARD_SERVER, THINGSBOARD_ACCESS_TOKEN))
    Serial.println("Connected to ThingsBoard");
  else
    Serial.println("Error connected to ThingsBoard");
}

void loop() {
  if (millis() % 10000 ==0)
  {
    digitalWrite(LED_BUILTIN, 1);
    float fHumidity = dht.getHumidity();
    float fTemperature = dht.getTemperature();
    float lux = lightMeter.readLightLevel();
    Serial.printf("Humidity: %.2f, Temperature: %.2f, Light: %.2f \n",
       fHumidity, fTemperature, lux);
    tb.sendTelemetryFloat("humidity", fHumidity);
    tb.sendTelemetryFloat("temperature", fTemperature);
    tb.sendTelemetryFloat("light", lux);
    digitalWrite(LED_BUILTIN, 0);
  }
  tb.loop();
}

void WifiConnect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }  
  Serial.print("System connected with IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("RSSI: %d\n", WiFi.RSSI());
}