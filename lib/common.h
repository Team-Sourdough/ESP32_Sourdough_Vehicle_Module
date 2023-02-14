#ifndef common
#define common

#include <FreeRTOS.h>
#include <message_buffer.h>

//This buffer will be used to send the gps data to the RF task to actually send that bitch
MessageBufferHandle_t xMessageBuffer;
//Our buffer size needs to be 25 bytes, however there are an additional 4 bytes to hold the size of the buffer
const size_t xMessageBufferSizeBytes = 30;
const TickType_t x100ms = pdMS_TO_TICKS( 100 );


#endif