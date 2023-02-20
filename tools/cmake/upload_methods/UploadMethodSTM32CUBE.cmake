# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

### STM32Cube Upload Method
# This method needs the following parameters:
# STM32CUBE_CONNECT_COMMAND - "Connect" (-c) command to pass to the programmer
# STM32CUBE_GDBSERVER_ARGS - Arguments to pass to the ST-Link gdbserver.
# This method creates the following options:
# STM32CUBE_PROBE_SN - Serial number of the ST-Link probe to connect to.  If blank, will connect to any probe.

set(STM32CUBE_PROBE_SN "" CACHE STRING "Serial number of the ST-Link probe to connect to.  If blank, will connect to any probe.")

### Check if upload method can be enabled on this machine
find_package(STLINKTools COMPONENTS STM32CubeProg OPTIONAL_COMPONENTS STLINK_gdbserver)
set(UPLOAD_STM32CUBE_FOUND ${STLINKTools_FOUND})

if(EXISTS "${STLINK_gdbserver_PATH}")
	set(UPLOAD_SUPPORTS_DEBUG TRUE)
else()
	set(UPLOAD_SUPPORTS_DEBUG FALSE)
endif()

### Function to generate upload target

set(STM32CUBE_UPLOAD_PROBE_ARGS "" CACHE INTERNAL "" FORCE)
set(STM32CUBE_GDB_PROBE_ARGS "" CACHE INTERNAL "" FORCE)

if(NOT "${STM32CUBE_PROBE_SN}" STREQUAL "")
	set(STM32CUBE_UPLOAD_PROBE_ARGS sn=${STM32CUBE_PROBE_SN} CACHE INTERNAL "" FORCE)
	set(STM32CUBE_GDB_PROBE_ARGS --serial-number ${STM32CUBE_PROBE_SN} CACHE INTERNAL "" FORCE)
endif()

function(gen_upload_target TARGET_NAME BIN_FILE HEX_FILE)

	add_custom_target(flash-${TARGET_NAME}
		COMMENT "Flashing ${TARGET_NAME} with STM32CubeProg..."
		COMMAND ${STM32CubeProg_COMMAND}
		${STM32CUBE_CONNECT_COMMAND}
		${STM32CUBE_UPLOAD_PROBE_ARGS} # probe arg must be immediately after -c command as it gets appended to -c
		-w "$<TARGET_FILE:${TARGET_NAME}>"
		-rst)

	add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

endfunction(gen_upload_target)

### Commands to run the debug server.

# The debugger needs to be passed the directory containing STM32CubeProg
get_filename_component(CUBE_PROG_DIR ${STM32CubeProg_PATH} DIRECTORY)

set(UPLOAD_GDBSERVER_DEBUG_COMMAND
	${STLINK_gdbserver_COMMAND}
	${STM32CUBE_GDBSERVER_ARGS}
	-cp "${CUBE_PROG_DIR}"
	-p ${GDB_PORT}
	--halt
	${STM32CUBE_GDB_PROBE_ARGS})

# Reference: https://github.com/Marus/cortex-debug/blob/056c03f01e008828e6527c571ef5c9adaf64083f/src/stlink.ts#L113
set(UPLOAD_LAUNCH_COMMANDS
	"monitor reset"
	"load"
	"break main"
	"monitor reset"
)
set(UPLOAD_RESTART_COMMANDS
	"monitor reset"
)