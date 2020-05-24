################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../port/demo.c 

OBJS += \
./port/demo.o 

C_DEPS += \
./port/demo.d 


# Each subdirectory must supply rules for building sources it contributes
port/%.o: ../port/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	C:\Renesas\Synergy\e2studio_v7.5.1_ssp_v1.7.5\Utilities\\/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"C:\Users\kirti\PK_S5D9\verify\synergy_cfg\ssp_cfg\bsp" -I"C:\Users\kirti\PK_S5D9\verify\synergy_cfg\ssp_cfg\driver" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc\bsp" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc\driver\api" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc\driver\instances" -I"C:\Users\kirti\PK_S5D9\verify\src" -I"C:\Users\kirti\PK_S5D9\verify\src\synergy_gen" -I"C:\Users\kirti\PK_S5D9\verify\synergy_cfg\ssp_cfg\framework\el" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\inc\framework\el" -I"C:\Users\kirti\PK_S5D9\verify\synergy\ssp\src\framework\el\tx" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


