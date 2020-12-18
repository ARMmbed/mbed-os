# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    INTERFACE
        .
)

target_sources(mbed-nanostack-sal_stack
    INTERFACE
        mle.c
        mle_tlv.c
)
