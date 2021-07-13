# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-stubs-connectivity)

target_compile_definitions(mbed-stubs-connectivity
    PRIVATE
        DEVICE_SERIAL=1
        DEVICE_INTERRUPTIN=1
        MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
        MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION=true
        MBED_CONF_LORA_AUTOMATIC_UPLINK_MESSAGE=true
        MBED_CONF_LORA_TX_MAX_SIZE=255
        MBED_CONF_CELLULAR_AT_HANDLER_BUFFER_SIZE=32
        MDMTXD=NC
        MDMRXD=NC
)

target_sources(mbed-stubs-connectivity
    PRIVATE
        ip4tos_stub.c
        stoip4_stub.c
)

target_link_libraries(mbed-stubs-connectivity
    PRIVATE
        mbed-headers
        mbed-stubs-headers
        mbed-stubs-rtos
        mbed-stubs-platform
        gtest
)
