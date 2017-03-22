################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ReChannel/components/rc_join.cpp \
../ReChannel/components/rc_mutex.cpp \
../ReChannel/components/rc_report_ids.cpp \
../ReChannel/components/rc_semaphore.cpp \
../ReChannel/components/rc_signal.cpp \
../ReChannel/components/rc_signal_resolved.cpp \
../ReChannel/components/rc_var.cpp 

OBJS += \
./ReChannel/components/rc_join.o \
./ReChannel/components/rc_mutex.o \
./ReChannel/components/rc_report_ids.o \
./ReChannel/components/rc_semaphore.o \
./ReChannel/components/rc_signal.o \
./ReChannel/components/rc_signal_resolved.o \
./ReChannel/components/rc_var.o 

CPP_DEPS += \
./ReChannel/components/rc_join.d \
./ReChannel/components/rc_mutex.d \
./ReChannel/components/rc_report_ids.d \
./ReChannel/components/rc_semaphore.d \
./ReChannel/components/rc_signal.d \
./ReChannel/components/rc_signal_resolved.d \
./ReChannel/components/rc_var.d 


# Each subdirectory must supply rules for building sources it contributes
ReChannel/components/%.o: ../ReChannel/components/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


