################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GAUL_Drivers/Communications/ADC_driver.c \
../Core/Src/GAUL_Drivers/Communications/GPIO_driver.c \
../Core/Src/GAUL_Drivers/Communications/SPI_driver.c \
../Core/Src/GAUL_Drivers/Communications/UART_driver.c \
../Core/Src/GAUL_Drivers/Communications/delai.c \
../Core/Src/GAUL_Drivers/Communications/pwm.c 

OBJS += \
./Core/Src/GAUL_Drivers/Communications/ADC_driver.o \
./Core/Src/GAUL_Drivers/Communications/GPIO_driver.o \
./Core/Src/GAUL_Drivers/Communications/SPI_driver.o \
./Core/Src/GAUL_Drivers/Communications/UART_driver.o \
./Core/Src/GAUL_Drivers/Communications/delai.o \
./Core/Src/GAUL_Drivers/Communications/pwm.o 

C_DEPS += \
./Core/Src/GAUL_Drivers/Communications/ADC_driver.d \
./Core/Src/GAUL_Drivers/Communications/GPIO_driver.d \
./Core/Src/GAUL_Drivers/Communications/SPI_driver.d \
./Core/Src/GAUL_Drivers/Communications/UART_driver.d \
./Core/Src/GAUL_Drivers/Communications/delai.d \
./Core/Src/GAUL_Drivers/Communications/pwm.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/GAUL_Drivers/Communications/%.o Core/Src/GAUL_Drivers/Communications/%.su Core/Src/GAUL_Drivers/Communications/%.cyclo: ../Core/Src/GAUL_Drivers/Communications/%.c Core/Src/GAUL_Drivers/Communications/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Zx -c -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-GAUL_Drivers-2f-Communications

clean-Core-2f-Src-2f-GAUL_Drivers-2f-Communications:
	-$(RM) ./Core/Src/GAUL_Drivers/Communications/ADC_driver.cyclo ./Core/Src/GAUL_Drivers/Communications/ADC_driver.d ./Core/Src/GAUL_Drivers/Communications/ADC_driver.o ./Core/Src/GAUL_Drivers/Communications/ADC_driver.su ./Core/Src/GAUL_Drivers/Communications/GPIO_driver.cyclo ./Core/Src/GAUL_Drivers/Communications/GPIO_driver.d ./Core/Src/GAUL_Drivers/Communications/GPIO_driver.o ./Core/Src/GAUL_Drivers/Communications/GPIO_driver.su ./Core/Src/GAUL_Drivers/Communications/SPI_driver.cyclo ./Core/Src/GAUL_Drivers/Communications/SPI_driver.d ./Core/Src/GAUL_Drivers/Communications/SPI_driver.o ./Core/Src/GAUL_Drivers/Communications/SPI_driver.su ./Core/Src/GAUL_Drivers/Communications/UART_driver.cyclo ./Core/Src/GAUL_Drivers/Communications/UART_driver.d ./Core/Src/GAUL_Drivers/Communications/UART_driver.o ./Core/Src/GAUL_Drivers/Communications/UART_driver.su ./Core/Src/GAUL_Drivers/Communications/delai.cyclo ./Core/Src/GAUL_Drivers/Communications/delai.d ./Core/Src/GAUL_Drivers/Communications/delai.o ./Core/Src/GAUL_Drivers/Communications/delai.su ./Core/Src/GAUL_Drivers/Communications/pwm.cyclo ./Core/Src/GAUL_Drivers/Communications/pwm.d ./Core/Src/GAUL_Drivers/Communications/pwm.o ./Core/Src/GAUL_Drivers/Communications/pwm.su

.PHONY: clean-Core-2f-Src-2f-GAUL_Drivers-2f-Communications

