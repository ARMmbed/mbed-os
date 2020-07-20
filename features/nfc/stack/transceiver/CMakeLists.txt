# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/pn512
)

target_sources(mbed-os
    PRIVATE
        transceiver.c

        pn512/pn512.c
        pn512/pn512_cmd.c
        pn512/pn512_hw.c
        pn512/pn512_irq.c
        pn512/pn512_poll.c
        pn512/pn512_registers.c
        pn512/pn512_rf.c
        pn512/pn512_timer.c
        pn512/pn512_transceive.c
)
