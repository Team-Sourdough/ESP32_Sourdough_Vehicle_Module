#ifndef SourDough_RF
#define SourDough_RF

#include <SPI.h>
#include <RH_RF95.h>
#include <Arduino.h>
#include "../common.h"
#include "../GPS/GPS.h"

//RF pin defines
#define RFM95_RST     5   
#define RFM95_CS      10   
#define RFM95_INT     4   
//Radio frequency
#define RF95_FREQ 915.0 //must match RX's freq!
#define RF_DATA_SIZE 14
uint16_t VEHICLE_ID = 123;
void RF_Setup(RH_RF95 *rf95);
void RF_Send_GPS(uint8_t *array, RH_RF95 *rf95);
void PackGPS(uint8_t *array, GPS_DATA *gpsData);

void RF_Task(void* p_arg);


#endif