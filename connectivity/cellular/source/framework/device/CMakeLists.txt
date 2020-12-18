# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-cellular
    INTERFACE
        ATHandler.cpp
        CellularContext.cpp
        CellularDevice.cpp
        CellularStateMachine.cpp
)
