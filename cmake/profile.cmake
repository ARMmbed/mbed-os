# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Mapping CMAKE_BUILD_TYPE into MBED_BUILD_TYPES, as we understand only 3 profiles
set(MBED_BUILD_TYPES Debug Release Develop)

# Force the build types to be case-insensitive for checking
set(LOWERCASE_MBED_BUILD_TYPES ${MBED_BUILD_TYPES})
list(TRANSFORM LOWERCASE_MBED_BUILD_TYPES TOLOWER)
string(TOLOWER ${CMAKE_BUILD_TYPE} LOWERCASE_CMAKE_BUILD_TYPE)

get_property(multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(multi_config)
  set(CMAKE_CONFIGURATION_TYPES "${MBED_BUILD_TYPES}" CACHE STRING "List of supported build types" FORCE)
else()
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${MBED_BUILD_TYPES}")
  if(NOT LOWERCASE_CMAKE_BUILD_TYPE IN_LIST LOWERCASE_MBED_BUILD_TYPES)
    message(FATAL_ERROR "Invalid build type '${CMAKE_BUILD_TYPE}'. Possible values:\n ${MBED_BUILD_TYPES}")
  endif()
endif()

include(${MBED_ROOT}/cmake/profiles/${LOWERCASE_CMAKE_BUILD_TYPE}.cmake)
