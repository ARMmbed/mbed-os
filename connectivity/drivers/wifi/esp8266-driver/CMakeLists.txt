# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-wifi
    INTERFACE
        ESP8266Interface.cpp
        ESP8266/ESP8266.cpp
)

target_include_directories(mbed-wifi
    INTERFACE
        .
        ./ESP8266
)
