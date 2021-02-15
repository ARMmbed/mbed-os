# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-mps2 INTERFACE)

add_subdirectory(TARGET_MPS2_M0 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MPS2_M0P EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MPS2_M3 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MPS2_M4 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MPS2_M7 EXCLUDE_FROM_ALL)

target_include_directories(mbed-mps2
    INTERFACE
        .
        SDK
)

target_sources(mbed-mps2
    INTERFACE
        analogin_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        pinmap.c
        port_api.c
        serial_api.c
        spi_api.c
        us_ticker.c

        SDK/ETH_MPS2.c
        SDK/fpga.c
        SDK/mps2_ethernet_api.c

)

target_link_libraries(mbed-mps2
    INTERFACE
        mbed-arm-ssg
)
