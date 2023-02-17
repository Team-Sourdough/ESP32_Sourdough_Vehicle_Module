#ifndef COMMON_HG
#define COMMON_HG

#include <Arduino.h>

//RTOS includes
#include <FreeRTOS.h>
#include <message_buffer.h>
#include <event_groups.h>


#define BUTTON_GPIO 2

//Message Buffer
//--------------------------------------------------------------------------------------------------------------
//This buffer will be used to send the gps data to the RF task to actually send that bitch
MessageBufferHandle_t xMessageBuffer;
//Our buffer size needs to be 25 bytes, however there are an additional 4 bytes to hold the size of the buffer
const size_t xMessageBufferSizeBytes = 30;
const int DataBufferSize = 25;

const TickType_t x100ms = pdMS_TO_TICKS( 100 );

//Function Definitions
void Message_Buffer_Recieve(MessageBufferHandle_t xMessageBuffer, uint8_t data_array[]);
MessageBufferHandle_t Message_Buffer_Create_25byte();
void Message_Buffer_Send( MessageBufferHandle_t xMessageBuffer, uint8_t data_array[]);
//--------------------------------------------------------------------------------------------------------------

//Event Group
//--------------------------------------------------------------------------------------------------------------
EventGroupHandle_t rfEventGroup;

//Function Definitions
EventGroupHandle_t EventGroupCreate();
//--------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------
//Function Definitions
//-------------------------------------------------------------------------------------------------------------


#endif