################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/UART/UART.c 

OBJS += \
./Drivers/UART/UART.o 

C_DEPS += \
./Drivers/UART/UART.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/UART/%.o Drivers/UART/%.su Drivers/UART/%.cyclo: ../Drivers/UART/%.c Drivers/UART/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L082xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/jason.peng/stm32IDE_code/smart-transceiver-rfio-module-fw/New_TRy/restart/rfio_transceiver/Middlewares" -I"C:/Users/jason.peng/stm32IDE_code/smart-transceiver-rfio-module-fw/New_TRy/restart/rfio_transceiver/HumPRO_900" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-UART

clean-Drivers-2f-UART:
	-$(RM) ./Drivers/UART/UART.cyclo ./Drivers/UART/UART.d ./Drivers/UART/UART.o ./Drivers/UART/UART.su

.PHONY: clean-Drivers-2f-UART

