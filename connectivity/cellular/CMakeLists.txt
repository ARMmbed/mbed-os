# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cellular OBJECT)

add_subdirectory(source/framework)

target_include_directories(mbed-cellular
    PUBLIC
        .
        ./include
        ./include/cellular
        ./include/cellular/framework
        ./include/cellular/framework/API
        ./include/cellular/framework/AT
        ./include/cellular/framework/common
        ./include/cellular/framework/device
)

target_compile_definitions(mbed-cellular
    PUBLIC
        MBED_CONF_CELLULAR_PRESENT=1
)

# All drivers here are part of mbed-cellular library by default
# Backward compatiblity with mbed-cli 1
target_link_libraries(mbed-cellular PRIVATE
    mbed-netsocket
    mbed-core
    mbed-rtos
    mbed-events

    mbed-cellular-alt1250-ppp
    mbed-cellular-gemalt-cinterion
    mbed-cellular-generic-at3gpp
    mbed-cellular-multitech-dragonfly_nano
    mbed-cellular-quectel-bc95
    mbed-cellular-quectel-bg96
    mbed-cellular-quectel-ec2x
    mbed-cellular-quectel-m26
    mbed-cellular-quectel-ug96
    mbed-cellular-riotmicro-at
    mbed-cellular-telit-he910
    mbed-cellular-telit-me310
    mbed-cellular-telit-me910
    mbed-cellular-ublox-ppp
    mbed-cellular-ublox-n2xx
    mbed-cellular-ublox-at

)
