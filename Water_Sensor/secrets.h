#define SECRETS

#define WIFI_MULTI false

// WiFi credentials.
#if(WIFI_MULTI)
  #define WIFI_SSID1 "wifi_ssid_1"
  #define WIFI_PASS1 "password1"
  #define WIFI_SSID2 "wifi_ssid_2"
  #define WIFI_PASS2 "password2"
#else
  #define WIFI_SSID "wifi_ssid"
  #define WIFI_PASS "wifi_password"
#endif

// MQTT Settings.
#define MQTT_HOST "192.168.yyy.xxx"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASS ""