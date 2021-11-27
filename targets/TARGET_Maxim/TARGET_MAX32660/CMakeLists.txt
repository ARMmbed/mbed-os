# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_MAX32660EVSYS EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE  device/TOOLCHAIN_ARM_STD/MAX32660.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_max32660.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE  device/TOOLCHAIN_GCC_ARM/max32660.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_max32660.S)
endif()

add_library(mbed-max32660 INTERFACE)
mbed_set_linker_script(mbed-max32660 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

set(MXM_PARTNUMBER          MAX32660)
set(MXM_SOURCE_DIR          ./Libraries/PeriphDrivers/Source)
set(MXM_PERIPH_DRIVER_DIR   ./Libraries/PeriphDrivers)
set(MXM_CMSIS_DIR           ./Libraries/CMSIS/Device/Maxim)

target_include_directories(mbed-max32660
    INTERFACE
        .
        device
        
        ${MXM_PERIPH_DRIVER_DIR}/Include/${MXM_PARTNUMBER}
        ${MXM_CMSIS_DIR}/${MXM_PARTNUMBER}/Include
        
        ${MXM_SOURCE_DIR}/DMA
        ${MXM_SOURCE_DIR}/LP
        ${MXM_SOURCE_DIR}/FLC
        ${MXM_SOURCE_DIR}/GPIO
        ${MXM_SOURCE_DIR}/I2C 
        ${MXM_SOURCE_DIR}/ICC
        ${MXM_SOURCE_DIR}/RTC
        ${MXM_SOURCE_DIR}/SPI
        ${MXM_SOURCE_DIR}/SPIMSS
        ${MXM_SOURCE_DIR}/TMR
        ${MXM_SOURCE_DIR}/UART
        ${MXM_SOURCE_DIR}/WDT
)

target_sources(mbed-max32660
    INTERFACE
        PeripheralPins.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        pinmap.c
        port_api.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c
        lp_ticker.c
        flash_api.c
        watchdog_api.c
        
        ${MXM_CMSIS_DIR}/${MXM_PARTNUMBER}/Source/system_max32660.c
         
        ${MXM_SOURCE_DIR}/SYS/mxc_assert.c
        ${MXM_SOURCE_DIR}/SYS/mxc_delay.c
        ${MXM_SOURCE_DIR}/SYS/mxc_lock.c
        ${MXM_SOURCE_DIR}/SYS/pins_me11.c
        ${MXM_SOURCE_DIR}/SYS/sys_me11.c

        ${MXM_SOURCE_DIR}/DMA/dma_me11.c
        ${MXM_SOURCE_DIR}/DMA/dma_reva.c

        ${MXM_SOURCE_DIR}/LP/lp_me11.c

        ${MXM_SOURCE_DIR}/FLC/flc_common.c
        ${MXM_SOURCE_DIR}/FLC/flc_me11.c
        ${MXM_SOURCE_DIR}/FLC/flc_reva.c

        ${MXM_SOURCE_DIR}/GPIO/gpio_common.c
        ${MXM_SOURCE_DIR}/GPIO/gpio_me11.c
        ${MXM_SOURCE_DIR}/GPIO/gpio_reva.c

        ${MXM_SOURCE_DIR}/I2C/i2c_me11.c
        ${MXM_SOURCE_DIR}/I2C/i2c_reva.c

        ${MXM_SOURCE_DIR}/SPIMSS/spimss_me11.c
        ${MXM_SOURCE_DIR}/SPIMSS/spimss_reva.c
        ${MXM_SOURCE_DIR}/SPIMSS/i2s_me11.c
        ${MXM_SOURCE_DIR}/SPIMSS/i2s_reva.c

        ${MXM_SOURCE_DIR}/ICC/icc_common.c
        ${MXM_SOURCE_DIR}/ICC/icc_me11.c
        ${MXM_SOURCE_DIR}/ICC/icc_reva.c

        ${MXM_SOURCE_DIR}/RTC/rtc_me11.c
        ${MXM_SOURCE_DIR}/RTC/rtc_reva.c

        ${MXM_SOURCE_DIR}/SPI/spi_me11.c
        ${MXM_SOURCE_DIR}/SPI/spi_reva.c

        ${MXM_SOURCE_DIR}/TMR/tmr_common.c
        ${MXM_SOURCE_DIR}/TMR/tmr_me11.c
        ${MXM_SOURCE_DIR}/TMR/tmr_reva.c

        ${MXM_SOURCE_DIR}/UART/uart_common.c
        ${MXM_SOURCE_DIR}/UART/uart_me11.c
        ${MXM_SOURCE_DIR}/UART/uart_reva.c

        ${MXM_SOURCE_DIR}/WDT/wdt_common.c
        ${MXM_SOURCE_DIR}/WDT/wdt_me11.c
        ${MXM_SOURCE_DIR}/WDT/wdt_reva.c

        ${STARTUP_FILE}
)

target_link_libraries(mbed-max32660
    INTERFACE 
        mbed-maxim
)


