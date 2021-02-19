# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-drivers)

target_sources(mbed-stubs-drivers
    PRIVATE
        BufferedSerial_stub.cpp
        SerialBase_stub.cpp
)

target_compile_definitions(mbed-stubs-drivers
    PRIVATE
        DEVICE_SERIAL=1
        DEVICE_INTERRUPTIN=1
        MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
)

target_link_libraries(mbed-stubs-drivers
    PRIVATE
        mbed-headers
        mbed-stubs-headers
)
