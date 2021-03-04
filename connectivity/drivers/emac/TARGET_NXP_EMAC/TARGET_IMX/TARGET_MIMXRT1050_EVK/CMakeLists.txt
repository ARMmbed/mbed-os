# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-mimxrt1050 INTERFACE)

target_sources(mbed-emac-mimxrt1050
    PRIVATE
        hardware_init.c
)

target_link_libraries(mbed-emac-mimxrt1050 INTERFACE mbed-emac-imx)
