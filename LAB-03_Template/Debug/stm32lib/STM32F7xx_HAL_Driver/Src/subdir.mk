################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.c \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c \
E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c 

OBJS += \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.o 

C_DEPS += \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.d 


# Each subdirectory must supply rules for building sources it contributes
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.o: E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/Local_REPO/STMWork/LAB-03_Template/inc" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/Local_REPO/STMWork/stm32lib/CMSIS/Include" -I"E:/Local_REPO/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

