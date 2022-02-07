void logEncoderGen(String mov, String who, int highlitedItem, int itemsCount, int encoderCount, int encoderLastValue, bool showEncoderValue){
  // Serial.println("-----------------------" +  mov  + "-------------------");
  // Serial.println("WHO: " + who);
  // Serial.println("highlithedItemInt: " + String((int32_t)highlitedItem));
  // Serial.println("highlithedSceneInt: " + String((int32_t)highlithedSceneInt));
  // Serial.println("itemsCount: " + String((int32_t)itemsCount));
  // Serial.println("getCurrentDisplayPage(highlitedItem): " + String(getCurrentDisplayPage(0)));
  // Serial.println("getCurrentDisplayPage(highlithedSceneInt): " + String(getCurrentDisplayPage(0)));
  // Serial.println("scenesCurrentPage: " + String(scenesCurrentPage));
  // Serial.println("scenesLastCurrentPage: " + String((int32_t)scenesLastCurrentPage));
  //  Serial.println("getPageFirstItemInt viewScenes: " + String((int32_t)getPageFirstItemInt(0)));
  //  Serial.println("getPageFirstItemInt viewSources: " + String((int32_t)getPageFirstItemInt(1)));
  // Serial.println("scenePageCount: " + String((int32_t)scenePageCount));
  // if(showEncoderValue){
  //   Serial.println("encoderLastValue: " + String((int32_t)encoderLastValue));
  //   Serial.println("encoderCount: " + String((int32_t)encoderCount));
  // }
  // Serial.print("Memoria: ");
  // Serial.println(ESP.getFreeHeap());
  // Serial.println("------------------------------------------------------------------");
}

void logger(String who, String subProcess, String presedent, String payload, bool showMemory){
  //checkPayload
  Serial.println("-------------------------------------------------------");
  Serial.println("Whos logging: " + who);
  Serial.println("Presedent: " + presedent);
  Serial.println("Sub Process: " + subProcess);

  Serial.println("log: " + payload);
  if(showMemory){
    Serial.print("Memoria: ");
    Serial.println(ESP.getFreeHeap());
  }
}

void listLogger(){
  return;
  tft.loadFont(LATO_REG_16);

  // LOG MENU POP SOURCES
  tft.setTextColor(themeColor.ink_color, themeColor.background_color);
  tft.fillRect(0,291,405,29,themeColor.background_color);
  String hsci = "auInLev: " + String(audioInputLevel);
  tft.drawString(hsci, 5, 291);
  String scp = "viPUp: " + String(viewPopUp);
  tft.drawString(scp, 100, 291);
  String sceCount = "auLeMod:" + String(audioInputLevelMode);
  tft.drawString(sceCount, 180, 291);

  String spc = "hiPopMenInt:" + String(highlithedPopUpMenuInt);
  tft.drawString(spc, 285, 291);


  //AQUI LOG TFT ENCODER AREA

  // String hsi = "hsi: " + String(highlithedSourceInt);
  // tft.drawString(hsi, 5, 308);
  // String socp = "souCurrPg: " + String(sourcesCurrentPage);
  // tft.drawString(socp, 70, 308);
  // String sopc = "souPgCnt: " + String(sourcesPageCount);
  // tft.drawString(sopc, 180, 308);
  // String souCount = "souCount: " + String(sourcesCount);
  // tft.drawString(souCount, 285, 308);


  // tft.loadFont(LATO_REG_16);
  // tft.setTextColor(themeColor.ink_color, themeColor.background_color);
  // tft.fillRect(0,291,405,29,themeColor.background_color);
  // String hsci = "hsci: " + String(highlithedSceneInt);
  // tft.drawString(hsci, 5, 291);
  // String scp = "sceCurrPag: " + String(scenesCurrentPage);
  // tft.drawString(scp, 70, 291);
  // String sceCount = "scePgCnt: " + String(scenePageCount);
  // tft.drawString(sceCount, 180, 291);

  // String spc = "sceCount: " + String(scenesCount);
  // tft.drawString(spc, 285, 291);

  // String hsi = "hsi: " + String(highlithedSourceInt);
  // tft.drawString(hsi, 5, 308);
  // String socp = "souCurrPg: " + String(sourcesCurrentPage);
  // tft.drawString(socp, 70, 308);
  // String sopc = "souPgCnt: " + String(sourcesPageCount);
  // tft.drawString(sopc, 180, 308);
  // String souCount = "souCount: " + String(sourcesCount);
  // tft.drawString(souCount, 285, 308);

  tft.unloadFont(); // Remove the font to recover memory used

}