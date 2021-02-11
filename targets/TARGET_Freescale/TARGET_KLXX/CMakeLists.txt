# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_KL25Z EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_KL46Z EXCLUDE_FROM_ALL)

add_library(mbed-klxx INTERFACE)

target_include_directories(mbed-klxx
    INTERFACE
        .
)

target_sources(mbed-klxx
    INTERFACE
        analogin_api.c
        analogout_api.c
        gpio_api.c
        i2c_api.c
        pinmap.c
        port_api.c
        pwmout_api.c
        rtc_api.c
        sleep.c
        us_ticker.c
)

target_link_libraries(mbed-klxx INTERFACE mbed-freescale)
