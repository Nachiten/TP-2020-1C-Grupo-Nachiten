################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../shared/cargador.c \
../shared/server.c \
../shared/socket.c \
../shared/terminarPrograma.c 

OBJS += \
./shared/cargador.o \
./shared/server.o \
./shared/socket.o \
./shared/terminarPrograma.o 

C_DEPS += \
./shared/cargador.d \
./shared/server.d \
./shared/socket.d \
./shared/terminarPrograma.d 


# Each subdirectory must supply rules for building sources it contributes
shared/%.o: ../shared/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/commons" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


