# Filesystem integration tests
---
Run integration type of tests for storage and filesystem with Greentea test framework.

Filesystem stress tests:

* Use default Block Device and Filesystem
   	* Supporting currently devices with external Flash or SD
* Use mbed OS POSIX API to write file into device storage using 1 byte - 32KiB buffers.
* Write a large file to filesystem with underlying storage and read it back again. 
* Tests that filesystem works with large files.
   
### Usage
Tests are executed with default or extended test set. The default test set is a subset of the extended test cases.
By enabling extended tests with a compiler argument the full test set is executed. 

#### Compile and run:   
  Default test set: `mbed test --compile -m TARGET -t TOOLCHAIN -n "*stress*"`  
  Or extended test set: `mbed test --compile -m TARGET -t TOOLCHAIN  -DMBED_EXTENDED_TESTS -n "*stress*"`
 Run tests with Greentea: `mbedgt -vV`

#### Configuration
Tests are configured with `mbed_lib.json` file located at root of stress test directory.
###### File size
Test directory contains few human readable text book files which have different sizes. In tests, the files are used for writing into filesystem. The default file used for all targets is defined with key `"protagonist-file"`. The file selection can be further defined specifically for each harware board in `mbed_lib.json` target configuration.

### Examples

_Default_ test set enabled for K66F target: 

	mbed test --compile -m K66F -t GCC_ARM -n "*stress*"
	mbedgt -vV

```
mbedgt: test case report:
+--------------+---------------+-------------------------------------+--------------------+--------+--------+--------+--------------------+
| target       | platform_name | test suite                          | test case          | passed | failed | result | elapsed_time (sec) |
+--------------+---------------+-------------------------------------+--------------------+--------+--------+--------+--------------------+
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | Format storage     | 1      | 0      | OK     | 5.97               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story     1        | 1      | 0      | OK     | 6.89               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story  1k          | 1      | 0      | OK     | 5.72               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story 32k          | 1      | 0      | OK     | 3.47               |
```

_Extended_ test set enabled for K66F target: 

	mbed test --compile -m K66F -t GCC_ARM -DMBED_EXTENDED_TESTS -n "*stress*"
	mbedgt -vV

```
mbedgt: test case report:
+--------------+---------------+-------------------------------------+--------------------+--------+--------+--------+--------------------+
| target       | platform_name | test suite                          | test case          | passed | failed | result | elapsed_time (sec) |
+--------------+---------------+-------------------------------------+--------------------+--------+--------+--------+--------------------+
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | Format storage     | 1      | 0      | OK     | 5.97               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story     1        | 1      | 0      | OK     | 6.89               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story   128        | 1      | 0      | OK     | 5.58               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story   256        | 1      | 0      | OK     | 5.74               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story   512        | 1      | 0      | OK     | 5.26               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story  1k          | 1      | 0      | OK     | 5.72               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story  2k          | 1      | 0      | OK     | 4.36               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story  4k          | 1      | 0      | OK     | 4.32               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story  8k          | 1      | 0      | OK     | 3.63               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story 16k          | 1      | 0      | OK     | 3.47               |
| K66F-GCC_ARM | K66F          | tests-stress-filesystem             | story 32k          | 1      | 0      | OK     | 3.47               |
```