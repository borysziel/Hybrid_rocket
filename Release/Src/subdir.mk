################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/SS_Borys_adc.c \
../Src/SS_Grazyna_xbee.c \
../Src/SS_MS6507.c \
../Src/SS_S25FL.c \
../Src/SS_can.c \
../Src/SS_ignition.c \
../Src/SS_it.c \
../Src/SS_sd.c \
../Src/SS_servos.c \
../Src/SS_support.c \
../Src/adc.c \
../Src/bsp_driver_sd.c \
../Src/can.c \
../Src/crc.c \
../Src/dma.c \
../Src/fatfs.c \
../Src/gpio.c \
../Src/main.c \
../Src/sd_diskio.c \
../Src/sdio.c \
../Src/spi.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/system_stm32f4xx.c \
../Src/tim.c \
../Src/usart.c 

OBJS += \
./Src/SS_Borys_adc.o \
./Src/SS_Grazyna_xbee.o \
./Src/SS_MS6507.o \
./Src/SS_S25FL.o \
./Src/SS_can.o \
./Src/SS_ignition.o \
./Src/SS_it.o \
./Src/SS_sd.o \
./Src/SS_servos.o \
./Src/SS_support.o \
./Src/adc.o \
./Src/bsp_driver_sd.o \
./Src/can.o \
./Src/crc.o \
./Src/dma.o \
./Src/fatfs.o \
./Src/gpio.o \
./Src/main.o \
./Src/sd_diskio.o \
./Src/sdio.o \
./Src/spi.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/system_stm32f4xx.o \
./Src/tim.o \
./Src/usart.o 

C_DEPS += \
./Src/SS_Borys_adc.d \
./Src/SS_Grazyna_xbee.d \
./Src/SS_MS6507.d \
./Src/SS_S25FL.d \
./Src/SS_can.d \
./Src/SS_ignition.d \
./Src/SS_it.d \
./Src/SS_sd.d \
./Src/SS_servos.d \
./Src/SS_support.d \
./Src/adc.d \
./Src/bsp_driver_sd.d \
./Src/can.d \
./Src/crc.d \
./Src/dma.d \
./Src/fatfs.d \
./Src/gpio.d \
./Src/main.d \
./Src/sd_diskio.d \
./Src/sdio.d \
./Src/spi.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/system_stm32f4xx.d \
./Src/tim.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F446xx -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Inc" -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/CMSIS/Include" -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Middlewares/Third_Party/FatFs/src"  -O2 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


