# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal-stack
    INTERFACE
        .
)

target_sources(mbed-nanostack-sal-stack
    INTERFACE
        mle.c
        mle_tlv.c
)
