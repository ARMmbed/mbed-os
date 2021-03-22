# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_K66F EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_K82F EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_KL43Z EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_KW41Z EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MCU_K22F EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MCU_K64F EXCLUDE_FROM_ALL)
add_subdirectory(middleware/wireless/TARGET_FRAMEWORK_5_3_3 EXCLUDE_FROM_ALL)

# We append freescale to the name as this should be unique but NXP/Freescale are in the tree covering
# same vendor
add_library(mbed-freescale-mcuxpresso-mcus INTERFACE)

target_include_directories(mbed-freescale-mcuxpresso-mcus
    INTERFACE
        api
)

target_sources(mbed-freescale-mcuxpresso-mcus
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

target_link_libraries(mbed-freescale-mcuxpresso-mcus INTERFACE mbed-freescale)
