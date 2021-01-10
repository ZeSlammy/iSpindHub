#include "screen.h"
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

void displaydata(String array_data[10],int last_seen_ms)
{   tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_BLUE);
    tft.setTextWrap(false); 
    // Cadre tout autour
    tft.drawRect(0,0,128,128,ST7735_WHITE);
    //tft.setTextSize(1);
    tft.setFont(&FreeSansBold12pt7b);
    // SG 
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(3,22);
    tft.print("SG: "+ array_data[5]);
    // Separator
    tft.drawLine(1,24,127,24,ST7735_LIME);
    // Temperature
    //tft.setCursor(2,44);
    tft.setTextColor(ST7735_YELLOW);
    //tft.print("T° : " + array_data[3] + " °" + array_data[8]);
    centerString("T° : " + array_data[3] + " °" + array_data[8],64,44);
    tft.drawLine(1,46,127,46,ST7735_LIME);
    //Battery
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(ST7735_MAGENTA);
    centerString("Battery : " + array_data[4].substring(0,4) + " V",62,62);
    tft.drawLine(1,65,127,65,ST7735_LIME);
    // iSpindel Name
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(ST7735_RED);
    centerString(array_data[1],64,80);
    tft.drawLine(1,83,127,83,ST7735_LIME);
    //Last Seen
    tft.setFont();
    tft.setTextSize(1);
    tft.setTextColor(ST7735_ORANGE);
    centerString("Last seen ",64,86);
    //centerString("27 minutes ago",64,98);
    char* last_seen;
    last_seen= pretty_time(last_seen_ms);
    centerString(String(last_seen_ms),64,98);
    // Signal Strength
    // Let's color depending on the signal strength
    int sign_strength = array_data[7].substring(1,3).toInt();
    if (sign_strength < 65){
        tft.setTextColor(ST7735_GREEN);
    }
    else if (sign_strength < 70){
        tft.setTextColor(ST7735_YELLOW);
    }
    else if (sign_strength < 80){
        tft.setTextColor(ST7735_ORANGE);
    }
    else tft.setTextColor(ST7735_RED);
    
    centerString("Signal : " + String(sign_strength) + "dB",64,108);

    // Local IP
    tft.setFont();
    tft.setTextColor(ST7735_BLUE);
    centerString("IP : " + WiFi.localIP().toString(),64,118);
    //tft.print("IP : " + WiFi.localIP().toString());
    //tft.drawLine(1,120,127,120,ST7735_LIME);

return;
}