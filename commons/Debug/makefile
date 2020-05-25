################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include commons/collections/subdir.mk
-include commons/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libcommons.so

dependents:
	-cd /home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/commons/Release && $(MAKE) all

# Tool invocations
libcommons.so: $(OBJS) $(USER_OBJS) /home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/commons/Release/libcommons.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/commons/Release" -shared -o "libcommons.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(LIBRARIES)$(OBJS)$(C_DEPS) libcommons.so
	-@echo ' '

.PHONY: all clean dependents
/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/commons/Release/libcommons.so:

-include ../makefile.targets
