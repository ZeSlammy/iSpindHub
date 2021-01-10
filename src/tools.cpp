#include "tools.h"
extern Adafruit_ST7735 tft;
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
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
    //Serial.print(buf);
    //Serial.print(w);
    tft.setCursor(x - w / 2, y);
    tft.print(buf);
    return;
}

char* pretty_time(long val){  
int days = elapsedDays(val);
int hours = numberOfHours(val);
int minutes = numberOfMinutes(val);
int seconds = numberOfSeconds(val);

 // digital clock display of current time
 Serial.print(days,DEC);  
 printDigits(hours);  
 printDigits(minutes);
 printDigits(seconds);
 Serial.println();  
 // Build the string for "last seen"
 char* last_seen = "";
 if (days > 0){
     last_seen = last_seen + days + 'd,';
 }
  if (hours > 0){
     last_seen = last_seen + hours + 'h,';
 }
 if (minutes > 0){
     last_seen = last_seen + minutes + 'm,';
 }
 if (seconds > 0){
     last_seen = last_seen + seconds + 's ago';
 }
return last_seen;
}

void printDigits(byte digits){
 // utility function for digital clock display: prints colon and leading 0
 Serial.print(":");
 if(digits < 10)
   Serial.print('0');
 Serial.print(digits,DEC);  
}
