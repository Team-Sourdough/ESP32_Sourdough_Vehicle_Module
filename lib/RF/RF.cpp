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
            // rf95->printBuffer("Vehicle ID :", array+12, (uint8_t)2);

            //Actual sending operation
            rf95->send(array, RF_DATA_SIZE);
} 


void PackGPS(uint8_t *array, GPS_DATA *gpsData){
      static const uint16_t VEHICLE_ID = 12345;

      //Adding the lattitude to array 
      memcpy(array, (&gpsData->latitude), sizeof(gpsData->latitude));
      memcpy(array + sizeof(gpsData->latitude), (&gpsData->longitude), sizeof(gpsData->longitude));
      memcpy(array + sizeof(gpsData->latitude) + sizeof(gpsData->longitude), (&gpsData->speed), sizeof(gpsData->speed));
      memcpy(array + sizeof(gpsData->latitude) + sizeof(gpsData->longitude) + sizeof(gpsData->speed), &VEHICLE_ID, sizeof(VEHICLE_ID));
}


void RF_Task(void* p_arg){  
      // Setup RF
      RH_RF95 rf95(RFM95_CS, RFM95_INT);
      RF_Setup(&rf95);
      Serial.println("Setup RF");
      static uint8_t rfDataArray[RF_DATA_SIZE];
      EventBits_t eventFlags;
      bool gpsDataInitialized{false};


      while(1){
            Serial.println("RF Task");
            
            //Pend on updateGPS and siren event flags
            eventFlags = xEventGroupWaitBits(rfEventGroup, (updateGPS | sirenDetected), pdFALSE, pdFALSE, EVENT_GROUP_PEND_BLOCKING);
            // eventFlags = updateGPS;

            if(updateGPS & eventFlags) { //update rfDataArray based on new GPS data from buffer
                  gpsDataInitialized = true;
                  Serial.println("UPDATE GPS FLAG");
                  //Pend Mutex
                  xSemaphoreTake(gpsDataMutex, portMAX_DELAY);
                  //Copy data into RF struct
                  PackGPS(rfDataArray, &gpsData);
                  //Post Mutex
                  xSemaphoreGive(gpsDataMutex);
                  //Manually clear GPS flag so we can leave siren detected flag active unitl mic (or button task) clears the bit
                  rf95.send(rfDataArray, 14);
                  xEventGroupClearBits(rfEventGroup, updateGPS); 
            }

            // if(sirenDetected & eventFlags && gpsDataInitialized){ //only send if we have gps fix and real data
            //       Serial.println("SIREN FLAG");
            //       RF_Send_GPS(rfDataArray,&rf95); 
            // }
            delay(2000);
      }
}
