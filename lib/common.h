#ifndef COMMON_HG
#define COMMON_HG

#include <Arduino.h>

//RTOS includes
#include <FreeRTOS.h>
#include <message_buffer.h>
#include <event_groups.h>


#define BUTTON_GPIO 2

struct GPS_DATA {
    float latitude;
    float longitude;
    float speed;
};

static GPS_DATA gpsData;
constexpr uint16_t VEHICLE_ID = 101;

int timerID = 5;
constexpr TickType_t x500ms = pdMS_TO_TICKS( 500 );
constexpr TickType_t x100ms = pdMS_TO_TICKS( 100 );
constexpr TickType_t x10ms = pdMS_TO_TICKS( 10 );

//Event Group
//--------------------------------------------------------------------------------------------------------------
EventGroupHandle_t rfEventGroup;
enum rfEventFlagsEnum {
    updateGPS = 0b1 << 0,
    sirenDetected = 0b1 << 1
};

const TickType_t EVENT_GROUP_PEND_BLOCKING = portMAX_DELAY; //Max number of ticks

//Function Definitions
EventGroupHandle_t EventGroupCreate();
//--------------------------------------------------------------------------------------------------------------

//Mutex
//--------------------------------------------------------------------------------------------------------------
static SemaphoreHandle_t gpsDataMutex; 


#endif