//#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library
#include <TFT_eSPI.h>
#include <Wire.h>
#include <SPI.h>
#include "tools.h"
#include <ESP8266WiFi.h>
#define ST7735_GRAY    0x8410
#define ST7735_LIME    0x07FF
#define ST7735_AQUA    0x04FF
#define ST7735_PINK    0xF8FF
//#define TFT_CS     2
//#define TFT_RST    -1  // you can also connect this to the Arduino reset
// in which case, set this #define pin to -1!
//#define TFT_DC     0
void displaydata(String array_data[10],int last_seen_ms);