#include "common.h"

#define BUTTON_GPIO 2


EventGroupHandle_t EventGroupCreate(){
      // Attempt to create the event group.
      EventGroupHandle_t xCreatedEventGroup = xEventGroupCreate();

      // Was the event group created successfully?
      if( xCreatedEventGroup == NULL )
      {
      // The event group was not created because there was insufficient
      // FreeRTOS heap available.
            while(1); //Get stuck if event group doesn't get created
      }
      else
      {
            return xCreatedEventGroup;
      }
}

