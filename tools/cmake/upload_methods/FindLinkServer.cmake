# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# CMake finder for LinkServer, the NXP command-line flash and debug tool
#
# This module defines:
# LinkServer - Whether the reqested tools were found.
# LinkServer_PATH - full path to the LinkServer command line tool.

# Check for LinkServer install folders on Windows
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
    # On Windows, LinkServer by default is installed into a subdirectory of
    # C:/nxp
    file(GLOB LINKSERVER_HINTS LIST_DIRECTORIES TRUE "C:/nxp/LinkServer_*")
elseif("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Darwin")
    # On Mac, it was observed to install into /Applications/LinkServer_1.2.45/dist
    file(GLOB LINKSERVER_HINTS LIST_DIRECTORIES TRUE "/Applications/LinkServer_*/dist")
else()
    set(LINKSERVER_HINTS /usr/local/LinkServer) # Linux package install location
endif()

find_program(LinkServer_PATH
    NAMES LinkServer
    DOC "Path to the LinkServer executable."
    HINTS ${LINKSERVER_HINTS}
)

find_package_handle_standard_args(LinkServer REQUIRED_VARS LinkServer_PATH)


