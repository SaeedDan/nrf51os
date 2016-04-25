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
#include <string.h>

#include "configuration.h"

#include "ble.h"
#include "ble_stack_handler_types.h"
#include "nrf.h"
#if defined(RTC_INCLUDE)
   #include "nrf_drv_rtc.h"
#endif
#include "nordic_common.h"
#include "os.h"
#include "softdevice_handler.h"


enum OS_EVENT event;
#if defined(RTC_INCLUDE)
   const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(1);
#endif


#if defined(RTC_INCLUDE)
   static uint32_t  rtc_init(void);
   static void rtc_evt_handler(nrf_drv_rtc_int_type_t int_type);
#endif
static void ble_event_handler(ble_evt_t* evt);


int main(void)
{
   // Enable the SoftDevice and set the BLE Handler. 
   SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_SYNTH_250_PPM, NULL);
   softdevice_ble_evt_handler_set(ble_event_handler);

#if defined(RTC_INCLUDE)
   // Enable the RTC Timer.
   rtc_init();
#endif
   
   while (1)
   {
      os_handler(event, NULL);   // TODO: Define in app module.

      // Enter power saving mode and wait for more events.
      sd_app_evt_wait();
   }

   return -1;
}

#if defined(RTC_INCLUDE)
static uint32_t rtc_init(void)
{
   uint32_t error_code;

   // Init RTC instance.
   error_code = nrf_drv_rtc_init(&rtc, NULL, rtc_evt_handler);

   // Enable tick event and interrupt.
   nrf_drv_rtc_tick_enable(&rtc, true);

   // Power on the RTC peripheral.
   nrf_drv_rtc_enable(&rtc);

   return error_code;
} 

static void rtc_evt_handler(nrf_drv_rtc_int_type_t int_type)
{
   // Check if Tick Interrupt.
   if (int_type == NRF_DRV_RTC_INT_TICK)
   {

   }
}
#endif

static void ble_event_handler(ble_evt_t* evt)
{
   
}

