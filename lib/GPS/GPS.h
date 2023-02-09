#ifndef SourDough_GPS
#define SourDough_GPS

#include <Adafruit_GPS.h>
#include <Arduino.h>

#define GPSSerial Serial1 //Using Serial1 because Serial is taken for computer
#define GPSECHO  false

void GPS_Setup(Adafruit_GPS *GPS);
bool GPS_Location(uint8_t *array, uint32_t timer,Adafruit_GPS *GPS);

void GPS_Task(void* p_arg);

#endif