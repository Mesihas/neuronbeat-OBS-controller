void drawCursorLocoNew(int x, int y, int lastX, int lastY)
{
  listLogger();
  // This function is encharged to print the cursor for Scenes, Sources and PopMenu
  // this function need to be refactored
  if(!viewPopUp)
  {
    int xs = 8;
    int ys = 8;
    sprHglScenes.setColorDepth(16);
    sprHglScenes.createSprite(16, 16);
    sprHglScenes.fillSprite(TFT_TRANSPARENT);
    if (lastY != -1)  // clean last
    {
      sprHglScenes.fillCircle(xs, ys, 9, themeColor.background_color);
      sprHglScenes.pushSprite(lastX-17, lastY+2, TFT_TRANSPARENT);
    }

      sprHglScenes.fillCircle(xs, ys, 7, TFT_DARKGREY);
      sprHglScenes.fillCircle(xs, ys, 6, TFT_GREEN);
      sprHglScenes.fillCircle(xs, ys, 5, TFT_LIGHTGREY);  

      sprHglScenes.fillCircle(xs, ys, 3, TFT_BLACK);
      sprHglScenes.pushSprite(x-17, y+2, TFT_TRANSPARENT);
      sprHglScenes.deleteSprite();
  }
  else
  {
    int xs = 20;
    int ys = 20;
    sprMenuSources.setColorDepth(16);
    sprMenuSources.createSprite(40, 40);
    sprMenuSources.fillSprite(TFT_TRANSPARENT);
    int yTest = getPopMenuYcoord() - 3 ;
    if (lastY != -1)  // clean last
    {
      sprMenuSources.drawRoundRect(0,0,40,40,4, rgb(250,250,250));
      sprMenuSources.drawRoundRect(1,1,38,38,4, rgb(240,240,240));
      sprMenuSources.drawRoundRect(2,2,36,36,4, rgb(250,250,250));

      sprMenuSources.pushSprite(lastX, yTest, TFT_TRANSPARENT);
    }

    sprMenuSources.drawRoundRect(0,0,40,40,4, rgb(100,200,180));      
    sprMenuSources.drawRoundRect(1,1,38,38,4, rgb(200,200,180));
    sprMenuSources.drawRoundRect(2,2,36,36,4, rgb(100,200,180));
    
    sprMenuSources.pushSprite(x, yTest, TFT_TRANSPARENT);
    sprMenuSources.deleteSprite();

    if(sourceItemsL[highlithedSourceInt][6] == 7 || sourceItemsL[highlithedSourceInt][6] == 4)
    { //&& highlithedPopUpMenuInt == 2){
      drawAudioInputState(393 - 25, 168, sourceItemsL[highlithedSourceInt][7]);
    }
    
    tft.loadFont(LATO_REG_16);
    tft.setTextColor(themeColor.ink_color, themeColor.background_color);
    tft.fillRect(0,291,405,29,themeColor.background_color);
    // THIS LOGS ON THE TFT THE VALUES FOR THE POPUP MENU
    // String hsci = "x: " + String(x);
    // tft.drawString(hsci, 5, 291);
    // String scp = "y: " + String(y);
    // tft.drawString(scp, 70, 291);
    // String sceCount = "hPopMeInt: " + String(highlithedPopUpMenuInt);
    // tft.drawString(sceCount, 180, 291);
    // String spc = "lastX: " + String(lastX);
    // tft.drawString(spc, 285, 291);
    // String hsi = "lastY: " + String(lastY);
    // tft.drawString(hsi, 5, 308);
    //
  }
    // String socp = "souCurrPg: " + String(sourcesCurrentPage);
    // tft.drawString(socp, 70, 308);
    // String sopc = "souPgCnt: " + String(sourcesPageCount);
    // tft.drawString(sopc, 180, 308);
    // String souCount = "souCount: " + String(sourcesCount);
    // tft.drawString(souCount, 285, 308);
    tft.unloadFont(); // Remove the font to recover memory used
}

void cleanAndPrintCursor(int mode){

  int row;
  int x; 
  int adjustedRow;
  int y;
  
  // scenesCurrentPage
  if (mode == 0)
  {
    x = 20;
    row = getListDisplayRow(highlithedSceneInt);
    adjustedRow = row - 3; //fine adjustment of row height
    drawCursorLocoNew(x, adjustedRow, x, lastCursorYscenes);
    lastCursorYscenes = adjustedRow;
  }
  else if(mode == 1 && !viewPopUp)
  {
    x = 259;
    row = getListDisplayRow(highlithedSourceInt);
    adjustedRow = row - 5; //fine adjustment of row height
    drawCursorLocoNew(x, adjustedRow, x, lastCursorYsources);
    lastCursorYsources = adjustedRow;

  }
  else if(mode == 1 && viewPopUp)
  {
    x = 260;
    int xGap = 53;

    //determines if the popUp will be positioned top half or botton half
    y = getYpopupPosition();
  
    switch (highlithedPopUpMenuInt)
    {
    case 0:
       //260
      break;
    case 1:
      x = x + xGap; // 317
      break;
    case 2:
      x = x + 2 + xGap * 2; // = 374
      break;
    case 3:
      x = x + 1 + xGap * 3;// = 431
      break;
        
    default:
      break;
    }
    drawCursorLocoNew(x, y, lastCursorXsourcesMenu, y);
    lastCursorXsourcesMenu = x;
  }
}

int getListDisplayRow(int hsi){
  // This function takes a highlithed item and returns the Y coordinate
  // for whatever thing need to be printed 
  // example: the popmenu is printed above or bellow the returned value

  int row;

  if(hsi == 0 || hsi == 10 || hsi == 20 || hsi == 30){
    row = lsdcY;
  }else if(hsi == 1 || hsi == 11 || hsi == 21 || hsi == 31){
    row = lsdcY + ldhi;
  }else if(hsi == 2 || hsi == 12 || hsi == 22 || hsi == 32){
    row = lsdcY + ldhi * 2;
  }else if(hsi == 3 || hsi == 13 || hsi == 23 || hsi == 33){
    row = lsdcY + ldhi * 3;
  }else if(hsi == 4 || hsi == 14 || hsi == 24 || hsi == 34){
    row = lsdcY + ldhi * 4;
  }else if(hsi == 5 || hsi == 15 || hsi == 25 || hsi == 35){
    row = lsdcY + ldhi * 5;
  }else if(hsi == 6 || hsi == 16 || hsi == 26 || hsi == 36){
    row = lsdcY + ldhi * 6;
  }else if(hsi == 7 || hsi == 17 || hsi == 27 || hsi == 37){
    row = lsdcY + ldhi * 7;
  }else if(hsi == 8 || hsi == 18 || hsi == 28 || hsi == 38){
    row = lsdcY + ldhi * 8;
  }else if(hsi == 9 || hsi == 19 || hsi == 29 || hsi == 39){
    row = lsdcY + ldhi * 9;
  }
  return row;
}

void displaySceneList(int currentSceneValue, int highlithedSceneInt, String requester){

  int tft_windowItems = 9; // 0-9 (10)
  // Captures when moving outside baunds in case 
  if(highlithedSceneInt < 0){
    highlithedSceneInt = 0;
  }
  else if(highlithedSceneInt > scenesCount -1){
    highlithedSceneInt = scenesCount -1;
  }  

  cleanAndPrintCursor(0);
  //decides if clean the area ... cuando cambia de pagina
  if(scenesCurrentPage != scenesLastCurrentPage){
    scenesLastCurrentPage = scenesCurrentPage;
    Serial.println("cleanTFTlist() >>>>>>>>>>>>>>>>>>>>>>>>");
    cleanTFTlist(0, false);
    drawScroll(0, scenesCurrentPage);
  }else{
    if(requester == "ENCODER DOWN" || requester == "ENCODER UP"){ // if nothing changed don't print the list
      return;
    }
    if(requester == "Encoder ESCAPE Botton limit" || requester == "Encoder ESCAPE Top limit"){ // if nothing changed don't print the list
      return;
    }
  }

  int y = lsdcY;
  int x = 25;
  int iniWindow;
  int finWindow;
  int heightIncrement = ldhi;
  iniWindow = setInitEndDisplayPage(0,0);
  finWindow = setInitEndDisplayPage(1,0);

  printPage(0);
  
  sprListBold.loadFont(LATO_BLK16);
  sprListBold.createSprite(200, 14);
  sprListBold.fillSprite(TFT_WHITE); 
  sprListRegular.loadFont(LATO_REG_16);
  sprListRegular.createSprite(200, 14);   // Create a sprite 100 pixels wide and 50 high
  sprListRegular.fillSprite(TFT_WHITE);

  for(int e=iniWindow; e<=finWindow; e++){
    int adjustedRow = y - 3;
    String myString = String(scenesL[e]); //  use evil Strings for this job
    String sl = myString.substring(0, 20); // take the biggining part of the scene name
    sprListRegular.fillSprite(TFT_WHITE); // clean sprite to allow new text

    if(!movingEncoder || 1 == 1 ){ // avoid redraw the list to allaw cursor to move faster
      if(e == currentSceneValue && e != highlithedSceneInt){    
        sprListBold.setTextColor(themeColor.current_scene_color, themeColor.background_color); 
        sprListBold.drawString(sl, 0, 0 ); 
        sprListBold.pushSprite(x, y); // Push to TFT screen coord x, y
      } else if(e == highlithedSceneInt && e == currentSceneValue){
        sprListBold.setTextColor(themeColor.current_scene_color, themeColor.background_color);
        sprListBold.drawString(sl, 0, 0 );
        sprListBold.pushSprite(x, y);
      } else if(e == highlithedSceneInt ){
        sprListRegular.setTextColor(themeColor.ink_color, themeColor.background_color);
        sprListRegular.drawString(sl, 0, 0 ); 
        sprListRegular.pushSprite(x, y);     
      }else{
        sprListRegular.setTextColor(themeColor.ink_color, themeColor.background_color);
        sprListRegular.drawString(sl, 0, 0 ); 
        sprListRegular.pushSprite(x, y);   
      }
    }
    y = y + heightIncrement;
  }
  sprListBold.unloadFont();
  sprListBold.deleteSprite();
  sprListRegular.unloadFont(); // Remove the font to recover memory used
  sprListRegular.deleteSprite();
  // logEncoderAfuera("displaySceneList FINAL",  "displaySceneList", scenesCount, -1, -1, false, requester);
}

void displaySourcesList(int highlithedSourceInt, bool updateOneRow, int itemIndexToUpdate, String requester){
  // this function prints Sources list on screen.
  // Is is a very slow process and most code is about to avoid printing if nothing changed.
  // It will print when highlithed scene changes.

  if(movingEncoder && viewScenes){return;}
  if(sourcesCount == 0){cleanTFTlist(1, true); printDisplayNoItems(1); return;}
  if(viewScenes &&!updateOneRow){cleanTFTlist(1, false);} //OBS Event - Everytime highlitedItem changes Sources is wiped from screen

  highlithedSourceInt = 0;
  int y = lsdcY;
  int x = 255;
  int iniWindow;
  int finWindow;

  int tft_windowItems = 9;
  int tft_winItemMinusOne = tft_windowItems -1;
  cleanAndPrintCursor(1);

  //decides if clean the area ... when page changes
  if(sourcesCurrentPage != sourcesLastItemPage){
    sourcesLastItemPage = sourcesCurrentPage;
    cleanTFTlist(1, false);
  }else{
    // Don't draw sources because i am moving the encoder 
    if(requester == "ENCODER DOWN" || requester == "ENCODER UP"){ // if nothing changed don't print the list
      return;
    }
  }
  drawScroll(1, sourcesCurrentPage);

  iniWindow = setInitEndDisplayPage(0,1);
  finWindow = setInitEndDisplayPage(1,1);
  printPage(1);

  sprListRegular.loadFont(LATO_REG_16);
  sprListRegular.createSprite(153, 14);   // Create a sprite 100 pixels wide and 50 high
  sprListRegular.setTextColor(TFT_BLACK, TFT_WHITE); // Set the sprite font colour and the background colour
  // redraw only the one that has changed
  for(int e=iniWindow; e<=finWindow; e++){
    if(movingEncoder && viewScenes) {return;}
    if (updateOneRow)
    {
      if (sourceItemsL[e][3] != itemIndexToUpdate)
      {
        y = y + 24;
        continue;
      } 
    }
    //HERE HANDLE NOT TO OVERPRINT MENU POPUP IN CASE OF EVENT FIRED THIS FUNCTION

    String myString = String(sourceItemsNames[e]); //  use evil Strings for this job
    String sl = myString.substring(0,20); // take the biggining part of the scene name

    sprListRegular.fillSprite(TFT_WHITE); // clean sprite for new text
    if(sourceItemsL[e][4] == 0) { //if enable------------------------------------------------------------------
      if(highlithedSourceInt == e && viewScenes == false){//if current highlithedSourceInt---------------------
      }else{
        sprListRegular.setTextColor(themeColor.ssid_highlithed_color, themeColor.background_color);
      }

      sprListRegular.drawString(sl, 0, 0 ); 
      sprListRegular.pushSprite(x+26, y); 

      drawArrayJpeg(mg_sources_state_unvisible, sizeof(mg_sources_state_unvisible), x + 180, y);
    }else{
      if(highlithedSourceInt == e && viewScenes == false){
      }else{
        sprListRegular.setTextColor(themeColor.ink_color, themeColor.background_color);
      }
      sprListRegular.drawString(sl, 0, 0 ); 
      sprListRegular.pushSprite(x+26, y); 
      drawArrayJpeg(mg_sources_state_visible, sizeof(mg_sources_state_visible), x + 180, y);
    }
    //if locked-------------------------------------------------------------------------------------------------
    if(sourceItemsL[e][5] == 0) {
      drawArrayJpeg(mg_sources_state_unlocked, sizeof(mg_sources_state_unlocked), x + 201, y-2);
    }else{
      drawArrayJpeg(mg_sources_state_locked, sizeof(mg_sources_state_locked), x + 201, y-2);
    }
    set_Input_kind_ima(sourceItemsL[e][6], x + 7, y); 
    y = y + 24;
  }
  sprListRegular.deleteSprite();
  tft.unloadFont(); // Remove the font to recover memory used
  listLogger();
}

void printDisplayNoItems(int mode){
    tft.loadFont(LATO_REG_30);
    tft.setTextColor(TFT_RED, themeColor.background_color);

    if (mode == 0)
    {
      tft.drawString("No Scenes", 10, 50);/* code */
    }
    else if (mode == 1)
    {
      tft.drawString("No Sources", 265, 50);/* code */
    }
    else
    {
      /* code */
    }
    tft.unloadFont(); // Remove the font to recover memory used
}

int setInitEndDisplayPage(int window, int mode){
  int currentPage;
  int iniWindow;
  int finWindow;
  //int mode; // used to send mode to getLastPageItem(mode)
  if(mode == 0)
  {
    currentPage = scenesCurrentPage;
  }
  else if(mode == 1)
  {
    currentPage = sourcesCurrentPage;
  }

  switch (currentPage)
  {
  case 1:
    iniWindow = 0;
    finWindow = getLastPageItem(mode);
    break;
  case 2:
    iniWindow = 10;
    finWindow = getLastPageItem(mode);
  break;
    case 3:
    iniWindow = 20;
    finWindow = getLastPageItem(mode);
  break;
    case 4:
    iniWindow = 30;
    finWindow = getLastPageItem(mode);
  break;
  default:
    break;
  }
  if(window == 0) //initWindow
  {
    return iniWindow;
  }
  else
  {
    return finWindow;
  }
}