# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("hm01b0" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            COMPONENT_hm01b0/hm01b0
    )

    target_sources(mbed-core
        INTERFACE
            COMPONENT_hm01b0/hm01b0/HM01B0.c
    )
endif()

if("lis2dh12" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            COMPONENT_lis2dh12/lis2dh12
    )

    target_sources(mbed-core
        INTERFACE
            COMPONENT_lis2dh12/lis2dh12/lis2dh12_platform_apollo3.c
            COMPONENT_lis2dh12/lis2dh12/lis2dh12_reg.c
    )
endif()

if("SFE_ARTEMIS" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS
            TARGET_SFE_ARTEMIS/bsp
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS/bsp/am_bsp.c
            TARGET_SFE_ARTEMIS/bsp/am_bsp_pins.c
    )
elseif("SFE_ARTEMIS_ATP" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS_ATP
            TARGET_SFE_ARTEMIS_ATP/bsp
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS_ATP/bsp/am_bsp.c
            TARGET_SFE_ARTEMIS_ATP/bsp/am_bsp_pins.c
    )
elseif("SFE_ARTEMIS_DK" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS_DK
            TARGET_SFE_ARTEMIS_DK/bsp
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS_DK/bsp/am_bsp.c
            TARGET_SFE_ARTEMIS_DK/bsp/am_bsp_pins.c
    )
elseif("SFE_ARTEMIS_MODULE" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS_MODULE
            TARGET_SFE_ARTEMIS_MODULE/bsp
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS_MODULE/bsp/am_bsp.c
            TARGET_SFE_ARTEMIS_MODULE/bsp/am_bsp_pins.c
    )
elseif("SFE_ARTEMIS_NANO" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS_NANO
            TARGET_SFE_ARTEMIS_NANO/bsp
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS_NANO/bsp/am_bsp.c
            TARGET_SFE_ARTEMIS_NANO/bsp/am_bsp_pins.c
    )
elseif("SFE_ARTEMIS_THING_PLUS" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS_THING_PLUS
            TARGET_SFE_ARTEMIS_THING_PLUS/bsp
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_SFE_ARTEMIS_THING_PLUS/bsp/am_bsp.c
            TARGET_SFE_ARTEMIS_THING_PLUS/bsp/am_bsp_pins.c
    )
elseif("SFE_EDGE" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_SFE_EDGE
            TARGET_SFE_EDGE/bsp
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_SFE_EDGE/bsp/am_bsp.c
            TARGET_SFE_EDGE/bsp/am_bsp_pins.c
    )
elseif("SFE_EDGE2" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_SFE_EDGE2
            TARGET_SFE_EDGE2/bsp
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_SFE_EDGE2/bsp/am_bsp.c
            TARGET_SFE_EDGE2/bsp/am_bsp_pins.c
    )
endif()

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE TOOLCHAIN_ARM_STD/AMA3B1KK.sct)
    set(STARTUP_FILE TOOLCHAIN_ARM_STD/startup_keil.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/AMA3B1KK.ld)
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_gcc.c)
endif()

set_property(GLOBAL PROPERTY MBED_TARGET_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-core
    INTERFACE
        device
        sdk/CMSIS/AmbiqMicro/Include
        sdk/mcu
        sdk/mcu/apollo3
        sdk/mcu/apollo3/hal
        sdk/mcu/apollo3/regs
)

target_sources(mbed-core
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

target_link_libraries(mbed-core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/sdk/CMSIS/ARM/Lib/ARM/libarm_cortexM4lf_math.a
)
