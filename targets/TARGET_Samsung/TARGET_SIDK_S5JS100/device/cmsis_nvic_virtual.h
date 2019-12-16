#include "cmsis.h"

#ifndef NVIC_VIRTUAL_H
#define NVIC_VIRTUAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "s5js100_type.h"

/* NVIC functions */
#define NVIC_SetPriorityGrouping    __NVIC_SetPriorityGrouping
#define NVIC_GetPriorityGrouping    __NVIC_GetPriorityGrouping
#define NVIC_EnableIRQ              __NVIC_EnableIRQ
#define NVIC_GetEnableIRQ           __NVIC_GetEnableIRQ
#define NVIC_DisableIRQ             __NVIC_DisableIRQ
#define NVIC_GetPendingIRQ          __NVIC_GetPendingIRQ
#define NVIC_SetPendingIRQ          __NVIC_SetPendingIRQ
#define NVIC_ClearPendingIRQ        __NVIC_ClearPendingIRQ
#define NVIC_GetActive              __NVIC_GetActive
#define NVIC_SetPriority            __NVIC_SetPriority
#define NVIC_GetPriority            __NVIC_GetPriority
#define NVIC_SystemReset            __NVIC_TFMSystemReset


/**
 * \brief Overriding the default CMSIS system reset implementation by calling
 *        secure TFM service.
 *
 */
__NO_RETURN __STATIC_INLINE void __NVIC_TFMSystemReset(void)
{
    putreg32(0x1, 0x8301100C);
    putreg32(0x1 << 1, 0x82020018);

    putreg32(0x4, 0x83011000); // enable watchdog
    putreg32(0x1, 0x83011010);
    putreg32(0x1, 0x83011020);
    putreg32(327, 0x83011004);  //set 10ms to be reset , 1 sec=32768
    putreg32(0xFF, 0x83011008); // force to load value to be reset

    /* Wait for the reset */
    for (;;) {
    }
}

#ifdef __cplusplus
}
#endif

#endif
