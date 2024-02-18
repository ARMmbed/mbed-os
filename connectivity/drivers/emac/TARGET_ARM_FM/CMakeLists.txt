# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Driver for LAN91C111 memory-mapped EMAC IC
if("FVP_MPS2" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_FVP_MPS2)
endif()
