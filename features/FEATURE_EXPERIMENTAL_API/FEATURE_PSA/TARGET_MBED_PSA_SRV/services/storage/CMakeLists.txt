# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/its
)

target_sources(mbed-os
    PRIVATE
        common/psa_storage_common_impl.cpp

        its/pits_impl.cpp
        its/psa_prot_internal_storage.cpp

        ps/protected_storage.cpp
)
