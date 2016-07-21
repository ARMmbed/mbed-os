# Configuration Store Getting Started Guide
Author: Simon Hughes

# Overview

## Executive Summary

This document describes how to get started with the configuration store module. 

## Terminology

The terminology used throughout this document is defined on the [CFSTORE Terminology](configuration_store_terminology.md) page.


# Pre-requisites

It is assumed that you have the following:
- The mbed-cli tool installed on your computer.
- GCC_ARM compiler installed on your computer
- The mbed greentea test suite tool installed on you computer.
- A Freescale FRDM K64F connected to your computer for running the test cases on this target.
  

# Overview of Steps Covered to Build Tests 

The following steps will be covered:
1. Creating a new mbed-os project in for building the test cases
2. Building the test cases
3. Running the test cases on a Freescale FRDM K64F.


## Step 1: Creating a New mbed-os Project

First create an empty working directory e.g. called cfstore. Move into the directoy and perfom the `mbed new .` command:

```
---
C:\Jenkins\jobs\cfstore_mbedos_blinky\workspace>mbed new . 
[mbed WARNING] Directory "." is not empty.
---
[mbed] Creating new program "." (git)
[mbed] Adding library "mbed-os" from "https://github.com/ARMmbed/mbed-os/" at latest revision in the current branch
[mbed] Adding library "mbed-os\core" from "https://github.com/mbedmicro/mbed/" at rev #ce830296d0297a8da543c24134bf859710fd7698
[mbed] Adding library "mbed-os\features\FEATURE_CLIENT\coap-service" from "https://github.com/ARMmbed/coap-service/" at rev #7a11be1ccb07b2ea953a37d6a5445baa9fd380b8
[mbed] Adding library "mbed-os\features\FEATURE_CLIENT\mbed-client-c" from "https://github.com/ARMmbed/mbed-client-c/" at rev #2aae9fd392af75704d6a40219647a71495ffe291
[mbed] Adding library "mbed-os\features\FEATURE_CLIENT\mbed-client-classic" from "https://github.com/ARMmbed/mbed-client-classic/" at rev #30260f355273100e9a2de47e26502d7b6ebe2f33
[mbed] Adding library "mbed-os\features\FEATURE_CLIENT\mbed-client-mbed-tls" from "https://github.com/ARMmbed/mbed-client-mbed-tls/" at rev #f644e7b9c350c92c43caace7f28330416b69ae45
[mbed] Adding library "mbed-os\features\FEATURE_CLIENT\mbed-client-randlib" from "https://github.com/ARMmbed/mbed-client-randlib/" at rev #80f5c491dd4d7c8e30b94eb575cdb402e9a17e83
[mbed] Adding library "mbed-os\features\FEATURE_CLIENT\mbed-client" from "https://github.com/ARMmbed/mbed-client/" at rev #7e09a3bc6a81eeb7032e61412bcfee09b8361132
[mbed] Adding library "mbed-os\features\FEATURE_IPV6\mbed-mesh-api" from "https://github.com/ARMmbed/mbed-mesh-api/" at rev #0e92921f3dcebd73925479156a02a5f40206bd5e
[mbed] Adding library "mbed-os\features\FEATURE_IPV6\mbed-trace" from "https://github.com/ARMmbed/mbed-trace/" at rev #e419c488f4f8079fbd14f90e61b6018308e33d73
[mbed] Adding library "mbed-os\features\FEATURE_IPV6\nanostack-hal-mbed-cmsis-rtos" from "https://github.com/ARMmbed/nanostack-hal-mbed-cmsis-rtos/" at rev #36968fc133c74cb43d95588ae324623fe0d20f78
[mbed] Adding library "mbed-os\features\FEATURE_IPV6\nanostack-libservice" from "https://github.com/ARMmbed/nanostack-libservice/" at rev #f61c845e0c59f9e0c033ae3de2ea4f190b7edef1
[mbed] Adding library "mbed-os\features\FEATURE_IPV6\sal-stack-nanostack-eventloop" from "https://github.com/ARMmbed/sal-stack-nanostack-eventloop/" at rev #c163be9183b09d7513532c50f2bc9ac52d3e580c
[mbed] Adding library "mbed-os\features\FEATURE_IPV6\sal-stack-nanostack-private" from "https://github.com/ARMmbed/sal-stack-nanostack-private/" at rev #5d3365ce7df303b31d88bdddf08d296088edd24d
[mbed] Adding library "mbed-os\frameworks\greentea-client" from "https://github.com/ARMmbed/greentea-client/" at rev #d0cbb41ae7939e1a0b159c110244ceea5ddaca49
[mbed] Adding library "mbed-os\frameworks\unity" from "https://github.com/ARMmbed/unity/" at rev #14fd303f30f9578b0ef6767c88e2fbc262db5fa1
[mbed] Updating reference "mbed-os" -> "https://github.com/ARMmbed/mbed-os/#f4790ce69f273f5138ff23bf628bbe0e3a62b9d2"

```

As shown in the above trace, the mbed new command creates a clone of the mbed-os repository, and the linked repostories. 
It takes some time to execute as the command downloads ~300MB of source files from github.

Once complete, you can see the list of linked repositories by using the mbed ls command:

```
(mx2_venv3) simhug01@E107851:/d/datastore/public/jobs/yr2016/2253/sdh_dev_mx2/cfstore_mbedos_blinky9$ mbed ls
cfstore_mbedos_blinky9 (69e5d843463c)
`- mbed-os (aafb02d090d1)
   |- core (7810829df18e)
   |- features\FEATURE_CLIENT\coap-service (7a11be1ccb07)
   |- features\FEATURE_CLIENT\mbed-client (7e09a3bc6a81)
   |- features\FEATURE_CLIENT\mbed-client-c (2aae9fd392af)
   |- features\FEATURE_CLIENT\mbed-client-classic (30260f355273)
   |- features\FEATURE_CLIENT\mbed-client-mbed-tls (f644e7b9c350)
   |- features\FEATURE_CLIENT\mbed-client-randlib (80f5c491dd4d)
   |- features\FEATURE_IPV6\mbed-mesh-api (0e92921f3dce)
   |- features\FEATURE_IPV6\mbed-trace (e419c488f4f8)
   |- features\FEATURE_IPV6\nanostack-hal-mbed-cmsis-rtos (36968fc133c7)
   |- features\FEATURE_IPV6\nanostack-libservice (f61c845e0c59)
   |- features\FEATURE_IPV6\sal-stack-nanostack-eventloop (c163be9183b0)
   |- features\FEATURE_IPV6\sal-stack-nanostack-private (5d3365ce7df3)
   |- frameworks\greentea-client (d0cbb41ae793)
   `- frameworks\unity (14fd303f30f9)
(mx2_venv3) simhug01@E107851:/d/datastore/public/jobs/yr2016/2253/sdh_dev_mx2/cfstore_mbedos_blinky9$
```

## Step 2: Build the Test Cases

Next, build the mbed-os test with the following command line:


```
C:\Jenkins\jobs\cfstore_mbedos_blinky\workspace>mbed compile --tests -m K64F -t GCC_ARM 
Building library mbed-build (K64F, GCC_ARM)
Copy: AnalogIn.h
Copy: AnalogOut.h
Copy: BusIn.h
Copy: BusInOut.h
Copy: BusOut.h
Copy: Callback.h
Copy: CallChain.h
Copy: CAN.h
Copy: can_helper.h
Copy: CircularBuffer.h
Copy: critical.h
Copy: CThunk.h
Copy: DigitalIn.h
Copy: DigitalInOut.h
Copy: DigitalOut.h
Copy: DirHandle.h
Copy: Ethernet.h
Copy: FileBase.h
... 
<lines deleted to save space>
...
Building project GCC_ARM to mbed-os-features-storage-tests-cfstore-flush2 (K64F, GCC_ARM)
Compile: flush2.cpp
Link: mbed-os-features-storage-tests-cfstore-flush2
Elf2Bin: mbed-os-features-storage-tests-cfstore-flush2
+----------------------------+-------+-------+------+
| Module                     | .text | .data | .bss |
+----------------------------+-------+-------+------+
| Fill                       |    95 |     7 | 2090 |
| Misc                       | 43411 |  2224 | 2152 |
| core/hal                   | 19188 |    16 |  795 |
| core/rtos                  |  6815 |    24 | 2662 |
| features/FEATURE_IPV4      |    96 |     0 |   48 |
| features/storage           | 15578 |   340 |  576 |
| frameworks/greentea-client |  1580 |    56 |   52 |
| frameworks/unity           |   584 |     0 |  160 |
| frameworks/utest           |  5444 |     5 |  805 |
| Subtotals                  | 92791 |  2672 | 9340 |
+----------------------------+-------+-------+------+
Allocated Heap: 65540 bytes
Allocated Stack: 32768 bytes
Total Static RAM memory (data + bss): 12012 bytes
Total RAM memory (data + bss + heap + stack): 110320 bytes
Total Flash memory (text + data + misc): 96503 bytes
Image: .build/tests/K64F/GCC_ARM/mbed-os/features/storage/TESTS/cfstore/flush2/mbed-os-features-storage-tests-cfstore-flush2.bin
Building project GCC_ARM to mbed-os-features-storage-tests-cfstore-flush3 (K64F, GCC_ARM)
Compile: flush3.cpp
Link: mbed-os-features-storage-tests-cfstore-flush3
Elf2Bin: mbed-os-features-storage-tests-cfstore-flush3
+----------------------------+-------+-------+------+
| Module                     | .text | .data | .bss |
+----------------------------+-------+-------+------+
| Fill                       |    99 |     7 | 2238 |
| Misc                       | 43095 |  2224 | 2152 |
| core/hal                   | 19188 |    16 |  795 |
| core/rtos                  |  6815 |    24 | 2662 |
| features/FEATURE_IPV4      |    96 |     0 |   48 |
| features/storage           | 13598 |   340 |  428 |
| frameworks/greentea-client |  1580 |    56 |   52 |
| frameworks/unity           |   584 |     0 |  160 |
| frameworks/utest           |  5312 |     5 |  805 |
| Subtotals                  | 90367 |  2672 | 9340 |
+----------------------------+-------+-------+------+
Allocated Heap: 65540 bytes
Allocated Stack: 32768 bytes
Total Static RAM memory (data + bss): 12012 bytes
Total RAM memory (data + bss + heap + stack): 110320 bytes
Total Flash memory (text + data + misc): 94079 bytes
Image: .build/tests/K64F/GCC_ARM/mbed-os/features/storage/TESTS/cfstore/flush3/mbed-os-features-storage-tests-cfstore-flush3.bin

<lines deleted to save space>


Building project GCC_ARM to mbed-os-tests-mbedmicro-rtos-mbed-semaphore (K64F, GCC_ARM)
Compile: main.cpp
[Warning] main.cpp@72: 'rtos::Thread::Thread(void (*)(const void*), void*, osPriority, uint32_t, unsigned char*)' is deprecated (declared at C:/Jenkins/jobs/cfstore_mbedos_blinky/workspace/.build/tests/K64F/GCC_ARM/mbed-os/core/rtos/rtos/Thread.h:138): Thread-spawning constructors hide errors and may lead to complex program state when a thread is declared [-Wdeprecated-declarations]
[Warning] main.cpp@73: 'rtos::Thread::Thread(void (*)(const void*), void*, osPriority, uint32_t, unsigned char*)' is deprecated (declared at C:/Jenkins/jobs/cfstore_mbedos_blinky/workspace/.build/tests/K64F/GCC_ARM/mbed-os/core/rtos/rtos/Thread.h:138): Thread-spawning constructors hide errors and may lead to complex program state when a thread is declared [-Wdeprecated-declarations]
[Warning] main.cpp@74: 'rtos::Thread::Thread(void (*)(const void*), void*, osPriority, uint32_t, unsigned char*)' is deprecated (declared at C:/Jenkins/jobs/cfstore_mbedos_blinky/workspace/.build/tests/K64F/GCC_ARM/mbed-os/core/rtos/rtos/Thread.h:138): Thread-spawning constructors hide errors and may lead to complex program state when a thread is declared [-Wdeprecated-declarations]
Link: mbed-os-tests-mbedmicro-rtos-mbed-semaphore
Elf2Bin: mbed-os-tests-mbedmicro-rtos-mbed-semaphore
+----------------------------+-------+-------+------+
| Module                     | .text | .data | .bss |
+----------------------------+-------+-------+------+
| Fill                       |    96 |     4 | 2232 |
| Misc                       | 42479 |  2224 | 2152 |
| TESTS/mbedmicro-rtos-mbed  |   459 |     0 |   25 |
| core/hal                   | 16055 |    16 |  565 |
| core/rtos                  |  7662 |    24 | 2662 |
| features/FEATURE_IPV4      |    96 |     0 |   48 |
| frameworks/greentea-client |  1580 |    52 |   52 |
| frameworks/utest           |  2988 |     0 |  732 |
| Subtotals                  | 71415 |  2320 | 8468 |
+----------------------------+-------+-------+------+
Allocated Heap: 65540 bytes
Allocated Stack: 32768 bytes
Total Static RAM memory (data + bss): 10788 bytes
Total RAM memory (data + bss + heap + stack): 109096 bytes
Total Flash memory (text + data + misc): 74775 bytes
Image: .build/tests/K64F/GCC_ARM/mbed-os/TESTS/mbedmicro-rtos-mbed/semaphore/mbed-os-tests-mbedmicro-rtos-mbed-semaphore.bin


Build successes:
  * K64F::GCC_ARM::MBED-BUILD
  * K64F::GCC_ARM::MBED-OS-FEATURES-FEATURE_IPV4-TESTS-MBEDMICRO-NET-NIST_INTERNET_TIME_SERVICE
  * K64F::GCC_ARM::MBED-OS-FEATURES-FEATURE_IPV4-TESTS-MBEDMICRO-NET-TCP_CLIENT_ECHO
  * K64F::GCC_ARM::MBED-OS-FEATURES-FEATURE_IPV4-TESTS-MBEDMICRO-NET-TCP_CLIENT_HELLO_WORLD
  * K64F::GCC_ARM::MBED-OS-FEATURES-FEATURE_IPV4-TESTS-MBEDMICRO-NET-UDP_ECHO_CLIENT
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-ADD_DEL
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-CLOSE
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-CREATE
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-DUMP
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-EXAMPLE1
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-EXAMPLE2
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-EXAMPLE3
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-EXAMPLE4
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-EXAMPLE5
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-FIND
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-FIND2
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-FLASH
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-FLASH_SET
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-FLUSH
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-FLUSH2
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-FLUSH3
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-INIT
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-MISC
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-OPEN
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-READ
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-CFSTORE-WRITE
  * K64F::GCC_ARM::MBED-OS-FEATURES-STORAGE-TESTS-FLASH_JOURNAL-BASICAPI
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-BASIC_TEST
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-BASIC_TEST_DEFAULT
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-CASE_ASYNC_VALIDATE
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-CASE_CONTROL_ASYNC
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-CASE_CONTROL_REPEAT
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-CASE_SELECTION
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-CASE_SETUP_FAILURE
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-CASE_TEARDOWN_FAILURE
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-CONTROL_TYPE
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-MINIMAL_ASYNC_SCHEDULER
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-MINIMAL_SCHEDULER
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-TEST_ASSERTION_FAILURE_TEST_SETUP
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-TEST_SETUP_CASE_SELECTION_FAILURE
  * K64F::GCC_ARM::MBED-OS-FRAMEWORKS-UTEST-TESTS-UNIT_TESTS-TEST_SETUP_FAILURE
  * K64F::GCC_ARM::MBED-OS-TESTS-INTEGRATION-BASIC
  * K64F::GCC_ARM::MBED-OS-TESTS-INTEGRATION-THREADED_BLINKY
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-MBED-ATTRIBUTES
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-MBED-CALL_BEFORE_MAIN
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-MBED-CPP
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-MBED-DIV
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-RTOS-MBED-BASIC
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-RTOS-MBED-ISR
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-RTOS-MBED-MAIL
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-RTOS-MBED-MUTEX
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-RTOS-MBED-QUEUE
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-RTOS-MBED-SEMAPHORE
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-RTOS-MBED-SIGNALS
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-RTOS-MBED-THREADS
  * K64F::GCC_ARM::MBED-OS-TESTS-MBEDMICRO-RTOS-MBED-TIMER
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-CALLBACK
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-C_STRINGS
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-DEV_NULL
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-ECHO
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-GENERIC_TESTS
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-RTC
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-STL_FEATURES
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-TICKER
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-TICKER_2
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-TICKER_3
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-TIMEOUT
  * K64F::GCC_ARM::MBED-OS-TESTS-MBED_DRIVERS-WAIT_US
  * K64F::GCC_ARM::MBED-OS-TESTS-STORAGE_ABSTRACTION-BASICAPI

<lines deleted to save space>
```

## Step 3: Run the Test Cases

Next run the mbed-os Tests usings the following command line

```
C:\Jenkins\jobs\cfstore_mbedos_blinky\workspace>mbedgt -VS --test-spec=.build/tests/K64F/GCC_ARM/test_spec.json 
mbedgt: test specification file '.build/tests/K64F/GCC_ARM/test_spec.json' (specified with --test-spec option)
mbedgt: using '.build/tests/K64F/GCC_ARM/test_spec.json' from current directory!
mbedgt: detecting connected mbed-enabled devices...
mbedgt: detected 1 device
	detected 'K64F' -> 'K64F[0]', console at 'COM64', mounted at 'E:', target id '0240000029304e450014500878a3003cf131000097969900'
mbedgt: processing target 'K64F' toolchain 'GCC_ARM' compatible platforms...
mbedgt: using platform 'K64F' for test:
	target_id_mbed_htm = '0240000029304e450014500878a3003cf131000097969900'
	daplink_interface_crc = '0xdc4d98a8'
	mount_point = 'E:'
	daplink_automation_allowed = '0'
	daplink_daplink_mode = 'Interface'
	daplink_local_mods = '0'
	daplink_unique_id = '0240000029304e450014500878a3003cf131000097969900'
	daplink_hif_id = '97969900'
	target_id = '0240000029304e450014500878a3003cf131000097969900'
	serial_port = 'COM64:9600'
	target_id_usb_id = '0240000029304e450014500878a3003cf131000097969900'
	platform_name = 'K64F'
	platform_name_unique = 'K64F[0]'
	daplink_git_sha = '3c733e851ea6d453b6834232466f7d2b17fff1b6'
	daplink_interface_version = '0240'
	daplink_auto_reset = '0'
	daplink_usb_interfaces = 'MSD, CDC, HID'
	daplink_version = '0240'
mbedgt: running 68 tests for platform 'K64F' and toolchain 'GCC_ARM'
	use 1 instance for testing
mbedgt: selecting test case observer...
	calling mbedhtrun: mbedhtrun -d E: -p COM64:9600 -f ".build/tests/K64F/GCC_ARM/mbed-os/features/storage/TESTS/cfstore/flush/mbed-os-features-storage-tests-cfstore-flush.bin" -C 4 -c shell -m K64F -t 0240000029304e450014500878a3003cf131000097969900
mbedgt: mbed-host-test-runner: started
[1467848528.72][HTST][INF] host test executor ver. 0.2.13
[1467848528.72][HTST][INF] copy image onto target...
        1 file(s) copied.
Plugin info: HostTestPluginCopyMethod_Shell::CopyMethod: Waiting up to 60 sec for '0240000029304e450014500878a3003cf131000097969900' mount point (current is 'E:')...
[1467848539.87][HTST][INF] starting host test process...
[1467848540.48][CONN][INF] starting serial connection process...
[1467848540.48][CONN][INF] notify event queue about extra 60 sec timeout for serial port pooling
[1467848540.48][CONN][INF] initializing serial port listener...
[1467848540.48][SERI][INF] serial(port=COM64, baudrate=9600, timeout=0)
[1467848540.48][HTST][INF] setting timeout to: 60 sec
Plugin info: HostTestPluginBase::BasePlugin: Waiting up to 60 sec for '0240000029304e450014500878a3003cf131000097969900' serial port (current is 'COM64')...
[1467848540.64][SERI][INF] reset device using 'default' plugin...
[1467848540.90][SERI][INF] wait for it...
[1467848541.91][CONN][INF] sending preamble 'eb507531-ae47-4c98-852b-c16efa1b2e4f'...
[1467848541.91][SERI][TXD] mbedmbedmbedmbedmbedmbedmbedmbedmbedmbed
[1467848541.91][SERI][TXD] {{__sync;eb507531-ae47-4c98-852b-c16efa1b2e4f}}
[1467848542.93][CONN][RXD] {{__sync;eb507531-ae47-4c98-852b-c16efa1b2e4f}}
[1467848542.93][CONN][RXD] {{__version;1.3.0}}
[1467848542.93][CONN][RXD] {{__timeout;60}}
[1467848542.93][CONN][RXD] {{__host_test_name;default_auto}}
[1467848542.93][CONN][RXD] {{__testcase_count;2}}
[1467848542.93][CONN][RXD] >>> Running 2 test cases...
[1467848542.93][CONN][RXD] {{__testcase_name;FLUSH_test_00}}
[1467848542.93][CONN][RXD] {{__testcase_name;FLUSH_test_01}}
[1467848542.93][CONN][RXD] >>> Running case #1: 'FLUSH_test_00'...
[1467848542.93][CONN][RXD] {{__testcase_start;FLUSH_test_00}}
[1467848542.93][CONN][RXD] INITIALIZING: caps.asynchronous_ops=1
[1467848542.93][CONN][RXD] CFSTORE Flash Entries
[1467848542.93][CONN][RXD] =====================
[1467848542.93][CONN][RXD] .
[1467848542.93][CONN][RXD]   caps.asynchronous_ops : 1
[1467848542.93][CONN][RXD] .
[1467848542.93][CONN][RXD] == End ==============
[1467848542.93][CONN][RXD] {{__testcase_finish;FLUSH_test_00;1;0}}
[1467848542.93][CONN][RXD] >>> 'FLUSH_test_00': 1 passed, 0 failed
[1467848542.93][CONN][RXD] >>> Running case #2: 'FLUSH_test_01'...
[1467848542.93][CONN][RXD] {{__testcase_start;FLUSH_test_01}}
[1467848542.93][CONN][INF] found SYNC in stream: {{__sync;eb507531-ae47-4c98-852b-c16efa1b2e4f}}, queued...
[1467848542.93][HTST][INF] sync KV found, uuid=eb507531-ae47-4c98-852b-c16efa1b2e4f, timestamp=1467848542.929000
[1467848542.93][CONN][INF] found KV pair in stream: {{__version;1.3.0}}, queued...
[1467848542.93][HTST][INF] DUT greentea-client version: 1.3.0
[1467848542.93][CONN][INF] found KV pair in stream: {{__timeout;60}}, queued...
[1467848542.93][HTST][INF] setting timeout to: 60 sec

```

At the end of the test run, you should see the following from the results summary table:

```
mbedgt: test suite report:
+--------------+---------------+------------------------------------------------------------------------------+---------+--------------------+-------------+
| target       | platform_name | test suite                                                                   | result  | elapsed_time (sec) | copy_method |
+--------------+---------------+------------------------------------------------------------------------------+---------+--------------------+-------------+
<lines deleted to save space>
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-add_del                               | OK      | 16.87              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-close                                 | OK      | 15.91              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-create                                | OK      | 20.09              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-dump                                  | OK      | 14.72              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-example1                              | OK      | 15.94              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-example2                              | OK      | 14.8               | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-example3                              | OK      | 15.76              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-example4                              | OK      | 14.64              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-example5                              | OK      | 14.59              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-find                                  | OK      | 17.11              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-find2                                 | OK      | 15.67              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-flash                                 | OK      | 15.91              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-flash_set                             | OK      | 14.67              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-flush                                 | OK      | 18.14              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-flush2                                | OK      | 16.85              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-flush3                                | OK      | 14.57              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-init                                  | OK      | 15.75              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-misc                                  | OK      | 16.92              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-open                                  | OK      | 19.13              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-read                                  | OK      | 15.73              | shell       |
| K64F-GCC_ARM | K64F          | mbed-os-features-storage-tests-cfstore-write                                 | OK      | 15.75              | shell       |
<lines deleted to save space>
+--------------+---------------+------------------------------------------------------------------------------+---------+--------------------+-------------+
mbedgt: test suite results: 64 OK / 4 TIMEOUT
mbedgt: test case report:
...
<lines deleted to save space>
...
```

# CFSTORE Build Configurations

CFSTORE can be configured using compilation options to build in the following configurations:
- CFSTORE SRAM-backed in-memory version. 
- CFSTORE Non-Volatile-Storage-backed (Flash-Journal) for persisting key-value attributes.

## CFSTORE SRAM-backed in-memory Version

The CFSTORE SRAM version can be described as follows:
- CFSTORE API implementation backed by SRAM (non-persistent).
- Synchronous mode implementation.
- Supported on the K64F target.

Use the following command line to build an application with SRAM support:

```
	mbed compile -v -m K64F -t GCC_ARM -DCFSTORE_STORAGE_DISABLE=0
```

Use the following command line to build the CFSTORE tests with SRAM support:

```
	mbed compile -v --tests -m K64F -t GCC_ARM -DCFSTORE_STORAGE_DISABLE=0
```

## CFSTORE Non-Volatile-Storage-backed (Flash-Journal) Version

The CFSTORE Flash-Journal Version can be described as follows:
- CFSTORE API implementation backed by SRAM and Non-Volatile (NV) storage for persisting key-value attributes.
- Flash-jounral asynchronous mode support.
- Flash-jounral synchronous mode support.
- Flash-jounral supported on the K64F target.


Use the following command line to build an application with Flash-Journal asynchronous mode support:

```
	mbed compile -v -m K64F -t GCC_ARM
```

Use the following command line to build an application with Flash-Journal synchronous mode support:

```
	mbed compile -v --tests -m K64F -t GCC_ARM -DSTORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS=0
```

Use the following command line to build the CFSTORE tests with Flash-Journal asynchronous mode support:

```
	mbed compile -v --tests -m K64F -t GCC_ARM
```

Use the following command line to build the CFSTORE tests with Flash-Journal synchronous mode support:

```
	mbed compile -v --tests -m K64F -t GCC_ARM -DSTORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS=0
```


# CFSTORE Code Examples

There are a number of code examples to help illustrate how to use the CFSTORE API.
* The CFSTORE Client Example 3 for flash-journal synchronous mode only (simpler code). 
  See the explantory comments in example3.cpp for more details. 
* The CFSTORE Client Example 1 for both flash-journal modes (asynchronous and synchronous)
  (more complicated but versatile code). See the explantory comments in example1.cpp for more details.
* The CFSTORE Client Example 5 for flash-journal synchronous mode only (simpler code) alternative example. 
  See the explantory comments in example5.cpp for more details. 

Instructions are included in the exampleX.cpp files for building as stand-alone applications. 
