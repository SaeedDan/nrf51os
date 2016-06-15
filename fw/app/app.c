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

#include "six_axis_comp_filter.h"

#define APP_FREQ         10   // App requested frequency. Derived from main Main RTC Clock Freq.
#define MPU_SAMPLE_RATE  100
#define MPU_SAMPLE_PERIOD ((float)1/(float)MPU_SAMPLE_RATE)
#define MPU_ACCEL_FSR    16
#define MPU_GYRO_FSR     2000

#if defined(NRF51_MPU9150)
   struct mpu_fifo_packet 
   {
      int16_t gyro[3];
      int16_t accel[3];
      uint32_t timestamp;
      int32_t accel_offset[3];
      int32_t gyro_offset[3];
      uint32_t calibration_accel_samples;
      uint32_t calibration_gyro_samples;
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
   static int accel_sens;
   static float gyro_sens;
   static float accelx_mps2;
   static float accely_mps2;
   static float accelz_mps2;
   static float gyrox_dps;
   static float gyroy_dps;
   static float gyroz_dps;
   static uint16_t is_calibration_in_progress;
#endif   // NRF51_MPU9250
#if defined(COMPFILTER_INCLUDE)
   static SixAxis compFilter;
   static bool is_first_sample;
   static float xRollAngle;
   static float yPitchAngle;
   static float zYawAngle;
#endif   // COMPFILTER_INCLUDE


#if defined(RTC_INCLUDE)
static void app_rtc_handler(void)
{
   rtc_counter++;

   if (APP_HZ_EVENT(rtc_counter, APP_FREQ))
   {  
      #if defined(BLE_NUS_INCLUDE)
         #if defined(NRF51_MPU9150)
            // os_ble_nus_send_data((uint8_t*) &result, sizeof(result));
         #endif // NRF51_MPU9250
      #endif   // BLE_NUS_INCLUDE
      #if defined(UART_INCLUDE)
         os_uart_send_data((uint8_t*) &result, sizeof(result));
      #endif   // UART_INCLUDE

      rtc_counter = 0;

      #if defined(NRF51_MPU9150)
         // Check if calibration is requested.
         if (is_calibration_in_progress)
         {
            is_calibration_in_progress--;

            // Check if calibration time is over.
            if (!is_calibration_in_progress)
            {
               // Calibration is over. Compute offsets.
               fifo_packet.accel_offset[0] /= fifo_packet.calibration_accel_samples;
               fifo_packet.accel_offset[1] /= fifo_packet.calibration_accel_samples;
               fifo_packet.accel_offset[2] /= fifo_packet.calibration_accel_samples;
               fifo_packet.gyro_offset[0] /= fifo_packet.calibration_gyro_samples;
               fifo_packet.gyro_offset[1] /= fifo_packet.calibration_gyro_samples;
               fifo_packet.gyro_offset[2] /= fifo_packet.calibration_gyro_samples;
            }
         }
      #endif // #if defined(NRF51_MPU9150)
      
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
      uint8_t fifo_data_to_send[20];

      memset(fifo_data_to_send, 0, sizeof(fifo_data_to_send));
      mpu_get_int_status(&int_status);
      
      // Check if our FIFO Data is ready.
      if (int_status == MPU_INT_STATUS_DATA_READY)
      {
         uint8_t sensors;
         uint8_t more_data_count;
         mpu_read_fifo(fifo_packet.gyro, fifo_packet.accel, &fifo_packet.timestamp, &sensors, &more_data_count);

         #if defined(COMPFILTER_INCLUDE)
            // TODO: Handle if more data available in FIFO.
            // Check if accel data available.
            if (sensors & INV_XYZ_ACCEL)
            {
               if (is_calibration_in_progress)
               {
                  fifo_packet.accel_offset[0] += fifo_packet.accel[0];
                  fifo_packet.accel_offset[1] += fifo_packet.accel[1];
                  fifo_packet.accel_offset[2] += fifo_packet.accel[2];
                  fifo_packet.calibration_accel_samples++;
               }
               
               accelx_mps2 = (float)(((float) fifo_packet.accel[0] - fifo_packet.accel_offset[0]) * 9.81) / (float)(accel_sens);
               accely_mps2 = (float)(((float) fifo_packet.accel[1] - fifo_packet.accel_offset[1]) * 9.81) / (float)(accel_sens);
               accelz_mps2 = (float)(((float) fifo_packet.accel[2] - fifo_packet.accel_offset[2]) * 9.81) / (float)(accel_sens);
               CompAccelUpdate(&compFilter,accelx_mps2, accely_mps2, accelz_mps2);
            }
         
            if (sensors & INV_XYZ_GYRO)
            {
               if (is_calibration_in_progress)
               {
                  fifo_packet.gyro_offset[0] += fifo_packet.gyro[0];
                  fifo_packet.gyro_offset[1] += fifo_packet.gyro[1];
                  fifo_packet.gyro_offset[2] += fifo_packet.gyro[2];
                  fifo_packet.calibration_gyro_samples++;

                  // return here since we only want to use calibrated data for filter.
                  return;
               }
               
               gyrox_dps = (float)((float) fifo_packet.gyro[0]  - fifo_packet.gyro_offset[0]) / (gyro_sens);
               gyroy_dps = (float)((float) fifo_packet.gyro[1]  - fifo_packet.gyro_offset[1]) / (gyro_sens);
               gyroz_dps = (float)((float) fifo_packet.gyro[2]  - fifo_packet.gyro_offset[2]) / (gyro_sens);
               CompGyroUpdate(&compFilter, CompDegreesToRadians(gyrox_dps), CompDegreesToRadians(gyroy_dps), CompDegreesToRadians(gyroz_dps));
            }

            // Check if first sample.
            if (is_first_sample)
            {
               is_first_sample = false;
               CompStart(&compFilter);
            }
            
            CompUpdate(&compFilter);
            CompAnglesGet(&compFilter, &xRollAngle, &yPitchAngle, 0);
            xRollAngle = CompRadiansToDegrees(xRollAngle);
            yPitchAngle = CompRadiansToDegrees(yPitchAngle);
            zYawAngle = gyroz_dps * MPU_SAMPLE_RATE;
         #endif   // COMPFILTER_INCLUDE

         #if defined(BLE_NUS_INCLUDE)
            // Send accel and gyro calculate values.
            fifo_data_to_send[0] = 1;
            memcpy(fifo_data_to_send + sizeof(uint8_t), &accelx_mps2, sizeof(float));
            memcpy(fifo_data_to_send + sizeof(uint8_t) + sizeof(float), &accely_mps2, sizeof(float));
            memcpy(fifo_data_to_send + sizeof(uint8_t) + sizeof(float) + sizeof(float), &accelz_mps2, sizeof(float));
            memcpy(fifo_data_to_send + sizeof(uint8_t) + sizeof(float) + sizeof(float) + sizeof(float), &fifo_packet.timestamp, sizeof(fifo_packet.timestamp)); 
            os_ble_nus_send_data(fifo_data_to_send, sizeof(fifo_data_to_send));        

            fifo_data_to_send[0] = 2;
            memcpy(fifo_data_to_send + sizeof(uint8_t), &gyrox_dps, sizeof(float));
            memcpy(fifo_data_to_send + sizeof(uint8_t) + sizeof(float), &gyroy_dps, sizeof(float));
            memcpy(fifo_data_to_send + sizeof(uint8_t) + sizeof(float) + sizeof(float), &gyroz_dps, sizeof(float));
            memcpy(fifo_data_to_send + sizeof(uint8_t) + sizeof(float) + sizeof(float) + sizeof(float), &fifo_packet.timestamp, sizeof(fifo_packet.timestamp));
            os_ble_nus_send_data(fifo_data_to_send, sizeof(fifo_data_to_send));
                
            // Send Angles.
            fifo_data_to_send[0] = 3;
            memcpy(fifo_data_to_send + sizeof(uint8_t), &xRollAngle, sizeof(float));
            memcpy(fifo_data_to_send + sizeof(uint8_t) + sizeof(float), &yPitchAngle, sizeof(float));
            memcpy(fifo_data_to_send + sizeof(uint8_t) + sizeof(float) + sizeof(float), &zYawAngle, sizeof(float));
            memcpy(fifo_data_to_send + sizeof(uint8_t) + sizeof(float) + sizeof(float) + sizeof(float), &fifo_packet.timestamp, sizeof(fifo_packet.timestamp));
            os_ble_nus_send_data(fifo_data_to_send, sizeof(fifo_data_to_send));
         #endif // BLE_NUS_INCLUDE
      }
     
   #endif // NRF51_MPU9150
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
           result |= mpu_set_gyro_fsr(MPU_GYRO_FSR);   // Set to +-2000dps
           result |= mpu_set_accel_fsr(MPU_ACCEL_FSR);    // Set to +-16g
           result |= mpu_set_sample_rate(MPU_SAMPLE_RATE);
           //result |= mpu_run_self_test(gyro_bias, accel_bias);  // Calibrate Gyro and Accel.
           //result |= mpu_set_gyro_bias_reg(gyro_bias);          // Push biases.
           //result |= mpu_set_accel_bias_6050_reg(accel_bias);   // Push biases.
           result |= mpu_get_accel_sens(&accel_sens);
           result |= mpu_get_gyro_sens(&gyro_sens);
           is_calibration_in_progress = 5 * APP_FREQ;
        #endif // NRF51_MPU9250
        #if defined(COMPFILTER_INCLUDE)
           is_first_sample = true;
           CompInit(&compFilter, MPU_SAMPLE_PERIOD, 1);   // TODO: Think about the tao value.
        #endif // COMPFILTER_INCLUDE
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

#if defined(NRF51_MPU9150)
void app_calibrate_sensor()
{
   
}
#endif   
