#include <FreeRTOS.h>
#include <task.h>
#include <Arduino.h>

#include "GPS.cpp"
#include "RF.cpp"
#include "Mic.cpp"
#include "Button.cpp"
#include "../lib/common.c"

void setup(){
    spi.begin(SCK, MISO, MOSI, CS);
    Serial.begin(115200); 
    //Create RTOS objects:
    TaskHandle_t gpsTask;
    TaskHandle_t rfTask;
    TaskHandle_t micTask;
    TaskHandle_t TestTask;
    rfEventGroup = EventGroupCreate();
    gpsDataMutex = xSemaphoreCreateMutex();


//Create component tasks
//CORE 0:
//    xTaskCreatePinnedToCore(
//                    &GPS_Task,   /* Task function. */
//                    "GPS Task",     /* name of task. */
//                    10240,       /* Stack size of task */
//                    NULL,        /* parameter of the task */
//                    10,           /* priority of the task */
//                    &gpsTask,      /* Task handle to keep track of created task */
//                    0);          /* pin task to core 0 */ 
   xTaskCreatePinnedToCore(
                   &RF_Task,   /* Task function. */
                   "RF Task",     /* name of task. */
                   10240,       /* Stack size of task */
                   NULL,        /* parameter of the task */
                   10,           /* priority of the task */
                   &rfTask,      /* Task handle to keep track of created task */
                   0);          /* pin task to core 0 */
// //CORE 1: (mic will run on its own core)
//    xTaskCreatePinnedToCore(
//                    &Test_Task,   /* Task function. */
//                    "Test Task",     /* name of task. */
//                    10240,       /* Stack size of task */
//                    NULL,        /* parameter of the task */
//                    10,           /* priority of the task */
//                    &TestTask,      /* Task handle to keep track of created task */
//                    0);          /* pin task to core 0 */  
   xTaskCreatePinnedToCore(
                  &Mic_Task,   /* Task function. */
                  "Mic Task",     /* name of task. */
                  20480,       /* Stack size of task */
                  NULL,        /* parameter of the task */
                  7,           /* priority of the task */
                  &micTask,      /* Task handle to keep track of created task */
                  1);          /* pin task to core 1 */ 


}

void loop(){
    delay(1);
} 