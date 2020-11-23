# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("K66F" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_K66F)
elseif("K82F" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_K82F)
elseif("KL27Z" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_KL27Z)
elseif("KL43Z" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_KL43Z)
elseif("KW41Z" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_KW41Z)
elseif("MCU_K22F" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCU_K22F)
elseif("MCU_K64F" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCU_K64F)
endif()

if("FRAMEWORK_5_3_3" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(middleware/wireless/TARGET_FRAMEWORK_5_3_3)
endif()

target_include_directories(mbed-core
    INTERFACE
        api
)

target_sources(mbed-core
    INTERFACE
        fsl_common.c

        api/analogin_api.c
        api/analogout_api.c
        api/dma_api.c
        api/flash_api.c
        api/gpio_api.c
        api/gpio_irq_api.c
        api/i2c_api.c
        api/lp_ticker.c
        api/pinmap.c
        api/port_api.c
        api/qspi_api.c
        api/rtc_api.c
        api/sleep.c
)
