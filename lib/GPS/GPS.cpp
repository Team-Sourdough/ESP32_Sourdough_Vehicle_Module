#include "./GPS.h"

uint32_t timer = millis();

//Performs initialization for GPS Module, by passing reference to driver.
void GPS_Setup(Adafruit_GPS *GPS) {
    GPSSerial.begin(9600, SERIAL_8N1, 8,7);
    GPS->begin(9600); 
    GPS->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    // Set the update rate
    GPS->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

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
            Serial.println("Got a fix");
            // //Calculate correct degrees for lat and lat minutes
            // float latitude = GPS->latitude/100.0; // as DD.MMMM
            // int latDegreesInt = (int)(latitude); //DD
            // float temp = latitude - (float)latDegreesInt; //.MMMM
            // float minutes = temp * (float)100.0; //MM.MM
            // float minToDegrees = minutes / 60.0;
            // latitude = (float)latDegreesInt + minToDegrees;
            // //Check if GPS is above or below equator
            // if(GPS->lat == (int)"S"){
            // latitude *= -1;
            // }
            // //More calculations for correct degrees for lat and lat minutes
            // float latDegrees = (float)latDegreesInt;

            // //Calculate correct degrees for long and long minutes
            // float longitude = GPS->longitude/100.0f;
            // int longDegreesInt = (int)(longitude); //DDD
            // float longTemp = longitude - (float)longDegreesInt; //.MMMM
            // float longMinutes = longTemp * (float)100.0; //MM.MM
            // float longminToDegrees = longMinutes / 60.0;
            // longitude = (float)longDegreesInt + longminToDegrees;

            // //Check if GPS is west or east of Prime Meridian
            // if(GPS->lon == (int)'W'){
            // longitude *= -1.0;
            // }

            // //Display Measured Latitude and Longitude
            // Serial.print("Lat: ");
            // Serial.println(latitude, 8);
            // Serial.print("Long: ");
            // Serial.println(longitude, 8);

            gpsData->latitude = GPS->latitude;
            gpsData->latDir = GPS->lat;
            gpsData->longitude = GPS->longitude;
            gpsData->longDir = GPS->lon;
            gpsData->speed = (GPS->speed * KNOTS_TO_MPH);

            return true;
      }else{ //TODO: Remove else, used for debugging
            Serial.println("No Fix");
            return false;
      }
}



// void vTimerCallback( TimerHandle_t pxTimer ){
//       // updateGPSStruct(&GPS, &gpsData);
//       Serial.println("Timer Callback");
// }

//GPS Task to handle recieving position and notifies RF task
void GPS_Task(void* p_arg){
      //Setup GPS 
      Serial.println("Got into GPS Task");
      GPS_Setup(&GPS);
      uint32_t timer = millis();
      //Serial.println("GPS setup");
      uint8_t data_array[DataBufferSize];
      int cont = 0;

      //Create Timer
      // gpsTimer = xTimerCreate("GPS Timer",       // Just a text name, not used by the kernel.
      //                          x500ms,   // The timer period in ticks.
      //                          pdTRUE,        // The timers will auto-reload themselves when they expire.
      //                          ( void * ) timerID,  // Assign each timer a unique id equal to its array index.
      //                          vTimerCallback // Each timer calls the same callback when it expires.
      //                               );
      // //Start Timer
      // if( gpsTimer == NULL ){
      //       Serial.println("Timer not created");
      // }else{
      //        // Start the timer.  No block time is specified, and even if one was
      //        // it would be ignored because the scheduler has not yet been
      //        // started.
      //       if(xTimerStart(gpsTimer, 0) != pdPASS){
      //            Serial.println("Timer not started"); 
      //        }
      // }                              


      Serial.println("Got GPS Setup");
      //Create the buffer that will be used to send data over to the RF task
      while(1){
            //Get updated location data
            updateGPSLocation(&GPS);
            if (millis() - timer > 500) {
                  //Reset the timer
                  timer = millis();
                  cont = updateGPSStruct(&GPS, &gpsData);
            }

            if(cont){
                  //To my knowledge, pack will get all our data into an array
                  // GPS_Pack_uint8(data_array, &GPS); //This will pack all of our info for the system

                  //This posts the data to the message buffer that the RF module will use
                  // Message_Buffer_Send(xMessageBuffer, data_array);

                  //Post updateGPS event flag
                  xEventGroupSetBits(rfEventGroup, updateGPS);
                  cont = false;
            }

            vTaskDelay(x10ms);
      }

}
