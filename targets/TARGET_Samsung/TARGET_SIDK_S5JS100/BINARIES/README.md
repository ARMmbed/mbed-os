Explanation of files
* pbl_dcache_jumpto_mbedos.bin: 
  PBL (Primary BootLoader).
  It has configuration of CPU, MMU, etc.
  Temporay, PBL jump to mbed-os directly, before implemtation mbed boot loader.
 
* bl2.bin: 
  BL2 (BootLoader 2, Second BootLoader).
  It will be replaced to mbed boot loader.

* cpboot.bin: 
  Bootloader for CP(Communication Processor)
 
* cp_main_S111NB_SMDK_ONESKU_QB2029203.bin: 
  Executable Binary for CP(Communication Processor).

* sss_fw.bin: 
  Executable binary for Secure Sub System(SSS). It provides secure function. (ex: TRNG)
  
