################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/get_command.c \
../Core/Src/App/schedulerport.c 

OBJS += \
./Core/Src/App/get_command.o \
./Core/Src/App/schedulerport.o 

C_DEPS += \
./Core/Src/App/get_command.d \
./Core/Src/App/schedulerport.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/%.o Core/Src/App/%.su Core/Src/App/%.cyclo: ../Core/Src/App/%.c Core/Src/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"D:/ESP/BTL/Read_sensor/Core/Lib/jsmn" -I"D:/ESP/BTL/Read_sensor/Core" -I"D:/ESP/BTL/Read_sensor/Core/Lib" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-App

clean-Core-2f-Src-2f-App:
	-$(RM) ./Core/Src/App/get_command.cyclo ./Core/Src/App/get_command.d ./Core/Src/App/get_command.o ./Core/Src/App/get_command.su ./Core/Src/App/schedulerport.cyclo ./Core/Src/App/schedulerport.d ./Core/Src/App/schedulerport.o ./Core/Src/App/schedulerport.su

.PHONY: clean-Core-2f-Src-2f-App

