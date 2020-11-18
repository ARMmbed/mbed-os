# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-core
    INTERFACE
        device
        include
        phy
)

target_sources(mbed-core
    INTERFACE
        device/usb_device_dci.c
        device/usb_device_ehci.c
        device/usb_device_khci.c
        device/usb_device_lpcip3511.c

        phy/usb_phy.c
)
