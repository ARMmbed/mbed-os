#ifndef INCLUDED_INARI_CM7_SS_DEVICE
#define INCLUDED_INARI_CM7_SS_DEVICE
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

struct cortex_m7_itm_s;
#ifdef __cplusplus
namespace device {
#endif
/** Instrumentation trace Macrocell registers->
 *Instrumentation trace Macrocell registers
*/
static volatile struct cortex_m7_itm_s* const cortex_m7_itm = (struct cortex_m7_itm_s*)(0xE0000000);
#ifdef __cplusplus
}
#endif

struct cortex_m7_dwt_s;
#ifdef __cplusplus
namespace device {
#endif
/** Data watchpoint and trace registers->
 *Embedded trace Macrocell registers
*/
static volatile struct cortex_m7_dwt_s* const cortex_m7_dwt = (struct cortex_m7_dwt_s*)(0xE0001000);
#ifdef __cplusplus
}
#endif

struct cortex_m7_scs_s;
#ifdef __cplusplus
namespace device {
#endif
/** System control, MPU, debug and NVIC registers->
 *System control, MPU, debug and NVIC registers
*/
static volatile struct cortex_m7_scs_s* const cortex_m7_scs = (struct cortex_m7_scs_s*)(0xE000E000);
#ifdef __cplusplus
}
#endif

struct inari_cm7_ss_ppb_s;
#ifdef __cplusplus
namespace device {
#endif
/** Private pripheral bus, light sleep controller slave->
 *Private pripheral bus, light sleep controller slave
*/
static volatile struct inari_cm7_ss_ppb_s* const inari_cm7_ss_ppb = (struct inari_cm7_ss_ppb_s*)(0xE0040000);
#ifdef __cplusplus
}
#endif

struct cortex_m7_etm_s;
#ifdef __cplusplus
namespace device {
#endif
/** Embedded trace Macrocell registers->
 *Embedded trace Macrocell registers
*/
static volatile struct cortex_m7_etm_s* const cortex_m7_etm = (struct cortex_m7_etm_s*)(0xE0041000);
#ifdef __cplusplus
}
#endif

struct cortex_m7_cti_s;
#ifdef __cplusplus
namespace device {
#endif
/** Cross Trigger Interface registers->
 *Coresight Cross Trigger Interface
*/
static volatile struct cortex_m7_cti_s* const cortex_m7_cti = (struct cortex_m7_cti_s*)(0xE0042000);
#ifdef __cplusplus
}
#endif

/* EOF inari_cm7_ss_device.h */
#endif
