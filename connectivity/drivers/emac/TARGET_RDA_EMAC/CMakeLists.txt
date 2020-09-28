# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-emac-rda5981x OBJECT)

add_subdirectory(lwip-wifi)

target_include_directories(mbed-os-emac-rda5981x
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-rda5981x
    PRIVATE
        RdaWiFiInterface.cpp
        rda5981x_emac.cpp
)
