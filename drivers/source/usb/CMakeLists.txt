# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-usb INTERFACE)

target_include_directories(mbed-usb
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-usb
    INTERFACE
        AsyncOp.cpp
        ByteBuffer.cpp
        EndpointResolver.cpp
        LinkedListBase.cpp
        OperationListBase.cpp
        PolledQueue.cpp
        TaskBase.cpp
        USBAudio.cpp
        USBCDC.cpp
        USBCDC_ECM.cpp
        USBDevice.cpp
        USBHID.cpp
        USBKeyboard.cpp
        USBMIDI.cpp
        USBMSD.cpp
        USBMouse.cpp
        USBMouseKeyboard.cpp
        USBSerial.cpp
)

target_link_libraries(mbed-usb INTERFACE mbed-storage)
