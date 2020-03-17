# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    kinetis_emac.cpp
    kinetis_emac.h
    kinetis_emac_config.h
)
mbed_add_cmake_directory_if_labels("TARGET")
