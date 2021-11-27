# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(wifi-EMW3080B INTERFACE)

target_include_directories(mbed-wifi
    INTERFACE
        .
        mx_wifi
        mx_wifi/core
)

target_sources(mbed-wifi
    INTERFACE
        EMW3080BInterface.cpp
        EMW3080B_EMAC.cpp
        EMW3080B_UART.cpp
        EMW3080B_SPI.cpp
        mx_wifi_mbed_os.cpp
        mx_wifi/mx_wifi.c
        mx_wifi/core/checksumutils.c
        mx_wifi/core/mx_wifi_hci.c
        mx_wifi/core/mx_wifi_ipc.c
        mx_wifi/core/mx_wifi_slip.c
)
