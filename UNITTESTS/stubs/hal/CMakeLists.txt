# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-hal)

target_sources(mbed-stubs-hal
    PUBLIC
        pwmout_api_stub.c
        watchdog_api_stub.c
)

target_link_libraries(mbed-stubs-hal
    PRIVATE
        mbed-headers
)
