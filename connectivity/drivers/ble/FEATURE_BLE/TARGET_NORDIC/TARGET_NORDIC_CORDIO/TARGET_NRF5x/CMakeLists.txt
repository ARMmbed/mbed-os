# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-ble-nrf5x-cordio INTEFACE)

target_include_directories(mbed-ble-nrf5x-cordio
    INTEFACE
        .
        ./stack/include
)

target_sources(mbed-ble-nrf5x-cordio
    INTEFACE
        NRFCordioHCIDriver.cpp
        NRFCordioHCITransportDriver.cpp

        stack/sources/pal_bb.c
        stack/sources/pal_bb_ble.c
        stack/sources/pal_bb_ble_rf.c
        stack/sources/pal_cfg.c
        stack/sources/pal_crypto.c
        stack/sources/pal_timer.c
)

target_link_libraries(mbed-ble-nrf5x-cordio
    INTEFACE
        mbed-ble-cordio_ll
        mbed-mbedtls-cryptocell310
)
