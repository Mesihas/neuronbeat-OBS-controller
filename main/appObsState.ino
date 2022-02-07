void processSceneListFromJson(JsonObject responseData){
  // ORDENAR SCENES SAME AS ITEMS
  Serial.println("processSceneListFromJson");
  memset(scenesL, 0, sizeof(scenesL));
  currentProgramSceneName = responseData["currentProgramSceneName"];

  JsonArray scenes = responseData["scenes"];
  scenesCount = scenes.size();
  int idx = scenesCount -1; //Start to append from last item as the json message order is wrong

  for (JsonObject responseData_scenes_item : scenes) {
    const char* sceneName = responseData_scenes_item["sceneName"];
    scenesL[idx] = strdup(sceneName);
    // set currentSceneInt
    if (strcmp(sceneName, currentProgramSceneName) == 0)
    {
      currentSceneInt = idx;
    }   
    idx--;
  }
  if(highlithedSceneInt == -1){highlithedSceneInt = currentSceneInt;}  // set highlithedSceneInt first load

  scenePageCount = setPageCount(0);

  // Serial.println("---------------------------------------: ");
  // Serial.println("scenesCount");
  // Serial.println(String(scenesCount));
  // Serial.println("---------------------------------------: ");
  // Serial.println("currentSceneInt");
  // Serial.println(String(currentSceneInt));  
  // Serial.println("---------------------------------------: ");
  // Serial.println("currentProgramSceneName: ");
  // Serial.println(currentProgramSceneName );
  // Serial.println("-------------for loop con scenecount solo --------------------------: ");
  // for(int i=0;  i < scenesCount; i++){
  //   Serial.println(scenesL[i]);
  // }
  // Serial.println("---------------------------------------: ");
  scenesCurrentPage = getCurrentDisplayPage(0); // set this global first time and everytime passes
}

void processSceneItemListFromJson(JsonObject responseData){
 // Serial.println("processSceneItemListFromJson");

  memset(sourceItemsL, 0, sizeof(sourceItemsL));
  memset(sourceItemsNames, 0, sizeof(sourceItemsNames));
  JsonArray sourceItems = responseData["sceneItems"];
  sourcesCount = sourceItems.size();
  char str1[2]; 
  String tmp = "";
  int idx = 0;
  for (size_t i = sourcesCount; i --> 0; )
  {
    for (JsonObject responseData_sourceItems : sourceItems) {
      int d_responseData_sceneItem_sceneItemIndex = responseData_sourceItems["sceneItemIndex"]; // 0, 1, 2, 3, ..
      if (d_responseData_sceneItem_sceneItemIndex == i)
      {
        int d_responseData_sceneItem_sceneItemId = responseData_sourceItems["sceneItemId"]; // 2, 1, 3, 6, 5, 4
        // JsonVariantConst sourceItemNameVariant = responseData_sourceItems["sourceName"];
        // String responseData_sourceItemName = sourceItemNameVariant.as<String>();
        const char* sourceItemName = responseData_sourceItems["sourceName"];
        bool isGroup = responseData_sourceItems["isGroup"]; // true, null, null
        sourceItemsNames[idx] = strdup(sourceItemName);
        sourceItemsL[idx][0] = isGroup ? 1 : 0; 
        // tmp = String(d_responseData_sceneItem_sceneItemId);
        sourceItemsL[idx][1] = d_responseData_sceneItem_sceneItemId;
        //  tmp = String(d_responseData_sceneItem_sceneItemIndex);
        sourceItemsL[idx][2] = d_responseData_sceneItem_sceneItemIndex;
        sourceItemsL[idx][3] = idx;
        const char* input_kind = responseData_sourceItems["inputKind"];
        sourceItemsL[idx][6] = getSourceInputKind(input_kind, isGroup);
        idx++;
        break;
      }
    }  
  }
  sourcesPageCount = setPageCount(1);
  sourcesCurrentPage = getCurrentDisplayPage(1);
  lastCursorYsources = -1;// for sprite cursor starts from the begining
  highlithedSourceInt = 0;
  cleanSourcesCursorDisplayArea();

  // THis logs sources names, index, input_kind, is group
  // Serial.println("---------------------------------------: ");
  // for(int i=0;  i < sourcesCount; i++){
  //   Serial.print("Source Name NEW ARRAY: ");  
  //   Serial.print(sourceItemsNames[i]);
  //   Serial.print(" / "); 
  //   Serial.print("Source Name NADA: ");  
  //   Serial.print(sourceItemsL[i][0]);
  //   Serial.print(" / "); 
  //   Serial.print("sceneItemId: ");  
  //   Serial.print(String(sourceItemsL[i][1])); 
  //   Serial.print(" / "); 
  //   Serial.print("sceneItemIndex: ");  
  //   Serial.print(String(sourceItemsL[i][2])); 
  //   Serial.print(" / "); 
  //   Serial.print("sceneItemIndexREAL: ");  
  //   Serial.print(String(sourceItemsL[i][3]));     
  //   Serial.println("");    
  // }
  // Serial.println("---------------------------------------: ");
 
  // Serial.println("--------------------------------------: ");
  // for(int i=0;  i < scenesCount; i++){
  //   Serial.println(scenesL[i]);
  // }
  // Serial.println("-------------------------------------------: ");
}

void processItemsStateFromJson(JsonObject responseData){ 
//  Serial.println("processItemsStateFromJson");
  JsonArray results = responseData["results"];
  int resultsCount = results.size();
 // Serial.println("Items results Count x 2: " + String((int32_t)resultsCount));
 // Serial.println(resultsCount);
  int idx = 0;
  for (JsonObject responseData_results : results) {
    JsonVariantConst requestIdVariant = responseData_results["requestId"]; 
    String requestId = requestIdVariant.as<String>();
    int code = responseData_results["requestStatus"]["code"]; // 100
    bool requestStatus_result = responseData_results["requestStatus"]["result"];
    const char* requestType  = responseData_results["requestType"]; 
    int dash = requestId.lastIndexOf('-');
    String index = requestId.substring(0, dash);

    for (size_t i = 0; i < sourcesCount; i++)
    {
      if (String(sourceItemsL[i][3]) ==  index)
      {   
        if(strcmp(requestType, "GetSceneItemEnabled") == 0){
          bool sceneItemEnabled = responseData_results["responseData"]["sceneItemEnabled"];// true
          sourceItemsL[i][4] = (sceneItemEnabled ) ? 1 : 0;
          break;        
        }
        if(strcmp(requestType, "GetSceneItemLocked") == 0){
          bool sceneItemLocked = responseData_results["responseData"]["sceneItemLocked"];// true
          sourceItemsL[i][5] = (sceneItemLocked ) ? 1 : 0;
          break; 
        }
        if(sourceItemsL[i][6] == 7 || sourceItemsL[i][6] == 4){  
          double level = responseData_results["responseData"]["inputVolumeDb"];
          sourceItemsL[i][7] = getIntAudioInputLevel(level); 
        }
      }
    }
  }
// this serial logs processed sources list

  // Serial.println("--------------processItemsStateFromJson------------------");
  // for(int i=0;  i < sourcesCount; i++){
  //   Serial.print("IsGroup?: ");  
  //   Serial.print(String(sourceItemsL[i][0])); 
  //   Serial.print(" / ");    
  //   Serial.print("sceneItemId: ");  
  //   Serial.print(String(sourceItemsL[i][1])); 
  //   Serial.print(" / "); 
  //   Serial.print("sceneItemIndex: ");  
  //   Serial.print(String(sourceItemsL[i][2])); 
  //   Serial.print(" / "); 
  //   Serial.print("sceneItemIndexREAL: ");  
  //   Serial.print(String(sourceItemsL[i][3])); 
  //   Serial.print(" / "); 
  //   Serial.print("sceneItemEnabled: ");  
  //   Serial.print(String(sourceItemsL[i][4]));  
  //   Serial.print(" / "); 
  //   Serial.print("sceneItemLocked: ");  
  //   Serial.print(String(sourceItemsL[i][5])); 
  //   Serial.print(" / ");
  //   Serial.print("Source Name: ");  
  //   Serial.print(sourceItemsNames[i]);
  //   Serial.print(" / ");
  //   Serial.print("input_kind: ");  
  //   Serial.print(String(sourceItemsL[i][6]));
  //   Serial.print(" / ");
  //   Serial.print("Level: "); 
  //   Serial.print(String(sourceItemsL[i][7]));           
  //   Serial.println(""); 
  // }
  // Serial.println("----------- end processItemsStateFromJson ---------------");
}

int getIntAudioInputLevel(double audioLevel){
  int level;
  if(audioLevel <= -100){
    level = 0;
  } // no vol
  else if(audioLevel > -100 && audioLevel < -80){// green
  level = 1;
  } 
  else if(audioLevel > -80 && audioLevel < -65){  // green
    level = 2;
  } 
  else if(audioLevel >= -65 && audioLevel < -30){ // green
    level = 3;
  }
  else if(audioLevel >= -30 && audioLevel < -15){  // green
    level = 4;
  } 
  else if(audioLevel >= -15 && audioLevel < -8){  // green
    level = 5;
  }
  else if(audioLevel >= -8 && audioLevel < -5){ // green -yellow
    level = 6;  
  }
  else if(audioLevel >= -5 && audioLevel < -3){ // green -yellow
    level = 7;     
  } 
  else if(audioLevel >= -3 && audioLevel < -1.2){ //red - green -yellow
    level = 8;
  } 
  else if(audioLevel >= -1.2 && audioLevel < 0.1){ //red - green -yellow
    level = 9;
  } 

  Serial.println("----------- FIN getIntAudioInputLevel -- Level int : " + String(level));
  Serial.println("audioLevel: " + String(audioLevel));
  Serial.println("-----------------------------------------------------");
  return level;
}

int setPageCount(int mode){
  int count = 0;
  int pageCount = 0;
  if(mode == 0){  //scenesView
    count = scenesCount;
  }else{//sourcesView
    count = sourcesCount;    
  }    
  if(count <= 10){pageCount = 1;}
  else if (count >=11  && count <=20){pageCount = 2;}
  else if (count >=21  && count <=30){pageCount = 3;}
  else if (count >=31  && count <=40){pageCount = 4;}
  return pageCount;
}

int getPageFirstItemInt(int mode){
  int currentPage;
  int pageFirstItem;
  if(mode == 0){  //scenesView
    currentPage = scenesCurrentPage;
  }else if(mode ==1){
    currentPage = sourcesCurrentPage;
  }  
  switch (currentPage) // use global scenesCurrentPage value
  {
  case 1:
    pageFirstItem = 0;
    break;
  case 2:
    pageFirstItem = 10;
    break;
  case 3:
    pageFirstItem = 20;
    break;
  case 4:
    pageFirstItem = 30;
    break;

  default:
    break;
  }
  return pageFirstItem;
}

int getCurrentDisplayPage(int mode){
  int cp;
  int hlsi;
  if(mode == 0){  //scenesView
    hlsi = highlithedSceneInt;
  }else if(mode == 1){ //sourcesView
    hlsi =  highlithedSourceInt; 
  }       
  if(hlsi <= 9){cp = 1;}
  if(hlsi >= 10 && hlsi <= 19){cp = 2;}
  if(hlsi >= 20 && hlsi <= 29){cp = 3;}
  if(hlsi >= 30 ){cp = 4;}
  return cp;
}

int getLastPageItem(int mode){ // gets last item of the page to avoid drawing empty slots
  int itemsCount = 0;
  int currentPage = 0;
  if(mode == 0){  //scenesView
    // Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>getLastPageItem>>>> >>>>>>>>itemsCount >>>>>> :" + String(int(itemsCount)));
    itemsCount = scenesCount;
    currentPage = scenesCurrentPage;
  }else if(mode == 1){ //sourcesView
    itemsCount = sourcesCount;
    currentPage = sourcesCurrentPage;   
  } 

  if(itemsCount <= 9)
  {
    if(currentPage == 1){ return itemsCount -1; } 
  }

  if(itemsCount >=  10 && itemsCount <= 19)
  {
    if(currentPage == 1){ return 10-1; } //2 pages - displaying page 1. 9 -1 is for array starting on 0
    if(currentPage == 2){ return itemsCount - 1; } //2 pages - displaying page 2
  }

  if(itemsCount >= 20 && itemsCount <= 29)
  {
    if(currentPage == 1){ return 10-1; } //3 pages - displaying page 1
    if(currentPage == 2){ return 20-1; } //3 pages - displaying page 2
    if(currentPage == 3){ return itemsCount - 1; } //2 pages - displaying page 3  
  }

  if(itemsCount >= 30)
  {
    if(currentPage == 1){ return 10-1; } //4 pages - displaying page 1
    if(currentPage == 2){ return 20-1; } //4 pages - displaying page 2
    if(currentPage == 3){ return 30-1; } //4 pages - displaying page 3 
    if(currentPage == 4){ return itemsCount - 1; } //4 pages - displaying page 4
  }
}

int getSourceInputKind(const char* input_kind, bool isGroup){
  //  search input_kind
  if(input_kind == nullptr && isGroup)
  {
    return 11;
  }
  if (input_kind == nullptr && !isGroup){
    return -1;
  }else{
    for (int i = 0; i < 20; i++)
    {
        if (strcmp(input_kind, input_kinds[i]) == 0)
        {
          return i;             
        }
    }
  }
}

double getInputLevelFromEncoderMove(int level){
  double audioLevel;
  
  if(level == 0 ){
    audioLevel = -100;
  } // no vol
  else if(level == 1) {// green
  audioLevel = -85; //-100 && audioLevel < -80)
  } 
  else if(level == 2){ // green
    audioLevel = -70;// -80 && audioLevel < -65
  } 
  else if(level == 3){ // green
    audioLevel = -40;//;-65 && audioLevel < -30
  }
  else if(level == 4){  // green
    audioLevel = -20;//;-30 && audioLevel < -15
  } 
  else if(level == 5){  // green
    audioLevel = -12;//-15 && audioLevel < -8
  }
  else if(level== 6){ // green -yellow
    audioLevel = -6.5;//  -8 && audioLevel < -5
  }
  else if(level == 7 ){ // green -yellow
    audioLevel = -4; //    >= -5 && audioLevel < -3
  } 
  else if(level == 8){ //red - green -yellow
    audioLevel = -2;//>= -3 && audioLevel < -1.2
  } 
  else if(level == 9){ //red - green -yellow
    audioLevel = 0;//>= -1.2 && audioLevel < 0.1
  } 
  return audioLevel;

}

int getYpopupPosition(){
  //determines if the popUp will be positioned top half or botton half
  int row = getListDisplayRow(highlithedSourceInt);
  int y;
  //y adjustment
  //40 + 24
  if(row >= lsdcY + ldhi * 5){ 
    y = lsdcY + 2 + ldhi * 2 ; // y = 83px // menu goes up - sprite too. 
  }else{
    y = lsdcY + 2 + ldhi * 5 ; // menu goes down - sprite too. //162 //168
  }
  return y;
}