void cfgLogger(String who, String info){

  Serial.println("-----------------------" +  who  + "-------------------");
  Serial.println("Moe info: " + info);
  Serial.println("highlithedGenericMenuint: " + String((int32_t)highlithedGenericMenuint));
  Serial.println("configuratorActive: " + String((bool)configuratorActive));
  Serial.println("currentConfigStep: " + String((int32_t)currentConfigStep));
  Serial.println("keyboardCallback : " + String((int32_t)keyboardCallback));
  Serial.println("keyboardActive : " + String((int32_t)keyboardActive)); 
  
  Serial.println("------------------------------------------------------------------");
}


int ssidCount = 11; //includes [0] option Back
int menuCount = 4; //sizeof(netConfigSteps) did not work;
bool updatedWSP =  false;

//########### Configuration active ? > task   ###################################################################;?
void isConfiguratorActive(){
  if (configuratorActive ){

    server.handleClient();
    // tft.drawString("currentConfigStep: ", x, 0 );
    // tft.drawString(String(currentConfigStep), x, 20);

    switch (currentConfigStep)
    {
    case -1:
      //configNetworkMenuEncoderHandler();
      configNetworkMenutouchHandler();
      break;     
    case 0: // old control of encoder ssid list
    //  configNetworkSSIDEncoderHandler();
      break;   
    case 1:   
      //keyboardHandler(); 
  //    Serial.println(" keyboardHandler(); :currentConfigStep  "+ String((int32_t)currentConfigStep));
      break;
    case 2: // Show root
      genericEncoderHandler(-1, 3);
  //   Serial.println("genericEncoderHandler(-1, 3);:currentConfigStep  "+ String((int32_t)currentConfigStep));
      /* code */
      break;
    case 3:  // Show Gral config
      genericEncoderHandler(0, 4);
 //     Serial.println("genericEncoderHandler(0, 4);:currentConfigStep  "+ String((int32_t)currentConfigStep));
      break;       
    case 4:  // Show Network config
      genericEncoderHandler(1, 6);
 //     Serial.println("genericEncoderHandler(1, 6):currentConfigStep  "+ String((int32_t)currentConfigStep));
        switch (keyboardCallback)
        {
        case 0:  //press Network name
          break;
        case 1: //press network password
          keyboardHandler();      
          break;
        case 2: //press Host
          keyboardHandler(); 
          break;
        case 3: // press Port
          keyboardHandler(); 
          break;
        case 4: // Host Password
          keyboardHandler(); 
          break;
        default:
          break;
        }
      break; 
    case 5: // encoder SSID
      genericEncoderHandler(-10, 11);               
    default:
      break;
    }  
  }else {
    checkTouch();
   // encoderHandler();
    encoderHandlerNew();
    timerTasks();
  }
}

// ##########################################################################

void scanNetworks(){
 // cleanSSIDlist();
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Scanning...", 10, 120);
  memset(wifiNetworks, 0, sizeof(wifiNetworks));
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i <= 9; ++i) {
      Serial.print(i);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");

      wifiNetworks[i+1] = WiFi.SSID(i); // leave first slot for Back option...
    }
    Serial.print("SSID count" + String((int32_t)ssidCount));
   wifiNetworks[0] = "  << Back";
   displaySSIDList(0);
  }
}

void displaySSIDList(int highlithedSSIDint){
  cleanSSIDlist();

  int y = 10;
  int x = 10;
  int iniWindow = 0;
  int finWindow = 10;
  tft.drawRoundRect(x-2, y - 5, 150, 27, 5, TFT_WHITE); //draws big rect on Scenes
// tft.fillRect(3, 34, 215, 250, TFT_BLACK); CLEAR AREA
  for(int e=iniWindow; e<=finWindow; e++){
    tft.setTextSize(1);
    tft.setFreeFont(LABEL3_FONT);
    String ssid = wifiNetworks[e].substring(0, 32); 
    if(highlithedSSIDint == e){
      //tft.setTextSize(1);
      tft.setFreeFont(LABEL1_FONT);
      tft.setTextColor(themeColor.ssid_highlithed_color);
    }else{
      tft.setTextColor(themeColor.ssid_color);
    }
    if(e == 0 && highlithedSSIDint != 0){ 
      tft.setTextSize(1);
      tft.setFreeFont(LABEL3_FONT);
      tft.setTextColor(TFT_WHITE);    
    }else if(e == 0 && highlithedSSIDint == 0){
      tft.setTextSize(1);
      tft.setFreeFont(LABEL1_FONT);
      tft.setTextColor(TFT_WHITE); 
    }

    tft.drawString(ssid, x, y);
    y = y + 27;
  }
}

void saveWiFiCredentials(){
  if (!SPIFFS.begin()) {
 //   Serial.println("Formating file system");
 //   SPIFFS.format();
    SPIFFS.begin();
  }
  DynamicJsonDocument doc(2048);
  
 // uint16_t websockets_server_port = 4444; // Enter server port
  char charBuf[64];
  String aux = "";
  doc["ssid"] = config.ssid;
  doc["password"] = config.password;
  doc["websockets_server_host"] = config.websockets_server_host;
  //doc["websockets_server_port"] = config.websockets_server_port;
  // check if Web Socket port has changed then update the value
  if(updatedWSP){
    doc["websockets_server_port"] = config.wsp;
  }else{
    doc["websockets_server_port"] = config.websockets_server_port;
  }

  doc["websockets_password"] = config.websockets_password;

  File f = SPIFFS.open(CREDENTIALS_FILE, "w");
  if (f) {
    // Produce a minified JSON document
    // Serialize JSON to file
    if (serializeJson(doc, f) == 0) {
      Serial.println(F("Failed to write to file"));
    }
    // Close the file
    f.close();
  }
  saveCredentials = true;
 // ESP.restart();
}

void configNetworkMenuEncoderHandler(){// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

}

void configNetworkMenuListDisplay(int highlithedNetMenuint){  // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

}

void configNetworkMenutouchHandler(){
  uint16_t x, y, z;
  z = tft.getTouchRawZ();

  if(z > 500){
    Serial.println("z int: ");
    Serial.println(z);
    tft.getTouchRaw(&x, &y);
    Serial.printf("x: %i     ", x);
    Serial.printf("y: %i     ", y);
    Serial.printf("z: %i \n", tft.getTouchRawZ());
    delay(250);

    if(x > 3300  && x < 3800 && y >2200 && y <2900) {
      Serial.println("Cancel papa");
      printMainGUI(true);
    }
    else if(x > 3300  && x < 3800 && y >3100 && y <3900) {
      Serial.println("Save papa");
      printMainGUI(true);
    }

  }
}

bool readWiFiCredentials()
{
  // check file system exists
  if (!SPIFFS.begin()) {
 //   Serial.println("Formating file system");
 //   SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CREDENTIALS_FILE)) {
    Serial.println("CREDENTIALS_FILE FILE EXSTS");
    File file = SPIFFS.open(CREDENTIALS_FILE, "r");

    StaticJsonDocument<512> doc;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    Serial.println(F("Failed to read file, using default configuration"));
      // Copy values from the JsonDocument to the Config
   // config.websockets_server_port = doc["websockets_server_port"];
    strlcpy(config.ssid,                  // <- destination
            doc["ssid"],  // <- source
            sizeof(config.ssid));         // <- destination's capacity
    strlcpy(config.password,                  // <- destination
            doc["password"],  // <- source
            sizeof(config.password));  
    strlcpy(config.websockets_server_host,                  // <- destination
          doc["websockets_server_host"],  // <- source
          sizeof(config.websockets_server_host));    

    String wsp_aux = doc["websockets_server_port"];
    JsonVariant wsPort = doc["websockets_server_port"];
    Serial.println("String wsp_aux");
    Serial.println(wsp_aux);
    unsigned int wsPortUint = wsPort.as<unsigned int>();
    Serial.println("unsigned int wsPortUint");
    Serial.println(wsPortUint);

    if(isValidNumber(wsp_aux)){
      config.websockets_server_port = wsPortUint;
      Serial.println("Valid");
    }else{
      Serial.println("Not Valid port");
      config.websockets_server_port = 4444;
    }

    strlcpy(config.websockets_password,                  // <- destination
          doc["websockets_password"],  // <- source
          sizeof(config.websockets_password));    


    // Close the file (Curiously, File's destructor doesn't close the file)
    file.close();
     
    Serial.println("--------Print Credentials----------------------------");
    Serial.println("SSID:");
    Serial.println(config.ssid);
    Serial.println("Password:");
    Serial.println("**********************");
    Serial.println("Websockets_server_host:");
    Serial.println(config.websockets_server_host);
    Serial.println("Websockets_server_Port:");
    Serial.println(config.websockets_server_port);
    Serial.println("Websockets_Password:");
    Serial.println("**********************");
    Serial.println("-----------------------------------------------------");
     
  }else{
    return false;
  }
}

boolean isValidNumber(String str){
  for(byte i=0;i<str.length();i++)
  {
  if(isDigit(str.charAt(i))) return true;
  }
  return false;
}