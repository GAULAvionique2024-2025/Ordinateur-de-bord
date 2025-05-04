################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GAUL_Drivers/Components/L76LM33.c \
../Core/Src/GAUL_Drivers/Components/MS5803.c \
../Core/Src/GAUL_Drivers/Components/RFD900.c 

OBJS += \
./Core/Src/GAUL_Drivers/Components/L76LM33.o \
./Core/Src/GAUL_Drivers/Components/MS5803.o \
./Core/Src/GAUL_Drivers/Components/RFD900.o 

C_DEPS += \
./Core/Src/GAUL_Drivers/Components/L76LM33.d \
./Core/Src/GAUL_Drivers/Components/MS5803.d \
./Core/Src/GAUL_Drivers/Components/RFD900.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/GAUL_Drivers/Components/%.o Core/Src/GAUL_Drivers/Components/%.su Core/Src/GAUL_Drivers/Components/%.cyclo: ../Core/Src/GAUL_Drivers/Components/%.c Core/Src/GAUL_Drivers/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Zx -c -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-GAUL_Drivers-2f-Components

clean-Core-2f-Src-2f-GAUL_Drivers-2f-Components:
	-$(RM) ./Core/Src/GAUL_Drivers/Components/L76LM33.cyclo ./Core/Src/GAUL_Drivers/Components/L76LM33.d ./Core/Src/GAUL_Drivers/Components/L76LM33.o ./Core/Src/GAUL_Drivers/Components/L76LM33.su ./Core/Src/GAUL_Drivers/Components/MS5803.cyclo ./Core/Src/GAUL_Drivers/Components/MS5803.d ./Core/Src/GAUL_Drivers/Components/MS5803.o ./Core/Src/GAUL_Drivers/Components/MS5803.su ./Core/Src/GAUL_Drivers/Components/RFD900.cyclo ./Core/Src/GAUL_Drivers/Components/RFD900.d ./Core/Src/GAUL_Drivers/Components/RFD900.o ./Core/Src/GAUL_Drivers/Components/RFD900.su

.PHONY: clean-Core-2f-Src-2f-GAUL_Drivers-2f-Components

