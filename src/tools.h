#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
void _delay(unsigned long);
#define DRD_TIMEOUT 3.0
#define DRD_ADDRESS 0x00
void centerString(String buf, int x, int y);