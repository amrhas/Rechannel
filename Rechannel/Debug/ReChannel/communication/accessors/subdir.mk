################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ReChannel/communication/accessors/rc_abstract_accessor.cpp \
../ReChannel/communication/accessors/rc_signal_accessors.cpp 

OBJS += \
./ReChannel/communication/accessors/rc_abstract_accessor.o \
./ReChannel/communication/accessors/rc_signal_accessors.o 

CPP_DEPS += \
./ReChannel/communication/accessors/rc_abstract_accessor.d \
./ReChannel/communication/accessors/rc_signal_accessors.d 


# Each subdirectory must supply rules for building sources it contributes
ReChannel/communication/accessors/%.o: ../ReChannel/communication/accessors/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"C:\systemc-2.2.0\include" -include"C:\systemc-2.2.0\include\systemc.h" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


