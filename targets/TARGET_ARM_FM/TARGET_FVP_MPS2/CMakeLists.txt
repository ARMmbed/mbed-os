# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_FVP_MPS2_M0 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_FVP_MPS2_M0P EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_FVP_MPS2_M3 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_FVP_MPS2_M4 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_FVP_MPS2_M7 EXCLUDE_FROM_ALL)

add_library(mbed-fvp-mps2 INTERFACE)

target_include_directories(mbed-fvp-mps2
    INTERFACE
        .
        drivers
        SDK
)

target_sources(mbed-fvp-mps2
    INTERFACE
        analogin_api.c
        flash_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        lp_ticker.c
        pinmap.c
        port_api.c
        serial_api.c
        sleep_api.c
        spi_api.c
        trng_api.c
        us_ticker.c

        drivers/lan91c111.c

        SDK/fpga.c
)

target_link_libraries(mbed-fvp-mps2 INTERFACE mbed-arm-fm)
