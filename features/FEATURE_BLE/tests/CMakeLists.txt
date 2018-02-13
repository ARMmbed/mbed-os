cmake_minimum_required(VERSION 2.8.11)

# Make PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR, and PROJECT_NAME available.
set(PROJECT_NAME ble-tests)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

################################
# GTEST
################################

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories(BEFORE SYSTEM
    "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")
else()
  target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
    "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")
endif()


################################
# Testing
################################

enable_testing()

###############################
# GattClient test
###############################

add_executable(gatt-client-tests 
  mbed_os_stub/mbed_assert.c
  generic/GattClient/mock/MockCallbacks.cpp
  generic/GattClient/mock/MockPalGattClient.cpp 
  generic/GattClient/util/Equality.cpp 
  generic/GattClient/TestCharacteristicDesctiptorDiscovery.cpp
  generic/GattClient/TestDiscoverAllServices.cpp
  generic/GattClient/TestNoCb.cpp
  generic/GattClient/TestRead.cpp
  generic/GattClient/TestServerEvent.cpp
  generic/GattClient/TestWrite.cpp
  ${PROJECT_SOURCE_DIR}/../source/generic/GenericGattClient.cpp
)

target_include_directories(gatt-client-tests PRIVATE 
  "${PROJECT_SOURCE_DIR}/.." 
  "${PROJECT_SOURCE_DIR}/../../.." 
  "${PROJECT_SOURCE_DIR}/generic/GattClient"
)

# Standard linking to gtest stuff.
target_link_libraries(gatt-client-tests gmock_main)

# This is so you can do 'make gatt-client-tests' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
add_test(NAME AllUnitTests COMMAND gatt-client-tests)


