# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-cellular
    INTERFACE
        AT_CellularContext.cpp
        AT_CellularDevice.cpp
        AT_CellularInformation.cpp
        AT_CellularNetwork.cpp
        AT_CellularSMS.cpp
        AT_CellularStack.cpp
        AT_ControlPlane_netif.cpp
)
