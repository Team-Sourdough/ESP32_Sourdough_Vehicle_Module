#include "./GPS.h"

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
}
//Reads from GPS module passed by reference, into array passed by reference. Returns 1 if successful, 0 else.
bool GPS_Location(uint8_t *array, uint32_t timer, Adafruit_GPS *GPS) {
      //Read single char? Just for test?
      char c = GPS->read();

      //Debug Info?
      if ((c) && (GPSECHO))
      Serial.write(c);

      //If a sentence is received, we can check the checksum, and parse it.
      if (GPS->newNMEAreceived()) {
            if (!GPS->parse(GPS->lastNMEA())) {   // This also sets the newNMEAreceived() flag to false
                  Serial.println("GPS failed to parse, waiting for another");
                  return 0;  // We can fail to parse a sentence in which case we should just wait for another
            }
      }

      //Todo: Refactor to use a semaphore timer instead!
      //Timer to update twice per second
      if (millis() - timer > 500) {
            //Reset the timer
            timer = millis();

            //Print current Fix status to determine if we have a connection.
            Serial.print("Fix: ");
            Serial.println((int)GPS->fix);
            
            //GPS is active: Calculate lat and longitude
            if (GPS->fix==1) {
                  //Calculate correct degrees for lat and lat minutes
                  float latitude = GPS->latitude/100.0; // as DD.MMMM
                  int latDegreesInt = (int)(latitude); //DD
                  float temp = latitude - (float)latDegreesInt; //.MMMM
                  float minutes = temp * (float)100.0; //MM.MM
                  float minToDegrees = minutes / 60.0;
                  latitude = (float)latDegreesInt + minToDegrees;
                  //Check if GPS is above or below equator
                  if(GPS->lat == (int)"S"){
                  latitude *= -1;
                  }
                  //More calculations for correct degrees for lat and lat minutes
                  float latDegrees = (float)latDegreesInt;

                  //Calculate correct degrees for long and long minutes
                  float longitude = GPS->longitude/100.0f;
                  int longDegreesInt = (int)(longitude); //DDD
                  float longTemp = longitude - (float)longDegreesInt; //.MMMM
                  float longMinutes = longTemp * (float)100.0; //MM.MM
                  float longminToDegrees = longMinutes / 60.0;
                  longitude = (float)longDegreesInt + longminToDegrees;

                  //Check if GPS is west or east of Prime Meridian
                  if(GPS->lon == (int)'W'){
                  longitude *= -1.0;
                  }
                  //Display Measured Latitude and Longitude
                  Serial.print("Latitude: ");
                  Serial.println(latitude, 8);
                  Serial.print("Longitude: ");
                  Serial.println(longitude, 8);

                  //Put found latitude and longitude into array to be sent.
                  uint8_t* tempLat = (uint8_t*)(&latitude);
                  uint8_t* tempLong = (uint8_t*)(&longitude);
                  for(int i = 0; i < 4; i++){
                        array[i] = tempLat[i];
                  }
                  for(int i = 0; i < 4; i++){
                        array[i+4] = tempLong[i];
                  }
                  return true;
            }
      }
      //If we got here... fix was zero / GPS not establishing connection.
      Serial.println("GPS failed in some way");
      return 0;
}

//GPS Task to handle recieving position and notifies RF task
void GPS_Task(void* p_arg){
      //Setup GPS 
      Adafruit_GPS GPS(&GPSSerial);
      GPS_Setup(&GPS);
      Serial.println("GPS setup");
      while(1){
            //Get location and post semaphore for RF
            // GPS_Location()


            //Message queue to RF 

            //Post semaphore
            Serial.println("GPS Task!");
            delay(10);
      }

}
