##Example tests

This folder contains example tests for Icetea
The test located under this folder is dependent of the application [`exampleapp`](https://github.com/ARMmbed/mbed-os/blob/master/TEST_APPS/device/exampleapp)
The exampleapp is disabled by default, to be able to run the test_cmdline with the exampleapp, either remove the preprocessor macro from exampleapp.cpp or add `-DICETEA_EXAMPLE_ENABLED` to the mbed test command

## Test cases

### `test_cmdline`

**Description:**
Send command line commands to target over serial interface.
This test introduces the Icetea test case structure to the user.

**Test steps:**
Send "echo hello world" to the target.
Target sends "hello world" back to the host machine.
Send help to the target.
Target prints out the command line commands the application supports.

**Expected result:**
The test exits without timeouts.
