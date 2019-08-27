################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FatFs/src/diskio.c \
../Middlewares/Third_Party/FatFs/src/ff.c \
../Middlewares/Third_Party/FatFs/src/ff_gen_drv.c 

OBJS += \
./Middlewares/Third_Party/FatFs/src/diskio.o \
./Middlewares/Third_Party/FatFs/src/ff.o \
./Middlewares/Third_Party/FatFs/src/ff_gen_drv.o 

C_DEPS += \
./Middlewares/Third_Party/FatFs/src/diskio.d \
./Middlewares/Third_Party/FatFs/src/ff.d \
./Middlewares/Third_Party/FatFs/src/ff_gen_drv.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FatFs/src/%.o: ../Middlewares/Third_Party/FatFs/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F446xx -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Inc" -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/CMSIS/Include" -I"D:/Elektronika programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Middlewares/Third_Party/FatFs/src"  -O2 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


