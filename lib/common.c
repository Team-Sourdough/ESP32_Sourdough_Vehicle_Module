#include "common.h"

void Message_Buffer_Recieve(MessageBufferHandle_t xMessageBuffer, uint8_t data_array[]){
      size_t xReceivedBytes;
      xReceivedBytes = xMessageBufferReceive( xMessageBuffer, ( void * ) data_array, sizeof(*data_array),x100ms);
      return;
}


//This function will create a message buffer of 25 bytes for later
//Returns the message buffer that was created
MessageBufferHandle_t Message_Buffer_Create_25byte(){
      xMessageBuffer = xMessageBufferCreate(xMessageBufferSizeBytes);

      if( xMessageBuffer == NULL ){
            // There was not enough heap memory space available to create the
            // message buffer.
            Serial.println("Something went wrong in the creation of the message buffer");
            return NULL;
      }
      else{
            return xMessageBuffer;
      }

}

//This is the function that will send our data to the data buffer, this will be read in the RF task so we can send the data!
//No output
void Message_Buffer_Send( MessageBufferHandle_t xMessageBuffer, uint8_t data_array[]){
      size_t xBytesSent;

      xBytesSent = xMessageBufferSend(xMessageBuffer, ( void * ) data_array, sizeof(*data_array), x100ms);

      if(xBytesSent != sizeof(*data_array)){
            // The call to xMessageBufferSend() times out before there was enough
            // space in the buffer for the data to be written.
            Serial.println("The buffer did not allocate space in time\n");
            return;
      }
      else{
            Serial.println("We good\n");
      }
      return;
}