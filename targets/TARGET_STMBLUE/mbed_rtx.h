/* mbed Microcontroller Library
 * Author: Antonio O., 07/11/2018
 */

#if MBED_MBED_RTX_H

#include <stdint.h>

#ifndef INITIAL_SP

#if (defined(TARGET_BLUENRG1))
#define INITIAL_SP              (0x20006000UL)

#elif (defined(TARGET_BLUENRG2))
#define INITIAL_SP              (0x20006000UL)  //TBC

#else
#error "INITIAL_SP is not defined for this target in the mbed_rtx.h file"
#endif

#endif // INITIAL_SP

#if (defined(TARGET_BLUENRG2))
#if (defined(__GNUC__) && !defined(__CC_ARM) && !defined(__ARMCC_VERSION))
extern uint32_t               __StackLimit;
extern uint32_t               __StackTop;
extern uint32_t               __end__;
extern uint32_t               __HeapLimit;
#define HEAP_START            ((unsigned char*) &__end__)
#define HEAP_SIZE             ((uint32_t)((uint32_t) &__HeapLimit - (uint32_t) HEAP_START))
#define ISR_STACK_START       ((unsigned char*) &__StackLimit)
#define ISR_STACK_SIZE        ((uint32_t)((uint32_t) &__StackTop - (uint32_t) &__StackLimit))
#endif

#ifdef MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE
#undef MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE
#endif
#define MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE 3072  //da rivedere

#endif

#endif // MBED_MBED_RTX_H
