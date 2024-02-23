################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Gerador\ Conflito.cpp 

OBJS += \
./src/Gerador\ Conflito.o 

CPP_DEPS += \
./src/Gerador\ Conflito.d 


# Each subdirectory must supply rules for building sources it contributes
src/Gerador\ Conflito.o: ../src/Gerador\ Conflito.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Gerador Conflito.d" -MT"src/Gerador\ Conflito.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


