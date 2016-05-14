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

#include "app.h"
#include "hw.h"
#include "os.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "invensense.h"
#include "invensense_adv.h"
#include "mltypes.h"
#include "mpu.h"

#define APP_FREQ         1   // App requested frequency. Derived from main Main RTC Clock Freq.

#if defined(RTC_INCLUDE)
   static uint32_t rtc_counter;
#endif   // RTC_INCLUDE
#if defined(NRF51_MPU9150)
   static int result = 0xAA;
   unsigned char *mpl_key = (unsigned char*)"eMPL 5.1";
#endif   // NRF51_MPU9250


#if defined(RTC_INCLUDE)
static void app_rtc_handler(void)
{
   rtc_counter++;

   if (APP_HZ_EVENT(rtc_counter, APP_FREQ))
   {  
      #if defined(BLE_NUS_INCLUDE)
         #if defined(NRF51_MPU9150)
            os_ble_nus_send_data(&result, sizeof(result));
         #endif // NRF51_MPU9250
      #endif   // BLE_NUS_INCLUDE
      #if defined(UART_INCLUDE)
         os_uart_send_data(&result, sizeof(result));
      #endif   // UART_INCLUDE

      rtc_counter = 0;
   }
}
#endif   // RTC_INCLUDE

static void app_ble_handler(void)
{
            
}

bool os_handler(enum OS_EVENT event, uint8_t* data)
{
   switch (event)
   {
     case OS_EVENT_BOOTUP:
     {
        #if defined(PINT_INCLUDE)
        os_pin_int_set(PINT_INT_PIN, PINT_POLARITY_LOW, PINT_PULLHI);  
        #endif   // PINT_INCLUDE
        #if defined(NRF51_MPU9150)
           // OS has booted up. Initialize app level modules and drivers.
           // Initialize HW Driver to default configuration.
           struct int_param_s int_param;
           result = mpu_init(&int_param);   
       #endif // NRF51_MPU9250
     }
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

     #if defined(PINT_INCLUDE)
     case OS_EVENT_PINT:
     {
        
     }
       break;
     #endif  // PINT_INCLUDE
         
      default:
         break;
   }

   return true;
}
