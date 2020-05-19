################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/it.c \
../Src/main.c \
../Src/msp.c \
../Src/syscalls.c \
../Src/system_stm32l4xx.c 

OBJS += \
./Src/it.o \
./Src/main.o \
./Src/msp.o \
./Src/syscalls.o \
./Src/system_stm32l4xx.o 

C_DEPS += \
./Src/it.d \
./Src/main.d \
./Src/msp.d \
./Src/syscalls.d \
./Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32L476xx -I"C:/C_Workspace/HSE_SYSCLK_8Mhz_Run2/Inc" -I"C:/C_Workspace/HSE_SYSCLK_8Mhz_Run2/Drivers/STM32L4xx_HAL_Driver/Inc" -I"C:/C_Workspace/HSE_SYSCLK_8Mhz_Run2/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"C:/C_Workspace/HSE_SYSCLK_8Mhz_Run2/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"C:/C_Workspace/HSE_SYSCLK_8Mhz_Run2/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


