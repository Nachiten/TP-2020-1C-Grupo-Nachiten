################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bitarray.c \
../src/conexiones.c \
../src/gamecard.c \
../src/iniciarFS.c \
../src/mensajes.c \
../src/semaforos.c 

OBJS += \
./src/bitarray.o \
./src/conexiones.o \
./src/gamecard.o \
./src/iniciarFS.o \
./src/mensajes.o \
./src/semaforos.o 

C_DEPS += \
./src/bitarray.d \
./src/conexiones.d \
./src/gamecard.d \
./src/iniciarFS.d \
./src/mensajes.d \
./src/semaforos.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/shared" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


