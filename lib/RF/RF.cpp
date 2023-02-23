#include "./RF.h"

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

void PackGPS(uint8_t *array, GPS_DATA *gpsData){
      uint8_t* temp_lat = (uint8_t*)(&gpsData->latitude);
      uint8_t* temp_long = (uint8_t*)(&gpsData->longitude);
      uint8_t* temp_speed = (uint8_t*)(&gpsData->speed);

      //Adding the lattitude to array 
      memcpy(array, temp_lat, 4);
      memcpy(array + 4, temp_long, 4);
      memcpy(array + 8, temp_speed, 4);
}

void ParseGPS(uint8_t *array, GPS_DATA *gpsData){
      int latInt = (int)((gpsData->latitude)/100.0);
      float latitude = (float)latInt + (((((gpsData->latitude)/100.0) - (float)latInt) * (float)100.00)/60);
      int longInt = (int)((gpsData->longitude)/100.0);
      float longitude = (float)longInt + (((((gpsData->longitude)/100.0) - (float)longInt) * (float)100.00)/60);

      if(gpsData->latDir == (int) 'S'){
             latitude *= -1.0;
      }
      if(gpsData->longDir == (int)'W'){
            longitude *= -1.0;
      }
      PackGPS(array, gpsData);
}


void RF_Task(void* p_arg){  
      // Setup RF
      RH_RF95 rf95(RFM95_CS, RFM95_INT);
      RF_Setup(&rf95);

      Serial.println("Setup RF");
      uint8_t rfDataArray[DataBufferSize];
      EventBits_t eventFlags;
      bool gpsDataInitialized{false};

      while(1){
            Serial.println("RF Task");
            
            //Pend on updateGPS and siren event flags
            eventFlags = xEventGroupWaitBits(rfEventGroup, (updateGPS | sirenDetected), pdFALSE, pdFALSE, EVENT_GROUP_PEND_BLOCKING);
            // eventFlags = updateGPS;

            if(updateGPS & eventFlags) { //update rfDataArray based on new GPS data from buffer
                  Message_Buffer_Recieve(xMessageBuffer, rfDataArray);
                  gpsDataInitialized = true;
                  Serial.println("UPDATE GPS FLAG");
                  float* latData = (float*)(&rfDataArray[0]);
                  float* longData = (float*)(&rfDataArray[4]);

                  Serial.print("Latitude: ");
                  Serial.println(*latData, 8);
                  Serial.print("Longitude: ");
                  Serial.println(*longData, 8);
                  //Manually clear GPS flag so we can leave siren detected flag active unitl mic (or button task) clears the bit
                  xEventGroupClearBits(rfEventGroup, updateGPS); 
            }

            if(sirenDetected & eventFlags && gpsDataInitialized){ //only send if we have gps fix and real data
                  Serial.println("SIREN FLAG");
                  RF_Send_GPS(rfDataArray,&rf95); 
            }
            vTaskDelay(x100ms);
      }
}
