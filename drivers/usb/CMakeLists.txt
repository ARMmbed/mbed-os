# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_ENABLE_OS_INTERNAL_TESTS)
    if(MBED_BUILD_GREENTEA_TESTS)
        add_subdirectory(tests/TESTS)
    endif()
endif()

if("DEVICE_USBDEVICE=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_library(mbed-usb STATIC EXCLUDE_FROM_ALL
        source/AsyncOp.cpp
        source/ByteBuffer.cpp
        source/EndpointResolver.cpp
        source/LinkedListBase.cpp
        source/OperationListBase.cpp
        source/PolledQueue.cpp
        source/TaskBase.cpp
        source/USBAudio.cpp
        source/USBCDC.cpp
        source/USBDevice.cpp
        source/USBHID.cpp
        source/USBKeyboard.cpp
        source/USBMIDI.cpp
        source/USBMouse.cpp
        source/USBMouseKeyboard.cpp
        source/USBSerial.cpp)

    target_include_directories(mbed-usb
        PUBLIC
            include
            include/usb
            include/usb/internal
    )

    target_link_libraries(mbed-usb PUBLIC mbed-core-flags)

    # USB Mass Storage Device library is separate because it pulls in a dependency on mbed-storage-blockdevice
    add_library(mbed-usb-msd STATIC EXCLUDE_FROM_ALL
        source/msd/USBMSD.cpp)

    target_include_directories(mbed-usb-msd
        PUBLIC
            include/usb/msd
    )

    target_link_libraries(mbed-usb-msd PUBLIC mbed-usb mbed-storage-blockdevice)

    # USB CDC ECM library is separate because it pulls in a dependency on mbed-rtos-flags
    add_library(mbed-usb-cdc-ecm STATIC EXCLUDE_FROM_ALL
        source/cdc_ecm/USBCDC_ECM.cpp)


    target_include_directories(mbed-usb-cdc-ecm
        PUBLIC
            include/usb/cdc_ecm
    )

    target_link_libraries(mbed-usb-cdc-ecm PUBLIC mbed-usb mbed-rtos-flags)
endif()