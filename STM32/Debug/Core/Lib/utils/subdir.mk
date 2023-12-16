################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/utils/utils_buffer.c \
../Core/Lib/utils/utils_string.c 

OBJS += \
./Core/Lib/utils/utils_buffer.o \
./Core/Lib/utils/utils_string.o 

C_DEPS += \
./Core/Lib/utils/utils_buffer.d \
./Core/Lib/utils/utils_string.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/utils/%.o Core/Lib/utils/%.su Core/Lib/utils/%.cyclo: ../Core/Lib/utils/%.c Core/Lib/utils/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"D:/ESP/BTL/Read_sensor/Core/Lib/jsmn" -I"D:/ESP/BTL/Read_sensor/Core" -I"D:/ESP/BTL/Read_sensor/Core/Lib" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Lib-2f-utils

clean-Core-2f-Lib-2f-utils:
	-$(RM) ./Core/Lib/utils/utils_buffer.cyclo ./Core/Lib/utils/utils_buffer.d ./Core/Lib/utils/utils_buffer.o ./Core/Lib/utils/utils_buffer.su ./Core/Lib/utils/utils_string.cyclo ./Core/Lib/utils/utils_string.d ./Core/Lib/utils/utils_string.o ./Core/Lib/utils/utils_string.su

.PHONY: clean-Core-2f-Lib-2f-utils

