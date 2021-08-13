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

The post build operations are supported for the following Mbed boards:
LPC1114, LPC1768, ARCH_PRO, LPC54114,
LPC546XX, FF_LPC546XX, CY8CKIT064B0S2_4343W, CYTFM_064B0S2_4343W, CYSBSYSKIT_01,
ARM_MUSCA_B1, ARM_MUSCA_S1.

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

## How to build and run greentea tests

### Set up
Install the [mbed-hosts-tests](https://pypi.org/project/mbed-host-tests/) python package.

Also install the prerequisites suggested in the previous section.

### Configuring the build system
1. Set your current directory to the mbed-os root.
1. Connect your target and detect it using `mbed-tools`
   ```
   mbedtools detect
   ```
   _Make a note of the mount point and serial port. We will need to pass them to CMake in a later step._

1. Generate an mbed_config.cmake configuration file for your target using `mbed-tools`
   ```
   mbedtools configure -t <TOOLCHAIN> -m <MBED_TARGET> --mbed-os-path . --output-dir __build
   ```
1. Generate the build system with CMake, configuring it to build greentea tests.
   Here you will pass the serial port and mount point you noted in an earlier
   step
   ```
   cmake -S . -B __build -G Ninja -DBUILD_GREENTEA_TESTS=ON -DCMAKE_CTEST_ARGUMENTS="--output-on-failure;-V" -DMBED_HTRUN_ARGUMENTS="-p;<serial-port-from-detect>;-d;<mount-point-from-detect>"
   ```
   The `MBED_HTRUN_ARGUMENTS` argument is a semicolon separated list of arguments
   to forward to `mbedhtrun`. You can pass any of the optional flags you would
   usually pass to `mbedhtrun` by adding them to this list.

   The `CMAKE_CTEST_ARGUMENTS` argument is a semicolon separated list of
   arguments to forward to CTest, for more details see [the CMake documentation.](https://cmake.org/cmake/help/latest/variable/CMAKE_CTEST_ARGUMENTS.html)

  If you want to build with the "bare-metal" profile, append
  `-DMBED_GREENTEA_TEST_BAREMETAL=ON` to the above command.

### Build and run tests
  To build and run the tests run the following commands
  ```
  cmake --build __build && cmake --build __build --target test
  ```
  Alternatively you can run ctest directly
  ```
  cmake --build __build
  cd __build
  ctest -V
  ```
  This will build the tests and run them using `mbedhtrun` to communicate with the target.
  For more information on `ctest` and the options it offers, see [the ctest documentation.](https://cmake.org/cmake/help/latest/manual/ctest.1.html)

Notes:
* Some Greentea tests require specific application configuration files in order to build and run successfully. For example, the `connectivity/mbedtls/tests/TESTS/mbedtls/sanity` test requires the configuration file found at `TESTs/configs/experimental.json`. The config should be passed to `mbed-tools configure` using the `--app-config` argument.

## Naming convention

- `mbed` namespace: Mbed CMake targets must have `mbed-` prefix
- all characters are lower case
- words separated by hyphens

For example: `mbed-ble-cordio`
