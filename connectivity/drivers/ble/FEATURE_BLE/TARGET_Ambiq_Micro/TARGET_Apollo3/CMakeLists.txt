# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-ble
    INTERFACE
        .
)

target_sources(mbed-ble
    INTERFACE
        AP3CordioHCIDriver.cpp
        AP3CordioHCITransportDriver.cpp
)

target_link_libraries(mbed-ble
    INTERFACE
        mbed-core
)
