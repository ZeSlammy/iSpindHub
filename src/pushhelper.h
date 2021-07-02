#ifndef _PUSHHELPER_H
#define _PUSHHELPER_H

#include "pushtarget.h"
#include "target.h"
#include "brewfather.h"
#include <ESP8266WiFi.h>

IPAddress resolveHost(const char hostname[129]);
bool pushToTarget(PushTarget *, IPAddress, int);
void updateLoop();
void setDoURLTarget();
void setDoBFTarget();
void setDoBrewfTarget();

#endif // _PUSHHELPER_H
