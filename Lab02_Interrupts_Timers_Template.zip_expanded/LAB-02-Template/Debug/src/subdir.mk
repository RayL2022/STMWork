################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Task3.c \
../src/init.c \
../src/uart.c 

OBJS += \
./src/Task3.o \
./src/init.o \
./src/uart.o 

C_DEPS += \
./src/Task3.d \
./src/init.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"E:/STMWork/Lab02_Interrupts_Timers_Template.zip_expanded/LAB-02-Template/inc" -I"E:/STMWork/stm32lib/CMSIS/Include" -I"E:/STMWork/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"E:/STMWork/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

