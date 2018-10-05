# ARM TrustZone True Number Generator

This software product contains the following open source components:

* ARM TrustZone TRNG driver.
* ARM TrustZone TRNG integration test.
* Characterization reference code.
	

Containing:

* READM.md (this file)
* Software and test build system:
    build.props
    proj.ext.cfg
    TRNG_test.c
    shared/
    host/  
* Software Integrator's Manual   (trustzone_true_random_number_generator_software_integrators_manual_101049_0000_00_en.pdf)

ARM TrustZone TRNG supports 32-bit systems.
The `.mbedignore` file allows the driver to be used as it is for TRNG implementation in mbed OS. That file will make it work under 800-90B TRNG mode.

## License 

This software is provided under the Apache-2.0 license. Contributions to this project are accepted under the same license.
		
	
## Release details

This release contains the ARM TrustZone TRNG software.

### Prerequisites:

* To be run on an Ubuntu 14.04 LTS system host for building. 

* gcc version 4.7.3 or arm-ds5 compiler are in your PATH.

* If using Linux as a host on the H/W, configure the KERNEL_DIR environment variable to point to your Linux (s).

* The target is running Linux kernel 4.4 or FreeRTOS.

Download the software bundle from https://github.com/ARM-software/TZ-TRNG.

### Build procedure

**Step 1**: Open a terminal on the Ubuntu system, and use 'cd <path>' to go to the folder where you unpacked the software bundle from the ARM github url.

**Step 2**: Define environment variables, depending on what the hw system target is running:

* For a target hw system running FreeRTOS:
```bash
   export KERNEL_DIR=/path/to/freertos
```
or

* For a target hw system running linux:
```bash
   export KERNEL_DIR=/path/to/linux
```				
  
**Step 3**: Build all the binaries (common for FreeRTOS and Linux):
```bash
   cd /path/to/tztrng
   make -C host/src/tztrng_lib/ clean
   make -C host/src/tztrng_lib/
   make -C host/src/tests/tztrng_test/ clean 
   make -C host/src/tests/tztrng_test/ 
```			
The tztrng library is located in:
```bash
   host/lib/libcc_tztrng.a        
```
	
The integration test executable is located in the following path:
   - When compiling with gcc: host/bin/tztrng_test.
   - When compiling with armcc: host/lib/libtztrng_test.a.


**Step 4**: Deploy:
* For a target hw system running linux:

  Copy host/bin/tztrng_test to the target linux file system and exeute:
  ```bash
     ./tztrng_test
  ```

* For a target hw system running FreeRTOS:

  ```bash
     cp host/lib/libcc_tztrng.a /path/to/your/library/folder
     cp host/lib/libtztrng_test.a /path/to/your/library/folder
     cp host/src/tests/tztrng_test/tztrng_test.h /path/to/includes
  ```
    
## Validation

1. Tests run

    See "Step 4: Deploy" above.

1. Known issues
        
    None

1. Planned enhancements
        
    None
