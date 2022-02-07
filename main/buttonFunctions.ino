void buttonEncoderClick() {
  Serial.println("Button encoder click.");
  if(viewScenes && !configuratorActive){ 
    setCurrentScene(scenesL[highlithedSceneInt]);
  }else if(viewSources && !configuratorActive && !viewPopUp){
      clickEncoderButtonTask = t.setTimeout(
          []() {                         
              Serial.println("buttonEncoderClick >> Sources popup mode >>> ENTRA");
              viewPopUp = true;
              startViewPopUp();
              listLogger();
          }, 50);   
  }else if(viewSources && !configuratorActive && viewPopUp){
    if(highlithedPopUpMenuInt == 3){
      clickEncoderButtonTask = t.setTimeout(
        []() {                         
          Serial.println("buttonEncoderClick >> popMenu mode >>> Back");
          viewPopUp = false;  
          stopViewPopup();
          displaySourcesList(highlithedSourceInt, false, 0, "buttonEncoderClick");
        }, 50);
    }else if(highlithedPopUpMenuInt == 2 && (sourceItemsL[highlithedSourceInt][6] == 7 || sourceItemsL[highlithedSourceInt][6] == 4)){
          // here the code for the set volumen usinf the encoder
      if(audioInputLevelMode)
      {
        audioInputLevelMode = false; 
        Serial.println("buttonEncoderClick >> popMenu mode >> audioInput > SALIR > audioInputLevelMode = false; ");
        return;
      }
      else
      {
        clickEncoderButtonTask = t.setTimeout(
          []() {                          
              Serial.println("buttonEncoderClick >> popMenu mode >>> entering > audioInputLevelMode");
              //entering > audio input level / encoder set to work in mode volume up/down
              viewPopUp = true;  
              audioInputLevelMode = true;
              audioInputLevel = sourceItemsL[highlithedSourceInt][7];
              listLogger();                     
        }, 50);
      }     
    
    }else if(highlithedPopUpMenuInt == 1){
          // here the code for toggle lock/unlock
      clickEncoderButtonTask = t.setTimeout(
      []() {                          
        Serial.println("buttonEncoderClick >> popMenu mode >>> entering > lock/unlock");
        //entering > set source visivility
        setSourceLockRequest(scenesL[highlithedSceneInt], sourceItemsL[highlithedSourceInt][1], sourceItemsL[highlithedSourceInt][5] == 0 ? true : false);
        listLogger();        
      }, 50);  
    }else if(highlithedPopUpMenuInt == 0){
          // here the code for toggle hide/unhide
      clickEncoderButtonTask = t.setTimeout(
      []() {                          
        Serial.println("buttonEncoderClick >> popMenu mode >>> entering > hide/unhide");
        //entering > set source visivility
        setSourceVisibilityRequest(scenesL[highlithedSceneInt], sourceItemsL[highlithedSourceInt][1], sourceItemsL[highlithedSourceInt][4] == 0 ? true : false);
        listLogger();       
      }, 50);     
    
    }
  }else if(configuratorActive){
    // ask which config step is current
    switch (currentConfigStep)
    {
    case -1:
          switch (highlithedNetMenuint)
          {
          case 0: 
            // currentConfigStep = 0; //GO to SSID Selection
            // scanNetworks();
            break;
          case 1:
            // currentConfigStep = 1;
            // keyboardInit();
            break;
          case 2:
            /* code */
            break;
          case 3:
            /* code */
            break;
                
          default:
            break;
          }
          break;     
    case 0:
      // if(highlithedSSIDint == 0){
      //   // SSID <<Back option selected- vuelve a menu network config
      //   createGUInetworkConfig();
      // }else{
      //   // SSID selected - Save and restart
      //   char charBuf[64];
      //   wifiNetworks[highlithedSSIDint].toCharArray(charBuf, 50);
      //   strlcpy(config.ssid,                  // <- destination
      //           charBuf,  // <- source
      //           sizeof(config.ssid));  
      //   printSelectedSSIDtoTFT();
      //   currentConfigStep = -1;

      //   saveWiFiCredentials();
      //   break; 
      // }
    case 1:    
      break;
    case 2: // Estoy en root
      switch (highlithedGenericMenuint)
          {
          case 0: // llamo Gral config
            tft.fillScreen(themeColor.background_color);
            currentConfigStep = 3;
            encoderSetZero();
            Serial.println("Enc botButton_Press >>> currentConfigStep = 3; ");
            displayGenericMenu(0, 0);         
            break;
          case 1: // llamo Network Config
            tft.fillScreen(themeColor.background_color);
            currentConfigStep = 4;
            encoderSetZero();
            Serial.println("Enc botButton_Press >>> currentConfigStep = 4; ");
            displayGenericMenu(1, 0);         
            break;
            
          case 2:
          if(saveCredentials){
            ESP.restart();
          }
            printMainGUI(true);
          
            break;
          case 3:
            /* code */
            break;
                
          default:
            break;
          }

      /* code */
      break;
    case 3: // Estoy en Gral config
      switch (highlithedGenericMenuint)
          {
          case 0: // Led Brigthness

            break;
          case 1: // button config
        
            break;
          case 2: // show ip 
              tft.setTextColor(TFT_BLACK);
              tft.drawString("Network Address:", 200, 100);
              tft.drawString("http://" + WiFi.localIP().toString(), 200, 120);
            break;
          case 3:
            tft.fillScreen(themeColor.background_color);
            currentConfigStep = 2;
            encoderSetZero();
            Serial.println("Enc botButton_Press >>> currentConfigStep = 2; ");
            displayGenericMenu(-1, 0);  
            break;
                
          default:
            break;
          }
        break;
    case 4: // Estoy en Network config
        switch (highlithedGenericMenuint)
          {
          case 0:  //press Network name
            currentConfigStep = 5;
            scanNetworks(); 
            break;
          case 1: //press network password
            keyboardCallback = 1;
            keyboardInit(3);       
            break;
          case 2: //press Host
            keyboardCallback = 2;
            keyboardInit(3);  
            break;
          case 3: // press Port
            keyboardCallback = 3;
            keyboardInit(1);  
            break;
          case 4: // Host Password
            keyboardCallback = 4;
            keyboardInit(3);  
            break;
          case 5:
            tft.fillScreen(themeColor.background_color);
            currentConfigStep = 2;
            encoderSetZero();
            Serial.println("Enc botButton_Press >>> currentConfigStep = 2; ");
            displayGenericMenu(-1, 0);  
            break;
          default:
            break;
          }
        break;
    case 5: // Estoy en SSID list
          if(highlithedGenericMenuint == 0){
            // SSID <<Back option selected- vuelve a menu network config
              tft.fillScreen(themeColor.background_color);
              currentConfigStep = 4;
              encoderSetZero();
              Serial.println("Enc botButton_Press >>> currentConfigStep = 4; ");
              displayGenericMenu(1, 0);  
          }else{
            // SSID selected - Save and restart
            char charBuf[64];
            wifiNetworks[highlithedGenericMenuint].toCharArray(charBuf, 50);
            strlcpy(config.ssid,                  // <- destination
                    charBuf,  // <- source
                    sizeof(config.ssid));             
            saveWiFiCredentials();
            currentConfigStep = 4;
            tft.fillScreen(themeColor.background_color);
            displayGenericMenu(1, 0);
            printSelectedSSIDtoTFT();
           // currentConfigStep = 4; para volver e imprimir el ssid en network name
            break; 
          }   
    default:
      break;
    } 
  }
}

void buttonEncoderDoubleclick() {
  Serial.println("buttonEncoderDoubleclick");
}

void buttonEncoderLongPress() {
// Serial.println("buttonEncoderLongPress >>>>>");
}

void buttonEncoderLongPressStart() {

  timeoutEncoderButtonLngPressStart = t.setTimeout(
    []() {                         
            encoderButLngPress = true;
            highlightPageMode(true);
            globalFadeInValue = 0;
            Serial.println("timeoutEncoderButtonLngPressStart  = t.setTimeout >>>>>");
          }, 50);      

}

void buttonEncoderLongPressStop() {

  timeoutEncoderButtonLngPressStart = t.setTimeout(
    []() {                         
            encoderButLngPress = false;
            //Serial.println("buttonEncoderLongPressStop 2>>>>>");  
            setPixel(2,0,0,0);
            showStrip();
            //Serial.println("buttonEncoderLongPressStop 3>>>>>");
            highlightPageMode(false);
            Serial.println("buttonEncoderLongPressStop 4>>>>>");
          }, 50);  

  
}

void botButton_Click(){
  getSceneList("Button");
  Serial.println("botButton_Click >> ");
}

void topButton_Click(){
  Serial.println("topButton_Click ");

}

void topButton_longPressStop(){
  ESP.restart();
}

void middButton_Click(){
  Serial.println("middButton_Click >> ");
}

void middButton_longPressStop() {
  Serial.println("middButton_longPressStop >> ");
}

void botButton_longPressStop() {
  tft.fillScreen(themeColor.background_color);
  configuratorActive = true;
   currentConfigStep = 2;
   encoderSetZero();
 // createGUIrootConfig();
  Serial.println("botButton_longPressStop");
  displayGenericMenu(-1, 0);
}

void encoderSetZero(){
  // used when displayn new menu
   encoderGenericLastValue = 0;
   encoder.setCount(0);
   highlithedGenericMenuint = 0;
}

void cursorSpriteImage(){

 // sprTest.setColorDepth(4);
  sprTest.loadFont(LATO_BLK16);
  sprTest.createSprite(160, 160);

  sprTest.setTextColor(TFT_BLACK, TFT_WHITE); 
  sprTest.fillSprite(TFT_WHITE);
  sprTest.pushImage(2, 2, 30, 30, (uint16_t *)mg_sources_state_visible);
  sprTest.drawString("dale", 10, 100 ); 
  sprTest.pushSprite(140, 5);
  sprTest.deleteSprite();
}
