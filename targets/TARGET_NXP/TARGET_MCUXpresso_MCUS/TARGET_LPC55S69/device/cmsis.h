/* mbed Microcontroller Library - CMSIS
 * Copyright (C) 2009-2017 ARM Limited. All rights reserved.
 *
 * A generic CMSIS include header, pulling in LPC54608 specifics
 */

#ifndef MBED_CMSIS_H
#define MBED_CMSIS_H

#include "fsl_device_registers.h"
#include "platform_regs.h"              /* Platform registers */
#include "platform_retarget.h"          /* Peripherals base addresses */

#if defined(TARGET_LPC55S69_NS)
#include "cmsis_nvic.h"
#endif

#endif
