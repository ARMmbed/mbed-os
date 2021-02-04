# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_MOTE_L152RC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_L152RE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_XDOT_L151CC EXCLUDE_FROM_ALL)

add_library(STM32L1 INTERFACE)

target_sources(STM32L1
    INTERFACE
        analogin_device.c
        analogout_device.c
        flash_api.c
        gpio_irq_device.c
        pwmout_device.c
        serial_device.c
        spi_api.c

        device/stm32l1xx_hal.c
        device/stm32l1xx_hal_adc.c
        device/stm32l1xx_hal_adc_ex.c
        device/stm32l1xx_hal_comp.c
        device/stm32l1xx_hal_cortex.c
        device/stm32l1xx_hal_crc.c
        device/stm32l1xx_hal_cryp.c
        device/stm32l1xx_hal_cryp_ex.c
        device/stm32l1xx_hal_dac.c
        device/stm32l1xx_hal_dac_ex.c
        device/stm32l1xx_hal_dma.c
        device/stm32l1xx_hal_flash.c
        device/stm32l1xx_hal_flash_ex.c
        device/stm32l1xx_hal_flash_ramfunc.c
        device/stm32l1xx_hal_gpio.c
        device/stm32l1xx_hal_i2c.c
        device/stm32l1xx_hal_i2s.c
        device/stm32l1xx_hal_irda.c
        device/stm32l1xx_hal_iwdg.c
        device/stm32l1xx_hal_lcd.c
        device/stm32l1xx_hal_nor.c
        device/stm32l1xx_hal_opamp.c
        device/stm32l1xx_hal_opamp_ex.c
        device/stm32l1xx_hal_pcd.c
        device/stm32l1xx_hal_pcd_ex.c
        device/stm32l1xx_hal_pwr.c
        device/stm32l1xx_hal_pwr_ex.c
        device/stm32l1xx_hal_rcc.c
        device/stm32l1xx_hal_rcc_ex.c
        device/stm32l1xx_hal_rtc.c
        device/stm32l1xx_hal_rtc_ex.c
        device/stm32l1xx_hal_sd.c
        device/stm32l1xx_hal_smartcard.c
        device/stm32l1xx_hal_spi.c
        device/stm32l1xx_hal_spi_ex.c
        device/stm32l1xx_hal_sram.c
        device/stm32l1xx_hal_tim.c
        device/stm32l1xx_hal_tim_ex.c
        device/stm32l1xx_hal_uart.c
        device/stm32l1xx_hal_usart.c
        device/stm32l1xx_hal_wwdg.c
        device/stm32l1xx_ll_adc.c
        device/stm32l1xx_ll_comp.c
        device/stm32l1xx_ll_crc.c
        device/stm32l1xx_ll_dac.c
        device/stm32l1xx_ll_dma.c
        device/stm32l1xx_ll_exti.c
        device/stm32l1xx_ll_fsmc.c
        device/stm32l1xx_ll_gpio.c
        device/stm32l1xx_ll_i2c.c
        device/stm32l1xx_ll_opamp.c
        device/stm32l1xx_ll_pwr.c
        device/stm32l1xx_ll_rcc.c
        device/stm32l1xx_ll_rtc.c
        device/stm32l1xx_ll_sdmmc.c
        device/stm32l1xx_ll_spi.c
        device/stm32l1xx_ll_tim.c
        device/stm32l1xx_ll_usart.c
        device/stm32l1xx_ll_utils.c
        device/system_stm32l1xx.c
)

target_include_directories(STM32L1
    INTERFACE
        .
        device
)

target_link_libraries(STM32L1 INTERFACE STM)
