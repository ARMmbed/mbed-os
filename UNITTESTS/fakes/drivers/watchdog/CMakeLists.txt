# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-fakes-drivers-watchdog)

target_compile_definitions(mbed-fakes-drivers-watchdog 
    PRIVATE
        DEVICE_WATCHDOG
        MBED_WDOG_ASSERT=1
)

target_sources(mbed-fakes-drivers-watchdog
    PRIVATE
        ${MBED_PATH}/drivers/source/Watchdog.cpp
)

target_link_libraries(mbed-fakes-drivers-watchdog
    PRIVATE
        mbed-headers
)
