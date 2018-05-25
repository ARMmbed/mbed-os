#ifndef INCLUDED_KM_SF
#define INCLUDED_KM_SF
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** km_sf HAL Spreadsheet version number */
#define KM_SF_HAL_VERSION 5

/** Klein Matterhorn Special Function Registers
*/
struct km_sf_s {
   /** Write the magic number 0xAE to the register to gain write access to the Mem Margin control registers at address offset 0x000, read-write */
   uint32_t memmarg;
   /** Each bit controls margin enable of memory group # 31 to 0 at address offset 0x004, read-write */
   uint32_t memmargen0;
   /** Each bit controls margin enable of memory group # 46 to 32 at address offset 0x008, read-write */
   uint32_t memmargen1;
   /** Each bit controls LSB of margin vector to memory group # 31 to 0 at address offset 0x00C, read-write */
   uint32_t memmarglsb0;
   /** Each bit controls LSB of margin vector to memory group # 46 to 32 at address offset 0x010, read-write */
   uint32_t memmarglsb1;
   /** Each bit controls MSB of margin vector to memory group # 31 to 0 at address offset 0x014, read-write */
   uint32_t memmargmsb0;
   /** Each bit controls MSB of margin vector to memory group # 46 to 32 at address offset 0x018, read-write */
   uint32_t memmargmsb1;
   /** RefClk Control Register at address offset 0x01C, read-write */
   uint32_t refclkctrl;
   /** Dynamic Voltage Scaling Control. at address offset 0x020, read-write */
   uint32_t dvshwctrl;
   /** Control for the Peripheral DMA module at address offset 0x024, read-write */
   uint32_t periphdmactrl;
   /** Control for the SDIO Interface at address offset 0x028, read-write */
   uint32_t sdioctrl;
   /** Control for the RFFE Reset issued to the AC on POR or Watchdog Reset at address offset 0x02C, read-write */
   uint32_t rfferset;
   /** OSPI Bug Fix Control for JIRA KM_CSD-880/881. OSPI IP does not support byte accesses in Octal DDR/DTR mode Enable at address offset 0x030, read-write */
   uint32_t ospictrls;
   /** OSPI Bug Fix Control for JIRA KM_CSD-880/881. OSPI IP does not support byte accesses in Octal DDR/DTR mode Disable at address offset 0x034, read-write */
   uint32_t ospictrlc;
   /** OSPI Bug Fix Control for JIRA KM_CSD-880/881. OSPI IP does not support byte accesses in Octal DDR/DTR mode at address offset 0x038, read-write */
   uint32_t ospictrl;
   /** RF Analog Reset Override. Functional override allowing control of resets when km_rf_dig_ccu is powered off or in reset Enable at address offset 0x03C, read-write */
   uint32_t rfanarstorides;
   /** RF Analog Reset Override. Functional override allowing control of resets when km_rf_dig_ccu is powered off or in reset Disable at address offset 0x040, read-write */
   uint32_t rfanarstoridec;
   /** RF Analog Reset Override. Functional override allowing control of resets when km_rf_dig_ccu is powered off or in reset at address offset 0x044, read-write */
   uint32_t rfanarstoride;
};

/** bit field defines for km_sf_s#memmarg */
#define KM_SF_MEMMARG_COOKIE_OFFSET 0
#define KM_SF_MEMMARG_COOKIE_SIZE 8
/** Magic Number */
#define KM_SF_MEMMARG_COOKIE_VAL_VALUE 174

/** bit field defines for km_sf_s#memmargen0 */
#define KM_SF_MEMMARGEN0_NONE_OFFSET 0
#define KM_SF_MEMMARGEN0_NONE_SIZE 32

/** bit field defines for km_sf_s#memmargen1 */
#define KM_SF_MEMMARGEN1_NONE_OFFSET 0
#define KM_SF_MEMMARGEN1_NONE_SIZE 15

/** bit field defines for km_sf_s#memmarglsb0 */
#define KM_SF_MEMMARGLSB0_NONE_OFFSET 0
#define KM_SF_MEMMARGLSB0_NONE_SIZE 32

/** bit field defines for km_sf_s#memmarglsb1 */
#define KM_SF_MEMMARGLSB1_NONE_OFFSET 0
#define KM_SF_MEMMARGLSB1_NONE_SIZE 15

/** bit field defines for km_sf_s#memmargmsb0 */
#define KM_SF_MEMMARGMSB0_NONE_OFFSET 0
#define KM_SF_MEMMARGMSB0_NONE_SIZE 32

/** bit field defines for km_sf_s#memmargmsb1 */
#define KM_SF_MEMMARGMSB1_NONE_OFFSET 0
#define KM_SF_MEMMARGMSB1_NONE_SIZE 15

/** bit field defines for km_sf_s#refclkctrl */
#define KM_SF_REFCLKCTRL_DIV2DISABLE_OFFSET 0
#define KM_SF_REFCLKCTRL_DIV2DISABLE_SIZE 1
/** Divied by 2 Enabled */
#define KM_SF_REFCLKCTRL_DIV2DISABLE_DIV2_VALUE 0
/** Divide by 2 Disabled */
#define KM_SF_REFCLKCTRL_DIV2DISABLE_NODIV_VALUE 1
#define KM_SF_REFCLKCTRL_REFCLKOUTSOURCE_OFFSET 1
#define KM_SF_REFCLKCTRL_REFCLKOUTSOURCE_SIZE 1
/** Source of the RefClk Output is post-Div2 Divider */
#define KM_SF_REFCLKCTRL_REFCLKOUTSOURCE_POSTDIV_VALUE 1
/** Source of the RefClk Output is pre-Div2 Divider */
#define KM_SF_REFCLKCTRL_REFCLKOUTSOURCE_PREDIV_VALUE 0

/** bit field defines for km_sf_s#dvshwctrl */
#define KM_SF_DVSHWCTRL_ENABLE_OFFSET 0
#define KM_SF_DVSHWCTRL_ENABLE_SIZE 1
#define KM_SF_DVSHWCTRL_ENWDGIRQEVNT_OFFSET 1
#define KM_SF_DVSHWCTRL_ENWDGIRQEVNT_SIZE 1
#define KM_SF_DVSHWCTRL_CTRLPOLARITY_OFFSET 2
#define KM_SF_DVSHWCTRL_CTRLPOLARITY_SIZE 1
#define KM_SF_DVSHWCTRL_OVRDPWRMODE_OFFSET 3
#define KM_SF_DVSHWCTRL_OVRDPWRMODE_SIZE 1
#define KM_SF_DVSHWCTRL_OVRDPWRMODEVAL_OFFSET 4
#define KM_SF_DVSHWCTRL_OVRDPWRMODEVAL_SIZE 3
#define KM_SF_DVSHWCTRL_SYNCOUTENABLE_OFFSET 7
#define KM_SF_DVSHWCTRL_SYNCOUTENABLE_SIZE 1
#define KM_SF_DVSHWCTRL_SYNCOUTMODE_OFFSET 8
#define KM_SF_DVSHWCTRL_SYNCOUTMODE_SIZE 2
#define KM_SF_DVSHWCTRL_HWITHIGH_OFFSET 10
#define KM_SF_DVSHWCTRL_HWITHIGH_SIZE 4
#define KM_SF_DVSHWCTRL_HWITLOW_OFFSET 14
#define KM_SF_DVSHWCTRL_HWITLOW_SIZE 4

/** bit field defines for km_sf_s#periphdmactrl */
#define KM_SF_PERIPHDMACTRL_PRIORITYMODE_OFFSET 0
#define KM_SF_PERIPHDMACTRL_PRIORITYMODE_SIZE 1

/** bit field defines for km_sf_s#sdioctrl */
#define KM_SF_SDIOCTRL_PADDELAYMASK_OFFSET 0
#define KM_SF_SDIOCTRL_PADDELAYMASK_SIZE 2

/** bit field defines for km_sf_s#rfferset */
#define KM_SF_RFFERSET_POLARITY_OFFSET 0
#define KM_SF_RFFERSET_POLARITY_SIZE 1

/** bit field defines for km_sf_s#ospictrls */
#define KM_SF_OSPICTRLS_BYTERDCONVENABLE_OFFSET 0
#define KM_SF_OSPICTRLS_BYTERDCONVENABLE_SIZE 1
#define KM_SF_OSPICTRLS_BYTEWRHRESPENABLE_OFFSET 1
#define KM_SF_OSPICTRLS_BYTEWRHRESPENABLE_SIZE 1
#define KM_SF_OSPICTRLS_BYTERDHRESPENABLE_OFFSET 2
#define KM_SF_OSPICTRLS_BYTERDHRESPENABLE_SIZE 1
#define KM_SF_OSPICTRLS_BYTESWAPENABLE_OFFSET 3
#define KM_SF_OSPICTRLS_BYTESWAPENABLE_SIZE 1

/** bit field defines for km_sf_s#ospictrlc */
#define KM_SF_OSPICTRLC_BYTERDCONVENABLE_OFFSET 0
#define KM_SF_OSPICTRLC_BYTERDCONVENABLE_SIZE 1
#define KM_SF_OSPICTRLC_BYTEWRHRESPENABLE_OFFSET 1
#define KM_SF_OSPICTRLC_BYTEWRHRESPENABLE_SIZE 1
#define KM_SF_OSPICTRLC_BYTERDHRESPENABLE_OFFSET 2
#define KM_SF_OSPICTRLC_BYTERDHRESPENABLE_SIZE 1
#define KM_SF_OSPICTRLC_BYTESWAPENABLE_OFFSET 3
#define KM_SF_OSPICTRLC_BYTESWAPENABLE_SIZE 1

/** bit field defines for km_sf_s#ospictrl */
#define KM_SF_OSPICTRL_BYTERDCONVENABLE_OFFSET 0
#define KM_SF_OSPICTRL_BYTERDCONVENABLE_SIZE 1
#define KM_SF_OSPICTRL_BYTEWRHRESPENABLE_OFFSET 1
#define KM_SF_OSPICTRL_BYTEWRHRESPENABLE_SIZE 1
#define KM_SF_OSPICTRL_BYTERDHRESPENABLE_OFFSET 2
#define KM_SF_OSPICTRL_BYTERDHRESPENABLE_SIZE 1
#define KM_SF_OSPICTRL_BYTESWAPENABLE_OFFSET 3
#define KM_SF_OSPICTRL_BYTESWAPENABLE_SIZE 1

/** bit field defines for km_sf_s#rfanarstorides */
#define KM_SF_RFANARSTORIDES_VAL_OFFSET 0
#define KM_SF_RFANARSTORIDES_VAL_SIZE 8
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDES_VAL_AUX_PRESETXARBI_VALUE 2
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDES_VAL_PLLRX_PRESETXARBI_VALUE 8
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDES_VAL_PLLRX_PLL_RESETN_VALUE 4
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDES_VAL_PLLTX_PRESETXARBI_VALUE 32
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDES_VAL_PLLTX_PLL_RESETN_VALUE 16
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDES_VAL_RX1_PRESETXARBI_VALUE 64
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDES_VAL_TX1_PRESETXARBI_VALUE 128
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDES_VAL_TX1_TXDFERESETXARBI_VALUE 1
#define KM_SF_RFANARSTORIDES_ENABLE_OFFSET 8
#define KM_SF_RFANARSTORIDES_ENABLE_SIZE 1

/** bit field defines for km_sf_s#rfanarstoridec */
#define KM_SF_RFANARSTORIDEC_VAL_OFFSET 0
#define KM_SF_RFANARSTORIDEC_VAL_SIZE 8
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDEC_VAL_AUX_PRESETXARBI_VALUE 2
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDEC_VAL_PLLRX_PRESETXARBI_VALUE 8
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDEC_VAL_PLLRX_PLL_RESETN_VALUE 4
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDEC_VAL_PLLTX_PRESETXARBI_VALUE 32
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDEC_VAL_PLLTX_PLL_RESETN_VALUE 16
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDEC_VAL_RX1_PRESETXARBI_VALUE 64
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDEC_VAL_TX1_PRESETXARBI_VALUE 128
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDEC_VAL_TX1_TXDFERESETXARBI_VALUE 1
#define KM_SF_RFANARSTORIDEC_ENABLE_OFFSET 8
#define KM_SF_RFANARSTORIDEC_ENABLE_SIZE 1

/** bit field defines for km_sf_s#rfanarstoride */
#define KM_SF_RFANARSTORIDE_VAL_OFFSET 0
#define KM_SF_RFANARSTORIDE_VAL_SIZE 8
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDE_VAL_AUX_PRESETXARBI_VALUE 2
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDE_VAL_PLLRX_PRESETXARBI_VALUE 8
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDE_VAL_PLLRX_PLL_RESETN_VALUE 4
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDE_VAL_PLLTX_PRESETXARBI_VALUE 32
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDE_VAL_PLLTX_PLL_RESETN_VALUE 16
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDE_VAL_RX1_PRESETXARBI_VALUE 64
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDE_VAL_TX1_PRESETXARBI_VALUE 128
/** Bit position for this reset. Active Low */
#define KM_SF_RFANARSTORIDE_VAL_TX1_TXDFERESETXARBI_VALUE 1
#define KM_SF_RFANARSTORIDE_ENABLE_OFFSET 8
#define KM_SF_RFANARSTORIDE_ENABLE_SIZE 1

/* EOF km_sf.h */
#endif
