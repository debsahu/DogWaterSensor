# DogWaterSensor

The STL files are posted here as well as https://www.thingiverse.com/thing:2588240

Things needed:
1. 3D Printed Case
2. 1x 18650
3. ESP8266 12E
4. Rain Water Level Sensor

Use Arduino IDE to upload code on ESP8266 by grounding GPIO0 with a 10K resistor.

This sensor connects to MQTT and Home Assistant (https://home-assistant.io/). See dogsensor.yaml for adding this to HA. Assumes that one has set up various notification sensors (https://home-assistant.io/components/#notifications) available in HA.
