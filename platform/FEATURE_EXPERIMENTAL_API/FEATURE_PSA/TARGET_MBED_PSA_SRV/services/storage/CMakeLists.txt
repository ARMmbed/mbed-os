# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-psa
    INTERFACE
        ./common
        ./its
)

target_sources(mbed-psa
    INTERFACE
        common/psa_storage_common_impl.cpp

        its/pits_impl.cpp
        its/psa_prot_internal_storage.cpp

        ps/protected_storage.cpp
)
