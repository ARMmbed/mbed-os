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
- Ambiq Micro targets
- Analog Devices targets
- ARM FM targets
- ARM SSG targets
- Cypress targets
- Freescale targets
- GigaDevice targets
- MAXIM targets
- Nordic targets
- NXP targets
- Renesas targets
- Samsung targets
- Silicon Labs targets
- STM targets
- Toshiba targets
- WICED targets

### Supported toolchains

Arm Compiler 6 and GNU Arm Embedded toolchains are supported.

### Examples

Supported examples can be identified by the presence of a top level `CMakeLists.txt` input source file.

### Known issues

- Ninja: ARMClang6 response files require unix paths on Windows. Reference: https://gitlab.kitware.com/cmake/cmake/-/issues/21093. We disabled response files for Ninja on Windows until CMake is fixed.

## How to build an application

Prerequisities:
- CMake >=3.19.0
- mbed-tools >=4.0.0
- Python modules defined in [`tools/cmake/requirements.txt`](./requirements.txt)

From the application root or wherever `mbed-os.lib` is found, run the following command to:
 * create the Mbed OS configuration CMake module
 * create a build directory
 * generate the project configuration
 * build the project using the `Ninja` build system

    ```
    mbedtools compile -m <mbed-target> -t <toolchain>
    ```

`CMAKE_BUILD_TYPE` can overridden with Mbed specific values: `Develop` (default value), `Release` and `Debug`.

`mbed-tools` will pass `-DCMAKE_BUILD_TYPE=<supported-build-type>` for you when using the `--build-type` optional argument of the `build` subcommand as follows:

```
mbedtools compile -m <mbed-target> -t <toolchain> -b <supported-build-type>
```

If you're running CMake directly, you may need to pass it in yourself as follows:
```
cmake -S <source-dir> -B <build-dir> -DCMAKE_BUILD_TYPE=debug
```

## How to build a greentea test

Install prerequisites suggested in the previous section and follow the below steps to build:
* Generate the `.mbedbuild/` configuration directory for the Mbed target you want to run the test on using [mbed-os-example-blinky](https://github.com/ARMmbed/mbed-os-example-blinky)
```
$ mbedtools configure -t <TOOLCHAIN> -m <MBED_TARGET> 
```
* Copy `.mbedbuild/` into the test suite directory.
* Set your current directory to the test suite directory
* Run the following command to build the test binary with the full profile

  ```
  touch mbed-os.lib && mkdir cmake_build && cd cmake_build && cmake .. -G Ninja && cmake --build .
  ```
* Run the following command to build the test binary with the baremetal profile
  ```
  touch mbed-os.lib && mkdir cmake_build && cd cmake_build && cmake .. -G Ninja -DMBED_BAREMETAL_GREENTEA_TEST=ON && cmake --build .
  ```

Note: These steps will change when `mbedtools` implements a sub-command to invoke Greentea tests
