################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/port/port/portevent.c \
../src/port/port/portother.c \
../src/port/port/portserial.c \
../src/port/port/porttimer.c 

OBJS += \
./src/port/port/portevent.o \
./src/port/port/portother.o \
./src/port/port/portserial.o \
./src/port/port/porttimer.o 

C_DEPS += \
./src/port/port/portevent.d \
./src/port/port/portother.d \
./src/port/port/portserial.d \
./src/port/port/porttimer.d 


# Each subdirectory must supply rules for building sources it contributes
src/port/port/%.o: ../src/port/port/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	C:\Renesas\Synergy\e2studio_v7.5.1_ssp_v1.7.5\Utilities\\/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"C:\Users\kirti\PK_S5D9\verify\synergy_cfg\ssp_cfg\bsp" -I"C:\Users\kirti\PK_S5D9\verify\synergy_cfg\ssp_cfg\driver" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc\bsp" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc\driver\api" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc\driver\instances" -I"C:\Users\kirti\PK_S5D9\verify\src" -I"C:\Users\kirti\PK_S5D9\verify\src\synergy_gen" -I"C:\Users\kirti\PK_S5D9\verify\synergy_cfg\ssp_cfg\framework\el" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc\framework\el" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\src\framework\el\tx" -I"C:\Users\kirti\PK_S5D9\verify\src\modbus" -I"C:\Users\kirti\PK_S5D9\verify\src\port" -I"C:\Users\kirti\PK_S5D9\verify\src\port\port" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


