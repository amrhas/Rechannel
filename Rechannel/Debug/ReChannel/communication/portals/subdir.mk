################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ReChannel/communication/portals/rc_abstract_portal.cpp 

OBJS += \
./ReChannel/communication/portals/rc_abstract_portal.o 

CPP_DEPS += \
./ReChannel/communication/portals/rc_abstract_portal.d 


# Each subdirectory must supply rules for building sources it contributes
ReChannel/communication/portals/%.o: ../ReChannel/communication/portals/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"C:\systemc-2.2.0\include" -include"C:\systemc-2.2.0\include\systemc.h" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


