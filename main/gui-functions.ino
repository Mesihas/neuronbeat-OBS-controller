void open_and_print_file(char* file, int x, int y){
  File fl = SPIFFS.open(file, "r");
  String bum = fl.name();
  const char *s = bum.c_str();
  if (strstr(s, ".jpg") != NULL) {
      printJPGtoTFT(s, x, y);
  }
  fl.close();
}

void printNotConnectedToWiFi(){
 //open_and_print_file(NO_WIFI_CONN, 40, 100);
 //longPressButtonC picture // Go to network Config section
 //
}

void printNotConnectedToOBS(){
 //open_and_print_file(NO_OBS_CONN, 40, 280);
  //longPressButtonC picture // Go to network Config section
}

void printMainGUI(bool fromConfig){
// if wifi and wesocket connected then continue or restart
  configuratorActive = false;
  currentConfigStep = -1; //-1 = no config step
  tft.fillScreen(themeColor.background_color);
  printScenesSourcesTittle();
  drawUIFrame(0);
  if(fromConfig){
    initRecCheck = false;
  }
}

void cleanSSIDlist(){
 tft.fillRect(10, 20, 228, 311, TFT_BLACK); 
}

void cleanContent(){
  if(configuratorActive){return;}
  cleanTFTlist(0, false); 
  cleanTFTlist(1, false);
  open_and_print_file(NO_MIC_WHITE_BACKGROUND, 80, 286);
}

void cleanTFTlist(int mode, bool noSources){
  if(mode == 0){  //scenesView
    tft.fillRect(20, 40, 200, 240, themeColor.background_color);
  }else if(mode == 1){ //sourcesView
    if(!noSources){
      tft.fillRect(260, 38, 211, 240, themeColor.background_color);
    }else{
      tft.fillRect(245, 38, 227, 240, themeColor.background_color); //clean source list including sprites, etc
    }
  }
}

void cleanSourcesCursorDisplayArea(){
  tft.fillRect(242, 38, 17, 240, themeColor.background_color);
}

//########### CHECK TOUCH   ###################################################################
void checkTouch(){
  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
  boolean pressed = tft.getTouch(&t_x, &t_y);
  int xx = 239;
  int yy = 286;
  if(!pressed){ return;}

  if ((t_x >= 0 && t_x <= 230) && (t_y >= 0 && t_y <= 280)) {
    /// SCENES AREA PRESSED
    if(viewScenes){return;}
    viewScenes = true;
    viewSources = false;
    viewPopUp = false;
    sourcesCurrentPage = 1; // this might change. Can remember the last source page/hlsi when come back?
    highlithedSourceInt = 0;
    drawUIFrame(0);
    Serial.println("SCENES AREA PRESSED--------------------");
  }else if((t_x >= 250 && t_x <= 480) && (t_y >= 0 && t_y <= 280)) {
    if(viewScenes == false){return;}
    /// SOURCES AREA PRESSED
    viewScenes = false;
    viewSources = true;
    drawUIFrame(1);
    Serial.println("SOURCES AREA PRESSED--------------------");
  }
  listLogger();
}

void check_and_print_WifiSignal(){
  File fl1;
  int x = 440;
  if(WiFi.status() != WL_CONNECTED) {
    open_and_print_file(WIFI_RSSI_0, x, bottomBarIconsHeight);
  }else{
    long rssi = WiFi.RSSI();
    if(rssi > -50){
      open_and_print_file(WIFI_RSSI_4, x, bottomBarIconsHeight);
    }else if(rssi > -65){
      open_and_print_file(WIFI_RSSI_3, x, bottomBarIconsHeight);
    }else if(rssi > -75){
      open_and_print_file(WIFI_RSSI_2, x, bottomBarIconsHeight);
    }else if(rssi > -80){
      open_and_print_file(WIFI_RSSI_1, x, bottomBarIconsHeight);
    }else{
      return;
    }
  }
}

void displayConfigIcon(){
  //open_and_print_file(CONFIG_ICON_WHITE_BACKGROUND, 385, 286);
}

void displayRecIcon(bool on){
  if(on && themeGlobal){
    open_and_print_file(REC_ON_W_BG, 0, 286);
    leds[2] = CRGB::Red;
  }else if(!on && themeGlobal){
    open_and_print_file(REC_Off_W_BG, 0, 286);
      leds[2] = CRGB::Black;
  }else if(on && !themeGlobal){
    open_and_print_file(REC_ON_B_BG, 0, 286);
    leds[2] = CRGB::Red;
  }else if(!on && !themeGlobal){
    open_and_print_file(REC_Off_B_BG, 0, 286);
      leds[2] = CRGB::Black;
  }
  FastLED.show();
}

void printMic_jpg_handler(bool state){

}

int imagecnt;   // mmm ? ver si util
void printJPGtoTFT(const char *name, int x, int y)
{
  boolean decoded = JpegDec.decodeFsFile(name);
  if (decoded == false) return;
  //   tft.setRotation(JpegDec.width > JpegDec.height ? 1 : 0);
  //   tft.fillScreen(TFT_BLACK);
  int t = jpegRender(x, y);
  char buf[80];
  //  sprintf(buf, "%3d: %-20s %dx%d %d ms", ++imagecnt, name, JpegDec.width, JpegDec.height, t);
  //  Serial.println(buf);
  drawJpeg(name, x, y);
}

int rgb(unsigned char r, unsigned char g, unsigned char b) {
    if (r < 0 || 255 < r || g < 0 || 255 < g || b < 0 || b > 255)
        return -1;
    unsigned char red = r >> 3;
    unsigned char green = g >> 2;
    unsigned char blue = b >> 3;
    int result = (red << (5 + 6)) | (green << 5) | blue;
//tests
    // printf("red: %x\n", red);
    // printf("green: %x\n", green);
    // printf("blue: %x\n", blue);
    // printf("result: %x\n", result);
    return result;
}

void touch_calibrate()
{

  //  uint16_t calData[5] = { 243, 3631, 222, 3535, 7 };
    uint16_t calData[5] = { 264, 3642, 275, 3516, 1 };
  // uint16_t calData[5];
   uint8_t calDataOK = 0;
  // check file system exists
  // if (!SPIFFS.begin()) {
  //   Serial.println("Formating file system");
  //  // SPIFFS.format();
  //   SPIFFS.begin();
  // }
  // check if calibration file exists and size is correct
  // if (SPIFFS.exists(CALIBRATION_FILE)) {
  //   if (REPEAT_CAL)
  //   {
  //     // Delete if we want to re-calibrate
  //     SPIFFS.remove(CALIBRATION_FILE);
  //   }
  //   else
  //   {
  //     File f = SPIFFS.open(CALIBRATION_FILE, "r");
  //     if (f) {
  //       if (f.readBytes((char *)calData, 14) == 14)
  //         calDataOK = 1;
  //       f.close();
  //     }
  //   }
  // }

 // if (calDataOK && !REPEAT_CAL) {
    if (1==1) {
    // calibration data valid
    //    uint16_t calData[5] = { 243, 3631, 222, 3535, 7 };

  //  uint16_t calData[5] = { 264, 3642, 275, 3516, 1 };

    // touch_calibrate
    tft.setTouch(calData);
    Serial.println("CALIBRATION OK!");
    for (uint8_t i = 0; i < 5; i++)
    {
      Serial.print(calData[i]);
      if (i < 4) Serial.print(", ");
    }
       Serial.println("");

  } else {
    configuratorActive = true;
    // data not valid so recalibrate
    tft.fillScreen(themeColor.background_color);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("Touch corners as indicated");
    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
    configuratorActive = false;
  }
}

void turnOffAllLeds() {
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;  
  leds[2] = CRGB::Black; 
  FastLED.show();
}

///// LED Effects functions //////////////////////////////////////
void ledWifiConnecting(){

    leds[0] = CRGB::Red;
    leds[1] = CRGB::Red;  
    leds[2] = CRGB::Red;        
    FastLED.show();
}

void ledStreaming(){

}

void ledMicOpen(){

}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void printAllNetCfgValuesToTFT(){
  printSelectedSSIDtoTFT();
  printSelectedPassToTFT();
  printSelectedHostToTFT();
  printSelectedWspToTFT();
  printSelectedWsPassToTFT();
}

void printSelectedSSIDtoTFT(){
  tft.setTextSize(1);
  tft.setTextColor(themeColor.ink_color, themeColor.background_color);
  tft.drawString(config.ssid, 250, 50);
  Serial.println("printSelectedSSIDtoTFT");
}

void printSelectedPassToTFT(){
  tft.setTextSize(1);
  tft.setTextColor(themeColor.ink_color, themeColor.background_color);
 // tft.drawString(config.password, 200, 80);
  int i;
  String passwordHideStarts;
  String pass = config.password;

  for (i = 0; i <  pass.length(); ++i)
  {
    passwordHideStarts = passwordHideStarts + "*";
  }
  tft.drawString(passwordHideStarts, 250, 80);
  Serial.println("printSelectedPassToTFT");
  keyboardActive = false;
}

void printSelectedHostToTFT(){
  tft.setTextSize(1);
  tft.setTextColor(themeColor.ink_color, themeColor.background_color);
  tft.drawString(config.websockets_server_host, 250, 110);
  Serial.println("void printSelectedHostToTFT");
  keyboardActive = false;
}

void printSelectedWspToTFT(){
  tft.setTextSize(1);
  tft.setTextColor(themeColor.ink_color, themeColor.background_color);
  if(config.wsp[0] == 0){
    tft.drawString(String(config.websockets_server_port), 250, 140);
    Serial.println("config.websockets_server_port in file = 0");

  }else{ 
    tft.drawString(config.wsp, 250, 140);
    Serial.print("config.wsp in file value");
  }
  keyboardActive = false;  
}

void printSelectedWsPassToTFT(){
  tft.setTextSize(1);
  tft.setTextColor(themeColor.ink_color, themeColor.background_color);
 // tft.drawString(config.websockets_password, 200, 170);
  int i;
  String passwordHideStarts;
  String pass = config.websockets_password;

  for (i = 0; i <  pass.length(); ++i)
  {
    passwordHideStarts = passwordHideStarts + "*";
  }
  tft.drawString(passwordHideStarts, 250, 170);

  Serial.println("printSelectedWsPassToTFT");
  keyboardActive = false;
}
/// Scene page
void highlightPageMode(bool state){
  int x,y,w,h,r;
    y = 2;
    w = 75;
    h = 27;
    r = 3;

  if (viewScenes)
  {
    x = 160;
  }
  else if(viewSources)
  {
    x = 395;
  }
  //tft.drawRoundRect(x-2, y - 5, 150, 27, 5, TFT_WHITE); //draws big rect on Scenes
  if (state)
  {
    tft.drawRoundRect(x, y, w, h, r, TFT_RED);
    tft.drawRoundRect(x+1, y+1, w-2, h-2, r, TFT_RED); 
  }
  else
  {
    tft.drawRoundRect(x, y, w, h, r, themeColor.background_color);
    tft.drawRoundRect(x+1, y+1, w-2, h-2, r, themeColor.background_color);
  }
}

void printPage(int mode){
  tft.loadFont(AA_FONT_LARGE);
  if(mode == 0){  //scenesView
    int scenesCurrentPage = getCurrentDisplayPage(0);   
    tft.setTextColor(themeColor.ink_color, themeColor.background_color);
    String pageNumber = "Pg: " + String(scenesCurrentPage) + "/" +  String(scenePageCount);
    tft.drawString(pageNumber, 165, 7);
  }else if(mode == 1){ //sourcesView
    int sourcesCurrentPage = getCurrentDisplayPage(1);   
    tft.setTextColor(themeColor.ink_color, themeColor.background_color);
    String pageNumber = "Pg: " + String(sourcesCurrentPage) + "/" +  String(sourcesPageCount);
    tft.drawString(pageNumber, 410, 7);
  }
  tft.unloadFont(); // Remove the font to recover memory used
}

void drawUIFrame(int mode){

  int x,y,w,h,xx,yy,ww,hh;

  x = 0;
  y = 30;
  w = 238;
  h = 260;

  xx = 241;
  yy = 30;
  ww = 230;
  hh = 260;

  if(mode == 0)
  {
    
    //Scenes
    tft.drawRect(x,y,w,h,rgb(173,216,230));
    tft.drawRect(x+1,y+1,w-2,h-2,rgb(0,0,255));
    tft.drawRect(x+2,y+2,w-4,h-4,rgb(25,25,112));

    tft.drawFastVLine(229, 32, 256, rgb(0,0,0)); //inner line of scroll section

    //Sources
    x = 240;
    tft.drawFastVLine(x, 30, 257, themeColor.sources_background_color);
    tft.drawFastVLine(x+1, 30, 257, themeColor.sources_background_color);// inner line

    tft.drawFastHLine(x, 30, 238, themeColor.sources_background_color);
    tft.drawFastHLine(x, 31, 238, themeColor.sources_background_color); // inner line


    tft.drawFastHLine(x, 288, 238, themeColor.sources_background_color);
    tft.drawFastHLine(x, 287, 238, themeColor.sources_background_color); // inner line

    tft.drawFastVLine(474, 32, 256, themeColor.sources_background_color); //inner line of scroll section
  }

  if(mode == 1)
  {
    //Scenes
    tft.drawRect(x,y,w,h,themeColor.sources_background_color);
    tft.drawRect(x+1,y+1,w-2,h-2,themeColor.sources_background_color);
    tft.drawFastVLine(229, 32, 256, themeColor.sources_background_color); //inner line of scroll section

    //Sources
    x = 240;
    w = 238;

    tft.drawFastVLine(x, 30, 258, TFT_BLUE);
    tft.drawFastVLine(x+1, 30, 257, TFT_BLUE);// inner line

    tft.drawFastHLine(x, 30, w, TFT_BLUE);
    tft.drawFastHLine(x, 31, w, TFT_BLUE); // inner line

    tft.drawFastHLine(x, 288, w, TFT_BLUE);
    tft.drawFastHLine(x, 287, w, TFT_BLUE); // inner line

    tft.drawFastVLine(473, 32, 256, themeColor.ink_color); //inner line of scroll section
  }
}

void drawScroll(int mode, int currentPage){
  int pageCount;
  if(mode == 0){
    pageCount = scenePageCount;
  }else if(mode == 1){
    pageCount = sourcesPageCount;
  }
// 256 -128 -85 - 64
  int x;
  int y;
  int w = 6;
  int h = 70;

  if(pageCount == 1){
    if(currentPage == 1){ y = 33;}
    h = 256;
  }

  if(pageCount == 2){
    h = 128;
    if(currentPage == 1){ y = 33;}
    if(currentPage == 2){ y = 161;}
  }

  if(pageCount == 3){
    h = 85;
    if(currentPage == 1){ y = 33;}
    if(currentPage == 2){ y = 118;}
    if(currentPage == 3){ y = 203;}   
  }

  if(pageCount == 4){
    h = 64;
    if(currentPage == 1){ y = 33;}
    if(currentPage == 2){ y = 97;}
    if(currentPage == 3){ y = 161;} 
    if(currentPage == 4){ y = 225;} 
  }

  if(mode == 0){
    tft.fillRect(230,32,6,256,themeColor.sources_background_color);
    x = 230;
    tft.fillRect(x,y,w,h,TFT_DARKGREY);
  }
  else if(mode == 1)
  {
    tft.fillRect(473,32,7,256,themeColor.sources_background_color);
    x = 473;
    w = 7;
    tft.fillRect(x,y,w,h,TFT_DARKGREY);
  }

}

void printScenesSourcesTittle(){
    tft.loadFont(LATO_REG_30);
    tft.setTextColor(themeColor.ink_color, themeColor.background_color);
    //String pageNumber = "Pg: " + String(sourcesCurrentPage);
    tft.drawString("Scenes", 5, 4);
    tft.drawString("Sources", 250, 4);
    tft.unloadFont(); // Remove the font to recover memory used
}

void drawPopUpMenu(int updatedIndex, int updatedField){
  if(updatedIndex != highlithedSourceInt){
    if(updatedIndex != -1 || updatedField != -1){
      return;// if updated row is not in popmenu.. exit.
    }
  }  

  int mode;
  int x = 255 ;
  int y = getPopMenuYcoord() - 8;

  int w = 212;
  int h = 50;
  int r = 3;

  if(updatedIndex == -1 && updatedField == -1)
  {
    mode = -1;
    tft.fillRect(460,y,12,h,TFT_WHITE); // clean display snall area on sources
    tft.fillRoundRect(x,y,w,h,r,rgb(250,250,250));
    tft.drawRoundRect(x,y,w,h,r, rgb(210,210,210));
    tft.drawRect(x+1,y+1,w-2,h-2,rgb(210,210,210));
  } //redraw everithing
  else 
  {
    //determines what to redraw
    if(updatedField == 4)
    {
      mode = 0;
    }
    else if(updatedField == 5)
    {
      mode = 1;
    }
    else if(updatedField == 7)
    {
      mode = 2;
    }    
  }

  x = x + 10;
  int ys = getPopMenuYcoord() + 6;
  int xGap = 54;

  drawSourcesIconsState(x, xGap, ys, mode);// determines wich icon to refresh
  cleanAndPrintCursor(1);
}

void drawSourcesIconsState(int x, int xGap, int ys, int mode){
  if(mode == 0 || mode == -1) // -1 redraw all icons
  {
    if(sourceItemsL[highlithedSourceInt][4] == 0){
      drawArrayJpeg(mg_sources_state_unvisible, sizeof(mg_sources_state_unvisible), x+3, ys + 2); //x = 270
    }else{
      drawArrayJpeg(mg_sources_state_visible, sizeof(mg_sources_state_visible), x + 3, ys + 2); //x = 270
    }
  }
  if(mode == 1|| mode == -1)
  {
    if(sourceItemsL[highlithedSourceInt][5] == 0){
      drawArrayJpeg(mg_sources_state_unlocked, sizeof(mg_sources_state_unlocked), x + 3 + xGap, ys + 2); // x = 327
    }else{
      drawArrayJpeg(mg_sources_state_locked, sizeof(mg_sources_state_locked), x + 3 + xGap, ys +2 ); // x = 327
    }
  }
  int input_kind = sourceItemsL[highlithedSourceInt][6];
  if((mode == 3 || mode == -1) && (input_kind == 7 ||input_kind == 4)) // check if audio input source type
  {
    drawArrayJpeg(wasapi_input_capture, sizeof(wasapi_input_capture), x + xGap * 2, ys);// x = 384
  }

  if(mode == 4 || mode == -1)
  {
    drawArrayJpeg(back, sizeof(back), x + xGap * 3, ys-4); // x = 441
  }
}

void stopViewPopup(){
   
  int w = 214;
  int h = 50;
  int r = 3;
  int x = 255;
  int y = getPopMenuYcoord() - 8;
  tft.fillRoundRect(x,y,w,h,r,TFT_WHITE);
}

void set_Input_kind_ima(int input_kind, int x, int y){
  x = x - 3;
  y = y - 3;
  switch (input_kind)
  {
  case -1:
      Serial.print("Display unknown input_kind icon"); 
    break;
  case 0:
    drawArrayJpeg(slideshow, sizeof(slideshow), x , y);
    break;
  case 1:
    drawArrayJpeg(ffmpeg_source, sizeof(ffmpeg_source), x , y);
    break;
  case 2:
    drawArrayJpeg(text_gdiplus, sizeof(text_gdiplus), x , y-1);
    break;
  case 3:
    drawArrayJpeg(browser_source, sizeof(browser_source), x , y);
    break;
  case 4:
    drawArrayJpeg(wasapi_output_capture, sizeof(wasapi_output_capture), x , y);  
    break;
  case 5:
    drawArrayJpeg(window_capture, sizeof(window_capture), x , y);
    break;
  case 6:
    drawArrayJpeg(monitor_capture, sizeof(monitor_capture), x , y);
    break;
  case 7:
    drawArrayJpeg(wasapi_input_capture, sizeof(wasapi_input_capture), x , y);
    break;
  case 8:
    drawArrayJpeg(dshow_input, sizeof(dshow_input), x , y);
    break;
  case 9:
    drawArrayJpeg(image_source, sizeof(image_source), x , y);
    break;
  case 10:
    drawArrayJpeg(color_source, sizeof(color_source), x , y);
    break;
  case 11:
    drawArrayJpeg(group, sizeof(group), x , y);
    break;
  case 12:
    drawArrayJpeg(gamePad, sizeof(gamePad), x , y);
    break;  
  case 13:
    drawArrayJpeg(wasapi_input_capture, sizeof(wasapi_input_capture), x , y);
    break; 
  case 14:
    drawArrayJpeg(wasapi_input_capture, sizeof(wasapi_input_capture), x , y);
    break; 
  case 15:
    drawArrayJpeg(wasapi_output_capture, sizeof(wasapi_output_capture), x , y);
    break; 
  case 16:
    drawArrayJpeg(monitor_capture, sizeof(monitor_capture), x , y);
    break; 
  case 17:
    drawArrayJpeg(monitor_capture, sizeof(monitor_capture), x , y);
    break; 
    
  default:
    break;
  }
}

void drawAudioInputState(int x, int yy, int level){
  // muted
  //barras de 3 px 
  int xs = 0;
  int w = 8;
  int h = 27;
  int lm; // lines Multiplier
  uint16_t lineColor;
  // this function shows the current selected item audio level and sets the global audioInputLevel too
  audioInputLevel = level; 
  sprMenuAudioLevel.setColorDepth(16);
  sprMenuAudioLevel.createSprite(w, h);
  sprMenuAudioLevel.fillSprite(rgb(250,250,250));
  
  x = x + 25; //adjust x for fine tunning position
  int y = getPopMenuYcoord() + 3;

  if(level == 0){
    lm = 0;
  } // no vol
  else if(level == 1){// green
    lm = 3;    
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(0,255,0));
  } 
  else if(level == 2){  // green
    lm = 6;   
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(0,255,0));
  } 
  else if(level == 3){ // green
    lm = 9;  
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(0,255,0));
  }
  else if(level == 4){  // green
    lm = 12;  
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(0,255,0));
  } 
  else if(level == 5){  // green
    lm = 15;  
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(0,255,0));
  } 
  else if(level == 6){ // green -yellow
    lm = 18;  
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(255,255,0));
    lm = 18 - 3;  //prints the green part
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(0,255,0));     
  } 
  else if(level == 7){ // green -yellow
    lm = 21;  
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(255,255,0));
    lm = 21 - 6;  //prints the green part
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(0,255,0));        
  } 
  else if(level == 8){ //red - green -yellow
    lm = 24; 
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(255,0,0));
    lm = 24 -3;   //prints the yellow part
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(255,255,0));
    lm = 24 - 9;  //prints the green part
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(0,255,0));  
  } 
  else if(level == 9){ //red - green -yellow
    lm = 27; 
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(255,0,0));  
    lm = 27 -6;   //prints the yellow part
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(255,255,0));
    lm = 27 - 12;  //prints the green part
    sprMenuAudioLevel.fillRect(xs, h - lm, w, lm, rgb(0,255,0));  
  } 
  //this serial log  values and coordinates for sources pop menu
  // Serial.println("x: " + String(x));
  // Serial.println("y: " + String(y));
  // Serial.println("level: " + String(level));
  // Serial.println("xs: " + String(xs));
  // Serial.println("h: " + String(h));
  // Serial.println("lm: " + String(lm));
  // Serial.println("w: " + String(w));

  sprMenuAudioLevel.pushSprite(x, y, TFT_TRANSPARENT);
  sprMenuAudioLevel.deleteSprite();

}

// get get Pop Menu Y coordinate // pop always comes up under the highlithed source execpt last two
int getPopMenuYcoord(){
  int hsi = highlithedSourceInt;
   int rowYpix;
  if(hsi == 8 || hsi == 18 || hsi == 28 || hsi == 38 ){
    rowYpix = lsdcY + ldhi * 6;

  }else if(hsi == 9 || hsi == 19 || hsi == 29 || hsi == 39){
    rowYpix = lsdcY + ldhi * 7;

  }else{
    rowYpix = getListDisplayRow(highlithedSourceInt) + 24;
  }
  return rowYpix;
}