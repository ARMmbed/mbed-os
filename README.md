# sd-driver

This repository contains the mbed-os SDCard driver for generic SPI SDCard support including:

- `SDBlockDevice.h` and `SDBlockDevice.cpp`, the SDCard driver presenting the Block Device API (derived from BlockDevice).
- POSIX File API test cases for testing the FAT32 filesystem on SDCard.
    - basic.cpp, a basic set of functional test cases.
    - fopen.cpp, more functional tests reading/writing greater volumes of data to SDCard, for example.
- The [POSIX File API documentation][FS_README] including detailed instruction on how to use the FAT filesystem and SDBlockDevice driver. 
- `mbed_app.json` mbed-os applicaton configuraton file with SPI pin configurations for the CI shield and overrides for specific targets. 
   This file allows the SPI pins to be specified for the target without having to edit the implementation files.

The following sample code illustrates how to use the sd-driver: 


    #include "mbed.h"
    #include "SDBlockDevice.h"

    // Instantiate the SDBlockDevice by specifying the SPI pins connected to the SDCard 
    // socket. The PINS are:
    //     MOSI (Master Out Slave In)
    //     MISO (Master In Slave Out)
    //     SCLK (Serial Clock)
    //     CS (Chip Select)
    SDBlockDevice sd(p5, p6, p7, p12); // mosi, miso, sclk, cs
    uint8_t block[512] = "Hello World!\n";
    
    int main() 
    {
        // call the SDBlockDevice instance initialisation method.
        sd.init();
        
        // Write some the data block to the device 
        sd.program(block, 0, 512);
        
        // read the data block from the device
        sd.read(block, 0, 512);
        
        // print the contents of the block
        printf("%s", block);
        
        // call the SDBlockDevice instance de-initialisation method.
        sd.deinit();
    }

# References 

* The [filesystem documentation including how to use the FAT filesystem on SDCard][FS_README].

[FS_README]: features\filesystem\README.md