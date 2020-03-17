# SPDX-License-Identifier: Apache-2.0

# PREFIX - TARGET or similar (can be empty string)
# KEYWORD_LIST - list of labels that are used to include directories
function(mbed_add_cmake_directory_if_labels PREFIX)
    get_property(target_labels GLOBAL PROPERTY MBED_OS_TARGET_LABELS)
    foreach(key ${target_labels})
        if(NOT "${PREFIX}" STREQUAL "")
            string(PREPEND key ${PREFIX} "_")
        endif()
        # assumption: relative path, no need to check for absolute here
        set(path ${CMAKE_CURRENT_SOURCE_DIR}/${key})
        if (EXISTS ${path})
            add_subdirectory(${path})
        endif()
    endforeach()
endfunction()

# Include other required CMake files
# TODO: @mbed-os-tools This needs to come from tools, we should include here toolchain cmake
include(${MBED_OS_ROOT}/cmake/gcc.cmake)

include(${MBED_OS_ROOT}/cmake/target.cmake)
include(${MBED_OS_ROOT}/cmake/env.cmake)

# Compiler setup
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Project setup
enable_language(C CXX ASM)

# if the environment does not specify build type, set to Debug
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "RelWithDebInfo"
        CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

# Create application executable
add_executable(app)

# Include Mbed OS main cmake
add_subdirectory(mbed-os)

# Include project configuration
add_subdirectory(gen_config)

# Link the example libs
target_link_libraries(app mbed-os gen_config)

# Custom pre/post build steps
add_custom_command(TARGET app PRE_LINK ${CMAKE_PRE_BUILD_COMMAND})
add_custom_command(TARGET app POST_BUILD ${CMAKE_POST_BUILD_COMMAND})
