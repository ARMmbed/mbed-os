# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nfc
    INTERFACE
        ./include
        ./include/nfc
        ./include/nfc/controllers
        ./source
        ./source/transceiver
)

target_sources(mbed-nfc
    INTERFACE
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
