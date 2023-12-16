################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Hal/clock.c \
../Core/Src/Hal/flash.c \
../Core/Src/Hal/gpio.c \
../Core/Src/Hal/i2c.c \
../Core/Src/Hal/timer.c \
../Core/Src/Hal/uart.c \
../Core/Src/Hal/watchdog.c 

OBJS += \
./Core/Src/Hal/clock.o \
./Core/Src/Hal/flash.o \
./Core/Src/Hal/gpio.o \
./Core/Src/Hal/i2c.o \
./Core/Src/Hal/timer.o \
./Core/Src/Hal/uart.o \
./Core/Src/Hal/watchdog.o 

C_DEPS += \
./Core/Src/Hal/clock.d \
./Core/Src/Hal/flash.d \
./Core/Src/Hal/gpio.d \
./Core/Src/Hal/i2c.d \
./Core/Src/Hal/timer.d \
./Core/Src/Hal/uart.d \
./Core/Src/Hal/watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Hal/%.o Core/Src/Hal/%.su Core/Src/Hal/%.cyclo: ../Core/Src/Hal/%.c Core/Src/Hal/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"D:/ESP/BTL/Read_sensor/Core/Lib/jsmn" -I"D:/ESP/BTL/Read_sensor/Core" -I"D:/ESP/BTL/Read_sensor/Core/Lib" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Hal

clean-Core-2f-Src-2f-Hal:
	-$(RM) ./Core/Src/Hal/clock.cyclo ./Core/Src/Hal/clock.d ./Core/Src/Hal/clock.o ./Core/Src/Hal/clock.su ./Core/Src/Hal/flash.cyclo ./Core/Src/Hal/flash.d ./Core/Src/Hal/flash.o ./Core/Src/Hal/flash.su ./Core/Src/Hal/gpio.cyclo ./Core/Src/Hal/gpio.d ./Core/Src/Hal/gpio.o ./Core/Src/Hal/gpio.su ./Core/Src/Hal/i2c.cyclo ./Core/Src/Hal/i2c.d ./Core/Src/Hal/i2c.o ./Core/Src/Hal/i2c.su ./Core/Src/Hal/timer.cyclo ./Core/Src/Hal/timer.d ./Core/Src/Hal/timer.o ./Core/Src/Hal/timer.su ./Core/Src/Hal/uart.cyclo ./Core/Src/Hal/uart.d ./Core/Src/Hal/uart.o ./Core/Src/Hal/uart.su ./Core/Src/Hal/watchdog.cyclo ./Core/Src/Hal/watchdog.d ./Core/Src/Hal/watchdog.o ./Core/Src/Hal/watchdog.su

.PHONY: clean-Core-2f-Src-2f-Hal

