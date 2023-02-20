# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# ----------------------------------------------
# Load the upload method that the user selects

# This variable should have been set in app.cmake or by the upload method cfg file, sanity check here
if(NOT DEFINED UPLOAD_METHOD_DEFAULT)
	message(FATAL_ERROR "UPLOAD_METHOD_DEFAULT not found.")
endif()

set(UPLOAD_METHOD "${UPLOAD_METHOD_DEFAULT}" CACHE STRING "Method for uploading programs to the mbed")

# use a higher numbered port to allow use without root on Linux/Mac
set(GDB_PORT 23331 CACHE STRING "Port that the GDB server will be started on.")

# Load the upload method.  This is expected to set the following variables:
# UPLOAD_${UPLOAD_METHOD}_FOUND - True iff the dependencies for this upload method were found
# UPLOAD_SUPPORTS_DEBUG - True iff this upload method supports debugging
# UPLOAD_GDBSERVER_DEBUG_COMMAND - Command to start a new GDB server
# UPLOAD_WANTS_EXTENDED_REMOTE - True iff GDB should use "target extended-remote" to connect to the GDB server
# UPLOAD_LAUNCH_COMMANDS - List of GDB commands to run after launching GDB.
# UPLOAD_RESTART_COMMANDS - List of commands to run when the "restart chip" function is used.
# See here for more info: https://github.com/mbed-ce/mbed-os/wiki/Debugger-Commands-and-State-in-Upload-Methods
include(UploadMethod${UPLOAD_METHOD})

if(NOT "${UPLOAD_${UPLOAD_METHOD}_FOUND}")
	message(FATAL_ERROR "The upload method ${UPLOAD_METHOD} does not have the dependencies needed to run on this machine.")
endif()

if(NOT (("${UPLOAD_METHOD}" STREQUAL NONE) OR ("${${UPLOAD_METHOD}_UPLOAD_ENABLED}")))
	message(FATAL_ERROR "The upload method ${UPLOAD_METHOD} is not enabled in the config code for this target -- set ${UPLOAD_METHOD}_UPLOAD_ENABLED to TRUE to enable it.")
endif()

if(UPLOAD_SUPPORTS_DEBUG)
	message(STATUS "Mbed: Code upload and debugging enabled via upload method ${UPLOAD_METHOD}")
elseif(NOT "${UPLOAD_METHOD}" STREQUAL "NONE")
	message(STATUS "Mbed: Code upload enabled via upload method ${UPLOAD_METHOD}")
endif()

# These variables need to be made into a cache variables so that they can
# be seen by higher level directories when they call mbed_generate_upload_debug_targets()
set(MBED_UPLOAD_SUPPORTS_DEBUG ${UPLOAD_SUPPORTS_DEBUG} CACHE INTERNAL "" FORCE)
set(MBED_UPLOAD_GDBSERVER_DEBUG_COMMAND ${UPLOAD_GDBSERVER_DEBUG_COMMAND} CACHE INTERNAL "" FORCE)
set(MBED_UPLOAD_WANTS_EXTENDED_REMOTE ${UPLOAD_WANTS_EXTENDED_REMOTE} CACHE INTERNAL "" FORCE)
set(MBED_UPLOAD_LAUNCH_COMMANDS ${UPLOAD_LAUNCH_COMMANDS} CACHE INTERNAL "" FORCE)
set(MBED_UPLOAD_RESTART_COMMANDS ${UPLOAD_RESTART_COMMANDS} CACHE INTERNAL "" FORCE)

# ----------------------------------------------
# Function for creating targets

function(mbed_generate_upload_target target)
	# add upload target
	gen_upload_target(${target}
		${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_BASE_NAME:${target}>.bin
		${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_BASE_NAME:${target}>.hex
	)
endfunction()