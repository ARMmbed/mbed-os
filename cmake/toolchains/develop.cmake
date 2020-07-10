include(common.cmake)

set(DEVELOP_DEFINITIONS
    MBED_TRAP_ERRORS_ENABLED=1
)


if(MBED_TOOLCHAIN strequal "GCC_ARM")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    -c
    -std=gnu11
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    -c
    -std=gnu++14
    -fno-rtti
    -Wvla
)
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS}
    -c
    -x
    assembler-with-cpp
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    -Wl,--gc-sections
    -Wl,--wrap,main
    -Wl,--wrap,_malloc_r
    -Wl,--wrap,_free_r
    -Wl,--wrap,_realloc_r
    -Wl,--wrap,_memalign_r
    -Wl,--wrap,_calloc_r
    -Wl,--wrap,exit
    -Wl,--wrap,atexit
    -Wl,-n
)


elseif(MBED_TOOLCHAIN strequal "ARMC6")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    -std=gnu11
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    -fno-rtti
    -fno-c++-static-destructors
    -std=gnu++14
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    --show_full_path
    --legacyalign
    --inline
    --any_contingency
    --keep=os_cb_sections
)

set(DEVELOP_DEFINITIONS ${DEVELOP_DEFINITIONS}
    __ASSERT_MSG
)


elseif(MBED_TOOLCHAIN strequal "ARM")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    --md
    --no_depend_system_headers
    --c99
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    --cpp11
    --no_rtti
    --no_vla
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    --show_full_path
    --any_contingency
    --inline
    --keep=os_cb_sections
)

set(DEVELOP_DEFINITIONS ${DEVELOP_DEFINITIONS}
    __ASSERT_MSG
)


elseif(MBED_TOOLCHAIN strequal "uARM")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    --md
    --no_depend_system_headers
    --c99
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    --cpp11
    --no_rtti
    --no_vla
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    --inline
    --library_type=microlib
)

set(DEVELOP_DEFINITIONS ${DEVELOP_DEFINITIONS}
    __ASSERT_MSG
    MBED_RTOS_SINGLE_THREAD
)


elseif(MBED_TOOLCHAIN strequal "IAR")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    --vla
    --diag_suppress=Pe546
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    --guard_calls
    --no_static_destruction
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    --skip_dynamic_initialization
    --threaded_lib
    --inline
)


endif()
