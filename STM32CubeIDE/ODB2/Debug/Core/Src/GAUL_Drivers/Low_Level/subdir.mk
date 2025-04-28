################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GAUL_Drivers/Low_Level/ADC_driver.c \
../Core/Src/GAUL_Drivers/Low_Level/GPIO_driver.c \
../Core/Src/GAUL_Drivers/Low_Level/PWM_driver.c \
../Core/Src/GAUL_Drivers/Low_Level/SPI_driver.c \
../Core/Src/GAUL_Drivers/Low_Level/UART_driver.c 

OBJS += \
./Core/Src/GAUL_Drivers/Low_Level/ADC_driver.o \
./Core/Src/GAUL_Drivers/Low_Level/GPIO_driver.o \
./Core/Src/GAUL_Drivers/Low_Level/PWM_driver.o \
./Core/Src/GAUL_Drivers/Low_Level/SPI_driver.o \
./Core/Src/GAUL_Drivers/Low_Level/UART_driver.o 

C_DEPS += \
./Core/Src/GAUL_Drivers/Low_Level/ADC_driver.d \
./Core/Src/GAUL_Drivers/Low_Level/GPIO_driver.d \
./Core/Src/GAUL_Drivers/Low_Level/PWM_driver.d \
./Core/Src/GAUL_Drivers/Low_Level/SPI_driver.d \
./Core/Src/GAUL_Drivers/Low_Level/UART_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/GAUL_Drivers/Low_Level/%.o Core/Src/GAUL_Drivers/Low_Level/%.su Core/Src/GAUL_Drivers/Low_Level/%.cyclo: ../Core/Src/GAUL_Drivers/Low_Level/%.c Core/Src/GAUL_Drivers/Low_Level/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Zx -DUSE_FULL_LL_DRIVER -c -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-GAUL_Drivers-2f-Low_Level

clean-Core-2f-Src-2f-GAUL_Drivers-2f-Low_Level:
	-$(RM) ./Core/Src/GAUL_Drivers/Low_Level/ADC_driver.cyclo ./Core/Src/GAUL_Drivers/Low_Level/ADC_driver.d ./Core/Src/GAUL_Drivers/Low_Level/ADC_driver.o ./Core/Src/GAUL_Drivers/Low_Level/ADC_driver.su ./Core/Src/GAUL_Drivers/Low_Level/GPIO_driver.cyclo ./Core/Src/GAUL_Drivers/Low_Level/GPIO_driver.d ./Core/Src/GAUL_Drivers/Low_Level/GPIO_driver.o ./Core/Src/GAUL_Drivers/Low_Level/GPIO_driver.su ./Core/Src/GAUL_Drivers/Low_Level/PWM_driver.cyclo ./Core/Src/GAUL_Drivers/Low_Level/PWM_driver.d ./Core/Src/GAUL_Drivers/Low_Level/PWM_driver.o ./Core/Src/GAUL_Drivers/Low_Level/PWM_driver.su ./Core/Src/GAUL_Drivers/Low_Level/SPI_driver.cyclo ./Core/Src/GAUL_Drivers/Low_Level/SPI_driver.d ./Core/Src/GAUL_Drivers/Low_Level/SPI_driver.o ./Core/Src/GAUL_Drivers/Low_Level/SPI_driver.su ./Core/Src/GAUL_Drivers/Low_Level/UART_driver.cyclo ./Core/Src/GAUL_Drivers/Low_Level/UART_driver.d ./Core/Src/GAUL_Drivers/Low_Level/UART_driver.o ./Core/Src/GAUL_Drivers/Low_Level/UART_driver.su

.PHONY: clean-Core-2f-Src-2f-GAUL_Drivers-2f-Low_Level

