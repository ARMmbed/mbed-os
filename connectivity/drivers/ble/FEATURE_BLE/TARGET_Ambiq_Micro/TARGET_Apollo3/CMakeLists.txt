# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-apollo3 OBJECT)

target_include_directories(mbed-ble-apollo3
    PUBLIC
        .
)

target_sources(mbed-ble-apollo3
    PRIVATE
        AP3CordioHCIDriver.cpp
        AP3CordioHCITransportDriver.cpp
)

target_link_libraries(mbed-ble-apollo3
    PRIVATE
        mbed-core
)
