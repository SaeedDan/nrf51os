/***************************************************************************************
# Copyright (c) 2015,  Vipin Bakshi
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
****************************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "configuration.h"

#include "hw.h"
#include "os.h"


#define APP_FREQ         10   // App requested frequency. Derived from main Main RTC Clock Freq.

#if defined(RTC_INCLUDE)
   uint32_t rtc_counter;
#endif   // RTC_INCLUDE


#if defined(RTC_INCLUDE)
void app_rtc_handler(void)
{
   rtc_counter++;

   if (IS_RTC_HZ_EVENT(rtc_counter, APP_FREQ))
   {
      rtc_counter = 0;

      #if defined(BLE_NUS_INCLUDE)
         char demoMessage[] = "Hello Message";
         os_ble_nus_send_data(demoMessage, sizeof(demoMessage));
      #endif   // BLE_NUS_INCLUDE

      #if defined(UART_INCLUDE)
         os_uart_send_data(demoMessage, sizeof(demoMessage));
      #endif   // UART_INCLUDE
   }
}
#endif


void app_ble_handler(void)
{
            
}

bool os_handler(enum OS_EVENT event, uint8_t* data)
{
   switch (event)
   {
     case OS_EVENT_BOOTUP:
        hw_init();
        break;
        
     #if defined(BLE_INCLUDE)
      case OS_EVENT_BLE:
         break;
      #endif

      #if defined(RTC_INCLUDE)   
      case OS_EVENT_RTC:
         app_rtc_handler();
         break;
      #endif

      default:
         break;
   }

   return true;
}
