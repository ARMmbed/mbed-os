SPI Communication Test
=============

Testcases
---------

### test_spi_com

**Description**  
This is the SPI communication test which verifies various SPI configurations.
This test transfers data between SPI master and SPI slave.
Two boards are required with SPI peripheral available and slave/master mode support.    

To run the test using same boards as master and slave perform the following steps:  
 1. Adjust configuration to your needs in ./TEST_APPS/device/spi_config/mbed_app.json.  
 2. Connect SPI interfaces as configured in ./TEST_APPS/device/spi_config/mbed_app.json.  
 3. Execute the following command:  
 mbed test -m BOARD -t TOOLCHAIN --icetea -n test_spi_com -v --app-config ./TEST_APPS/device/spi_config/mbed_app.json  

Note:  
It is possible to use two different boards for spi master and spi slave, but the test must be  run using icetea (not mbed test). Check the manual for more details.

**Which SPI features are verified by the test?**  
 - master/slave mode,  
 - synchronous/asynchronous modes,  
 - full duplex/half duplex modes,  
 - symbol sizes: [1, 32],  
 - clock polarity/phase,  
 - bit ordering during transmission,  
 - clock frequency,  
 - rx count == tx count, tx count != tx count,  
 - undefined rx/tx buffer,  
 - internal/external ss handling by master.  

**Test scenario**  
 - Configuration is validated against the capabilities on master and slave side.  
 - If configuration is not supported by master or slave, then the configuration is skipped.  
 - If configuration can be handled by master and slave, then it is loaded and communication test is performed.  

**Expected result:**  
 The test exits without errors.

