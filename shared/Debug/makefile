################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include shared/subdir.mk
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
all: libshared.so

<<<<<<< HEAD
# Tool invocations
libshared.so: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -shared -o "libshared.so" $(OBJS) $(USER_OBJS) $(LIBS)
=======
dependents:
	-cd /home/utnso/workspace/commons/Debug && $(MAKE) all

# Tool invocations
libshared.so: $(OBJS) $(USER_OBJS) /home/utnso/workspace/commons/Debug/libcommons.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/workspace/commons/Debug" -shared -o "libshared.so" $(OBJS) $(USER_OBJS) $(LIBS)
>>>>>>> Gameboy
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(LIBRARIES)$(OBJS)$(C_DEPS) libshared.so
	-@echo ' '

.PHONY: all clean dependents
<<<<<<< HEAD
=======
/home/utnso/workspace/commons/Debug/libcommons.so:
>>>>>>> Gameboy

-include ../makefile.targets
