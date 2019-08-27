################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SS_Ignition.c 

OBJS += \
./Drivers/SS_Ignition.o 

C_DEPS += \
./Drivers/SS_Ignition.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o: ../Drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F446xx -DARM_MATH_CM4 -I"D:/Elektronika_programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Inc" -I"D:/Elektronika_programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/Elektronika_programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"D:/Elektronika_programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/Elektronika_programowanie/SPACE SYSTEMS/HYBRID_Rocket_bottom/hybrid_rocket_bottom_wlasciwy/hybrid_rocket_bottomwlasciwy/Drivers/CMSIS/Include"  -O2 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


