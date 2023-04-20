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
      }
      //Success!
      Serial.println("LoRa radio init OK!");

      //Set frequency for RF Module
      if (!rf95->setFrequency(RF95_FREQ)) {
      Serial.println("setFrequency failed");
      }
      //Success!
      Serial.println("Set Freq to: "); Serial.println(RF95_FREQ);
      rf95->setTxPower(23, false);
}

static float longIncrement{0};
static float latIncrement{0};
static float speedIncrement{1};
void PackGPS(uint8_t *array, GPS_DATA *gpsData){
      Serial.println(latIncrement);
      Serial.println(longIncrement);
      Serial.println(speedIncrement);
      gpsData->latitude = 40.000113 + latIncrement;
      gpsData->longitude = -105.236410 + longIncrement;
      gpsData->speed = gpsData->speed + 5*speedIncrement;
      //Adding the data to array 
      memcpy(array, (&gpsData->latitude), sizeof(gpsData->latitude));
      memcpy(array + sizeof(gpsData->latitude), (&gpsData->longitude), sizeof(gpsData->longitude));
      memcpy(array + sizeof(gpsData->latitude) + sizeof(gpsData->longitude), (&gpsData->speed), sizeof(gpsData->speed));
      memcpy(array + sizeof(gpsData->latitude) + sizeof(gpsData->longitude) + sizeof(gpsData->speed), &VEHICLE_ID, sizeof(VEHICLE_ID));
      memcpy(array + sizeof(gpsData->latitude) + sizeof(gpsData->longitude) + sizeof(gpsData->speed) + sizeof(VEHICLE_ID), &CYCLE_LIGHT, sizeof(CYCLE_LIGHT));

      latIncrement += .1;
      longIncrement += .3;
      if(gpsData->speed >= 75.0){
            speedIncrement = -1;
      }else if(gpsData->speed <= 0){
            speedIncrement= 1;
      }

}


void RF_Task(void* p_arg){  
      // Setup RF
      Serial.println("Attempting to Setup RF");
      RH_RF95 rf95(RFM95_CS, RFM95_INT);
      RF_Setup(&rf95);
      Serial.println("Setup RF");
      static uint8_t rfDataArray[RF_DATA_SIZE];
      EventBits_t eventFlags;
      bool gpsDataInitialized{false};

      while(1){
            Serial.println("RF Task");
            
            //Pend on updateGPS and siren event flags
            eventFlags = xEventGroupWaitBits(rfEventGroup, (sirenDetected), pdFALSE, pdFALSE, EVENT_GROUP_PEND_BLOCKING);
            // eventFlags = updateGPS;

            // if(updateGPS & eventFlags) { //update rfDataArray based on new GPS data from buffer
            //       gpsDataInitialized = true;
            //       Serial.println("UPDATE GPS FLAG");
            //       //Pend Mutex
            //       xSemaphoreTake(gpsDataMutex, portMAX_DELAY);
            //       //Copy data into RF struct
            //       
            //       //Post Mutex
            //       xSemaphoreGive(gpsDataMutex);
            //       //Manually clear GPS flag so we can leave siren detected flag active unitl mic (or button task) clears the bit
            //       // rf95.send(rfDataArray, 14);
            //       xEventGroupClearBits(rfEventGroup, updateGPS); 
            // }
            if(sirenDetected & eventFlags){ //only send if we have gps fix and real data
                  Serial.println("SIREN FLAG");
                  delay(1000);//send data only 1 time a second
                  PackGPS(rfDataArray, &gpsData);
                  rf95.send(rfDataArray, RF_DATA_SIZE);
                  CYCLE_LIGHT = 0;
            }
            vTaskDelay(x100ms);
      }
}
