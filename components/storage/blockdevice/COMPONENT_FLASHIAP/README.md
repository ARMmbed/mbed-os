# Block Device driver build on top of FlashIAP API

## Warning 
This driver is **EXPERIMENTAL** and improper usage could kill your board's flash.

This driver should only be used on platforms where the FlashIAP implementation is using external flash or in conjunction with a filesystem with wear leveling, that can operate on a page size granularity.

Additional concerns:
- The FlashIAP may freeze code execution for a long period of time while writing to flash. Not even high-priority irqs will be allowed to run, which may interrupt background processes.


## Configuration
None.

## Tested on

* K82F
* K64F
