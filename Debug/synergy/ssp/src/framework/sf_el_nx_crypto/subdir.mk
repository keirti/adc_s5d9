################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_aes_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_ec_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_ecdh_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_ecdsa_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_hmac_md5_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_hmac_sha1_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_hmac_sha2_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_md5_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_phash_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_rng_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_rsa_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_sha1_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_sha2_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_synergys7_ciphersuites.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_tls_prf_1_sce.c \
../synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_tls_prf_sha256_sce.c 

OBJS += \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_aes_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_ec_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_ecdh_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_ecdsa_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_hmac_md5_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_hmac_sha1_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_hmac_sha2_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_md5_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_phash_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_rng_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_rsa_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_sha1_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_sha2_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_synergys7_ciphersuites.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_tls_prf_1_sce.o \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_tls_prf_sha256_sce.o 

C_DEPS += \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_aes_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_ec_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_ecdh_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_ecdsa_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_hmac_md5_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_hmac_sha1_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_hmac_sha2_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_md5_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_phash_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_rng_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_rsa_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_sha1_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_sha2_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_synergys7_ciphersuites.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_tls_prf_1_sce.d \
./synergy/ssp/src/framework/sf_el_nx_crypto/nx_crypto_tls_prf_sha256_sce.d 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/framework/sf_el_nx_crypto/%.o: ../synergy/ssp/src/framework/sf_el_nx_crypto/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	F:\Renesas\e2_studio\Utilities\\/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -D_RENESAS_SYNERGY_ -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\bsp" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\driver" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\bsp" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\bsp\cmsis\Include" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\driver\api" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\driver\instances" -I"K:\GitHub\adc_s5d9\EMS\src" -I"K:\GitHub\adc_s5d9\EMS\src\synergy_gen" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\framework" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\api" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\instances" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\sf_el_nx\phy" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\el\nxd_application_layer" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\sf_el_nx_crypto" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nxd_application_layer\nxd_crypto_generic" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\el\nxd" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nxd" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nxd\nxd_src" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nxd_application_layer\nxd_mqtt_client" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\nxd_application_layer\nxd_tls_secure" -I"K:\GitHub\adc_s5d9\EMS\synergy_cfg\ssp_cfg\framework\el" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\inc\framework\el" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\tx" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\ux" -I"K:\GitHub\adc_s5d9\EMS\synergy\ssp\src\framework\el\tx\tx_src" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


