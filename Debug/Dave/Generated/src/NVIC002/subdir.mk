################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/src/NVIC002/NVIC002.c \
../Dave/Generated/src/NVIC002/NVIC002_Conf.c 

OBJS += \
./Dave/Generated/src/NVIC002/NVIC002.o \
./Dave/Generated/src/NVIC002/NVIC002_Conf.o 

C_DEPS += \
./Dave/Generated/src/NVIC002/NVIC002.d \
./Dave/Generated/src/NVIC002/NVIC002_Conf.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/src/NVIC002/%.o: ../Dave/Generated/src/NVIC002/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"C:\DAVE-3.1.10\ARM-GCC/bin/arm-none-eabi-gcc" -DUC_ID=1101007 -DDAVE_CE -I"C:\DAVE-3.1.10\eclipse\/../CMSIS/Include" -I"C:\DAVE-3.1.10\eclipse\/../CMSIS/Infineon/Include" -I"C:\DAVE-3.1.10\ARM-GCC/arm-none-eabi/include" -I"C:\DAVE-3.1.10\eclipse\/../emWin/Start/GUI/inc" -I"C:\DAVE-3.1.10\eclipse\/../CMSIS/Infineon/XMC1100_series/Include" -I"C:\DAVE3_workspace\ws3.1.10\boschSpeedUp\Dave\Generated\inc\LIBS" -I"C:\DAVE3_workspace\ws3.1.10\boschSpeedUp\Dave\Generated\inc\DAVESupport" -O0 -ffunction-sections -Wall -std=gnu99 -mfloat-abi=soft -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


