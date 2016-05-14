/***************************************************************************************
# Copyright (c) 2015,  Vipin Bakshi
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
–# You may obtain a copy of the License at
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

#define APP_FREQ         10   // App requested frequency. Derived from main Main RTC Clock Freq.

#if defined(NRF51_MPU9150)
   struct mpu_fifo_packet 
   {
      int16_t gyro[3];
      int16_t accel[3];
      uint32_t timestamp;
   };
#endif   // NRF51_MPU9150

#if defined(RTC_INCLUDE)
   static uint32_t rtc_counter;
#endif   // RTC_INCLUDE
#if defined(NRF51_MPU9150)
   static int result = 0xAA;
   static unsigned char *mpl_key = (unsigned char*)"eMPL 5.1";
   static long gyro_bias[3];
   static long accel_bias[3];
   static struct mpu_fifo_packet fifo_packet;
#endif   // NRF51_MPU9250


#if defined(RTC_INCLUDE)
static void app_rtc_handler(void)
{
   rtc_counter++;

   if (APP_HZ_EVENT(rtc_counter, APP_FREQ))
   {  
      #if defined(BLE_NUS_INCLUDE)
         #if defined(NRF51_MPU9150)
//            os_ble_nus_send_data((uint8_t*) &result, sizeof(result));
         #endif // NRF51_MPU9250
      #endif   // BLE_NUS_INCLUDE
      #if defined(UART_INCLUDE)
      os_uart_send_data((uint8_t*) &result, sizeof(result));
      #endif   // UART_INCLUDE

      rtc_counter = 0;
   }
}
#endif   // RTC_INCLUDE

static void app_ble_handler(void)
{
            
}

#if defined(PINT_INCLUDE)
static void app_pint_handler(void)
{
   // TODO: Make this call have more information regarding the pin.
   #if defined(NRF51_MPU9150)
      // Handle the Data Ready Interrupt.
      short int_status;
      result = mpu_get_int_status(&int_status);

      // Check if our FIFO Data is ready.
      if (int_status == MPU_INT_STATUS_DATA_READY)
      {
         uint8_t sensors;
         uint8_t more_data_count;
         result = mpu_read_fifo(fifo_packet.gyro, fifo_packet.accel, &fifo_packet.timestamp, &sensors, &more_data_count);
         
         
         // TODO: Handle if more data available in FIFO.
         // Check if accel data available.
         if (sensors & INV_XYZ_ACCEL)
         {
            // TODO: Push to algo.
         }
         
         if (sensors & INV_XYZ_GYRO)
         {
            // TODO: Push to algo.
         }
      }
   #endif // NRF51_MPU9150

   #if defined(BLE_NUS_INCLUDE)
      uint8_t fifo_data_to_send[20];
      memcpy(fifo_data_to_send, fifo_packet.gyro, sizeof(fifo_packet.gyro));
      memcpy(fifo_data_to_send + sizeof(fifo_packet.gyro), fifo_packet.accel, sizeof(fifo_packet.accel));
      memcpy(fifo_data_to_send + sizeof(fifo_packet.gyro) + sizeof(fifo_packet.accel), &fifo_packet.timestamp, sizeof(fifo_packet.timestamp));
      os_ble_nus_send_data(fifo_data_to_send, sizeof(fifo_data_to_send));
   #endif // BLE_NUS_INCLUDE
}
#endif   // PINT_INCLUDE

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
           struct int_param_s int_params;
           uint8_t temp;
           memset(&fifo_packet, 0, sizeof(fifo_packet));
           result = mpu_init(&int_params);
           result |= mpu_set_sensors(INV_XYZ_ACCEL | INV_XYZ_GYRO);    
           result |= mpu_configure_fifo(INV_XYZ_ACCEL | INV_XYZ_GYRO);     // Configure FIFO for Accel and Gyro. 
           result |= mpu_set_gyro_fsr(2000);   // Set to +-2000dps
           result |= mpu_set_accel_fsr(16);    // Set to +-16g
           result |= mpu_set_sample_rate(10);
           result |= mpu_run_self_test(gyro_bias, accel_bias);   // Calibrate Gyro and Accel.
           result |= mpu_set_gyro_bias_reg(gyro_bias);   // Push biases.
           result |= mpu_set_accel_bias_6050_reg(accel_bias);   // Push biases.
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
       app_pint_handler(); 
       break;
     #endif  // PINT_INCLUDE
         
      default:
         break;
   }

   return true;
}
