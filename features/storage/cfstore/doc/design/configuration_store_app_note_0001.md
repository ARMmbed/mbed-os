# Configuration Store Application Note 0001: NXP Freescale Kinetis FRDM-K64F Flash Memory Usage 
Author: Simon Hughes
Revision: v1.0.0
Date: 7 Jul 2016
DOCID: CFSTORE_APPNOTE_0001_v1_0_0


## Executive Summary

The Configuration-Store (CFSTORE)/Flash-Journal Subsystem can be used to store
application data in the same non-volatile storage medium as the firmware binary image. 
Flash storage is typically composed of 1..n blocks (the FRDM K64F devboard has 2 x 512kB blocks
for example) each with its own support logic. Blocks usually have the limitation of: 
- Prohibiting simultaneous READ and WRITE/ERASE operations in the same flash block, which would otherwise cause
  READ While WRITE (RWW) errors and consequent system faults.
- Taking ~100ms for flash WRITE/ERASE operations of ~4kB in size. 

This document outlines the impact of these limitations on the system as a whole, including: 
- Allowing applications to use both CFSTORE/Flash Journal asynchronous and synchronous modes at the 
  expense of storing CFSTORE data and firmware in separate flash blocks. See Scenario 1 
  discussed in the next section. 
- Limiting applications to CFSTORE/Flash Journal synchronous mode when storing CFSTORE data 
  and firmware in the same flash block. See Scenario 2 below for further details.
- Increasing interrupt latency in Scenario 2 where interrupts have to be disabled to 
  prevent RWW errors.

### Scenario 1: Firmware & CFSTORE Data Stored In Different Flash Blocks

When firmware and CFSTORE data are stored in different flash blocks then: 
- Both CFSTORE/Flash Journal asynchronous or synchronous modes can be used.
- Interrupts are not disabled during flash transactions and 
  interrupt latency is not adversely affected.

### Scenario 2: Firmware & CFSTORE Data Stored the Same Flash Block

When firmware and CFSTORE data are stored in the same flash block then: 
- Only CFSTORE/Flash Journal synchronous mode can be used. Flash Journal busy-waits while flash
  WRITE/ERASE transactions are in-progress.
- Interrupts are disabled when a flash transaction is in-progress causing
  interrupt latency to increase.

### Consequences of Disabling Interrupts

- The disabling of interrupts increases interrupt latency to ~115ms for erasing a 4kB flash sector. 
  The interrupt latency depends on the number of bytes written/erased in the transaction with ~30us/byte
  an indicative metric.
- For applications requiring low interrupt latency 
  (e.g. BLE, 6LowPAN, 3G, LTE communication stacks requiring ~1ms interrupt latency)
  then the application must be cognisant of the impact on
  interrupt latency and perform flash operations at times which will not have a 
  detrimental impact on communication.


## Problem Statement: Freescale Kinetis FRDM-K64F Co-Existence of CFSTORE/Flash-Journal Key-Value Data with Firmware Binary Image in Non-Volatile (Flash) Storage  

A design goal of Configuration-Store/Flash-Journal subsystem is to offer persistence storage of:
- CFSTORE Key Value data items (application data), and 
- the application firmware image

in the *same* non-volatile storage component (e.g. flash).  

To achieve this goal the implementation must ensure READ while WRITE errors are avoided when implementing 
in-software flash-programming applications (e.g. CFSTORE/Flash-Journal) as discussed in the 
[NXP Application Note on Avoiding Read While Write Errors][NXP_FREESCALE_AN4695]: 


> Embedded flash memory is grouped into blocks. Each block contains the 
> circuitry required to read, erase, and program within
> that block. Most of the flash memory technologies have a 
> limitation of not allowing read operations at the same time as an
> erase or program operation is occurring within the same block. 
> Thus, erasing/programming a sector is not allowed if code
> execution (fetching instructions = reading) is taking place within 
> the same block even if the read is in a different flash sector
> than the erase/program. This is called a Read While Write (RWW) 
> violation and will result in a Read Collision error on
> Kinetis and other microcontrollers.


To summarise the above, the CFSTORE/Flash-Journal implementation must not cause 
READ while WRITE (RWW) violations. 

## Freescale FRDM K64 Flash Address Maps for CFSTORE and Firmware Storage

### Scenario 1: Firmware & CFSTORE Data Stored In Different Flash Blocks

```
 -------------------------------- 0x00100000 = 1MB      flash_addr_end      ^
 |                              |                                           |
 | CFSTORE Flash Journal Slot 3 |                                           |
 |                              |                                           |
 |- - - - - - - - - - - - - - - |                                           |
 |                              |                                           B
 | CFSTORE Flash Journal Slot 2 |                                           L
 |                              |                                           O
 |- - - - - - - - - - - - - - - |                                           C
 |                              |                                           K
 | CFSTORE Flash Journal Slot 1 |                                           1
 |                              |                                           |
 |- - - - - - - - - - - - - - - |                                           |
 |                              |                                           |
 | CFSTORE Flash Journal Slot 0 |                                           |
 |                              |                                           v
 -------------------------------- 0x00080000 = 512kB                        ^
 |                              | CONFIG_HARDWARE_MTD_START_ADDR            |
 |       Unused Flash           | (defaults to 0x80000UL)                   |
 |                              |                                           |
 |------------------------------| firmware_addr_end == __uvisor_flash_end   |
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           B
 |//////////////////////////////|                                           L
 |////////          ////////////|                                           O
 |//////// Firmware ////////////|                                           C
 |////////          ////////////|                                           K
 |//////////////////////////////|                                           0
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           |
 |//////////////////////////////|  											|
 --------------------------------  0x00000000           flash_addr_start    v
                                                    == firmware_addr_start
 

 Figure 1: Scenario 1 K64F Flash Address Map Showing Default Firmware and CFSTORE Allocations. 
 
```

The K64F has 2 flash blocks (see Figure 1):
- BLOCK0 for 0x00000000-0x00007ffff of the flash memory map (lower 512kB).
- BLOCK1 for 0x00007ffff-0x000fffff of the flash memory map (upper 512kB).

In this scenario (which is also the default CFSTORE/Flash Journal configuration) 
BLOCK0 is used exclusively to store program code/data and 
BLOCK1 is used exclusively for CFSTORE key-value data. It's therefore possible to operate flash-journal 
in both flash journal synchronous and asynchronous modes provided certain restrictions are observed in the implementation. 

Figure 1 shows the flash memory layout on the K64F for Scenario 1 where the following points can be noted:
- The K64F has 1MB of flash divided into 2 x 512kB blocks, shown as BLOCK0 and BLOCK1 in the figure.
- By default, CFSTORE is configured to occupy the whole of BLOCK1 (512kB) leaving BLOCK0 (512kB) 
  for the firmware image. This can be changed with configuration options (see the next section for details).
- CFSTORE/Flash Journal are written so that the CFSTORE can be configured to occupy as small a space 
  as required using configuration options.
- CFSTORE should always be at the top of flash memory i.e. there should not be a gap after the 
  end of the CFSTORE area, as this space can be used to store a firmware image (and would 
  otherwise be wasted).


#### Scenario 1 Advantages and Disadvantages

This scenario has the following advantages/disadvantages:
- Advantage: the separation of the firmware image and CFSTORE KV data into different
  blocks implicitly avoids RWW violations. The model is also simple to picture 
  and understand.
- Advantage: CFSTORE/Flash-journal can be operated in both synchronous or asynchronous modes. Asynchronous
  mode has performance benefits as CPU/SRAM bound processing can proceed in parallel
  with the flash IO transaction.
- Disadvantage: The maximum firmware image size is 512kB because the other 
  flash block is being used exclusively by CFSTORE, even if only small amounts of 
  KV data are being persisted to flash. 
- Disadvantage: Release binary sizes may typically be smaller than 512kB but debug
  build binaries, or firmware instrumented with test code/harnesses may typically
  be larger, making testing/debugging problematic.

  
#### Scenario 1 Eliminates Flash READ While WRITE Collisions 

Flash-Journal asynchronous mode can be used because RWW collisions are not possible i.e. 
a flash READ operation for program instructions during 
an in-progress WRITE/ERASE transaction in the same flash block are not possible. Flash READs for 
instructions are read from BLOCK0; flash WRITE/ERASE operations are made to BLOCK1. Hence there is no
possibility of RWW collisions.

  	
### Scenario 2: Firmware & CFSTORE Data Stored In The Same Flash Block 


```
 -------------------------------- 0x00080000 = 512kB    flash_addr_end      ^
 |                              |                                           |
 | CFSTORE Flash Journal Slot 3 |                                           |
 |                              |                                           |
 |- - - - - - - - - - - - - - - |                                           |
 |                              |                                           B
 | CFSTORE Flash Journal Slot 2 |                                           L
 |                              |                                           O
 |- - - - - - - - - - - - - - - |                                           C
 |                              |                                           K
 | CFSTORE Flash Journal Slot 1 |                                           0
 |                              |                                           |
 |- - - - - - - - - - - - - - - |                                           |
 |                              |                                           |
 | CFSTORE Flash Journal Slot 0 |                                           |
 |                              |                                           |
 -------------------------------- CONFIG_HARDWARE_MTD_START_ADDR            |
 |                              |                                           |
 |       Unused Flash           |                                           |
 |                              |                                           |
 |------------------------------| firmware_addr_end == __uvisor_flash_end   |
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           |
 |////////          ////////////|                                           |
 |//////// Firmware ////////////|                                           |
 |////////          ////////////|                                           |
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           |
 |//////////////////////////////|                                           |
 |//////////////////////////////|  											|
 --------------------------------  0x00000000           flash_addr_start    v
                                                    == firmware_addr_start
 

 Figure 2: Scenario 2 K64F Flash Address Map Showing Default Firmware and CFSTORE Allocations. 
 
```


In this scenario BLOCK0 is used for both program code/data and CFSTORE key-value data, and therefore
the application must only use CFSTORE/Flash Journal synchronous mode to store data. This is explained in the following:
- BLOCK0 is used to store the firmware image. Program execution therefore reads code instructions
  from flash BLOCK0.
- BLOCK0 is used to write/erase CFSTORE configuration data (the same block used for reading coding instructions).
- Hence there is the possibility of a concurrent READ access while a WRITE/ERASE transaction is in progress, 
  which must not happen. This is prevented by the CFSTORE/Flash Journal implementation.
- Sync mode is used and Flash Journal busy waits for the pending flash transaction to complete. 
- All interrupts are disabled to prevent the execution of interrupt handlers concurrently with the in-progress flash
  transaction. If permitted, executing the interrupt handler will cause instructions to be read from BLOCK0 leaving the 
  possibility of a RWW error.
- Flash Journal functions executing while the flash transaction is outstanding must be resident in memory 
  (otherwise their execution would cause a RWW error when the instructions are read from BLOCK0).
  This is done with the RAMFUNC function decoration (see implementation).
	  
In this scenario, it is not possible to use CFSTORE asynchronous mode because execution requires 
the reading of instructions from BLOCK0 at the same time as the in-progress flash transaction, 
which is not allowed as it causes RWW errors.


#### Scenario 2 Advantages and Disadvantages

This scenario has the following advantages/disadvantages:
- Advantage: the combined storage of the firmware image and CFSTORE KV data into a single
  block reduces the bill of material costs. 
- Disadvantage: CFSTORE/Flash-journal can only be operated in synchronous modes.
- Disadvantage: Interrupt latency times increase because interrupts have to be disabled
  during flash WRITE/ERASE transactions to prevent interrupt handlers reading code 
  instructions from the same block.

## Configuring the CFSTORE Flash Area Start Address and Size 

The default configuration is controlled by the following symbols:
- CONFIG_HARDWARE_MTD_START_ADDR. This is the start address of 
  the CFSTORE area in the flash storage. It defaults to the start of 
  BLOCK1 i.e. 0x80000. See Figure 1 for the location of the
  CONFIG_HARDWARE_MTD_START_ADDR in the flash address map. 
- CONFIG_HARDWARE_MTD_SIZE. This is the size of CFSTORE flash 
  memory area, and defaults to the size of BLOCK1 (i.e. 0x00080000 = 512kB).

The following file contains CONFIG_HARDWARE_MTD_START_ADDR and 
CONFIG_HARDWARE_MTD_SIZE processing:

```
mbed-os_root\core\hal\targets\hal\TARGET_Freescale\TARGET_KSDK2_MCUS\TARGET_K64F\storage_driver.c
```

as shown here:

```
        /**< This is the start address of the flash block. */
#ifdef CONFIG_HARDWARE_MTD_START_ADDR
        .addr       = CONFIG_HARDWARE_MTD_START_ADDR,
#else
        .addr       = BLOCK1_START_ADDR,
#endif

        /**< This is the size of the flash block, in units of bytes.
         *   Together with addr, it describes a range [addr, addr+size). */
#ifdef CONFIG_HARDWARE_MTD_SIZE
        .size       = CONFIG_HARDWARE_MTD_SIZE,
#else
        .size       = BLOCK1_SIZE,
#endif
```

Use `CONFIG_HARDWARE_MTD_START_ADDR` to set the start address of the 
storage area and `CONFIG_HARDWARE_MTD_SIZE` to specify its size.

The storage area used by flash-journal is divided into a number of slots, 
where log/commit operations are rotated through the slots rather than 
writing/re-writing a single slot. This gives wear-levelling and reliability benefits.
Currently its configured here:

```
mbed-os_root\features\storage\flash-journal\flash-journal-strategy-sequential\config.h
```

which defines the number of slots (`SEQUENTIAL_FLASH_JOURNAL_MAX_LOGGED_BLOBS`) to be 4:

```
#define SEQUENTIAL_FLASH_JOURNAL_MAX_LOGGED_BLOBS 4
```

## CFSTORE/Flash-Journal Impact on System Resources and Operation


### Code Size

It is possible to compute the CFSTORE/Flash-Journal code size by simply building the tree and 
adding the following binary sizes together e.g. for CFSTORE version 0.3.3 the total is 35896 bytes:

The following are the CFSTORE code sizes:

```
(mx2_venv3) cfstore_mbedos_blinky9/.build/tests/K64F/GCC_ARM/mbed-os/features/storage/cfstore/source$ ls -la *.o
-rw-r--r--    1 simhug01 Administ     2544 Jul  7 11:26 cfstore_fnmatch.o
-rw-r--r--    1 simhug01 Administ    18760 Jul  7 11:26 configuration_store.o
<line removed as cfstore_test.o bound to test case binaries and therefore not relevant>
```

The following are the flash journal code sizes:

```
(mx2_venv3) cfstore_mbedos_blinky9/.build/tests/K64F/GCC_ARM/mbed-os/features/storage/flash-journal/flash-journal-strategy-sequential$ ls -la *.o
-rw-r--r--    1 simhug01 Administ     3408 Jul  7 11:26 strategy.o
-rw-r--r--    1 simhug01 Administ     4324 Jul  7 11:26 support_funcs.o
```

The following is the platform flash device driver:

```
mx2_venv3) cfstore_mbedos_blinky9/.build/tests/K64F/GCC_ARM/mbed-os/core/hal/targets/hal/TARGET_Freescale/TARGET_KSDK2_MCUS/TARG
T_K64F $ ls -la *.o
rw-r--r--    1 simhug01 Administ     6860 Jul  7 11:25 storage_driver.o

```

### SRAM Footprint

The CFSTORE implementation uses SRAM in the following way:
- The size in memory of each key-value pair is the sum of the following component sizes:
    - KEY_NAME_LEN: the keyname length in bytes (not including a terminating null).
    - VALUE_LEN: the length of the value specified to the Create() call.
    - HEADER_SIZE: each attribute has a header the length of sizeof(cfstore_area_header_t) = 8 bytes
       used to store housekeeping information. Hence, each KV has an 8 byte overhead which is typically 
       small compared with the size of the information stored.   
- As an example, to store 100 key-value attributes with keynames 30 chars long and values each 1kB in size, the
  total_SRAM_used = 100 * (30 + 1024 + 8) = 106200 == ~104kB.  
- CFSTORE maintains an SRAM area containing all the stored attributes. 
    - Flush() causes the memory area to be stored in a Flash-Journal slot in non-volatile storage (flash).
    - Initialize() reads the data from the last used Flash-Journal slot into SRAM for use by the 
      system.
    - Hence, the current CFSTORE implementation has the limitation that the maximum 
      size of data stored in flash is limited by the maximum amount of SRAM that can be used to store
      the KVs prior to being flushed to flash. In the previous
      example 104kB of SRAM must be available to store 104kB of data in flash. This limitation will be
      removed in a future implementation.

### Non-Volatile Storage (Flash) Footprint

CFSTORE uses Flash Journal slots as the non-volatile backing store for key-value attributes.
- By default, Flash Journal is configured to have n_slots=4 slots.
- A sequence of {0, 1, 2, 3, 4, 5, ..., i} CFSTORE API Flush() operations write to the slots in sequential
  order to provide wear-levelling across the flash, i.e. :
    - Flush() call number 0 stores data in slot 0.
    - Flush() call number 1 stores data in slot 1.
    - Flush() call number 2 stores data in slot 2.
    - Flush() call number 3 stores data in slot 3.
    - Flush() call number 4 stores data in slot 0.
    - Flush() call number 5 stores data in slot 1.
    - Flush() call number i stores data in slot (i % n_slots).
- The size of the flash storage area CONFIG_HARDWARE_MTD_SIZE must therefore be (at a minimum) 
  CFSTORE_flash_size = n_slots * total_SRAM_used, rounded up to the nearest ERASE_SIZE size (4kB on K64F). 
  To continue the previous example where total_SRAM_used = 104kB, CFSTORE_flash_size = 4 * 104 = 416kB.

### CFSTORE Key-Value Data Read/Write Times From Flash

The CFSTORE API Initialize() call reads the key-value data from non-volatile storage. 
Measurements have not been made on how long CFSTORE takes to read data.

The CFSTORE API Uninitialize() call writes the key-value data to non-volatile storage if any of
the attributes have changed since the Initialize() call. The write operation:
- erases the flash slot being used. Erase operations have been measured at ~1.4us/byte for 
  erase sizes of 4-32kB in increments of 4kB (averaged over ~20 measurements).  
- programs the flash slot being used. Program operations have been measured at ~9.2 us/byte
  for program sizes of 8b-8kB (averaged over ~20 measurements).
Thus an Uninitialize() operation for 104kB data will take ~1.1s 
( = (9.2+1.4)* 1e^-6 *104 8 1024 seconds) to complete.

### Access Times

As all CFSTORE key-value attributes are held in SRAM, key-value attribute access times are restricted
only by the speed of SRAM memory access times.   

### Interrupt Latency

See NXP Freescale Kinetis K64F Sub-Family Data Sheet K64P144M120SF5.pdf. section 
3.4.1.2 Flash timing specifications, commands Table 24. Flash command timing specifications 
for flash transaction times.

The above document reference indicates the duration of a flash ERASE operation to take ~115ms for a 4kB ERASE operation. This can 
therefore delay an interrupt by 115ms. This is much slower that the previously reported measurements and 
therefore suggests a worst case scenario factoring in device aging. Further investigation is recommended.

### Typical Embedded System Flash Configuration
 
Embedded systems commonly have the following configurations:
- flash partition0: bootloader (single system copy, and boot environment data e.g. active bank).
- flash partition1: firmware image copy 0 (optionally including bootloader image).
- flash partition2: filesystem/data for image copy 0 (e.g. certificates, keys, kernel boot parameters).
- flash partition3: firmware image copy 0 (optionally including bootloader image).
- flash partition4: filesystem/data for image copy 0 (e.g. certificates, keys, kernel boot parameters).

Typical platform (system) configurations may be dimensioned (depending on cost, reliability, etc) as follows:
- partitions 0, 1, 2, 3, 4, 5 stored on single flash part are typically ~{128MB, 256MB, 512MB} in size. This 
  configuration corresponds to Scenario 2.
- partitions 0, 1, 2 stored on 1st flash part (BANK0),  partitions 3, 4 stored on 2nd flash (BANK1), where parts 
  are typically ~{128MB, 256MB, 512MB} in size. This configuration corresponds to Scenario 1. 


### Recommendations for System Designers When Adopting CFSTORE/Flash-Journal

The following conclusions can be drawn from the considerations in the previous sections:
- A system designer optimising for performance would use at least 2 flash BLOCKs, 
  sized to allow the firmware code to be located in one block and CFSTORE data in another.
  This equates to Scenario 1 described in previous sections.   
- A system designer optimising for cost would select a single flash BLOCK part and accept
  the lower performance of CFSTORE/Flash-Journal operating in synchronous mode. This equates to
  scenario 2.
- SRAM is needed to hold all stored CFSTORE data in memory for processing.
  

## References 

* [NXP Freescale Application Note AN4695.pdf: Avoiding Read While Write Errors][NXP_FREESCALE_AN4695]
* NXP Freescale Kinetis K64F Sub-Family Data Sheet K64P144M120SF5.pdf.

[NXP_FREESCALE_AN4695]: http://cache.freescale.com/files/32bit/doc/app_note/AN4695.pdf



