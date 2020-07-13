# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(COMMON_FLAGS " \
    -c \
    --gnu \
    -Ospace \
    --split_sections \
    --apcs=interwork \
    --brief_diagnostics \
    --restrict \
    --multibyte_chars \
    -O3 \
    --library_type=microlib \
")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    ${COMMON_FLAGS} \
")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    ${COMMON_FLAGS} \
")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} \
    ${COMMON_FLAGS} \
")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} \
    ${COMMON_FLAGS} \
")
