################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sdk-stm32l0xx/src/FBAllocator.c \
../sdk-stm32l0xx/src/IMsgQueue.c \
../sdk-stm32l0xx/src/Module.c \
../sdk-stm32l0xx/src/Module_Bus_Command_Parser.c \
../sdk-stm32l0xx/src/Module_Bus_Driver.c \
../sdk-stm32l0xx/src/Module_Bus_Packet.c \
../sdk-stm32l0xx/src/Module_EEPROM.c \
../sdk-stm32l0xx/src/Module_I2C_Driver.c \
../sdk-stm32l0xx/src/MsgQueue.c \
../sdk-stm32l0xx/src/RTC.c \
../sdk-stm32l0xx/src/Reg_Level_I2C_Slave.c \
../sdk-stm32l0xx/src/led_task.c \
../sdk-stm32l0xx/src/pmg_print.c \
../sdk-stm32l0xx/src/util.c 

OBJS += \
./sdk-stm32l0xx/src/FBAllocator.o \
./sdk-stm32l0xx/src/IMsgQueue.o \
./sdk-stm32l0xx/src/Module.o \
./sdk-stm32l0xx/src/Module_Bus_Command_Parser.o \
./sdk-stm32l0xx/src/Module_Bus_Driver.o \
./sdk-stm32l0xx/src/Module_Bus_Packet.o \
./sdk-stm32l0xx/src/Module_EEPROM.o \
./sdk-stm32l0xx/src/Module_I2C_Driver.o \
./sdk-stm32l0xx/src/MsgQueue.o \
./sdk-stm32l0xx/src/RTC.o \
./sdk-stm32l0xx/src/Reg_Level_I2C_Slave.o \
./sdk-stm32l0xx/src/led_task.o \
./sdk-stm32l0xx/src/pmg_print.o \
./sdk-stm32l0xx/src/util.o 

C_DEPS += \
./sdk-stm32l0xx/src/FBAllocator.d \
./sdk-stm32l0xx/src/IMsgQueue.d \
./sdk-stm32l0xx/src/Module.d \
./sdk-stm32l0xx/src/Module_Bus_Command_Parser.d \
./sdk-stm32l0xx/src/Module_Bus_Driver.d \
./sdk-stm32l0xx/src/Module_Bus_Packet.d \
./sdk-stm32l0xx/src/Module_EEPROM.d \
./sdk-stm32l0xx/src/Module_I2C_Driver.d \
./sdk-stm32l0xx/src/MsgQueue.d \
./sdk-stm32l0xx/src/RTC.d \
./sdk-stm32l0xx/src/Reg_Level_I2C_Slave.d \
./sdk-stm32l0xx/src/led_task.d \
./sdk-stm32l0xx/src/pmg_print.d \
./sdk-stm32l0xx/src/util.d 


# Each subdirectory must supply rules for building sources it contributes
sdk-stm32l0xx/src/%.o sdk-stm32l0xx/src/%.su sdk-stm32l0xx/src/%.cyclo: ../sdk-stm32l0xx/src/%.c sdk-stm32l0xx/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L082xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/HumPRO_900" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/Core/Middlewares" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/Core" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/Drivers" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/sdk-stm32l0xx/Inc" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/sdk-stm32l0xx" -I"C:/Users/14005/Documents/ACI_Work/RFIO/new_smart/rfio_transceiver/Core/App/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-sdk-2d-stm32l0xx-2f-src

clean-sdk-2d-stm32l0xx-2f-src:
	-$(RM) ./sdk-stm32l0xx/src/FBAllocator.cyclo ./sdk-stm32l0xx/src/FBAllocator.d ./sdk-stm32l0xx/src/FBAllocator.o ./sdk-stm32l0xx/src/FBAllocator.su ./sdk-stm32l0xx/src/IMsgQueue.cyclo ./sdk-stm32l0xx/src/IMsgQueue.d ./sdk-stm32l0xx/src/IMsgQueue.o ./sdk-stm32l0xx/src/IMsgQueue.su ./sdk-stm32l0xx/src/Module.cyclo ./sdk-stm32l0xx/src/Module.d ./sdk-stm32l0xx/src/Module.o ./sdk-stm32l0xx/src/Module.su ./sdk-stm32l0xx/src/Module_Bus_Command_Parser.cyclo ./sdk-stm32l0xx/src/Module_Bus_Command_Parser.d ./sdk-stm32l0xx/src/Module_Bus_Command_Parser.o ./sdk-stm32l0xx/src/Module_Bus_Command_Parser.su ./sdk-stm32l0xx/src/Module_Bus_Driver.cyclo ./sdk-stm32l0xx/src/Module_Bus_Driver.d ./sdk-stm32l0xx/src/Module_Bus_Driver.o ./sdk-stm32l0xx/src/Module_Bus_Driver.su ./sdk-stm32l0xx/src/Module_Bus_Packet.cyclo ./sdk-stm32l0xx/src/Module_Bus_Packet.d ./sdk-stm32l0xx/src/Module_Bus_Packet.o ./sdk-stm32l0xx/src/Module_Bus_Packet.su ./sdk-stm32l0xx/src/Module_EEPROM.cyclo ./sdk-stm32l0xx/src/Module_EEPROM.d ./sdk-stm32l0xx/src/Module_EEPROM.o ./sdk-stm32l0xx/src/Module_EEPROM.su ./sdk-stm32l0xx/src/Module_I2C_Driver.cyclo ./sdk-stm32l0xx/src/Module_I2C_Driver.d ./sdk-stm32l0xx/src/Module_I2C_Driver.o ./sdk-stm32l0xx/src/Module_I2C_Driver.su ./sdk-stm32l0xx/src/MsgQueue.cyclo ./sdk-stm32l0xx/src/MsgQueue.d ./sdk-stm32l0xx/src/MsgQueue.o ./sdk-stm32l0xx/src/MsgQueue.su ./sdk-stm32l0xx/src/RTC.cyclo ./sdk-stm32l0xx/src/RTC.d ./sdk-stm32l0xx/src/RTC.o ./sdk-stm32l0xx/src/RTC.su ./sdk-stm32l0xx/src/Reg_Level_I2C_Slave.cyclo ./sdk-stm32l0xx/src/Reg_Level_I2C_Slave.d ./sdk-stm32l0xx/src/Reg_Level_I2C_Slave.o ./sdk-stm32l0xx/src/Reg_Level_I2C_Slave.su ./sdk-stm32l0xx/src/led_task.cyclo ./sdk-stm32l0xx/src/led_task.d ./sdk-stm32l0xx/src/led_task.o ./sdk-stm32l0xx/src/led_task.su ./sdk-stm32l0xx/src/pmg_print.cyclo ./sdk-stm32l0xx/src/pmg_print.d ./sdk-stm32l0xx/src/pmg_print.o ./sdk-stm32l0xx/src/pmg_print.su ./sdk-stm32l0xx/src/util.cyclo ./sdk-stm32l0xx/src/util.d ./sdk-stm32l0xx/src/util.o ./sdk-stm32l0xx/src/util.su

.PHONY: clean-sdk-2d-stm32l0xx-2f-src

