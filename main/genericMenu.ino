void getEncoderMotionStop(){
  countOfEncoderEntries = 0; // reset var before counting
  for (size_t i = 0; i < 5; i++)
  {
    if(encoderTimeArray[i][1] > 0 )
    {
      countOfEncoderEntries++;
    }
    else
    {
      break;
    }
  }
  
  Serial.println("-------getEncoderMotionStop()  -----------------------------------");
  Serial.println("----countOfEncoderEntries " + String((int32_t)countOfEncoderEntries));

  for (size_t i = 0; i < countOfEncoderEntries ; i++)
  {
    Serial.println("----index>> : " + String((int32_t)i) + "----" + String((long)encoderTimeArray[i][1]));
  }
  Serial.println("------------------------------------------------------------------");  
}

// ############### MAIN ENCODER HANDLER #############################################################
void encoderHandlerNew(){
  int encoderCount = encoder.getCount();
  if(encoderLastValue == encoderCount){return;}
  int itemsCount;
  int highlithedItemInt;
  String who = "itemsCount is cero a esta altura";
  listLogger();

  // encoder motion detection eneabled if browsing scenes
  if(!encoderButLngPress && !viewPopUp){ 
    if(eat >= 5 ){
      eat = 0;
      memset(encoderTimeArray, 0, sizeof(encoderTimeArray));
      resetArrayEncoderMotion();
      countOfEncoderEntries = 0;
      Serial.println("encoderHandlerNew() eat reset____________>>>>>>: ");
    }

    encoderTimeArray[eat][0] = 1; //[direction] [millis] [0-scenes/1-sources/2-config]
    encoderTimeArray[eat][1] = millis();
    encoderTimeArray[eat][2] = 0;
    eat++;
    getEncoderMotionStop();
  }

  if(viewScenes)
  {
    itemsCount = scenesCount;
    highlithedItemInt = highlithedSceneInt;
    who = "viewScenes"; // logging purpose

    // ver bien esto-----------------------------------
    if(timeOutSetWSrequest){
      logEncoderGen("if(encoderHandlerNew())", " if(timeOutSetWSrequest)", highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);
      if(highlithedItemInt == 0  ) {// avoid cancelling timer if in the top limit of the list ||| encoderCount < 0  VER !! 
        Serial.println("avoid cancelling timer if in the top limit of the list");
      }else if(highlithedItemInt == itemsCount -1){ // 
        Serial.println("avoid cancelling timer if in the Botton limit of the list");
      }else{ 
        t.cancel(timeoutWSrequest); 
        timeOutSetWSrequest = false; 
        Serial.println("Timer Cancelled--->>>>>>>>>>>>>>>>>>>>>>>>>>>>");
      }
    }
    Serial.println("I AM PASSING  2");
    // -------------------------------------------
  }
  else if(viewSources && !viewPopUp)
  {
    itemsCount = sourcesCount;
    highlithedItemInt = highlithedSourceInt;
    who = "viewSources";
  }
  else if(viewSources && viewPopUp)
  {
    if(audioInputLevelMode){
      itemsCount = 10;
      who = "MenuPopUp-AudioInput"; 
       highlithedItemInt = audioInputLevel; 
    }else{
      itemsCount = 4;
      highlithedItemInt = highlithedPopUpMenuInt;
      who = "MenuPopUp-ROOT";  
    }

    // switch (highlithedPopUpMenuInt)
    // {
    // case 0:
    //  // highlithedItemInt = highlithedPopUpMenuInt;
    //   break;
    // case 1:

    //   break;
    // case 3:
    //   itemsCount = 4;
    //   highlithedItemInt = highlithedPopUpMenuInt;
    //   who = "MenuPopUp-ROOT";  
    //   Serial.println("case 3:>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    //   break;
    // case 2:
    //   itemsCount = 9;
    //   who = "MenuPopUp-AudioInput"; 
    //    highlithedItemInt = audioInputLevel; 
    //   break;      
    // default:
    //   break;
    // }
  }
  //else if (viewConfig){}
  ///////////////////
  int itemsFwd;
//  logEncoderGen("Encoder HAY MOV DE:   ",  who, highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);

  if(encoderLastValue > encoderCount){// ENCODER ARRIBA  ////////////////////////////////////////#########################################
    //if(highlithedItemInt <= 0 && encoderLastValue <= 0 && encoderCount < 0) {
    if(highlithedItemInt <= 0) {
     // logEncoderGen("Encoder ESCAPE Top limit",  who, highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);
      encoder.setCount(0);
      encoderCount = encoder.getCount();
      encoderLastValue = encoderCount;
      if(viewScenes){highlithedSceneInt = 0; displaySceneList(currentSceneInt, highlithedItemInt, "Encoder ESCAPE Top limit");}
      Serial.println("I AM PASSING Encoder ESCAPE Top limit");
      return;
    }else if(encoderButLngPress && viewScenes && scenesCurrentPage == 1 ){ // Escape scenes page top limit
      Serial.println("I AM PASSING Encoder ESCAPE PAGE Top limit");//,  who, highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);
      return;
    }else if(encoderButLngPress && viewSources && sourcesCurrentPage == 1 ){ // Escape sources page top limit
      Serial.println("I AM PASSING Encoder SOURCES PAGE Top limit");//,  who, highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);
      return;
    }else if(viewSources && viewPopUp){// Escape popup left limit
      Serial.println("I AM PASSING  Encoder SOURCES PopUp Menu Left limit");//,  who, highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);
      // return;
    }
    // NORMAL MOVE   ///////////////////////////////////////////////////////////////
    
    encoderLastValue = encoderCount;
    //logEncoderGen("Movimiento normal Izquierda", who, highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);

    if(viewScenes){
      if(encoderButLngPress){ // navigating scenes page mode up
        scenesCurrentPage--;
        highlithedItemInt = getPageFirstItemInt(0); // set cursor 1st item of the page based on scenesCurrentPage
        highlithedSceneInt = highlithedItemInt;//need to set global as AsyncTimer does not support In lambda captured
        logEncoderGen("UP viewScenes ",  "encoderButLngPress", highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);   
      }else{// navigating scenes normal mode up
        highlithedItemInt--; 
        highlithedSceneInt = highlithedItemInt;//need to set global as AsyncTimer does not support In lambda captured
        scenesCurrentPage = getCurrentDisplayPage(0); // need to set the current page in case it has changed
      }
    //   Serial.println("getSceneItemList() after 200 Millisseconds ARRIBA ----->>>>>>>>>>");
      timeoutWSrequest = t.setTimeout(
      []() {            
             // displaySceneList(currentSceneInt, highlithedSceneInt, "ENCODER ARRIBA timeOutSetWSrequest");
              getSceneItemList(scenesL[highlithedSceneInt], "ENCODER UP timeout");
            }, 250);
      timeOutSetWSrequest = true;
      displaySceneList(currentSceneInt, highlithedItemInt, "ENCODER UP");  
     Serial.println("I AM PASSING   4");  
    }else if(viewSources && !viewPopUp){//encoder mode Sources
      if(encoderButLngPress){// navigating sources page mode up
        sourcesCurrentPage--;
        highlithedItemInt = getPageFirstItemInt(1); // set cursor 1st item of the page
        highlithedSourceInt = highlithedItemInt;//need to set global ????
        logEncoderGen("ABAJO viewSources ",  "encoderButLngPress", highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);  
      }else{// navigating sources normal mode up
        highlithedItemInt--; 
        highlithedSourceInt = highlithedItemInt;
        sourcesCurrentPage = getCurrentDisplayPage(1);
      }
      logEncoderGen("Movimiento normal Izquierda",  "viewSources", highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);
      displaySourcesList(highlithedSourceInt, false, 0, "ENCODER UP");  
      Serial.println("I AM PASSING   5 Encoder UP");  
    }else if(viewSources && viewPopUp){ //encoder mode Sources menu popup
        
      if(audioInputLevelMode){
        highlithedItemInt--;
        listLogger();
     
        setAudioInputLevelRequest(sourceItemsNames[highlithedSourceInt], getInputLevelFromEncoderMove(highlithedItemInt));
        audioInputLevel = highlithedItemInt;
      }else{
        highlithedItemInt--;
        highlithedPopUpMenuInt = highlithedItemInt;
        cleanAndPrintCursor(1);
      }
        // switch (highlithedPopUpMenuInt)
        // {
        // case 0:
        //   Serial.println("I AM PASSING case  0 ARRIBA");
        //   break;
        // case 1:
        //   Serial.println("I AM PASSING case  1 ARRIBA");
        //   break;
        // case 2: //AudioInput
        //   Serial.println("I AM PASSING case  2 ARRIBA");
        //   // const char* inputName = sourceItemsNames[highlithedSourceInt];
        //   // double inputVolumeDb = getInputLevelFromEncoderMove(sourceItemsL[highlithedSourceInt][7]);
        //   setAudioInputLevelRequest(sourceItemsNames[highlithedSourceInt], getInputLevelFromEncoderMove(highlithedItemInt));
        //   audioInputLevel = highlithedItemInt;
        //   //drawAudioInputState(393,168,sourceItemsL[highlithedSourceInt][7])??
        //   break; 
        // case 3:
        //   Serial.println("I AM PASSING case  3 ARRIBA");
        //   highlithedPopUpMenuInt = highlithedItemInt;
        //   cleanAndPrintCursor(1);
        //   break;
     
        // default:
        //   break;
        // } 
    }
  } 
  
  if(encoderLastValue < encoderCount){ // ENCODER ABAJO  /////////////////////////////////////////////#######################################
    if(highlithedItemInt>=itemsCount -1){  // set highlithedSceneInt last item count -1 (botton of the list)
      highlithedItemInt = itemsCount -1; // set highlithedSceneInt last item count -1 (botton of the list)
      if(viewScenes){highlithedSceneInt = highlithedItemInt;} // set highlithedSceneInt last item count -1 (botton of the list)
      if(viewSources && !viewPopUp){highlithedSourceInt = highlithedItemInt;}// set highlithedSourceInt last item count -1 (botton of the list)
      if(viewSources && viewPopUp && !audioInputLevelMode){highlithedPopUpMenuInt = highlithedItemInt;}// set highlithedPopUpMenuInt last item count -1 (right of the list)
      encoder.setCount(itemsCount-1); // set encoder.setCount last item count -1 (botton of the list)
      encoderCount = encoder.getCount();
      encoderLastValue = encoderCount;
      if(viewScenes){displaySceneList(currentSceneInt, highlithedItemInt, "Encoder ESCAPE Botton limit");} // ??? ver /// ??????????
      //   logEncoderGen("Encoder ESCAPE Botton limit",  who, highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);
      Serial.println("Encoder ESCAPE Botton limit 1 ");
      return;
    }else if(encoderButLngPress && viewScenes && scenesCurrentPage == scenePageCount){
      //     logEncoderGen("Encoder ESCAPE PAGE Botton limit",  who, highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);
        Serial.println("Encoder ESCAPE SCENES PAGE Botton limit 2 ");
      return;
    }else if(encoderButLngPress && viewSources && sourcesCurrentPage == sourcesPageCount ){ // Escape sources page top limit
  //    logEncoderGen("Encoder SOURCES PAGE Botton limit",  who, highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);
      Serial.println("Encoder SOURCES PAGE Botton limit 3 ");
      return;
    }else if(viewSources && viewPopUp){// Escape popup left limit
      // return;
    }
    // NORMAL MOVE   //////////////////////////////////////////////////////////////////////
    encoderLastValue = encoderCount;
    if(viewScenes){
      if(encoderButLngPress){ // page move 
        scenesCurrentPage++;
        highlithedItemInt = getPageFirstItemInt(0); // set cursor 1st item of the page
        highlithedSceneInt = highlithedItemInt;//need to set global as AsyncTimer does not support In lambda captured 
        logEncoderGen("ENCODER DOWN viewScenes ",  "encoderButLngPress", highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);   
      }else {    
        highlithedItemInt++;
        highlithedSceneInt = highlithedItemInt;//need to set global as AsyncTimer does not support In lambda captured
        scenesCurrentPage = getCurrentDisplayPage(0);
        logEncoderGen("ENCODER DOWN viewScenes ",  " NOT encoderButLngPress", highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true); 
      }
      // Serial.println("getSceneItemList() after 200 Millisseconds ABAJO ----->>>>>>>>>>");
      timeoutWSrequest = t.setTimeout(
      []() {       
           //   displaySceneList(currentSceneInt, highlithedSceneInt, "ENCODER ABAJO timeOutSetWSrequest");
              getSceneItemList(scenesL[highlithedSceneInt], "ENCODER DOWN timeout");           
            }, 250);   
      timeOutSetWSrequest = true;  
      displaySceneList(currentSceneInt, highlithedItemInt, "ENCODER DOWN");   

    }
    else if(viewSources && !viewPopUp){ //encoder mode Sources
      if(encoderButLngPress){ // page move 
        sourcesCurrentPage++;
        highlithedItemInt = getPageFirstItemInt(1); // set cursor 1st item of the page
        highlithedSourceInt = highlithedItemInt;//need to set global ????
        logEncoderGen("ENCODER DOWN viewSources ",  "encoderButLngPress", highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);   
      }else{
        highlithedItemInt++;
        highlithedSourceInt = highlithedItemInt;
        sourcesCurrentPage = getCurrentDisplayPage(1);
      }
      logEncoderGen("Normal movement down",  "viewSources", highlithedItemInt, itemsCount, encoderCount, encoderLastValue, true);
      displaySourcesList(highlithedSourceInt, false, 0, "ENCODER DOWN");   
    }else if(viewSources && viewPopUp){//encoder mode Sources menu popup

      if(audioInputLevelMode){
        highlithedItemInt++; 
        listLogger();     
        setAudioInputLevelRequest(sourceItemsNames[highlithedSourceInt], getInputLevelFromEncoderMove(highlithedItemInt));
        audioInputLevel = highlithedItemInt;
      }else{
        highlithedItemInt++; 
        highlithedPopUpMenuInt = highlithedItemInt;
        cleanAndPrintCursor(1);
      }

      // switch (highlithedPopUpMenuInt)
      // {
      // case 0:
      //   Serial.println("I AM PASSING case  0 ABAJO");
      //   break;
      // case 1:
      //     Serial.println("I AM PASSING case  1 ABAJO");
      //   break;
      // case 2: //AudioInput

      //   //drawAudioInputState(393,168,sourceItemsL[highlithedSourceInt][7])??
      //   break; 
      // case 3:

      //   break;
    
      // default:
      //   break;
      // } 
    }
  }
}   

// ############### This handles encoder while in configuration menu ###################################### 
// This function will be integrated to MAIN ENCODER HANDLER
void genericEncoderHandler(int menu, int menuItems){
  int encoderCount = encoder.getCount();

  if(encoderGenericLastValue == encoderCount){
    return;
  }

  //////%%%%%%%%%%%%%%%%%%%%%%%% SSID LIST ########################################
  if(encoderGenericLastValue < encoderCount){
    //// DOUBLE CHECK ENCODER DIRECTION ////////////////////////////////////////////////////
    encoderDirection = 1;
    // if(!checkEncoderDirection(encoderDirection)){return;}
    //END DOUBLE CHECK ENCODER DIRECTION ///////////////////////////////////////////////////

    // FIN Double check direction //////////////////////////////////////////////////////
    if(highlithedGenericMenuint >= menuItems - 1){
      highlithedGenericMenuint = menuItems  -1; 
      encoderGenericLastValue = highlithedGenericMenuint;
      encoder.setCount(highlithedGenericMenuint);
      Serial.println("------------- Escape Generic Encoder Botton limit-------------------");
      // Serial.println("menuCount Count: " + String((int32_t)menuCount));
      // Serial.println("highlithedNetMenuint: " + String((int32_t)highlithedNetMenuint));
      // Serial.println("-------------------------------------------------------");
      return;
    }

    // NORMAL MOVE   //////////////////////////////////////////////////////////////////////
    encoderGenericLastValue = encoderCount;
    highlithedGenericMenuint = highlithedGenericMenuint + 1;
    encoderLastDirection = encoderDirection;
    cfgLogger("genericEncoderHandler", "DOWN");
    // Serial.println("----------------------- Movimiento SSID normal DERECHA -------------------");
    // Serial.println("highlithedNetMenuint: " + String((int32_t)highlithedNetMenuint));
    displayGenericMenu(menu, highlithedGenericMenuint);
  }

  if(encoderGenericLastValue > encoderCount){
    //// DOUBLE CHECK ENCODER DIRECTION ////////////////////////////////////////////////////
    encoderDirection = 0;
  //   if(!checkEncoderDirection(encoderDirection)){return;}
    //END DOUBLE CHECK ENCODER DIRECTION ///////////////////////////////////////////////////
    if(highlithedGenericMenuint < 0) {
      Serial.println("------------- Escapes generic Encoder Top limit -------------------");
      // Serial.println("menuCount: " + String((int32_t)menuCount));
      // Serial.println("highlithedNetMenuint: " + String((int32_t)highlithedNetMenuint));
      // Serial.println("-------------------------------------------------------");
      encoder.setCount(0); 
      highlithedGenericMenuint = 0; 
      return;
    }
    // NORMAL MOVE   //////////////////////////////////////////////////////////////////////
    encoderGenericLastValue = encoderCount;
    if(highlithedGenericMenuint != 0){ highlithedGenericMenuint = highlithedGenericMenuint - 1;}
    encoderLastDirection = encoderDirection;
    cfgLogger("genericEncoderHandler", "Encoder UP");
  //  Serial.println("----------------------- Movimiento normal SSID IZQUIERDA -------------------");
  //  Serial.println("highlithedNetMenuint: " + String((int32_t)highlithedNetMenuint));
      // Ejecutar aqui el codigo correspondiente
    displayGenericMenu(menu, highlithedGenericMenuint);
  } 
}

void displayGenericMenu(int menu, int hglItem){
  if(menu == -10){displaySSIDList(hglItem); return;}
//  deserializeJson(docMenu, dataMenu);
 //docMenu.clear();
 char dataMenu[] = "{\"name\":\"root\",\"id\":\"1\",\"items\":[{\"name\":\"General Config\",\"id\":\"1.1\",\"items\":[{\"id\":\"1.1.1\",\"name\":\"Led Brigthness\",\"render\":false},{\"id\":\"1.1.2\",\"name\":\"Button config\",\"render\":true},{\"id\":\"1.1.3\",\"name\":\"Show IP\",\"render\":true},{\"id\":\"1.1.4\",\"name\":\"<--Back \",\"render\":false}]},{\"name\":\"Network Config\",\"id\":\"1.2\",\"items\":[{\"id\":\"1.2.1\",\"name\":\"Network name\",\"render\":false},{\"id\":\"1.2.2\",\"name\":\"Network password\",\"render\":true},{\"id\":\"1.2.3\",\"name\":\"Host\",\"render\":true},{\"id\":\"1.2.4\",\"name\":\"Port\",\"render\":true},{\"id\":\"1.2.5\",\"name\":\"Host password\",\"render\":false},{\"id\":\"1.2.6\",\"name\":\"<--Back \",\"render\":false}]},{\"name\":\"<--Back\",\"id\":\"1.3\",\"items\":[]}]}";

  DeserializationError err = deserializeJson(docMenu, dataMenu);
    // Parse succeeded?
  if (err) {
    Serial.print(F("deserializeJson() returned "));
    Serial.println(err.c_str());
    return;
  }

  int memoryUsed = docMenu.memoryUsage();

  JsonArray items = docMenu["items"];
  String menuNameX;
  JsonArray current_menu_items;
  int current_menu_items_count;
  
  if(menu == -1){ //-1 root
    String menuName = docMenu["name"];
    menuNameX = menuName;
    current_menu_items_count = items.size();
    current_menu_items = items;
    Serial.println("------------- menu == -1  -------------------");
  // }else if(menu == -10){

  }else{
    String menuName = items[menu]["name"];
    current_menu_items = items[menu]["items"];
    current_menu_items_count = items[menu].size();
    menuNameX = menuName;   
  }

  Serial.println("------------- displayGenericMenu -------------------");
/// print title
  tft.setTextSize(2);
  tft.setFreeFont(LABEL1_FONT);
  tft.setTextColor(TFT_BLACK);
  String menuNameAndfirmWVer = menuNameX + " - Firmware Ver. " + firmwareVersion;
  tft.drawString(menuNameAndfirmWVer, 0, 0);
///

  int y = 50;
  int x = 20;
  int iniWindow = 0;
  int finWindow = current_menu_items_count -1 ;

  tft.setTextSize(1);
  int  e = 0;

  for (JsonObject repo : current_menu_items) {
    const char* menuItem = repo["name"];   
   // Serial.println(name);

    if(hglItem == e){
      tft.setTextColor(TFT_RED);
    }else{
      tft.setTextColor(TFT_BLACK);
    }

    tft.drawString(menuItem, x, y);
    y = y + 30;
    e = e + 1;
  }
  tft.setTextColor(TFT_BLACK);
  if(menu == 1){ printAllNetCfgValuesToTFT(); } //if menu net cfg display values
}

void startViewPopUp(){
 // viewPopUp = true;

  // get inputkind
 int input_kind = sourceItemsL[highlithedSourceInt][6];

 switch (input_kind)
  {
  case 0:

    break;
  case 1:

    break;
  case 2:

    break;
  case 3:

    break;
  case 4:
    //wasapi_output_capture
    // show popup
    // Hide | on-off | level | Back
    break;
  case 5:

    break;
  case 6:

    break;
  case 7:
    // wasapi_input_capture 
    // show popup
    // Hide | on-off | level | Back
    
    break;
  case 8:

    break;
  case 9:

    break;
  case 10:

    break;
  case 11:

    break;                          
  default:
    break;
  }
  // get menu options
  drawPopUpMenu(-1, -1);
  // print options
  //handdle nav

}