# MPU Execute Never RAM Support

# Table of contents

1. [Mbed OS design document](#mbed-os-design-document).
1. [Table of contents](#table-of-contents).
    1. [Revision history](#revision-history).
1. [Introduction](#introduction).
    1. [Overview and background](#overview-and-background).
    1. [Requirements and assumptions](#requirements-and-assumptions).
1. [System architecture and high-level design](#system-architecture-and-high-level-design).
    1. [System architecture and component interaction](#system-architecture-and-component-interaction).
1. [Detailed design](#detailed-design).
1. [Usage scenarios and examples](#usage-scenarios-and-examples).

### Revision history

1.0 - Initial revision - Russ Butler - 10/17/2018.

# Introduction

### Overview and background

This MPU support is intended to increase the security of mbed-os by restricting when code can execute from RAM. One common attack on embedded devices is remote code execution by modifying RAM. This happens by overflowing a RAM buffer and then executing instructions from that buffer. By disabling execution from RAM mbed-os is more resistant against this form of attack.

### Requirements and assumptions

This API is limited to just enabling/disabling execution from RAM. It does not include runtime reconfiguration of the MPU regions.
- Only standard Arm MPU supported initially
- Only ARMv6m and ARMv7m devices with an MPU supported initially
- Single common memory map used

# System architecture and high-level design

The proposed change consists of two APIs, one for porting and one for general use, system integration and initial porting work:

 - HAL API so partners can port custom MPUs
 - Platform API so applications and libraries can enable or disable the execution from RAM
 - System integration so the execution from RAM is disabled by default
 - Initial porting for the Arm MPU

The goal of this is to increase security for a large number of devices by disabling execution from RAM when it is not required.

### System architecture and component interaction

On supported devices execution from RAM will be enabled and disable automatically at various points in mbed-os. Details and the reasons for this are below.

- Execution from RAM is disabled on boot

    As part of the boot sequence execution from RAM is disabled. This is done before `main` and is intended to protect against application programming errors.

- Execution from RAM is enabled when starting a new application

    As part of deinitializing the system in preparation for starting a new application execution from RAM is enabled. This is to maximize compatibility with the new application, which may be relying on execution from RAM.


- Execution from RAM is enabled when components require execution from RAM

    Some mbed-os system components require execute from RAM to work correctly. Examples of this include the `CThunk` and `FlashIAP` classes. When these are used then execution from RAM will be automatically enabled transparently to the user.

# Detailed design

### MPU memory map

Below is a table of showing the memory breakdown for ARMv6m and ARMv7m devices, along with the protection added by the default implementation of the MPU API. Regions which were not executable to begin with have no further protection added. The second half of the `Code` region is also protected by the MPU since some MCU families have RAM in this location.

ARMv6m and ARMv7m memory map:
```
Start        End            Name            Executable by default        Mbed MPU protection
0x00000000 - 0x1FFFFFFF     Code            Yes                          Execute disabled for second half
0x20000000 - 0x3FFFFFFF     SRAM            Yes                          Execute disabled
0x40000000 - 0x5FFFFFFF     Peripheral      No
0x60000000 - 0x7FFFFFFF     RAM             Yes                          Execute disabled
0x80000000 - 0x9FFFFFFF     RAM             Yes                          Execute disabled
0xA0000000 - 0xBFFFFFFF     Device          No
0xC0000000 - 0xDFFFFFFF     Device          No
0xE0000000 - 0xFFFFFFFF     System          No
```

### HAL MPU API

The HAL MPU API is the porting layer for mbed. Devices with non-Arm MPUs need to port to this API to the execute never API.

```C
/**
  * Initialize the MPU
  *
  * Initialize or re-initialize the memory protection unit.
  * It is implementation defined what region are protected
  * by the MPU after initialization.
  */
void mbed_mpu_init(void);

/**
  * Enable or disable RAM MPU protection
  *
  * This function is used to mark all of RAM as execute never.
  * When enabled code is only allowed to execute from flash.
  *
  * @param enable true to disable execution in RAM, false otherwise
  */
void mbed_mpu_enable_ram_xn(bool enable);

/** Deinitialize the MPU
  *
  * Powerdown the MPU in preparation for powerdown, reset or jumping to another application.
  */
void mbed_mpu_free(void);
```

### Platform MPU API

 The platform MPU API is a user facing API which can be used to disable the MPU to allow execution from RAM. It consists of a pair of lock and unlock functions. When locked by any part of the system then execute from RAM is allowed.

```C
/** Lock RAM execute never mode off
  *
  * This disables the MPU's execute never RAM protection and allows
  * functions to run from RAM. Execution directly from RAM will be
  * allowed if this function is invoked at least once(the internal
  * counter is non-zero).
  *
  * Use this locking mechanism for code which needs to execute from
  * RAM such as flash programming algorithms and RAM thunks.
  *
  * The lock is a counter, can be locked up to USHRT_MAX
  * This function is IRQ and thread safe
  */
 void mbed_mpu_manager_lock_mem_xn(void);

 /** Unlock RAM execute never mode
   *
   * Use unlocking in pair with mbed_mpu_manager_lock_mem_xn().
   *
   * The lock is a counter, should be equally unlocked as locked
   * This function is IRQ and thread safe
   */
  void mbed_mpu_manager_unlock_mem_xn(void);
```

# Usage scenarios and examples

### Scenario 1 user turning on execution from RAM

Users can opt-out of execute never RAM by calling the execute never lock function.

```C
#include "mbed.h"

int main()
{
    // Permanently enable execute from RAM for this application
    mbed_mpu_manager_lock_mem_xn();

    some_function_in_ram();
}
```

### Scenario 2 library temporarily allowing execution from RAM

Code that must execute from RAM, such as the flash programming shown below, can do so safely by locking execute never and the unlocking it after making the desired call. It is recommended that the execute never lock is held in a critical section whenever possible. This prevents RAM execution from being allowed in other contexts such as interrupts and threads.

```C
MBED_NONSECURE_ENTRY int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    core_util_critical_section_enter();
    mbed_mpu_manager_lock_mem_xn();

    flash_algo_init(obj, address, MBED_FLASH_ALGO_PROGRAM);

    args_t arguments = {
        .r0 = address,
        .r1 = size,
        .r2 = (uint32_t)data,
        .r3 = 0,
        .r9 = (uint32_t)obj->flash_algo->algo_blob + obj->flash_algo->static_base,
        .pc = (uint32_t)obj->flash_algo->algo_blob + obj->flash_algo->program_page
    };
    int32_t ret = ((flash_algo_jump_t)(((uint32_t)&jump_to_flash_algo) | 1))(&arguments);

    flash_algo_uninit(obj, address, MBED_FLASH_ALGO_PROGRAM);

    mbed_mpu_manager_unlock_mem_xn();
    core_util_critical_section_exit();
    return ret ? -1 : 0;
}
```
