# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(lorastack)
add_subdirectory(system)

target_sources(mbed-os
    PRIVATE
        LoRaWANInterface.cpp
        LoRaWANStack.cpp
)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
