################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/freemb/modbus/functions/mbfunccoils.c \
../src/freemb/modbus/functions/mbfuncdiag.c \
../src/freemb/modbus/functions/mbfuncdisc.c \
../src/freemb/modbus/functions/mbfuncholding.c \
../src/freemb/modbus/functions/mbfuncinput.c \
../src/freemb/modbus/functions/mbfuncother.c \
../src/freemb/modbus/functions/mbutils.c 

OBJS += \
./src/freemb/modbus/functions/mbfunccoils.o \
./src/freemb/modbus/functions/mbfuncdiag.o \
./src/freemb/modbus/functions/mbfuncdisc.o \
./src/freemb/modbus/functions/mbfuncholding.o \
./src/freemb/modbus/functions/mbfuncinput.o \
./src/freemb/modbus/functions/mbfuncother.o \
./src/freemb/modbus/functions/mbutils.o 

C_DEPS += \
./src/freemb/modbus/functions/mbfunccoils.d \
./src/freemb/modbus/functions/mbfuncdiag.d \
./src/freemb/modbus/functions/mbfuncdisc.d \
./src/freemb/modbus/functions/mbfuncholding.d \
./src/freemb/modbus/functions/mbfuncinput.d \
./src/freemb/modbus/functions/mbfuncother.d \
./src/freemb/modbus/functions/mbutils.d 


# Each subdirectory must supply rules for building sources it contributes
src/freemb/modbus/functions/%.o: ../src/freemb/modbus/functions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	F:\Renesas\e2_studio\Utilities\\/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"K:\GitHub\adc_s5d9\EMS\include" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\driver" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\bsp" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\bsp\cmsis\Include" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\driver\api" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\driver\instances" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\bsp" -I"K:\GitHub\adc_s5d9\EMS\src" -I"K:\GitHub\adc_s5d9\EMS\src\synergy_gen" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\framework\el" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\el" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\tx" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\framework" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\api" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\instances" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\el\nx" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nx" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nx_md5" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\el\nx_application_layer" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp_supplemental\filex_stub" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\sf_el_nx\phy" -I"K:\GitHub\adc_s5d9\EMS\src\freemb\demo\port" -I"K:\GitHub\adc_s5d9\EMS\src\freemb\modbus" -I"K:\GitHub\adc_s5d9\EMS\src\freemb\modbus\functions" -I"K:\GitHub\adc_s5d9\EMS\src\freemb\modbus\rtu" -I"K:\GitHub\adc_s5d9\EMS\src\freemb\modbus\include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


