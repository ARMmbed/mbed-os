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
# CMSIS-RTOS2 API
# ============================================================================
#
# CMSIS-RTOS2 API Real-time OS API provides RTOS API headers
# The API is backend-agnostic - works with RTX, FreeRTOS, or other RTOS
# These headers are needed even for bare-metal builds (for type definitions)
#
# Submodule: CMSIS 6 (https://github.com/ARM-software/CMSIS_6)
# Components used: CMSIS-RTOS2 API 2.3.0
# Path: /CMSIS_6/CMSIS/RTOS2

message(STATUS "CMSIS-RTOS2 API 2.3.0")


target_include_directories(mbed-core-flags
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS_6/CMSIS/RTOS2/Include
)

# OS Tick source files
if("CORTEX_A" IN_LIST MBED_TARGET_LABELS)
    # Cortex-A startup
    target_sources(mbed-rtos-sources INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS_6/CMSIS/RTOS2/Source/os_tick_ptim.c
    )
elseif("CORTEX_M" IN_LIST MBED_TARGET_LABELS)
    # Cortex-M startup
    target_sources(mbed-rtos-sources INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS_6/CMSIS/RTOS2/Source/os_systick.c
    )
endif()