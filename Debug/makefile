################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include MCAL/GIE/subdir.mk
-include MCAL/EXTI/subdir.mk
-include MCAL/DIO/subdir.mk
-include HAL/Switch/subdir.mk
-include HAL/SevSeg/subdir.mk
-include HAL/LD/subdir.mk
-include HAL/LCD/subdir.mk
-include HAL/Keypad/subdir.mk
-include App/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
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

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
ATmega32_Maddi_503.lss \

FLASH_IMAGE += \
ATmega32_Maddi_503.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: ATmega32_Maddi_503.elf secondary-outputs

# Tool invocations
ATmega32_Maddi_503.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,ATmega32_Maddi_503.map -mmcu=atmega32 -o "ATmega32_Maddi_503.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ATmega32_Maddi_503.lss: ATmega32_Maddi_503.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S ATmega32_Maddi_503.elf  >"ATmega32_Maddi_503.lss"
	@echo 'Finished building: $@'
	@echo ' '

ATmega32_Maddi_503.hex: ATmega32_Maddi_503.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex ATmega32_Maddi_503.elf  "ATmega32_Maddi_503.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: ATmega32_Maddi_503.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 ATmega32_Maddi_503.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) ATmega32_Maddi_503.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
