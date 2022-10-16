### Arduino Bossac upload method
### This method can be used for boards with Arduino bootloaders that talk to the bossac flash program.
# This method creates the following options:
# ARDUINO_BOSSAC_SERIAL_PORT - Serial port to talk to the device bootloader on, e.g. "COM7"

set(UPLOAD_SUPPORTS_DEBUG FALSE)

### Check if upload method can be enabled on this machine
find_package(ArduinoBossac)
set(UPLOAD_ARDUINO_BOSSAC_FOUND ${ArduinoBossac_FOUND})

### Set up options
set(ARDUINO_BOSSAC_SERIAL_PORT "" CACHE STRING "Serial port to talk to the device bootloader on, e.g. 'COM7'")
if("${ARDUINO_BOSSAC_SERIAL_PORT}" STREQUAL "")
	message(WARNING "Please set ARDUINO_BOSSAC_SERIAL_PORT to the serial port to communicate with the target bootloader on.  Until this is set, flashing will not work.")
endif()

### Function to generate upload target
function(gen_upload_target TARGET_NAME BIN_FILE)

	add_custom_target(flash-${TARGET_NAME}
		COMMAND ${ArduinoBossac}
		--debug
		--port=${ARDUINO_BOSSAC_SERIAL_PORT}
		--usb-port=1
		--info
		--erase
		--write ${BIN_FILE}
		--reset)

	add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

endfunction(gen_upload_target)
