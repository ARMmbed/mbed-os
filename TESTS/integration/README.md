# Integration tests for Mbed OS




Mbed OS provides a set of integration tests to help users identify integration issues around storage device and connectivity device.

## Test suites

| **Test suite** | **Description** |
| ------------- | ------------- |
| `fs-single` | File system single-threaded tests with write buffer sizes - 1 byte, 4b, 16b, 64b, 256b, 1kb, 4kb, 16kb, 64kb. |
| `fs-multi` | File system multithreaded test with write buffer sizes - 1 byte, 4b, 16b, 64b, 256b, 1kb, 4kb, 16kb, 64kb. |
| `net-single` | Network single-threaded test with receive buffer sizes - 128 bytes, 256b, 1kb, 2kb, 4kb. |
| `net-multi` | Network multithreaded test for 1, 2 and 3 download threads with 1kb receive buffer size. |
| `stress-net-fs` | Network and file system single and multithreaded tests:<ul><li>memory allocation - 10k, 20k, 40k, 60k</li><li>1 thread (sequential) - 1 download (1kb buffer), 1 file thread (1kb buffer)</li><li>2 parallel threads - 1 download, 1 file thread (1kb buffer)</li><li>3 parallel threads - 1 download, 2 file (256 bytes, 1 kb buffer)</li><li>4 parallel threads - 1 download, 3 file (1 byte, 256 bytes, 1kb buffer)</li></ul> |



## Execute tests

The integration test requires both the connectivity and and storage device to to configured. Currently we have a pre-configuration file defined in the COMMON folder, and that file covers some of the platform connectivity/storage combinations.  If you have your own platform or you want to define your own configuration, please follow the [configuration guide](#Tests-configuration).

To compile and run the tests inside mbed-os directory, the configuration file need to be passed in the command line: 

    ```bash
    $ mbed test -t <toolchain> -m <platform> -n *integraion-* --app-config TESTS\integration\COMMON\mbed_app.json
    ```

### table of platforms in the configuration file


Platform                        |  Connectivity      | Storage 
--------------------------------| -------------------| --------- 
Nuvoton NUMAKER-IOT-M487            | Wi-Fi              | SD Card 
Nuvoton NUMAKER-PFM-M487            | Ethernet           | SD Card 
Nuvoton NUMAKER-PFM-NUC472          | Ethernet           | SD Card 
NXP FRDM-K64F                       | Ethernet           | SD Card 
NXP FRDM-K66F                       | Ethernet           | SD Card 
Renesas GR-PEACH                    | Ethernet           | SD Card 
Renesas GR-LYCHEE                   | Wi-Fi              | SD Card
Sigma Delta Technologies SDT64B     | Ethernet           | SD Card 
Silicon Labs Giant Gecko 11         | Ethernet           | QSPI 
Silicon Labs Thunderboard Sense 2   | 15.4 Thread        | SPI Flash
ST DISCO_L475E_IOT01A               | Wi-Fi              | QSPI 
ST DISCO_F413H                      | Wi-Fi              | QSPI 
ST DISCO_F469NI                     | Wi-Fi Shield       | QSPI
ST DISCO_F746NG                     | Ethernet           | QSPI
ST DISCO_F769NI                     | Ethernet           | QSPI
ST DISCO_L496AG                     | Wi-Fi Shield       | QSPI
ST NUCLEO_F207ZG                    | Ethernet           | SD Card
ST NUCLEO_F412ZG                    | Wi-Fi Shield       | SD Card
ST NUCLEO_F429ZI                    | Ethernet           | SD Card
ST NUCLEO_F767ZI                    | Ethernet           | SD Card
ST NUCLEO_F746ZG                    | Ethernet           | SD Card
ST NUCLEO_L476RG                    | Wi-Fi Shield       | SD Card 
ST NUCLEO_L496ZG                    | Wi-Fi Shield       | SD Card
ST NUCLEO_L4R5ZI                    | Wi-Fi Shield       | SD Card
UBlox C030 U201                     | Cellular           | SD Card
UBlox EVK ODIN W2                   | Wi-Fi              | SD Card

## Tests configuration

Edit the `mbed_app.json` file, and create a new entry under `target_overrides` with the target name for your device:

- **Connectivity** - Specify the default connectivity type for your target. It's essential with targets that lack default connectivity set in `targets.json` or for targets that support multiple connectivity options. For example:
  
   ```
            "target.network-default-interface-type" : "ETHERNET",
   ```
   
   The possible options are `ETHERNET`, `WIFI` and `CELLULAR`.
   
   Depending on connectivity type, you might have to specify more config options. For an example, please see the defined `CELLULAR` targets in `mbed_app.json`.

- **Storage** - Specify the storage block device type, which dynamically adds the block device driver you specified at compile time. For example:

   ```
            "target.components_add" : ["SD"],
   ```

   Valid options are `SD`, `SPIF`, `QSPIF` and `FLASHIAP` (not recommended). For more available options, please see the [block device components](https://github.com/ARMmbed/mbed-os/tree/master/components/storage/blockdevice).

   You also have to specify block device pin configuration, which may vary from one block device type to another. Here's an example for `SD`:
   
   ```
            "sd.SPI_MOSI"                      : "PE_6",
            "sd.SPI_MISO"                      : "PE_5",
            "sd.SPI_CLK"                       : "PE_2",
            "sd.SPI_CS"                        : "PE_4",
   ```
   
    If you are using SPI/QSPI flash, please make sure you have specified the correct SPI frequency by configuring `spif-driver.SPI_FREQ`. If it is not configured, 40Mhz will be applied by default.
   


