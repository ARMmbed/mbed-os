## Unit testing

This document describes how to write and test unit tests for Arm Mbed OS. To prevent and solve problems, please see the [troubleshooting](#troubleshooting) section.

### Introduction

Unit tests test code in small sections on a host machine. Unlike other testing tools, unit testing doesn't require embedded hardware, and it doesn't need to build the full operating system. Because of this, unit testing can result in faster tests than other testing tools. Unit testing takes place in a build environment where we test each C or C++ class or module in isolation. This means we build test suites into separate test binaries and stub all access outside to remove dependencies on any specific embedded hardware or software combination. This allows us to complete the testing using native compilers on the build machine.

### Prerequisites

Please install the following dependencies to use Mbed OS unit testing. 

- GNU toolchains.
   - GCC 6 or later. We recommend you use MinGW-W64 on Windows, but any Windows port of the above GCC versions works. Default compilers can be used on Mac OS instead of GCC to shorten build times, but code coverage results can then differ.
- CMake 3.0 or newer.
- Python 2.7.x, 3.5 or newer.
- Pip 10.0 or newer.
- Gcovr 4.1 or newer.
- Arm Mbed CLI 1.8.0 or newer.

Detailed instructions for supported operating systems are below.

#### Installing dependencies on Debian or Ubuntu

1. `sudo apt-get -y install build-essential cmake`
1. Install Python and Pip with:

   ```
   sudo apt-get -y install python python-setuptools
   sudo easy_install pip
   ```

1. Install Gcovr and [Mbed CLI](https://os.mbed.com/docs/latest/tools/developing-arm-mbed-cli.html) with `pip install "gcovr>=4.1" mbed-cli`.

#### Installing dependencies on macOS

1. Install [Homebrew](https://brew.sh/).
1. Install Xcode Command Line Tools with `xcode-select --install`.
1. Install CMake with: `brew install cmake`.
1. Install Python and Pip:

   ```
   brew install python
   sudo easy_install pip
   ```

1. Install Gcovr and [Mbed CLI](https://os.mbed.com/docs/latest/tools/developing-arm-mbed-cli.html) with `pip install "gcovr>=4.1" mbed-cli`.
1. (Optional) Install GCC with `brew install gcc`.

#### Installing dependencies on Windows

1. Download and install [MinGW-W64](http://mingw-w64.org/).
1. Download CMake binaries from https://cmake.org/download/, and run the installer.
1. Download Python 2.7 or Python 3 from https://www.python.org/getit/, and run the installer.
1. Add MinGW, CMake and Python into system PATH.
1. Install Gcovr and [Mbed CLI](https://os.mbed.com/docs/latest/tools/developing-arm-mbed-cli.html) with `pip install "gcovr>=4.1" mbed-cli`.

### Test code structure

Unit tests are located in the Mbed OS repository under the `UNITTESTS` folder. We recommend unit test files use an identical directory path to the file under test. This makes it easier to find unit tests for a particular class or a module. For example, if the file under test is `some/example/path/ClassName.cpp`, then all the test files are in the `UNITTESTS/some/example/path/ClassName` directory. Each test suite needs to have its own `unittest.cmake` file for test configuration.

#### Test discovery

Registering unit tests for running is automatic, and the test runner handles registration. However, test files are not automatically assigned to be built. We build unit tests by using a separate build system, which searches for unit tests under the `UNITTESTS` directory.

For the build system to find and build any test suite automatically, you must include a unit test configuration file named `unittest.cmake` for each unit test suite. This configuration file contains all the source files required for the build.

#### Test names

The build system automatically generates names of test suites. The name is constructed by taking a relative file path from the UNITTESTS directory to the test directory and replacing path separators with dashes. For example, the test suite name for `some/example/path/ClassName.cpp` is `some-example-path-ClassName`. Suite names are used when deciding which test suites to run.

### Unit testing with Mbed CLI

Mbed CLI supports unit tests through `mbed test --unittests` command. For information on using Mbed CLI, please see the [CLI documentation](https://os.mbed.com/docs/latest/tools/developing-arm-mbed-cli.html).

### Writing unit tests

Create two files in the test directory for each test suite:

- Unit test source file (`test_ClassName.cpp`).
- Unit test configuration file (`unittest.cmake`).

List all the files required for the build in the `unittest.cmake` file. We recommend you list the file paths relative to the `UNITTESTS` folder. Use the following variables to list the source files and include paths:

- **unittest-includes** - List of header include paths. You can use this to extend or overwrite default paths listed in CMakeLists.txt.
- **unittest-sources** - List of files under test.
- **unittest-test-sources** - List of test sources and stubs.

With the following steps, you can write a simple unit test. In this example, `rtos/Semaphore.cpp` is a class under test.

1. Create a directory for unit test files in `UNITTESTS/rtos/Semaphore`.
1. Create a test configuration file `UNITTESTS/rtos/Semaphore/unittest.cmake` with the following content:

   ```
   set(unittest-sources
       ../rtos/Semaphore.cpp
   )
   
   set(unittest-test-sources
       stubs/mbed_assert.c
       rtos/Semaphore/test_Semaphore.cpp
   )
   ```

1. Create a test source file `UNITTESTS/rtos/Semaphore/test_Semaphore.cpp` with the following content:

```
#include  "gtest/gtest.h"
#include  "rtos/Semaphore.h"

static osStatus_t retval = osOK;
static uint32_t count = 0;

// Test stubs
osStatus_t osSemaphoreAcquire(osSemaphoreId_t semaphore_id, uint32_t timeout)
{
    return retval;
}
osStatus_t osSemaphoreDelete(osSemaphoreId_t semaphore_id)
{
    return retval;
}
osStatus_t osSemaphoreRelease(osSemaphoreId_t semaphore_id)
{
    return retval;
}
uint32_t osSemaphoreGetCount(osSemaphoreId_t semaphore_id)
{
    return count;
}
osSemaphoreId_t osSemaphoreNew(uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr)
{
    return (void *)&count; // Just a dymmy reference
}

class  TestSemaphore : public  testing::Test {
protected:
    rtos::Semaphore *sem;

    virtual  void  SetUp()
    {
        sem = new rtos::Semaphore();
    }

    virtual  void  TearDown()
    {
        delete sem;
    }
};

TEST_F(TestSemaphore, constructor)
{
    EXPECT_TRUE(sem);
}
```

### Building and running unit tests

Use Mbed CLI to build and run unit tests. For advanced use, you can run CMake and a Make program directly.

#### Build tests directly with CMake

1. Create a build directory `mkdir UNITTESTS/build`.
1. Move to the build directory `cd UNITTESTS/build`.
1. Run CMake using a relative path to `UNITTESTS` folder as the argument. So from `UNITTESTS/build` use `cmake ..`:
   - Add `-g [generator]` if generating other than Unix Makefiles such in case of MinGW use `-g "MinGW Makefiles"`.
   - Add `-DCMAKE_MAKE_PROGRAM=<value>`, `-DCMAKE_CXX_COMPILER=<value>` and `-DCMAKE_C_COMPILER=<value>` to use a specific Make program and compilers.
   - Add `-DCMAKE_BUILD_TYPE=Debug` to build a debug build.
   - Add `-DCOVERAGE=True` to add coverage compiler flags.
   - See the [CMake manual](https://cmake.org/cmake/help/v3.0/manual/cmake.1.html) for more information.
1. Run a Make program to build the tests.

#### Run tests directly with CTest

Run a test binary in the build directory to run a unit test suite. To run multiple test suites at once, use CTest test runner. Run CTest with `ctest`. Add `-v` to get results for each test case. See the [CTest manual](https://cmake.org/cmake/help/v3.0/manual/ctest.1.html) for more information.

#### Run tests with GUI test runner

1. Install *gtest-runner* using the [documentation](https://github.com/nholthaus/gtest-runner).
1. Run *gtest-runner*
1. Add test executables into the list.
1. Run them.

### Debugging

1. Use Mbed CLI to build a debug build. For advanced use, run CMake directly with `-DCMAKE_BUILD_TYPE=Debug`, and then run a Make program.
1. Run GDB with a test executable as an argument to debug unit tests.

### Get code coverage

Use Mbed CLI to generate code coverage reports. For advanced use, follow these steps:

1. Run CMake with both `-DCMAKE_BUILD_TYPE=Debug` and `-DCOVERAGE=True`.
1. Run a Make program to build the tests.
1. Run the tests.
1. Run Gcovr or any other code coverage tool directly in the build directory.

### Troubleshooting

**Problem:** Generic problems with CMake or with the build process.
* **Solution**: Delete the build directory. Make sure that CMake, g++, GCC and a Make program can be found in the path and are correct versions.

**Problem:** Virus protection identifies files generated by CMake as malicious and quarantines the files on Windows.
* **Solution**: Restore the false positive files from the quarantine.

**Problem:** CMake compiler check fails on Mac OS Mojave when using GCC-8.
* **Solution**: Make sure gnm (binutils) is not installed. Uninstall binutils with `brew uninstall binutils`.
