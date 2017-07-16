# DataFlash Driver

Block device driver for I2C based EEPROM devices such as the Adesto AT45DB
series of devices.

DataFlash is a memory protocol that combines flash with SRAM buffers for a
simple programming interface. DataFlash supports byte-sized read and writes,
with an erase size of around 528 bytes or sometimes 1056 bytes. DataFlash
provides erase sizes with and extra 16 bytes for error correction codes (ECC)
so that a flash translation layer (FTL) may still present 512 byte erase sizes.

The DataFlashBlockDevice can be configured to force the underlying device
to use either the binary size (ie 512 bytes) or the raw DataFlash size
(ie 528 bytes).

More info on DataFlash can be found on wikipedia:
https://en.wikipedia.org/wiki/DataFlash

``` cpp
// Here's an example using the AT45DB on the K64F
#include "mbed.h"
#include "DataFlashBlockDevice.h"

// Create DataFlash on SPI bus with PTE5 as chip select
DataFlashBlockDevice at45db(PTE2, PTE4, PTE1, PTE5);

// Create DataFlash on SPI bus with PTE6 as write-protect
DataFlashBlockDevice at45db(PTE2, PTE4, PTE1, PTE5, PTE6);

int main() {
    printf("dataflash test\n");

    // Initialize the SPI flash device and print the memory layout
    dataflash.init();
    printf("dataflash size: %llu\n", dataflash.size());
    printf("dataflash read size: %llu\n", dataflash.get_read_size());
    printf("dataflash program size: %llu\n", dataflash.get_program_size());
    printf("dataflash erase size: %llu\n", dataflash.get_erase_size());

    // Write "Hello World!" to the first block
    uint8_t *buffer = (uint8_t*)malloc(spif.get_erase_size());
    sprintf(buffer, "Hello World!\n");
    spif.erase(0, spif.get_erase_size());
    spif.program(buffer, 0, spif.get_erase_size());

    // Read back what was stored
    spif.read(buffer, 0, spif.get_erase_size());
    printf("%s", buffer);

    // Deinitialize the device
    spif.deinit();
}
```

