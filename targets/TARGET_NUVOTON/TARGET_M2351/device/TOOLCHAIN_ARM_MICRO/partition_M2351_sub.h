/**************************************************************************//**
 * @file     partition_M2351_sub.c
 * @version  V3.00
 * @brief    SAU configuration for secure/nonsecure region settings.
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/

#ifndef PARTITION_M2351_SUB
#define PARTITION_M2351_SUB

/* Secure flash size: 256 KB */
#define NU_TZ_SECURE_FLASH_SIZE     0x40000

/* Secure SRAM size: 24 KB */
#define NU_TZ_SECURE_SRAM_SIZE      0x6000

/* NSC region size: 4 KB */
#define NU_TZ_NSC_REGION_SIZE       0x1000

#endif  /* PARTITION_M2351_SUB */
