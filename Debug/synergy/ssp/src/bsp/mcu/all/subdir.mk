################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/bsp/mcu/all/bsp_common.c \
../synergy/ssp/src/bsp/mcu/all/bsp_common_leds.c \
../synergy/ssp/src/bsp/mcu/all/bsp_delay.c \
../synergy/ssp/src/bsp/mcu/all/bsp_irq.c \
../synergy/ssp/src/bsp/mcu/all/bsp_locking.c \
../synergy/ssp/src/bsp/mcu/all/bsp_register_protection.c \
../synergy/ssp/src/bsp/mcu/all/bsp_sbrk.c 

OBJS += \
./synergy/ssp/src/bsp/mcu/all/bsp_common.o \
./synergy/ssp/src/bsp/mcu/all/bsp_common_leds.o \
./synergy/ssp/src/bsp/mcu/all/bsp_delay.o \
./synergy/ssp/src/bsp/mcu/all/bsp_irq.o \
./synergy/ssp/src/bsp/mcu/all/bsp_locking.o \
./synergy/ssp/src/bsp/mcu/all/bsp_register_protection.o \
./synergy/ssp/src/bsp/mcu/all/bsp_sbrk.o 

C_DEPS += \
./synergy/ssp/src/bsp/mcu/all/bsp_common.d \
./synergy/ssp/src/bsp/mcu/all/bsp_common_leds.d \
./synergy/ssp/src/bsp/mcu/all/bsp_delay.d \
./synergy/ssp/src/bsp/mcu/all/bsp_irq.d \
./synergy/ssp/src/bsp/mcu/all/bsp_locking.d \
./synergy/ssp/src/bsp/mcu/all/bsp_register_protection.d \
./synergy/ssp/src/bsp/mcu/all/bsp_sbrk.d 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/bsp/mcu/all/%.o: ../synergy/ssp/src/bsp/mcu/all/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	F:\Renesas\e2_studio\Utilities\\/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\bsp" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\driver" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\bsp" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\bsp\cmsis\Include" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\driver\api" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\driver\instances" -I"K:\GitHub\adc_s5d9\EMS\src" -I"K:\GitHub\adc_s5d9\EMS\src\synergy_gen" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\framework" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\api" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\instances" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\sf_el_nx\phy" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\el\nxd_application_layer" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\sf_el_nx_crypto" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nxd_application_layer\nxd_crypto_generic" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\el\nxd" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nxd" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nxd\nxd_src" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nxd_application_layer\nxd_mqtt_client" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nxd_application_layer\nxd_tls_secure" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\framework\el" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\el" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\tx" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\ux" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\tx\tx_src" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


