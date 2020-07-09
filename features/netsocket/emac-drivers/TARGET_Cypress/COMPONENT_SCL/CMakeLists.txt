# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/interface
)

target_sources(mbed-os
    PRIVATE
        interface/SclSTAInterface.cpp
        interface/default_wifi_interface.cpp
        interface/scl_emac.cpp
)
