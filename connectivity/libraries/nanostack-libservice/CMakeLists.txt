# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-libservice
    INTERFACE
        .
        ./mbed-client-libservice
        ./mbed-client-libservice/platform
)

target_sources(mbed-nanostack-libservice
    INTERFACE
        source/IPv6_fcf_lib/ip_fsc.c
        source/libList/ns_list.c
        source/libip4string/ip4tos.c
        source/libip4string/stoip4.c
        source/libip6string/stoip6.c
        source/nsdynmemLIB/nsdynmemLIB.c
        source/nsdynmemtracker/nsdynmem_tracker_lib.c
        source/nvmHelper/ns_nvm_helper.c
)

# The definition, source files and include directories below
# are needed by mbed-trace which is part of the mbed-core CMake target
target_compile_definitions(mbed-core
    INTERFACE
        MBED_CONF_NANOSTACK_LIBSERVICE_PRESENT=1
)
target_include_directories(mbed-core
    INTERFACE
        .
        ./mbed-client-libservice
)
target_sources(mbed-core
    INTERFACE
        source/libBits/common_functions.c
        source/libip6string/ip6tos.c
)
