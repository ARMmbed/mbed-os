# Filesystem integration tests
---
Run integration type of tests for storage and filesystem with Greentea test framework.

Filesystem stress tests:

* Use default Block Device and Filesystem
   	* Supporting currently devices with external Flash or SD
* Use mbed OS POSIX API to write file into device storage using 1B, 512B, 1K, 2K, 4K, 8K, 16K and 32KiB buffers.
* Write a large file to filesystem with underlying storage and read it back again. 
* Tests that filesystem works with large files.
   
### Usage
Tests are executed by default in a extended test set.
`-DMBED_EXTENDED_TESTS` compiler argument flag is needed to compile and run the tests. 

#### Compile and run:   
  Extended test set: `mbed test --compile -m TARGET -t TOOLCHAIN  -DMBED_EXTENDED_TESTS -n "*stress*"`
 
Run tests with Greentea: `mbedgt -vV`

#### Configuration
Tests are configured with `mbed_lib.json` file located at root of stress test directory.
###### File size
Test directory contains few text files which have different sizes. The files contain random generated Lorem Ipsum sentences. In tests, the files are used for writing into filesystem. The default file used for all targets is defined with key `"protagonist-file"`. The file selection can be further defined specifically for each harware board in `mbed_lib.json` target configuration.

### Examples

_Extended_ test set enabled for K66F target: 

	mbed test --compile -m K66F -t GCC_ARM -DMBED_EXTENDED_TESTS -n "*stress*"
	mbedgt -vV

```
mbedgt: test case report:
+--------------+---------------+--------------------------+----------------+--------+--------+--------+--------------------+
| target       | platform_name | test suite               | test case      | passed | failed | result | elapsed_time (sec) |
+--------------+---------------+--------------------------+----------------+--------+--------+--------+--------------------+
| K66F-GCC_ARM | K66F          | tests-integration-stress | Format storage | 1      | 0      | OK     | 13.15              |
| K66F-GCC_ARM | K66F          | tests-integration-stress | story     1    | 1      | 0      | OK     | 34.76              |
| K66F-GCC_ARM | K66F          | tests-integration-stress | story   512    | 1      | 0      | OK     | 26.88              |
| K66F-GCC_ARM | K66F          | tests-integration-stress | story  1k      | 1      | 0      | OK     | 27.13              |
| K66F-GCC_ARM | K66F          | tests-integration-stress | story  2k      | 1      | 0      | OK     | 22.12              |
| K66F-GCC_ARM | K66F          | tests-integration-stress | story  4k      | 1      | 0      | OK     | 19.74              |
| K66F-GCC_ARM | K66F          | tests-integration-stress | story  8k      | 1      | 0      | OK     | 18.52              |
| K66F-GCC_ARM | K66F          | tests-integration-stress | story 16k      | 1      | 0      | OK     | 17.88              |
| K66F-GCC_ARM | K66F          | tests-integration-stress | story 32k      | 1      | 0      | OK     | 17.27              |

```