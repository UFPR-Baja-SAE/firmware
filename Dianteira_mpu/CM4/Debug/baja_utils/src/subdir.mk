################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../baja_utils/src/msg.c 

OBJS += \
./baja_utils/src/msg.o 

C_DEPS += \
./baja_utils/src/msg.d 


# Each subdirectory must supply rules for building sources it contributes
baja_utils/src/%.o baja_utils/src/%.su baja_utils/src/%.cyclo: ../baja_utils/src/%.c baja_utils/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DNO_ATOMIC_64_SUPPORT -DMETAL_INTERNAL -DMETAL_MAX_DEVICE_REGIONS=2 -DVIRTIO_SLAVE_ONLY -DUSE_HAL_DRIVER -DSTM32MP157Fxx -DMETAL_FREERTOS -c -I../OPENAMP -I../Core/Inc -I../../Middlewares/Third_Party/OpenAMP/open-amp/lib/include -I../../Middlewares/Third_Party/OpenAMP/libmetal/lib/include -I../../Drivers/STM32MP1xx_HAL_Driver/Inc -I../../Drivers/STM32MP1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32MP1xx/Include -I../../Middlewares/Third_Party/OpenAMP/virtual_driver -I../../Drivers/CMSIS/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/pauli/baja/firmware/Dianteira_mpu/CM4/Middlewares/Third_Party/OpenAMP" -I"/home/pauli/baja/firmware/Dianteira_mpu/CM4/baja_utils/inc" -I"/home/pauli/baja/firmware/Dianteira_mpu/CM4/baja_utils/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-baja_utils-2f-src

clean-baja_utils-2f-src:
	-$(RM) ./baja_utils/src/msg.cyclo ./baja_utils/src/msg.d ./baja_utils/src/msg.o ./baja_utils/src/msg.su

.PHONY: clean-baja_utils-2f-src

