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

#ifndef _OS_H_
#define _OS_H_

#include <stdint.h>
#include <stdbool.h>

#include "configuration.h"

enum OS_EVENT
{
   OS_EVENT_BOOTUP,
   OS_EVENT_BLE,
   OS_EVENT_RTC,
   OS_EVENT_PINT
};

bool os_handler(enum OS_EVENT event, uint8_t* data);
void os_shut_down(void);

#if defined(BLE_INCLUDE)
   enum OS_BLE_EVENT
   {
      OS_BLE_EVENT_CONNECTED,
      OS_BLE_EVENT_SERVICE_ENABLED,
      OS_BLE_EVENT_SERVICE_DATA_AVAILABLE,
      OS_BLE_EVENT_DISCONNECTED
   };

   bool os_ble_advertising_start(void);
   bool os_ble_advertising_stop(void);
   #if defined(BLE_NUS_INCLUDE)
      bool os_ble_nus_send_data(uint8_t* data, uint16_t length);
   #endif   // BLE_NUS_INCLUDE
#endif   // BLE_INCLUDE

#if defined(PINT_INCLUDE)
   void os_pin_int_set(uint8_t pin, uint8_t polarity, uint8_t pulled_up);
#endif   // PINT_INCLUDE

#if defined(RTC_INCLUDE)
   void os_get_time_ms(uint32_t* timestamp);
#endif   // RTC_INCLUDE

#if defined(UART_INCLUDE)
   bool os_uart_send_data(uint8_t* data, uint16_t length);
#endif   // UART_INCLUDE

#endif   // _OS_H_
