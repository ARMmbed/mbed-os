# Development moved

The development of Greentea has been moved into the [mbed-os-tools](../../src/mbed_os_tools) package. You can continue to use this module for legacy reasons, however all further development should be continued in the new package.

-------------

[![PyPI version](https://badge.fury.io/py/mbed-greentea.svg)](https://badge.fury.io/py/mbed-greentea)

# Greentea - test automation for mbed
_**G**eneric **re**gression **en**vironment for **te**st **a**utomation_

## Introduction

Greentea is the automated testing tool for mbed OS development. It automates the process of flashing mbed boards, driving the test and accumulating test results into test reports. Developers use it for local development as well as for automation in a Continuous Integration environment.

This document should help you start using Greentea. Please see the [htrun documentation](https://github.com/ARMmbed/htrun), the tool Greentea uses to drive tests, for the technical details of the interactions between the platform and the host machine.

Because Greentea is an open source project, we accept contributions! Please see our [contributing document](CONTRIBUTING.md) for more information.

### Prerequistes

Greentea requires [Python version 2.7](https://www.python.org/downloads/). It supports the following OSes:

- Windows
- Linux (Ubuntu preferred)
- OS X (experimental)

### Installing

Tools that depend on Greentea usually install it. Determine if Greentea is already installed by running:
```
$ mbedgt --version
1.2.5
```

You can also install it manually via pip.

```
pip install mbed-greentea
```

## Test specification JSON format

The Greentea test specification format decouples the tool from your build system. It provides important data, such as test names, paths to test binaries and the platform on which the binaries should run.

Greentea automatically looks for files called `test_spec.json` in your working directory. You can also use the `--test-spec` argument to direct Greentea to a specific test specification file.

When you use the `-t` / `--target` argument with the `--test-spec` argument, you can select which "build" should be used. In the example below, you could provide the arguments `--test-spec test_spec.json -t K64F-ARM` to only run that build's tests.

### Example of test specification file

In the below example, there are two defined builds:
* Build `K64F-ARM` for NXP `K64F` platform compiled with `ARMCC` compiler.
* Build `K64F-GCC` for NXP `K64F` platform compiled with `GCC ARM` compiler.

```json
{
    "builds": {
        "K64F-ARM": {
            "platform": "K64F",
            "toolchain": "ARM",
            "base_path": "./BUILD/K64F/ARM",
            "baud_rate": 9600,
            "tests": {
                "tests-mbedmicro-rtos-mbed-mail": {
                    "binaries": [
                        {
                            "binary_type": "bootable",
                            "path": "./BUILD/K64F/ARM/tests-mbedmicro-rtos-mbed-mail.bin"
                        }
                    ]
                },
                "tests-mbed_drivers-c_strings": {
                    "binaries": [
                        {
                            "binary_type": "bootable",
                            "path": "./BUILD/K64F/ARM/tests-mbed_drivers-c_strings.bin"
                        }
                    ]
                }
            }
        },
        "K64F-GCC": {
            "platform": "K64F",
            "toolchain": "GCC_ARM",
            "base_path": "./BUILD/K64F/GCC_ARM",
            "baud_rate": 9600,
            "tests": {
                "tests-mbedmicro-rtos-mbed-mail": {
                    "binaries": [
                        {
                            "binary_type": "bootable",
                            "path": "./BUILD/K64F/GCC_ARM/tests-mbedmicro-rtos-mbed-mail.bin"
                        }
                    ]
                }
            }
        }
    }
}
```

The examples below use the above test specification file.

## Command-line usage
This section highlights a few of the capabilities of the Greentea command-line interface. For a full list of the available options, please run `mbedgt --help`.

Assume for the examples below that the above `test_spec.json` file is in the current directory.

### Listing all tests
You can use the `-l` argument to list all available tests:

```
$ mbedgt -l
mbedgt: greentea test automation tool ver. 1.2.5
mbedgt: using multiple test specifications from current directory!
        using 'BUILD\tests\K64F\ARM\test_spec.json'
        using 'BUILD\tests\K64F\GCC_ARM\test_spec.json'
mbedgt: available tests for built 'K64F-GCC_ARM', location 'BUILD/tests/K64F/GCC_ARM'
        test 'tests-mbedmicro-rtos-mbed-mail'
mbedgt: available tests for built 'K64F-ARM', location 'BUILD/tests/K64F/ARM'
        test 'tests-mbed_drivers-c_strings'
        test 'tests-mbedmicro-rtos-mbed-mail'
```

### Executing all tests
The default action of Greentea using `mbedgt` is to execute all tests that are found in `test_spec.json` files. You can also add `-V` to make the output more verbose.


### Limiting tests
You can select test cases by name using the `-n` argument. This command executes all tests named `tests-mbedmicro-rtos-mbed-mail` from all builds in the test specification:
```
$ mbedgt -n tests-mbedmicro-rtos-mbed-mail
```

When using the `-n` argument, you can use the `*` character at the end of a test name to match all tests that share a prefix. This command executes all tests that start with `tests*`:

```
$ mbedgt -n tests*
```

You can use the `-t` argument to select which build to use when finding tests. This command executes the test `tests-mbedmicro-rtos-mbed-mail` for the `K64F-ARM` build in the test specification:
```
$ mbedgt -n tests-mbedmicro-rtos-mbed-mail -t K64F-ARM
```

You can use a comma (`,`) to separate test names (argument `-n`) and build names (argument `-t`). This command executes the tests `tests-mbedmicro-rtos-mbed-mail` and `tests-mbed_drivers-c_strings` for the `K64F-ARM` and `K64F-GCC_ARM` builds in the test specification:
```
$ mbedgt -n tests-mbedmicro-rtos-mbed-mail,tests-mbed_drivers-c_strings -t K64F-ARM,K64F-GCC_ARM
```

### Selecting platforms
You can limit which boards Greentea uses for testing by using the `--use-tids` argument.

```
$ mbedgt --use-tids 02400203C3423E603EBEC3D8,024002031E031E6AE3FFE3D2
```

Where ```02400203C3423E603EBEC3D8``` and ```024002031E031E6AE3FFE3D2``` are the target IDs of platforms attached to your system.

You can view target IDs using the [mbed-ls](https://pypi.org/project/mbed-ls) tool (installed with Greentea).

```
$ mbedls
+--------------+---------------------+------------+------------+-------------------------+
|platform_name |platform_name_unique |mount_point |serial_port |target_id                |
+--------------+---------------------+------------+------------+-------------------------+
|K64F          |K64F[0]              |E:          |COM160      |024002031E031E6AE3FFE3D2 |
|K64F          |K64F[1]              |F:          |COM162      |02400203C3423E603EBEC3D8 |
|LPC1768       |LPC1768[0]           |G:          |COM5        |1010ac87cfc4f23c4c57438d |
+--------------+---------------------+------------+------------+-------------------------+
```
In this case, you won't test one target, the LPC1768.

### Testing on Fast Model FVPs

Fast Models FVPs are software models for Arm reference design platfrom

Greentea supports running test on Fast Models. And [mbed-fastmodel-agent](https://github.com/ARMmbed/mbed-fastmodel-agent) module is required for this purpose.

The "--fm" option only available when the `mbed-fastmodel-agent` module is installed :

You can run tests for FVP_MPS2_Cortex-M3 models, by '--fm' option:
```
$ mbedgt --fm FVP_MPS2_M3:DEFAULT
```

Where ```FVP_MPS2_M3``` is the platfrom name for the ```FVP_MPS2_Cortex-M3``` models in mbed OS.

And ```DEFAULT``` is the config to the Fast Model, you can find out using ```mbedfm``` command

### Creating reports
Greentea supports a number of report formats.

#### HTML
This creates an interactive HTML page with test results and logs.
```
mbedgt --report-html html_report.html
```

#### JUnit
This creates an XML JUnit report, which you can use with popular Continuous Integration software, such as [Jenkins](https://jenkins.io/index.html).
```
mbedgt --report-junit junit_report.xml
```

#### JSON
This creates a general JSON report.
```
mbedgt --report-json json_report.json
```

#### Plain text
This creates a human-friendly text summary of the test run.
```
mbedgt --report-text text_report.text
```

## Host test detection
When developing with mbed OS, Greentea detects host tests automatically if you place them in the correct location. All tests in mbed OS are placed under a `TESTS` directory. You may place custom host test scripts in a folder named `host_tests` in this folder. For more information about the mbed OS test directory structure, please see the [mbed CLI documentation](https://docs.mbed.com/docs/mbed-os-handbook/en/latest/dev_tools/cli/#test-directory-structure).

## Common issues

### `IOERR_SERIAL` errors
Possible causes:
- Another program is using the serial port. Be sure all terminals and other instances of Greentea are closed before trying again.
- The mbed interface firmware is out of date. Please see the platform's page on [developer.mbed.org](https://developer.mbed.org/) for details about how to update it.
