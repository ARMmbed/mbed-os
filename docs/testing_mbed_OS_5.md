# Testing in mbed OS 5

The way tests are ran and compiled in mbed OS 5 is substantially different than in previous versions of mbed. Previously tests were located in one known location and a python file was kept (`tools/tests.py`) keeping track of their dependencies, capabilities, and configurations. mbed OS 5 has adopted a more distributed approach to testing. Now test code lives alongside the test code, and these are dynamically discovered by the test tools.

## Table of Contents

- [Using tests](#using-tests)
  - [Test code structure](#test-code-structure)
    - [Test discovery](#test-discovery)
    - [Test names](#test-names)
  - [Building tests](#building-tests)
  - [Running tests](#running-tests)
  - [Writing tests](#writing-tests)
- [Debugging tests](#debugging-tests)
- [Known issues](#known-issues)

## Test code structure

Tests can exist throughout mbed OS and your project's code. They are located under a special directory called `TESTS` (case is important!).

Placing code under this directory means it will be ignored when building applications and libraries. This code is only ever used when building tests. This is important since all tests require a `main()` function, and building it with your application would cause multiple `main()` functions to be defined.

In addition to being placed under a `TESTS` directory, test sources must exist under two other directories: a "test group" folder and a "test case" folder. The following is an example of this structure:
```
myproject/TESTS/test_group/test_case_1
```

In this example, `myproject` is the project root and all the source files under the `test_case_1` directory will be included in the test. Any other source files from the OS, libraries, and your project that apply to your target's configuration will also be included in the build of your test.

### Test discovery

Since test cases can exist throughout a project, the tools must find them in your project's file structure before building them. This is done by searching for paths that match the pattern detailed above in the [Test code structure](#test-code-structure) section.

Test discovery also obeys the same rules that is used when building your project. This means that tests that are placed under a folder with a prefix like `TARGET_`, `TOOLCHAIN_`, or `FEATURE_` will only be discovered, built, and ran if your current configuration matches this prefix.

For example, if a test was placed under the folder `FEATURE_BLE` with the following path:

```
myproject/mbed-os/features/FEATURE_BLE/TESTS/ble_tests/unit_test
```

This test case would only be discovered if the target that was being testing supported the BLE feature. Otherwise, the test will be ignored.

Generally, a test should not be placed under a `TARGET_` or `TOOLCHAIN_` folder, since most tests should be designed to work for all target and toolchain configurations.

Tests can also be completely ignored by using the `.mbedignore` file descirbed [here]()

### Test names

A test case is named from its position in your project's file structure. For instance, in the above example, a test case with the path `myproject/TESTS/test_group/test_case_1` would be named `tests-test_group-test_case_1`. You will notice that the name is created by joining the directories that make up the path to the test case with a "dash" (`-`) character. This will be a unique name to identify the test case. You will see this name used throughout the build and testing process.

## Building tests

Tests can be built easily through mbed CLI. For information on using mbed CLI, please see its documentation.

When tests are built for a target and a given toolchain, the available tests are first discovered, then built in series. You can also create a "test specification" file, which can be used by our testing tools to run automated hardware tests. For more information on the test specification file, please see the docmentation [here](https://github.com/ARMmbed/greentea#test-specification-json-formatted-input).

### Building process

The process for building tests is handled by the `test.py` script (not to be confused with `tests.py`) located under the `tools` directory. This handles the discovery and building of all tests cases for a given target and toolchain.

The actual process that occurs is as follows:

1. Build the "shared" code (all non test code), but do not link it
  - The resulting object files are placed in the build directory
1. Find all tests that pertain to the given target and toolchain
1. For each discovered test, build all of its source files and link it with the "shared" code that was built in step 1
1. If specified, create a test specification file and place it in the given directory for use by testing tools
  - This is placed in the build directory by default when using mbed CLI

### Running tests

Automated tests can be ran easily through mbed CLI. For information on using mbed CLI, please see its documentation.

The testing process requires that the tests are built and that a test specification JSON file exists that describes these available tests. The test specification format is detailed [here](https://github.com/ARMmbed/greentea#test-specification-json-formatted-input).

The actual testing process is handled by the Greentea tool. To read more about this tool, please visit its [GitHub repository](https://github.com/ARMmbed/greentea).

### Writing tests

You can write your own tests for your project, or add more tests to mbed OS. Tests are written using the [Greentea client](https://github.com/ARMmbed/mbed-os/tree/master/features/frameworks/greentea-client), [UNITY](https://github.com/ARMmbed/mbed-os/tree/master/features/frameworks/unity), and [utest](https://github.com/ARMmbed/mbed-os/tree/master/features/frameworks/utest) frameworks, located in `/features/frameworks`. Below is an example test that uses all of these frameworks:

```c++
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"

using namespace utest::v1;

// A test that returns successfully is considered successful
void test_success() {
    TEST_ASSERT(true);
}

// Tests that assert are considered failing
void test_failure() {
    TEST_ASSERT(false);
}

utest::v1::status_t test_setup(const size_t number_of_cases) {
    // Setup Greentea using a reasonable timeout in seconds
    GREENTEA_SETUP(40, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

// Test cases
Case cases[] = {
    Case("Testing success test", test_success),
    Case("Testing failure test", test_failure),
};

Specification specification(test_setup, cases);

// Entry point into the tests
int main() {
    return !Harness::run(specification);
}
```

This test will first run a case that succeeds, then it will run a test that fails. This is a good template to use when creating tests. For more complex testing examples, please see the documentation for [utest](https://github.com/ARMmbed/mbed-os/tree/master/features/frameworks/utest).

## Debugging tests

Debugging tests is a crucial part of the development and porting process. This section will cover exporting the test, then driving the test with the test tools while the target is attached to a debugger.

### Exporting tests

Currently, the easiest way to export a test is to copy the test's source code from its test folder to your project's root. This way it will be treated like a normal application by the tools.

You can find the path to the test you wish to export by running the following command:

```
mbed test --compile-list -n <test name>
```

Once you've copied all of the test's source files to your project root, go ahead and export your project:

```
mbed export -i <ide name>
```

Your exported project should now be in `projectfiles/<ide>_<target>`. Go ahead and open this project in your IDE.

### Running a test while debugging

Assuming your test has exported correctly to your IDE, go ahead and build the project and load it onto your target via your debugger.

Bring the target out of reset and run the program. Your target will now be waiting for a synchronizing character string to be sent from the test tools over the serial port. The `mbed test` command cannot be run as that will attempt to flash the device, which you've already done with your IDE.

Instead, the underlying test tools can be used to drive the test. [htrun](https://github.com/ARMmbed/htrun) is the tool that needs to be used in this case. This is installed when you install the requirements for mbed OS. However, if you don't have it installed you can do this by running `pip install mbed-host-tests`.

First, find your target's serial port by running the following command:

```
$ mbed detect

[mbed] Detected KL46Z, port COM270, mounted D:

...
```

From the output, take note of where your target's serial port (in this case, it's `COM270`).

Run the following command when your device is running the test in your debugger:

```
mbedhtrun --skip-flashing --skip-reset -p <serial port>:9600
```

Replace `<serial port>` with the serial port you found by running `mbed detect` above.

So for the example above, the command would be:

```
mbedhtrun --skip-flashing --skip-reset -p COM270:9600
```

This will detect your attached target and drive the test. At this point the test should proceed and you should be able to debug it. If you need to rerun the test, simply flash and/or reset the device with your debugger, run the program, and run the same command.

For an explanation of the arguments used in this command, please run `mbedhtrun --help`.

## Known issues

- There cannot be a `main()` function outside of a `TESTS` directory when building and running tests. This is because this function will be included in the "shared" build as described in the [Building process](#building-process) section. When the test code is compiled and linked with this "shared" build, a linker error will occur due to their being multiple `main()` functions defined. For this reason, please either rename your main application file if you need to build and run tests or use a different project.
  - **NOTE:** This does not affect building projects or applications, just building and running tests.