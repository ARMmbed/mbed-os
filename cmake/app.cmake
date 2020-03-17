# SPDX-License-Identifier: Apache-2.0

# Include other required CMake files
# TODO: @mbed-os-tools This needs to come from tools, we should include here toolchain cmake
include(${MBED_OS_ROOT}/cmake/gcc.cmake)

include(${MBED_OS_ROOT}/cmake/target.cmake)
include(${MBED_OS_ROOT}/cmake/env.cmake)
include(${MBED_OS_ROOT}/cmake/util.cmake)

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
