#include "Mic.h"
#include <Arduino.h>

void mic_setup(){
    //Connect GPIO 2 to Interrupt Service Routine to act as mic
    pinMode(BUTTON_GPIO, INPUT_PULLUP);
    attachInterrupt(BUTTON_GPIO, Button_ISR, FALLING);

}

void Mic_Task(void* p_arg){
    //Mic setup
    Serial.println("Mic Setup");
    while(1){
        Serial.println("Mic Task!");
        delay(10);
    }
}