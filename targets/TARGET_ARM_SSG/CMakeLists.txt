# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("CM3DS_MPS2" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CM3DS_MPS2)
elseif("MPS2" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MPS2)
elseif("MUSCA_B1" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MUSCA_B1)
elseif("MUSCA_S1" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MUSCA_S1)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
)
