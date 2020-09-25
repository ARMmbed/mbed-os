# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-nanostack-libservice OBJECT)

mbed_configure_component(mbed-os-nanostack-libservice)

target_sources(mbed-os-nanostack-libservice
    PRIVATE
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
target_include_directories(mbed-os-nanostack-libservice
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/mbed-client-libservice
        ${CMAKE_CURRENT_SOURCE_DIR}/mbed-client-libservice/platform
)
