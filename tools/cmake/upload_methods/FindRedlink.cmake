# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# CMake finder for Redlink, the NXP MCUXpresso debug tool
#
# This module defines:
# Redlink_FOUND - Whether the reqested tools were found.
# crt_emu_cm_redlink_PATH - full path to the Redlink command line tool (crt_emu_cm_redlink).
# REDLINK_FLASH_LOADER_PATH - path to the directory where flash loaders are (contains MIMXRT1050_SFDP_HYPERFLASH.cfx)

# first try to locate MCUXpresso IDE in its default location
set(MCUXPRESSO_IDE_LINUX_HINTS "")
set(MCUXPRESSO_IDE_WINDOWS_HINTS "")
if(EXISTS "C:/nxp/")
    # On Windows, MCUXpresso by default is installed into a subdirectory of
    # C:/nxp
    file(GLOB MCUXPRESSO_IDE_WINDOWS_HINTS LIST_DIRECTORIES TRUE "C:/nxp/MCUXpressoIDE_*/ide")
endif()

find_path(MCUXPRESSO_IDE_PATH
    NAMES mcuxpressoide.ini
    DOC "Path to MCUXPresso IDE.  This directory should contain mcuxpressoide.ini.  Used to find Redlink"
    PATHS
        ${MCUXPRESSO_IDE_WINDOWS_HINTS} # Windows
        /usr/local/mcuxpressoide/ide # Linux
        /Applications/MCUXpressoIDE.app/Contents/Eclipse # OS X
        )

set(Redlink_HINTS "")
if(EXISTS "${MCUXPRESSO_IDE_PATH}")
    message(STATUS "Located MCUXpresso IDE: ${MCUXPRESSO_IDE_PATH}")

    # find install dirs inside IDE, which also have version numbers
    file(GLOB REDLINK_INSTALL_DIRS LIST_DIRECTORIES TRUE "${MCUXPRESSO_IDE_PATH}/plugins/com.nxp.mcuxpresso.tools.bin.*/binaries")
    list(GET REDLINK_INSTALL_DIRS 0 REDLINK_INSTALL_DIR) # If glob returns multiple just pick one
    if(EXISTS "${REDLINK_INSTALL_DIR}")
        list(APPEND Redlink_HINTS ${REDLINK_INSTALL_DIR})
    endif()
elseif()
    message(WARNING "Failed to find MCUXpresso IDE, will still look for Redlink (crt_emu_cm_redlink) on your PATH.  Recommend setting MCUXPRESSO_IDE_PATH to the location of MCUXpresso IDE.")
endif()

find_program(crt_emu_cm_redlink_PATH
    NAMES crt_emu_cm_redlink
    DOC "Path to the Redlink crt_emu_cm_redlink executable."
    HINTS ${Redlink_HINTS})

find_path(REDLINK_FLASH_LOADER_PATH
    NAMES MIMXRT1050_SFDP_HYPERFLASH.cfx
    DOC "Path to the directory where the Redlink flash loaders are (contains MIMXRT1050_SFDP_HYPERFLASH.cfx)."
    HINTS ${Redlink_HINTS}
    PATH_SUFFIXES Flash)

find_package_handle_standard_args(Redlink REQUIRED_VARS crt_emu_cm_redlink_PATH REDLINK_FLASH_LOADER_PATH)


