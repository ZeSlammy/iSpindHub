#include <TFT_eSPI.h>
#include <Wire.h>
#include <SPI.h>
#include "tools.h"
#include <ESP8266WiFi.h>
#define ST7735_GRAY    0x8410
#define ST7735_LIME    0x07FF
#define ST7735_AQUA    0x04FF
#define ST7735_PINK    0xF8FF
#define FreeSans9 "FreeSans20"
//#define FreeSansGras9 "FreeSansGras9"
#define FreeSans12 "FreeSans12"
#define FreeSansGras12 "SegLight20"
#include <FS.h>
#include <LittleFS.h>
#define FlashFS LittleFS
// void displaydata(String array_data[10],int last_seen_ms,String model);