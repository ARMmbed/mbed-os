# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-arm-ssg INTERFACE)

add_subdirectory(TARGET_CM3DS_MPS2  EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MPS2 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MUSCA_B1 EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_MUSCA_S1 EXCLUDE_FROM_ALL)

target_include_directories(mbed-arm-ssg
    INTERFACE
        .
)

target_link_libraries(mbed-arm-ssg INTERFACE mbed-cmsis-cortex-m)
