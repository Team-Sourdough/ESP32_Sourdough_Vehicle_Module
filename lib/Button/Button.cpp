#include <Arduino.h>

#define BUTTON_GPIO 2

void IRAM_ATTR Button_ISR(){
    Serial.println("Button pressed");
    //Send update Siren event flag

}
