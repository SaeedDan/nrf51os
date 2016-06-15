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

#include "hw.h"
#include "hw_config.h"
#include "nrf_gpio.h"

void hw_init(void)
{
   // Set Outputs.
   nrf_gpio_pin_set(GPS_NRESET);
   nrf_gpio_pin_set(MEM_NCS);
   
   // Configure Ouputs.
   nrf_gpio_range_cfg_output(0, 30);

    // Configure Inputs.
   nrf_gpio_cfg_input(MPR_INT2, GPIO_PIN_CNF_PULL_Disabled);   
   nrf_gpio_cfg_input(MPR_INT1, GPIO_PIN_CNF_PULL_Disabled);
   nrf_gpio_cfg_input(MPU_INT, GPIO_PIN_CNF_PULL_Disabled);
}
