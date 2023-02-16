#include <Arduino.h>
#include "../common.h"

void IRAM_ATTR Button_ISR(){
    Serial.println("Button pressed");
    //Send update Siren event flag

}
