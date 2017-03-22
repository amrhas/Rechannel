################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ReChannel/communication/exportals/rc_abstract_exportal.cpp \
../ReChannel/communication/exportals/rc_wrapper_pool.cpp 

OBJS += \
./ReChannel/communication/exportals/rc_abstract_exportal.o \
./ReChannel/communication/exportals/rc_wrapper_pool.o 

CPP_DEPS += \
./ReChannel/communication/exportals/rc_abstract_exportal.d \
./ReChannel/communication/exportals/rc_wrapper_pool.d 


# Each subdirectory must supply rules for building sources it contributes
ReChannel/communication/exportals/%.o: ../ReChannel/communication/exportals/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


