################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/colaCaught.c \
../src/colaMensajesTeam.c \
../src/colaReady.c \
../src/config.c \
../src/deadlock.c \
../src/funciones_team.c \
../src/team.c 

OBJS += \
./src/colaCaught.o \
./src/colaMensajesTeam.o \
./src/colaReady.o \
./src/config.o \
./src/deadlock.o \
./src/funciones_team.o \
./src/team.o 

C_DEPS += \
./src/colaCaught.d \
./src/colaMensajesTeam.d \
./src/colaReady.d \
./src/config.d \
./src/deadlock.d \
./src/funciones_team.d \
./src/team.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/shared" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


