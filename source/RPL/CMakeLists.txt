# Copyright (c) 2020-2021, Pelion and affiliates.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    INTERFACE
        .
)

target_sources(mbed-nanostack-sal_stack
    INTERFACE
        rpl_control.c
        rpl_data.c
        rpl_downward.c
        rpl_mrhof.c
        rpl_objective.c
        rpl_of0.c
        rpl_policy.c
        rpl_upward.c
)
