################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
OBJS += \
./Lib/utils/utils_buffer.o \
./Lib/utils/utils_string.o 

C_DEPS += \
./Lib/utils/utils_buffer.d \
./Lib/utils/utils_string.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/utils/utils_buffer.o: D:/FINAL\ PROJECT/Read_sensor/Core/Lib/utils/utils_buffer.c Lib/utils/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"D:/FINAL PROJECT/Read_sensor/Core/Lib/jsmn" -I"D:/FINAL PROJECT/Read_sensor/Core" -I"D:/FINAL PROJECT/Read_sensor/Core/Lib" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Lib/utils/utils_string.o: D:/FINAL\ PROJECT/Read_sensor/Core/Lib/utils/utils_string.c Lib/utils/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"D:/FINAL PROJECT/Read_sensor/Core/Lib/jsmn" -I"D:/FINAL PROJECT/Read_sensor/Core" -I"D:/FINAL PROJECT/Read_sensor/Core/Lib" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Lib-2f-utils

clean-Lib-2f-utils:
	-$(RM) ./Lib/utils/utils_buffer.cyclo ./Lib/utils/utils_buffer.d ./Lib/utils/utils_buffer.o ./Lib/utils/utils_buffer.su ./Lib/utils/utils_string.cyclo ./Lib/utils/utils_string.d ./Lib/utils/utils_string.o ./Lib/utils/utils_string.su

.PHONY: clean-Lib-2f-utils

