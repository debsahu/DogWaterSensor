# DogWaterSensor

[![Dog Water Sensor](https://img.youtube.com/vi/AfrIyBWgfdM/0.jpg)](https://www.youtube.com/watch?v=AfrIyBWgfdM)

The STL files are posted here as well as https://www.thingiverse.com/thing:2588240

Things needed:
1. 3D Printed Case
2. 18650 Battery
3. ESP8266 12E
4. Rain Water Level Sensor

Use Arduino IDE to upload code on ESP8266 by connecting GPIO0 to Ground. **For Deep-Sleep to work one has to conenct GPIO16 to RESET on ESP8266**.

This sensor connects to MQTT and Home Assistant (https://home-assistant.io/). See dogsensor.yaml for adding this to HA. Assumes that one has set up various notification sensors (https://home-assistant.io/components/#notifications) available in HA.
