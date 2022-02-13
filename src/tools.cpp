#include "tools.h"
//extern Adafruit_ST7735 tft;
extern TFT_eSPI tft;
void _delay(unsigned long ulDelay) {
    // Safe semi-blocking delay
#ifdef ESP32
    vTaskDelay(ulDelay); // Builtin to ESP32
#elif defined ESP8266
    unsigned long ulNow = millis();
    unsigned long ulThen = ulNow + ulDelay;
    while (ulThen > millis()) {
        yield(); // ESP8266 needs to yield()
    }
#endif
}

void centerString(String buf, int x, int y){
    //int16_t x1, y1;
    uint16_t w, h;
    //tft.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
    w = tft.textWidth(buf);
    //Serial.println(buf);
    //Serial.println(tft.fontHeight());
    //Serial.println("x" + (String)x);
    //Serial.println("y" + (String)y);
    //Serial.println("w" + (String)w);
    //Serial.print(w);
    tft.setCursor(x - (w / 2), y);
    tft.print(buf);
    return;
}

String pretty_time(long val){  
int days = elapsedDays(val);
int hours = numberOfHours(val);
int minutes = numberOfMinutes(val);
int seconds = numberOfSeconds(val);

 // digital clock display of current time
 //Serial.print(days,DEC);  
 //printDigits(hours);  
 //printDigits(minutes);
 //printDigits(seconds);
 //Serial.println();  
 // Build the string for "last seen"
 String last_seen = "";
 if (days > 0){
     last_seen = last_seen + days;
     last_seen = last_seen + "d,";
 }
  if (hours > 0){
    last_seen = last_seen + hours;
    last_seen = last_seen + "h,";
 }
 if (minutes > 0){
    last_seen = last_seen + minutes;
    last_seen = last_seen + + "m,";
 }
 if (seconds < 10){
 last_seen = last_seen + "0";    
 last_seen = last_seen + seconds;    
 }
 else {
last_seen = last_seen + seconds;         
 }
 last_seen = last_seen + "s ago";
 
return last_seen;
}

void printDigits(byte digits){
 // utility function for digital clock display: prints colon and leading 0
 Serial.print(":");
 if(digits < 10)
   Serial.print('0');
 Serial.print(digits,DEC);  
}


String get_last_value(String iSpinData){
int str_len = iSpinData.length() +1;
//char charData[str_len];
//iSpinData.toCharArray(charData,str_len);
//Split by the \r char
int line_len = iSpinData.indexOf("\r");
//char* splitData = strtok(charData,"\r");
//int line_len = strlen(splitData);
// Get to the LAST record
Serial.println("Position, Longueur ");
Serial.println(str_len);
Serial.println(line_len);
String lastData = iSpinData.substring(str_len-line_len,str_len);
return lastData;
}