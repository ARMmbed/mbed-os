# FAT32 Filesystem Support README #

Simon Hughes

20161219

Version 0.02


# Executive Summary

These notes are intended to help the mbed client team adopt the mbedOS POSIX File API support. The notes cover:

- Brief notes on how to setup the mbedOS development environment, including links to other resources.
- How to build the mbedOS test cases.
- How to run the POSIX File API test case.

Note the following:

- The mbedOS FAT32/SDCard support implements a POSIX File API for off-chip, persistent file storage. 
- The FAT32/SDCard support was present in mbedOS 2, but was moved to the "unsupported" area of the source tree in mbedOS 5. 
  This is because there are currently no FAT32/SDCard POSIX API test cases in the public repository, software quality is hence unknown, and the features are therefore unsupported.
- The implementation of new test cases is an active development activity. So far, one test binary (basic.cpp) has been implemented and is available on the mbed-os branch called 'feature-storage'. The test case support
  is being enhanced as a matter of urgency.
- The work described here is intended to make the POSIX File API available to the mbed client team before the Christmas holidays. 
  Further test cases will be implemented and when completed, the work will be merged into the public repository master branch. In the meantime,
  the work is available from feature-storage branch.

# Getting Started

This section describes how to build and run the POSIX file API test case. The following steps are covered:

- [Installing the Tools](#installing-the-tools). This section briefly describes how to setup the mbedos development environment.
- [Git Clone the Public mbedOS Repo](#git-clone-the-mbedos-repo). This section describes how to git clone the mbedOS repository containing the FAT32/SDCard and POSIX File API test case of interest.
- [Build the mbedOS Test Cases](#build-the-mbedos-test-cases). This section describes how to build the mbedOS test cases.
- [Run the POSIX File Test Case](#run-the-posix-file-test-cases).This section describes how to run the POSIX file test case basic.cpp.

### <a name="installing-the-tools"></a> Installing the Tools 

The following tools should be installed:

- arm-none-eabi-gcc. See [mbedOS Development Environment Setup Howto Notes][MBED_DEVENV_NOTES] for guidance.
- Python 2.7.9 or later. See [mbedOS Development Environment Setup Howto Notes][MBED_DEVENV_NOTES] for guidance.
- mbed Greentea. This is the mbedOS test tool.
- Git Bash.  See [mbedOS Development Environment Setup Howto Notes][MBED_DEVENV_NOTES] for guidance.
- mbed-cli. This is the tool used to make mbedOS application and test builds.

Using a Git Bash terminal, setup mbed-cli in the following way:

    simhug01@E107851:/d/demo_area$ git clone git@github.com:/armmbed/mbed-cli
    <trace removed>
    simhug01@E107851:/d/demo_area$ pushd mbed-cli
    simhug01@E107851:/d/demo_area/mbed-cli/$ python.exe setup.py install
    simhug01@E107851:/d/demo_area/mbed-cli/$ popd


Using a Git Bash terminal, setup Greentea in the following way:

    simhug01@E107851:/d/demo_area$ git clone git@github.com:/armmbed/greentea
    <trace removed>
    simhug01@E107851:/d/demo_area$ pushd greentea
    simhug01@E107851:/d/demo_area/greentea/$ python.exe setup.py install
    simhug01@E107851:/d/demo_area/greentea/$ popd
    simhug01@E107851:/d/demo_area/$


### <a name="git-clone-the-mbedos-repo"></a> Git Clone the Public mbedOS Repo 

Get a clone of public mbedOS repository in the following way

    simhug01@E107851:/d/demo_area$ mkdir ex_app1
    simhug01@E107851:/d/demo_area$ pushd ex_app1
    simhug01@E107851:/d/demo_area/ex_app1$ git clone git@github.com:/armmbed/mbed-os
    <trace removed>
    simhug01@E107851:/d/demo_area/ex_app1$


### <a name="build-the-mbedos-test-cases"></a> Build the mbedOS Test Cases

Switch to the `feature-storage` branch using the following commands:

    simhug01@E107851:/d/demo_area/ex_app1$ pushd mbed-os
    simhug01@E107851:/d/demo_area/ex_app1$ git branch
    * master
      feature-storage
      <other branches deleted>
    simhug01@E107851:/d/demo_area/ex_app1$ git checkout feature-storage
    Switched to branch 'feature-storage'
    simhug01@E107851:/d/demo_area/ex_app1$ git branch
      master
    * feature-storage
      <other branches deleted>
    simhug01@E107851:/d/demo_area/ex_app1$

Build the test cases for the K64F target using the following commands:

    simhug01@E107851:/d/demo_area/ex_app1$ mbed -v test --compile -t GCC_ARM -m K64F -DFSFAT_SDCARD_INSTALLED 2>&1 | tee build_tests_gcc_20161219_1007.txt
    <trace removed>
    simhug01@E107851:/d/demo_area/ex_app1$

The complete [build log][BUILD-TESTS-GCC-20161219-1007] is available for reference.


### <a name="run-the-posix-file-test-cases"></a> Run the POSIX File Test Case

To setup for running the test cases, complete the following steps:

- Insert a micro SDCard into K64F SDCard socket.
- Connect the K64F development board to your PC using a suitable USB cable.

All tests can be run using the following command:

    simhug01@E107851:/d/demo_area/ex_app1$ mbedgt -VS
    <trace removed>

However, it's possible to run a particular test case using the following .json file:

    {
      "builds": {
        "K64F-GCC_ARM": {
          "binary_type": "bootable", 
          "tests": {
            "mbed-os-features-storage-feature_storage-tests-fs-fat-basic": {
              "binaries": [
                {
                  "path": "BUILD/tests/K64F/GCC_ARM/mbed-os/features/storage/FEATURE_STORAGE/TESTS/fs-fat/basic/basic.bin"
                }
              ]
            } 
          }, 
          "toolchain": "GCC_ARM", 
          "base_path": "BUILD/tests/K64F/GCC_ARM", 
          "baud_rate": 9600, 
          "platform": "K64F"
        }
      }
    }

Store the above text in a file called `ex_app1_fat_basic_spec.json` in the ex_app1 directory. The test can be run using the following command:

    simhug01@E107851:/d/demo_area/ex_app1$ mbedgt -VS --test-spec=ex_app1_fat_basic_spec.json 2>&1 | tee run_tests_master_gcc_ex_app2_fat_basic_20161219_1011.txt
    <trace removed>

On a successful run, the following table will be shown at the end of the log:

    mbedgt: test suite 'mbed-os-features-storage-feature_storage-tests-fs-fat-basic' ..................... OK in 15.86 sec
        test case: 'FSFAT_test_00: fopen()/fgetc()/fprintf()/fclose() test.' ......................... OK in 0.90 sec
        test case: 'FSFAT_test_01: fopen()/fseek()/fclose() test.' ................................... OK in 0.32 sec
    mbedgt: test case summary: 2 passes, 0 failures
    mbedgt: all tests finished!
    mbedgt: shuffle seed: 0.7720862854
    mbedgt: test suite report:
    +--------------+---------------+-------------------------------------------------------------+--------+--------------------+-------------+
    | target       | platform_name | test suite                                                  | result | elapsed_time (sec) | copy_method |
    +--------------+---------------+-------------------------------------------------------------+--------+--------------------+-------------+
    | K64F-GCC_ARM | K64F          | mbed-os-features-storage-feature_storage-tests-fs-fat-basic | OK     | 15.86              | shell       |
    +--------------+---------------+-------------------------------------------------------------+--------+--------------------+-------------+
    mbedgt: test suite results: 1 OK
    mbedgt: test case report:
    +--------------+---------------+-------------------------------------------------------------+---------------------------------------------------------+--------+--------+--------+--------------------+
    | target       | platform_name | test suite                                                  | test case                                               | passed | failed | result | elapsed_time (sec) |
    +--------------+---------------+-------------------------------------------------------------+---------------------------------------------------------+--------+--------+--------+--------------------+
    | K64F-GCC_ARM | K64F          | mbed-os-features-storage-feature_storage-tests-fs-fat-basic | FSFAT_test_00: fopen()/fgetc()/fprintf()/fclose() test. | 1      | 0      | OK     | 0.9                |
    | K64F-GCC_ARM | K64F          | mbed-os-features-storage-feature_storage-tests-fs-fat-basic | FSFAT_test_01: fopen()/fseek()/fclose() test.           | 1      | 0      | OK     | 0.32               |
    +--------------+---------------+-------------------------------------------------------------+---------------------------------------------------------+--------+--------+--------+--------------------+
    mbedgt: test case results: 2 OK
    mbedgt: completed in 16.53 sec
    

The full [test output log][RUN-TESTS-GCC-20161219-1011] is available for reference.

    
# POSIX File API

mbedOS supports a subset of the POSIX File API, as outlined below:

- clearerr()
    - STATUS: Basic testing implemented. Working.
- [fclose()][MAN_FCLOSE].
    - STATUS: Basic testing implemented. Working.
- ferror()
    - STATUS: Basic testing implemented. Working.
- [fgetc()][MAN_FGETS].
    - STATUS: Basic testing implemented. Working.
- [fgets()][MAN_FGETS].
    - STATUS: Basic testing implemented. Working.
- [fputc()][MAN_FPUTS].
    - STATUS: Unknown.
- [fputs()][MAN_FPUTS].
    - STATUS: Basic testing implemented. Working.
- fprintf()
    - STATUS: Basic testing implemented. Working.
- fopen()
    - STATUS: Basic testing implemented. Working. 
- freopen()
    - STATUS: This is not tested.
- [fread()][MAN_FREAD]
    - STATUS: Basic testing implemented. Working.
- ftell()
    - STATUS: Basic testing implemented. Working.
- [fwrite()][MAN_FWRITE]
    - STATUS: Basic testing implemented. Working.
- [fseek()][MAN_FSEEK]
    - STATUS: Basic testing implemented. Working.
- [getc()][MAN_FGETS].
    - STATUS: Basic testing implemented. Working.
- [gets()][MAN_FGETS].
    - STATUS: Unknown.
- [putc()][MAN_FPUTS].
    - STATUS: Unknown.
- [puts()][MAN_FPUTS].
    - STATUS: Unknown.
- [remove()][MAN_REMOVE]
    - STATUS: Basic testing implemented. Working.
- rewinddir().
    - STATUS: Implemented. Not tested.
- stat()
    - STATUS: Implemented. Not tested.
- tmpfile()
    - STATUS: Not implemented.
- tmpnam() 
    - STATUS: Not implemented.

Supported directory related operations are as follows:

- closedir().
    - STATUS: Implemented. Not tested.
- mkdir(). 
    - STATUS: Basic testing implemented. Working.
- opendir(). 
    - STATUS: Implemented. Not tested.
- readdir().
    - STATUS: Implemented. Not tested.
- [remove()][MAN_REMOVE]
    - STATUS: Basic testing implemented. Working.
- rename()
    - STATUS: Implemented. Not tested.
- [rewind()][MAN_REWIND].
    - STATUS: Basic testing implemented. Working. 
- seekdir()
    - STATUS: Implemented. Not tested.
- telldir().
    - STATUS: Implemented. Not tested.

## errno 

Basic errno reporting is supported, tested and known to be working. This will be extended 
as further test cases are implemented.


## Miscellaneous Information

The FAT32/SDCard support is at the following location in the source code tree:

    <mbed-os_src_root>\features\storage\FEATURE_STORAGE\fs
    
The FAT32/SDCard test cases are at following locations in the source code tree:

    <mbed-os_src_root>\features\storage\FEATURE_STORAGE\TESTS\filesystem\basic\basic.cpp
    <mbed-os_src_root>\features\storage\FEATURE_STORAGE\TESTS\filesystem\fopen\fopen.cpp





# Further Reading

* The [mbedOS Development Environment Setup Notes][MBED_DEVENV_NOTES].
* The example mbedOS build log [BUILD-TESTS-GCC-20161219-1007][BUILD-TESTS-GCC-20161219-1007] for reference.
* The example mbedOS test run log [RUN-TESTS-GCC-20161219-1011][RUN-TESTS-GCC-20161219-1011] for reference.


[MBED_DEVENV_NOTES]: https://github.com/ARMmbed/meVo/blob/master/docs/ARM_MBED/TN/ARM_MBED_TN_0017/12-mbed_devenv_setup_how_to_notes.docx
[BUILD-TESTS-GCC-20161219-1007]: https://github.com/ARMmbed/meVo/blob/master/docs/ARM_MBED/TN/ARM_MBED_TN_0017/build_tests_gcc_20161219_1007.txt
[RUN-TESTS-GCC-20161219-1011]: https://github.com/ARMmbed/meVo/blob/master/docs/ARM_MBED/TN/ARM_MBED_TN_0017/run_tests_master_gcc_ex_app2_fat_basic_20161219_1011.txt

[MAN_FCLOSE]: https://linux.die.net/man/3/fclose
[MAN_FGETS]: https://linux.die.net/man/3/fgets
[MAN_FPUTS]: https://linux.die.net/man/3/fputs
[MAN_FREAD]: https://linux.die.net/man/3/fread
[MAN_FSEEK]: https://linux.die.net/man/3/fseek
[MAN_FWRITE]: https://linux.die.net/man/3/fwrite
[MAN_REMOVE]: https://linux.die.net/man/3/remove
[MAN_REWIND]: https://linux.die.net/man/3/rewind
