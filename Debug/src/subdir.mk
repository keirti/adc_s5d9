################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc_thread.c \
../src/adc_thread_entry.c \
../src/hal_entry.c \
../src/led_thread.c \
../src/main.c \
../src/threadx_interface.c 

OBJS += \
./src/adc_thread.o \
./src/adc_thread_entry.o \
./src/hal_entry.o \
./src/led_thread.o \
./src/main.o \
./src/threadx_interface.o 

C_DEPS += \
./src/adc_thread.d \
./src/adc_thread_entry.d \
./src/hal_entry.d \
./src/led_thread.d \
./src/main.d \
./src/threadx_interface.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	F:\Renesas\e2_studio\Utilities\\/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy_cfg\ssp_cfg\bsp" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy_cfg\ssp_cfg\driver" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy\ssp\inc" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy\ssp\inc\bsp" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy\ssp\inc\driver\api" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy\ssp\inc\driver\instances" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\src" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\src\synergy_gen" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy_cfg\ssp_cfg\framework" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy\ssp\inc\framework\api" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy\ssp\inc\framework\instances" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy_cfg\ssp_cfg\framework\el" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy\ssp\inc\framework\el" -I"C:\Users\cjg19\Documents\GitHub\adc_s5d9\synergy\ssp\src\framework\el\tx" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


