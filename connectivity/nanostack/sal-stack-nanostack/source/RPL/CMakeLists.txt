# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        rpl_control.c
        rpl_data.c
        rpl_downward.c
        rpl_mrhof.c
        rpl_objective.c
        rpl_of0.c
        rpl_policy.c
        rpl_upward.c
)
