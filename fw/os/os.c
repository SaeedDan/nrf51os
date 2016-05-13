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

#if defined(UART_INCLUDE)
   #include "app_uart.h"
#endif
#if defined (BLE_INCLUDE)
   #include "ble.h"
   #include "ble_advertising.h"
   #include "ble_advdata.h"
   #include "ble_gap.h"
   #include "ble_gatts.h"
   #include "ble_types.h"
   #include "ble_stack_handler_types.h"
   #if defined(BLE_NUS_INCLUDE)
      #include "ble_nus.h"
   #endif   // BLE_NUS_INCLUDE
#endif   // BLE_INCLUDE
#include "hw.h"
#include "nrf.h"
#if defined(RTC_INCLUDE)
   #include "nrf_drv_rtc.h"
#endif   // RTC_INCLUDE
#include "nrf_soc.h"
#include "nordic_common.h"
#include "os.h"
#include "softdevice_handler.h"
#if defined(TWI_INCLUDE)
   #include "twi_master.h"
#endif   // TWI_INCLUDE
#include "nrf_delay.h"

#define EVENT_BLE       0x01
#define EVENT_RTC_TICK  0x02


static volatile uint8_t  event;
static uint8_t* running_event_data;
#if defined(RTC_INCLUDE)
   const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(1);
   static uint32_t os_rtc_time_ms = 0;
   static uint32_t os_cumulative_rtc_ticks = 0;
#endif
#if defined(BLE_INCLUDE)
   static uint32_t ble_connection_handle;
   #if defined(BLE_NUS_INCLUDE)
      static ble_nus_t ble_nus;
   #endif   // BLE_NUS_INCLUDE
#endif   // BLE_INCLUDE


#if defined(BLE_INCLUDE)
   static void os_ble_event_handler(ble_evt_t* evt);
   static bool os_ble_gap_init(void);
   static bool os_ble_advertising_init(void);
   #if defined(BLE_NUS_INCLUDE)
      static void os_ble_nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);   
   #endif   // BLE_NUS_INCLUDE
#endif   // BLE_INCLUDE
#if defined(RTC_INCLUDE)
   static uint32_t  os_rtc_init(void);
   static void os_rtc_evt_handler(nrf_drv_rtc_int_type_t int_type);
#endif   // RTC_INCLUDE
#if defined(UART_INCLUDE)
   static void os_uart_init(void);
   static void os_uart_event_handle(app_uart_evt_t * p_event);
#endif // UART_INCLUDE

int main(void)
{
   enum OS_EVENT running_event;
   uint8_t critical_region;
   uint32_t err_code;
   event = 0;
   #if defined(BLE_INCLUDE)
      ble_connection_handle = BLE_CONN_HANDLE_INVALID;
      #if defined(BLE_NUS_INCLUDE)
         ble_nus_init_t ble_nus_initial;
      #endif   // BLE_NUS_INCLUDE
   #endif   // BLE_INCLUDE

   // Initialize the port map to a stable setting.      
   //hw_init();      
         
   // Enable the SoftDevice and set the BLE Handler. 
   SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_75_PPM, NULL);
   #if defined(BLE_INCLUDE)
      #if defined(S110) || defined(S130) || defined(S310)
          // Enable BLE stack.
          ble_enable_params_t ble_enable_params;
          memset(&ble_enable_params, 0, sizeof(ble_enable_params));
       #if defined(S130) || defined(S310)
          ble_enable_params.gatts_enable_params.attr_tab_size   = BLE_GATTS_ATTR_TAB_SIZE_DEFAULT;
       #endif
       ble_enable_params.gatts_enable_params.service_changed = 1;
       err_code = sd_ble_enable(&ble_enable_params);
       APP_ERROR_CHECK(err_code);
    #endif
      softdevice_ble_evt_handler_set(os_ble_event_handler);
      os_ble_gap_init();
      os_ble_advertising_init();
      os_ble_advertising_start();
      #if defined(BLE_NUS_INCLUDE)
         memset(&ble_nus_initial, 0, sizeof(ble_nus_initial));
         ble_nus_initial.data_handler = os_ble_nus_data_handler;
         ble_nus_init(&ble_nus, &ble_nus_initial);
      #endif   // BLE_NUS_INCLUDE
   #endif   // BLE_INCLUDE
   #if defined(RTC_INCLUDE)
      // Enable the RTC Timer.
      os_rtc_init();
   #endif   // RTC_INCLUDE
   #if defined(TWI_INCLUDE)
      twi_master_init();      
   #endif   // TWI_INCLUDE
   #if defined(UART_INCLUDE)
      os_uart_init();
   #endif   // UART_INCLUDE


      nrf_delay_us(3000);   
   // Send event to app that OS has booted up.   
   os_handler(OS_EVENT_BOOTUP, NULL);
      
   while (1)
   {
      // Event Dispatcher.
      sd_nvic_critical_region_enter(&critical_region);
      if (event & EVENT_BLE)
      {
         running_event = OS_EVENT_BLE;
         event &= ~EVENT_BLE;
      }
      else if (event & EVENT_RTC_TICK)
      {
         running_event = OS_EVENT_RTC;
         event &= ~EVENT_RTC_TICK;
         running_event_data = NULL;
      }
      sd_nvic_critical_region_exit(critical_region);

      // Dispatch Event to Application.
      os_handler(running_event, running_event_data);
      
      // Check if all events have been handled.
      if (!event)   
      {
         // Enter power saving mode and wait for more events.
         sd_app_evt_wait();
      }
   }

   return -1;
}

#if defined(RTC_INCLUDE)
static uint32_t os_rtc_init(void)
{
   uint32_t error_code;
   nrf_drv_rtc_config_t rtc_config;
   
   // Init RTC instance.
   rtc_config.prescaler = (uint16_t)(32768 / RTC_TICK_FRQ)-1;
   rtc_config.interrupt_priority = 3;   // Lowest priority.
   rtc_config.reliable = false;
   rtc_config.tick_latency = RTC_US_TO_TICKS(NRF_MAXIMUM_LATENCY_US, RTC_TICK_FRQ);
   error_code = nrf_drv_rtc_init(&rtc, &rtc_config, os_rtc_evt_handler);

   // Enable tick event and interrupt.
   nrf_drv_rtc_tick_enable(&rtc, true);

   // Power on the RTC peripheral.
   nrf_drv_rtc_enable(&rtc);

   return error_code;
} 

static void os_rtc_evt_handler(nrf_drv_rtc_int_type_t int_type)
{
   // Check if Tick Interrupt.
   if (int_type == NRF_DRV_RTC_INT_TICK)
   {
      event |= EVENT_RTC_TICK;

      // TODO: Ugly and coarse way to convert to ms. FIX!
      os_rtc_time_ms = os_cumulative_rtc_ticks / 32;  // Convert to ms.
   }
}

void os_get_time_ms(uint32_t* timestamp)
{
   *timestamp = os_rtc_time_ms;
}
#endif   // RTC_INCLUDE

#if defined(BLE_INCLUDE)
bool os_ble_advertising_start(void)
{
    uint32_t err_code;

    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
    
    if (err_code == NRF_SUCCESS)
       return true;

    return false;
}

bool os_ble_advertising_stop(void)
{
   uint32_t err_code;

   err_code = sd_ble_gap_adv_stop();
   
    if (err_code == NRF_SUCCESS)
       return true;

    return false;
}

static bool os_ble_gap_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_UNKNOWN);
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);

    if (err_code == NRF_SUCCESS)
       return true;

    return false;
}

static bool os_ble_advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;

    // Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance      = true;
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    //advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    //advdata.uuids_complete.p_uuids  = m_adv_uuids;
    
    ble_adv_modes_config_t options = {0};
    options.ble_adv_fast_enabled  = BLE_ADV_FAST_ENABLED;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;
    
    err_code = ble_advertising_init(&advdata, NULL, &options, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    if (err_code == NRF_SUCCESS)
       return true;

    return false;
}

static void os_ble_event_handler(ble_evt_t* evt)
{
   event |= EVENT_BLE;

   switch (evt->header.evt_id)
   {
      case BLE_GAP_EVT_CONNECTED:
         ble_connection_handle = evt->evt.gap_evt.conn_handle;
         break;
      case BLE_GAP_EVT_DISCONNECTED:
         ble_connection_handle = BLE_CONN_HANDLE_INVALID;
         os_ble_advertising_start();   // TODO: App should handle this.
         break;
      case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
         sd_ble_gap_sec_params_reply(ble_connection_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
         break;
      case BLE_GAP_EVT_SEC_INFO_REQUEST:
         break;
      case BLE_GAP_EVT_SEC_REQUEST:
         break;
      case BLE_GAP_EVT_TIMEOUT:
         break;
      case BLE_GATTS_EVT_WRITE:
         break;
      case BLE_GATTS_EVT_SYS_ATTR_MISSING:
         sd_ble_gatts_sys_attr_set(ble_connection_handle, NULL, 0, 0);
         break;
      case BLE_GATTS_EVT_HVC:
         break;
      case BLE_GATTS_EVT_TIMEOUT:
        break;
      default:
         break;
   }

   #if defined(BLE_NUS_INCLUDE)
      ble_nus_on_ble_evt(&ble_nus, evt);
   #endif   // BLE_NUS_INCLUDE
}

#if defined(BLE_NUS_INCLUDE)
   bool os_ble_nus_send_data(uint8_t* data, uint16_t length)
   {
      if (ble_nus_string_send(&ble_nus, data, length) ==  NRF_SUCCESS)
         return true;

      return false;
   }
 
   static void os_ble_nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)
   {
      // Data received from Peer over BLE.
      
   }
#endif   /// BLE_NUS_INCLUDE
#endif   // BLE_INCLUDE


#if defined(UART_INCLUDE)
static void os_uart_init(void)
{
    uint32_t                     err_code;
    const app_uart_comm_params_t comm_params =
    {
        0,
        1,
        2,
        3,
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
        UART_BAUDRATE_BAUDRATE_Baud1M
    };

    APP_UART_INIT( &comm_params,
                    os_uart_event_handle,
                    APP_IRQ_PRIORITY_LOW,
                    err_code);
    APP_ERROR_CHECK(err_code);
}

bool os_uart_send_data(uint8_t* data, uint16_t length)
{
   for (uint16_t i = 0; i < length; i++)
   {
      while (app_uart_put(data[i]) != NRF_SUCCESS);
   }
   while(app_uart_put('\n') != NRF_SUCCESS);

   return true;
}

static void os_uart_event_handle(app_uart_evt_t * p_event)
{
    static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
    static uint8_t index = 0;

    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY:
            UNUSED_VARIABLE(app_uart_get(&data_array[index]));
            index++;
            break;

        case APP_UART_COMMUNICATION_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_communication);
            break;

        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;

        default:
            break;
    }
}
#endif   // UART_INCLUDE
