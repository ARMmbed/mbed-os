### Mbed USB Drive Upload Method
# This method needs no parameters.
# This method creates the following options:
# MBED_DRIVE_PATH - Path to mbed virtual USB drive to upload to

set(UPLOAD_MBED_FOUND TRUE) # this has no dependencies
set(UPLOAD_SUPPORTS_DEBUG FALSE)

set(MBED_DRIVE_PATH "" CACHE PATH "Path to mbed virtual USB drive to upload to")

### Function to generate upload target

# Can only access CMAKE_CURRENT_LIST_DIR outside function
set(UPLOAD_SCRIPT_PATH ${CMAKE_CURRENT_LIST_DIR}/install_bin_file.cmake)

function(gen_upload_target TARGET_NAME BIN_FILE)

	add_custom_target(flash-${TARGET_NAME}
		COMMAND ${CMAKE_COMMAND}
		-DBIN_FILE=${BIN_FILE}
		-DMBED_PATH=${MBED_DRIVE_PATH}
		-P ${UPLOAD_SCRIPT_PATH})

	add_dependencies(flash-${TARGET_NAME} ${TARGET_NAME})

endfunction(gen_upload_target)