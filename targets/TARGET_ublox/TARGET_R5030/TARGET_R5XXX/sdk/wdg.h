#ifndef INCLUDED_WDG
#define INCLUDED_WDG
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** wdg HAL Spreadsheet version number */
#define WDG_HAL_VERSION 1

/** Watchdog IP block has the following features : 
- APB slave interface
- 16 bit down counters with 15 bit prescale (7 settings for prescale possible)
- Interrupt output generation on timeout
- Reset output generation on timeout with possibility of delayed reset
- Access keys to protect registers from being altered by software unintentionally
- Stall feature supported via external stall input
*/
struct wdg_s {
   /** Watchdog Overflow mode register at address offset 0x000, read-write */
   uint32_t omr;
   /** Watchdog Control mode register at address offset 0x004, read-write */
   uint32_t cmr;
   /** Watchdog control register at address offset 0x008, write-only */
   uint32_t cr;
   /** Watchdog Status regsiter at address offset 0x00C, read-write */
   uint32_t sr;
};

/** bit field defines for wdg_s#omr */
#define WDG_OMR_EN_OFFSET 0
#define WDG_OMR_EN_SIZE 1
#define WDG_OMR_RSTEN_OFFSET 1
#define WDG_OMR_RSTEN_SIZE 1
#define WDG_OMR_IRQEN_OFFSET 2
#define WDG_OMR_IRQEN_SIZE 1
#define WDG_OMR_EXTEN_OFFSET 3
#define WDG_OMR_EXTEN_SIZE 1
#define WDG_OMR_DEL_RSTEN_OFFSET 4
#define WDG_OMR_DEL_RSTEN_SIZE 1
#define WDG_OMR_OKEY_OFFSET 5
#define WDG_OMR_OKEY_SIZE 11
/** Default Overflow Access Key */
#define WDG_OMR_OKEY_OKEY_VALUE 564

/** bit field defines for wdg_s#cmr */
#define WDG_CMR_PRESCLKS_OFFSET 0
#define WDG_CMR_PRESCLKS_SIZE 3
#define WDG_CMR_HPCV_OFFSET 3
#define WDG_CMR_HPCV_SIZE 4
#define WDG_CMR_CKEY_OFFSET 7
#define WDG_CMR_CKEY_SIZE 9
/** Default Clock Access Key */
#define WDG_CMR_CKEY_CKEY_VALUE 110

/** bit field defines for wdg_s#cr */
#define WDG_CR_RKEY_OFFSET 0
#define WDG_CR_RKEY_SIZE 16
/** Default Restart Key */
#define WDG_CR_RKEY_RKEY_VALUE 49265

/** bit field defines for wdg_s#sr */
#define WDG_SR_OVF_OFFSET 0
#define WDG_SR_OVF_SIZE 1

/* EOF wdg.h */
#endif
