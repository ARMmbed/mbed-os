# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-emac-rda5981x INTERFACE)

add_subdirectory(lwip-wifi)

target_include_directories(mbed-os-emac-rda5981x
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-emac-rda5981x
    INTERFACE
        RdaWiFiInterface.cpp
        rda5981x_emac.cpp
)

target_link_libraries(mbed-os-emac-rda5981x INTERFACE mbed-os-netsocket)
