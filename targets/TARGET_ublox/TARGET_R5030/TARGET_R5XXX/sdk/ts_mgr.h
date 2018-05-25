#ifndef INCLUDED_TS_MGR
#define INCLUDED_TS_MGR
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** ts_mgr HAL Spreadsheet version number */
#define TS_MGR_HAL_VERSION 1

/** RTC and Timestamp Manager
*/
struct ts_mgr_s {
   /** RTC and Timestamp Manager Control Enable at address offset 0x000, read-write */
   uint32_t tsmgrctls;
   /** RTC and Timestamp Manager Control Disable at address offset 0x004, read-write */
   uint32_t tsmgrctlc;
   /** RTC and Timestamp Manager Control at address offset 0x008, read-write */
   uint32_t tsmgrctl;
   /** Update Shadow Registers at address offset 0x00C, read-write */
   uint32_t tsmgrsdwupd;
   /** RTC Clock Divide Ratio. IncrementRate = ClkInRate/(Reg+1)  (requires shadow copy) at address offset 0x010, read-write */
   uint32_t tsmgrrtcn;
   /** Fractional RTC Increment per tick  (requires shadow copy) at address offset 0x014, read-write */
   uint32_t tsmgrrtcinc;
   /** RTC Control (requires shadow copy) at address offset 0x018, read-write */
   uint32_t tsmgrrtccfg;
   /** RTC Count at address offset 0x01C, read-write */
   uint32_t tsmgrrtcint;
   /** Current captured RTC Count - fractional part at address offset 0x020, read-only */
   uint32_t tsmgrrtcfrac;
   /** Subframe Duration (integer) in divided RTC ticks  (requires shadow copy). at address offset 0x024, read-write */
   uint32_t tsmgrsfdurint;
   /** Ttas0 Value  (requires shadow copy) at address offset 0x028, read-write */
   uint32_t tsmgrttas0;
   /** Ttas1 Value  (requires shadow copy) at address offset 0x02C, read-write */
   uint32_t tsmgrttas1;
   /** Ttas2 Value  (requires shadow copy) at address offset 0x030, read-write */
   uint32_t tsmgrttas2;
   /** Ttas3 Value  (requires shadow copy) at address offset 0x034, read-write */
   uint32_t tsmgrttas3;
   /** Timestamp Manager Control at address offset 0x038, read-write */
   uint32_t tsmgrsfcfg;
   /** Timestamp 0 at address offset 0x03C, read-only */
   uint32_t tsmgrts0;
   /** Timestamp 1 at address offset 0x040, read-only */
   uint32_t tsmgrts1;
   /** Timestamp 2 at address offset 0x044, read-only */
   uint32_t tsmgrts2;
   /** Timestamp 3 at address offset 0x048, read-only */
   uint32_t tsmgrts3;
   /** Interrupt Mask at address offset 0x04C, read-write */
   uint32_t tsmgrirqmask;
   /** Interrupt Status. Set by HW, Write 1 to clear each bit at address offset 0x050, read-write */
   uint32_t tsmgrirqstatus;
   /** Configuration for when TSUpdateSrc uses TS_SYNC for timing at address offset 0x054, read-write */
   uint32_t tssynccfg;
};

/** bit field defines for ts_mgr_s#tsmgrctls */
#define TS_MGR_TSMGRCTLS_EN_OFFSET 0
#define TS_MGR_TSMGRCTLS_EN_SIZE 1
#define TS_MGR_TSMGRCTLS_FRACEN_OFFSET 1
#define TS_MGR_TSMGRCTLS_FRACEN_SIZE 1
#define TS_MGR_TSMGRCTLS_FORCECLKEN_OFFSET 2
#define TS_MGR_TSMGRCTLS_FORCECLKEN_SIZE 1
#define TS_MGR_TSMGRCTLS_SBFEN_OFFSET 3
#define TS_MGR_TSMGRCTLS_SBFEN_SIZE 4
#define TS_MGR_TSMGRCTLS_TSUPDATESRC_OFFSET 7
#define TS_MGR_TSMGRCTLS_TSUPDATESRC_SIZE 1
/** Use TAS for Timestamp Update */
#define TS_MGR_TSMGRCTLS_TSUPDATESRC_TAS_VALUE 0
/** Use TS_SYNC for Timestamp Update */
#define TS_MGR_TSMGRCTLS_TSUPDATESRC_TS_SYNC_VALUE 1

/** bit field defines for ts_mgr_s#tsmgrctlc */
#define TS_MGR_TSMGRCTLC_EN_OFFSET 0
#define TS_MGR_TSMGRCTLC_EN_SIZE 1
#define TS_MGR_TSMGRCTLC_FRACEN_OFFSET 1
#define TS_MGR_TSMGRCTLC_FRACEN_SIZE 1
#define TS_MGR_TSMGRCTLC_FORCECLKEN_OFFSET 2
#define TS_MGR_TSMGRCTLC_FORCECLKEN_SIZE 1
#define TS_MGR_TSMGRCTLC_SBFEN_OFFSET 3
#define TS_MGR_TSMGRCTLC_SBFEN_SIZE 4
#define TS_MGR_TSMGRCTLC_TSUPDATESRC_OFFSET 7
#define TS_MGR_TSMGRCTLC_TSUPDATESRC_SIZE 1
/** Use TAS for Timestamp Update */
#define TS_MGR_TSMGRCTLC_TSUPDATESRC_TAS_VALUE 0
/** Use TS_SYNC for Timestamp Update */
#define TS_MGR_TSMGRCTLC_TSUPDATESRC_TS_SYNC_VALUE 1

/** bit field defines for ts_mgr_s#tsmgrctl */
#define TS_MGR_TSMGRCTL_EN_OFFSET 0
#define TS_MGR_TSMGRCTL_EN_SIZE 1
#define TS_MGR_TSMGRCTL_FRACEN_OFFSET 1
#define TS_MGR_TSMGRCTL_FRACEN_SIZE 1
#define TS_MGR_TSMGRCTL_FORCECLKEN_OFFSET 2
#define TS_MGR_TSMGRCTL_FORCECLKEN_SIZE 1
#define TS_MGR_TSMGRCTL_SBFEN_OFFSET 3
#define TS_MGR_TSMGRCTL_SBFEN_SIZE 4
#define TS_MGR_TSMGRCTL_TSUPDATESRC_OFFSET 7
#define TS_MGR_TSMGRCTL_TSUPDATESRC_SIZE 1
/** Use TAS for Timestamp Update */
#define TS_MGR_TSMGRCTL_TSUPDATESRC_TAS_VALUE 0
/** Use TS_SYNC for Timestamp Update */
#define TS_MGR_TSMGRCTL_TSUPDATESRC_TS_SYNC_VALUE 1

/** bit field defines for ts_mgr_s#tsmgrsdwupd */
#define TS_MGR_TSMGRSDWUPD_NONE_OFFSET 0
#define TS_MGR_TSMGRSDWUPD_NONE_SIZE 1

/** bit field defines for ts_mgr_s#tsmgrrtcn */
#define TS_MGR_TSMGRRTCN_NONE_OFFSET 0
#define TS_MGR_TSMGRRTCN_NONE_SIZE 2

/** bit field defines for ts_mgr_s#tsmgrrtcinc */
#define TS_MGR_TSMGRRTCINC_NONE_OFFSET 0
#define TS_MGR_TSMGRRTCINC_NONE_SIZE 25

/** bit field defines for ts_mgr_s#tsmgrrtccfg */
#define TS_MGR_TSMGRRTCCFG_RTCEN_OFFSET 0
#define TS_MGR_TSMGRRTCCFG_RTCEN_SIZE 1
#define TS_MGR_TSMGRRTCCFG_RTCSEL_OFFSET 1
#define TS_MGR_TSMGRRTCCFG_RTCSEL_SIZE 1
/** Use TsMgrSdwUpd write to update RTC parameters */
#define TS_MGR_TSMGRRTCCFG_RTCSEL_REG_VALUE 0
/** Use Scheduler event to update RTC parameters */
#define TS_MGR_TSMGRRTCCFG_RTCSEL_SCH_VALUE 1

/** bit field defines for ts_mgr_s#tsmgrrtcint */
#define TS_MGR_TSMGRRTCINT_VAL_OFFSET 0
#define TS_MGR_TSMGRRTCINT_VAL_SIZE 18
#define TS_MGR_TSMGRRTCINT_RTCVALID_OFFSET 30
#define TS_MGR_TSMGRRTCINT_RTCVALID_SIZE 1
#define TS_MGR_TSMGRRTCINT_RTCGET_OFFSET 31
#define TS_MGR_TSMGRRTCINT_RTCGET_SIZE 1

/** bit field defines for ts_mgr_s#tsmgrrtcfrac */
#define TS_MGR_TSMGRRTCFRAC_NONE_OFFSET 0
#define TS_MGR_TSMGRRTCFRAC_NONE_SIZE 24

/** bit field defines for ts_mgr_s#tsmgrsfdurint */
#define TS_MGR_TSMGRSFDURINT_NONE_OFFSET 0
#define TS_MGR_TSMGRSFDURINT_NONE_SIZE 18

/** bit field defines for ts_mgr_s#tsmgrttas0 */
#define TS_MGR_TSMGRTTAS0_NONE_OFFSET 0
#define TS_MGR_TSMGRTTAS0_NONE_SIZE 18

/** bit field defines for ts_mgr_s#tsmgrttas1 */
#define TS_MGR_TSMGRTTAS1_NONE_OFFSET 0
#define TS_MGR_TSMGRTTAS1_NONE_SIZE 18

/** bit field defines for ts_mgr_s#tsmgrttas2 */
#define TS_MGR_TSMGRTTAS2_NONE_OFFSET 0
#define TS_MGR_TSMGRTTAS2_NONE_SIZE 18

/** bit field defines for ts_mgr_s#tsmgrttas3 */
#define TS_MGR_TSMGRTTAS3_NONE_OFFSET 0
#define TS_MGR_TSMGRTTAS3_NONE_SIZE 18

/** bit field defines for ts_mgr_s#tsmgrsfcfg */
#define TS_MGR_TSMGRSFCFG_SFUPDATESEL_OFFSET 0
#define TS_MGR_TSMGRSFCFG_SFUPDATESEL_SIZE 1
/** Timestamp parameters updated by TsMgrSdwUpd register write */
#define TS_MGR_TSMGRSFCFG_SFUPDATESEL_REG_VALUE 0
/** Timestamp parameters updated by scheduler event */
#define TS_MGR_TSMGRSFCFG_SFUPDATESEL_SCH_VALUE 1
#define TS_MGR_TSMGRSFCFG_SFTSUPDATE_OFFSET 1
#define TS_MGR_TSMGRSFCFG_SFTSUPDATE_SIZE 1

/** bit field defines for ts_mgr_s#tsmgrts0 */
#define TS_MGR_TSMGRTS0_VAL_OFFSET 0
#define TS_MGR_TSMGRTS0_VAL_SIZE 18
#define TS_MGR_TSMGRTS0_TAS_OFFSET 18
#define TS_MGR_TSMGRTS0_TAS_SIZE 1

/** bit field defines for ts_mgr_s#tsmgrts1 */
#define TS_MGR_TSMGRTS1_VAL_OFFSET 0
#define TS_MGR_TSMGRTS1_VAL_SIZE 18
#define TS_MGR_TSMGRTS1_TAS_OFFSET 18
#define TS_MGR_TSMGRTS1_TAS_SIZE 1

/** bit field defines for ts_mgr_s#tsmgrts2 */
#define TS_MGR_TSMGRTS2_VAL_OFFSET 0
#define TS_MGR_TSMGRTS2_VAL_SIZE 18
#define TS_MGR_TSMGRTS2_TAS_OFFSET 18
#define TS_MGR_TSMGRTS2_TAS_SIZE 1

/** bit field defines for ts_mgr_s#tsmgrts3 */
#define TS_MGR_TSMGRTS3_VAL_OFFSET 0
#define TS_MGR_TSMGRTS3_VAL_SIZE 18
#define TS_MGR_TSMGRTS3_TAS_OFFSET 18
#define TS_MGR_TSMGRTS3_TAS_SIZE 1

/** bit field defines for ts_mgr_s#tsmgrirqmask */
#define TS_MGR_TSMGRIRQMASK_TSUPDATED_OFFSET 0
#define TS_MGR_TSMGRIRQMASK_TSUPDATED_SIZE 4
#define TS_MGR_TSMGRIRQMASK_GLOBAL_OFFSET 4
#define TS_MGR_TSMGRIRQMASK_GLOBAL_SIZE 1

/** bit field defines for ts_mgr_s#tsmgrirqstatus */
#define TS_MGR_TSMGRIRQSTATUS_TSUPDATED_OFFSET 0
#define TS_MGR_TSMGRIRQSTATUS_TSUPDATED_SIZE 4

/** bit field defines for ts_mgr_s#tssynccfg */
#define TS_MGR_TSSYNCCFG_NONE_OFFSET 0
#define TS_MGR_TSSYNCCFG_NONE_SIZE 4

/* EOF ts_mgr.h */
#endif
