################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include MCAL/WIRE/subdir.mk
-include MCAL/TIMER1/subdir.mk
-include MCAL/I2C/subdir.mk
-include MCAL/GPIO/subdir.mk
-include MCAL/ADC/subdir.mk
-include HAL/VOLTMETER/subdir.mk
-include HAL/OHMMETER/subdir.mk
-include HAL/MODE_SELECTOR/subdir.mk
-include HAL/LM35_SENSOR/subdir.mk
-include HAL/LCD/subdir.mk
-include HAL/INDUCTANCE_METER/subdir.mk
-include HAL/CURRENT_METER/subdir.mk
-include HAL/CAPACITANCE_METER/subdir.mk
-include HAL/BUZZER/subdir.mk
-include HAL/ADS1015/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Multimeter
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Multimeter.lss \

FLASH_IMAGE += \
Multimeter.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: main-build

# Main-build Target
main-build: Multimeter.elf secondary-outputs

# Tool invocations
Multimeter.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Multimeter.map -mmcu=atmega32 -o "Multimeter.elf" $(OBJS) $(USER_OBJS) $(LIBS)  -lm
	@echo 'Finished building target: $@'
	@echo ' '

Multimeter.lss: Multimeter.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Multimeter.elf  >"Multimeter.lss"
	@echo 'Finished building: $@'
	@echo ' '

Multimeter.hex: Multimeter.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Multimeter.elf  "Multimeter.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Multimeter.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 Multimeter.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Multimeter.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents main-build

-include ../makefile.targets
