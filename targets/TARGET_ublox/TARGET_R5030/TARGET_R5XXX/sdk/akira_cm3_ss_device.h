#ifndef INCLUDED_AKIRA_CM3_SS_DEVICE
#define INCLUDED_AKIRA_CM3_SS_DEVICE
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

struct cortex_m3_itm_s;
#ifdef __cplusplus
namespace device {
#endif
/** Instrumentation trace Macrocell registers->
 *Instrumentation trace Macrocell registers
*/
static volatile struct cortex_m3_itm_s* const cortex_m3_itm = (struct cortex_m3_itm_s*)(0xE0000000);
#ifdef __cplusplus
}
#endif

struct cortex_m3_dwt_s;
#ifdef __cplusplus
namespace device {
#endif
/** Data watchpoint and trance registers->
 *Data watchpoint and trance registers
*/
static volatile struct cortex_m3_dwt_s* const cortex_m3_dwt = (struct cortex_m3_dwt_s*)(0xE0001000);
#ifdef __cplusplus
}
#endif

struct cortex_m3_scs_s;
#ifdef __cplusplus
namespace device {
#endif
/** System  control, MPU, debug and NVIC registers->
 *System  control, MPU, debug and NVIC registers
*/
static volatile struct cortex_m3_scs_s* const cortex_m3_scs = (struct cortex_m3_scs_s*)(0xE000E000);
#ifdef __cplusplus
}
#endif

struct akira_cm3_ss_ppb_s;
#ifdef __cplusplus
namespace device {
#endif
/** Private pripheral bus, light sleep controller slave->
 *Private pripheral bus, light sleep controller slave
*/
static volatile struct akira_cm3_ss_ppb_s* const akira_cm3_ss_ppb = (struct akira_cm3_ss_ppb_s*)(0xE0040000);
#ifdef __cplusplus
}
#endif

struct cortex_m3_etm_s;
#ifdef __cplusplus
namespace device {
#endif
/** Embedded trace Macrocell registers->
 *Embedded trace Macrocell registers
*/
static volatile struct cortex_m3_etm_s* const cortex_m3_etm = (struct cortex_m3_etm_s*)(0xE0041000);
#ifdef __cplusplus
}
#endif

struct cti_s;
#ifdef __cplusplus
namespace device {
#endif
/** Cross Trigger Interface registers->
 *Coresight Cross Trigger Interface
*/
static volatile struct cti_s* const cortex_m3_cti = (struct cti_s*)(0xE0042000);
#ifdef __cplusplus
}
#endif

/* EOF akira_cm3_ss_device.h */
#endif
