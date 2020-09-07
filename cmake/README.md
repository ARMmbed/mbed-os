## CMake for Mbed OS

Until further notice, CMake for Mbed OS is still in the development. 

Application profile selection (`bare metal` or `full`), printf library selection (`minimal-printf` or `std`), C library selection (`small` or `std`) and other features that require altering compiler/linker command line options are not yet supported.
The full profile with the standard printf and C library are selected by default.

### Supported targets

Only a limited set of targets is supported at the moment.

These targets should work:
- K64F 
- DISCO_L475VG_IOT01A 
- NRF52840_DK 
- WIO_3G
- K66F

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

From the application root run the following:
1. To create the Mbed OS configuration CMake module, from the root of the application or wherever `mbed-os.lib` is found: 

    ```
    mbedtools configure -m <mbed-target> -t <toolchain>
    ```

1. Run the following command to create a build directory, generate the project configuration and build the project using `Ninja`: 

    ```
    mkdir build && cd build && cmake .. -GNinja && cmake --build .
    ```

The default build type is Develop. Use `CMAKE_BUILD_TYPE` to change it. The options are Develop, Release or Debug: 

```
cmake .. -GNinja -DCMAKE_BUILD_TYPE=<supported-build-type> && cmake --build .
```
