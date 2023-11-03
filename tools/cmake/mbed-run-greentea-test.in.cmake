# This file is configured by CMake to create the script used to execute each Greentea test.

# First flash the target using its configured Mbed OS upload method
# -----------------------------------------------------------------------
execute_process(
	# Note: we don't use cmake --build because that performs a reconfigure of the build system each time
	COMMAND @CMAKE_MAKE_PROGRAM@ flash-@MBED_GREENTEA_TEST_NAME@
	WORKING_DIRECTORY @CMAKE_BINARY_DIR@
	COMMAND_ERROR_IS_FATAL ANY)

# Then run and communicate with the test
# -----------------------------------------------------------------------
set(MBEDHTRUN_ARGS --skip-flashing @MBED_HTRUN_ARGUMENTS@) # filled in by configure script

# Print out command
string(REPLACE ";" " " MBEDHTRUN_ARGS_FOR_DISPLAY "${MBEDHTRUN_ARGS}")
message("Executing: @Python3_EXECUTABLE@ -m mbed_host_tests.mbedhtrun ${MBEDHTRUN_ARGS_FOR_DISPLAY}")

# Note: For this command, we need to survive mbedhtrun not being on the PATH, so we import the package and call the main function using "python -c"
execute_process(
	COMMAND @Python3_EXECUTABLE@ -m mbed_host_tests.mbedhtrun ${MBEDHTRUN_ARGS}
	WORKING_DIRECTORY "@mbed-os_SOURCE_DIR@/tools/python"
	COMMAND_ERROR_IS_FATAL ANY)