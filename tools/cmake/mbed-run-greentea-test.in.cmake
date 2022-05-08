# This file is configured by CMake to create the script used to execute each Greentea test.

# First flash the target using its configured Mbed OS upload method
execute_process(
	# Note: we don't use cmake --build because that performs a reconfigure of the build system each time
	COMMAND @CMAKE_MAKE_PROGRAM@ flash-@MBED_GREENTEA_TEST_NAME@
	WORKING_DIRECTORY @CMAKE_BINARY_DIR@
	COMMAND_ERROR_IS_FATAL ANY)

# Then run and communicate with the test
execute_process(
	COMMAND mbedhtrun --skip-flashing -f @MBED_GREENTEA_TEST_IMAGE_NAME@ @MBED_HTRUN_ARGUMENTS@
	WORKING_DIRECTORY "@CMAKE_CURRENT_BINARY_DIR@"
	COMMAND_ERROR_IS_FATAL ANY)