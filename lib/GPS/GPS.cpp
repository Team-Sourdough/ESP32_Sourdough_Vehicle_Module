#include "./GPS.h"

uint32_t timer = millis();

//Performs initialization for GPS Module, by passing reference to driver.
void GPS_Setup(Adafruit_GPS *GPS) {
    GPSSerial.begin(9600, SERIAL_8N1, 8,7);
    GPS->begin(9600); 
    GPS->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    // Set the update rate
    GPS->sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);

// #define PMTK_SET_NMEA_UPDATE_1HZ "$PMTK220,1000*1F" ///<  1 Hz
// #define PMTK_SET_NMEA_UPDATE_2HZ "$PMTK220,500*2B"  ///<  2 Hz
// #define PMTK_SET_NMEA_UPDATE_5HZ "$PMTK220,200*2C"  ///<  5 Hz
// #define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F" ///< 10 Hz

    // Request updates on antenna status, wait short delay
    GPS->sendCommand(PGCMD_ANTENNA);
    delay(1000);

    // Ask for firmware version
    GPSSerial.println(PMTK_Q_RELEASE);

    //Success!
    Serial.println("GPS Init Success!");
    gpsData.latitude = 0.0f;
    gpsData.longitude = 0.0f;
    gpsData.speed = 0.0f;
}
//Reads from GPS module passed by reference, into array passed by reference. Returns 1 if successful, 0 else.
void updateGPSLocation(Adafruit_GPS *GPS) {
      //Read single char? Just for test?
      char c = GPS->read();

      //Debug Info?
      if ((c) && (GPSECHO))
      Serial.write(c);

      //If a sentence is received, we can check the checksum, and parse it.
      if (GPS->newNMEAreceived()) {
            if (!GPS->parse(GPS->lastNMEA())) {   // This also sets the newNMEAreceived() flag to false
                  Serial.println("GPS failed to parse, waiting for another");
            }
      }
}

bool updateGPSStruct(Adafruit_GPS *GPS, GPS_DATA *gpsData){
      //Timer to update twice per second
      //GPS is active: Calculate lat and longitude
      if (GPS->fix==1) {
            // Serial.println("Got a fix");
            //Obtain Mutex
            xSemaphoreTake(gpsDataMutex, portMAX_DELAY);
            //Update struct (write)
            gpsData->latitude = GPS->latitudeDegrees;
            gpsData->longitude = GPS->longitudeDegrees;
            gpsData->speed = (GPS->speed * KNOTS_TO_MPH);
            Serial.println(gpsData->latitude,8);
            Serial.println(gpsData->longitude,8);
            Serial.println(gpsData->speed);
            //Release Mutex
            xSemaphoreGive(gpsDataMutex);
            GPS->fix = 0;
            return true;
      }else{ //TODO: Remove else, used for debugging
            // Serial.println("No Fix");
            return false;
      }
}

//GPS Task to handle recieving position and notifies RF task
void GPS_Task(void* p_arg){
      //Setup GPS 
      GPS_Setup(&GPS);
      uint32_t timer = millis();
      bool gpsUpdated{false};                     
      while(1){
            //Get updated location data
            updateGPSLocation(&GPS);
            if (millis() - timer > 1000) {
                  //Reset the timer
                  timer = millis();
                  gpsUpdated = updateGPSStruct(&GPS, &gpsData);
            }

            if(gpsUpdated){
                  //Post updateGPS event flag
                  xEventGroupSetBits(rfEventGroup, updateGPS);
                  gpsUpdated = false;
            }

            vTaskDelay(x10ms);
      }

}
