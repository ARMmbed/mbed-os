# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_IMX EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_LPC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_LPC54114 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MCU_LPC546XX EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MIMXRT1050 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MIMXRT1170 EXCLUDE_FROM_ALL)
add_subdirectory(middleware/TARGET_USB EXCLUDE_FROM_ALL)

add_library(mbed-mcuxpresso-mcus INTERFACE)

target_include_directories(mbed-mcuxpresso-mcus
    INTERFACE
        .
        middleware/osa
)

target_sources(mbed-mcuxpresso-mcus
    INTERFACE
        USBPhy_MCUXpresso.cpp

        middleware/osa/fsl_os_abstraction_mbed.c
)

target_link_libraries(mbed-mcuxpresso-mcus INTERFACE mbed-nxp)
