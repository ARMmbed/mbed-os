# SPI Flash Driver

Block device driver for NOR based SPI flash devices that support SFDP.

NOR based SPI flash supports byte-sized read and writes, with an erase size of around 4kbytes. An erase sets a block to all 1s, with successive writes clearing set bits.

More info on NOR flash can be found on wikipedia:
https://en.wikipedia.org/wiki/Flash_memory#NOR_memories

``` cpp
// Here's an example using the MX25R SPI flash device on the K82F
#include "mbed.h"
#include "SPIFBlockDevice.h"

// Create flash device on SPI bus with PTE5 as chip select
SPIFBlockDevice spif(PTE2, PTE4, PTE1, PTE5);

int main() {
    printf("spif test\n");

    // Initialize the SPI flash device and print the memory layout
    spif.init();
    printf("spif size: %llu\n",         spif.size());
    printf("spif read size: %llu\n",    spif.get_read_size());
    printf("spif program size: %llu\n", spif.get_program_size());
    printf("spif erase size: %llu\n",   spif.get_erase_size());

    // Write "Hello World!" to the first block
    char *buffer = (char*)malloc(spif.get_erase_size());
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

