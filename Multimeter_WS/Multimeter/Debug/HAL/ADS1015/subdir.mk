################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/ADS1015/ads1015.c 

OBJS += \
./HAL/ADS1015/ads1015.o 

C_DEPS += \
./HAL/ADS1015/ads1015.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/ADS1015/%.o: ../HAL/ADS1015/%.c HAL/ADS1015/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


