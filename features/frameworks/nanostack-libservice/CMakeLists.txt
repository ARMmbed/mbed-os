# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    mbed-client-libservice/common_functions.h
    mbed-client-libservice/ip4string.h
    mbed-client-libservice/ip6string.h
    mbed-client-libservice/ip_fsc.h
    mbed-client-libservice/ns_list.h
    mbed-client-libservice/ns_nvm_helper.h
    mbed-client-libservice/ns_trace.h
    mbed-client-libservice/ns_types.h
    mbed-client-libservice/nsdynmemLIB.h
    mbed-client-libservice/platform/arm_hal_interrupt.h
    mbed-client-libservice/platform/arm_hal_nvm.h
    source/IPv6_fcf_lib
    source/IPv6_fcf_lib/ip_fsc.c
    source/libBits/common_functions.c
    source/libList/ns_list.c
    source/libTrace/scripts/fetch_groups.sh
    source/libip4string/ip4tos.c
    source/libip4string/stoip4.c
    source/libip6string/ip6tos.c
    source/libip6string/stoip6.c
    source/nsdynmemLIB/nsdynmemLIB.c
    source/nvmHelper/ns_nvm_helper.c
)
target_include_directories(mbed-os PRIVATE mbed-client-libservice)
target_include_directories(mbed-os PRIVATE mbed-client-libservice/platform)
