# Unit testing Mbed OS

This document describes how to run and write unit tests for Mbed OS.

## Prerequisites

* GNU toolchains installed.
    * GCC 6 or later
    * MinGW-W64 GCC-6.4.0 or MinGW-W64 GCC-7.3.0 (Windows)
* CMake 3.0+ installed.
* Python 2.7.x or >3.5 and pip 10.0 (or newer) installed.
* gcovr >=4.1

### Installing dependencies on Debian/Ubuntu

1. `sudo apt-get -y install build-essential cmake`
2. Install python and pip:
```
sudo apt-get -y install python python-setuptools
sudo easy_install pip
```
3. Install [gcovr](#installing-covr).
4. (Optional) Install [Mbed CLI](https://os.mbed.com/docs/latest/tools/arm-mbed-cli.html).

### Installing dependencies on Mac OS

1. Install [Homebrew](https://brew.sh/).
2. Install gcc compilers and cmake with: `brew install gcc cmake`
3. Install python and pip:
```
brew install python
sudo easy_install pip
```
3. Install [gcovr](#installing-covr).
4. (Optional) Install [Mbed CLI](https://os.mbed.com/docs/latest/tools/arm-mbed-cli.html).

### Installing dependencies on Windows

1. Download and install [MinGW-W64](http://mingw-w64.org/).
2. Download CMake binaries from https://cmake.org/download/ and run the installer.
3. Download Python2.7 or Python3 from https://www.python.org/getit/ and run the installer.
4. Add MinGW, CMake and Python into PATH.
5. Install [gcovr](#installing-covr).
6. (Optional) Install [Mbed CLI](https://os.mbed.com/docs/latest/tools/arm-mbed-cli.html).

### Installing covr

Install gcovr code coverage tool globally with `pip install "gcovr>=4.1"` or using virtualenv:

#### virtualenv

1. Install virtualenv if not installed with `pip install virtualenv`
2. Install gcovr with:

**[Debian/Linux/Mac OS]**
```
virtualenv pyenv
. pyenv/bin/activate
pip install 'gcovr>=4.1'
```
**[Windows]**
```
virtualenv pyenv
pyenv\\Scripts\\activate
pip install "gcovr>=4.1"
```

## Building and running unit tests

> In case of running into problems see [troubleshooting](#troubleshooting) section.

`UNITTESTS/mbed_unittest.py` contains testing scripts for Mbed OS unit testing. Mbed CLI supports unit testing through `mbed test --unittests` command with the same arguments.

### Testing with Mbed CLI

```
mbed test --unittests
```

A subset of tests can be run by providing `-r` flag for the tool which runs tests matching a regular expression.

e.g. `mbed test --unittests --run -r features-netsocket`

### Build manually without Python tools

1. Create a build directory e.g. `mkdir UNITTESTS/build`.
2. Move to the build directory `cd UNITTESTS/build`.
3. Run CMake with `cmake [RELATIVE PATH TO UNITTESTS DIR] [OPTIONAL ARGUMENTS]` e.g. `cmake ..`:
	* Add `-g [generator]` argument if target other than Unix Makefiles e.g. MinGW `-g "MinGW Makefiles"`
4. Run make program (make, gmake, mingw32-make, etc).

##### Run CMake and build Unix Makefiles (GNU make)

```
cmake ..
make
```

##### Run CMake and build MinGW Makefiles (mingw32-make)

```
cmake -G "MinGW Makefiles" ..
mingw32-make
```

#### Custom CMake variables

Usage:
`cmake [RELATIVE PATH TO UNITTESTS DIR] [OPTIONS]`

Keyword variables (usage `cmake -D<VARIABLE>(:<TYPE>)=<value>`:

| Variable | Type | Accepted values | Description |
| -------- | ---- | --------------- | ----------- |
| COVERAGE | STRING | merged<br>separate | Generate merged or individual reports |

### Run in terminal

Unit tests can be run separately from each executable or by using ctest test runner. Run ctest with make program using target test. Options can be passed to ctest using ARGS argument. See [ctest manual](https://cmake.org/cmake/help/v3.0/manual/ctest.1.html) for more information.

Run ctest on test suite level:
```
{MAKE_PROGRAM} test -C [RELATIVE PATH TO BUILD DIRECTORY]
```
e.g. `make test -C UNITTESTS/build` or `mingw32-make test -C UNITTESTS/build`

Run ctest verbose (show each test case):
```
{MAKE_PROGRAM} test -C UNITTESTS/build ARGS="-V"
```

Run ctest dashboard test and create test results:
```
{MAKE_PROGRAM} test --C UNITTESTS/build ARGS="-D ExperimentalTest"
```

### Run with GUI test runner

1. Build and/or install *gtest-runner* using the documentation: https://github.com/nholthaus/gtest-runner

2. Run the application, add built test executables into the list and run it.

### Get code coverage

Python tools use gcovr to build code coverage reports. Generate html report `UNITTESTS/build/coverage/index.html` with:
```
mbed test --unittests --coverage html
```

To get coverage for a single test suite, run gcovr separately for suite coverage data directory. See [gcovr documentation](https://gcovr.com/guide.html#filter-options) for more information.

e.g. for features/netsocket/InternetSocket coverage:

Debian/Ubuntu/Mac OS:
```
mkdir UNITTESTS/build
cd UNITTESTS/build
cmake -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE:STRING=html  ..
make
./features-netsocket-InternetSocket
gcovr -r ../.. --html --html-detail -o ./index.html ./CMakeFiles/features-netsocket-InternetSocket.MbedOS.dir/
```
Windows:
```
mkdir UNITTESTS/build
cd UNITTESTS/build
cmake -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE:STRING=html -g "MinGW Makefiles" ..
mingw32-make
features-netsocket-InternetSocket.exe
gcovr -r ..\.. --html --html-detail -o .\index.html .\CMakeFiles\features-netsocket-InternetSocket.MbedOS.dir\
```

## The structure of unit tests

The structure of the unit tests directory looks like this:
```
UNITTESTS
  ├── mbed_unittest.py                       Python tool for unit testing
  ├── unit_test                              Python tool modules
  ├── CMakeLists.txt                         CMake project definition file
  ├── CMakeSettings.json                     CMake configurations for Visual Studio 2017
  ├── README.md
  ├── googletest-CMakeLists.txt.in           CMake project definition file for Google Test
  │
  ├── features
  │   └── netsocket                          Directory tree that mirrors Mbed OS root
  │       ├── NetworkInterface               Name of the class to be tested
  │       │   ├── test_NetworkInterface.cpp
  │       │   └── unittest.cmake             CMake module for unit test
  │       └── Socket
  │
  ├── stubs                                  Shared stubs which can be used for tests.
  ├── target_h                               Shared headers which can be used for tests.
  └── template                               Templates for creating new unittests
```
Each unit test has an identical directory tree as seen in the Mbed OS root folder. This is not a mandatory requirement but helps to maintain tests. Each class to be tested have their own `unittest.cmake` which is found by `CMakeLists.txt`.

## Creating a unit test

Each class to be tested requires two files for unit testing:
1. C++ unit test source file (e.g. `test_NetworkInterface.cpp`)
2. CMake module file for unit test definition (`unittest.cmake`)

A unit test definition file `unittest.cmake` requires variables to be set for a test to be configured. File source paths in `unittest.cmake` files need to be relative to the unit test folder and `CMakeLists.txt`.

* **TEST_SUITE_NAME** - Identifier for the test suite. Use naming convention *PATH_TO_THE_TESTABLE_FILE* e.g. *features-netsocket-InternetSocket*
* **unittest-includes** - Include paths for headers needed to build the tests in addition to the base include paths listed in [CMakeLists.txt](CMakeLists.txt). Optional.
* **unittest-sources** - Mbed OS source files and stubs included for the build.
* **unittest-test-sources** - Unit test source files.

#### Creating unit tests files with Mbed CLI

```
mbed test --unittests --new <FILEPATH>
```

E.g. `mbed test --unittests --new rtos/Semaphore.cpp`

The generator script only creates the files required for a unit test. It does not write unit tests automatically nor does it handle source dependencies.

#### Create files manually

For example to create a unit test for `rtos/Semaphore.cpp`:

1. Create a directory for unit test files in `UNITTESTS/rtos/Semaphore`.
2. Create a test definition file `UNITTESTS/rtos/Semaphore/unittest.cmake` with the following content:
```
set(TEST_SUITE_NAME "rtos-Semaphore")

set(unittest-sources
	stubs/mbed_assert.c
	../rtos/Semaphore.cpp
)

set(unittest-test-sources
	rtos/Semaphore/test_Semaphore.cpp
)
```
3. Create a test source file `UNITTESTS/rtos/Semaphore/test_Semaphore.cpp` with the following content:
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

## Troubleshooting

**Problem:** virus protection identifies files generated by CMake as malicious and quarantines the files on Windows.

* **Solution**: restore the false positive files from the quarantine.
