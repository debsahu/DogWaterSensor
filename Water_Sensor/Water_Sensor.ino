#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define MQTT_SERVER "192.168.0.xxx"
const char* ssid     = "xxxxxxxxxxxxxx";
const char* password = "xxxxxxxxxxxxxx";

const int sleepTimeS = 300; // 5*60 sec

WiFiClient wifiClient;
PubSubClient client(wifiClient);

//MQTT callback
void callback(char* topic, byte* payload, unsigned int length) {
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("kosmo_water_sensor")) {
      Serial.println("connected");
      client.subscribe("kosmowatersensor/status");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup_wifi() {
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(10);
  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
  delay(10);
  reconnect();
  delay(2);
  client.publish("kosmowatersensor/status", "ON");
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  delay(1000);
  int sensorValue = analogRead(A0);
  delay(100);
  digitalWrite(5, LOW);
  client.publish("kosmowatersensor/adc", String(sensorValue).c_str());
  Serial.print("ADC: ");
  Serial.println(sensorValue);
  client.publish("kosmowatersensor/status", "SLEEPING");
  ESP.deepSleep(sleepTimeS * 1000000);
}

void loop() {
//  do nothing
}

