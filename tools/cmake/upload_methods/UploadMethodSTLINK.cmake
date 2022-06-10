# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

### stlink Upload Method
# See docs for valid parameters.

set(UPLOAD_SUPPORTS_DEBUG TRUE)

### Check if upload method can be enabled on this machine
find_package(stlink)
set(UPLOAD_STLINK_FOUND ${stlink_FOUND})

### Figure out --serial argument
if(DEFINED STLINK_PROBE_SN AND NOT "${STLINK_PROBE_SN}" STREQUAL "")
	set(STLINK_SERIAL_ARGUMENT --serial ${STLINK_PROBE_SN} CACHE INTERNAL "" FORCE)
else()
	set(STLINK_SERIAL_ARGUMENT "" CACHE INTERNAL "" FORCE)
endif()

### Function to generate upload target
function(gen_upload_target TARGET_NAME BIN_FILE)

	add_custom_target(flash-${TARGET_NAME}
		COMMENT "Flashing ${TARGET_NAME} with stlink..."
		COMMAND ${st-flash_PATH}
		--reset # Reset chip after flashing
		${STLINK_SERIAL_ARGUMENT}
		${STLINK_ARGS}
		write ${BIN_FILE} ${STLINK_LOAD_ADDRESS})

	add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})
endfunction(gen_upload_target)

### Function to generate debug target
add_custom_target(gdbserver
	COMMENT "Starting st-util GDB server"
	COMMAND
	${st-util_PATH}
	${STLINK_SERIAL_ARGUMENT}
	${STLINK_ARGS}
	--listen_port=${GDB_PORT}
	USES_TERMINAL)
