## Module testing

This document describes how to write and use module tests for Arm Mbed OS. 

### Introduction

Module tests test code in small sections on a host machine. Unlike other testing tools, module testing doesn't require embedded hardware or need to build a full operating system. Because of this, module testing can result in faster tests than other tools. Module testing happens in a build environment where you test each C or C++ class or module in isolation. Build test suites into separate test binaries and stub all access outside to remove dependencies on any specific embedded hardware or software combination. This allows you to complete tests using native compilers on the build machine.

### Prerequisites

Please install the following dependencies to use Mbed OS module testing:

* GNU toolchains.
   * GCC 6 or later. We recommend you use MinGW-W64 on Windows, but any Windows port of the above GCC versions works. Default compilers can be used on Mac OS instead of GCC to shorten build times, but code coverage results can differ.
* CMake 3.0 or newer.
* Python 2.7.x, 3.5 or newer.
* Pip 10.0 or newer.
* Gcovr 4.1 or newer.
* Arm Mbed CLI 1.8.0 or newer.

Detailed instructions for supported operating systems are below.

#### Installing dependencies on Debian or Ubuntu

In a terminal window:

1. `sudo apt-get -y install build-essential cmake`
1. Install Python and Pip with:

   ```
   sudo apt-get -y install python python-setuptools
   sudo easy_install pip
   ```

1. Install Gcovr and [Mbed CLI](https://os.mbed.com/docs/mbed-os/latest/tools/developing-mbed-cli.html) with `pip install "gcovr>=4.1" mbed-cli`.

#### Installing dependencies on macOS

In a terminal window:

1. Install [Homebrew](https://brew.sh/).
1. Install Xcode Command Line Tools with `xcode-select --install`.
1. Install CMake with: `brew install cmake`.
1. Install Python and Pip:

   ```
   brew install python
   sudo easy_install pip
   ```

1. Install Gcovr and [Mbed CLI](https://os.mbed.com/docs/mbed-os/latest/tools/developing-mbed-cli.html) with `pip install "gcovr>=4.1" mbed-cli`.
1. (Optional) Install GCC with `brew install gcc`.

#### Installing dependencies on Windows

In a terminal window:

1. Download and install MinGW-W64 from [SourceForge](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/).
1. Download CMake binaries from https://cmake.org/download/, and run the installer.
1. Download Python 2.7 or Python 3 from https://www.python.org/getit/, and run the installer.
1. Add MinGW, CMake and Python into system PATH.
1. Install Gcovr and [Mbed CLI](https://os.mbed.com/docs/mbed-os/latest/tools/developing-mbed-cli.html) with `pip install "gcovr>=4.1" mbed-cli`.

### Test code structure

Find module tests in the Mbed OS repository under the `MODULETESTS` folder. We recommend module test files use an identical directory path as the files under test. This makes it easier to find module tests for a particular module. For example, if the files you're testing are in `some/example/path/`, then all the test files are in the `MODULETESTS/some/example/path/ModuleName` directory. Each test suite needs to have its own `moduletest.cmake` file for test configuration.

All the class stubs should be located in the `MODULETESTS/stubs` directory. A single stub class can be used by multiple test suites and should follow the naming convention `ClassName_stub.cpp` for the source file, and `ClassName_stub.h` for the header file. Use the actual header files for the module tests, and don't stub headers if possible. The stubbed headers reside in the `MODULETESTS/target_h` directory.

#### Test discovery

Registering module tests to run happens automatically, and the test runner handles registration. However, test files do not automatically  build. Build module tests with a separate system that searches for module tests under the `MODULETESTS` directory.

For the build system to find and build any test suite automatically, include a module test configuration file named `moduletest.cmake` for each module test suite. This configuration file lists all the source files required for the test build.

#### Test names

The build system automatically generates names of test suites. The name is constructed by taking a relative file path from the MODULETESTS directory to the test directory and replacing path separators with dashes. For example, the test suite name for `some/example/path/ModuleName` is `some-example-path-ModuleName`. Suite names are used when deciding which test suites to run.

### Module testing with Mbed CLI

Mbed CLI supports module tests through the `mbed test --moduletests` command. For information on using Mbed CLI, please see the [CLI documentation](https://os.mbed.com/docs/mbed-os/latest/tools/developing-mbed-cli.html).

### Writing module tests

A module tests suite consists of one or more test cases. The test cases should cover all the API functions for a module under test. All the external dependencies are stubbed. Avoid stubbing header files. Finally, analyze code coverage to ensure all code is tested, and no dead code is found.

Please see the [documentation for Google Test](https://github.com/google/googletest/blob/master/googletest/docs/primer.md) to learn how to write module tests using its framework. See the [documentation for Google Mock](https://github.com/google/googletest/blob/master/googlemock/docs/Documentation.md) if you want to write and use C++ mock classes instead of stubs.

#### Test suite configuration

Create two files in the test directory for each test suite:

* Module test source file (`test_ModuleName.cpp`).
* Module test configuration file (`moduletest.cmake`).

List all the required files for the build in the `moduletest.cmake` file with paths relative to the `MODULETESTS` folder. Use the following variables to list the source files and include paths:

* **moduletest-includes**: List of header include paths. You can use this to extend or overwrite default paths listed in `MODULETESTS/CMakeLists.txt`.
* **moduletest-sources**: List of files under test.
* **moduletest-test-sources**: List of test sources and stubs.

You can also set custom compiler flags and other configurations supported by CMake in `moduletest.cmake`.

### Building and running module tests

Use Mbed CLI to build and run module tests. For advanced use, you can run CMake and a Make program directly.

#### Build tests directly with CMake

1. Create a build directory `mkdir MODULETESTS/build`.
1. Move to the build directory `cd MODULETESTS/build`.
1. Run CMake using a relative path to `MODULETESTS` folder as the argument. So from `MODULETESTS/build` use `cmake ..`:
   * Add `-g [generator]` if generating other than Unix Makefiles such in case of MinGW use `-g "MinGW Makefiles"`.
   * Add `-DCMAKE_MAKE_PROGRAM=<value>`, `-DCMAKE_CXX_COMPILER=<value>` and `-DCMAKE_C_COMPILER=<value>` to use a specific Make program and compilers.
   * Add `-DCMAKE_BUILD_TYPE=Debug` for a debug build.
   * Add `-DCOVERAGE=True` to add coverage compiler flags.
   * Add `-Dgtest_disable_pthreads=ON` to run in a single thread.
   * See the [CMake manual](https://cmake.org/cmake/help/v3.0/manual/cmake.1.html) for more information.
1. Run a Make program to build tests.

#### Run tests directly with CTest

Run a test binary in the build directory to run a module test suite. To run multiple test suites at once, use the CTest test runner. Run CTest with `ctest`. Add `-v` to get results for each test case. See the [CTest manual](https://cmake.org/cmake/help/v3.0/manual/ctest.1.html) for more information.

#### Run tests with GUI test runner

1. Install `gtest-runner` according to the [documentation](https://github.com/nholthaus/gtest-runner).
1. Run `gtest-runner`.
1. Add test executables into the list and run.

### Debugging

1. Use Mbed CLI to build a debug build. For advanced use, run CMake directly with `-DCMAKE_BUILD_TYPE=Debug`, and then run a Make program.
1. Run GDB with a test executable as an argument to debug module tests.
1. Run tests with Valgrind to analyze the test memory profile.

### Get code coverage

Use Mbed CLI to generate code coverage reports. For advanced use, follow these steps:

1. Run CMake with both `-DCMAKE_BUILD_TYPE=Debug` and `-DCOVERAGE=True`.
1. Run a Make program to build the tests.
1. Run the tests.
1. Run Gcovr or any other code coverage tool directly in the build directory.

### Troubleshooting

**Problem:** Generic problems with CMake or with the build process.
* **Solution**: Delete the build directory. Make sure that CMake, g++, GCC and a Make program can be found in the path and are correct versions.

**Problem:** (Windows) Virus protection identifies files generated by CMake as malicious and quarantines the files.
* **Solution**: Restore false-positive files from the quarantine.

**Problem:** (Windows) Git with shell installation adds sh.exe to the path and then CMake throws an error: sh.exe was found in your PATH. For MinGW make to work correctly, sh.exe must NOT be in your path.
* **Solution**:  Remove sh.exe from the system path.

**Problem:** (Mac OS) CMake compiler check fails on Mac OS Mojave when using GCC-8.
* **Solution**: Make sure gnm (binutils) is not installed. Uninstall binutils with `brew uninstall binutils`.
