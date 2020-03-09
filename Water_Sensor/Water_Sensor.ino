#include <MQTT.h>          //https://github.com/256dpi/arduino-mqtt
#include "secrets.h"       // WiFi/MQTT settings >> please edit accordingly
#ifdef ESP32
  #include <WiFi.h>
  #if(WIFI_MULTI)
  #include <WiFiMulti.h>
  WiFiMulti wifiMulti;
  #endif
#else
  #include <ESP8266WiFi.h>
  #if(WIFI_MULTI)
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #endif
#endif

#define SENSOR_PWR_PIN 5   // GPIO5 to V+ of sensor

// Signal from sensor connected to >> ADC pin <<
#ifdef ESP32
  #define SIGNAL_PIN 36    // Signal from sensor connected to ADC pin GPIO36/VP(lolin D32) on ESP32
#else
  #define SIGNAL_PIN A0    // Signal from sensor connected to only ADC pin A0 on ESP8266
#endif

#define uS_TO_S_FACTOR 1000000
const int timeToSleep = 300;   // 5 * 60 sec = 5 min

////////////////////////////////////////////////////////////////////////

WiFiClient wifiClient;
MQTTClient client(512);

boolean connect_mqtt(void)
{
  uint8_t retries = 0;
  Serial.print(F("Connecting MQTT "));
  while (!client.connect("KOSMO_WATER_SENSOR", MQTT_USER, MQTT_PASS) and retries < 15)
  {
    Serial.print(".");
    delay(5000);
    retries++;
  }
  if (!client.connected())
  {
    Serial.println(F(" failed!"));
    return false;
  }
  Serial.println(F(" connected!"));
  return true;
}

boolean setup_wifi() {
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
#if(WIFI_MULTI)
  Serial.println("WiFi...");
  wifiMulti.addAP(WIFI_SSID1, WIFI_PASS1);
  wifiMulti.addAP(WIFI_SSID2, WIFI_PASS2);
  uint8_t counter = 0;
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    counter++;
    if (counter >= 60)
    { //after 30 seconds timeout
      Serial.println("failed!");
      return false;
    }
  }
#else
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  uint8_t counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if (counter >= 60)
    { //after 30 seconds timeout
      Serial.println("failed!");
      return false;
    }
  }
  #endif

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(10);
  if ( setup_wifi() )
  {
    client.begin(MQTT_HOST, MQTT_PORT, wifiClient);
    pinMode(SENSOR_PWR_PIN, OUTPUT);
    digitalWrite(SENSOR_PWR_PIN, HIGH);
    delay(1000);
    if( connect_mqtt() )
    {
      delay(2);
      client.publish("kosmowatersensor/status", "ON");
      int sensorValue = analogRead(SIGNAL_PIN);
      client.publish("kosmowatersensor/adc", String(sensorValue));
      Serial.print("ADC: ");
      Serial.println(sensorValue);
      client.publish("kosmowatersensor/status", "SLEEPING");
      client.disconnect();
    }
    delay(100);
    digitalWrite(SENSOR_PWR_PIN, LOW);
  }
  
  Serial.flush();
  WiFi.disconnect(true);
#ifdef ESP32 
  esp_sleep_enable_timer_wakeup(timeToSleep * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
#else
  ESP.deepSleep(timeToSleep * uS_TO_S_FACTOR); // Connect RESET and GPIO16 for ESP8266
#endif
}

void loop() {
//  do nothing
}