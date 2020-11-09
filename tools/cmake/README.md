## CMake for Mbed OS

In order to [replace the traditional Mbed tools](https://os.mbed.com/blog/entry/Introducing-the-new-Mbed-Tools/), we are adding CMake support to allow the generation of build systems instructions to build Mbed OS as a library.

This is still at the experimental stage and still in development. It does not yet completely replace our current build tools. If you find a bug, please report it or raise a pull request as contributions are welcome!

Mbed OS is built as collection of all core libraries. Application project executables can be linked with the `mbed-os` library (which includes the core libraries of Mbed OS) and optional `mbed-os-` prefixed libraries depending on the additional features required by the projects and supported by the Mbed target.

The list of optional libraries for each component can be seen in the CMake input source file at the root of each component directory.

The following features are not yet supported and will be progressively added:
* Application profile selection (`bare metal`)
* Features that require altering compiler/linker command line options (except for printf library and C selections)

The full profile with the selected printf and C libraries.

### Supported targets

Only a limited set of targets is supported at the moment.

The following targets are supported:
- DISCO_L475VG_IOT01A 
- K64F 
- K66F
- NRF52840_DK
- NUCLEO_F303K8
- NUCLEO_F401RE
- WIO_3G

### Supported toolchains

Arm Compiler 6 and GNU Arm Embedded toolchains are supported.

### Examples

Supported examples can be identified by the presence of a top level `CMakelists.txt` input source file.

### Known issues

- Ninja: ARMClang6 response files require unix paths on Windows. Reference: https://gitlab.kitware.com/cmake/cmake/-/issues/21093. We disabled response files for Ninja on Windows until CMake is fixed.

## How to build an application

Prerequisities:
- CMake >=3.19.0
- mbed-tools >=3.4.0

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
