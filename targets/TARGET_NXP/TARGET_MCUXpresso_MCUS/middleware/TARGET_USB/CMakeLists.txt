# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nxp-usb INTERFACE)

target_include_directories(mbed-nxp-usb
    INTERFACE
        device
        include
        phy
)

target_sources(mbed-nxp-usb
    INTERFACE
        device/usb_device_dci.c
        device/usb_device_ehci.c
        device/usb_device_khci.c
        device/usb_device_lpcip3511.c

        phy/usb_phy.c
)
