#ifndef common
#define common

#include <FreeRTOS.h>
#include <message_buffer.h>

//This buffer will be used to send the gps data to the RF task to actually send that bitch
MessageBufferHandle_t xMessageBuffer;
const size_t xMessageBufferSizeBytes = 25;
const TickType_t x100ms = pdMS_TO_TICKS( 100 );


#endif