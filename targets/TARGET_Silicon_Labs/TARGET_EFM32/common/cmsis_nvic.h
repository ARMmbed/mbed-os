/* mbed Microcontroller Library - cmsis_nvic
 * Copyright (c) 2009-2011 ARM Limited. All rights reserved.
 */

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#define NVIC_NUM_VECTORS        (16 + EXT_IRQ_COUNT)     // CORE + MCU Peripherals

/* For GCC, use dynamic vector table placement since otherwise we run into an alignment conflict */
#if (defined (__GNUC__) && (!defined(__CC_ARM) && (!defined(__ARMCC_VERSION))))
extern uint32_t __start_vector_table__;       // Dynamic vector positioning in GCC
#define NVIC_RAM_VECTOR_ADDRESS (&__start_vector_table__)
#else
#define NVIC_RAM_VECTOR_ADDRESS 0x20000000    // Vectors positioned at start of RAM
#endif

#endif /* MBED_CMSIS_NVIC_H */
