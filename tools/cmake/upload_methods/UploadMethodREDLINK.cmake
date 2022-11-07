# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

### Redlink Upload Method
# This method needs the following parameters:
# REDLINK_PART_NUMBER - Part number (-p) argument to pass to the upload tool
# REDLINK_PART_XML_DIR - Directory where the XML files for this MCU can be found.
# REDLINK_CLOCK_SPEED - JTAG/SWD clock speed to talk to the target at.
# REDLINK_CONNECT_ARGS - Extra connect arguments to pass to Redlink tool.  These can be gotten by watching the command that MCUXpresso IDE executes when you start a debug session.
# This method creates the following options:
# REDLINK_PROBE_SN - Serial number of the debug probe to connect to.  If blank, will connect to any probe.

### Handle options
set(REDLINK_PROBE_SN "" CACHE STRING "Serial number of the debug probe to connect to for Redlink.  Set to empty to detect any matching adapter.")

if("${REDLINK_PROBE_SN}" STREQUAL "")
	# This argument causes Redlink to connect to the first available debug probe
	set(REDLINK_PROBE_ARGS --probehandle 1 CACHE INTERNAL "" FORCE)
else()
	set(REDLINK_PROBE_ARGS --probeserial ${REDLINK_PROBE_SN} CACHE INTERNAL "" FORCE)
endif()

if("${REDLINK_PART_NUMBER}" STREQUAL "")
	message(FATAL_ERROR "You must set REDLINK_PART_NUMBER in your CMake scripts to use REDLINK")
endif()

if("${REDLINK_PART_XML_DIR}" STREQUAL "")
	message(FATAL_ERROR "You must set REDLINK_PART_XML_DIR in your CMake scripts to use REDLINK")
endif()

if("${REDLINK_CLOCK_SPEED}" STREQUAL "")
	message(FATAL_ERROR "You must set REDLINK_CLOCK_SPEED in your CMake scripts to use REDLINK")
endif()

### Check if upload method can be enabled on this machine
find_package(Redlink)
set(UPLOAD_REDLINK_FOUND ${Redlink_FOUND})

### Function to generate upload target

function(gen_upload_target TARGET_NAME BIN_FILE HEX_FILE)

	add_custom_target(flash-${TARGET_NAME}
		COMMENT "Flashing ${TARGET_NAME} with Redlink..."
		COMMAND ${crt_emu_cm_redlink_PATH}
			-p ${REDLINK_PART_NUMBER}
			--flash-hashing
			-x ${REDLINK_PART_XML_DIR}
			--flash-dir ${REDLINK_FLASH_LOADER_PATH}
			-g
			-s ${REDLINK_CLOCK_SPEED}
			${REDLINK_CONNECT_ARGS}
			${REDLINK_PROBE_ARGS}
			--flash-load-exec $<TARGET_FILE:${TARGET_NAME}>)

	add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

endfunction(gen_upload_target)

### Function to generate debug target
add_custom_target(gdbserver
	COMMENT "Starting Redlink GDB server"
	COMMAND
		${crt_emu_cm_redlink_PATH}
		-p ${REDLINK_PART_NUMBER}
		--flash-hashing
		-x ${REDLINK_PART_XML_DIR}
		--flash-dir ${REDLINK_FLASH_LOADER_PATH}
		-g
		-s ${REDLINK_CLOCK_SPEED}
		-2
		${REDLINK_CONNECT_ARGS}
		${REDLINK_PROBE_ARGS}
		--server :${GDB_PORT}
		--vc
		--connect-reset system
		--kill-server # Because redlink seems to not handle Ctrl-C, we use this to close it when GDB exits
	USES_TERMINAL
	VERBATIM)


# request extended-remote GDB sessions
set(UPLOAD_WANTS_EXTENDED_REMOTE TRUE)