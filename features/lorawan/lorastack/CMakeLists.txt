# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        mac/LoRaMac.cpp
        mac/LoRaMacChannelPlan.cpp
        mac/LoRaMacCommand.cpp
        mac/LoRaMacCrypto.cpp

        phy/LoRaPHY.cpp
        phy/LoRaPHYAS923.cpp
        phy/LoRaPHYAU915.cpp
        phy/LoRaPHYCN470.cpp
        phy/LoRaPHYCN779.cpp
        phy/LoRaPHYEU433.cpp
        phy/LoRaPHYEU868.cpp
        phy/LoRaPHYIN865.cpp
        phy/LoRaPHYKR920.cpp
        phy/LoRaPHYUS915.cpp
)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/mac
        ${CMAKE_CURRENT_SOURCE_DIR}/phy
)
