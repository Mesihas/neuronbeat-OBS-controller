# neuronbeat-OBS-controller
WiFi remote control for OBS
![alt text](https://github.com/Mesihas/neuronbeat-OBS-controller/blob/main/Docs/images/NeuronBeat-OBS-Controller-popmenu.png)

### About The Project
neuronbeat-OBS-controller: a dedicated WiFi remote control for OBS.   
It works like Studio mode, can browse scenes and sources without affecting what OBS is showing.
One of its main features is that the user can control OBS while doing something else as the encoder and ligths
offers feedback to the user.    
This device uses a rotary encoder with switch as a navigator.   
Supports 40 Scenes and 40 Sources per Scene. 

### Hardware

CPU: ESP32   
Display: TFT SPI 480*320 with touchscreen   
Nav knob Encoder with switch and 3 buttons   
3 Neopixel leds   

### Software
C++   
Arduino IDE   

<!-- ROADMAP -->
## Roadmap

- [ ] Add encription to credential stored on file system / Screen
- [ ] Add Groups
- [ ] Add Mute All
- [ ] Add REC button
- [ ] Cretate documentation
- [ ] Improve configuration
- [ ] Add User config for buttons and leds
- [ ] Add filters control to sources


## Prerequisites
### OBS Websokets 5 plugin must be installed and running
OBS Websokets: the amazing plugin that does the magic on OBS side   
https://github.com/obsproject/obs-websocket

### Necesary libraries 

 <vfs_api.h> <FS.h> <FSImpl.h>   
 <ArduinoJson.h> https://github.com/bblanchon/ArduinoJson   
 <WebSocketsClient.h> https://github.com/Links2004/arduinoWebSockets   
 <WiFi.h>   
 <FastLED.h> https://github.com/FastLED/FastLED   
 <SPI.h>   
 <TFT_eSPI.h> https://github.com/Bodmer/TFT_eSPI   
 <JPEGDecoder.h> https://github.com/Bodmer/TFT_eSPI   
 <OneButton.h> https://github.com/mathertel/OneButton   
 <ESP32Encoder.h> https://github.com/madhephaestus/ESP32Encoder   
 <WiFiClient.h>   
 <WebServer.h>   
 <ESPmDNS.h>   
 <Update.h> https://github.com/espressif/arduino-esp32/blob/master/libraries/Update/src/Update.h   
 <mbedtls/sha256.h>   
 <mbedtls/base64.h>   
 <AsyncTimer.h> https://github.com/Aasim-A/AsyncTimer   


<p align="right">(<a href="#top">back to top</a>)</p>

### ESP32 partition scheme
This project needs SPIFFS With OTA support
We suggest one of these schemes
* Default 4MB with spiffs(1.2MB APP/1.5MB SPIFFS)
* Minimal SPIFFS (1.9 MB APP with OTA/190KB SPIFFS) For more APP PROGRAM SPACE


### create a setup file for the TFT_eSPI Library
https://github.com/Bodmer/TFT_eSPI


#define TFT_MOSI 23   

#define TFT_SCLK 18   

#define TFT_CS   17  // Chip select control pin   

#define TFT_DC    2  // Data Command control pin  

#define TFT_RST   4  // Reset pin (could connect to RST pin)   

#define TOUCH_CS 16    // Chip select pin (T_CS) of touch screen   


## Circuit 

TFT, Encoder, Buttons and Leds

![alt text](https://github.com/Mesihas/neuronbeat-OBS-controller/blob/main/NeuronBeat%20Controller%20circuit.png)

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>