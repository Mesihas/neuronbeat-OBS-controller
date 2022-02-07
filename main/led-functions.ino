//led functions

// pagina mode

void fadeInLeds(int globalFadeInValue){
    // Fade IN
    int k = globalFadeInValue;
  //  for(int k = 0; k < 256; k++) {
      setPixel(2,0,0,k);
      showStrip();
      //delay(3);
 //   }
}

void fadeOutLeds(){
      // Fade OUT
    int k = globalFadeOutValue;
  //  for(int k = 255; k >= 0; k--) {
      setPixel(2,0,0,k);
      showStrip();
    //   delay(3);
    // }
}

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void executeLedTask(){
  //FadeIn for ScenePageMode
  if(encoderButLngPress && viewScenes)
  { 
    if(globalFadeInValue >= 255){globalFadeInValue = 0;} // restart fadeIn value

    if(globalFadeInValue <= 255)
    {
      int r,g;
      r = random(255);
      g = random(255);
      setPixel(2,r,g,globalFadeInValue);
      showStrip();
     // fadeInLeds(globalFadeInValue);
      globalFadeInValue++;

    }
    else if(!encoderButLngPress && viewScenes)
    {
      globalFadeInValue = 0;
      Serial.println("Fin execute Led Task()");
    }
  }
  //FadeIn for SourcesPageMode
  if(encoderButLngPress && viewSources)
  { 
    if(globalFadeInValue >= 255){globalFadeInValue = 0;} // restart fadeIn value

    if(globalFadeInValue <= 255)
    {
      int r,g,b;
      r = random(100);
      b = random(180);

      setPixel(2,r,globalFadeInValue,b);
      showStrip();
     // fadeInLeds(globalFadeInValue);
      globalFadeInValue++;
    }
    else if(!encoderButLngPress && viewScenes)
    {
      globalFadeInValue = 0;
      Serial.println("Fin execute Led Task()");
    }
  }
}