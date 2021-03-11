# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nanostack-libservice OBJECT)

target_include_directories(mbed-nanostack-libservice
    PUBLIC
        .
        ./mbed-client-libservice
        ./mbed-client-libservice/platform
)

target_sources(mbed-nanostack-libservice
    PRIVATE
        source/IPv6_fcf_lib/ip_fsc.c
        source/libList/ns_list.c
        source/libip4string/ip4tos.c
        source/libip4string/stoip4.c
        source/libip6string/stoip6.c
        source/nsdynmemLIB/nsdynmemLIB.c
        source/nsdynmemtracker/nsdynmem_tracker_lib.c
        source/nvmHelper/ns_nvm_helper.c
        source/libBits/common_functions.c
        source/libip6string/ip6tos.c
)

target_compile_definitions(mbed-nanostack-libservice
    PUBLIC
        MBED_CONF_NANOSTACK_LIBSERVICE_PRESENT=1
)

target_link_libraries(mbed-nanostack-libservice PRIVATE mbed-core)
