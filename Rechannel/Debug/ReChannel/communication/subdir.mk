################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ReChannel/communication/rc_abstract_interface_wrapper.cpp \
../ReChannel/communication/rc_abstract_switch.cpp \
../ReChannel/communication/rc_report_ids.cpp 

OBJS += \
./ReChannel/communication/rc_abstract_interface_wrapper.o \
./ReChannel/communication/rc_abstract_switch.o \
./ReChannel/communication/rc_report_ids.o 

CPP_DEPS += \
./ReChannel/communication/rc_abstract_interface_wrapper.d \
./ReChannel/communication/rc_abstract_switch.d \
./ReChannel/communication/rc_report_ids.d 


# Each subdirectory must supply rules for building sources it contributes
ReChannel/communication/%.o: ../ReChannel/communication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"C:\systemc-2.2.0\include" -include"C:\systemc-2.2.0\include\systemc.h" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


