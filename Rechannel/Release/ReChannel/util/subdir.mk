################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ReChannel/util/rc_delta_sync_object.cpp \
../ReChannel/util/rc_driver_object.cpp \
../ReChannel/util/rc_export_handle.cpp \
../ReChannel/util/rc_mutex_object.cpp \
../ReChannel/util/rc_object_handle.cpp \
../ReChannel/util/rc_port_handle.cpp \
../ReChannel/util/rc_report.cpp \
../ReChannel/util/rc_throwable.cpp 

OBJS += \
./ReChannel/util/rc_delta_sync_object.o \
./ReChannel/util/rc_driver_object.o \
./ReChannel/util/rc_export_handle.o \
./ReChannel/util/rc_mutex_object.o \
./ReChannel/util/rc_object_handle.o \
./ReChannel/util/rc_port_handle.o \
./ReChannel/util/rc_report.o \
./ReChannel/util/rc_throwable.o 

CPP_DEPS += \
./ReChannel/util/rc_delta_sync_object.d \
./ReChannel/util/rc_driver_object.d \
./ReChannel/util/rc_export_handle.d \
./ReChannel/util/rc_mutex_object.d \
./ReChannel/util/rc_object_handle.d \
./ReChannel/util/rc_port_handle.d \
./ReChannel/util/rc_report.d \
./ReChannel/util/rc_throwable.d 


# Each subdirectory must supply rules for building sources it contributes
ReChannel/util/%.o: ../ReChannel/util/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


