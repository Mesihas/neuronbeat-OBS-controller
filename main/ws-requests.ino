void SendRequest(const char* req){
  if(currentSceneCollectionChanging){return;}
  webSocket.sendTXT(req);
}

void getSceneList(String requester){
  StaticJsonDocument<192> doc;
  doc["op"] = 6;
  JsonObject d = doc.createNestedObject("d");
  d["requestType"] = "GetSceneList";
  if(requester == "Event handler"){
    d["requestId"] = "001-Event";
  }else{
    d["requestId"] = "001";   
  }

  char output[192];
  serializeJson(doc, output);
  webSocket.sendTXT(output); 
}

void getCurrentScene(){
  StaticJsonDocument<192> doc;
  doc["op"] = 6;
  JsonObject d = doc.createNestedObject("d");
  d["requestType"] = "GetCurrentProgramScene";
  d["requestId"] = "003";
  char output[192];
  serializeJson(doc, output);
  SendRequest(output);  
}

void setCurrentScene(const char* sceneName){
  StaticJsonDocument<192> doc;
  doc["op"] = 6;
  JsonObject d = doc.createNestedObject("d");
  d["requestType"] = "SetCurrentProgramScene";
  d["requestId"] = "002";
  d["requestData"]["sceneName"] = sceneName;
  char output[192];
  serializeJson(doc, output);
  SendRequest(output);  
}

void getSceneItemList(const char* sceneName, String requester){
  if(movingEncoder){
    Serial.println("getSceneItemList WS REQUEST  movingEncoder true CHAU-->>>> -------");
    return;
  }
  //reset encoder timer and avoid requesting same Scane Items multiple times
  if(timeOutSetWSrequest && strcmp(asyncTimerLastSceneNameReq, sceneName) == 0){
    timeOutSetWSrequest = false;
    asyncTimerLastSceneNameReq = strdup(sceneName);
    Serial.println("getSceneItemList timeOutSetWSrequest  CHAU-->>>> -------");
    return;
  }
  timeOutSetWSrequest = false;
  asyncTimerLastSceneNameReq = strdup(sceneName);
  //------------------------------------------------------------------------
  StaticJsonDocument<192> doc;
  doc["op"] = 6;
  JsonObject d = doc.createNestedObject("d");
  d["requestType"] = "GetSceneItemList";
  d["requestId"] = "002";
  d["requestData"]["sceneName"] = sceneName;
  char output[192];
  serializeJson(doc, output);
  Serial.println("getSceneItemList REQUEST -->>>> " + requester + "--------");
  // Serial.println(output);
  SendRequest(output);  
}

void getItemsStateRequest(int highlithedSceneInt){
 // Serial.println("GetItemsStateRequest");
  DynamicJsonDocument docItems(15360);
  const char* sceneName = scenesL[highlithedSceneInt];
  docItems["op"] = 8;
  JsonObject d = docItems.createNestedObject("d");
  d["requestId"] = "GetItemsStateRequest-01";
  d["haltOnFailure"] = true;

  JsonArray d_requests = d.createNestedArray("requests");

  // Serial.println("sourcesCount: " + String((int32_t)sourcesCount));
  // Serial.print("sceneName: ");
  // Serial.println(scenesL[0]);


  // request GetSceneItemEnabled 
  for (size_t i = 0; i < sourcesCount; i++)
  {
    String conc = String(sourceItemsL[i][3]) + "-" + sourceItemsNames[i];
    JsonObject d_requests_2 = d_requests.createNestedObject();
    d_requests_2["requestType"] = "GetSceneItemEnabled";
    d_requests_2["requestId"] = conc;
    d_requests_2["requestData"]["sceneName"] = sceneName;
    d_requests_2["requestData"]["sceneItemId"] = sourceItemsL[i][1];
  }
  // request GetSceneItemLocked
   for (size_t i = 0; i < sourcesCount; i++)
  {
    String conc = String(sourceItemsL[i][3]) + "-" + sourceItemsNames[i];

    JsonObject d_requests_2 = d_requests.createNestedObject();
    d_requests_2["requestType"] = "GetSceneItemLocked";
    d_requests_2["requestId"] = conc;
    d_requests_2["requestData"]["sceneName"] = sceneName;
    d_requests_2["requestData"]["sceneItemId"] = sourceItemsL[i][1];
  }
  //if sound input get audio level
  for (size_t i = 0; i < sourcesCount; i++)
  {
    if(sourceItemsL[i][6] == 7 || sourceItemsL[i][6] == 4){
      String conc = String(sourceItemsL[i][3]) + "-" + sourceItemsNames[i];

      JsonObject d_requests_2 = d_requests.createNestedObject();
      d_requests_2["requestType"] = "GetInputVolume";
      d_requests_2["requestId"] = conc;
      d_requests_2["requestData"]["inputName"] = sourceItemsNames[i];
    }
  }
 Serial.println("Pase x aquiPase x aquiPase x aquiPase x aquiPase x aquiPase x aquiPase x aqui");

  String output;

  serializeJson(docItems, output); ////////////
  // Serial.print("Request: ");
  // Serial.println(output);
  webSocket.sendTXT(output);
}

//AUDIO SECTION

void setAudioInputLevelRequest(const char* inputName, double inputVolumeDb){
  StaticJsonDocument<192> doc;
  doc["op"] = 6;
  JsonObject d = doc.createNestedObject("d");
  d["requestType"] = "SetInputVolume";
  d["requestId"] = "060";
  d["requestData"]["inputName"] = inputName;
  d["requestData"]["inputVolumeDb"] = inputVolumeDb;
//  d["requestData"]["inputName"] = inputName;
  char output[192];
  serializeJson(doc, output);
//  Serial.print("output : ");
//  Serial.println(output);
  SendRequest(output);  
}

// This method is to be called when encoder button pressed
void setSourceVisibilityRequest(const char* sceneName, int sceneItemId, bool sceneItemEnabled){
  StaticJsonDocument<192> doc;
  doc["op"] = 6;
  JsonObject d = doc.createNestedObject("d");
  d["requestType"] = "SetSceneItemEnabled";
  d["requestId"] = "060";
  d["requestData"]["sceneName"] = sceneName;
  d["requestData"]["sceneItemId"] = sceneItemId;
  d["requestData"]["sceneItemEnabled"] = sceneItemEnabled;
  //  d["requestData"]["inputName"] = inputName;
  char output[192];
  serializeJson(doc, output);
  Serial.print("output : ");
  Serial.println(output);
  SendRequest(output);  
}

// This method is to be called when encoder button pressed
void setSourceLockRequest(const char* sceneName, int sceneItemId, bool sceneItemLocked){
  StaticJsonDocument<192> doc;
  doc["op"] = 6;
  JsonObject d = doc.createNestedObject("d");
  d["requestType"] = "SetSceneItemLocked";
  d["requestId"] = "060";
  d["requestData"]["sceneName"] = sceneName;
  d["requestData"]["sceneItemId"] = sceneItemId;
  d["requestData"]["sceneItemLocked"] = sceneItemLocked;
  //  d["requestData"]["inputName"] = inputName;
  char output[192];
  serializeJson(doc, output);
  Serial.print("output : ");
  Serial.println(output);
  SendRequest(output);  
}