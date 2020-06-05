# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        kinetis_emac.cpp
)

target_include_directories(mbed-os
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

mbed_add_cmake_directory_if_labels("TARGET")
