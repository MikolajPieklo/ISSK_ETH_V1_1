################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Modbus.c \
../Src/ModbusTCP_IP.c \
../Src/WIZ5500.c \
../Src/dhcp.c \
../Src/dma.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/socket.c \
../Src/spi.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/system_stm32f1xx.c \
../Src/usart.c \
../Src/w5500.c \
../Src/wizchip_conf.c 

OBJS += \
./Src/Modbus.o \
./Src/ModbusTCP_IP.o \
./Src/WIZ5500.o \
./Src/dhcp.o \
./Src/dma.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/socket.o \
./Src/spi.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/system_stm32f1xx.o \
./Src/usart.o \
./Src/w5500.o \
./Src/wizchip_conf.o 

C_DEPS += \
./Src/Modbus.d \
./Src/ModbusTCP_IP.d \
./Src/WIZ5500.d \
./Src/dhcp.d \
./Src/dma.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/socket.d \
./Src/spi.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/system_stm32f1xx.d \
./Src/usart.d \
./Src/w5500.d \
./Src/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xE -I"C:/Users/mikolajp/Desktop/softstarter/4. Ethernet/Program/ISSK_ETH_V1_1/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/mikolajp/Desktop/softstarter/4. Ethernet/Program/ISSK_ETH_V1_1/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/mikolajp/Desktop/softstarter/4. Ethernet/Program/ISSK_ETH_V1_1/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/mikolajp/Desktop/softstarter/4. Ethernet/Program/ISSK_ETH_V1_1/Drivers/CMSIS/Include" -I"C:/Users/mikolajp/Desktop/softstarter/4. Ethernet/Program/ISSK_ETH_V1_1/Inc" -I"C:/Users/mikolajp/Desktop/softstarter/4. Ethernet/Program/ISSK_ETH_V1_1/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


