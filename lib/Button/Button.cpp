#include "Button.h"

#include <Arduino.h>
#include "../common.h"


//NOTE: THE RF->SEND FUNCTION WILL BLOCK INTERRUPTS WHICH MAY PREVENT US FROM UNSETTING THE SIREN FLAG 

void IRAM_ATTR Button_ISR(){
    Serial.println("Button pressed");
    //Send update Siren event flag
    BaseType_t xResult, xHigherPriorityTaskWoken;
    EventBits_t eventFlags = xEventGroupGetBitsFromISR(rfEventGroup);
    bool sirenDetectedFlag = (sirenDetected & eventFlags) >> 1;
    xHigherPriorityTaskWoken = pdFALSE; // xHigherPriorityTaskWoken must be initialised to pdFALSE.
    if(sirenDetectedFlag){ //Flag was set so clear bit 
        xResult = xEventGroupClearBitsFromISR(
                         rfEventGroup,    
                         sirenDetected); 
        if( xResult == pdPASS ) { //Bit was cleared successfully
                sirenDetectedFlag = false;
        }
        CYCLE_LIGHT = 1;
        Serial.println("Clearing bit");
    }else{ //flag is not set so set bit
        xResult = xEventGroupSetBitsFromISR(rfEventGroup,    // The event group being updated.
                                            sirenDetected,  // The bits being set.
                                            &xHigherPriorityTaskWoken);

        if( xResult == pdPASS ) { //Bit was set successfully
                portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
                sirenDetectedFlag = true;
        }else{
            Serial.println("Error setting bits");
        }
        Serial.println("bit already cleared");
    }

}
