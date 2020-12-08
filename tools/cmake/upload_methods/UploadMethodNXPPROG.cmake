### NXPProg Upload Method
# This method needs the following parameters:
# NXPPROG_BAUD - Baudrate to talk to the bootloader over.
# NXPPROG_OSCFREQ - The chip's internal oscillator frequency, this needs to be sent to the bootloader for it to operate.
# This method creates the following options:
# NXPPROG_COM_PORT - Serial port that the NXP processor is accessible on.

### Check if upload method can be enabled on this machine

# NXPPROG requires pyserial
include(CheckPythonPackage)
check_python_package(serial HAVE_PYSERIAL)

set(UPLOAD_NXPPROG_FOUND ${HAVE_PYSERIAL})
set(UPLOAD_SUPPORTS_DEBUG FALSE)

### Set up options

set(NXPPROG_COM_PORT "" CACHE STRING "COM port for bootloader upload.  Should be \"COMXX\" on Windows, and /dev/ttyXX on Linux/Mac")
message(STATUS "COM port for bootloader upload: ${NXPPROG_COM_PORT}")

### Function to generate upload target

# Can only access CMAKE_CURRENT_LIST_DIR outside function
set(NXPPROG_PATH ${CMAKE_CURRENT_LIST_DIR}/nxpprog.py)

function(gen_upload_target TARGET_NAME BIN_FILE)

	if("${NXPPROG_COM_PORT}" STREQUAL "")
		add_custom_target(flash-${TARGET_NAME}
			COMMAND ${CMAKE_COMMAND}
			-E echo "ERROR: Cannot flash, no COM_PORT configured!")
	else()
		add_custom_target(flash-${TARGET_NAME}
			COMMENT "Flashing ${TARGET_NAME} through bootloader..."
			COMMAND ${Python3_EXECUTABLE} ${NXPPROG_PATH}
			--control
			--oscfreq=${NXPPROG_OSCFREQ}
			--baud=${NXPPROG_BAUD}
			${NXPPROG_COM_PORT}
			${BIN_FILE})
	endif()

	add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

endfunction(gen_upload_target)