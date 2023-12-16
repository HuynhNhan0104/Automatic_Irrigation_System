################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/pid/pid.c 

OBJS += \
./Core/Lib/pid/pid.o 

C_DEPS += \
./Core/Lib/pid/pid.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/pid/%.o Core/Lib/pid/%.su Core/Lib/pid/%.cyclo: ../Core/Lib/pid/%.c Core/Lib/pid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"D:/FINAL PROJECT/Read_sensor/Core/Lib/jsmn" -I"D:/FINAL PROJECT/Read_sensor/Core" -I"D:/FINAL PROJECT/Read_sensor/Core/Lib" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Lib-2f-pid

clean-Core-2f-Lib-2f-pid:
	-$(RM) ./Core/Lib/pid/pid.cyclo ./Core/Lib/pid/pid.d ./Core/Lib/pid/pid.o ./Core/Lib/pid/pid.su

.PHONY: clean-Core-2f-Lib-2f-pid

