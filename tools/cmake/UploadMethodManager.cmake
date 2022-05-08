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

include(UploadMethod${UPLOAD_METHOD})

if(NOT "${UPLOAD_${UPLOAD_METHOD}_FOUND}")
	message(FATAL_ERROR "The upload method ${UPLOAD_METHOD} does not have the dependencies needed to run on this machine.")
endif()

if(NOT (("${UPLOAD_METHOD}" STREQUAL NONE) OR ("${${UPLOAD_METHOD}_UPLOAD_ENABLED}")))
	message(FATAL_ERROR "The upload method ${UPLOAD_METHOD} is not enabled in the config code for this target -- set ${UPLOAD_METHOD}_UPLOAD_ENABLED to TRUE to enable it.")
endif()

message(STATUS "Board upload method set to ${UPLOAD_METHOD}")

# ----------------------------------------------
# Generate gdbinit if needed

# path where the gdbinit file will be written
set(GDBINIT_PATH ${CMAKE_CURRENT_BINARY_DIR}/mbed-cmake.gdbinit)

if(UPLOAD_SUPPORTS_DEBUG)
	# create init file for GDB client
	file(GENERATE OUTPUT ${GDBINIT_PATH} CONTENT
"# connect to GDB server
target remote localhost:${GDB_PORT}
")
endif()

# ----------------------------------------------
# Function for creating targets

function(mbed_generate_upload_debug_targets target)
	# add upload target
	gen_upload_target(${target} ${CMAKE_CURRENT_BINARY_DIR}/${target}.bin ${CMAKE_CURRENT_BINARY_DIR}/${target}.hex)

	# add debug target
	if(UPLOAD_SUPPORTS_DEBUG)
		add_custom_target(debug-${target}
			COMMENT "starting GDB to debug ${target}..."
			COMMAND arm-none-eabi-gdb
			--command=${GDBINIT_PATH}
			$<TARGET_FILE:${target}>
			USES_TERMINAL)
	endif()
endfunction()