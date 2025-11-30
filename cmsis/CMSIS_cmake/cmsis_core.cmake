# Copyright (c) 2025 MbedCE Community Contributors (Jan Kamidra)
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Created by: Jan Kamidra with GitHub Copilot

# CMSIS Integration for MbedCE
# ============================================================================
# CMSIS-Core
# ============================================================================
#
# This file integrates CMSIS Core from external submodules CMSIS_6 without
# modifying the submodule contents. 
# Provides Core headers for Cortex-M and Cortex-A processors which are 
# needed for all builds (bare-metal and RTOS)
#
# Submodule: CMSIS 6 (https://github.com/ARM-software/CMSIS_6)
# Components used: CMSIS-Core 6.1.1
# Path: CMSIS_6/CMSIS/Core

message(STATUS "CMSIS-Core 6.1.1")

# Submodule presence
if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/../CMSIS_6)
    message(FATAL_ERROR "CMSIS_6 submodule directory not found!")
endif()

# Add CMSIS-Core base include directories
target_include_directories(mbed-core-flags
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS_6/CMSIS/Core/Include 
)

if("CORTEX_A" IN_LIST MBED_TARGET_LABELS)
    message(STATUS "CMSIS-Core CORTEX-A")
    # Cortex-A startup
    add_library(mbed-cmsis-cortex-a INTERFACE)

    target_include_directories(mbed-cmsis-cortex-a
        INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/../CMSIS_6/CMSIS/Core/Include/a-profile
    )
    
    target_sources(mbed-cmsis-cortex-a
        INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/../CMSIS_6/CMSIS/Core/Source/irq_ctrl_gic.c
    )

elseif("CORTEX_M" IN_LIST MBED_TARGET_LABELS)
    message(STATUS "CMSIS-Core CORTEX-M")
    # Cortex-M startup
    add_library(mbed-cmsis-cortex-m INTERFACE)

    target_include_directories(mbed-cmsis-cortex-m
        INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/../CMSIS_6/CMSIS/Core/Include/m-profile 
    )

    target_sources(mbed-cmsis-cortex-m
        INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/../device/rtos/Source/mbed_tz_context.c
    )

endif()