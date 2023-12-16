################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
OBJS += \
./Lib/scheduler/scheduler.o 

C_DEPS += \
./Lib/scheduler/scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/scheduler/scheduler.o: D:/FINAL\ PROJECT/Read_sensor/Core/Lib/scheduler/scheduler.c Lib/scheduler/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"D:/FINAL PROJECT/Read_sensor/Core/Lib/jsmn" -I"D:/FINAL PROJECT/Read_sensor/Core" -I"D:/FINAL PROJECT/Read_sensor/Core/Lib" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Lib-2f-scheduler

clean-Lib-2f-scheduler:
	-$(RM) ./Lib/scheduler/scheduler.cyclo ./Lib/scheduler/scheduler.d ./Lib/scheduler/scheduler.o ./Lib/scheduler/scheduler.su

.PHONY: clean-Lib-2f-scheduler

