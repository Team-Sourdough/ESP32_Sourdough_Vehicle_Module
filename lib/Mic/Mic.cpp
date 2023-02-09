#include "Mic.h"
#include <Arduino.h>

void mic_setup(){

}

void Mic_Task(void* p_arg){
    //Mic setup

    Serial.println("Mic Setup");
    while(1){
        Serial.println("Mic Task!");
        delay(10);
    }
}