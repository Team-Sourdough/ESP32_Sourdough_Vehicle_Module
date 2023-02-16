#include "./RF.h"
#include "../GPS/GPS.h"


//Performs initialization for RF Module, by passing reference to driver.
void RF_Setup(RH_RF95 *rf95) {
      //Establish RST Pin and set.
      pinMode(RFM95_RST, OUTPUT); 
      digitalWrite(RFM95_RST, HIGH); 

      //Perform Manual reset before continuing init
      digitalWrite(RFM95_RST, LOW);
      delay(10);
      digitalWrite(RFM95_RST, HIGH);
      delay(10);

      //Check if init succeeded.
      while (!rf95->init()) {
      Serial.println("LoRa radio init failed");
      Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
      while (1);
      }
      //Success!
      Serial.println("LoRa radio init OK!");

      //Set frequency for RF Module
      if (!rf95->setFrequency(RF95_FREQ)) {
      Serial.println("setFrequency failed");
      while (1);
      }
      //Success!
      Serial.println("Set Freq to: "); Serial.println(RF95_FREQ);
      rf95->setTxPower(23, false);
}
//Sends GPS coordinates in latitude, longitude 
void RF_Send_GPS(uint8_t *array, RH_RF95 *rf95) {
            //Display on Sending side to confirm buffer structure and what is being sent
            // rf95->printBuffer("latitude array :", array, (uint8_t)4);
            // rf95->printBuffer("longitude array :", array+4, (uint8_t)4);
            // rf95->printBuffer("speed array :", array+8, (uint8_t)4);
            // rf95->printBuffer("angle array :", array+12, (uint8_t)4);
            // rf95->printBuffer("second array :", array+16, (uint8_t)4);
            // rf95->printBuffer("minute array :", array+20, (uint8_t)1);
            // rf95->printBuffer("hour array :", array+21, (uint8_t)1);
            // rf95->printBuffer("day array :", array+22, (uint8_t)1);
            // rf95->printBuffer("month array :", array+23, (uint8_t)1);
            // rf95->printBuffer("year array :", array+24, (uint8_t)1);
            
            // rf95->printBuffer("Sending :", array, (uint8_t)25);
            //Actual sending operation
            rf95->send(array, 25);
} 


void RF_Task(void* p_arg){  
      // Setup RF
      RH_RF95 rf95(RFM95_CS, RFM95_INT);
      RF_Setup(&rf95);

      Serial.println("Setup RF");
      uint8_t data_array[DataBufferSize];

      while(1){
            Serial.println("RF Task");
            Message_Buffer_Recieve(xMessageBuffer, data_array);
            RF_Send_GPS(data_array,&rf95); 
            delay(100);
      }
}
