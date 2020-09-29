# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-nfc-pn512 OBJECT)

target_include_directories(mbed-os-nfc-pn512
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nfc
        ${CMAKE_CURRENT_SOURCE_DIR}/include/nfc/controllers
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        ${CMAKE_CURRENT_SOURCE_DIR}/source/transceiver
)

target_sources(mbed-os-nfc-pn512
    PRIVATE
        source/PN512Driver.cpp
        source/PN512SPITransportDriver.cpp
        source/PN512TransportDriver.cpp

        source/transceiver/pn512.c
        source/transceiver/pn512_cmd.c
        source/transceiver/pn512_hw.c
        source/transceiver/pn512_irq.c
        source/transceiver/pn512_poll.c
        source/transceiver/pn512_registers.c
        source/transceiver/pn512_rf.c
        source/transceiver/pn512_timer.c
        source/transceiver/pn512_transceive.c
)

mbed_link_component(mbed-os-nfc-pn512 mbed-os mbed-os-nfc)
