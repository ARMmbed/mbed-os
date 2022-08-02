# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(COMPONENT_hm01b0/hm01b0 EXCLUDE_FROM_ALL)
add_subdirectory(COMPONENT_lis2dh12/lis2dh12 EXCLUDE_FROM_ALL)

add_subdirectory(TARGET_SFE_ARTEMIS EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_SFE_ARTEMIS_ATP EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_SFE_ARTEMIS_DK EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_SFE_ARTEMIS_MODULE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_SFE_ARTEMIS_NANO EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_SFE_ARTEMIS_THING_PLUS EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_SFE_EDGE EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_SFE_EDGE2 EXCLUDE_FROM_ALL)

add_library(mbed-apollo3 INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE TOOLCHAIN_ARM_STD/AMA3B1KK.sct)
    set(STARTUP_FILE TOOLCHAIN_ARM_STD/startup_keil.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/AMA3B1KK.ld)
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_gcc.c)
endif()

mbed_set_linker_script(mbed-apollo3 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-apollo3
    INTERFACE
        device
        sdk/CMSIS/AmbiqMicro/Include
        sdk/mcu
        sdk/mcu/apollo3
        sdk/mcu/apollo3/hal
        sdk/mcu/apollo3/regs
)

target_sources(mbed-apollo3
    INTERFACE
        device/PeripheralPinConfigs.c
        device/PeripheralPins.c
        device/flash_api.c
        device/gpio_api.c
        device/gpio_irq_api.c
        device/i2c_api.c
        device/iom_api.c
        device/isr.c
        device/lp_ticker.c
        device/pinmap.c
        device/serial_api.c
        device/sleep.c
        device/spi_api.c
        device/us_ticker.c
        device/itm_api.c

        sdk/CMSIS/AmbiqMicro/Source/system_apollo3.c

        sdk/mcu/apollo3/hal/am_hal_adc.c
        sdk/mcu/apollo3/hal/am_hal_ble.c
        sdk/mcu/apollo3/hal/am_hal_ble_patch.c
        sdk/mcu/apollo3/hal/am_hal_ble_patch_b0.c
        sdk/mcu/apollo3/hal/am_hal_burst.c
        sdk/mcu/apollo3/hal/am_hal_cachectrl.c
        sdk/mcu/apollo3/hal/am_hal_clkgen.c
        sdk/mcu/apollo3/hal/am_hal_cmdq.c
        sdk/mcu/apollo3/hal/am_hal_ctimer.c
        sdk/mcu/apollo3/hal/am_hal_debug.c
        sdk/mcu/apollo3/hal/am_hal_flash.c
        sdk/mcu/apollo3/hal/am_hal_global.c
        sdk/mcu/apollo3/hal/am_hal_gpio.c
        sdk/mcu/apollo3/hal/am_hal_interrupt.c
        sdk/mcu/apollo3/hal/am_hal_iom.c
        sdk/mcu/apollo3/hal/am_hal_ios.c
        sdk/mcu/apollo3/hal/am_hal_itm.c
        sdk/mcu/apollo3/hal/am_hal_mcuctrl.c
        sdk/mcu/apollo3/hal/am_hal_mspi.c
        sdk/mcu/apollo3/hal/am_hal_pdm.c
        sdk/mcu/apollo3/hal/am_hal_pwrctrl.c
        sdk/mcu/apollo3/hal/am_hal_queue.c
        sdk/mcu/apollo3/hal/am_hal_reset.c
        sdk/mcu/apollo3/hal/am_hal_rtc.c
        sdk/mcu/apollo3/hal/am_hal_scard.c
        sdk/mcu/apollo3/hal/am_hal_secure_ota.c
        sdk/mcu/apollo3/hal/am_hal_security.c
        sdk/mcu/apollo3/hal/am_hal_stimer.c
        sdk/mcu/apollo3/hal/am_hal_sysctrl.c
        sdk/mcu/apollo3/hal/am_hal_systick.c
        sdk/mcu/apollo3/hal/am_hal_tpiu.c
        sdk/mcu/apollo3/hal/am_hal_uart.c
        sdk/mcu/apollo3/hal/am_hal_wdt.c

        ${STARTUP_FILE}
)

target_link_libraries(mbed-apollo3
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/sdk/CMSIS/ARM/Lib/ARM/libarm_cortexM4lf_math.a
        mbed-ambiq-micro
)
