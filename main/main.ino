#define FS_NO_GLOBALS
#include <vfs_api.h>
#include <FS.h>
#include <FSImpl.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <WiFi.h>
#include <FastLED.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <JPEGDecoder.h>
#include <OneButton.h>
#include <ESP32Encoder.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <mbedtls/sha256.h>
#include <mbedtls/base64.h>
#include <AsyncTimer.h>
#include "web-server.h" //stored web pages
#include "sourceIcons.h"


ESP32Encoder encoder;
// fast led
#define NUM_LEDS 3
#define DATA_PIN 27
// Define the array of leds
CRGB leds[NUM_LEDS];
#define BRIGHTNESS  5
// flies-----------------------------------------------------------------
#define CREDENTIALS_FILE "/CredentialsData.txt"
#define CALIBRATION_FILE "/TouchCalData"
#define PAYLOAD_FILE "/PayloadData"

// Mic
#define MIC_ON_WHITE_BACKGROUND "/mic-on-white-bg.jpg"
#define MIC_OFF_WHITE_BACKGROUND "/mic-off-white-bg.jpg"
#define MIC_OFF_BLACK_BACKGROUND "/mic-off-black-bg.jpg"
#define NO_MIC_WHITE_BACKGROUND "/no-mic-white-bg.jpg"
// wifi icon jpg
#define WIFI_RSSI_0 "/no-wifi-white-bg.jpg"
#define WIFI_RSSI_1 "/wifi-signal-1.jpg"
#define WIFI_RSSI_2 "/wifi-signal-2.jpg"
#define WIFI_RSSI_3 "/wifi-signal-3.jpg"
#define WIFI_RSSI_4 "/wifi-signal-4.jpg"
// obs icon
#define OBS_CONN_W_BG "/obs-conn-white-bg.jpg"
#define OBS_NOT_CONN_W_BG "/obs-not-conn-w-bg.jpg"
#define OBS_CONN_B_BG "/obs-conn-black-bg.jpg"
#define OBS_NOT_CONN_B_BG "/obs-not-conn-b-bg.jpg"

#define CONFIG_ICON_WHITE_BACKGROUND "/cfg_icon_white_bg.jpg"

char LOGO_NB_BIG[64] = "/NB-logo-gr.jpg";
// REC icon
char REC_ON_W_BG[64] = "/reco-on-whiteBG.jpg";
char REC_Off_W_BG[64] = "/reco-off-whiteBG.jpg";
char REC_ON_B_BG[64] = "/reco-on-blackBG.jpg";
char REC_Off_B_BG[64] = "/reco-off-blackBG.jpg";
// FIRMAWARE VERSION ----------------------------------------------------------------------
String firmwareVersion =  "4.3.0";
// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false
//====================================================================
// Stock font and GFXFF reference handle
#define GFXFF 1
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
#define LABEL2_FONT &FreeSansBold12pt7b 
#define LABEL3_FONT &FreeSans9pt7b 
#define AA_FONT_LARGE "Lato-Light20"
#define AA_FONT_SMALL  "Lato-Light12"

#define LATO_REG_16 "Lato-Regular16"
#define LATO_REG_30 "Lato-Regular30"
#define LATO_BLK30 "Lato-Black30"
#define LATO_BLK16  "Lato-Black16"

/// Auth ----------------------------------------------------------------------
#define SHA256_HASH_LEN 32 /* SHA-256 digest length */
/// Theme ----------------------------------------------------------------------
bool themeGlobal; //defines theme globally true = ligth / false = dark
/// Config ----------------------------------------------------------------------
struct Config {
  char ssid[64];
  char password[64];
  char websockets_server_host[64];
  uint16_t websockets_server_port;
  char wsp[64]; // used to store the keyboard input char
  char websockets_password[64];
};
Config config;

bool saveCredentials = false;
bool starting = true; // use to do a task once
bool firstLoad = true;
// ??????????????????????????????????????????????????????????????????????????????????????????????????????????????
bool connectedToOBS = false;
bool configuratorActive = false ;

String netConfigSteps[4] = {"WiFi Network name:", "Password:", "WS server:", "WS port:"};
int currentConfigStep = -1; // -1 mainLevel 0 SSID - 1 Pass - 2 WSserver - 3 Port
int highlithedNetMenuint = 0;

int highlithedGenericMenuint = 0;
int encoderGenericLastValue = 0;
int keyboardCallback = 0;
bool keyboardActive = false;

//// Timer------------------------------------------------------------------
#define INTERVAL_REC 1000
#define INTERVAL_RSSI 5000
#define INTERVAL_MESSAGE3 11000
#define INTERVAL_MESSAGE4 13000
 
unsigned long time_rec = 0;
unsigned long time_rssi = 0;
unsigned long time_3 = 0;
unsigned long time_4 = 0;

AsyncTimer t; // used in conbination with the encoder
char* asyncTimerLastSceneNameReq;
//-------------- ARDUINOJson------------------------------------------------------------
DynamicJsonDocument docAuth(1000);
DynamicJsonDocument docMessage(20000);

WebSocketsClient webSocket;
//---------APP STATE DATA STRUCTURE ---------------------------------------------------
char * scenesL[40];
char * sourceItemsNames[40];
char * input_kinds[20];

int sourceItemsL[40][8]; //app state / stores  items or sources for the highlithed scene. 
// [0] [isGroup? ]
// [1] [sceneItemId]
// [2] [sceneItemIndex]
// [3] [sceneItemIndexREAL]
// [4] [sceneItemEnabled]
// [5] [GetSceneItemLocked]
// [6] [inputKind] 
// [7][generic]{inputKind ==7 stores mic level}

int sourcesCount;
int scenesCount;
int currentSceneInt; //encoder
//---------APP STATE For Encoder and so---------------------------------------------------
//new paging system vars
int scenePageCount = 1; // IU - Scene page count
int scenesCurrentPage; // UI - scene current page
int scenesLastCurrentPage = 0; // UI - scene list clean

int sourcesPageCount = 1; // IU - Sources page count
int sourcesCurrentPage; // UI - Sources current page
int sourcesLastItemPage = 0; // UI - Sources list clean
///////////////////////
const char* currentProgramSceneName = "";

int highlithedSceneInt = -1; // -1 = not set  // keeps higlithed scene value global
//int lastHighlithedSceneInt = -1; //  not used
int highlithedSourceInt = 0;
int lastHighlithedSourceInt = 0;
//int lastCursorY = -1; //-1 == not set
int lastCursorYscenes = -1; //-1 : not set- this help to clean sprite last highlithed
int lastCursorYsources = -1; //-1 : not set- this help to clean sprite last highlithed
int lastCursorXsourcesMenu = -1; //-1 : not set- this help to clean sprite last highlithed
int encoderLastValue = 0;
int encoderLastDirection;
int encoderDirection = 0; //0 previus // 1 next
boolean viewScenes = true;
boolean viewSources = false;
boolean viewPopUp = false;

int highlithedPopUpMenuInt = 3;  // keeps higlithed popUp menu selected option value global

/// menu popUp sources 

int audioInputLevelMode = false;
int audioInputLevel = 0; // holds audioInputLevel for encoder
//////////////////////

bool movingEncoder = false;
bool encoderButLngPress = false; //??????
int globalFadeInValue = 0;
int globalFadeOutValue = 255;
int sceneIntToUpdate = 0;
int itemIndexToUpdate = 0;
int countOfEncoderEntries;

long encoderTimeArray[30][3]; //[direction] [millis] [0-scenes/1-sources/2-config]

long eat = 0; //encoderTimeArray // has to be long to avoid conversion later on
bool encoderStopMotion = true;
////////////////////////////////////// ESTO SALE DEL MENSAJEEEEEEEEEEEEEEEEEEEEEE
bool lastMic = true; //????
bool recording = false; //???
bool lastRec = false;//????
bool scenesLoaded = false;//????
//---------Setup and Network---------------------------------------------------
bool initRecCheck = false;
String wifiNetworks[11];
int highlithedSSIDint = 0;
boolean currentSceneCollectionChanging = false; //to trigger a pause of all polling requests to avoid crashing
//gui
int bottomBarIconsHeight = 294;
int lsdcY = 40; //lists Start display Coordinates (px)
int ldhi = 24; //list display row height Increment 


/// this is for payload filter purposes
String auxPayloadFilter = "";
int updateTypeFilter;

// TFT & Touch
TFT_eSPI tft = TFT_eSPI();
uint16_t x, y;
#define Z_THRESHOLD 1200 // Touch pressure threshold for validating touches

TFT_eSprite sprHglScenes = TFT_eSprite(&tft);  // Create Sprite object "sprHglScenes" with pointer to "tft" object
TFT_eSprite sprListRegular = TFT_eSprite(&tft); // 
TFT_eSprite sprListBold = TFT_eSprite(&tft); // 
TFT_eSprite sprMenuSources = TFT_eSprite(&tft); // popMenu sources
TFT_eSprite sprMenuAudioLevel = TFT_eSprite(&tft); // popMenu sources

TFT_eSprite sprTest = TFT_eSprite(&tft); 

//====================================================================
OneButton buttonEncoder(33, true);
OneButton botButton(21, true);
OneButton topButton(22, true);
OneButton middButton(32, true);

// OTA ----------------------------------------------
const char* host = "esp32";
WebServer server(80);
// --webSocketEvent -----------------------------------------------

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  // Serial.println("webSocketEvent Message Length: ");
  // Serial.println(length);
  // Serial.print("webSocketEvent ram memory free: " + String((int32_t)ESP.getFreeHeap()));
  // Serial.println("-------------------------------------------------------");
	switch(type) {
		case WStype_DISCONNECTED:
      connectedToOBS = false;
      Serial.println("WStype_DISCONNECTED!");
			Serial.printf("[WSc] Disconnected!\n");
      tft.setTextSize(1);
      tft.setCursor(240, 295, 2);
      tft.println("Not Connected to OBS!");
      open_and_print_file(OBS_NOT_CONN_W_BG, 410, bottomBarIconsHeight);        
      // cleanContent();
      // starting = true;
      // initRecCheck = false;
			break;
		case WStype_CONNECTED:

      // Serial.println("WStype_CONNECTED!");
      // Serial.println("webSocketEvent Message Length: ");
      // Serial.println(length);
      open_and_print_file(OBS_CONN_W_BG, 410, bottomBarIconsHeight);
      tft.setCursor(240, 295, 2);
      tft.println("                          ");
      // // connectedToOBS = true;
      // // tft.fillRect(130, 290, 220, 30, TFT_WHITE);
      // // if (starting == true){
      // //   starting = false;
      // //   Serial.print("getStreamingStatus at starting : "); Serial.println(ESP.getFreeHeap());      
      // //   if(!initRecCheck){         
      // //     getStreamingStatus();
      // //   }
      // //   Serial.println("FIN starting--------------- ");  
      // // }

		case WStype_TEXT:
      //open_and_print_file(OBS_CONN_W_BG, 410, bottomBarIconsHeight + 2);         
      checkPayload(payload, length);
			break;
		case WStype_BIN:
      Serial.println("WStype_BIN !");
			Serial.printf("[WSc] get binary length: %u\n", length);
        //	hexdump(payload, length);
			// send data to server
			// webSocket.sendBIN(payload, length);
			break;
		case WStype_ERROR:			
      Serial.println("WStype_ERROR!");	 
      break;
		case WStype_FRAGMENT_TEXT_START:
		case WStype_FRAGMENT_BIN_START:
		case WStype_FRAGMENT:
//		case WStype_FRAGMENT_FIN:
			break;
	}
}

struct ThemeColor {
  uint16_t ink_color;
  uint16_t background_color;
  uint16_t current_scene_color;
  uint16_t visible_source_color;
  uint16_t hidden_source_color;
  uint16_t sources_background_color;
  uint16_t focus_scenes_color;
  uint16_t focus_sources_color;
  uint16_t ssid_highlithed_color;
  uint16_t ssid_color;
};

ThemeColor themeColor; 

StaticJsonDocument<1024> docMenu;

// AsyncTimer
unsigned short timeoutWSrequest;
unsigned short timeoutEncoderButtonLngPressStart; //Please rename -  used to change state "on release" button encoder
unsigned short timeoutMovingEncoderTrue; // used to control GetSceneItemList Async task (asyncTimer)
unsigned short clickEncoderButtonTask; // used to control GetSceneItemList Async task (asyncTimer)
bool timeOutSetWSrequest = false;
//bool timeOutSetGetSceneItemList = false;

void setup() {
  Serial.begin(115200);
  t.setup(); //async timer
  xTaskCreatePinnedToCore(secondCoreParallelTask, "Task1", 10000, NULL, 1, NULL,  1); 

  themeColor.ink_color = rgb(0,0,0);
  themeColor.background_color = rgb(255,255,255);
  themeColor.current_scene_color = rgb(255,0,0);
  themeColor.visible_source_color = rgb(255,0,0);
  themeColor.hidden_source_color = rgb(0,0,255);
  themeColor.sources_background_color = rgb(211,211,211);//TFT_LIGHTGREY
  themeColor.focus_scenes_color = rgb(255,0,0);
  themeColor.focus_sources_color = rgb(0,0,255);
  themeColor.ssid_highlithed_color = rgb(128,128,128);//TFT_DARKGREY
  themeColor.ssid_color = rgb(211,211,211);//TFT_LIGHTGREY
  //themeColor.focus_sources_color = rgb(0,0,255);

  FastLED.setBrightness(BRIGHTNESS);
 // Serial.begin(115200);
  // link the buttonEncoder  functions.
  buttonEncoder.attachClick(buttonEncoderClick);
  buttonEncoder.attachDoubleClick(buttonEncoderDoubleclick);
  buttonEncoder.attachDuringLongPress(buttonEncoderLongPress);
  buttonEncoder.attachLongPressStart(buttonEncoderLongPressStart);
  buttonEncoder.attachLongPressStop(buttonEncoderLongPressStop);
  // link the topButtonoards  functions.

  topButton.attachClick(topButton_Click);
  topButton.attachLongPressStop(topButton_longPressStop);

  middButton.attachClick(middButton_Click);
  middButton.attachLongPressStop(middButton_longPressStop);

  botButton.attachClick(botButton_Click);
  botButton.attachLongPressStop(botButton_longPressStop);

  if (!SPIFFS.begin()) {
      Serial.println("SPIFFS initialisation failed!");
      while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nInitialisation done.");
  //listFiles(); // Lists the files so you can see what is in the SPIFFS


  // ESP32 will crash if any of the fonts are missing, so check
  bool font_missing = false;
  if (SPIFFS.exists("/Lato-Light20.vlw") == false) font_missing = true;

  //if (SPIFFS.exists("/Lato-Light12.vlw") == false) font_missing = true;
  if (SPIFFS.exists("/Lato-Regular16.vlw") == false) font_missing = true;
  if (SPIFFS.exists("/Lato-Regular30.vlw") == false) font_missing = true;
  if (SPIFFS.exists("/Lato-Black30.vlw") == false) font_missing = true;
  if (SPIFFS.exists("/Lato-Black16.vlw") == false) font_missing = true;

  if (font_missing)
  {
    Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
    while (1) yield();
  }
  else Serial.println("\r\nFonts found OK.");

  //ESP32Encoder::useInternalWeakPullResistors=DOWN;
  // Enable the weak pull up resistors
 // ESP32Encoder::useInternalWeakPullResistors=UP;

  // Attache pins for use as encoder pins
  encoder.attachSingleEdge(25, 26);
  encoder.setCount(0);
  //===========================
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(themeColor.background_color);

  open_and_print_file(LOGO_NB_BIG, 40, 100);

  touch_calibrate();

  tft.loadFont(AA_FONT_LARGE);
  if(!readWiFiCredentials()){
/////////////////////////////////////////////////////////////////   showWiFiConfigData();
  }else{
   Serial.println("Credenciales leidas!");
  }
   //////////// NEOPIXEL ///////////////////////////////////////////////////
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

  // Connect to wifi   
  WiFi.begin(config.ssid, config.password);

  // Wait some time to connect to wifi
  int brightness = 0;
  for(int i = 0; i < 10; i++) {//} && WiFi.status() != WL_CONNECTED; i++) {
    Serial.print(".");

    leds[0] = CRGB::Red;
    leds[1] = CRGB::Red;  
    leds[2] = CRGB::Red;        

    leds[0].fadeLightBy(brightness + i * 5);
    FastLED.show();
    delay(200);
    leds[1].fadeLightBy(brightness + i * 10);
    FastLED.show();
    delay(200);
    leds[2].fadeLightBy(brightness + i * 20);
    FastLED.show();
    delay(200);
  }

  // Check if connected to wifi
  if(WiFi.status() != WL_CONNECTED) {
      Serial.println("No Wifi!");
    ////////////////////////////////////////#################################
    // CALL WIFI CONFIGURATION
    // if default wifi is not available show message > "do you want to edit your wifi connexion?"
    // if yes ? > pantalla de configuracion
    WiFi.disconnect();
   // check_and_print_WifiSignal();
    botButton_longPressStop();
     ///////////////////////////////////////////////////////////////////////////////////////////////////   showNotConnToWiFi();
    // configuratorActive = true; ?????
    // configurator ?????
    return;
    ////////////////////////////////////////#################################
  }else{
    Serial.println("Connected to Wifi");
    Serial.println(WiFi.localIP());
    check_and_print_WifiSignal();
  }
  //////////// webSocket ///////////////////////////////////////////////////
	// server address, port and URL
  delay(100); 
  Serial.println("Connecting to WS Server");

	webSocket.begin(config.websockets_server_host, config.websockets_server_port, "/");
	webSocket.onEvent(webSocketEvent); 	// event handler
	webSocket.setReconnectInterval(5000);

  // OTA ---------------------------------------------------------------------
  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
  // OTA ---------------------------------------------------------------------
  input_kinds[0]; // keeps input kinds list
  turnOffAllLeds();
  resetArrayEncoderMotion();

  printMainGUI(false);
//  getSceneList();
  Serial.print("FIN SETUP--------------- : "); 
  Serial.println(ESP.getFreeHeap());

  input_kinds[0] = "slideshow";
  input_kinds[1] = "ffmpeg_source";
  input_kinds[2] = "text_gdiplus_v2";
  input_kinds[3] = "browser_source";
  input_kinds[4] = "wasapi_output_capture";
  input_kinds[5] = "window_capture";
  input_kinds[6] = "monitor_capture";
  input_kinds[7] = "wasapi_input_capture";
  input_kinds[8] = "dshow_input";
  input_kinds[9] = "image_source";
  input_kinds[10] = "color_source_v3";
  input_kinds[11] = "group";
  input_kinds[12] = "game_capture";
// Blackmagic Device
// Intel® RealSense™ 3D Camera GreenScreen
// Media Source
// Scene
// VLC Video Source
// Deprecated Sources
}

void resetArrayEncoderMotion(){
  for (size_t i = 0; i < 5; i++)
  {
    encoderTimeArray[i][0] = 0; //[direction] 
    encoderTimeArray[i][1] = 0; //[millis]
    encoderTimeArray[i][2] = 0; //[0-scenes/1-sources/2-config]
  }
}

// second core encharged of Led effects, Encoder button, encoder motion
void secondCoreParallelTask( void * pvParameters ){
  for(;;){
    executeLedTask(); // use 2nd core for Led effects
    buttonEncoder.tick(); // use 2nd core for encoder button task
    // getEncoderMotionStop();
    if(!configuratorActive || !viewPopUp){
      if(countOfEncoderEntries > 0 )
      {
        long start = encoderTimeArray[0][1];
        long end = encoderTimeArray[countOfEncoderEntries-1][1];
        long now = millis();
        if(now - end > 200)
        {
          // Serial.println("-------secondCoreParallelTask()  -----------------------------------");
          // //Serial.println("----getEncoderMotionStop>> > 200ms  --------------------------: ");
          // Serial.println("----start>> : " + String((long)start));
          // Serial.println("----end>> : " + String((long)end));
          // Serial.println("----now>> : " + String((long)now));
          // Serial.println("----countOfEncoderEntries " + String((int32_t)countOfEncoderEntries));
          // Serial.println("------------------------------------------------------------------");
          memset(encoderTimeArray, 0, sizeof(encoderTimeArray));
          resetArrayEncoderMotion();
          countOfEncoderEntries = 0;
          eat = 0 ;
          Serial.println("No  MOVING ENCODER ");
          movingEncoder = false; 
        }else{   
        //  Serial.println("MOVING ENCODER");
          movingEncoder = true;
        }
      }
    }
  }
}

void loop() {
  // let the websockets client check for incoming messages
  if(!configuratorActive){
    webSocket.loop();
  }
  ///////////////////////////////////////
  isConfiguratorActive();
  // Buttons
  middButton.tick();
  t.handle();
  botButton.tick();
  topButton.tick();
 // buttonEncoder.tick();
}

void checkPayload(uint8_t* payload, size_t length){    
  Serial.println("--------------------------------------------------------------------------");
  Serial.println("checkPayload Message Length: " + String((int32_t)length));
  Serial.printf("payload: %s\n", payload);

  auxPayloadFilter = (char * )payload;
  updateTypeFilter = auxPayloadFilter.indexOf("authentication");
  if(updateTypeFilter != -1){ 
    updateTypeFilter = auxPayloadFilter.indexOf("challenge");
    if(updateTypeFilter != -1){
      logger("checkPayload", "--NA", "WStype_TEXT!", "authRequired FOUND !!", true);
      auxPayloadFilter =="";
      authorization(payload, true, true);
    }
  }

  updateTypeFilter = auxPayloadFilter.indexOf("negotiatedRpcVersion");
  if(updateTypeFilter != -1){ 
    //logger("checkPayload", "--NA", "WStype_TEXT!", "authRequired FOUND !!", true);
    getSceneList("firstLoad"); 
  }else{ // normal message (no auth mess)z
   // Serial.println("checkPayload going to parseX");	
    parseX(payload, true, false);
  }
}
//######################### parseX #############################################################
void parseX(uint8_t* message, bool isResponseData, bool auth){
 // logger("ParseX: ", "--NA", "checkPayload", "parseX init", true);
  if(isResponseData){
    DeserializationError err = deserializeJson(docMessage, message);
      // Parse succeeded?
    if (err) {
      Serial.print(F("deserializeJson() returned "));
      Serial.println(err.c_str());
      return;
    }

    int memoryUsed = docMessage.memoryUsage();
    //logger("parseX docAuth deserializeJson","NA", "SF", "isUpdateType", true);
    // Fetch values.
    JsonObject d = docMessage["d"];
    int opCode = docMessage["op"];
    const char* requestId = d["requestId"];
    // Serial.println("<---------------------------------------");
    // Serial.println("opCode: ");
    // Serial.println(opCode);
    // Serial.println("requestId: ");
    // Serial.println(requestId);
    if(opCode == 5){///eventType
     JsonObject eventData = d["eventData"];
     const char* eventType = d["eventType"];
     obsEventHandler(eventType, eventData);

    // Serial.println("<---------------------------------------");
    // Serial.println("opCode: ");
    // Serial.println(opCode);
    // Serial.println("eventType: ");
    // Serial.println(eventType);   
    // Serial.println("<---------------------------------------");
  
    }else if(opCode == 9){
  //    Serial.println("--------------------------------------->");
      if(strcmp(requestId, "GetItemsStateRequest-01") == 0){
        processItemsStateFromJson(d); // pass whole jsonObject
        displaySourcesList(highlithedSourceInt, false, 0, "EVENT");
      }

    }else if(opCode == 7){
      const char* requestType = d["requestType"];
      int requestStatus_code = d["requestStatus"]["code"];
      JsonObject responseData = d["responseData"];
      bool requestStatus_result = d["requestStatus"]["result"];

      // Serial.println("requestType: ");
      // Serial.println(requestType);
      // Serial.println("requestStatus_code: ");
      // Serial.println(requestStatus_code);
      // Serial.println("--------------------------------------->");

       // See here what to do in case a message is empty or invalid
      if(requestStatus_code != 100 && !requestStatus_result){return;}
      if(strcmp(requestType, "GetSceneList") == 0 && strcmp(requestId, "001") == 0)
      {
        processSceneListFromJson(responseData);
        if(firstLoad)
        {
          getSceneItemList(currentProgramSceneName, "parseX GetSceneList requestType firstLoad");
        }else{
          getSceneItemList(scenesL[highlithedSceneInt], "parseX GetSceneList requestType");
        }
      }
      else if(strcmp(requestType, "GetSceneList") == 0 && strcmp(requestId, "001-Event") == 0) // ? Button tambien?
      {
      //  Serial.println("-------------obsEventHandler SceneListChanged getSceneList 001-Event --------------------------: ");
        processSceneListFromJson(responseData);
        displaySceneList(currentSceneInt, currentSceneInt, "Event handler");
      }else if(strcmp(requestType, "GetSceneItemList") == 0 ){
        // cancel processSceneItemListFromJson if moving encoder. 
        // This avoids cpu busy and encoder move smootly.

        if(movingEncoder){ 
          Serial.println("----parseX >> requestType, GetSceneItemList >> movingEncoder > return; --------------------------: ");
          return;
        }
        processSceneItemListFromJson(responseData);
        //draw scenes  displaySceneList(int currentSceneValue, int highlithedSceneInt)

      // for(int i=0;  i < scenesCount; i++){
      //   Serial.println(scenesL[i]);
      // }
      // Serial.println("---------------------------------------: ");
        if(firstLoad)
        {
          displaySceneList(currentSceneInt ,currentSceneInt, "firstLoad GetSceneList");
            
          highlithedSceneInt = currentSceneInt;
          firstLoad = false;
          getItemsStateRequest(currentSceneInt);
        }else{
          
          //displaySceneList(currentSceneInt ,highlithedSceneInt, "NO firstLoad GetSceneList");
          Serial.println("----parseX >> requestType, GetSceneItemList >> No firstLoad  --------------------------: ");
          getItemsStateRequest(highlithedSceneInt);
        }
      }
    }
  }
}

void obsEventHandler(const char* et, JsonObject eventData){
  if (strcmp(et, "CurrentSceneCollectionChanging") == 0){currentSceneCollectionChanging = true; return;}
  else if (strcmp(et, "SceneListChanged") == 0  || strcmp(et, "SceneItemCreated") == 0) //unir estos? / SceneNameChanged disregarded
  {
    Serial.println("-------------obsEventHandler SceneListChanged--------------------------: "); 
    getSceneList("Event handler");
  }
  else if (strcmp(et, "CurrentSceneChanged") == 0)
  {
    const char* sceneName  = eventData["sceneName"];
    displaySceneList(searchCurrentSceneInList(sceneName),highlithedSceneInt, "CurrentSceneChanged");
  }
  else if(strcmp(et, "SceneItemCreated") == 0 || strcmp(et, "SceneItemListReindexed") == 0)
  {
    const char* sceneName = eventData["sceneName"];

    // InputCreated
    if(strcmp(sceneName, scenesL[highlithedSceneInt]) == 0) //the even is for the highlithed scene(in the ctrl)
    {
      getSceneItemList(scenesL[highlithedSceneInt], "Event SceneItemCreated ListReindexed");
    }
  }
  else if(strcmp(et, "SceneItemEnableStateChanged") == 0 ||strcmp(et, "SceneItemLockStateChanged") == 0)
  {
     // displayed Scene?
    const char* sceneName = eventData["sceneName"];
    if(strcmp(sceneName, scenesL[highlithedSceneInt]) == 0){
      int sceneItemId = eventData["sceneItemId"];
      size_t i;
      int itemIndex;
      int updatedIndex; // used to redraw only icon updated later on
      int updatedField;// used to redraw only icon updated later on
      //update Scene Items State array
      for (i = 0; i < sourcesCount; i++)
      {        
        itemIndex = i;
        if (sourceItemsL[i][1] == sceneItemId )
        {
          if (strcmp(et, "SceneItemEnableStateChanged") == 0)
          {
            bool sceneItemEnabled = eventData["sceneItemEnabled"];
            sourceItemsL[i][4] = sceneItemEnabled ? 1 : 0;
            updatedField = 4;
          }
          else
          {
            bool sceneItemLocked = eventData["sceneItemLocked"];
            sourceItemsL[i][5] = sceneItemLocked ? 1 : 0;
            updatedField = 5;
          }   
          updatedIndex = i;
          break;     
        }
      }
      // update the changed row only
      if(!viewPopUp)
      { /// app state has changed and has to be reflected in the screen !!!!!!!!!!!!!!!
        displaySourcesList(highlithedSourceInt, true, sourceItemsL[i][3], "EVENT !viewPopUp");
      }
      else
      {
        displaySourcesList(highlithedSourceInt, true, sourceItemsL[i][3], "EVENT viewPopUp");
        drawPopUpMenu(updatedIndex, updatedField);
      }
    }
  }
  else if(strcmp(et, "InputVolumeChanged") == 0 )// here we update app state for that source then display list
  {
    // next include to update source list mic image if not in menu pop
    const char* inputName = eventData["inputName"];
    double level = eventData["inputVolumeDb"];

    for (size_t i = 0; i < sourcesCount; i++)
    {
      if(strcmp(inputName, sourceItemsNames[i]) == 0)
      {        
        sourceItemsL[i][7] = getIntAudioInputLevel(level); 
        // ASK IF CHANGES BELONGS TO THE higlithedItem >>> is popMenu
        if(i == highlithedSourceInt)
        {
          if(viewPopUp)
          { 
            drawPopUpMenu(i, 7);  // redraw input icon only 
            drawAudioInputState(393 - 25, 168, sourceItemsL[highlithedSourceInt][7]);                     
          }
          else
          {
            // nada
          }
        }
        else
        {
          //no popup menu present. Update mic icon if muted?
        }
        break;
      }
    }

  }
  else if (strcmp(et, "InputMuteStateChanged") == 0)
  {
  
  }
}

int searchCurrentSceneInList(const char* sceneName){
  for(int y=0; y<scenesCount; y++){
  //   Serial.println(sceneList[y]);
    if (strcmp(scenesL[y], sceneName) == 0){
      currentSceneInt = y; 
      return y;
    }     
  }
}

// display WIFI signal and recording 
void timerTasks(){
  unsigned long currentTime = millis();
   if ((currentTime - time_rssi >= INTERVAL_RSSI)) {
    check_and_print_WifiSignal();
    time_rssi = currentTime;
  }
  if(configuratorActive){return;}

  if ((currentTime - time_rec >= INTERVAL_REC) && recording) {
    displayRecIcon(lastRec);
    lastRec = !lastRec;
    time_rec = currentTime;
  }
}