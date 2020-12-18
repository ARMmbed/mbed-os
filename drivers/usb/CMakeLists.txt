# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-usb INTERFACE)

target_include_directories(mbed-usb
    INTERFACE
        include
        include/usb
        include/usb/internal
)

target_sources(mbed-usb
    INTERFACE
        source/AsyncOp.cpp
        source/ByteBuffer.cpp
        source/EndpointResolver.cpp
        source/LinkedListBase.cpp
        source/OperationListBase.cpp
        source/PolledQueue.cpp
        source/TaskBase.cpp
        source/USBAudio.cpp
        source/USBCDC.cpp
        source/USBCDC_ECM.cpp
        source/USBDevice.cpp
        source/USBHID.cpp
        source/USBKeyboard.cpp
        source/USBMIDI.cpp
        source/USBMSD.cpp
        source/USBMouse.cpp
        source/USBMouseKeyboard.cpp
        source/USBSerial.cpp
)

target_link_libraries(mbed-usb INTERFACE mbed-storage)
