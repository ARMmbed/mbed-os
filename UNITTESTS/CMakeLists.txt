# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/.. CACHE INTERNAL "")

# Setup c++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MINGW)
    # enable PRIx formatting globally
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__STDC_FORMAT_MACROS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__STDC_FORMAT_MACROS")
endif (MINGW)

####################
# GTEST
####################
include(FetchContent)
# Download and unpack googletest
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
)
FetchContent_MakeAvailable(googletest)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


####################
# TESTING
####################

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        "${CMAKE_BINARY_DIR}/Testing"
        )

####################
# CODE COVERAGE SETUP
####################

if (COVERAGE)

    if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "Non-debug build may result misleading code coverage results.")
    endif()

    # Append coverage compiler flags
    set(COVERAGE_COMPILER_FLAGS "-g -O0 --coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}")

endif(COVERAGE)

if (VALGRIND)
    find_program(MEMORYCHECK_COMMAND valgrind)
endif(VALGRIND)

####################
# UNIT TESTS
####################

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUNITTEST")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNITTEST")

add_subdirectory(stubs)

macro(mbed_add_all_subdirectories subdirectories_list)
    foreach(dir ${subdirectories_list})
        if(IS_DIRECTORY  ${CMAKE_CURRENT_LIST_DIR}/${dir})
            add_subdirectory(${dir})
        endif()
    endforeach()
endmacro()
