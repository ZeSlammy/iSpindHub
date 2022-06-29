#include <TFT_eSPI.h>
#include <Wire.h>
#include <SPI.h>
#include "tools.h"
#include <ESP8266WiFi.h>
#include <StreamUtils.h>
#define ST7735_GRAY    0x8410
#define ST7735_LIME    0x07FF
#define ST7735_AQUA    0x04FF
#define ST7735_PINK    0xF8FF
#define FreeSans9 "FreeSans20"
//#define FreeSansGras9 "FreeSansGras9"
#define FreeSans12 "FreeSans12"
#define Arial9 "Arial9"
#define Arial12 "Arial12"
#define Arial20 "Arial20"
#define RThin9 "RThin9"
#define RThin12 "RThin12"
#define RThin20 "RThin20"
#define RBold20 "RBold20"
#define TMS10 "TMS10"
#define TMS12 "TMS12"
#define FreeSansGras12 "SegLight20"
#define SegLight20 "SegLight20"
#include <FS.h>
#include <LittleFS.h>
#define FlashFS LittleFS
// void displaydata(String array_data[10],int last_seen_ms,String model);
void parse_screen_template(String screen_template,String array_data[10],int last_seen_ms);
void handle_global(String global_json);
void handle_line(String line_json);
void make_text_line(JsonObject text_line_json);
void make_line_line(JsonObject line_line_json);
char* get_font(String font);
