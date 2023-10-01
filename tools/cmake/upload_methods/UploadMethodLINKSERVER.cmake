# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

### NXP LinkServer Upload Method
# This method needs the following parameters:
# LINKSERVER_DEVICE - Chip name and board to connect to, separated by a colon.
# LINKSERVER_PROBE_SN - Serial number, or serial number substring, of the debug probe to connect to.  If blank, will connect to any probe.

set(UPLOAD_SUPPORTS_DEBUG TRUE)

### Handle options
set(LINKSERVER_PROBE_SN "" CACHE STRING "Serial number, or serial number substring, of the debug probe to connect to.  If blank, will connect to any probe.")

if("${LINKSERVER_PROBE_SN}" STREQUAL "")
	# This argument causes Redlink to connect to the first available debug probe
	set(LINKSERVER_PROBE_ARGS "" CACHE INTERNAL "" FORCE)
else()
	set(LINKSERVER_PROBE_ARGS --probe ${LINKSERVER_PROBE_SN} CACHE INTERNAL "" FORCE)
endif()

if("${LINKSERVER_DEVICE}" STREQUAL "")
	message(FATAL_ERROR "You must set LINKSERVER_DEVICE in your CMake scripts to use REDLINK")
endif()

### Check if upload method can be enabled on this machine
find_package(LinkServer)
set(UPLOAD_LINKSERVER_FOUND ${LinkServer_FOUND})

### Function to generate upload target

function(gen_upload_target TARGET_NAME BIN_FILE HEX_FILE)

	add_custom_target(flash-${TARGET_NAME}
		COMMENT "Flashing ${TARGET_NAME} with LinkServer..."
		COMMAND ${LinkServer_PATH}
			flash
			${LINKSERVER_PROBE_ARGS}
			${LINKSERVER_DEVICE}
			load
			$<TARGET_FILE:${TARGET_NAME}>)

	add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

endfunction(gen_upload_target)

### Commands to run the debug server.
set(UPLOAD_GDBSERVER_DEBUG_COMMAND
	${LinkServer_PATH}
	gdbserver
	${LINKSERVER_PROBE_ARGS}
	--gdb-port ${GDB_PORT}
	${LINKSERVER_DEVICE}
)

# request extended-remote GDB sessions
set(UPLOAD_WANTS_EXTENDED_REMOTE TRUE)

set(UPLOAD_LAUNCH_COMMANDS
	"monitor reset" # undocumented, but works
	"load"
	"break main"
	"monitor reset"

	# Workaround for LinkServer supplying incomplete memory information to GDB
	# (in particular, seems to be missing the peripheral memory space and external RAM).
	# Without this command, GDB will block the user from accessing values in any of these
	# memory spaces.
	"set mem inaccessible-by-default off"
)
set(UPLOAD_RESTART_COMMANDS
	"monitor reset"
)