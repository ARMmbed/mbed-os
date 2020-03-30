cmake_minimum_required(VERSION 3.0.2)

set(PROJECT_NAME unittests)
set(LIB_NAME MbedOS)

project(${PROJECT_NAME})

# Setup c++ standard
macro(use_cxx14)
    if (CMAKE_VERSION VERSION_LESS 3.1)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")
        endif()
    else()
        set(CMAKE_CXX_STANDARD 14)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
    endif()
endmacro()

use_cxx14()


if (MINGW)
  # enable PRIx formatting globally
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__STDC_FORMAT_MACROS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__STDC_FORMAT_MACROS")
endif (MINGW)

####################
# GTEST
####################

# Download and unpack googletest at configure time
configure_file(googletest-CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake failed for google test: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build failed for google test: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main/gmock/gmock_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later.
target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
  "$<BUILD_INTERFACE:${gtest_SOURCE_DIR}/include>"
  "$<BUILD_INTERFACE:${gmock_SOURCE_DIR}/include>")

####################
# TESTING
####################

include(CTest)

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

# Set include dirs.
set(unittest-includes-base
  "${PROJECT_SOURCE_DIR}/target_h"
  "${PROJECT_SOURCE_DIR}/target_h/events"
  "${PROJECT_SOURCE_DIR}/target_h/events/equeue"
  "${PROJECT_SOURCE_DIR}/target_h/platform"
  "${PROJECT_SOURCE_DIR}/target_h/platform/cxxsupport"
  "${PROJECT_SOURCE_DIR}/target_h/drivers"
  "${PROJECT_SOURCE_DIR}/stubs"
  "${PROJECT_SOURCE_DIR}/.."
  "${PROJECT_SOURCE_DIR}/../features"
  "${PROJECT_SOURCE_DIR}/../features/netsocket"
  "${PROJECT_SOURCE_DIR}/../platform"
  "${PROJECT_SOURCE_DIR}/../drivers"
  "${PROJECT_SOURCE_DIR}/../hal"
  "${PROJECT_SOURCE_DIR}/../events"
  "${PROJECT_SOURCE_DIR}/../events/source"
  "${PROJECT_SOURCE_DIR}/../events/internal"
  "${PROJECT_SOURCE_DIR}/../rtos"
  "${PROJECT_SOURCE_DIR}/../rtos/TARGET_CORTEX"
  "${PROJECT_SOURCE_DIR}/../rtos/TARGET_CORTEX/rtx5/Include"
  "${PROJECT_SOURCE_DIR}/../cmsis"
  "${PROJECT_SOURCE_DIR}/../features/frameworks"
  "${PROJECT_SOURCE_DIR}/../features/frameworks/mbed-trace"
  "${PROJECT_SOURCE_DIR}/../features/frameworks/nanostack-libservice"
  "${PROJECT_SOURCE_DIR}/../features/frameworks/nanostack-libservice/mbed-client-libservice"
  "${PROJECT_SOURCE_DIR}/../features/filesystem/fat"
  "${PROJECT_SOURCE_DIR}/../features/filesystem/fat/ChaN"
  "${PROJECT_SOURCE_DIR}/../features/filesystem/bd"
  "${PROJECT_SOURCE_DIR}/../features/filesystem/"
  "${PROJECT_SOURCE_DIR}/../features/filesystem/littlefs"
  "${PROJECT_SOURCE_DIR}/../features/filesystem/littlefs/littlefs"
  "${PROJECT_SOURCE_DIR}/../features/cellular/framework/API"
  "${PROJECT_SOURCE_DIR}/../features/cellular/framework/AT"
  "${PROJECT_SOURCE_DIR}/../features/cellular/framework/device"
  "${PROJECT_SOURCE_DIR}/../features/cellular/framework"
  "${PROJECT_SOURCE_DIR}/../features/cellular/framework/common"
  "${PROJECT_SOURCE_DIR}/../features/lorawan"
  "${PROJECT_SOURCE_DIR}/../features/lorawan/lorastack"
  "${PROJECT_SOURCE_DIR}/../features/lorawan/lorastack/mac"
  "${PROJECT_SOURCE_DIR}/../features/lorawan/lorastack/phy"
  "${PROJECT_SOURCE_DIR}/../features/lorawan/system"
  "${PROJECT_SOURCE_DIR}/../features/mbedtls"
  "${PROJECT_SOURCE_DIR}/../features/mbedtls/inc"
  "${PROJECT_SOURCE_DIR}/../features/mbedtls/mbed-crypto/inc"
  "${PROJECT_SOURCE_DIR}/../features/storage/kvstore/conf"
)

# Create a list for test suites.
set(TEST_SUITES)

# Get all matched tests.
file(GLOB_RECURSE unittest-file-list
  "unittest.cmake"
)

if ("${unittest-file-list}" STREQUAL "")
  message(FATAL_ERROR "No tests found. Exiting...")
endif()

# Create unit test targets
foreach(testfile ${unittest-file-list})
  ####################
  # DEFINE TARGETS
  ####################

  # Init file lists.
  set(unittest-includes ${unittest-includes-base})
  set(unittest-sources)
  set(unittest-test-sources)
  set(unittest-test-flags)

  # Get source files
  include("${testfile}")

  get_filename_component(TEST_SUITE_DIR ${testfile} DIRECTORY)

  file(RELATIVE_PATH
       TEST_SUITE_NAME # output
       ${PROJECT_SOURCE_DIR} # root
       ${TEST_SUITE_DIR} #abs dirpath
  )

  string(REGEX REPLACE "/|\\\\" "-" TEST_SUITE_NAME ${TEST_SUITE_NAME})

  set(TEST_SUITES ${TEST_SUITES} ${TEST_SUITE_NAME})

  set(LIBS_TO_BE_LINKED gmock_main)

  # Build directories list
  set(BUILD_DIRECTORIES)

  if (unittest-sources)
    # Create the testable static library.
    add_library("${TEST_SUITE_NAME}.${LIB_NAME}" STATIC ${unittest-sources})
    target_include_directories("${TEST_SUITE_NAME}.${LIB_NAME}" PRIVATE
      ${unittest-includes})
    target_compile_options("${TEST_SUITE_NAME}.${LIB_NAME}" PRIVATE
      ${unittest-test-flags})
    set(LIBS_TO_BE_LINKED ${LIBS_TO_BE_LINKED} "${TEST_SUITE_NAME}.${LIB_NAME}")

    # Append lib build directory to list
    list(APPEND BUILD_DIRECTORIES "./CMakeFiles/${TEST_SUITE_NAME}.${LIB_NAME}.dir")
  endif(unittest-sources)

  if (unittest-test-sources)
    # Create the executable.
    add_executable(${TEST_SUITE_NAME} ${unittest-test-sources})

    target_include_directories(${TEST_SUITE_NAME} PRIVATE
      ${unittest-includes})
    target_compile_options(${TEST_SUITE_NAME} PRIVATE
      ${unittest-test-flags})

    # Link the executable with the libraries.
    target_link_libraries(${TEST_SUITE_NAME} ${LIBS_TO_BE_LINKED})

    add_test(NAME "${TEST_SUITE_NAME}" COMMAND ${TEST_SUITE_NAME})

    # Append test build directory to list
    list(APPEND BUILD_DIRECTORIES "./CMakeFiles/${TEST_SUITE_NAME}.dir")
  else()
    message(WARNING "No test source files found for ${TEST_SUITE_NAME}.\n")
  endif(unittest-test-sources)
endforeach(testfile)

