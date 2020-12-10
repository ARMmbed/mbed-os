# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("IMX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_IMX)
endif()

if("LPC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_LPC)
endif()

if("LPC54114" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_LPC54114)
endif()

if("LPC55S69" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_LPC55S69)
endif()

if("MCU_LPC546XX" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCU_LPC546XX)
endif()

if("MIMXRT1050" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MIMXRT1050)
endif()

if("USB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(middleware/TARGET_USB)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
        middleware/osa
)

target_sources(mbed-core
    INTERFACE
        USBPhy_MCUXpresso.cpp

        middleware/osa/fsl_os_abstraction_mbed.c
)
