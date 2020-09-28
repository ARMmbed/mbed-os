# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-wifi-esp8266 OBJECT)

target_sources(mbed-os-wifi-esp8266
    PRIVATE
        ESP8266Interface.cpp
        ESP8266/ESP8266.cpp
)

target_include_directories(mbed-os-wifi-esp8266
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ESP8266
)

target_link_libraries(mbed-os-wifi-esp8266 mbed-os)
