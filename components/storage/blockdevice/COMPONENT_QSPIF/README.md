# Quad SPI (QSPI) Flash Block Device

Block device driver for NOR based QSPI flash devices that support SFDP.

NOR based QSPI flash supports up to 4bits per cycle of instruction, address and data.
SFDP based QSPI Flash supports variable bus modes (single, dual, quad), several sector erase size types, multiple regions of sector size types and more.

SFDP JEDEC standard can be found in:
https://www.jedec.org/system/files/docs/JESD216B.pdf

### Debugging
Set `MBED_CONF_MBED_TRACE_ENABLE` as true(1) to enable logs.

### Example

``` cpp
// QSPI SFDP Flash - Block Device example
#include "mbed.h"
#include "QSPIFBlockDevice.h"

QSPIFBlockDevice block_device(MBED_CONF_QSPIF_QSPI_IO0,MBED_CONF_QSPIF_QSPI_IO1,MBED_CONF_QSPIF_QSPI_IO2,MBED_CONF_QSPIF_QSPI_IO3,
        MBED_CONF_QSPIF_QSPI_CLK,MBED_CONF_QSPIF_QSPI_CS,QSPIF_POLARITY_MODE_0,MBED_CONF_QSPIF_QSPI_FREQ);


int main() {

#if defined(MBED_CONF_MBED_TRACE_ENABLE) && MBED_CONF_MBED_TRACE_ENABLE
    mbed_trace_init();
#endif
    printf("QSPI SFDP Flash Block Device example\n");

    int status = 0;
    // Initialize the SPI flash device and print the memory layout
    status = block_device.init();
    if (0 != status) {
        printf("QSPI Flash init failed \n");
        return status;
    }
    bd_size_t sector_size_at_address_0 = block_device.get_erase_size(0);

    printf("QSPIF BD size: %llu\n",         block_device.size());
    printf("QSPIF BD read size: %llu\n",    block_device.get_read_size());
    printf("QSPIF BD program size: %llu\n", block_device.get_program_size());
    printf("QSPIF BD erase size (at address 0): %llu\n", sector_size_at_address_0);

    // Write "Hello World!" to the first block
    char *buffer = (char*) malloc(sector_size_at_address_0);
    if (buffer == NULL) {
        printf("Malloc failed\n");
        return -1;
    }
    sprintf(buffer, "Hello World!\n");

    int address = 0;
    status = block_device.erase(address, sector_size_at_address_0);
    if (0 != status) {
        printf("QSPI Flash Erase failed, address = 0x%x size = %lld\n", address, sector_size_at_address_0);
        return status;
    }

    status = block_device.program(buffer, address, sector_size_at_address_0);
    if (0 != status) {
        printf("QSPI Flash program failed, address = 0x%x size = %lld\n", address, sector_size_at_address_0);
        return status;
    }

    // Read back what was stored
    status = block_device.read(buffer, address, sector_size_at_address_0);
    if (0 != status) {
        printf("QSPI Flash read failed, address = 0x%x size = %lld\n", address, sector_size_at_address_0);
        return status;
    }

    printf("%s\n", buffer);

    // Deinitialize the device
    status = block_device.deinit();
    if (0 != status) {
        printf("QSPI Flash deinit failed \n");
        return status;
    }
    printf ("Finished \n");
    return 0;
}
```
