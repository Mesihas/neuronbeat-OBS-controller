/*====================================================================================
  This sketch contains support functions for the ESP6266 SPIFFS filing system

  Created by Bodmer 15th Jan 2017
  ==================================================================================*/
 
//====================================================================================
//                 Print a SPIFFS directory list (root directory)
//====================================================================================
void listFiles(void) {
  Serial.println();
  Serial.println("SPIFFS files found:");

  String  line = "=====================================";

  Serial.println(line);
  Serial.println("  File name               Size");
  Serial.println(line);

#if defined(ESP32)
    File dir = SPIFFS.open("/");
    File f;
    while (f = dir.openNextFile()) {
        String fileName = f.name();
        Serial.print(fileName);
        int spaces = 25 - fileName.length(); // Tabulate nicely
        while (spaces--) Serial.print(" ");
        Serial.print(f.size()); Serial.println(" bytes");
    }
#else   //old ESP8266 has different SPIFFS methods
    fs::Dir dir = SPIFFS.openDir("/"); // Root directory
    fs::File f;
    while (dir.next()) {
        f = dir.openFile("r");  //current dir entry
        String fileName = f.name();
        Serial.print(fileName);
        int spaces = 25 - fileName.length(); // Tabulate nicely
        while (spaces--) Serial.print(" ");
        Serial.print(f.size()); Serial.println(" bytes");
    }
#endif

  Serial.println(line);
  Serial.println();
  delay(1000);
}
//====================================================================================
