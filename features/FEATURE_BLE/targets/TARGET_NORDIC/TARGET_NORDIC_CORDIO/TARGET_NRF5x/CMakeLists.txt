# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/stack/include
)

target_sources(mbed-os
    PRIVATE
        NRFCordioHCIDriver.cpp
        NRFCordioHCITransportDriver.cpp

        stack/sources/pal_bb.c
        stack/sources/pal_bb_ble.c
        stack/sources/pal_bb_ble_rf.c
        stack/sources/pal_cfg.c
        stack/sources/pal_crypto.c
        stack/sources/pal_led.c
        stack/sources/pal_nvm.c
        stack/sources/pal_rtc.c
        stack/sources/pal_sys.c
        stack/sources/pal_timer.c
        stack/sources/pal_twi.c
        stack/sources/pal_uart.c
)
