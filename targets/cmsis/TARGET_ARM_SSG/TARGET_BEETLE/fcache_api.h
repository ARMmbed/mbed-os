/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _FCACHE_DRV_H
#define _FCACHE_DRV_H

#ifdef __cplusplus

extern "C" {
#else
#include <stdio.h>
#endif

    /* Flash Cache Address Map */
#define SYS_FCACHE_BASE 0x40003000
/* Configuration and Control Register */
#define SYS_FCACHE_CCR  (SYS_FCACHE_BASE)
/* Status Register */
#define SYS_FCACHE_SR   (SYS_FCACHE_BASE + 0x4)
/* Interrupt Req Status Register */
#define SYS_FCACHE_IRQSTAT  (SYS_FCACHE_BASE + 0x8)
/* Cache Statistic Hit Register */
#define SYS_FCACHE_CSHR (SYS_FCACHE_BASE + 0x14)
/* Cache Statistic Miss Register */
#define SYS_FCACHE_CSMR (SYS_FCACHE_BASE + 0x18)

    /* SYS_FCACHE_CCR (RW): Configuration and Control Register */
#define FCACHE_EN           1           /* FCache Enable */
#define FCACHE_INV_REQ      (1 << 1)    /* Manual Invalidate Request */
#define FCACHE_POW_REQ      (1 << 2)    /* Manual SRAM Power Request */
#define FCACHE_SET_MAN_POW  (1 << 3)    /* Power Control Setting */
#define FCACHE_SET_MAN_INV  (1 << 4)    /* Invalidate Control Setting */
#define FCACHE_SET_PREFETCH (1 << 5)    /* Cache Prefetch Setting */
#define FCACHE_STATISTIC_EN (1 << 6)    /* Enable Statistics Logic */

   /* SYS_FCACHE_SR (RO): Status Register */
#define FCACHE_CS           0x3     /* Cache Status Mask */
#define FCACHE_CS_DISABLED  0x0
#define FCACHE_CS_ENABLING  0x1
#define FCACHE_CS_ENABLED   0x2
#define FCACHE_CS_DISABLING 0x3
#define FCACHE_INV_STAT     0x4     /* Invalidating Status */
#define FCACHE_POW_STAT     0x10    /* SRAM Power Ack */

    /* SYS_FCACHE_IRQSTAT (RW): Interrupt Req Status Register */
#define FCACHE_POW_ERR      1           /* SRAM Power Error */
#define FCACHE_MAN_INV_ERR  (1 << 1)    /* Manual Invalidation error status */

    /* Macros */
#define FCache_Readl(reg) *(volatile unsigned int *)reg
#define FCache_Writel(reg, val)  *(volatile unsigned int *)reg = val;

/* Functions */

/*
 * FCache_DriverInitialize: flash cache driver initialize funtion
 */
void FCache_DriverInitialize(void);

/*
 * FCache_Enable: Enables the flash cache mode
 * mode: supported modes:
 * 0 - auto-power auto-invalidate
 * 1 - manual-power, manual-invalidate
 */
void FCache_Enable(int mode);

/*
 * FCache_Disable: Disables the flash cache mode previously enabled
 */
void FCache_Disable(void);

/*
 * FCache_Invalidate: to be invalidated the cache needs to be disabled.
 * return -1: flash cannot be disabled
 *        -2: flash cannot be enabled
 */
int FCache_Invalidate(void);

/*
 * FCache_GetStats: provides cache stats
 */
unsigned int * FCache_GetStats(void);

/*
 * FCache_isEnabled: returns 1 if FCache is enabled
 */
unsigned int FCache_isEnabled(void);

#ifdef __cplusplus
}
#endif
#endif /* _FCACHE_DRV_H */
