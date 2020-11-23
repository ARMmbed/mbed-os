# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("KL25Z" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_KL25Z)
elseif("KL46Z" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_KL46Z)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)

target_sources(mbed-core
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
