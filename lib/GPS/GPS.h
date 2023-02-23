#ifndef SourDough_GPS
#define SourDough_GPS

#include <Adafruit_GPS.h>
#include <Arduino.h>
#include "../common.h"

#define GPSSerial Serial1 //Using Serial1 because Serial is taken for computer
#define GPSECHO  false

Adafruit_GPS GPS(&GPSSerial);
TimerHandle_t gpsTimer;

void GPS_Setup(Adafruit_GPS *GPS);
void updateGPSLocation(Adafruit_GPS *GPS);
bool updateGPSStruct(Adafruit_GPS *GPS, GPS_DATA *gpsData);


void GPS_Task(void* p_arg);
#endif