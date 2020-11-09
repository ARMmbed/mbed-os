# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-mbed_mesh_api
    INTERFACE
        .
        ./mbed-mesh-api
        ./source/include
)

target_sources(mbed-nanostack-mbed_mesh_api
    INTERFACE
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

target_link_libraries(mbed-nanostack
    INTERFACE
        mbed-nanostack-hal_mbed_cmsis_rtos
)
