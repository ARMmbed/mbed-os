# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-psa
    INTERFACE
        src/platform_multicore.c
        src/platform_ns_mailbox.c
        src/tfm_mbed_boot.c
        src/tfm_multi_core_api.c
        src/tfm_multi_core_psa_ns_api.c
        src/tfm_ns_mailbox.c
)
