#ifndef INCLUDED_SCH
#define INCLUDED_SCH
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** sch HAL Spreadsheet version number */
#define SCH_HAL_VERSION 100

/** This is the Breithorn Scheduler module register interface
*/
struct sch_s {
   /** Scheduler Configuration Enable at address offset 0x000, read-write */
   uint32_t schcfgs;
   /** Scheduler Configuration Disable at address offset 0x004, read-write */
   uint32_t schcfgc;
   /** Scheduler Configuration at address offset 0x008, read-write */
   uint32_t schcfg;
   /** SCH Queue at address offset 0x00C, read-write */
   uint32_t schq_0;
   /** SCH Queue at address offset 0x010, read-write */
   uint32_t schq_1;
   /** SCH Queue at address offset 0x014, read-write */
   uint32_t schq_2;
   /** SCH Queue at address offset 0x018, read-write */
   uint32_t schq_3;
   /** SCH Queue at address offset 0x01C, read-write */
   uint32_t schq_4;
   /** SCH Queue at address offset 0x020, read-write */
   uint32_t schq_5;
   /** SCH Queue at address offset 0x024, read-write */
   uint32_t schq_6;
   /** SCH Queue at address offset 0x028, read-write */
   uint32_t schq_7;
   /** SCH Queue 13 at address offset 0x02C, read-write */
   uint32_t schq_13;
   /** SCH Queue 14 Most Significant. Causes Q load. Always set the SchQ_14L register prior to writing here.  at address offset 0x030, read-write */
   uint32_t schq_14m;
   /** SCH Queue 14 Least Significant at address offset 0x034, read-write */
   uint32_t schq_14l;
   /** SCH Queue 15 at address offset 0x038, read-write */
   uint32_t schq_15;
   /** Status - Most Significant at address offset 0x03C, read-write */
   uint32_t schstatusm;
   /** Interrupt Status - Least Significant at address offset 0x040, read-write */
   uint32_t schstatusl;
   /** Interrupt Mask - Most Significant at address offset 0x044, read-write */
   uint32_t schintmaskm;
   /** Interrupt Mask - Least Significant at address offset 0x048, read-write */
   uint32_t schintmaskl;
   /** Interrupt Status - Most Significant. Masked SchStatusM.  Write to SchStatusM to clear at address offset 0x04C, read-only */
   uint32_t schintstatusm;
   /** Interrupt Status - Least Significant.  Masked SchStatusL.  Write to SchStatusL to clear at address offset 0x050, read-only */
   uint32_t schintstatusl;
   /** Clear the Queue associated with each bit.  Write 1 then 0. at address offset 0x054, read-write */
   uint32_t schqclear;
   /** Debug Register at address offset 0x058, read-only */
   uint32_t schdbg;
   /** Number of module clock cycles per pulse output (min). 0 = 1, 1 = 2, 2 = 3, 3 = 4 at address offset 0x05C, read-write */
   uint32_t schpulsewidth;
};

/** bit field defines for sch_s#schcfgs */
#define SCH_SCHCFGS_TX1EN_OFFSET 0
#define SCH_SCHCFGS_TX1EN_SIZE 1
#define SCH_SCHCFGS_TX2EN_OFFSET 1
#define SCH_SCHCFGS_TX2EN_SIZE 1
#define SCH_SCHCFGS_RX1EN_OFFSET 4
#define SCH_SCHCFGS_RX1EN_SIZE 1
#define SCH_SCHCFGS_RX2EN_OFFSET 5
#define SCH_SCHCFGS_RX2EN_SIZE 1
#define SCH_SCHCFGS_PLL1EN_OFFSET 8
#define SCH_SCHCFGS_PLL1EN_SIZE 1
#define SCH_SCHCFGS_PLL2EN_OFFSET 9
#define SCH_SCHCFGS_PLL2EN_SIZE 1
#define SCH_SCHCFGS_AUXEN_OFFSET 11
#define SCH_SCHCFGS_AUXEN_SIZE 1
#define SCH_SCHCFGS_MASEN_OFFSET 12
#define SCH_SCHCFGS_MASEN_SIZE 1
#define SCH_SCHCFGS_CLKSWEN_OFFSET 13
#define SCH_SCHCFGS_CLKSWEN_SIZE 1
#define SCH_SCHCFGS_RFFEEN_OFFSET 14
#define SCH_SCHCFGS_RFFEEN_SIZE 1
#define SCH_SCHCFGS_CPUEN_OFFSET 15
#define SCH_SCHCFGS_CPUEN_SIZE 1
#define SCH_SCHCFGS_ERR_OFFSET 16
#define SCH_SCHCFGS_ERR_SIZE 1
/** Ignore late commands and discard.  Raise an error interrupt if mask is enabled */
#define SCH_SCHCFGS_ERR_IGN_W_ERR_INT_VALUE 1
/** Send late commands immediately.  Raise an error interrupt if mask is enabled */
#define SCH_SCHCFGS_ERR_IMM_W_ERR_INT_VALUE 0
#define SCH_SCHCFGS_FRACRTCMODE_OFFSET 17
#define SCH_SCHCFGS_FRACRTCMODE_SIZE 1
#define SCH_SCHCFGS_FORCECLKEN_OFFSET 18
#define SCH_SCHCFGS_FORCECLKEN_SIZE 1
#define SCH_SCHCFGS_IRQSYNCENABLE_OFFSET 19
#define SCH_SCHCFGS_IRQSYNCENABLE_SIZE 1

/** bit field defines for sch_s#schcfgc */
#define SCH_SCHCFGC_TX1EN_OFFSET 0
#define SCH_SCHCFGC_TX1EN_SIZE 1
#define SCH_SCHCFGC_TX2EN_OFFSET 1
#define SCH_SCHCFGC_TX2EN_SIZE 1
#define SCH_SCHCFGC_RX1EN_OFFSET 4
#define SCH_SCHCFGC_RX1EN_SIZE 1
#define SCH_SCHCFGC_RX2EN_OFFSET 5
#define SCH_SCHCFGC_RX2EN_SIZE 1
#define SCH_SCHCFGC_PLL1EN_OFFSET 8
#define SCH_SCHCFGC_PLL1EN_SIZE 1
#define SCH_SCHCFGC_PLL2EN_OFFSET 9
#define SCH_SCHCFGC_PLL2EN_SIZE 1
#define SCH_SCHCFGC_AUXEN_OFFSET 11
#define SCH_SCHCFGC_AUXEN_SIZE 1
#define SCH_SCHCFGC_MASEN_OFFSET 12
#define SCH_SCHCFGC_MASEN_SIZE 1
#define SCH_SCHCFGC_CLKSWEN_OFFSET 13
#define SCH_SCHCFGC_CLKSWEN_SIZE 1
#define SCH_SCHCFGC_RFFEEN_OFFSET 14
#define SCH_SCHCFGC_RFFEEN_SIZE 1
#define SCH_SCHCFGC_CPUEN_OFFSET 15
#define SCH_SCHCFGC_CPUEN_SIZE 1
#define SCH_SCHCFGC_ERR_OFFSET 16
#define SCH_SCHCFGC_ERR_SIZE 1
/** Ignore late commands and discard.  Raise an error interrupt if mask is enabled */
#define SCH_SCHCFGC_ERR_IGN_W_ERR_INT_VALUE 1
/** Send late commands immediately.  Raise an error interrupt if mask is enabled */
#define SCH_SCHCFGC_ERR_IMM_W_ERR_INT_VALUE 0
#define SCH_SCHCFGC_FRACRTCMODE_OFFSET 17
#define SCH_SCHCFGC_FRACRTCMODE_SIZE 1
#define SCH_SCHCFGC_FORCECLKEN_OFFSET 18
#define SCH_SCHCFGC_FORCECLKEN_SIZE 1
#define SCH_SCHCFGC_IRQSYNCENABLE_OFFSET 19
#define SCH_SCHCFGC_IRQSYNCENABLE_SIZE 1

/** bit field defines for sch_s#schcfg */
#define SCH_SCHCFG_TX1EN_OFFSET 0
#define SCH_SCHCFG_TX1EN_SIZE 1
#define SCH_SCHCFG_TX2EN_OFFSET 1
#define SCH_SCHCFG_TX2EN_SIZE 1
#define SCH_SCHCFG_RX1EN_OFFSET 4
#define SCH_SCHCFG_RX1EN_SIZE 1
#define SCH_SCHCFG_RX2EN_OFFSET 5
#define SCH_SCHCFG_RX2EN_SIZE 1
#define SCH_SCHCFG_PLL1EN_OFFSET 8
#define SCH_SCHCFG_PLL1EN_SIZE 1
#define SCH_SCHCFG_PLL2EN_OFFSET 9
#define SCH_SCHCFG_PLL2EN_SIZE 1
#define SCH_SCHCFG_AUXEN_OFFSET 11
#define SCH_SCHCFG_AUXEN_SIZE 1
#define SCH_SCHCFG_MASEN_OFFSET 12
#define SCH_SCHCFG_MASEN_SIZE 1
#define SCH_SCHCFG_CLKSWEN_OFFSET 13
#define SCH_SCHCFG_CLKSWEN_SIZE 1
#define SCH_SCHCFG_RFFEEN_OFFSET 14
#define SCH_SCHCFG_RFFEEN_SIZE 1
#define SCH_SCHCFG_CPUEN_OFFSET 15
#define SCH_SCHCFG_CPUEN_SIZE 1
#define SCH_SCHCFG_ERR_OFFSET 16
#define SCH_SCHCFG_ERR_SIZE 1
/** Ignore late commands and discard.  Raise an error interrupt if mask is enabled */
#define SCH_SCHCFG_ERR_IGN_W_ERR_INT_VALUE 1
/** Send late commands immediately.  Raise an error interrupt if mask is enabled */
#define SCH_SCHCFG_ERR_IMM_W_ERR_INT_VALUE 0
#define SCH_SCHCFG_FRACRTCMODE_OFFSET 17
#define SCH_SCHCFG_FRACRTCMODE_SIZE 1
#define SCH_SCHCFG_FORCECLKEN_OFFSET 18
#define SCH_SCHCFG_FORCECLKEN_SIZE 1
#define SCH_SCHCFG_IRQSYNCENABLE_OFFSET 19
#define SCH_SCHCFG_IRQSYNCENABLE_SIZE 1

/** bit field defines for sch_s#schq_0 */
#define SCH_SCHQ_0_VAL_OFFSET 0
#define SCH_SCHQ_0_VAL_SIZE 5
#define SCH_SCHQ_0_IGNTIME_OFFSET 6
#define SCH_SCHQ_0_IGNTIME_SIZE 1
#define SCH_SCHQ_0_CB_OFFSET 7
#define SCH_SCHQ_0_CB_SIZE 1
#define SCH_SCHQ_0_TS_OFFSET 8
#define SCH_SCHQ_0_TS_SIZE 18
#define SCH_SCHQ_0_EMPTY_OFFSET 30
#define SCH_SCHQ_0_EMPTY_SIZE 1
#define SCH_SCHQ_0_FULL_OFFSET 31
#define SCH_SCHQ_0_FULL_SIZE 1

/** bit field defines for sch_s#schq_1 */
#define SCH_SCHQ_1_VAL_OFFSET 0
#define SCH_SCHQ_1_VAL_SIZE 5
#define SCH_SCHQ_1_IGNTIME_OFFSET 6
#define SCH_SCHQ_1_IGNTIME_SIZE 1
#define SCH_SCHQ_1_CB_OFFSET 7
#define SCH_SCHQ_1_CB_SIZE 1
#define SCH_SCHQ_1_TS_OFFSET 8
#define SCH_SCHQ_1_TS_SIZE 18
#define SCH_SCHQ_1_EMPTY_OFFSET 30
#define SCH_SCHQ_1_EMPTY_SIZE 1
#define SCH_SCHQ_1_FULL_OFFSET 31
#define SCH_SCHQ_1_FULL_SIZE 1

/** bit field defines for sch_s#schq_2 */
#define SCH_SCHQ_2_VAL_OFFSET 0
#define SCH_SCHQ_2_VAL_SIZE 5
#define SCH_SCHQ_2_IGNTIME_OFFSET 6
#define SCH_SCHQ_2_IGNTIME_SIZE 1
#define SCH_SCHQ_2_CB_OFFSET 7
#define SCH_SCHQ_2_CB_SIZE 1
#define SCH_SCHQ_2_TS_OFFSET 8
#define SCH_SCHQ_2_TS_SIZE 18
#define SCH_SCHQ_2_EMPTY_OFFSET 30
#define SCH_SCHQ_2_EMPTY_SIZE 1
#define SCH_SCHQ_2_FULL_OFFSET 31
#define SCH_SCHQ_2_FULL_SIZE 1

/** bit field defines for sch_s#schq_3 */
#define SCH_SCHQ_3_VAL_OFFSET 0
#define SCH_SCHQ_3_VAL_SIZE 5
#define SCH_SCHQ_3_IGNTIME_OFFSET 6
#define SCH_SCHQ_3_IGNTIME_SIZE 1
#define SCH_SCHQ_3_CB_OFFSET 7
#define SCH_SCHQ_3_CB_SIZE 1
#define SCH_SCHQ_3_TS_OFFSET 8
#define SCH_SCHQ_3_TS_SIZE 18
#define SCH_SCHQ_3_EMPTY_OFFSET 30
#define SCH_SCHQ_3_EMPTY_SIZE 1
#define SCH_SCHQ_3_FULL_OFFSET 31
#define SCH_SCHQ_3_FULL_SIZE 1

/** bit field defines for sch_s#schq_4 */
#define SCH_SCHQ_4_VAL_OFFSET 0
#define SCH_SCHQ_4_VAL_SIZE 5
#define SCH_SCHQ_4_IGNTIME_OFFSET 6
#define SCH_SCHQ_4_IGNTIME_SIZE 1
#define SCH_SCHQ_4_CB_OFFSET 7
#define SCH_SCHQ_4_CB_SIZE 1
#define SCH_SCHQ_4_TS_OFFSET 8
#define SCH_SCHQ_4_TS_SIZE 18
#define SCH_SCHQ_4_EMPTY_OFFSET 30
#define SCH_SCHQ_4_EMPTY_SIZE 1
#define SCH_SCHQ_4_FULL_OFFSET 31
#define SCH_SCHQ_4_FULL_SIZE 1

/** bit field defines for sch_s#schq_5 */
#define SCH_SCHQ_5_VAL_OFFSET 0
#define SCH_SCHQ_5_VAL_SIZE 5
#define SCH_SCHQ_5_IGNTIME_OFFSET 6
#define SCH_SCHQ_5_IGNTIME_SIZE 1
#define SCH_SCHQ_5_CB_OFFSET 7
#define SCH_SCHQ_5_CB_SIZE 1
#define SCH_SCHQ_5_TS_OFFSET 8
#define SCH_SCHQ_5_TS_SIZE 18
#define SCH_SCHQ_5_EMPTY_OFFSET 30
#define SCH_SCHQ_5_EMPTY_SIZE 1
#define SCH_SCHQ_5_FULL_OFFSET 31
#define SCH_SCHQ_5_FULL_SIZE 1

/** bit field defines for sch_s#schq_6 */
#define SCH_SCHQ_6_VAL_OFFSET 0
#define SCH_SCHQ_6_VAL_SIZE 5
#define SCH_SCHQ_6_IGNTIME_OFFSET 6
#define SCH_SCHQ_6_IGNTIME_SIZE 1
#define SCH_SCHQ_6_CB_OFFSET 7
#define SCH_SCHQ_6_CB_SIZE 1
#define SCH_SCHQ_6_TS_OFFSET 8
#define SCH_SCHQ_6_TS_SIZE 18
#define SCH_SCHQ_6_EMPTY_OFFSET 30
#define SCH_SCHQ_6_EMPTY_SIZE 1
#define SCH_SCHQ_6_FULL_OFFSET 31
#define SCH_SCHQ_6_FULL_SIZE 1

/** bit field defines for sch_s#schq_7 */
#define SCH_SCHQ_7_VAL_OFFSET 0
#define SCH_SCHQ_7_VAL_SIZE 5
#define SCH_SCHQ_7_IGNTIME_OFFSET 6
#define SCH_SCHQ_7_IGNTIME_SIZE 1
#define SCH_SCHQ_7_CB_OFFSET 7
#define SCH_SCHQ_7_CB_SIZE 1
#define SCH_SCHQ_7_TS_OFFSET 8
#define SCH_SCHQ_7_TS_SIZE 18
#define SCH_SCHQ_7_EMPTY_OFFSET 30
#define SCH_SCHQ_7_EMPTY_SIZE 1
#define SCH_SCHQ_7_FULL_OFFSET 31
#define SCH_SCHQ_7_FULL_SIZE 1

/** bit field defines for sch_s#schq_13 */
#define SCH_SCHQ_13_RTC0_OFFSET 3
#define SCH_SCHQ_13_RTC0_SIZE 1
#define SCH_SCHQ_13_CCULD_OFFSET 4
#define SCH_SCHQ_13_CCULD_SIZE 1
#define SCH_SCHQ_13_IGNTIME_OFFSET 6
#define SCH_SCHQ_13_IGNTIME_SIZE 1
#define SCH_SCHQ_13_CB_OFFSET 7
#define SCH_SCHQ_13_CB_SIZE 1
#define SCH_SCHQ_13_TS_OFFSET 8
#define SCH_SCHQ_13_TS_SIZE 18
#define SCH_SCHQ_13_EMPTY_OFFSET 30
#define SCH_SCHQ_13_EMPTY_SIZE 1
#define SCH_SCHQ_13_FULL_OFFSET 31
#define SCH_SCHQ_13_FULL_SIZE 1

/** bit field defines for sch_s#schq_14m */
#define SCH_SCHQ_14M_IGNTIME_OFFSET 6
#define SCH_SCHQ_14M_IGNTIME_SIZE 1
#define SCH_SCHQ_14M_CB_OFFSET 7
#define SCH_SCHQ_14M_CB_SIZE 1
#define SCH_SCHQ_14M_TS_OFFSET 8
#define SCH_SCHQ_14M_TS_SIZE 18
#define SCH_SCHQ_14M_EMPTY_OFFSET 30
#define SCH_SCHQ_14M_EMPTY_SIZE 1
#define SCH_SCHQ_14M_FULL_OFFSET 31
#define SCH_SCHQ_14M_FULL_SIZE 1

/** bit field defines for sch_s#schq_14l */
#define SCH_SCHQ_14L_RF1_OFFSET 0
#define SCH_SCHQ_14L_RF1_SIZE 1
#define SCH_SCHQ_14L_GP_OFFSET 16
#define SCH_SCHQ_14L_GP_SIZE 16

/** bit field defines for sch_s#schq_15 */
#define SCH_SCHQ_15_CPUINT_OFFSET 0
#define SCH_SCHQ_15_CPUINT_SIZE 4
#define SCH_SCHQ_15_IGNTIME_OFFSET 6
#define SCH_SCHQ_15_IGNTIME_SIZE 1
#define SCH_SCHQ_15_CB_OFFSET 7
#define SCH_SCHQ_15_CB_SIZE 1
#define SCH_SCHQ_15_TS_OFFSET 8
#define SCH_SCHQ_15_TS_SIZE 18
#define SCH_SCHQ_15_EMPTY_OFFSET 30
#define SCH_SCHQ_15_EMPTY_SIZE 1
#define SCH_SCHQ_15_FULL_OFFSET 31
#define SCH_SCHQ_15_FULL_SIZE 1

/** bit field defines for sch_s#schstatusm */
#define SCH_SCHSTATUSM_QTIMEEXC_OFFSET 0
#define SCH_SCHSTATUSM_QTIMEEXC_SIZE 11
#define SCH_SCHSTATUSM_QLOADERR_OFFSET 16
#define SCH_SCHSTATUSM_QLOADERR_SIZE 11

/** bit field defines for sch_s#schstatusl */
#define SCH_SCHSTATUSL_QCB_OFFSET 0
#define SCH_SCHSTATUSL_QCB_SIZE 11
#define SCH_SCHSTATUSL_CPUINT_OFFSET 16
#define SCH_SCHSTATUSL_CPUINT_SIZE 4

/** bit field defines for sch_s#schintmaskm */
#define SCH_SCHINTMASKM_QTIMEEXC_OFFSET 0
#define SCH_SCHINTMASKM_QTIMEEXC_SIZE 11
#define SCH_SCHINTMASKM_QLOADERR_OFFSET 16
#define SCH_SCHINTMASKM_QLOADERR_SIZE 11

/** bit field defines for sch_s#schintmaskl */
#define SCH_SCHINTMASKL_QCB_OFFSET 0
#define SCH_SCHINTMASKL_QCB_SIZE 11
#define SCH_SCHINTMASKL_CPUINT_OFFSET 16
#define SCH_SCHINTMASKL_CPUINT_SIZE 4

/** bit field defines for sch_s#schintstatusm */
#define SCH_SCHINTSTATUSM_QTIMEEXC_OFFSET 0
#define SCH_SCHINTSTATUSM_QTIMEEXC_SIZE 11
#define SCH_SCHINTSTATUSM_QLOADERR_OFFSET 16
#define SCH_SCHINTSTATUSM_QLOADERR_SIZE 11

/** bit field defines for sch_s#schintstatusl */
#define SCH_SCHINTSTATUSL_QCB_OFFSET 0
#define SCH_SCHINTSTATUSL_QCB_SIZE 11
#define SCH_SCHINTSTATUSL_CPUINT_OFFSET 16
#define SCH_SCHINTSTATUSL_CPUINT_SIZE 4

/** bit field defines for sch_s#schqclear */
#define SCH_SCHQCLEAR_NONE_OFFSET 0
#define SCH_SCHQCLEAR_NONE_SIZE 11

/** bit field defines for sch_s#schdbg */
#define SCH_SCHDBG_Q14_GPO_OFFSET 16
#define SCH_SCHDBG_Q14_GPO_SIZE 16

/** bit field defines for sch_s#schpulsewidth */
#define SCH_SCHPULSEWIDTH_NONE_OFFSET 0
#define SCH_SCHPULSEWIDTH_NONE_SIZE 2

/* EOF sch.h */
#endif
