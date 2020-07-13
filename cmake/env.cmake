# SPDX-License-Identifier: Apache-2.0

# Compiler setup
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Project setup
enable_language(C CXX ASM)

# Enabled components within a project
# TODO: @mbed-os-tools This needs to come from tools
# If this is a list like rtos/wifi/networking, we need to figure out the path in Mbed OS (have dictionary or
# something similar). The best would be if this list contains list of module paths (rtos/ features/ble etc)
set_property(GLOBAL PROPERTY MBED_COMPONENTS "")

