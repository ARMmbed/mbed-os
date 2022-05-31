# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nanostack-mbed_mesh_api STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-nanostack-mbed_mesh_api
    PUBLIC
        .
        ./mbed-mesh-api
        ./source/include
        ../include/nanostack-interface
    )

target_sources(mbed-nanostack-mbed_mesh_api
    PRIVATE
        source/CallbackHandler.cpp
        source/LoWPANNDInterface.cpp
        source/MeshInterfaceNanostack.cpp
        source/NanostackEMACInterface.cpp
        source/NanostackEthernetInterface.cpp
        source/NanostackMemoryManager.cpp
        source/NanostackPPPInterface.cpp
        source/ThreadInterface.cpp
        source/WisunBorderRouter.cpp
        source/WisunInterface.cpp
        source/ethernet_tasklet.c
        source/mesh_system.c
        source/nd_tasklet.c
        source/thread_tasklet.c
        source/wisun_tasklet.c
)

target_link_libraries(mbed-nanostack-mbed_mesh_api
    PUBLIC
        mbed-nanostack-hal_mbed_cmsis_rtos
        mbed-nanostack-sal_stack
        mbed-netsocket-api
        mbed-core-flags
    PRIVATE
        mbed-rtos-flags
)
