#define CREDENTIALS_FILE "/CredentialsData.txt"
// Keypad start position, key sizes and spacing
#define KEY_X 30 // Centre of key
#define KEY_Y 70
#define KEY_W 40 // Width and height
#define KEY_H 30
#define KEY_SPACING_X 7 // X and Y gap
#define KEY_SPACING_Y 12
#define KEY_TEXTSIZE 1  // Font size multiplier

// Using two fonts since numbers are nice when bold
// #define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
// #define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2

// Numeric display box size and location
#define DISP_X 1
#define DISP_Y 1
#define DISP_W 470
#define DISP_H 35
#define DISP_TSIZE 1
#define DISP_TCOLOR TFT_CYAN

// Number length, buffer for storing it and character index
#define NUM_LEN 25
char numberBuffer[NUM_LEN + 1] = "";
uint8_t numberIndex = 0;

// We have a status line for messages
#define STATUS_X 120 // Centred on this
#define STATUS_Y 65

int currentKeyboardSet = 0;
//abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_+=~`[]{}|\:;"'<>,.?/ with a space.
//                        26                        26        10                                33    
//

char keyLabelNumbers[30][2] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 
                               "!", "@", "#", "$", "%", "^", "&", "*", "(", ")",
                               "-", "+", "_", "+", "`", "~", ",", ";", ":", "."};

char keyLabelUpperCase[30][3] = {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", 
                                 "A", "S", "D", "F", "G", "H", "J", "K", "L", "/""",
                                 "Z", "X", "C", "V", "B", "N", "M", ".", ",", ";"}; 

char keyLabelLowerCase[40][3] = { "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", 
                                  "a", "s", "d", "f", "g", "h", "j", "k", "l", "|",
                                  "z", "x", "c", "v", "b", "n", "m", ",", ".", "/",
                                  "[", "]", ":", "\"", "{", "}", ";", "'", "<", ">"};

uint16_t keyColorNumbers[7] = {TFT_BLUE, TFT_RED, TFT_DARKGREY, TFT_DARKGREY, TFT_DARKGREY, TFT_RED, TFT_DARKGREEN };
                         
// Invoke the TFT_eSPI button class and create all the button objects
TFT_eSPI_Button key[40];

// Create 15 keys for the keypad
char key_mainButtonsLabelSpace[1][2] = {" "};
char key_mainButtonsLabel[7][5] = {"Spc", "Del ", "Num", "A", "a", "Exit", "Ok"};
TFT_eSPI_Button key_main[7];
//------------------------------------------------------------------------------------------

void keyboardInit(int key) {
  cfgLogger("keyboardInit",  "key : " + String((int32_t)key));
  tft.fillScreen(TFT_BLACK);

  // Draw keypad background
  //tft.fillRect(0, 0, 240, 320, TFT_DARKGREY);

  // Draw number display area and frame
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);
  
  // clear value stored in buffer
  numberIndex = 0; // Reset index to 0
  numberBuffer[numberIndex] = 0; // Place null in buffer
  tft.fillRect(0, 80, 475, 180, TFT_BLACK);
  // Draw keypad
  drawKeypad(key);
  drawMainLabel();
  keyboardActive = true;
}

//------------------------------------------------------------------------------------------

void keyboardHandler() {
  if(!keyboardActive){return;}
  Serial.println(" keyboardHandler(); :keyboardActive  TRUE");
  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates

  // Pressed will be set true is there is a valid touch on the screen
  boolean pressed = tft.getTouch(&t_x, &t_y);

  // / Check if any key coordinate boxes contain the touch coordinates FOR MAIN BUTTONS
  for (uint8_t b = 0; b < 7; b++) {
    if (pressed && key_main[b].contains(t_x, t_y)) {
      // if(b > 31){ 
      //   delay(10); // UI debouncing 
      //   return; 
      // }
      key_main[b].press(true);  // tell the button it is pressed
    } else {
      key_main[b].press(false);  // tell the button it is NOT pressed
    }
  }
  cfgLogger("keyboardHandler()", "Paso 1");
// Check if any key has changed state
  for (uint8_t b = 0; b < 7; b++) {

    // if (b < 3) tft.setFreeFont(LABEL1_FONT);
    // else tft.setFreeFont(LABEL2_FONT);

    if (key_main[b].justReleased()) key_main[b].drawButton();     // draw normal

    if (key_main[b].justPressed()) {
      key_main[b].drawButton(true);  // draw invert

      // Space button
      if (b == 0) {
        if (numberIndex < NUM_LEN) {
          numberBuffer[numberIndex] = key_mainButtonsLabelSpace[0][0];
          numberIndex++;
          numberBuffer[numberIndex] = 0; // zero terminate
        }
        status(""); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 1) {
        numberBuffer[numberIndex] = 0;
        if (numberIndex > 0) {
          numberIndex--;
          numberBuffer[numberIndex] = 0;//' ';
        }
        status(""); // Clear the old status
      }

      // Mumeric keypad button
      if (b == 2) {
        drawKeypad(1);
        status(""); // Clear the old status
      }

      // Uppercase keypad button
      if (b == 3) {
        drawKeypad(2);
        status(""); // Clear the old status
      }

      // Lowercase keypad button
      if (b == 4) {
        drawKeypad(3);
        status(""); // Clear the old status
      }

     //exit button
      if (b == 5) {
        // switch (keyboardCallback)
        // {
        //   case 1:  //press Network name
            status(""); // Clear the old status
            keyboardCallback = 0;
            currentConfigStep = 4;
            tft.fillScreen(themeColor.background_color);
            displayGenericMenu(1, 0);
            printAllNetCfgValuesToTFT();
            keyboardActive = false;
            return;
            break;
        // }

      }
      //ok button
      if (b == 6) {
        switch (keyboardCallback)
        {
          case 1:  //press Network password
            strlcpy(config.password,                  // <- destination
                    numberBuffer,  // <- source
                    sizeof(config.password));
            //createGUInetworkConfig();
            status(""); // Clear the old status
            currentConfigStep = 4;
            tft.fillScreen(themeColor.background_color);
            saveWiFiCredentials();
            displayGenericMenu(1, 0);
            printSelectedPassToTFT();
            return;
            break;

          case 2:  //press WS host
            strlcpy(config.websockets_server_host,                  // <- destination
                    numberBuffer,  // <- source
                    sizeof(config.websockets_server_host));
            //createGUInetworkConfig();
            status(""); // Clear the old status
            currentConfigStep = 4;
            tft.fillScreen(themeColor.background_color);
            saveWiFiCredentials();
            displayGenericMenu(1, 0);
            printSelectedHostToTFT();
            return;
            break;

          case 3:  //press WS port
            strlcpy(config.wsp,                  // <- destination
                    numberBuffer,  // <- source
                    sizeof(config.wsp));

            status(""); // Clear the old status
            updatedWSP =  true;
            currentConfigStep = 4;
            tft.fillScreen(themeColor.background_color);
            saveWiFiCredentials();
            displayGenericMenu(1, 0);
            printSelectedWspToTFT();
            return;
          break;

          case 4:  //press Network websockets_password
            strlcpy(config.websockets_password,                  // <- destination
                    numberBuffer,  // <- source
                    sizeof(config.websockets_password));
            //createGUInetworkConfig();
            status(""); // Clear the old status
            currentConfigStep = 4;
            tft.fillScreen(themeColor.background_color);
            saveWiFiCredentials();
            displayGenericMenu(1, 0);
            printSelectedWsPassToTFT();
            return;
          break;

          case 5:  //press Network name

          break;
        }
      }

      // // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSansOblique12pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(numberBuffer, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
     tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);
    }
  }

  cfgLogger("keyboardHandler()", "Paso 2");
  //##############################################################################################################

  // Character keyboards///////////////
  // / Check if any key coordinate boxes contain the touch coordinates
  int keybSetlen = 30;
  if(currentKeyboardSet == 3) {keybSetlen = 40;}

  for (uint8_t b = 0; b <keybSetlen; b++) {
    if (pressed && key[b].contains(t_x, t_y)) {
      // if(b > 31){ 
      //   delay(10); // UI debouncing 
      //   return; 
      // }
      key[b].press(true);  // tell the button it is pressed
    } else {
      key[b].press(false);  // tell the button it is NOT pressed
    }
  }

  cfgLogger("keyboardHandler()", "Paso 3");
  // Check if any key has changed state
  for (uint8_t b = 0; b < keybSetlen; b++) {

    // if (b < 3) tft.setFreeFont(LABEL1_FONT);
    // else tft.setFreeFont(LABEL2_FONT);

    if (key[b].justReleased()) key[b].drawButton();     // draw normal

    if (key[b].justPressed()) {
      key[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the numberBuffer
      if (b >= 0) {
        //check if port input is numeric
        if(keyboardCallback == 3){
          Serial.println(keyLabelNumbers[b][0]);
          Serial.println("numberIndex");
          Serial.println(numberIndex);
           if(!isValidNumber(String(keyLabelNumbers[b][0])) || numberIndex > 4){Serial.println("NO numeric");return;};
        } 
        ////////////////////////////////
        if (numberIndex < NUM_LEN) {
          switch (currentKeyboardSet)
          {
          case 1:
            numberBuffer[numberIndex] = keyLabelNumbers[b][0];
            break;
          case 2:
            numberBuffer[numberIndex] = keyLabelUpperCase[b][0];
            break;
          case 3:
            numberBuffer[numberIndex] = keyLabelLowerCase[b][0];
            break;          
          default:
            break;
          }
          
          numberIndex++;
          numberBuffer[numberIndex] = 0; // zero terminate
        }
        status(""); // Clear the old status
      }

      // Del button, so delete last char
      // if (b == 1) {
      //   numberBuffer[numberIndex] = 0;
      //   if (numberIndex > 0) {
      //     numberIndex--;
      //     numberBuffer[numberIndex] = 0;//' ';
      //   }
      //   status(""); // Clear the old status
      // }

      // if (b == 2) {
      //   status("Sent value to serial port");
      //   Serial.println(numberBuffer);
      // }
      // // we dont really check that the text field makes sense
      // // just try to call

      // if (b == 0) {
      //  // status("Value cleared");
      //  // numberIndex = 0; // Reset index to 0
      // //  numberBuffer[numberIndex] = 0; // Place null in buffer
      //  Serial.println("DALE PUTA ");

      // }

      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSansOblique12pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(numberBuffer, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
    }
  }
    cfgLogger("keyboardHandler()", "Paso final");
}

//------------------------------------------------------------------------------------------

void drawKeypad(int k)
{
  // Draw the keys
  uint8_t b = 0;
  int r=4;
 // if(k == 3) r = 4;
  currentKeyboardSet = k;

  for (uint8_t row = 0; row < r; row++) {
    for (uint8_t col = 0; col < 10; col++) {

      if (b < 3) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);
      
      switch (k)
      {
      case 1:     
        key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                          KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                          KEY_W, KEY_H, TFT_WHITE, TFT_BLUE   , TFT_WHITE,
                          keyLabelNumbers[b], KEY_TEXTSIZE);  
        break;
      case 2:
        key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                          KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                          KEY_W, KEY_H, TFT_WHITE, TFT_BLUE   , TFT_WHITE,
                          keyLabelUpperCase[b], KEY_TEXTSIZE);
        break;
      case 3:
        key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                          KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                          KEY_W, KEY_H, TFT_WHITE, TFT_BLUE   , TFT_WHITE,
                          keyLabelLowerCase[b], KEY_TEXTSIZE);
        break;
      default:
        break;
      }

      // Serial.println("Value B: ");
      // Serial.println(b);
      key[b].drawButton();
      b = b + 1;
      if(k != 3 && b > 30) return;
    }
  }
}

void drawMainLabel(){
  //coord de bottons for touch
  //
    // Draw the keys
    for (uint8_t col = 0; col < 7; col++) {
      // x, y, w, h, outline, fill, text
      key_main[col].initButton(&tft, 
      40 +  col * 67, 
      290, 
      62, 
      45, 
      TFT_WHITE, keyColorNumbers[col], TFT_WHITE,
      key_mainButtonsLabel[col], 
      KEY_TEXTSIZE);

      key_main[col].drawButton();
    }
}

//------------------------------------------------------------------------------------------
// Print something in the mini status bar
void status(const char *msg) {
  // tft.setTextPadding(240);
  // //tft.setCursor(STATUS_X, STATUS_Y);
  // tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  // tft.setTextFont(0);
  // tft.setTextDatum(TC_DATUM);
  // tft.setTextSize(1);
  // tft.drawString(msg, STATUS_X, STATUS_Y);
}

//------------------------------------------------------------------------------------------
