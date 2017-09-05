#ifndef INCLUDED_AR1DE_MWU
#define INCLUDED_AR1DE_MWU
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** Number of Memory Watch Regions */
#define AR1DE_MWU_NR_REGIONS 4
#define AR1DE_MWU_DEVCONF 0x04
#define AR1DE_MWU_DEVTYPE 0x45
#define AR1DE_MWU_PERIPHERALID4 0x07
#define AR1DE_MWU_PERIPHERALID5 0x00
#define AR1DE_MWU_PERIPHERALID6 0x00
#define AR1DE_MWU_PERIPHERALID7 0x00
#define AR1DE_MWU_PERIPHERALID0 0x81
#define AR1DE_MWU_PERIPHERALID1 0xDA
#define AR1DE_MWU_PERIPHERALID2 0x08
#define AR1DE_MWU_PERIPHERALID3 0x00
#define AR1DE_MWU_COMPONENTID0 0x0D
#define AR1DE_MWU_COMPONENTID1 0x90
#define AR1DE_MWU_COMPONENTID2 0x05
#define AR1DE_MWU_COMPONENTID3 0xB1

/** Memory Watch Regions
*/
struct ar1de_mwu_mwr_s {
   /** Memory Watch Region Start Register at address offset 0x000, read-write */
   uint32_t sr;
   /** Memory Watch Region End Register at address offset 0x004, read-write */
   uint32_t er;
   /** Memory Watch Region Configuration at address offset 0x008, read-write */
   uint32_t cf;
   /** Reserved space \private at address offset 0x00C, read-write */
   uint32_t fill;
};

/** bit field defines for ar1de_mwu_mwr_s#sr */
#define AR1DE_MWU_MWR_SR_SR_OFFSET 0
#define AR1DE_MWU_MWR_SR_SR_SIZE 32

/** bit field defines for ar1de_mwu_mwr_s#er */
#define AR1DE_MWU_MWR_ER_ER_OFFSET 0
#define AR1DE_MWU_MWR_ER_ER_SIZE 32

/** bit field defines for ar1de_mwu_mwr_s#cf */
#define AR1DE_MWU_MWR_CF_REGION_ENABLE_OFFSET 0
#define AR1DE_MWU_MWR_CF_REGION_ENABLE_SIZE 1
#define AR1DE_MWU_MWR_CF_REGION_ID_OFFSET 1
#define AR1DE_MWU_MWR_CF_REGION_ID_SIZE 2
#define AR1DE_MWU_MWR_CF_MASK_WIDTH_OFFSET 27
#define AR1DE_MWU_MWR_CF_MASK_WIDTH_SIZE 5

/** Memory Watch Unit
*/
struct ar1de_mwu_s {
   /** Memory Watch Regions at address offset 0x000 size: 64 */
   struct ar1de_mwu_mwr_s mwr[AR1DE_MWU_NR_REGIONS];
   /** Reserved space */
   uint8_t fill0[3776];
   /** Integration Mode Control at address offset 0xF00, read-only */
   uint32_t intmctrl;
   /** Reserved space */
   uint8_t fill1[156];
   /** Claim Tag Set Register at address offset 0xFA0, read-only */
   uint32_t claimset;
   /** Claim Tag Clear Register at address offset 0xFA4, read-only */
   uint32_t claimclr;
   /** Reserved space */
   uint8_t fill2[8];
   /** Lock Access Register at address offset 0xFB0, write-only */
   uint32_t lockaccess;
   /** Lock Status register at address offset 0xFB4, read-only */
   uint32_t lockstatus;
   /** Authentication Status Register at address offset 0xFB8, read-only */
   uint32_t authstatus;
   /** Reserved space */
   uint8_t fill3[12];
   /** Device configuration register at address offset 0xFC8, read-only constant 0x00000004 */
   uint32_t devconf;
   /** Device type register at address offset 0xFCC, read-only constant 0x00000045 */
   uint32_t devtype;
   /** Peripheral ID Register [39:32] at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** Peripheral ID Register [47:40] at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID Register [55:48] at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID Register [63:56] at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID Register [7:0] at address offset 0xFE0, read-only constant 0x00000081 */
   uint32_t peripheralid0;
   /** Peripheral ID Register [15:8] at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** Peripheral ID Register [23:16] at address offset 0xFE8, read-only constant 0x00000008 */
   uint32_t peripheralid2;
   /** Peripheral ID Register [31:24] at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Preamble at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Component class is CoreSight Component at address offset 0xFF4, read-only constant 0x00000090 */
   uint32_t componentid1;
   /** Preamble at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Preamble at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for ar1de_mwu_s#intmctrl */
#define AR1DE_MWU_INTMCTRL_INTMCTRL_OFFSET 0
#define AR1DE_MWU_INTMCTRL_INTMCTRL_SIZE 32

/** bit field defines for ar1de_mwu_s#claimset */
#define AR1DE_MWU_CLAIMSET_CLAIMSET_OFFSET 0
#define AR1DE_MWU_CLAIMSET_CLAIMSET_SIZE 8

/** bit field defines for ar1de_mwu_s#claimclr */
#define AR1DE_MWU_CLAIMCLR_CLAIMCLR_OFFSET 0
#define AR1DE_MWU_CLAIMCLR_CLAIMCLR_SIZE 8

/** bit field defines for ar1de_mwu_s#lockaccess */
#define AR1DE_MWU_LOCKACCESS_LOCKACCESS_OFFSET 0
#define AR1DE_MWU_LOCKACCESS_LOCKACCESS_SIZE 32

/** bit field defines for ar1de_mwu_s#lockstatus */
#define AR1DE_MWU_LOCKSTATUS_LOCK_CTRL_EXISTS_OFFSET 0
#define AR1DE_MWU_LOCKSTATUS_LOCK_CTRL_EXISTS_SIZE 1
#define AR1DE_MWU_LOCKSTATUS_ACCESS_PERMISSION_OFFSET 1
#define AR1DE_MWU_LOCKSTATUS_ACCESS_PERMISSION_SIZE 1
#define AR1DE_MWU_LOCKSTATUS_EIGHTBIT_LOCK_ACCESS_REG_IMPL_OFFSET 2
#define AR1DE_MWU_LOCKSTATUS_EIGHTBIT_LOCK_ACCESS_REG_IMPL_SIZE 1

/** bit field defines for ar1de_mwu_s#authstatus */
#define AR1DE_MWU_AUTHSTATUS_NON_SECURE_INVASIVE_DEBUG_OFFSET 0
#define AR1DE_MWU_AUTHSTATUS_NON_SECURE_INVASIVE_DEBUG_SIZE 2
#define AR1DE_MWU_AUTHSTATUS_NON_SECURE_NON_INVASIVE_DEBUG_OFFSET 2
#define AR1DE_MWU_AUTHSTATUS_NON_SECURE_NON_INVASIVE_DEBUG_SIZE 2
#define AR1DE_MWU_AUTHSTATUS_SECURE_INVASIVE_DEBUG_OFFSET 4
#define AR1DE_MWU_AUTHSTATUS_SECURE_INVASIVE_DEBUG_SIZE 2
#define AR1DE_MWU_AUTHSTATUS_SECURE_NON_INVASIVE_DEBUG_OFFSET 6
#define AR1DE_MWU_AUTHSTATUS_SECURE_NON_INVASIVE_DEBUG_SIZE 2

/** bit field defines for ar1de_mwu_s#devconf */
#define AR1DE_MWU_DEVCONF_DEVCONF_OFFSET 0
#define AR1DE_MWU_DEVCONF_DEVCONF_SIZE 8

/** bit field defines for ar1de_mwu_s#devtype */
#define AR1DE_MWU_DEVTYPE_DEVTYPE_OFFSET 0
#define AR1DE_MWU_DEVTYPE_DEVTYPE_SIZE 8

/** bit field defines for ar1de_mwu_s#peripheralid4 */
#define AR1DE_MWU_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define AR1DE_MWU_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for ar1de_mwu_s#peripheralid5 */
#define AR1DE_MWU_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define AR1DE_MWU_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for ar1de_mwu_s#peripheralid6 */
#define AR1DE_MWU_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define AR1DE_MWU_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for ar1de_mwu_s#peripheralid7 */
#define AR1DE_MWU_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define AR1DE_MWU_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for ar1de_mwu_s#peripheralid0 */
#define AR1DE_MWU_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define AR1DE_MWU_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for ar1de_mwu_s#peripheralid1 */
#define AR1DE_MWU_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define AR1DE_MWU_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for ar1de_mwu_s#peripheralid2 */
#define AR1DE_MWU_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define AR1DE_MWU_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for ar1de_mwu_s#peripheralid3 */
#define AR1DE_MWU_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define AR1DE_MWU_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for ar1de_mwu_s#componentid0 */
#define AR1DE_MWU_COMPONENTID0_COMPONENTID0_OFFSET 0
#define AR1DE_MWU_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for ar1de_mwu_s#componentid1 */
#define AR1DE_MWU_COMPONENTID1_COMPONENTID1_OFFSET 0
#define AR1DE_MWU_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for ar1de_mwu_s#componentid2 */
#define AR1DE_MWU_COMPONENTID2_COMPONENTID2_OFFSET 0
#define AR1DE_MWU_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for ar1de_mwu_s#componentid3 */
#define AR1DE_MWU_COMPONENTID3_COMPONENTID3_OFFSET 0
#define AR1DE_MWU_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF ar1de_mwu.h */
#endif
