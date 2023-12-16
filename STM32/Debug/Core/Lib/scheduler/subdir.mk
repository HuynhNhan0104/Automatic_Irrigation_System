################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/scheduler/scheduler.c 

OBJS += \
./Core/Lib/scheduler/scheduler.o 

C_DEPS += \
./Core/Lib/scheduler/scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/scheduler/%.o Core/Lib/scheduler/%.su Core/Lib/scheduler/%.cyclo: ../Core/Lib/scheduler/%.c Core/Lib/scheduler/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"D:/ESP/BTL/Read_sensor/Core/Lib/jsmn" -I"D:/ESP/BTL/Read_sensor/Core" -I"D:/ESP/BTL/Read_sensor/Core/Lib" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Lib-2f-scheduler

clean-Core-2f-Lib-2f-scheduler:
	-$(RM) ./Core/Lib/scheduler/scheduler.cyclo ./Core/Lib/scheduler/scheduler.d ./Core/Lib/scheduler/scheduler.o ./Core/Lib/scheduler/scheduler.su

.PHONY: clean-Core-2f-Lib-2f-scheduler

