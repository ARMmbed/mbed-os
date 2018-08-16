# Unit testing Mbed OS

This document describes the motivation for unit testing and how to run and write unit tests for Mbed OS.

## Introduction
Traditional software testing is defined into three main levels: unit testing, integration testing and system testing. These levels are often pictured as a pyramid to indicate the amount of testing per level.
```
^ Testing level
|
|       /\
|      /  \       System testing
|     /----\
|    /      \     Integration testing
|   /--------\
|  /          \   Unit testing
| /------------\
|
*-------------------> Amount of tests
```
Integration and system testing are performed in an environment with full Mbed OS included in the build.

Unit testing takes place in a build environment where each C/C++ class or module is tested in isolation. This means test suites are built into separate test binaries and all access outside is stubbed to remove dependency of any specific hardware or software combination. This allows the testing to be done quickly using native compilers on the build machine.

## Required tools

* GNU toolchains installed.
    * GCC 6 or later
    * MinGW-W64 GCC-6.4.0 or MinGW-W64 GCC-7.3.0 (Windows)
* CMake 3.0+ installed.
* Python and pip installed.
* gcovr and optionally virtualenv pip modules installed.

### Installing dependencies on Debian/Ubuntu
1. `sudo apt-get -y install build-essential cmake`
2. Install python and pip:
```
sudo apt-get -y install python python-setuptools
sudo easy_install pip
```
3. Install [gcovr](#installing-covr).

### Installing dependencies on Mac OS
1. Install [Homebrew](https://brew.sh/).
2. Install gcc compilers and cmake with: `brew install gcc cmake`
3. Install python and pip:
```
brew install python
sudo easy_install pip
```
3. Install [gcovr](#installing-covr).

### Installing dependencies on Windows
1. Download and install [MinGW-W64](http://mingw-w64.org/).
2. Download CMake binaries from https://cmake.org/download/ and run the installer.
3. Download Python2.7 or Python3 from https://www.python.org/getit/ and run the installer.
4. Add MinGW, CMake and Python into PATH if not yet.
	a. Open Start Menu > search Environment variables > Environment Variables...
	b. Edit 'Path' and add the directory paths e.g. (C:\mingw64\bin, C:\Program Files\CMake\bin, etc.).
5. Install [gcovr](#installing-covr).

### Installing covr

Install gcovr code coverage tool with virtualenv or globally:

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
pip install 'gcovr>=4.1'
```

#### globally
`pip install 'gcovr>=4.1'`

## Building and running unit tests

> In case of running into problems see [troubleshooting](#troubleshooting) section.

Start at the root directory of Mbed OS.

### Build and run with Python script:
```
./UNITTESTS/mbed_unittest.py
```

##### Parameters to mbed_unittest.py
```
usage: mbed_unittest.py [-h] [--clean] [-d]
                        [--coverage {html,xml,html-merged,xml-merged,both,both-merged}]
                        [-m {gmake,make,mingw32-make,ninja}]
                        [-g {Unix Makefiles,MinGW Makefiles,Ninja}]
                        [-r TEST_REGEX] [--build-path BUILD_PATH]
                        [-v | --quiet]
                        [{build,run}]
positional arguments:
  {build,run}           only build or only run tests

optional arguments:
  -h, --help            show this help message and exit
  --clean               clean previous build data
  -d, --debug           enable debug build
  --coverage {html,xml,html-merged,xml-merged,both,both-merged}
                        generate code coverage report(s)
  -m {gmake,make,mingw32-make,ninja}, --make-program {gmake,make,mingw32-make,ninja}
                        which make program to use
  -g {Unix Makefiles,MinGW Makefiles,Ninja}, --generator {Unix Makefiles,MinGW Makefiles,Ninja}
                        which CMake generator to use
  -r TEST_REGEX, --regex TEST_REGEX
                        run tests matching regular expression
  --build-path BUILD_PATH
                        specify relative build path
  -v, --verbose         verbose output
  --quiet               quiet output
```

A subset of tests can be run by providing `-r` flag for the tool which runs tests matching a regular expression.

e.g. `./UNITTESTS/mbed_unittest.py run -r features_netsocket`

### Build manually on command line:
1. Create a build directory e.g. `mkdir UNITTESTS/build`.
2. Move to the build directory `cd UNITTESTS/build`.
3. Run CMake with `cmake [RELATIVE PATH TO UNITTESTS DIR] [OPTIONAL ARGUMENTS]` e.g. `cmake ..`:
	* Add `-g [generator]` argument if target other than Unix Makefiles e.g. MinGW `-g "MinGW Makefiles"`
4. Run make program (make, gmake, mingw32-make, etc).

##### Run CMake and build Unix Makefiles (GNU make): 
```
cmake ..
make
```

##### Run CMake and build MinGW Makefiles (mingw32-make):
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
| COVERAGE_FTYPE | STRING | html<br>xml<br>both | Generate HTML, XML or both reports |
| GCOV_PROGRAM | | N/A | Name of the gcov tool to use |

### Run in terminal:
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

### Run with GUI test runner:

1. Build and/or install *gtest-runner* using the documentation: https://github.com/nholthaus/gtest-runner

2. Run the application, add built test executables into the list and run it.

### Get code coverage
```
./UNITTESTS/mbed_unittest.py --coverage html-merged
```
Open ./UNITTESTS/build/coverage_merged/index.html

To get coverage for a single test suite, use separate coverage reports and build manually using *TEST_SUITE_NAME*_coverage for target name.

e.g. for features/netsocket/InternetSocket coverage:

Debian/Ubuntu/Mac OS:
```
mkdir UNITTESTS/build
cd UNITTESTS/build
cmake -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE_FTYPE:STRING=html -DCOVERAGE:STRING=separate ..
make
make features_netsocket_InternetSocket_coverage
```
Windows:
```
mkdir UNITTESTS/build
cd UNITTESTS/build
cmake -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE_FTYPE:STRING=html -DCOVERAGE:STRING=separate -g "MinGW Makefiles" ..
mingw32-make
mingw32-make features_netsocket_InternetSocket_coverage
```

## The structure of unit tests
The structure of the unit tests directory looks like this:
```
UNITTESTS
  ├── mbed_unittest.py                       Script for building and running unit tests
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
  ├── stubs                                  Stubs used in tests.
  ├── target_h                               Stub headers used in tests.
  ├── Modules                                CMake modules
  └── template                               Templates for quickly generating unittest tree
```
Each unit test has an identical directory tree as seen in the Mbed OS root folder. This is not a mandatory requirement but helps to maintain tests. Each class to be tested have their own `unittest.cmake` which is found by `CMakeLists.txt`.

## Creating a unit test

Each class to be tested requires two files for unit testing:
1. C++ unit test source file (e.g. test_NetworkInterface.cpp)
2. CMake module file for unit test suite (unittest.cmake)

#### Create files manually
1. Create a directory for unit test files in `UNITTESTS/rtos/Semaphore`.
2. Create a CMake module file `UNITTESTS/rtos/Semaphore/unittest.cmake` with the following content:
```
set(TEST_SUITE_NAME "rtos_Semaphore")

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
osStatus_t osSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout) { return retval; }
osStatus_t osSemaphoreDelete (osSemaphoreId_t semaphore_id)                    { return retval; }
osStatus_t osSemaphoreRelease (osSemaphoreId_t semaphore_id)                   { return retval; }
uint32_t osSemaphoreGetCount (osSemaphoreId_t semaphore_id)                    { return count; }
osSemaphoreId_t osSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr)
{
    return (void*)&count; // Just a dymmy reference
}

class  TestSemaphore : public  testing::Test
{
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

#### Unit test suite CMake variables

A suite definition file *unittest.cmake* requires variables to be set for a test to be configured.

* **TEST_SUITE_NAME** - Identifier for the test suite. Use naming convention *PATH_TO_THE_TESTABLE_FILE* e.g. *features_netsocket_InternetSocket*
* **unittest-includes** - Include paths for headers needed to build the tests in addition to the base include paths listed in [CMakeLists.txt](CMakeLists.txt). Optional.
* **unittest-sources** - Mbed OS source files and stubs included for the build.
* **unittest-test-sources** - Unit test source files.

## Troubleshooting

**Problem:** virus protection identifies files generated by CMake as malicious and quarantines the files on Windows.

* **Solution**: restore the false positive files from the quarantine.