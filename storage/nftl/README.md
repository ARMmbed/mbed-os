## Contents

## Introduction
**nftl** module provides logical block to physical page mapping, bad block management, wear leveling, power loss protect, gabage collection, etc.

```
/**
 * Short description:
 * 
 * NFTL (i.e. Nand Flash Translation Layer), is used to provide
 * logical-to-physical block mapping on Nand Flash.
 *
 * Detailed description:
 *
 * NFTL stores mapping info (16 bytes, see later chapter for detailed date format description)
 * in the spare area on each flash page.
 *
 * During boot, NFTL read back the mapping info, and restore the mapping table in RAM.
 *
 * NFTL uses the last page on each block to store the backup info of all pages' spare info,
 * to accelerate the scan process to setup the mapping table in RAM.
 *
 * Hereafter is detailed description of the NFTL data format on flash:
 *
 * 1. data+spare on each page:
 *
 *    --------------------------------
 *    | data(4KB) | spare(16B) + ECC |    -> page 0
 *    --------------------------------
 *    | data(4KB) | spare(16B) + ECC |    -> page 1
 *    --------------------------------
 *                .
 *                .
 *                .
 *
 * 2. data format in spare area:
 *
 *    |----------|----------|---------------------|----------------------------|
 *    0    LB    3   Rev    7     reserved       15  used by vendor (variable) |
 *
 *    - totally 16 bytes used by NFTL.
 *    - Logical Block index: 0 - 3 byte, this is the logical block number.
 *    - Revision number: 4 - 7 byte, this is the revision of the logical block.
 *                       RFC1982 serial bumber arithmetic used to avoid overflow..
 *    - Reserved: 8 - 16 byte, reserved for future use.
 *    - Other: used by vendor (ECC, bad block flag, etc.).
 *
 *    Note: 1. vendor area may be stored before or after nftl bytes, depending on
 *             how the vendor hal APIs (hal_flash_xxx) implemented.
 *          2. LB and Rev data is stored in little-endian format on flash.
 *             Please ensure endian conversion when read from to write 
 *             to the flash area.
 *
 * 3. page format on each block:
 *
 *    |---------|-------|
 *    | data    | spare |    -> page 0
 *    |---------|-------|
 *    | data    | spare |    -> page 1
 *    |---------|-------|
 *    | data    | spare |    -> page 2
 *    |---------|-------|
 *      ...        ...       -> ...
 *    |---------|-------|
 *    |accl data|       |    -> last page (i.e. ACCELERATE page)
 *    |---------|-------|
 *
 *    The last page on each block is used to store backup/accelerate data
 *    of each page on this block. So the last page on each block is somehow
 *    wasted (no valid data stored). It's used to decrease the scan time to setup
 *    the logical-to-physical mapping table (LB and Rev data).
 *
 *    The data format on the ACCELERATE page:
 *
 *    0               7              15
 *    |---------------|---------------|
 *    | page0's spare | page1's spare |
 *    |---------------|---------------|
 *    | page2's spare | page3's spare |
 *    |---------------|---------------|
 *    | page4's spare | page5's spare |
 *    |---------------|---------------|
 *    |  accl page's  |               |
 *    |---------------|---------------|
 *    |    ...        |    ...        |
 *    |---------------|---------------|
 *
 *    Note: All data store on the accl page is in little-endian format.
 */
```

### Features
- logical block to physical page mapping
- bad block management
- wear leveling
- power loss protect
- gabage collection

### Directories

```sh
nftl/
├── aos.mk
├── Config.in
├── nftl.c
├── nftl.h
├── inc
│   └── nftl_util.h
│   └── nftl_conf.h
│   └── nftl_partition.h
├── README.md
```

### Dependencies

### Notice

## Reference
