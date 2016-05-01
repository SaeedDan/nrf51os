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
INCS    += -I./peripherals/ 
INCS 	+= -I../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/gcc/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/util/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/common/softdevice_handler/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/scheduler/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/s310/headers/ 
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/device/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/hal/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/rtc/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/config/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/common/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/pstorage/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/pstorage/config/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/ble_flash/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/ble/common/
INCS    += -I../resources/nRF51_SDK_9.0.0_2e23562/components/ble/ble_advertising/
SRCS    += ./
SRCS     = $(notdir $(wildcard ./app/*.c))
SRCS    += $(notdir $(wildcard ./bsp/*.c))
SRCS    += $(notdir $(wildcard ./os/*.c))
SRCS    += $(notdir $(wildcard ./peripherals/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/system_nrf51422.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/util/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/common/softdevice_handler/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/scheduler/app_scheduler.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/rtc/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/common/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/pstorage/pstorage.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/ble_flash/*.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/ble/common/ble_advdata.c))
SRCS    += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/ble/ble_advertising/*.c))
SRCS_AS += $(notdir $(wildcard ../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/gcc/*.s))
OBJS     = $(SRCS:.c=.o)
OBJS_AS  = $(SRCS_AS:.s=.o)
SRC_PTH += ./
SRC_PTH += ./app/
SRC_PTH += ./bsp/
SRC_PTH += ./os/
SRC_PTH += ./peripherals/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/util/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/common/softdevice_handler/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/libraries/scheduler/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/device/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/rtc/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/common/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/pstorage/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/drivers_nrf/ble_flash/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/ble/common/
SRC_PTH += ../resources/nRF51_SDK_9.0.0_2e23562/components/ble/ble_advertising/
SRC_PTH_AS += ../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/gcc/
L_COMMON += ../resources/nRF51_SDK_9.0.0_2e23562/components/toolchain/gcc/
L_SCRIPT = ../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/s310/toolchain/armgcc/armgcc_s310_nrf51422_xxaa.ld


# Flags
CFLAGS   = -mcpu=cortex-m0 -mthumb -mabi=aapcs --std=gnu99 -Wall -DNRF51 -DBLE_STACK_SUPPORT_REQD -DSOFTDEVICE_PRESENT
LDFLAGS  = -mcpu=cortex-m0 -mthumb -mabi=aapcs -L$(L_COMMON) -T$(L_SCRIPT) -Xlinker -Map=$(PRJ)/$(PRJ).map


# Source Lookup Paths
vpath %.c $(SRC_PTH)
vpath %.s $(SRC_PTH_AS)


# Execution
all: clean makedir $(OBJS) $(OBJS_AS) moveobjects linkobjects elftohex printsize flash

clean:
	$(RM) ./*.o
	$(RM) $(PRJ)

makedir:
	mkdir $(PRJ)

%.o:%.c
	$(ARM_CC) $(CFLAGS) $(INCS) -c -o  $@ $<
	@echo $(SRCS)

%.o:%.s
	$(ARM_AS) -c -o  $@ $<
	@echo $(SRCS_AS)

moveobjects:
	$(MV) ./*.o ./$(PRJ)

linkobjects:
	$(ARM_LD) $(LDFLAGS) ./$(PRJ)/*.o -o ./$(PRJ)/$(PRJ).elf

elftohex:
	$(ARM_OBJCPY) -O ihex ./$(PRJ)/$(PRJ).elf ./$(PRJ)/$(PRJ).hex

printsize:
	$(ARM_SIZE)	--format=SysV ./$(PRJ)/$(PRJ).elf

flash:
	@echo Flashing s310_softdevice.hex
	$(NRFJPROG) --program ../resources/nRF51_SDK_9.0.0_2e23562/components/softdevice/s310/hex/s310_softdevice.hex --sectorerase --verify
	@echo Flashing $(PRJ)/$(PRJ).hex
	$(NRFJPROG) --program $(PRJ)/$(PRJ).hex -f nrf51 --sectorerase --verify
	$(NRFJPROG) --reset
	$(NRFJPROG) --run
