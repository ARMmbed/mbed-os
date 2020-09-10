## CMake for Mbed OS

In order to [replace the traditional Mbed tools](https://os.mbed.com/blog/entry/Introducing-the-new-Mbed-Tools/), we are adding CMake support to allow the generation of build systems instructions to build Mbed OS as a library.

This is still at the experimental stage and still in development. It does not yet completely replace our current build tools. If you find a bug, please report it or raise a pull request as contributions are welcome!

Mbed OS is built as collection of all libraries supported by a given Mbed target. Application project executables can be linked with the monolithic `mbed-os` library to build Mbed applications.

We are currently working to break down the `mbed-os` library into several individual libraries that applications can selectively choose to include depending on their needs.

The following features are not yet supported and will be progressively added:
* Application profile selection (`bare metal`)
* C library selection (`small`)
* `s/f/v/printf` library selection (`minimal-printf`)
* Other features that require altering compiler/linker command line options

The full profile with the standard printf and C library are selected by default.

### Supported targets

Only a limited set of targets is supported at the moment.

The following targets are supported:
- DISCO_L475VG_IOT01A 
- K64F 
- K66F
- NRF52840_DK 
- WIO_3G

### Supported toolchains

Arm Compiler 6 and GNU Arm Embedded toolchains are supported.

### Examples

Supported examples can be identified by the presence of a top level `CMakelists.txt` input source file.

### Known issues

- ARMClang6: the assembler does not include definitions. It will be fixed in the upcoming release (>3.18.2). Reference: https://gitlab.kitware.com/cmake/cmake/-/issues/21148#note_823305
- Ninja: ARMClang6 response files require unix paths on Windows. Reference: https://gitlab.kitware.com/cmake/cmake/-/issues/21093

## How to build an application

Prerequisities:
- CMake >=3.18.2
- mbed-tools >=3.2.0

From the application root or wherever `mbed-os.lib` is found:
1. Run the following command to create the Mbed OS configuration CMake module: 

    ```
    mbedtools configure -m <mbed-target> -t <toolchain>
    ```

1. Run the following command to create a build directory, generate the project configuration and build the project using `Ninja`: 

    ```
    mkdir build && cd build && cmake .. -GNinja && cmake --build .
    ```

The default build type is `Develop`. Use `CMAKE_BUILD_TYPE` to select `Develop`, `Release` or `Debug` as follows: 

```
cmake .. -GNinja -DCMAKE_BUILD_TYPE=<supported-build-type> && cmake --build .
```
