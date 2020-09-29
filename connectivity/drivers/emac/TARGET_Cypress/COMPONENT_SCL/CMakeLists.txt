# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-emac-scl INTERFACE)

target_include_directories(mbed-os-emac-scl
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/interface
)

target_sources(mbed-os-emac-scl
    INTERFACE
        interface/SclSTAInterface.cpp
        interface/default_wifi_interface.cpp
        interface/scl_emac.cpp
)

target_link_libraries(mbed-os-emac-scl INTERFACE mbed-os-netsocket)
