
// TaskHandle_t testTask2;

//void IRAM_ATTR onTimer(){
//  Serial.println("A minute has passed");
//}
//t_idle: idles the system for both cores when nothing is happening
// void Task1_Handler(void * arg){
//  Serial.println("Inside Task1");
//  while(1){
//    Serial.println("task1 running");
//    vTaskDelay(1);
//  } 
// }
// void Task2_Handler(void * arg){
//  Serial.println("Inside Task2");
//  for(;;){
//    Serial.println("task2 running");
//    vTaskDelay(1);
//  } 
// }


#include <FreeRTOS.h>
#include <task.h>
#include <Arduino.h>

#include "GPS.cpp"
#include "RF.cpp"
#include "Mic.cpp"
#include "Button.cpp"
#include "../lib/common.c"


void setup(){

     Serial.begin(115200); 
//Create RTOS objects:
    TaskHandle_t gpsTask;
    TaskHandle_t rfTask;
    TaskHandle_t micTask;
    xMessageBuffer = Message_Buffer_Create_25byte();
    rfEventGroup = EventGroupCreate();

//Create component tasks
//CORE 0:
   xTaskCreatePinnedToCore(
                   &GPS_Task,   /* Task function. */
                   "GPS Task",     /* name of task. */
                   10240,       /* Stack size of task */
                   NULL,        /* parameter of the task */
                   10,           /* priority of the task */
                   &gpsTask,      /* Task handle to keep track of created task */
                   0);          /* pin task to core 1 */ 
   xTaskCreatePinnedToCore(
                   &RF_Task,   /* Task function. */
                   "RF Task",     /* name of task. */
                   10240,       /* Stack size of task */
                   NULL,        /* parameter of the task */
                   10,           /* priority of the task */
                   &rfTask,      /* Task handle to keep track of created task */
                   0);          /* pin task to core 1 */


// //CORE 1: (mic will run on its own core) 
   xTaskCreatePinnedToCore(
                  &Mic_Task,   /* Task function. */
                  "Mic Task",     /* name of task. */
                  10240,       /* Stack size of task */
                  NULL,        /* parameter of the task */
                  10,           /* priority of the task */
                  &micTask,      /* Task handle to keep track of created task */
                  1);          /* pin task to core 1 */ 


}

void loop(){
}

   My_timer = timerBegin(0, 80, true);
   timerAttachInterrupt(My_timer, &onTimer, true);
   timerAlarmWrite(My_timer, 10 * 1000000, true);  