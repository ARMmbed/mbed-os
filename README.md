# flashiap-driver
Block device driver using the mbed OS FlashIAP API.

# Warning 
This driver should only be used on platforms where the FlashIAP implementation is using external flash or in conjunction with a filesystem with wear leveling, that can operate on a page size granularity.

The BlockDevice API assumes a uniform erase size so the underlying flash must also have uniform sectors.

# Tested on

* Realtek RTL8195AM
