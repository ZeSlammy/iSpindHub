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