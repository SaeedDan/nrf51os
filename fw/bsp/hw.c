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
   // For this template project. Configure all pins as output.
   // For a custom designed board, define board pin configuration
   // in hw_confuguration.h and initialize it accordingly usign the
   // nrf_gpio APIs.
   nrf_gpio_range_cfg_output(0, 30);
}
