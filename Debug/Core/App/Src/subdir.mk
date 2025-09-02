################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/App/Src/rfio_driver.c 

OBJS += \
./Core/App/Src/rfio_driver.o 

C_DEPS += \
./Core/App/Src/rfio_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/App/Src/%.o Core/App/Src/%.su Core/App/Src/%.cyclo: ../Core/App/Src/%.c Core/App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L082xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/HumPRO_900" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/Core/Middlewares" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/Core" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/Drivers" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/sdk-stm32l0xx/Inc" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/sdk-stm32l0xx" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/Core/App/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-App-2f-Src

clean-Core-2f-App-2f-Src:
	-$(RM) ./Core/App/Src/rfio_driver.cyclo ./Core/App/Src/rfio_driver.d ./Core/App/Src/rfio_driver.o ./Core/App/Src/rfio_driver.su

.PHONY: clean-Core-2f-App-2f-Src

