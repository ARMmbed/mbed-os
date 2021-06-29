# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-psa-tal INTERFACE)

target_include_directories(mbed-psa-tal
    INTERFACE
        pal
        val
)

target_sources(mbed-psa-tal
    INTERFACE
        pal/pal_attestation_eat.c
        pal/pal_attestation_intf.c
        pal/pal_client_api_empty_intf.c
        pal/pal_client_api_intf.c
        pal/pal_internal_trusted_storage_intf.c
        pal/pal_protected_storage_intf.c
        pal/pal_mbed_os_intf.cpp

        val/val_attestation.c
        val/val_interfaces.c
        val/val_internal_trusted_storage.c
        val/val_protected_storage.c
        val/val_greentea.cpp
)

target_link_libraries(mbed-psa-tal
    INTERFACE
        mbed-psa
)
