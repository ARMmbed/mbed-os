# mbed-stress-test

### Filesystem stress testing:

 * Use mbed OS POSIX API to write file to storage using 1 byte - 32KiB buffers.

 
 Test:
 * Filesystem: 
   * Write a large file to filesystem and read it back again. 
   * Tests filesystem works with large files.
   
### Usage

 * Compile: `mbed test --compile -m TARGET -t TOOLCHAIN --app-config mbed_app.json -n "*stress*"`
 * Run: `mbedgt -vV`
 * Note: 

Example output:
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

### Frequently Asked Questions