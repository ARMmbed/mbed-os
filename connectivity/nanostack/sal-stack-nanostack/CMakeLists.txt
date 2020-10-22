# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(source/6LoWPAN)
add_subdirectory(source/BorderRouter)
add_subdirectory(source/Common_Protocols)
add_subdirectory(source/Core)
add_subdirectory(source/DHCPv6_Server)
add_subdirectory(source/DHCPv6_client)
add_subdirectory(source/MAC)
add_subdirectory(source/MLE)
add_subdirectory(source/MPL)
add_subdirectory(source/NWK_INTERFACE)
add_subdirectory(source/RPL)
add_subdirectory(source/Security)
add_subdirectory(source/Service_Libs)
add_subdirectory(source/configs)
add_subdirectory(source/ipv6_stack)
add_subdirectory(source/libDHCPv6)
add_subdirectory(source/libNET)

target_include_directories(mbed-nanostack-sal_stack
    INTERFACE
        .
        ./nanostack
        ./nanostack/platform
        ./source
)

target_link_libraries(mbed-nanostack
    INTERFACE
        mbed-nanostack-coap_service
)
