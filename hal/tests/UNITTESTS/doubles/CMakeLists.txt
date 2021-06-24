# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-hal)

target_compile_definitions(mbed-stubs-hal
    PRIVATE
        DEVICE_PWMOUT
        DEVICE_WATCHDOG
        MBED_WDOG_ASSERT=1
)

target_sources(mbed-stubs-hal
    PRIVATE
        pwmout_api_stub.c
        us_ticker_stub.cpp
        watchdog_api_stub.c
)

target_link_options(mbed-stubs-hal
    PRIVATE
        --coverage
)

target_link_libraries(mbed-stubs-hal
    PRIVATE
        mbed-headers
        mbed-stubs-headers
)
