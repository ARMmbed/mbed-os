# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        src/cmsis_nvic_virtual.c
        src/tfm_mbed_boot.c
        src/tfm_ns_interface.c
        src/tfm_psa_ns_api.c
)
