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
# CMSIS-RTX (RTOS Backend)
# ============================================================================
#
# CMSIS-RTX provides the RTX5 RTOS kernel implementation for CMSIS-RTOS2 API
#
# Submodule: CMSIS-RTX (https://github.com/ARM-software/CMSIS-RTX)
# Components used: CMSIS-RTX 5.9.0
# Path: /CMSIS-RTX

# Submodule presence
if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX)
    message(FATAL_ERROR "CMSIS-RTX submodule directory not found at ${CMSIS_6_ROOT}.")
endif()

# Make RTX headers and Mbed's device RTOS wrapper headers visible to ALL profiles
# (needed for typedefs like mbed_rtos_storage_* used by headers even in bare-metal)
target_include_directories(mbed-core-flags
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/../device/rtos/include/RTX/config
    ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Include
)

if(APPLICATION_PROFILE_CONFIG_FULL)
    message(STATUS "CMSIS-RTX 5.9.0")
  
    # Source directory only needed when actually building the kernel (full profile)
    target_include_directories(mbed-rtos-flags
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source
    )

    # RTX source files (only compiled when building with RTOS)
    target_sources(mbed-rtos-sources
        INTERFACE
        # Old CMSIS-RTOS v1 compatibility layer
        ${CMAKE_CURRENT_LIST_DIR}/../device/rtos/source/cmsis_os1.c     
        # Configuration
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Config/RTX_Config.c      
        # RTX kernel sources
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_delay.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_evflags.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_evr.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_kernel.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_lib.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_memory.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_mempool.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_msgqueue.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_mutex.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_semaphore.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_system.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_thread.c
        ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/rtx_timer.c
    )

    # Add toolchain-specific interrupt handlers based on core type
    if(${CMAKE_CROSSCOMPILING})
        set(RTX_IRQ_FILE_PATH "${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Source/GCC/")
        # Determine IRQ handler file based on Cortex core type
        # CMSIS-RTX 5.9.0 uses ARM architecture names (armv6m, armv7m, armv8m, etc.)
        foreach(core_label ${MBED_TARGET_LABELS})
            if(${core_label} STREQUAL "CORTEX_A")
                # Cortex-A startup
                target_sources(mbed-rtos-sources INTERFACE
                    ${CMAKE_CURRENT_LIST_DIR}/../CMSIS-RTX/Config/handlers.c
                )
                set(RTX_IRQ_FILE "irq_armv7a.S")
            elseif(${core_label} MATCHES "M0")
                # Cortex-M0/M0+ use ARMv6-M architecture
                set(RTX_IRQ_FILE "irq_armv6m.S")
            elseif(${core_label} MATCHES "M23")
                # Cortex-M23 uses ARMv8-M Baseline
                set(RTX_IRQ_FILE "irq_armv8mbl.S")
            elseif(${core_label} MATCHES "M(3|4|7)")
                # Cortex-M3/M4/M7 use ARMv7-M architecture
                set(RTX_IRQ_FILE "irq_armv7m.S") #irq_cm4f.S
            elseif(${core_label} MATCHES "M(33|35|55|85)")
                # Cortex-M33/M35/M55/M85 use ARMv8-M Mainline
                set(RTX_IRQ_FILE "irq_armv8mml.S")
            endif()
        endforeach()
            
        # Add the IRQ handler file if determined
        if(DEFINED RTX_IRQ_FILE AND EXISTS ${RTX_IRQ_FILE_PATH}${RTX_IRQ_FILE})
            target_sources(mbed-rtos-sources INTERFACE ${RTX_IRQ_FILE_PATH}${RTX_IRQ_FILE})
        else()
            message(WARNING "CMSIS-RTX: No IRQ handler found for this target (labels: ${MBED_TARGET_LABELS})")
        endif()
     endif()
endif()