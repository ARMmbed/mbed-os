# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("STM32F091xC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F091xC)
elseif("NUCLEO_F070RB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F070RB)
elseif("NUCLEO_F072RB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F072RB)
endif()

target_sources(mbed-core
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c

        device/stm32f0xx_hal.c
        device/stm32f0xx_hal_adc.c
        device/stm32f0xx_hal_adc_ex.c
        device/stm32f0xx_hal_can.c
        device/stm32f0xx_hal_cec.c
        device/stm32f0xx_hal_comp.c
        device/stm32f0xx_hal_cortex.c
        device/stm32f0xx_hal_crc.c
        device/stm32f0xx_hal_crc_ex.c
        device/stm32f0xx_hal_dac.c
        device/stm32f0xx_hal_dac_ex.c
        device/stm32f0xx_hal_dma.c
        device/stm32f0xx_hal_flash.c
        device/stm32f0xx_hal_flash_ex.c
        device/stm32f0xx_hal_gpio.c
        device/stm32f0xx_hal_i2c.c
        device/stm32f0xx_hal_i2c_ex.c
        device/stm32f0xx_hal_i2s.c
        device/stm32f0xx_hal_irda.c
        device/stm32f0xx_hal_iwdg.c
        device/stm32f0xx_hal_pcd.c
        device/stm32f0xx_hal_pcd_ex.c
        device/stm32f0xx_hal_pwr.c
        device/stm32f0xx_hal_pwr_ex.c
        device/stm32f0xx_hal_rcc.c
        device/stm32f0xx_hal_rcc_ex.c
        device/stm32f0xx_hal_rtc.c
        device/stm32f0xx_hal_rtc_ex.c
        device/stm32f0xx_hal_smartcard.c
        device/stm32f0xx_hal_smartcard_ex.c
        device/stm32f0xx_hal_smbus.c
        device/stm32f0xx_hal_spi.c
        device/stm32f0xx_hal_spi_ex.c
        device/stm32f0xx_hal_tim.c
        device/stm32f0xx_hal_tim_ex.c
        device/stm32f0xx_hal_tsc.c
        device/stm32f0xx_hal_uart.c
        device/stm32f0xx_hal_uart_ex.c
        device/stm32f0xx_hal_usart.c
        device/stm32f0xx_hal_wwdg.c
        device/stm32f0xx_ll_adc.c
        device/stm32f0xx_ll_comp.c
        device/stm32f0xx_ll_crc.c
        device/stm32f0xx_ll_crs.c
        device/stm32f0xx_ll_dac.c
        device/stm32f0xx_ll_dma.c
        device/stm32f0xx_ll_exti.c
        device/stm32f0xx_ll_gpio.c
        device/stm32f0xx_ll_i2c.c
        device/stm32f0xx_ll_pwr.c
        device/stm32f0xx_ll_rcc.c
        device/stm32f0xx_ll_rtc.c
        device/stm32f0xx_ll_spi.c
        device/stm32f0xx_ll_tim.c
        device/stm32f0xx_ll_usart.c
        device/stm32f0xx_ll_utils.c
        device/system_stm32f0xx.c
)

target_include_directories(mbed-core
    INTERFACE
        .
        device
)
