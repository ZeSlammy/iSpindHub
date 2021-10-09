#include "screen.h"
//#include <Fonts/FreeSansBold12pt7b.h>
//#include <Fonts/FreeSansBold18pt7b.h>
//#include <Fonts/FreeSans9pt7b.h>
//#include <Fonts/FreeSansBold9pt7b.h>
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
TFT_eSPI tft = TFT_eSPI();  
void displaydata(String array_data[10],int last_seen_ms)
{   wdt_disable();
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_BLUE);
    tft.setTextWrap(false);
    // Cadre tout autour
    tft.drawRect(0,0,TFT_WIDTH,TFT_HEIGHT,TFT_WHITE);
    tft.setFreeFont(&FreeSansBold12pt7b);
    centerString("SG: "+ array_data[5],(TFT_WIDTH-2)/2,TFT_HEIGHT/8-2);
    //tft.setCursor(3,22);
    //tft.print("SG: "+ array_data[5]);
    // Separator
    //tft.drawLine(1,24,127,24,ST7735_LIME);
    tft.drawLine(1,TFT_HEIGHT/8 +2 ,TFT_WIDTH-1,TFT_HEIGHT/8 + 2,ST7735_LIME);
    // Temperature
    tft.setTextColor(TFT_YELLOW);
    centerString("T° : " + array_data[3] + " °" + array_data[8],(TFT_WIDTH/2),TFT_HEIGHT/4-2);
    
    tft.drawLine(1,TFT_HEIGHT/4 +2,TFT_WIDTH-1,TFT_HEIGHT/4 +2,ST7735_LIME);
    //Battery
    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_MAGENTA);
    //centerString("Battery : " + array_data[4].substring(0,4) + " V",(TFT_WIDTH/2),3*TFT_HEIGHT/8-2);
    centerString(array_data[2].substring(0,5) +" ÷ /" + array_data[4].substring(0,4) + " V",(TFT_WIDTH/2),3*TFT_HEIGHT/8-2);
    tft.drawLine(1,3*TFT_HEIGHT/8+2,TFT_WIDTH,3*TFT_HEIGHT/8+2,ST7735_LIME);
    // iSpindel Name
    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_RED);
    centerString(array_data[1],(TFT_WIDTH/2),TFT_HEIGHT/2-2);
    tft.drawLine(1,TFT_HEIGHT/2+2,TFT_WIDTH,TFT_HEIGHT/2+2,ST7735_LIME);
    //Last Seen
    tft.setFreeFont();
    tft.setTextSize(1);
    tft.setTextColor(TFT_ORANGE);
    centerString("Last seen ",(TFT_WIDTH/2),9*TFT_HEIGHT/16-2);
    String last_seen;
    last_seen= pretty_time(last_seen_ms);
    centerString(String(last_seen),(TFT_WIDTH/2),10*TFT_HEIGHT/16-2);
    // Signal Strength
    // Let's color depending on the signal strength
    int sign_strength = array_data[7].substring(1,3).toInt();
    if (sign_strength < 65){
        //tft.setTextColor(ST7735_GREEN);
        tft.setTextColor(TFT_GREEN);
    }
    else if (sign_strength < 70){
        //tft.setTextColor(ST7735_YELLOW);
        tft.setTextColor(TFT_YELLOW);
    }
    else if (sign_strength < 80){
        //tft.setTextColor(ST7735_ORANGE);
        tft.setTextColor(TFT_ORANGE);
    }
    //else tft.setTextColor(ST7735_RED);
    else tft.setTextColor(TFT_RED);
    
    centerString("Signal : " + String(sign_strength) + "dB",(TFT_WIDTH/2),6*TFT_HEIGHT/8-2);
    // Local IP
    //tft.setFont();
    tft.setFreeFont();
    //tft.setTextColor(ST7735_BLUE);
    tft.setTextColor(TFT_BLUE);
    centerString("IP : " + WiFi.localIP().toString(),(TFT_WIDTH/2),7*TFT_HEIGHT/8);
    //tft.print("IP : " + WiFi.localIP().toString());
    //tft.drawLine(1,120,127,120,ST7735_LIME);
    wdt_enable(WDTO_8S);
return;
}