#ifndef _TOOLS_H
#define _TOOLS_H
#include <Arduino.h>
#include <ArduinoJson.h>
//#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library
#include <TFT_eSPI.h>
void _delay(unsigned long);
#define DRD_TIMEOUT 3.0
#define DRD_ADDRESS 0x00
void centerString(String buf, int x, int y);
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)
#include <time.h>
/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY) 
String pretty_time(long val);
void printDigits(byte digits);
String get_last_value(String iSpinData);
uint32_t get_color(String colorString);
#endif