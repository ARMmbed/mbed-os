# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

### J-Link Upload Method
#   This method needs the following parameters:
# JLINK_UPLOAD_INTERFACE - Interface to use with J-Link.  Should be "JTAG" or "SWD".
# JLINK_CLOCK_SPEED - Speed to run the J-Link at, in KHz.
# JLINK_CPU_NAME - Name of CPU to pass to J-Link
#   This method has the following options:
# JLINK_USB_SERIAL_NUMBER - Use a J-Link connected over USB with the specified serial number.
# JLINK_NETWORK_ADDRESS - Use a J-Link connected over the network with the given <ip addr>[:port]
# JLINK_NO_GUI - If set to true, suppress GUI dialog boxes from the J-Link software.
#

set(UPLOAD_SUPPORTS_DEBUG TRUE)

### Check if upload method can be enabled on this machine
find_package(JLINK)
set(UPLOAD_JLINK_FOUND ${JLINK_FOUND})

### Setup options
set(JLINK_USB_SERIAL_NUMBER "" CACHE STRING "Use a J-Link connected over USB with the specified serial number.")
set(JLINK_NETWORK_ADDRESS "" CACHE STRING "Use a J-Link connected over the network with the given <ip addr>[:port]")

# Figure out -select option.  See here: https://wiki.segger.com/J-Link_GDB_Server#-select
if((NOT "${JLINK_USB_SERIAL_NUMBER}" STREQUAL "") AND (NOT "${JLINK_NETWORK_ADDRESS}" STREQUAL ""))
	message(FATAL_ERROR "Cannot use both JLINK_USB_SERIAL_NUMBER and JLINK_NETWORK_ADDRESS at the same time!")
elseif(NOT "${JLINK_USB_SERIAL_NUMBER}" STREQUAL "")
	set(JLINK_SELECT_ARG -Select usb=${JLINK_USB_SERIAL_NUMBER} CACHE INTERNAL "" FORCE)
elseif(NOT "${JLINK_NETWORK_ADDRESS}" STREQUAL "")
	set(JLINK_SELECT_ARG -Select ip=${JLINK_NETWORK_ADDRESS} CACHE INTERNAL "" FORCE)
else()
	# use default behavior
	set(JLINK_SELECT_ARG "" CACHE INTERNAL "" FORCE)
endif()

# default to JTAG
if(NOT DEFINED JLINK_UPLOAD_INTERFACE)
	set(JLINK_UPLOAD_INTERFACE JTAG CACHE INTERNAL "" FORCE)
endif()

option(JLINK_NO_GUI "If true, suppress GUI dialog boxes from the J-Link software.  Note: does not suppress license dialogs from J-Link EDU and On-Board probes, these are intentionally impossible to disable." FALSE)
if(JLINK_NO_GUI)
	set(JLINK_NOGUI_ARG -Nogui CACHE INTERNAL "" FORCE)
else()
	set(JLINK_NOGUI_ARG "" CACHE INTERNAL "" FORCE)
endif()

### Function to generate upload target
function(gen_upload_target TARGET_NAME BIN_FILE HEX_FILE)

	# create command file for j-link
	set(COMMAND_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/flash-${TARGET_NAME}.jlink)
	file(GENERATE OUTPUT ${COMMAND_FILE_PATH} CONTENT
"loadfile ${HEX_FILE}
r
exit
")
	add_custom_target(flash-${TARGET_NAME}
		COMMENT "Flashing ${TARGET_NAME} with J-Link..."
		COMMAND ${JLINK}
		${JLINK_SELECT_ARG}
		${JLINK_NOGUI_ARG}
		-Device \"${JLINK_CPU_NAME}\"
		-Speed ${JLINK_CLOCK_SPEED}
		-if ${JLINK_UPLOAD_INTERFACE}
		-JTAGConf -1,-1
		-AutoConnect 1
		-ExitOnError
		-CommandFile ${COMMAND_FILE_PATH})


	add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

endfunction(gen_upload_target)

### Commands to run the debug server.
# Note: Command-line options for the GDB server are documented on the wiki here:
# https://wiki.segger.com/J-Link_GDB_Server:#Command_line_options
set(UPLOAD_GDBSERVER_DEBUG_COMMAND
	"${JLINK_GDBSERVER}"
	${JLINK_SELECT_ARG}
	${JLINK_NOGUI_ARG}
	-Device \"${JLINK_CPU_NAME}\"
	-Speed ${JLINK_CLOCK_SPEED}
	-endian little
	-if ${JLINK_UPLOAD_INTERFACE}
	-JTAGConf -1,-1
	-LocalhostOnly
	-noIR
	-port ${GDB_PORT}
	-singlerun # Terminate GDB server after GDB disconnects
	)

# Reference: https://github.com/Marus/cortex-debug/blob/056c03f01e008828e6527c571ef5c9adaf64083f/src/jlink.ts#L42
set(UPLOAD_LAUNCH_COMMANDS
	"monitor halt"
	"monitor reset"
	"load"
	"break main"
	"monitor reset"
)
set(UPLOAD_RESTART_COMMANDS
	"monitor halt"
	"monitor reset"
)