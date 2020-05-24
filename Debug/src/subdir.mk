################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Leds_entry.c \
../src/adc_interface.c \
../src/adc_thread_entry.c \
../src/hal_entry.c \
../src/thread0_entry.c \
../src/watchdog_entry.c 

OBJS += \
./src/Leds_entry.o \
./src/adc_interface.o \
./src/adc_thread_entry.o \
./src/hal_entry.o \
./src/thread0_entry.o \
./src/watchdog_entry.o 

C_DEPS += \
./src/Leds_entry.d \
./src/adc_interface.d \
./src/adc_thread_entry.d \
./src/hal_entry.d \
./src/thread0_entry.d \
./src/watchdog_entry.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	F:\Renesas\e2_studio\Utilities\\/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"K:\GitHub\adc_s5d9\EMS\include" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\driver" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\bsp" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\bsp\cmsis\Include" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\driver\api" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\driver\instances" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\bsp" -I"K:\GitHub\adc_s5d9\EMS\src" -I"K:\GitHub\adc_s5d9\EMS\src\synergy_gen" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\framework\el" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\el" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\tx" -I"K:\GitHub\adc_s5d9\EMS\src\freemb\demo\port" -I"K:\GitHub\adc_s5d9\EMS\src\freemb\modbus" -I"K:\GitHub\adc_s5d9\EMS\src\freemb\modbus\functions" -I"K:\GitHub\adc_s5d9\EMS\src\freemb\modbus\rtu" -I"K:\GitHub\adc_s5d9\EMS\src\freemb\modbus\include" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\framework" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\api" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\instances" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


