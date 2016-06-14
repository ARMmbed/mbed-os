# Configuration Store Getting Started
Author: Simon Hughes

# Overview

## Executive Summary

This document describes how to get started with the configuration store module.

## Terminology

The terminology used throughout this document is defined on the [CFSTORE Terminology](https://github.com/ARMmbed/configuration-store/blob/master/doc/design/configuration_store_terminology.md) page.


# CFSTORE : Getting started

This is the link to the configuration store github repo, with links to further documentation from the top level readme.md is [here.](https://github.com/ARMmbed/configuration-store/README.md)

The documentation explains how the API works. There are also a number of test cases in the repo which can be used as examples of how to use the API.

This is the project plan is [here.](https://github.com/ARMmbed/configuration-store/blob/master/doc/design/configuration_store_project.md)

* Milestone 1 (SRAM version) has been released and tagged MBED_V_0_1_0. 
* Milestone 2 (CFSTORE Flash Journal Integration) has been released and tagged MBED_V_0_2_0. 


# CFSTORE : Building the Module

First take a clone of the configuration store github repository from the tag of the last release. For example: 

```
	simhug01@E107851:/d/sdh_dev$ git clone git@github.com:ARMmbed/configuration-store.git
```

Then cd into the configuration-store sub-dir and get a list of the tags

```
	simhug01@E107851:/d/sdh_dev$ cd configuration-store
	simhug01@E107851:/d/sdh_dev/configuration-store $ git tag -l
	MBED_V_0_1_0
	MBED_V_0_2_0
```

The current release is MBED_V_0_2_0 (CFSTORE-flash journal integrated version). Checkout this version for evaluation:

```
	simhug01@E107851:/d/sdh_dev/configuration-store $ git checkout tags/MBED_V_0_2_0
```

configuration-store depends on modules published in the private yotta registry and therefore you need access to this registry. To login to the yotta private use the following command:
 
```
	simhug01@E107851:/d/sdh_dev/configuration-store $ yotta login --registry https://yotta-private.herokuapp.com --apikey=873_b8isfdns3684_327t2evut3r
```


Set the yotta target:

```
	simhug01@E107851:/d/sdh_dev/configuration-store $ yt target frdm-k64f-gcc
```

Now install the yotta dependencies (which will require access to the yotta private registry to succeed):

```
	simhug01@E107851:/d/sdh_dev/configuration-store $ yt install
```

You should see something like the following for the output to yotta list, which shows the CFSTORE module dependency graph:

```
	simhug01@E107851:/d/datastore/public/jobs/yr2016/2247/sdh_dev_8/configuration-store$  yt list
	configuration-store 0.2.0
	|_ mbed-drivers 1.5.0
	| |_ mbed-hal 1.3.0 yotta_modules\mbed-hal
	| | \_ mbed-hal-freescale 1.1.0 yotta_modules\mbed-hal-freescale
	| |   \_ mbed-hal-ksdk-mcu 1.3.0 yotta_modules\mbed-hal-ksdk-mcu
	| |     |_ uvisor-lib 2.1.2 yotta_modules\uvisor-lib
	| |     \_ mbed-hal-k64f 1.3.0 yotta_modules\mbed-hal-k64f
	| |       \_ mbed-hal-frdm-k64f 1.1.1 yotta_modules\mbed-hal-frdm-k64f
	| |_ cmsis-core 1.2.0 yotta_modules\cmsis-core
	| | \_ cmsis-core-freescale 1.1.0 yotta_modules\cmsis-core-freescale
	| |   \_ cmsis-core-k64f 1.1.0 yotta_modules\cmsis-core-k64f
	| |_ ualloc 1.3.0 yotta_modules\ualloc
	| | \_ dlmalloc 1.1.0 yotta_modules\dlmalloc
	| |_ minar 1.3.0 yotta_modules\minar
	| | \_ minar-platform 1.1.0 yotta_modules\minar-platform
	| |   \_ minar-platform-mbed 1.3.0 yotta_modules\minar-platform-mbed
	| |_ core-util 1.8.0 yotta_modules\core-util
	| \_ compiler-polyfill 1.3.0 yotta_modules\compiler-polyfill
	|_ flash-journal-strategy-sequential 0.3.0
	| \_ flash-journal 0.3.0 yotta_modules\flash-journal
	|   \_ flash-abstraction 0.3.0 yotta_modules\flash-abstraction
	|     \_ mtd-k64f 0.3.0 yotta_modules\mtd-k64f
	|_ unity 2.2.0 (test dependency)
	| \_ utest 1.12.2 yotta_modules\utest
	\_ greentea-client 0.1.8 (test dependency)
	
```

Now build configuration-store:

```
	simhug01@E107851:/d/sdh_dev/configuration-store $ yt build
```

The above command builds CFSTORE with support for asynchronous mode flash-journal. If you would like synchronous mode support then use 
the following build command: 

```
	simhug01@E107851:/d/sdh_dev/configuration-store $ yotta --config='{"config":{"hardware":{"mtd":{"async":{"ops":0}}}}}' build
```

Having built successfully, you can now test with the following mbed-greentea command:

```
	simhug01@E107851:/d/sdh_dev/configuration-store $ mbedgt -V
```

The test results should be similar to the test results form the [Jenkins CFSTORE Build and Test Job](http://e108747.cambridge.arm.com:8080/job/configuration-store-nightly-build-and-test/) the summary of which is provided below for reference:

```
mbedgt: test suite report:
+---------------+---------------+----------------------------------+--------+--------------------+-------------+
| target        | platform_name | test suite                       | result | elapsed_time (sec) | copy_method |
+---------------+---------------+----------------------------------+--------+--------------------+-------------+
| frdm-k64f-gcc | K64F          | configuration-store-test-acl     | OK     | 10.26              | shell       |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | OK     | 20.78              | shell       |
| frdm-k64f-gcc | K64F          | configuration-store-test-close   | OK     | 13.02              | shell       |
| frdm-k64f-gcc | K64F          | configuration-store-test-create  | OK     | 12.8               | shell       |
| frdm-k64f-gcc | K64F          | configuration-store-test-find    | OK     | 24.31              | shell       |
| frdm-k64f-gcc | K64F          | configuration-store-test-flash   | OK     | 11.27              | shell       |
| frdm-k64f-gcc | K64F          | configuration-store-test-flush   | OK     | 12.51              | shell       |
| frdm-k64f-gcc | K64F          | configuration-store-test-flush2  | OK     | 12.53              | shell       |
| frdm-k64f-gcc | K64F          | configuration-store-test-misc    | OK     | 14.64              | shell       |
| frdm-k64f-gcc | K64F          | configuration-store-test-open    | OK     | 14.5               | shell       |
| frdm-k64f-gcc | K64F          | configuration-store-test-read    | OK     | 12.78              | shell       |
| frdm-k64f-gcc | K64F          | configuration-store-test-write   | OK     | 12.88              | shell       |
+---------------+---------------+----------------------------------+--------+--------------------+-------------+
mbedgt: test suite results: 12 OK
mbedgt: test case report:
+---------------+---------------+----------------------------------+------------------------------+--------+--------+--------+--------------------+
| target        | platform_name | test suite                       | test case                    | passed | failed | result | elapsed_time (sec) |
+---------------+---------------+----------------------------------+------------------------------+--------+--------+--------+--------------------+
| frdm-k64f-gcc | K64F          | configuration-store-test-acl     | CFSTORE_ACL_test_01          | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_01              | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_02              | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_03              | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_04              | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_05              | 1      | 0      | OK     | 0.11               |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_06              | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_07_end          | 1      | 0      | OK     | 0.25               |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_07_start        | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_08_end          | 1      | 0      | OK     | 0.54               |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_08_start        | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_09_end          | 1      | 0      | OK     | 4.94               |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_09_start        | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-add_del | ADD_DEL_test_10              | 1      | 0      | OK     | 0.11               |
| frdm-k64f-gcc | K64F          | configuration-store-test-close   | CLOSE_test_01_end            | 1      | 0      | OK     | 0.51               |
| frdm-k64f-gcc | K64F          | configuration-store-test-close   | CLOSE_test_01_start          | 1      | 0      | OK     | 0.04               |
| frdm-k64f-gcc | K64F          | configuration-store-test-create  | CREATE_test_01_end           | 1      | 0      | OK     | 0.19               |
| frdm-k64f-gcc | K64F          | configuration-store-test-create  | CREATE_test_01_start         | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-find    | FIND_test_01                 | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-find    | FIND_test_02                 | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-find    | FIND_test_03_end             | 1      | 0      | OK     | 10.53              |
| frdm-k64f-gcc | K64F          | configuration-store-test-find    | FIND_test_03_start           | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-find    | FIND_test_04                 | 1      | 0      | OK     | 0.09               |
| frdm-k64f-gcc | K64F          | configuration-store-test-find    | FIND_test_05_end             | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-find    | FIND_test_05_start           | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-flash   | flash_journal_async_test_01  | 1      | 0      | OK     | 0.22               |
| frdm-k64f-gcc | K64F          | configuration-store-test-flush   | initialize                   | 1      | 0      | OK     | 0.03               |
| frdm-k64f-gcc | K64F          | configuration-store-test-flush2  | CFSTORE_FLUSH2_flush         | 1      | 0      | OK     | 0.17               |
| frdm-k64f-gcc | K64F          | configuration-store-test-flush2  | CFSTORE_FLUSH2_init_on_entry | 1      | 0      | OK     | 0.06               |
| frdm-k64f-gcc | K64F          | configuration-store-test-misc    | MISC_test_00_end             | 1      | 0      | OK     | 0.04               |
| frdm-k64f-gcc | K64F          | configuration-store-test-misc    | MISC_test_00_start           | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-misc    | MISC_test_01                 | 1      | 0      | OK     | 0.04               |
| frdm-k64f-gcc | K64F          | configuration-store-test-misc    | MISC_test_02                 | 1      | 0      | OK     | 0.04               |
| frdm-k64f-gcc | K64F          | configuration-store-test-misc    | MISC_test_03_end             | 1      | 0      | OK     | 0.4                |
| frdm-k64f-gcc | K64F          | configuration-store-test-misc    | MISC_test_03_start           | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-misc    | MISC_test_04_end             | 1      | 0      | OK     | 0.24               |
| frdm-k64f-gcc | K64F          | configuration-store-test-misc    | MISC_test_04_start           | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-misc    | MISC_test_05_end             | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-misc    | MISC_test_05_start           | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-open    | OPEN_test_01_end             | 1      | 0      | OK     | 0.26               |
| frdm-k64f-gcc | K64F          | configuration-store-test-open    | OPEN_test_01_start           | 1      | 0      | OK     | 0.04               |
| frdm-k64f-gcc | K64F          | configuration-store-test-open    | OPEN_test_02_end             | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-open    | OPEN_test_02_start           | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-open    | OPEN_test_100                | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-open    | OPEN_test_101                | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-open    | OPEN_test_102                | 1      | 0      | OK     | 0.09               |
| frdm-k64f-gcc | K64F          | configuration-store-test-open    | OPEN_test_103                | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-open    | OPEN_test_104                | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-open    | OPEN_test_105                | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-read    | READ_test_01_end             | 1      | 0      | OK     | 0.05               |
| frdm-k64f-gcc | K64F          | configuration-store-test-read    | READ_test_01_start           | 1      | 0      | OK     | 0.04               |
| frdm-k64f-gcc | K64F          | configuration-store-test-read    | READ_test_02                 | 1      | 0      | OK     | 0.1                |
| frdm-k64f-gcc | K64F          | configuration-store-test-write   | WRITE_test_01_end            | 1      | 0      | OK     | 0.04               |
| frdm-k64f-gcc | K64F          | configuration-store-test-write   | WRITE_test_01_start          | 1      | 0      | OK     | 0.04               |
| frdm-k64f-gcc | K64F          | configuration-store-test-write   | WRITE_test_02_end            | 1      | 0      | OK     | 0.04               |
| frdm-k64f-gcc | K64F          | configuration-store-test-write   | WRITE_test_02_start          | 1      | 0      | OK     | 0.05               |
+---------------+---------------+----------------------------------+------------------------------+--------+--------+--------+--------------------+
mbedgt: test case results: 56 OK
mbedgt: completed in 177.39 sec
simhug01@E107851:/d/datastore/public/jobs/yr2016/2247/sdh_dev_8/configuration-store$```


# FAQ

## Q: What version has been published in the yotta registry?

A: Version 0.1.0 is the latest version published in the yotta registry. 

## Q: Does Configuration-Store work with mbed OS 16.3?
A: Yes

## Q: Does Configuration-Store work with mbed OS 15.11?
A: Yes


