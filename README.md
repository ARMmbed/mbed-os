# mbed OS SDCard Driver (sd-driver) for FAT32 Filesystem Support


Simon Hughes

20170329

Version 1.00


# Executive Summary

The purpose of this document is to describe how to use the mbed OS SDCard 
driver (sd-driver) so applications can read/write 
data to flash storage cards using the standard POSIX File API 
programming interface. The sd-driver uses the SDCard SPI-mode of operation 
which is a subset of possible SDCard functionality.  

This repository contains the mbed-os SDCard driver for generic SPI 
SDCard support and other resources, as outlined below:

- `SDBlockDevice.h` and `SDBlockDevice.cpp`. This is the SDCard driver module presenting 
  a Block Device API (derived from BlockDevice) to the underlying SDCard.
- POSIX File API test cases for testing the FAT32 filesystem on SDCard.
    - basic.cpp, a basic set of functional test cases.
    - fopen.cpp, more functional tests reading/writing greater volumes of data to SDCard, for example.
- `mbed_app.json` mbed-os application configuration file with SPI pin configurations for the CI shield and overrides for specific targets. 
   This file allows the SPI pins to be specified for the target without having to edit the implementation files.
- This README which includes [Summary of POSIX File API Documentation](#summary-posix-api-documentation) 
  including detailed instruction on how to use the FAT filesystem and SDBlockDevice driver. 

The SDCard driver is maintained in this repository as a component separate from the main mbed OS repository. 
Hence the 2 repositories (mbed-os and sd-driver) have to be used together 
to deliver the FAT32 Filesystem/SDCard support. This document explains how to do this.


# Introduction

### Overview

The scope of this document is to describe how applications use the FAT filesystem and sd-driver
components to persistently store data on SDCards. The document is intended to help developers adopt the 
mbed OS POSIX File API support, and in particular to help explain:

- How the software components work together to deliver the storage functionality. 
- How to work with the sd-driver and mbed OS to build the examples. The example code can easily
  be copied into your new application code.
- How to work with the CI Test Shield, which adds an SDCard slot to those targets that do not have already have one.
- How to run the POSIX File API mbed Greentea test cases, which provide further example code of how to use
  the POSIX File API.
 
Section 1 provides an Executive Summary, describing the purpose of the sd-driver, the supporting
software, examples, test cases and documentation.

Section 2 provides an an overview of the material covered including descriptions of the major sections. 

Section 3 provides an overview of the mbed OS filesystem software components, 
including the inter-relationships between the application, POSIX file API, the standard c-library, 
the mbed OS filesystem and the SDCard driver (sd-driver). 

Section 4 describes how to build and run an example application for reading 
and writing data to an SDCard using the POSIX File API. The example begins by describing
the procedure for building and testing on the K64F target. The final sub-sections
describe how to use the test shield to add an SDCard slot to any mbed target, 
and hence enable the persistent storage of data on any supported target.

Section 5 describes an example application which uses the raw 
BlockDevice API to read and write data to the SDCard.

Section 6 describes how to build and run the SDCard POSIX File API mbed Greentea test cases.
There are a number of functional test cases demonstrating how to use the 
mbed OS POSIX File API. 

Section 7 describes the POSIX File API and provides links to useful API documentation web pages. 


### Known mbed-os and sd-driver Compatible Versions

The following versions of the mbed-os and sd-driver repositories are known to work together: 

- {mbed-os, sd-driver} = {mbed-os-5.4.0-rc2, sd-driver-0.0.1-mbed-os-5.4.0-rc2}. 
  `K64F`, `NUCLEO_F429ZI` and `UBLOX_EVK_ODIN_W2` fopen and basic filesystem tests working.
- {mbed-os, sd-driver} = {mbed-os-5.4.0, sd-driver-0.0.2-mbed-os-5.4.0}. 
  `K64F`, `NUCLEO_F429ZI` and `UBLOX_EVK_ODIN_W2` fopen and basic filesystem tests working.
- {mbed-os, sd-driver} = {mbed-os-5.4.1, sd-driver-0.0.3-mbed-os-5.4.1}. 

To find the latest compatible versions, use the following command to see the messages attached to the tags
in the sd-driver repository:

	ex_app7/$ cd sd-driver
	ex_app7/sd-driver$ git tag -n
	sd-driver-0.0.1-mbed-os-5.3.4 Version compatible with mbed-os-5.3.4, and private_mbedos_filesystems-0.0.1-mbed-os-5.3.4.
	sd-driver-0.0.2-mbed-os-5.4.0 Updated README.md to include worked exmaples and restructuring of information.
	sd-driver-0.0.3-mbed-os-5.4.1 Version compatible with mbed-os-5.4.1.


### Known Issues With This Document

There are no known issues with this document.


# Overview of mbed OS Filesystem Software Component Stack


    ------------------------
    |                      |
    |    Application       |        // This application uses the POSIX File API  
    |                      |        // to read/write data to persistent storage backends.
    ------------------------

    ------------------------        // POSIX File API (ISO).

    ------------------------
    |                      |
    |     libc             |        // The standard c library implementation 
    |                      |        // e.g. newlib.
    ------------------------

    ------------------------        // sys_xxx equivalent API.

    ------------------------
    |                      |
    |  mbed_retarget.cpp   |        // Target specific mapping layer.
    |                      |
    ------------------------

    ------------------------        // Filesystem Upper Edge API. 

    ------------------------
    |                      |
    |     File System      |        // File system wrappers and implementation.
    |                      |
    ------------------------

    ------------------------        // FS Lower Edge API (Block Store Interface).

    ------------------------
    |    Block API         |
    |    Device Driver     |        // The SDCard driver, for example.
    |  e.g. sd-driver      |        
    ------------------------
    
    ------------------------        // SPI.h interface.

    ------------------------
    |                      |
    |       SPI            |        // SPI subsystem (C++ classes and C-HAL implementation).
    |                      |
    ------------------------
    
    Figure 1. mbedOS generic architecture of filesystem software stack.

The figure above shows the mbed OS software component stack used for data
storage on SDCard:

- At the top level is the application component which uses the standard POSIX File API 
  to read and write application data to persistent storage.
- The newlib standard library (libc) stdio.h interface (POSIX File API)  
  implementation is used as it's optimised for resource limited embedded systems.
- mbed_retarget.cpp implements the libc back-end file OS handlers and maps them 
  to the FileSystem. 
- The File System code (hosted in mbed-os) is composed of 2 parts: 
	- The mbed OS file system wrapper classes (e.g. FileSystem, File, FileBase classes)
	  which are used to present a consistent API to the retarget module for different 
	  (third-party) file system implementations. 
	- The FAT filesystem implementation code. 
	  The [FATFS: Generic FAT File System Module](http://elm-chan.org/fsw/ff/00index_e.html)
	  (ChanFS) has been integrated within mbed-os. 
- The Block API Device Driver. The SDCard driver is an example of a persistent storage driver.
  It's maintained as a separate component from the mbed OS repository (in this repository). 
- The SPI module provides the mbed OS generic SPI API. This functionality is maintained in 
  mbed OS. 


# SDCard POSIX File API Example App for Reading/Writing Data

### Overview

This section describes how to build and run an example application that 
uses the POSIX File API to read and write data to SDCard. The discussion begins by 
descibing how to run the example on the FRDM K64F target, but this is later
generalised to all target platforms that have the standard 
Arduino form factor headers. Tthe Continuous Integration (CI) Test Shield
can be inserted into the headers to add a SDCard slot to the target.   

The example code is a modified version of the 
[mbed-os-example-fat-filesystem](https://github.com/ARMmbed/mbed-os-example-fat-filesystem) example
modified for use with the sd-driver.

The following sub-sections describe the steps for building and running the example:
 
- The [Pre-Requisites](#pre-requisites) section describes the development environment used for this example.
  Other similar development environments can be used.
- The [Create the Example Project](#create-the-example-project) section describes how the application project is created
  by including the mbed-os and sd-driver code.
- The [Build the Example Project](#build-the-example-project) section describes how to build the example application.
- The [Insert SDCard into K64F](#insert-sdcard-into-k64f) section describes how to select a card and insert it into the
  SDCard slot on the K64F.
- The [Run the Example Binary on the K64F](#run-the-example-binary-on-the-k64f) section describes how to run the 
  example binary on the target and verify the example has run correctly.
- The [Testing with an SDCard on Target XYZ](#testing-with-an-sdcard-on-target-xyx) section describes the use 
  of Continuous Integration Test Shield, which hosts an SDCard slot. By inserting the CI test shield into the
  Arduino headers of an mbed target platform, the SDCard/FAT Filesystem components can be used to store data
  persistently on any standard mbed target development board.


### <a name="pre-requisites"></a> Pre-Requisites

To work through this example, you should have a working development environment on your machine. For example, 
the following tools should be installed:

- A compiler e.g. arm-none-eabi-gcc. 
- Python 2.7.9 or later. 
- [mbed Greentea](https://github.com/armmbed/greentea),  the mbed OS test tool.
- Git Bash or a similar git command line tool to interact with the ARM mbed GitHub repositories. 
- [mbed-cli](https://github.com/armmbed/mbed-cli), the tool used to make mbed OS application and test builds.

For more information on how to setup a development environment, please review the documentation on the  
[mbed documentation site](https://docs.mbed.com).


### <a name="create-the-example-project"></a> Create the Example Project 

First create the top level application directory sd_ex1 and move into it:

    simhug01@E107851:/d/demo_area$ mkdir sd_ex1
    simhug01@E107851:/d/demo_area$ cd sd_ex1
    simhug01@E107851:/d/demo_area/sd_ex1$ 

Next, perform the "mbed new" operation to download the mbed-os repository into this directory:

	simhug01@E107851:/d/demo_area/sd_ex1$ mbed new .
	[mbed] Creating new program "ex_sdcard" (git)
	[mbed] Adding library "mbed-os" from "https://github.com/ARMmbed/mbed-os" at branch latest
	[mbed] Updating reference "mbed-os" -> "https://github.com/ARMmbed/mbed-os/#5faf4b26c5954d15c7c1cccac6498e0c690ad101"
	warning: LF will be replaced by CRLF in mbed-os.lib.
	The file will have its original line endings in your working directory.
	(mx1_venv1) simhug01@E107851:/d/demo_area/sd_ex1$ ls -1
	mbed-os
	mbed-os.lib
	mbed_settings.py
	(mx1_venv1) simhug01@E107851:/d/demo_area/sd_ex1$

Next, get add the sd-driver component to the application project: 

    simhug01@E107851:/d/demo_area/sd_ex1$ mbed add sd-driver
    <trace removed>
    simhug01@E107851:/d/demo_area/sd_ex1$ 

Next, copy the example1.cpp file and `mbed_app.json` files from inside the sd-driver directory to the top level sd_ex1 directory:
    
    simhug01@E107851:/d/demo_area/sd_ex1$ cp sd-driver/features/TESTS/examples/example1.cpp .
    simhug01@E107851:/d/demo_area/sd_ex1$ cp sd-driver/config/mbed_app.json .
    simhug01@E107851:/d/demo_area/sd_ex1$

The `mbed_app.json` file specifies the SPI bus pin configuration for different targets. 
The file includes a specific configuration of the K64F which is used
because the mbed compile command specifies the K64F build target. The `mbed_app.json` file 
is described in more detail in the [Testing with an SDCard on Target XYZ](#testing-with-an-sdcard-on-target-xyx) section. 

### <a name="build-the-example-project"></a> Build the Example Project 

Next, build the example application: 

	simhug01@E107851:/d/demo_area/sd_ex1$ mbed compile -m K64F -t GCC_ARM 2>&1 | tee build_log.txt


#### WARNING: "mbed new ." command and possible mbed-os sd-driver versioning incompatibilities

If you experience problems building the example then it may mean the version
of the mbed-os repository created with the "mbed new ." command is not compatible with 
the sd-driver repository version created with "mbed add sd-driver" command. This is because:

- The "mbed new ." creates the mbed-os repository at the latest "Release" e.g. `mbed-os-5.4.0`.
- The "mbed add sd-driver" command creates the sd-driver repository at the latest version of 
  master i.e. the tip of master. Changes may be present that are not compatible with 
  the latest mbed-os release e.g. in preparation for the next release.

This situation can be resolved by checking out compatible versions of the repositories as 
described in the section [Setting mbed-os/sd-driver Repositories To Compatible Versions](#settting-repos-to-compatible-versions)

### <a name="insert-sdcard-into-k64f"></a> Insert SDCard into K64F

The examples and test cases have been run on a K64F with the following pre-formatted microSDHC cards:

- Kingston 2GB mircoSDHC card.  
- Kingston 8GB mircoSDHC card.  
- SanDisk 16GB mircoSDHC ultra card.  

If the card requires formatting then the following procedure is known to work:

- Insert microSD card into SD adapter in USB stick (or similar) so the microSD card can be insert into windows PC.
- Within file explorer, right click/Format on the USB drive.
- Select FAT32, 4096 cluster size, Quick Format.
- Format the drive.

The microSD card should then be ready for use in the K64F. Insert the formatted card
into the SDCard slot on the K64F PCB. 


### <a name="run-the-example-binary-on-the-k64f"></a> Run the Example Binary on the K64F 

Once the binary is built, copy the binary from `/d/demo_area/sd_ex1/BUILD/K64F/GCC_ARM/example1.bin` to the K64F. 
After connecting a serial console and resetting the target, the following trace should be seen:

	Welcome to the filesystem example.
	Opening a new file, numbers.txt. done.
	Writing decimal numbers to a file (20/20) done.
	Closing file. done.
	Re-opening file read-only. done.
	Dumping file to screen.
	0
	1
	2
	3
	4
	5
	6
	7
	8
	9
	10
	11
	12
	13
	14
	15
	16
	17
	18
	19
	EOF.
	Closing file. done.
	Opening root directory. done.
	Printing all filenames:
	  numbers.txt
	Closeing root directory. done.
	Filesystem Demo complete.


### <a name="testing-with-an-sdcard-on-target-xyx"></a> Testing with an SDCard on Target XYZ

The standard way to test is with the mbed CI Test Shield plugged into the 
target board. This pin mapping for this configuration is parameterised in 
the `mbed_app.json` file.

The following is an example of the `mbed_app.json` file available in the repository:

    {
        "config": {
            "UART_RX": "D0",
            "UART_TX": "D1",
            "DIO_0": "D0",
            "DIO_1": "D1",
            "DIO_2": "D2",
            "DIO_3": "D3",
            "DIO_4": "D4",
            "DIO_5": "D5",
            "DIO_6": "D6",
            "DIO_7": "D7",
            "DIO_8": "D8",
            "DIO_9": "D9",
            "SPI_CS": "D10",
            "SPI_MOSI": "D11",
            "SPI_MISO": "D12",
            "SPI_CLK": "D13",
            "I2C_SDA": "D14",
            "I2C_SCL": "D15",
            "I2C_TEMP_ADDR":"0x90",
            "I2C_EEPROM_ADDR":"0xA0",
            "AIN_0": "A0",
            "AIN_1": "A1",
            "AIN_2": "A2",
            "AIN_3": "A3",
            "AIN_4": "A4",
            "AIN_5": "A5",
            "AOUT" : "A5",
            "PWM_0": "D3",
            "PWM_1": "D5",
            "PWM_2": "D6",
            "PWM_3": "D9",
            "DEBUG_MSG": 0,
            "DEVICE_SPI": 1,
            "FSFAT_SDCARD_INSTALLED": 1
        },
        "target_overrides": {
            "DISCO_F051R8": {
                 "SPI_MOSI": "SPI_MOSI",
                 "SPI_MISO": "SPI_MISO",
                 "SPI_CLK":  "SPI_SCK",
                 "SPI_CS":   "SPI_CS"
            },
            "K20D50M": {
                 "SPI_MOSI": "PTD2",
                 "SPI_MISO": "PTD3",
                 "SPI_CLK":  "PTD1",
                 "SPI_CS":   "PTC2"
            },
            "KL22F": {
                 "SPI_MOSI": "PTD6",
                 "SPI_MISO": "PTD7",
                 "SPI_CLK":  "PTD5",
                 "SPI_CS":   "PTD4"
            },
            "KL25Z": {
                 "SPI_MOSI": "PTD2",
                 "SPI_MISO": "PTD3",
                 "SPI_CLK":  "PTD1",
                 "SPI_CS":   "PTD0"
            },
            "KL43Z": {
                 "SPI_MOSI": "PTD6",
                 "SPI_MISO": "PTD7",
                 "SPI_CLK":  "PTD5",
                 "SPI_CS":   "PTD4"
            },
            "KL46Z": {
                 "SPI_MOSI": "PTD6",
                 "SPI_MISO": "PTD7",
                 "SPI_CLK":  "PTD5",
                 "SPI_CS":   "PTD4"
            },
            "K64F": {
                 "SPI_MOSI": "PTE3",
                 "SPI_MISO": "PTE1",
                 "SPI_CLK":  "PTE2",
                 "SPI_CS":   "PTE4"
            },
            "K66F": {
                 "SPI_MOSI": "PTE3",
                 "SPI_MISO": "PTE1",
                 "SPI_CLK":  "PTE2",
                 "SPI_CS":   "PTE4"
            },
            "LPC11U37H_401": {
                 "SPI_MOSI": "SDMOSI",
                 "SPI_MISO": "SDMISO",
                 "SPI_CLK":  "SDSCLK",
                 "SPI_CS":   "SDSSEL"
            },
            "LPC2368": {
                 "SPI_MOSI": "p11",
                 "SPI_MISO": "p12",
                 "SPI_CLK":  "p13",
                 "SPI_CS":   "p14"
            },
            "NUCLEO_L031K6": {
                 "SPI_MOSI": "SPI_MOSI",
                 "SPI_MISO": "SPI_MISO",
                 "SPI_CLK":  "SPI_SCK",
                 "SPI_CS":   "SPI_CS"
            },
            "nRF51822": {
                 "SPI_MOSI": "p12",
                 "SPI_MISO": "p13",
                 "SPI_CLK":  "p15",
                 "SPI_CS":   "p14"
            },
            "RZ_A1H": {
                 "SPI_MOSI": "P8_5",
                 "SPI_MISO": "P8_6",
                 "SPI_CLK":  "P8_3",
                 "SPI_CS":   "P8_4"
            }
        }
    }

Note the following things about the `mbed_app.json` file:

- The `mbed_app.json` file is used to define target specific symbols for the SPI pins connecting the SDCard slot to the target MCU:
    - "SPI\_CS". This is the Chip Select line.
    - "SPI\_MOSI". This is the Master Out Slave In data line.
    - "SPI\_MISO". This is the Master In Slave Out data line.
    - "SPI\_CLK".  This is the serial Clock line.
- The default configuration defined in the "config" section is for the standard Arduino header pin mappings for the SPI bus. 
  The "config" section defines a dictionary mapping functional names to target board Arduino header pins: 
    - "SPI\_CS": "D10". This causes the MBED\_CONF\_APP\_SPI\_CS symbol to be defined in mbed\_config.h as D10, which is used in the filesystem test implementation.
      D10 is defined in the target specific PinNames.h file.
    - "SPI\_MOSI": "D11". This causes the MBED\_CONF\_APP\_SPI\_MOSI symbol to be defined in mbed\_config.h.
    - "SPI\_MISO": "D12". This causes the MBED\_CONF\_APP\_SPI\_MISO symbol to be defined in mbed\_config.h.
    - "SPI\_CLK": "D13". This causes the MBED\_CONF\_APP\_SPI\_CLK symbol to be defined in mbed\_config.h.
- The `"target_overrides"` section is used to override the "SPI\_xxx" symbols for specific target boards, which may have an SDCard slot, for example.
  This is the case for the K64F, where the "SPI\_xxx" are mapped to the pin names for the on-board SDCard.

  ```
    "K64F": {
         "SPI_MOSI": "PTE3",
         "SPI_MISO": "PTE1",
         "SPI_CLK":  "PTE2",
         "SPI_CS":   "PTE4"
    }
    ```  
- Thus, in the absence of any target specific definitions in the `"target_overrides"` section, all boards will default to 
  using the Arduino header configuration. For those platforms with a `"target_overrides"` section then this configuration
  will be used in preference. 
- Hence in the case that you want to test a platform with an SDCard inserted into a 
  fitted CI test shield (rather than the on-board SDCard slot)
  and there is a `"target_overrides"` section present in the `mbed_app.json` file, you must then delete the `"target_overrides"`
  section before building. This will result in the default configuration being used (suitable for the CI
  Test Shield).
- Note when inserting the v1.0.0 CI Test Shield into the Arduino header of the target platform, the shield pins D0 and
  D1 should be bent to be parallel to the shield PCB so they are not inserted into the Arduino header. This is because
  some boards use the same UART on DAPLINK and D0/D1, which means the serial debug channel breaks and hence the mbed greentea
  test suite will not work correctly. This is mainly on older ST boards and should not be a problem on 
  `K64F`, `NUCLEO_F429ZI` and `UBLOX_EVK_ODIN_W2`. Note also that the v2.0.0 CI Test Shield doesn't suffer from this
  problem and the pins don't need to be bent.  
- When inserting the SDCard into the card slot on the CI test shield, make sure the card is fully inserted. 
  On insertion, there should be a small clicking sound when the card registers, and the back edge of the card
  should protrude no more than ~1mm over the edge of the CI test shield PCB. If the SDCard fails to register, 
  try gently pushing the metal flexible strip in the shape of a spade at the top edge of the SDCard metal slot
  casing with a pair of tweezers, bending it a little to lower it into the slot casing. This helps with the
  insertion mechanism.


### Target K64F with CI Test Shield fitted

![alt text](docs/pics/sd_driver_k64_with_ci_test_shield.jpg "unseen title text")

**Figure 2. The figure shows the K64F platform with the CI shield fitted.**

The above figure shows the K64F with the v1.0.0 CI test shield fitted. Note:

- The pins D0/D1 (top right of CI test shield) are bent sideways so as not to insert into the header.
- The SDCard is fully inserted into the slot and overhangs the PCB by ~1mm.  
  

# SDBlockDevice Example Application 

The following sample code illustrates how to use the sd-driver Block Device API: 


    #include "mbed.h"
    #include "SDBlockDevice.h"

    // Instantiate the SDBlockDevice by specifying the SPI pins connected to the SDCard 
    // socket. The PINS are:
    //     MOSI (Master Out Slave In)
    //     MISO (Master In Slave Out)
    //     SCLK (Serial Clock)
    //     CS (Chip Select)
    SDBlockDevice sd(p5, p6, p7, p12); // mosi, miso, sclk, cs
    uint8_t block[512] = "Hello World!\n";
    
    int main() 
    {
        // call the SDBlockDevice instance initialisation method.
        sd.init();
        
        // Write some the data block to the device 
        sd.program(block, 0, 512);
        
        // read the data block from the device
        sd.read(block, 0, 512);
        
        // print the contents of the block
        printf("%s", block);
        
        // call the SDBlockDevice instance de-initialisation method.
        sd.deinit();
    }


# SDCard POSIX File API mbed Greentea Test Cases

This section describes how to build and run the POSIX file API test cases. 
The following steps are covered:

- [Create the FAT/SDCard Application Project](#create-fat-sdcard-application-project). 
  This section describes how to git clone the mbed OS and sd-driver repositories containing the 
  code and test cases of interest.
- [Build the mbed OS Test Cases](#build-the-mbedos-test-cases). This section 
  describes how to build the mbed OS test cases.
- [Insert a microSD Card Into the K64F for Greentea Testing](#greentea-insert-sdcard-into-k64f).This section 
  describes how to format (if required) a microSD card prior to running the tests.
- [Run the POSIX File Test Case](#run-the-posix-file-test-cases).This section 
  describes how to run the POSIX file test cases.


### <a name="create-fat-sdcard-application-project"></a> Create the FAT/SDCard Application Project

This section describes how to create an application project combining the 
mbed-os and sd-driver repositories into a single project. 
In summary the following steps will be covered in this section:

- A top level application project directory is created. The directory name is ex_app1.
- In the ex_app1 directory, the mbed-os repository is cloned.
- In the ex_app1 directory at the same level as the mbed-os directory, the sd-driver repository is cloned.
- The `mbed_app.json` file is copied from the `sd-driver/config/mbed_app.json` to the ex_app1 directory.

First create the top level application directory ex_app1 and move into it:

    simhug01@E107851:/d/demo_area$ mkdir ex_app1
    simhug01@E107851:/d/demo_area$ pushd ex_app1

Next, get a clone of public mbed OS repository in the following way:

    simhug01@E107851:/d/demo_area/ex_app1$ git clone git@github.com:/armmbed/mbed-os
    <trace removed>
    simhug01@E107851:/d/demo_area/ex_app1$

Next, get a clone of the sd-driver repository:

    simhug01@E107851:/d/demo_area/ex_app1$ git clone git@github.com:/armmbed/sd-driver
    <trace removed>
    simhug01@E107851:/d/demo_area/ex_app1$
    
Finally, copy the `mbed_app.json` application configuration file from `sd-driver/config/mbed_app.json` to the ex_app1 directory:

    simhug01@E107851:/d/demo_area/ex_app1$ cp sd-driver/config/mbed_app.json .
    simhug01@E107851:/d/demo_area/ex_app1$

The `mbed_app.json` file specifies the SPI bus pin configuration for different targets, 
and is discussed in the [Testing with an SDCard on Target XYZ](#testing-with-an-sdcard-on-target-xyx) section. 


### <a name="build-the-mbedos-test-cases"></a> Build the mbed OS Test Cases

Build the test cases for the K64F target using the following command:

    simhug01@E107851:/d/demo_area/ex_app1$ mbed -v test --compile -t GCC_ARM -m K64F --app-config mbed_app.json 2>&1 | tee build_tests_gcc_20161219_1007.txt
    <trace removed>
    simhug01@E107851:/d/demo_area/ex_app1$

The build trace is quite extensive but on a successful build you should see the following output at the end of the log:

	Build successes:
	  * K64F::GCC_ARM::MBED-BUILD
	  * K64F::GCC_ARM::MBED-OS-FEATURES-FEATURE_LWIP-TESTS-MBEDMICRO-NET-CONNECTIVITY
	  <trace removed>
	  * K64F::GCC_ARM::MBED-OS-FEATURES-TESTS-FILESYSTEM-FAT_FILE_SYSTEM
	  * K64F::GCC_ARM::MBED-OS-FEATURES-TESTS-FILESYSTEM-HEAP_BLOCK_DEVICE
	  * K64F::GCC_ARM::MBED-OS-FEATURES-TESTS-FILESYSTEM-UTIL_BLOCK_DEVICE
	  <trace removed>
	  * K64F::GCC_ARM::SD-DRIVER-FEATURES-TESTS-FILESYSTEM-BASIC
	  * K64F::GCC_ARM::SD-DRIVER-FEATURES-TESTS-FILESYSTEM-FOPEN
	
	Build skips:
	  * K64F::GCC_ARM::MBED-OS-FEATURES-FEATURE_LWIP-TESTS-MBEDMICRO-NET-TCP_PACKET_PRESSURE
	  <trace removed>


Notice the following tests in the sd-driver tree are listed above:     

- `K64F::GCC_ARM::SD-DRIVER-FEATURES-TESTS-FILESYSTEM-BASIC`
- `K64F::GCC_ARM::SD-DRIVER-FEATURES-TESTS-FILESYSTEM-FOPEN`


The FAT32/SDCard test cases are at following locations in the source code tree:

    /d/demo_area/ex_app1/sd-driver/features/TESTS/filesystem/basic/basic.cpp
    /d/demo_area/ex_app1/sd-driver/features/TESTS/filesystem/fopen/fopen.cpp


#### <a name="settting-repos-to-compatible-versions"></a> Setting mbed-os/sd-driver Repositories To Compatible Versions 

The sd-driver master HEAD and the mbed-os master HEAD should be compatible 
with one another and therefore no specific tagged versions need to be checked out.
However, in the case that you experience problems building, checkout out the compatible 
tagged version of each repository, as shown below:

    simhug01@E107851:/d/demo_area/ex_app1$ pushd mbed-os
    simhug01@E107851:/d/demo_area/ex_app1$ git checkout tags/mbed-os-5.4.0
    simhug01@E107851:/d/demo_area/ex_app1$ popd 
    simhug01@E107851:/d/demo_area/ex_app1$ pushd sd-driver
    simhug01@E107851:/d/demo_area/ex_app1$ git checkout tags/sd-driver-0.0.2-mbed-os-5.4.0
    simhug01@E107851:/d/demo_area/ex_app1$ popd 

In the above: 

- `mbed-os-5.4.0` should be replaced with the latest mbed-os release tag.
-  For an mbed-os release tag `mbed-os-x.y.z`, use the equivalent sd-driver tag `sd-driver-a.b.c-mbed-os-x.y.z`
   where `a.b.c` is the latest version code for the `mbed-os-x.y.z` tag.

### <a name="greentea-insert-sdcard-into-k64f"></a> Insert SDCard into K64F for Greentea Testing

See the previous section for [Insert SDCard into K64F](#insert-sdcard-into-k64f) for details.


### <a name="run-the-posix-file-test-cases"></a> Run the POSIX File Test Case

To setup for running the test cases, connect the K64F development board to your 
PC using a suitable USB cable.

All tests can be run using the following command:

    simhug01@E107851:/d/demo_area/ex_app1$ mbedgt -VS
    <trace removed>

However, it's possible to run a particular test case using the following form of the mbedgt command:

    simhug01@E107851:/d/demo_area/ex_app1$ mbedgt -VS --test-by-names=<test-name>
    
The names of the tests can be listed using: 

    simhug01@E107851:/d/demo_area/ex_app1$ mbedgt -VS --list

For example, to run the basic test use:
    
    simhug01@E107851:/d/demo_area/ex_app1$ mbedgt -VS --test-by-names=sd-driver-features-tests-filesystem-basic 2>&1 | tee run_tests_basic.txt

To run the fopen test use:

    simhug01@E107851:/d/demo_area/ex_app1$ mbedgt -VS --test-by-names=sd-driver-features-tests-filesystem-fopen 2>&1 | tee run_tests_fopen.txt
    
On a successful run, results similar to the following will be shown:

	mbedgt: test suite report:
	+--------------+---------------+-------------------------------------------+--------+--------------------+-------------+
	| target       | platform_name | test suite                                | result | elapsed_time (sec) | copy_method |
	+--------------+---------------+-------------------------------------------+--------+--------------------+-------------+
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | OK     | 151.46             | shell       |
	+--------------+---------------+-------------------------------------------+--------+--------------------+-------------+
	mbedgt: test suite results: 1 OK
	mbedgt: test case report:
	+--------------+---------------+-------------------------------------------+----------------------------------------------------------------------------------------+--------+--------+--------+--------------------+
	| target       | platform_name | test suite                                | test case                                                                              | passed | failed | result | elapsed_time (sec) |
	+--------------+---------------+-------------------------------------------+----------------------------------------------------------------------------------------+--------+--------+--------+--------------------+
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_01: fopen()/fwrite()/fclose() directories/file in multi-dir filepath. | 1      | 0      | OK     | 7.57               |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_02: fopen(r) pre-existing file try to write it.                       | 1      | 0      | OK     | 0.2                |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_03: fopen(w+) pre-existing file try to write it.                      | 1      | 0      | OK     | 0.41               |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_04: fopen() with a filename exceeding the maximum length.             | 1      | 0      | OK     | 0.11               |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_06: fopen() with bad filenames (minimal).                             | 1      | 0      | OK     | 0.1                |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_07: fopen()/errno handling.                                           | 1      | 0      | OK     | 0.07               |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_08: ferror()/clearerr()/errno handling.                               | 1      | 0      | OK     | 0.1                |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_09: ftell() handling.                                                 | 1      | 0      | OK     | 0.17               |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_10: remove() test.                                                    | 1      | 0      | OK     | 1.28               |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_11: rename().                                                         | 1      | 0      | OK     | 2.3                |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_12: opendir(), readdir(), closedir() test.                            | 1      | 0      | OK     | 3.57               |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_13: mkdir() test.                                                     | 1      | 0      | OK     | 1.21               |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_14: stat() test.                                                      | 1      | 0      | OK     | 1.47               |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_15: format() test.                                                    | 1      | 0      | OK     | 26.12              |
	| K64F-GCC_ARM | K64F          | sd-driver-features-tests-filesystem-fopen | FSFAT_FOPEN_TEST_16: write/check n x 25kB data files.                                  | 1      | 0      | OK     | 87.11              |
	+--------------+---------------+-------------------------------------------+----------------------------------------------------------------------------------------+--------+--------+--------+--------------------+
	mbedgt: test case results: 15 OK
	mbedgt: completed in 152.35 sec
    

# <a name="summary-posix-api-documentation"></a> Summary of POSIX File API Documentation

### POSIX File API

mbed OS supports a subset of the POSIX File API, as outlined below:

- [clearerr()](https://linux.die.net/man/3/clearerr).
    - STATUS: Basic testing implemented. Working.
- [fclose()](https://linux.die.net/man/3/fclose).
    - STATUS: Basic testing implemented. Working.
- [ferror()](https://linux.die.net/man/3/clearerr).
    - STATUS: Basic testing implemented. 
    - STATUS: GCC_ARM: Working. 
    - STATUS: ARMCC: ARMCC has problem with ferror(filep) where filep is NULL. Appears to work for non-NULL pointer.
- [fgetc()](https://linux.die.net/man/3/fgets).
    - STATUS: Basic testing implemented. Working.
- [fgets()](https://linux.die.net/man/3/fgets).
    - STATUS: Basic testing implemented. Working.
- [fputc()](https://linux.die.net/man/3/fputs).
    - STATUS: Unknown.
- [fputs()](https://linux.die.net/man/3/fputs).
    - STATUS: Basic testing implemented. Working.
- [fprintf()](https://linux.die.net/man/3/fprintf).
    - STATUS: Basic testing implemented. Working.
- [fopen()](https://linux.die.net/man/3/fopen).
    - STATUS: Basic testing implemented. Working. 
- [freopen()](https://linux.die.net/man/3/fopen).
    - STATUS: This is not tested.
- [fread()](https://linux.die.net/man/3/fread).
    - STATUS: Basic testing implemented. Working.
    - STATUS: n x 25kB stress test working.
- [ftell()](https://linux.die.net/man/3/ftell).
    - STATUS: Basic testing implemented. Working.
- [fwrite()](https://linux.die.net/man/3/fwrite).
    - STATUS: Basic testing implemented. Working.
    - STATUS: n x 25kB stress test working.
- [fseek()](https://linux.die.net/man/3/fseek)
    - STATUS: Basic testing implemented. Working.
- [getc()](https://linux.die.net/man/3/fgets).
    - STATUS: Basic testing implemented. Working.
- [gets()](https://linux.die.net/man/3/fgets).
    - STATUS: Unknown.
- [putc()](https://linux.die.net/man/3/fputs).
    - STATUS: Unknown.
- [puts()](https://linux.die.net/man/3/fputs).
    - STATUS: Unknown.
- [remove()](https://linux.die.net/man/3/remove)
    - STATUS: Basic testing implemented. Working.
- [rewind()](https://linux.die.net/man/3/rewind).
    - STATUS: Basic testing implemented. Working. 
- [stat()](https://linux.die.net/man/2/stat)
    - STATUS: Implemented. Working.
    - STATUS: Not supported by ARMCC/IAR libc.
- [tmpfile()](https://linux.die.net/man/3/tmpfile).
    - STATUS: Not implemented.
- [tmpnam()](https://linux.die.net/man/3/tmpnam). 
    - STATUS: Not implemented.

Supported directory related operations are as follows:

- [closedir()](https://linux.die.net/man/3/closedir).
    - STATUS: Implemented. Working.
- [mkdir()](https://linux.die.net/man/3/mkdir). 
    - STATUS: Basic testing implemented. Working.
- [opendir()](https://linux.die.net/man/3/opendir). 
    - STATUS: Implemented. Working.
- [readdir()](https://linux.die.net/man/3/readdir).
    - STATUS: Implemented. Working.
- [remove()](https://linux.die.net/man/3/remove).
    - STATUS: Basic testing implemented. Working.
- [rename()](https://linux.die.net/man/3/rename).
    - STATUS: Implemented. Not tested.
- [rewinddir()](https://linux.die.net/man/3/rewinddir).
    - STATUS: Implemented. Found not to work. Test case not present in repo.
- [seekdir()](https://linux.die.net/man/3/seekdir).
    - STATUS: Implemented. Found not to work. Test case not present in repo. 
- [telldir()](https://linux.die.net/man/3/telldir).
    - STATUS: Implemented. Found not to work. Test case not present in repo.

### errno 

Basic errno reporting is supported, tested and known to be working.


# Related Projects Resources

The following are related mbed storage projects and useful resources:

- The [mbed-os repository](https://github.com/ARMmbed/mbed-os). This is the main mbed OS repository.
- The [mbed-os-example-fat-filesystem repository](https://github.com/ARMmbed/mbed-os-example-fat-filesystem).
  This is an example project for the mbed OS FAT filesystem.
- The [spiflash-driver repository](https://github.com/armmbed/spiflash-driver)
- The [i2ceeprom-driver repository](https://github.com/ARMmbed/i2ceeprom-driver.git)
- The [ci-test-shield repository](https://github.com/ARMmbed/ci-test-shield). This is the project describing 
  the mbed-os Continuous Integration test shield, together with standard tests.
- [POSIX File Interface ISO/IEC 9899:TC2 Documentation](http://www.eng.utah.edu/~cs5785/slides-f10/n1124.pdf).
- [FATFS: Generic FAT File System Module used in mbed OS](http://elm-chan.org/fsw/ff/00index_e.html)


# Appendix 1: Getting Started with SPIFlash-Driver Example 

## Overview

This example describes how to build and run the spiflash-driver SPIFBlockDevice examples to 
read and write data to a SPI NOR flash part connected to a K64F.

Hardware required:

- K64F.
- CI test shield.
- SPI NOR Flash Device wired to Arduino header pins e.g. the Macronix MX25R2035F. The datasheet is available 
  from the [Macronix website](http://www.macronix.com/).
- Micro USB cable.

Software required:

- mbed CLI (with all other dependencies installed).
- ARMCC / GCC_ARM / IAR compiler.
- mbed greentea.
- git account.

Github repos to use:

- The [mbed OS repository](https://github.com/armmbed/mbed-os) 
- The [SPI Flash Driver repository](https://github.com/armmbed/spiflash-driver) 

## Simple SPIFBlockDevice Example

This section describes how to create an application project combining the 
mbed-os and spiflash-driver repositories into a single project. 
In summary the following steps will be covered in this section:

- A top level application project directory is created. The directory name is ex_app2.
- In the ex_app2 directory, the mbed-os repository is cloned.
- In the ex_app2 directory at the same level as the mbed-os directory, the spiflash-driver repository is cloned.

First create the top level application directory ex_app2 and move into it:

    simhug01@E107851:/d/demo_area$ mkdir ex_app2
    simhug01@E107851:/d/demo_area$ pushd ex_app2

Next, get a clone of public mbed OS repository in the following way:

    simhug01@E107851:/d/demo_area/ex_app2$ git clone git@github.com:/armmbed/mbed-os
    <trace removed>
    simhug01@E107851:/d/demo_area/ex_app2$

Next, get a clone of the spiflash-driver repository:

    simhug01@E107851:/d/demo_area/ex_app2$ git clone git@github.com:/armmbed/spiflash-driver
    <trace removed>
    simhug01@E107851:/d/demo_area/ex_app2$

In the top level directory create the example2.cpp:     

    simhug01@E107851:/d/demo_area/ex_app2$ touch example2.cpp

Copy the [spiflash-driver example code](https://github.com/armmbed/spiflash-driver) 
and paste into example2.cpp (reproduced here for convenience and corrected to build for 
GCC_ARM):

	// Here's an example using the MX25R SPI flash device on the K82F
	#include "mbed.h"
	#include "SPIFBlockDevice.h"

	/* This is the original configuration of the SPI Flash Driver
	 * pins for Freescale K82F development board. We're not using
	 * this as we're using the CI Test Shield
	 */
	// Create flash device on SPI bus with PTE5 as chip select
	//SPIFBlockDevice spif(PTE2, PTE4, PTE1, PTE5);
	
	/* This configuration of the SPI Flash Driver pins is for
	 * the Freescale K64F connecting the SPI pins on the
	 * Arduino header to the SPI NOR part.
	 */
	SPIFBlockDevice spif(D11, D12, D13, D10);
	
	
	int main() {
	    printf("spif test\n");
	
	    // Initialize the SPI flash device and print the memory layout
	    spif.init();
	    printf("spif size: %llu\n",         spif.size());
	    printf("spif read size: %llu\n",    spif.get_read_size());
	    printf("spif program size: %llu\n", spif.get_program_size());
	    printf("spif erase size: %llu\n",   spif.get_erase_size());
	
	    // Write "Hello World!" to the first block
	    char *buffer = (char*) malloc(spif.get_erase_size());
	    sprintf(buffer, "Hello World!\n");
	    spif.erase(0, spif.get_erase_size());
	    spif.program(buffer, 0, spif.get_erase_size());
	
	    // Read back what was stored
	    spif.read(buffer, 0, spif.get_erase_size());
	    printf("%s", buffer);
	
	    // Deinitialize the device
	    spif.deinit();
	}

Note the following modifications to the original code sample:

- The SPI Flash Driver instance `spif` is given the pin configuration
  for the SPI bus from the Arduino header pins D11, D12, D13 and D10, 
  as noted in the comments.
- The buffer type has been modified to char* to (see line with malloc() above). 

The application can be built with the following command:

	simhug01@E107851:/d/demo_area/ex_app2$ mbed compile -m K64F -t GCC_ARM 2>&1 | tee build_app_ex_app2_log.txt
	
Once the binary is built, copy the binary from `/d/demo_area/ex_app2/BUILD/K64F/GCC_ARM/example2.bin` to the K64F. 
After connecting a serial console and resetting the target, the following trace should be seen:

	spif test
	spif size: 2097152
	spif read size: 1
	spif program size: 1
	spif erase size: 4096
	Hello World!

	
## Build the mbed OS Test Cases

If you have completed the previous section "Simple SPIFBlockDevice Example" then first prepare the environment by removing the BUILD
directory and hiding or removing the example2.cpp:

    simhug01@E107851:/d/demo_area/ex_app2$ rm -fR BUILD
    simhug01@E107851:/d/demo_area/ex_app2$ cp example2.cpp example2_cpp
    
Build the test cases for the K64F target using the following command:

    simhug01@E107851:/d/demo_area/ex_app2$ mbed -v test --compile -t GCC_ARM -m K64F 2>&1 | tee build_tests_gcc_20170322_1007.txt
    <trace removed>
    simhug01@E107851:/d/demo_area/ex_app2$

The build trace is quite extensive but on a successful build you should see the following output at the end of the log:

  	Build successes:
	  * K64F::GCC_ARM::MBED-BUILD
	  * K64F::GCC_ARM::MBED-OS-FEATURES-FEATURE_LWIP-TESTS-MBEDMICRO-NET-CONNECTIVITY
	  <trace removed>
	  * K64F::GCC_ARM::MBED-OS-FEATURES-TESTS-FILESYSTEM-FAT_FILE_SYSTEM
	  * K64F::GCC_ARM::MBED-OS-FEATURES-TESTS-FILESYSTEM-HEAP_BLOCK_DEVICE
	  * K64F::GCC_ARM::MBED-OS-FEATURES-TESTS-FILESYSTEM-UTIL_BLOCK_DEVICE
	  <trace removed>
	  * K64F::GCC_ARM::MBED-OS-TESTS-STORAGE_ABSTRACTION-BASICAPI
	  * K64F::GCC_ARM::SPIFLASH-DRIVER-TESTS-BLOCK_DEVICE-SPIF
	
	
	Build skips:
	  * K64F::GCC_ARM::MBED-OS-FEATURES-FEATURE_LWIP-TESTS-MBEDMICRO-NET-TCP_PACKET_PRESSURE
	  <trace removed>
 
Notice the following test in the spiflash-driver tree listed above:     

- `K64F::GCC_ARM::SPIFLASH-DRIVER-TESTS-BLOCK_DEVICE-SPIF`


The SPIFBlockDevice test case is at following locations in the source code tree:

    /d/demo_area/ex_app2/spiflash-driver/TESTS/block_device/spif/main.cpp

This provides an example of reading and writing data blocks to the block device interface for the SPI NOR part.

Run the test using the following command:

    simhug01@E107851:/d/demo_area/ex_app2$ mbedgt -VS --test-by-names=spiflash-driver-tests-block_device-spif 2>&1 | tee run_test_gcc_20170322_1007.txt

The test output should look similar to the following trace:

	(mx_env1) simhug01@E107851:/d/datastore/public/jobs/yr2017/2278/sdh_dev_mx1/ex_app5$ mbedgt -VS --test-by-names=spiflash-driver-tests-block_device-sp
	if 2>&1 | tee 2278_run_test_ex_app5_br_master_time_20170322_1207_spif.txt
	mbedgt: greentea test automation tool ver. 1.2.5
	mbedgt: using multiple test specifications from current directory!
	        using 'BUILD\tests\K64F\GCC_ARM\test_spec.json'
	mbedgt: detecting connected mbed-enabled devices...
	mbedgt: detected 1 device
	        +---------------+----------------------+-------------+-------------+--------------------------------------------------+
	        | platform_name | platform_name_unique | serial_port | mount_point | target_id                                        |
	        +---------------+----------------------+-------------+-------------+--------------------------------------------------+
	        | K64F          | K64F[0]              | COM46       | E:          | 0240000029304e450023500878a3001df131000097969900 |
	        +---------------+----------------------+-------------+-------------+--------------------------------------------------+
	mbedgt: processing target 'K64F' toolchain 'GCC_ARM' compatible platforms... (note: switch set to --parallel 1)
	        +---------------+----------------------+-------------+-------------+--------------------------------------------------+
	        | platform_name | platform_name_unique | serial_port | mount_point | target_id                                        |
	        +---------------+----------------------+-------------+-------------+--------------------------------------------------+
	        | K64F          | K64F[0]              | COM46:9600  | E:          | 0240000029304e450023500878a3001df131000097969900 |
	        +---------------+----------------------+-------------+-------------+--------------------------------------------------+
	mbedgt: test case filter (specified with -n option)
	        test filtered in 'spiflash-driver-tests-block_device-spif'
	mbedgt: running 1 test for platform 'K64F' and toolchain 'GCC_ARM'
	        use 1 instance of execution threads for testing
	mbedgt: checking for 'host_tests' directory above image directory structure
	        'host_tests' directory not found: two directory levels above image path checked
	mbedgt: selecting test case observer...
	        calling mbedhtrun: mbedhtrun -m K64F -p COM46:9600 -f "BUILD/tests/K64F/GCC_ARM/spiflash-driver/TESTS/block_device/spif/spif.bin" -d E: -C 4 -
	c shell -t 0240000029304e450023500878a3001df131000097969900
	mbedgt: mbed-host-test-runner: started
	[1490184626.50][HTST][INF] host test executor ver. 1.1.6
	[1490184626.50][HTST][INF] copy image onto target...
	[1490184626.50][COPY][INF] Waiting up to 60 sec for '0240000029304e450023500878a3001df131000097969900' mount point (current is 'E:')...
	        1 file(s) copied.
	[1490184635.79][HTST][INF] starting host test process...
	[1490184636.10][CONN][INF] starting connection process...
	[1490184636.10][CONN][INF] notify event queue about extra 60 sec timeout for serial port pooling
	[1490184636.10][CONN][INF] initializing serial port listener...
	[1490184636.10][PLGN][INF] Waiting up to 60 sec for '0240000029304e450023500878a3001df131000097969900' serial port (current is 'COM46')...
	[1490184636.12][HTST][INF] setting timeout to: 60 sec
	[1490184636.24][SERI][INF] serial(port=COM46, baudrate=9600, timeout=0.01)
	<lines deleted to save space>
	[1490184649.90][CONN][INF] found KV pair in stream: {{__testcase_name;Testing read write random blocks}}, queued...
	[1490184649.97][CONN][RXD] >>> Running case #1: 'Testing read write random blocks'...
	[1490184650.02][CONN][INF] found KV pair in stream: {{__testcase_start;Testing read write random blocks}}, queued...
	[1490184650.05][CONN][RXD] read size: 1bytes (1bytes)
	[1490184650.08][CONN][RXD] program size: 1bytes (1bytes)
	[1490184650.12][CONN][RXD] erase size: 4kbytes (4096bytes)
	[1490184650.13][CONN][RXD] total size: 2Mbytes (2097152bytes)
	[1490184650.17][CONN][RXD] test  002d000:4096...
	[1490184650.36][CONN][RXD] write 002d000:4096 aad8573abd84e79e5e3684fa5519aabb...
	[1490184650.50][CONN][RXD] read  002d000:4096 aad8573abd84e79e5e3684fa5519aabb...
	[1490184650.56][CONN][RXD] error 002d000:4096 00000000000000000000000000000000
	[1490184650.58][CONN][RXD] test  0036000:4096...
	[1490184650.77][CONN][RXD] write 0036000:4096 92fc08f5b4113047225a8d3b855e5460...
	[1490184650.91][CONN][RXD] read  0036000:4096 92fc08f5b4113047225a8d3b855e5460...
	[1490184650.97][CONN][RXD] error 0036000:4096 00000000000000000000000000000000
	[1490184650.99][CONN][RXD] test  00c6000:4096...
	[1490184651.16][CONN][RXD] write 00c6000:4096 89a030a34b17ca3545c7b007001ef74f...
	[1490184651.32][CONN][RXD] read  00c6000:4096 89a030a34b17ca3545c7b007001ef74f...
	[1490184651.38][CONN][RXD] error 00c6000:4096 00000000000000000000000000000000
	[1490184651.40][CONN][RXD] test  00da000:4096...
	[1490184651.60][CONN][RXD] write 00da000:4096 446fd0232a3d053af820b69c614b3662...
	[1490184651.73][CONN][RXD] read  00da000:4096 446fd0232a3d053af820b69c614b3662...
	[1490184651.79][CONN][RXD] error 00da000:4096 00000000000000000000000000000000
	[1490184651.81][CONN][RXD] test  0188000:4096...
	[1490184652.00][CONN][RXD] write 0188000:4096 9a36d3c6d4034958cade542a9f1e22c2...
	[1490184652.14][CONN][RXD] read  0188000:4096 9a36d3c6d4034958cade542a9f1e22c2...
	[1490184652.20][CONN][RXD] error 0188000:4096 00000000000000000000000000000000
	[1490184652.21][CONN][RXD] test  015f000:4096...
	[1490184652.42][CONN][RXD] write 015f000:4096 70f83b9cc6713736c60089a0fa55f12d...
	[1490184652.55][CONN][RXD] read  015f000:4096 70f83b9cc6713736c60089a0fa55f12d...
	[1490184652.61][CONN][RXD] error 015f000:4096 00000000000000000000000000000000
	[1490184652.63][CONN][RXD] test  005c000:4096...
	[1490184652.82][CONN][RXD] write 005c000:4096 47a0f043fda26135877bb11c7b7016dc...
	[1490184652.96][CONN][RXD] read  005c000:4096 47a0f043fda26135877bb11c7b7016dc...
	[1490184653.02][CONN][RXD] error 005c000:4096 00000000000000000000000000000000
	[1490184653.04][CONN][RXD] test  0177000:4096...
	[1490184653.24][CONN][RXD] write 0177000:4096 174f13941b6385d4a829f2d066a1e375...
	[1490184653.37][CONN][RXD] read  0177000:4096 174f13941b6385d4a829f2d066a1e375...
	[1490184653.42][CONN][RXD] error 0177000:4096 00000000000000000000000000000000
	[1490184653.45][CONN][RXD] test  0173000:4096...
	[1490184653.65][CONN][RXD] write 0173000:4096 383f0ca8cc86e3225362805329e0d659...
	[1490184653.78][CONN][RXD] read  0173000:4096 383f0ca8cc86e3225362805329e0d659...
	[1490184653.84][CONN][RXD] error 0173000:4096 00000000000000000000000000000000
	[1490184653.86][CONN][RXD] test  01d9000:4096...
	[1490184654.05][CONN][RXD] write 01d9000:4096 73f32decf08112f271131f9837b76f28...
	[1490184654.19][CONN][RXD] read  01d9000:4096 73f32decf08112f271131f9837b76f28...
	[1490184654.24][CONN][RXD] error 01d9000:4096 00000000000000000000000000000000
	[1490184654.31][CONN][INF] found KV pair in stream: {{__testcase_finish;Testing read write random blocks;1;0}}, queued...
	[1490184654.38][CONN][RXD] >>> 'Testing read write random blocks': 1 passed, 0 failed
	[1490184654.38][CONN][RXD]
	[1490184654.41][CONN][RXD] >>> Test cases: 1 passed, 0 failed
	[1490184654.44][CONN][INF] found KV pair in stream: {{__testcase_summary;1;0}}, queued...
	[1490184654.47][CONN][INF] found KV pair in stream: {{max_heap_usage;0}}, queued...
	[1490184654.48][CONN][INF] found KV pair in stream: {{end;success}}, queued...
	[1490184654.48][HTST][ERR] orphan event in main phase: {{max_heap_usage;0}}, timestamp=1490184654.467000
	[1490184654.48][HTST][INF] __notify_complete(True)
	[1490184654.50][CONN][INF] found KV pair in stream: {{__exit;0}}, queued...
	[1490184654.51][HTST][INF] __exit(0)
	[1490184654.52][HTST][INF] __exit_event_queue received
	[1490184654.52][HTST][INF] test suite run finished after 4.75 sec...
	[1490184654.53][CONN][INF] received special even '__host_test_finished' value='True', finishing
	[1490184654.53][HTST][INF] CONN exited with code: 0
	[1490184654.53][HTST][INF] No events in queue
	[1490184654.53][HTST][INF] stopped consuming events
	[1490184654.53][HTST][INF] host test result() call skipped, received: True
	[1490184654.53][HTST][INF] calling blocking teardown()
	[1490184654.53][HTST][INF] teardown() finished
	[1490184654.53][HTST][INF] {{result;success}}
	mbedgt: checking for GCOV data...
	mbedgt: mbed-host-test-runner: stopped and returned 'OK'
	mbedgt: test on hardware with target id: 0240000029304e450023500878a3001df131000097969900
	mbedgt: test suite 'spiflash-driver-tests-block_device-spif' ......................................... OK in 28.42 sec
	        test case: 'Testing read write random blocks' ................................................ OK in 4.29 sec
	mbedgt: test case summary: 1 pass, 0 failures
	mbedgt: all tests finished!
	mbedgt: shuffle seed: 0.0217829158
	mbedgt: test suite report:
	+--------------+---------------+-----------------------------------------+--------+--------------------+-------------+
	| target       | platform_name | test suite                              | result | elapsed_time (sec) | copy_method |
	+--------------+---------------+-----------------------------------------+--------+--------------------+-------------+
	| K64F-GCC_ARM | K64F          | spiflash-driver-tests-block_device-spif | OK     | 28.42              | shell       |
	+--------------+---------------+-----------------------------------------+--------+--------------------+-------------+
	mbedgt: test suite results: 1 OK
	mbedgt: test case report:
	+--------------+---------------+-----------------------------------------+----------------------------------+--------+--------+--------+--------------------+
	| target       | platform_name | test suite                              | test case                        | passed | failed | result | elapsed_time (sec) |
	+--------------+---------------+-----------------------------------------+----------------------------------+--------+--------+--------+--------------------+
	| K64F-GCC_ARM | K64F          | spiflash-driver-tests-block_device-spif | Testing read write random blocks | 1      | 0      | OK     | 4.29               |
	+--------------+---------------+-----------------------------------------+----------------------------------+--------+--------+--------+--------------------+
	mbedgt: test case results: 1 OK
	mbedgt: completed in 35.04 sec
	(mx_env1) simhug01@E107851:/d/datastore/public/jobs/yr2017/2278/sdh_dev_mx1/ex_app5$



# Appendix 2: Getting Started With The I2C EEPROM Driver

Hardware required:

- K64F.
- CI test shield.
- Micro USB cable.

Software required:

- mbed CLI (with all other dependencies installed).
- ARMCC / GCC / IAR compiler.
- mbed greentea.
- git account.

Github repos to use:

- The [mbed OS repository](https://github.com/armmbed/mbed-os) 
- The [I2C EEPROM driver repository](https://github.com/ARMmbed/i2ceeprom-driver.git) 
- The [CI test shield repository](https://github.com/ARMmbed/ci-test-shield.git) for `mbed_app.json` application configuration file. 

Steps to follow:

- Create an empty example project in a suitable directory. Move into it.
- Download mbed OS into the example directory via `mbed new .`
- Add the I2C EEPROM driver via `mbed add i2ceeprom-driver`
- Clone the CI test shield repository to another suitable directory. Copy the mbed_app.json 
  from the CI test shield directory to the top level of the newly created example directory.
- Make sure the I2C pins are SDA on D14 and SCL on D15 and the I2C EEPROM slave address is 
  0xA0 in the mbed_app.json that you just copied.
- Connect the target to the host machine. Run `mbed detect` to make sure the target is detected.
- Now we are ready to run the greentea tests on this target with 
  `mbed test -t ARM -m K64F -n i2ceeprom-driver-tests-block_device-i2cee  -v`
- Note that the greentea test above makes use of the main.cpp supplied in the 
  `TESTS\block_device\i2cee` directory. You can customize this if required or use your own test 
  application via main.cpp. Be sure to have only 1 main(). If using a custom main() then you 
  can either have this in the TESTS directory or at the top level example directory.
- The tests should pass. If not, time to debug!!
- For other targets, please change the target ID string in the test command above to the 
  appropriate one. You can check the supported targets from mbed CLI using `mbed target --supported`.

The output should be like this:


	Building library mbed-build (K64F, ARM)
	Scan: i2c_ex1
	Scan: FEATURE_BLE
	Scan: FEATURE_COMMON_PAL
	Scan: FEATURE_LWIP
	Scan: FEATURE_UVISOR
	Scan: FEATURE_ETHERNET_HOST
	Scan: FEATURE_LOWPAN_BORDER_ROUTER
	Scan: FEATURE_LOWPAN_HOST
	Scan: FEATURE_LOWPAN_ROUTER
	Scan: FEATURE_NANOSTACK
	Scan: FEATURE_NANOSTACK_FULL
	Scan: FEATURE_THREAD_BORDER_ROUTER
	Scan: FEATURE_THREAD_END_DEVICE
	Scan: FEATURE_THREAD_ROUTER
	Scan: FEATURE_STORAGE
	Scan: ARM
	Scan: FEATURE_LWIP
	Scan: FEATURE_STORAGE
	Building project i2cee (K64F, ARM)
	Scan: ARM
	Scan: FEATURE_LWIP
	Scan: FEATURE_STORAGE
	Scan: i2cee
	+-----------+-------+-------+-------+
	| Module    | .text | .data |  .bss |
	+-----------+-------+-------+-------+
	| Misc      | 49473 |   420 | 11628 |
	| Subtotals | 49473 |   420 | 11628 |
	+-----------+-------+-------+-------+
	Allocated Heap: unknown
	Allocated Stack: unknown
	Total Static RAM memory (data + bss): 12048 bytes
	Total RAM memory (data + bss + heap + stack): 12048 bytes
	Total Flash memory (text + data + misc): 49893 bytes
	Image: BUILD/tests/K64F/ARM/i2ceeprom-driver/TESTS/block_device/i2cee/i2cee.bin
	
	
	Memory map breakdown for built projects (values in Bytes):
	+-------+--------+-----------+------------+-------+------+-----------+-------------+
	| name  | target | toolchain | static_ram | stack | heap | total_ram | total_flash |
	+-------+--------+-----------+------------+-------+------+-----------+-------------+
	| i2cee | K64F   | ARM       |      12048 |     0 |    0 |     12048 |       49893 |
	+-------+--------+-----------+------------+-------+------+-----------+-------------+
	
	
	Build successes:
	  * K64F::ARM::I2CEEPROM-DRIVER-TESTS-BLOCK_DEVICE-I2CEE
	  * K64F::ARM::MBED-BUILD
	mbedgt: greentea test automation tool ver. 1.2.5
	mbedgt: test specification file 'C:\Ashok\SiPWorkshop\Filesystem\i2c_ex1\BUILD\tests\K64F\ARM\test_spec.json' (specified with --test-spec option)
	mbedgt: using 'C:\Ashok\SiPWorkshop\Filesystem\i2c_ex1\BUILD\tests\K64F\ARM\test_spec.json' from current directory!
	mbedgt: detecting connected mbed-enabled devices...
	mbedgt: detected 1 device
	mbedgt: processing target 'K64F' toolchain 'ARM' compatible platforms... (note: switch set to --parallel 1)
	mbedgt: test case filter (specified with -n option)
		test filtered in 'i2ceeprom-driver-tests-block_device-i2cee'
	mbedgt: running 1 test for platform 'K64F' and toolchain 'ARM'
	mbedgt: mbed-host-test-runner: started
	mbedgt: checking for GCOV data...
	mbedgt: test on hardware with target id: 0240000034544e45002600048e3800285a91000097969900
	mbedgt: test suite 'i2ceeprom-driver-tests-block_device-i2cee' ....................................... OK in 11.79 sec
		test case: 'Testing read write random blocks' ................................................ OK in 1.23 sec
	mbedgt: test case summary: 1 pass, 0 failures
	mbedgt: all tests finished!
	mbedgt: shuffle seed: 0.1529521449
	mbedgt: test suite report:
	+----------+---------------+-------------------------------------------+--------+--------------------+-------------+
	| target   | platform_name | test suite                                | result | elapsed_time (sec) | copy_method |
	+----------+---------------+-------------------------------------------+--------+--------------------+-------------+
	| K64F-ARM | K64F          | i2ceeprom-driver-tests-block_device-i2cee | OK     | 11.79              | shell       |
	+----------+---------------+-------------------------------------------+--------+--------------------+-------------+
	mbedgt: test suite results: 1 OK
	mbedgt: test case report:
	+----------+---------------+-------------------------------------------+----------------------------------+--------+--------+--------+--------------------+
	| target   | platform_name | test suite                                | test case                        | passed | failed | result | elapsed_time (sec) |
	+----------+---------------+-------------------------------------------+----------------------------------+--------+--------+--------+--------------------+
	| K64F-ARM | K64F          | i2ceeprom-driver-tests-block_device-i2cee | Testing read write random blocks | 1      | 0      | OK     | 1.23               |
	+----------+---------------+-------------------------------------------+----------------------------------+--------+--------+--------+--------------------+
	mbedgt: test case results: 1 OK
	mbedgt: completed in 13.30 sec
