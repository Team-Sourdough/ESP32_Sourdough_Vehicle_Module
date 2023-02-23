#ifndef SourDough_GPS
#define SourDough_GPS

#include <Arduino.h>
#include "../common.h"

#define GPSECHO  false

TimerHandle_t gpsTimer;

void GPS_Setup(Adafruit_GPS *GPS);
void updateGPSLocation(Adafruit_GPS *GPS);
bool updateGPSStruct(Adafruit_GPS *GPS, GPS_DATA *gpsData);


void GPS_Task(void* p_arg);
#endif