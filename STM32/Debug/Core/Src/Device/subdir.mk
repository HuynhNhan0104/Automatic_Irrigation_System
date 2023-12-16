################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Device/SHT30.c \
../Core/Src/Device/bh1750.c \
../Core/Src/Device/delay_timer.c \
../Core/Src/Device/lcd_i2c.c 

OBJS += \
./Core/Src/Device/SHT30.o \
./Core/Src/Device/bh1750.o \
./Core/Src/Device/delay_timer.o \
./Core/Src/Device/lcd_i2c.o 

C_DEPS += \
./Core/Src/Device/SHT30.d \
./Core/Src/Device/bh1750.d \
./Core/Src/Device/delay_timer.d \
./Core/Src/Device/lcd_i2c.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Device/%.o Core/Src/Device/%.su Core/Src/Device/%.cyclo: ../Core/Src/Device/%.c Core/Src/Device/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"D:/ESP/BTL/Read_sensor/Core/Lib/jsmn" -I"D:/ESP/BTL/Read_sensor/Core" -I"D:/ESP/BTL/Read_sensor/Core/Lib" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Device

clean-Core-2f-Src-2f-Device:
	-$(RM) ./Core/Src/Device/SHT30.cyclo ./Core/Src/Device/SHT30.d ./Core/Src/Device/SHT30.o ./Core/Src/Device/SHT30.su ./Core/Src/Device/bh1750.cyclo ./Core/Src/Device/bh1750.d ./Core/Src/Device/bh1750.o ./Core/Src/Device/bh1750.su ./Core/Src/Device/delay_timer.cyclo ./Core/Src/Device/delay_timer.d ./Core/Src/Device/delay_timer.o ./Core/Src/Device/delay_timer.su ./Core/Src/Device/lcd_i2c.cyclo ./Core/Src/Device/lcd_i2c.d ./Core/Src/Device/lcd_i2c.o ./Core/Src/Device/lcd_i2c.su

.PHONY: clean-Core-2f-Src-2f-Device

