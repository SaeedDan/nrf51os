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
#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

// APPLICATION CONFIGURATION OPTIONS
#define BLE_INCLUDE
#define PINT_INCLUDE
#define RTC_INCLUDE
#define TWI_INCLUDE
//#define UART_INCLUDE
#define MPU9150
#define NRF51_MPU9150
#define COMPFILTER_INCLUDE
#define FREDERICK
// END APPLICATION CONFIGURATION OPTIONS

#if defined(BLE_INCLUDE)
   // TODO: Clean up defines in this sectio. A true mess!   
   #define BLE_NUS_INCLUDE
   #define MIN_CONN_INTERVAL                MSEC_TO_UNITS(400, UNIT_1_25_MS)           /**< Minimum acceptable connection interval (0.4 seconds). */
   #define MAX_CONN_INTERVAL                MSEC_TO_UNITS(650, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (0.65 second). */
   #define SLAVE_LATENCY                    0                                          /**< Slave latency. */
   #define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(4000, UNIT_10_MS)            /**< Connection supervisory timeout (4 seconds). */ 
   #define DEVICE_NAME                      "nrf51"
   #define APP_ADV_INTERVAL                 300                                        /**< The advertising interval (in units of 0.625 ms. This value corresponds to 25 ms). */
   #define APP_ADV_TIMEOUT_IN_SECONDS       180                                        /**< The advertising timeout in units of seconds. */
   #define SEC_PARAM_BOND                   0                                          /**< Perform bonding. */
   #define SEC_PARAM_MITM                   0                                          /**< Man In The Middle protection not required. */
   #define SEC_PARAM_IO_CAPABILITIES        BLE_GAP_IO_CAPS_NONE                       /**< No I/O capabilities. */
   #define SEC_PARAM_OOB                    0                                          /**< Out Of Band data not available. */
   #define SEC_PARAM_MIN_KEY_SIZE           7                                          /**< Minimum encryption key size. */
   #define SEC_PARAM_MAX_KEY_SIZE           16                                         /**< Maximum encryption key size. */
#endif   // BLE_INCLUDE

#if defined(PINT_INCLUDE)
   #define PINT_POLARITY_LOW               0
   #define PINT_POLARITY_HI                1
   #define PINT_POLARITY_TOGGLE            2

   #define PINT_NOPULL                     0
   #define PINT_PULLHI                     1
   #define PINT_PULLLO                     2

   #define PINT_INT_PINS                   1
   #define PINT_INT_PIN                    30
#endif   // PINT_INCLUDE

#if defined(RTC_INCLUDE)
   #define RTC_TICK_FRQ                   100
#endif   // RTC_INCLUDE

#if defined(UART_INCLUDE)
#endif   // UART_INCLUDE
#endif
