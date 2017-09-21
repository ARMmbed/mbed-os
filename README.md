# flashiap-driver
Block device driver using the mbed OS FlashIAP API.

# Warning 
This driver is **EXPERIMENTAL** and improper usage could kill your board's flash.

This driver should only be used on platforms where the FlashIAP implementation is using external flash or in conjunction with a filesystem with wear leveling, that can operate on a page size granularity.

Additional concerns:
- The BlockDevice API assumes a uniform erase size so the underlying flash must also have uniform sectors.
- The FlashIAP may freeze code execution for a long period of time while writing to flash. Not even high-priority irqs will be allowed to run, which may interrupt background processes.

# Tested on

* Realtek RTL8195AM
