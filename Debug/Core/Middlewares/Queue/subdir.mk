################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Middlewares/Queue/Queue.c 

OBJS += \
./Core/Middlewares/Queue/Queue.o 

C_DEPS += \
./Core/Middlewares/Queue/Queue.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Middlewares/Queue/%.o Core/Middlewares/Queue/%.su Core/Middlewares/Queue/%.cyclo: ../Core/Middlewares/Queue/%.c Core/Middlewares/Queue/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L082xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/HumPRO_900" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/Core/Middlewares" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/Core" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/Drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Middlewares-2f-Queue

clean-Core-2f-Middlewares-2f-Queue:
	-$(RM) ./Core/Middlewares/Queue/Queue.cyclo ./Core/Middlewares/Queue/Queue.d ./Core/Middlewares/Queue/Queue.o ./Core/Middlewares/Queue/Queue.su

.PHONY: clean-Core-2f-Middlewares-2f-Queue

