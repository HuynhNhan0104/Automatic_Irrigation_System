################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DeviceManager/device_output_manager.c \
../Core/Src/DeviceManager/sensor_manager.c 

OBJS += \
./Core/Src/DeviceManager/device_output_manager.o \
./Core/Src/DeviceManager/sensor_manager.o 

C_DEPS += \
./Core/Src/DeviceManager/device_output_manager.d \
./Core/Src/DeviceManager/sensor_manager.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/DeviceManager/%.o Core/Src/DeviceManager/%.su Core/Src/DeviceManager/%.cyclo: ../Core/Src/DeviceManager/%.c Core/Src/DeviceManager/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"D:/ESP/BTL/Read_sensor/Core/Lib/jsmn" -I"D:/ESP/BTL/Read_sensor/Core" -I"D:/ESP/BTL/Read_sensor/Core/Lib" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-DeviceManager

clean-Core-2f-Src-2f-DeviceManager:
	-$(RM) ./Core/Src/DeviceManager/device_output_manager.cyclo ./Core/Src/DeviceManager/device_output_manager.d ./Core/Src/DeviceManager/device_output_manager.o ./Core/Src/DeviceManager/device_output_manager.su ./Core/Src/DeviceManager/sensor_manager.cyclo ./Core/Src/DeviceManager/sensor_manager.d ./Core/Src/DeviceManager/sensor_manager.o ./Core/Src/DeviceManager/sensor_manager.su

.PHONY: clean-Core-2f-Src-2f-DeviceManager

