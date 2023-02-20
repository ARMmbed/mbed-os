# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

### pyOCD Upload Method
# This method needs the following parameters:
# PYOCD_TARGET_NAME - Name of your processor as passed to the -t option of pyOCD. This is usually the full or partial model number.
# PYOCD_CLOCK_SPEED - Clock speed of the JTAG or SWD connection. Default is in Hz, but can use k and M suffixes for MHz and GHz
# This method creates the following options:
# PYOCD_PROBE_UID - Probe UID to pass to pyOCD commands. You can get the UIDs from `python -m pyocd list`.  Set to empty to detect any probe.

set(UPLOAD_SUPPORTS_DEBUG TRUE)

### Check if upload method can be enabled on this machine
include(CheckPythonPackage)
check_python_package(pyocd HAVE_PYOCD)
set(UPLOAD_PYOCD_FOUND ${HAVE_PYOCD})

### Setup options
set(PYOCD_PROBE_UID "" CACHE STRING "Probe UID to pass to pyOCD commands. You can get the UIDs from `python -m pyocd list`.  Set to empty to detect any probe.")

### Function to generate upload target
set(PYOCD_PROBE_ARGS "")
if(NOT "${PYOCD_PROBE_UID}" STREQUAL "")
	set(PYOCD_PROBE_ARGS --probe ${PYOCD_PROBE_UID} CACHE INTERNAL "" FORCE)
endif()

function(gen_upload_target TARGET_NAME BIN_FILE)

	add_custom_target(flash-${TARGET_NAME}
		COMMENT "Flashing ${TARGET_NAME} with pyOCD..."
		COMMAND ${Python3_EXECUTABLE}
		-m pyocd
		flash
		-v
		--no-wait
		-f ${PYOCD_CLOCK_SPEED}
		-t ${PYOCD_TARGET_NAME}
		${PYOCD_PROBE_ARGS}
		${BIN_FILE})

	add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})
endfunction(gen_upload_target)

### Commands to run the debug server.
set(UPLOAD_GDBSERVER_DEBUG_COMMAND
	${Python3_EXECUTABLE}
	-m pyocd
	gdbserver
	--no-wait
	-t ${PYOCD_TARGET_NAME}
	${PYOCD_PROBE_ARGS}
	-f ${PYOCD_CLOCK_SPEED}
	-p ${GDB_PORT})

# Reference: https://github.com/Marus/cortex-debug/blob/056c03f01e008828e6527c571ef5c9adaf64083f/src/pyocd.ts#L40
set(UPLOAD_LAUNCH_COMMANDS
"monitor reset halt"
"load"
"break main"
)
set(UPLOAD_RESTART_COMMANDS
"monitor reset"
)