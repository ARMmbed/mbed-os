# Block Device driver build on top of FlashIAP API

## Warning 
This driver is **EXPERIMENTAL** and improper usage could kill your board's flash.

This driver should only be used on platforms where the FlashIAP implementation is using external flash or in conjunction with a filesystem with wear leveling, that can operate on a page size granularity.

Additional concerns:
- The BlockDevice API assumes a uniform erase size so the underlying flash must also have uniform sectors.
- The FlashIAP may freeze code execution for a long period of time while writing to flash. Not even high-priority irqs will be allowed to run, which may interrupt background processes.


## Configuration

The driver must be configured with the starting address for the internal flash area reserved for the block device and it's size in bytes. In the application's `mbed_app.json`, add the following lines:

```json
{
    "target_overrides": {
        "K64F": {
            "flashiap-block-device.base-address": "<internal flash address where the block device starts>",
            "flashiap-block-device.size": "<number of bytes allocated to the internal flash block device>"
        }
    }
}
```


## Tested on

* Realtek RTL8195AM
* K64F
