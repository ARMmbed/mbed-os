# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(DEVELOP_DEFINITIONS
    -DMBED_TRAP_ERRORS_ENABLED=1
)

if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
-c \
-std=gnu11 \
")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  \
-std=gnu++14 \
-fno-rtti \
-Wvla \
")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} \
-x assembler-with-cpp \
")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} \
-Wl,--gc-sections \
-Wl,--wrap,main \
-Wl,--wrap,_malloc_r \
-Wl,--wrap,_free_r \
-Wl,--wrap,_realloc_r \
-Wl,--wrap,_memalign_r \
-Wl,--wrap,_calloc_r \
-Wl,--wrap,exit \
-Wl,--wrap,atexit \
-Wl,-n \
")

elseif(MBED_TOOLCHAIN STREQUAL "ARM")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    -std=gnu11 \
")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -fno-rtti \
    -fno-c++-static-destructors \
    -std=gnu++14 \
")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} \
    --show_full_path \
    --legacyalign \
    --inline \
    --any_contingency \
    --keep=os_cb_sections \
")

set(DEVELOP_DEFINITIONS ${DEVELOP_DEFINITIONS}
    -D__ASSERT_MSG
)

endif()

add_definitions(${DEVELOP_DEFINITIONS})
