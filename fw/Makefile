######################################################################################
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
######################################################################################

# GNU ARM Toolchain Setup
ARM_GCC_BIN = $(ARM_GCC_LOC)/bin
ARM_GCC_LIB = $(ARM_GCC_LOC)/arm_none_eabi/lib
ARM_GCC_INC = $(ARM_GCC_LOC)/arm_none_eabi/include
ARM_AS      = $(ARM_GCC_BIN)/arm-none-eabi-as
ARM_CC      = $(ARM_GCC_BIN)/arm-none-eabi-gcc
ARM_LD      = $(ARM_GCC_BIN)/arm-none-eabi-gcc
ARM_OBJCPY  = $(ARM_GCC_BIN)/arm-none-eabi-objcopy
ARM_SIZE    = $(ARM_GCC_BIN)/arm-none-eabi-size

# Platform Specific Commands
ifeq ($(OS),Windows_NT)
	RM 		= ..\resources\GnuWin32\bin\rm.exe -rf
	MV 		= ..\resources\GnuWin32\bin\mv.exe
	NRFJPROG	= nrfjprog
	ARM_GCC_LOC     = ../resources/gcc-arm-none-eabi-4_9-2015q3-20150921-win32/gcc-arm-none-eabi-4_9-2015q3
else
	UNAME_S		= $(shell uname -s)
	UNAME_V		= $(shell uname -v)
	RM 	  	= rm -rf
	MV 	  	= mv

	ifeq ($(UNAME_S), Darwin)
		NRFJPROG    	= ../resources/nRF5x-CommandLineTools/nRF5x-Command-Line-Tools_8_4_0_OSX/nrfjprog/nrfjprog
		ARM_GCC_LOC     = ../resources/gcc-arm-none-eabi-4_9-2015q3-20150921-mac/gcc-arm-none-eabi-4_9-2015q3
	else
		ifeq ($(UNAME_P), x86_64)
			NRFJPROG    	= ../resources/nRF5x-CommandLineTools/nRF5x-Command-Line-Tools_8_4_0-Linux-x86_64/nrfjprog/nrfjprog
			ARM_GCC_LOC     = ../resources/gcc-arm-none-eabi-4_9-2015q3-20150921-linux/gcc-arm-none-eabi-4_9-2015q3
		else
			NRFJPROG    	= ../resources/nRF5x-CommandLineTools/nRF5x-Command-Line-Tools_8_4_0-Linux_i386/nrfjprog/nrfjprog
			ARM_GCC_LOC     = ../resources/gcc-arm-none-eabi-4_9-2015q3-20150921-linux/gcc-arm-none-eabi-4_9-2015q3
		endif
	endif
endif


# Source Collection
PRJ      = _cyccmp
INCS    += -I.
INCS    += -I./app/
INCS    += -I./bsp/
INCS    += -I./os/
INCS    += -I./external/mpu9250/
INCS    += -I./external/mpu9250/mpl/
INCS    += -I./external/mpu9250/mllite/
INCS    += -I./external/mpu9250/include/
INCS    += -I./external/comp_filter/Six_Axis_Complementary_Filter/C/
#INCS    += -I./external/mpu9250/USB_Common/
#INCS    += -I./external/mpu9250/USB_Common/include/
INCS 	+= -I../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/gcc/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/util/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/common/softdevice_handler/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/scheduler/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/s110/headers/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/device/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/hal/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/rtc/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/config/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/common/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/pstorage/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/pstorage/config/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/ble_flash/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/uart/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/gpiote/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/twi_master/incubated/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/twi_master/incubated/config/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/ble/common/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/ble/ble_advertising/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/ble/ble_services/ble_nus/
SRCS    += ./
SRCS     = $(notdir $(wildcard ./app/*.c))
SRCS    += $(notdir $(wildcard ./bsp/*.c))
SRCS    += $(notdir $(wildcard ./os/*.c))
SRCS    += $(notdir $(wildcard ./peripherals/*.c))
SRCS    += $(notdir $(wildcard ./external/mpu9250/*.c))
SRCS    += $(notdir $(wildcard ./external/mpu9250/mllite/*.c))
SRCS    += $(notdir $(wildcard ./external/comp_filter/Six_Axis_Complementary_Filter/C/*.c))
#SRCS    += $(notdir $(wildcard ./external/mpu9250/USB_Common/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/system_nrf51422.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/util/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/common/softdevice_handler/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/scheduler/app_scheduler.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/hal/nrf_delay.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/rtc/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/common/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/pstorage/pstorage.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/ble_flash/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/uart/app_uart.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/gpiote/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/twi_master/incubated/twi_hw_master.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/ble/common/ble_advdata.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/ble/ble_advertising/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/ble/ble_services/ble_nus/*.c))
SRCS_AS += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/gcc/*.s))
OBJS     = $(SRCS:.c=.o)
OBJS_AS  = $(SRCS_AS:.s=.o)
SRC_PTH += ./
SRC_PTH += ./app/
SRC_PTH += ./bsp/
SRC_PTH += ./os/
SRC_PTH += ./peripherals/
SRC_PTH += ./external/mpu9250/
SRC_PTH += ./external/mpu9250/mpl/
SRC_PTH += ./external/mpu9250/mllite/
#SRC_PTH += ./external/mpu9250/USB_Common/
#SRC_PTH += ./external/mpu9250/USB_Common/include/
SRC_PTH += ./external/mpu9250/include/
SRC_PTH += ./external/comp_filter/Six_Axis_Complementary_Filter/C/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/util/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/common/softdevice_handler/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/scheduler/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/device/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/hal/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/rtc/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/common/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/pstorage/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/ble_flash/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/uart/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/gpiote/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/twi_master/incubated/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/twi_master/incubated/config/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/ble/common/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/ble/ble_advertising/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/ble/ble_services/ble_nus/
SRC_PTH_AS += ../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/gcc/
L_COMMON     = ../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/gcc/
L_9150_MPL   = ./external/mpu9250/mpl
L_SCRIPT = ../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/s110/toolchain/armgcc/armgcc_s110_nrf51422_xxaa.ld


# Flags
CFLAGS   = -mcpu=cortex-m0 -mthumb -mabi=aapcs --std=gnu99 -Wall  -mfloat-abi=soft -ffunction-sections -fdata-sections -fno-strict-aliasing  -fno-builtin --short-enums
CFLAGS   += -DNRF51  -DSOFTDEVICE_PRESENT -DBLE_STACK_SUPPORT_REQD -DS110
#CFLAGS   += -DEMPL -DUSE_DMP -DMPU9250 -DREMOVE_LOGGING -DMPL_LPG_NDEBUG=1
CFLAGS   += -DEMPL -DUSE_DMP -DMPU9150 -DREMOVE_LOGGING -DMPL_LOG_NDEBUG=0
AFLAGS   = -x assembler-with-cpp -DSOFTDEVICE_PRESENT -DNRF51 -DBLE_STACK_SUPPORT_REQD -DS110 -DNRF51 -DEMPL -DUSE_DMP -DMPU9150 -DREMOVE_LOGGING  -DMPL_LPG_NDEBUG=0
LDFLAGS = -mcpu=cortex-m0 -mthumb -mabi=aapcs -L$(L_9150_MPL) -L$(L_COMMON) -T$(L_SCRIPT) -Xlinker -Map=$(PRJ)/$(PRJ).map -Wl,--gc-sections #-nostartfiles -nodefaultlibs -nostdlib

# Source Lookup Paths
vpath %.c $(SRC_PTH)
vpath %.s $(SRC_PTH_AS)


# Execution
all: clean makedir $(OBJS) $(OBJS_AS) moveobjects linkobjects elftohex printsize flash_app

clean:
	$(RM) ./*.o
	$(RM) $(PRJ)

makedir:
	mkdir $(PRJ)

%.o:%.c
	$(ARM_CC) $(CFLAGS) $(INCS) -c -o  $@ $< 
	@echo $(SRCS)

%.o:%.s
	$(ARM_CC) $(AFLAGS) -c -o  $@ $< 
	@echo $(SRCS_AS)

moveobjects:
	$(MV) ./*.o ./$(PRJ)

linkobjects:
	$(ARM_LD) ./$(PRJ)/*.o  -o ./$(PRJ)/$(PRJ).elf $(LDFLAGS) 

elftohex:
	$(ARM_OBJCPY) -O ihex ./$(PRJ)/$(PRJ).elf ./$(PRJ)/$(PRJ).hex 

printsize:
	$(ARM_SIZE)	--format=SysV ./$(PRJ)/$(PRJ).elf

flash_app:
	@echo Flashing $(PRJ)/$(PRJ).hex
	$(NRFJPROG) --program $(PRJ)/$(PRJ).hex -f nrf51 --sectorerase --verify
	$(NRFJPROG) --reset
	$(NRFJPROG) --run

flash_sd:
	@echo Flashing s110_softdevice.hex
	$(NRFJPROG) --program ../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/s110/hex/s110_softdevice.hex --sectorerase --verify
