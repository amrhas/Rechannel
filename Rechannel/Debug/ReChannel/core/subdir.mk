################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ReChannel/core/rc_common_header.cpp \
../ReChannel/core/rc_control.cpp \
../ReChannel/core/rc_non_reconfigurable.cpp \
../ReChannel/core/rc_portmap.cpp \
../ReChannel/core/rc_process.cpp \
../ReChannel/core/rc_process_support.cpp \
../ReChannel/core/rc_reconfigurable.cpp \
../ReChannel/core/rc_reconfigurable_set.cpp \
../ReChannel/core/rc_report_ids.cpp \
../ReChannel/core/rc_reset_channel_if.cpp \
../ReChannel/core/rc_switch_connector.cpp \
../ReChannel/core/rc_transaction_counter.cpp 

OBJS += \
./ReChannel/core/rc_common_header.o \
./ReChannel/core/rc_control.o \
./ReChannel/core/rc_non_reconfigurable.o \
./ReChannel/core/rc_portmap.o \
./ReChannel/core/rc_process.o \
./ReChannel/core/rc_process_support.o \
./ReChannel/core/rc_reconfigurable.o \
./ReChannel/core/rc_reconfigurable_set.o \
./ReChannel/core/rc_report_ids.o \
./ReChannel/core/rc_reset_channel_if.o \
./ReChannel/core/rc_switch_connector.o \
./ReChannel/core/rc_transaction_counter.o 

CPP_DEPS += \
./ReChannel/core/rc_common_header.d \
./ReChannel/core/rc_control.d \
./ReChannel/core/rc_non_reconfigurable.d \
./ReChannel/core/rc_portmap.d \
./ReChannel/core/rc_process.d \
./ReChannel/core/rc_process_support.d \
./ReChannel/core/rc_reconfigurable.d \
./ReChannel/core/rc_reconfigurable_set.d \
./ReChannel/core/rc_report_ids.d \
./ReChannel/core/rc_reset_channel_if.d \
./ReChannel/core/rc_switch_connector.d \
./ReChannel/core/rc_transaction_counter.d 


# Each subdirectory must supply rules for building sources it contributes
ReChannel/core/%.o: ../ReChannel/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"C:\systemc-2.2.0\include" -include"C:\systemc-2.2.0\include\systemc.h" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


