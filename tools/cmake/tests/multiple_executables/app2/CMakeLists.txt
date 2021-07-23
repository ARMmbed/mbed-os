# Copyright (c) 2021 Arm Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_executable(app2 main.cpp)

target_link_libraries(app2 mbed-os)

mbed_set_post_build(app2)
