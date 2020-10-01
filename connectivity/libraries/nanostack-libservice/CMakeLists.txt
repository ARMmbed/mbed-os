# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-nanostack-libservice INTERFACE)

target_include_directories(mbed-os-nanostack-libservice
    INTERFACE
        .
        ./mbed-client-libservice
        ./mbed-client-libservice/platform
)

target_sources(mbed-os-nanostack-libservice
    INTERFACE
        source/IPv6_fcf_lib/ip_fsc.c
        source/libBits/common_functions.c
        source/libList/ns_list.c
        source/libip4string/ip4tos.c
        source/libip4string/stoip4.c
        source/libip6string/ip6tos.c
        source/libip6string/stoip6.c
        source/nsdynmemLIB/nsdynmemLIB.c
        source/nvmHelper/ns_nvm_helper.c
)

target_compile_definitions(mbed-os-nanostack-libservice
    INTERFACE
        MBED_CONF_NANOSTACK_LIBSERVICE_PRESENT=1
)
