#ifndef INCLUDED_RFFE
#define INCLUDED_RFFE
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** rffe HAL Spreadsheet version number */
#define RFFE_HAL_VERSION 2

/** MIPI RFFE v1.10a Interface
*/
struct rffe_s {
   /** RFFE Module Control Enable at address offset 0x000, read-write */
   uint32_t rffectls;
   /** RFFE Module Control Disable at address offset 0x004, read-write */
   uint32_t rffectlc;
   /** RFFE Module Control at address offset 0x008, read-write */
   uint32_t rffectl;
   /** RFFE Debug Control Enable at address offset 0x00C, read-write */
   uint32_t rffedbgs;
   /** RFFE Debug Control Disable at address offset 0x010, read-write */
   uint32_t rffedbgc;
   /** RFFE Debug Control at address offset 0x014, read-write */
   uint32_t rffedbg;
   /** RFFE Module Status at address offset 0x018, read-write */
   uint32_t rffestatus;
   /** Interrupt Status Register at address offset 0x01C, read-only */
   uint32_t rffeint;
   /** RFFE Trigger Control at address offset 0x020, read-write */
   uint32_t rffetrig;
   /** Clock Divider Settings at address offset 0x024, read-write */
   uint32_t rffeclk;
   /** Write to the Command & Data FIFO at address offset 0x028, read-write */
   uint32_t rffecmdfifo;
   /** Read FIFO Data Status at address offset 0x02C, read-only */
   uint32_t rfferddatastatus;
   /** Read from the Read Data FIFO at address offset 0x030, read-only */
   uint32_t rfferddata;
   /** Readback for the current number of entries present in each FIFO at address offset 0x034, read-only */
   uint32_t rffefifolevel;
};

/** bit field defines for rffe_s#rffectls */
#define RFFE_RFFECTLS_EN_OFFSET 0
#define RFFE_RFFECTLS_EN_SIZE 1
#define RFFE_RFFECTLS_CLRCMDFIFO_OFFSET 1
#define RFFE_RFFECTLS_CLRCMDFIFO_SIZE 1
#define RFFE_RFFECTLS_CLRRDATAFIFO_OFFSET 2
#define RFFE_RFFECTLS_CLRRDATAFIFO_SIZE 1
#define RFFE_RFFECTLS_INTMASK_OFFSET 3
#define RFFE_RFFECTLS_INTMASK_SIZE 5
/** Interrupt set when the command FIFO is emptied, but either of the Command or Last Command counters are non-zero.  There was a command programming error. */
#define RFFE_RFFECTLS_INTMASK_CMDERR_VALUE 16
/** Interrupt set on Command FIFO Overflow */
#define RFFE_RFFECTLS_INTMASK_CMDOFLOW_VALUE 2
/** Interrupt set when a TCU trigger occurs but there are no complete command sequences available to send. */
#define RFFE_RFFECTLS_INTMASK_MISSEDTRIG_VALUE 8
/** Interrupt set on Read Data FIFO Overflow */
#define RFFE_RFFECTLS_INTMASK_RDOFLOW_VALUE 4
/** Interrupt set on Pending Read */
#define RFFE_RFFECTLS_INTMASK_RDPEND_VALUE 1
#define RFFE_RFFECTLS_RDPARITYRPT_OFFSET 8
#define RFFE_RFFECTLS_RDPARITYRPT_SIZE 1
#define RFFE_RFFECTLS_HSE_OFFSET 9
#define RFFE_RFFECTLS_HSE_SIZE 1
#define RFFE_RFFECTLS_CLKDRIVEMODE_OFFSET 10
#define RFFE_RFFECTLS_CLKDRIVEMODE_SIZE 1
/** Always Drive SCLK */
#define RFFE_RFFECTLS_CLKDRIVEMODE_ALWAYS_VALUE 1
/** Drive when RffeCtlEn = 1 */
#define RFFE_RFFECTLS_CLKDRIVEMODE_USEEN_VALUE 0
#define RFFE_RFFECTLS_TRIGMODE_OFFSET 11
#define RFFE_RFFECTLS_TRIGMODE_SIZE 1

/** bit field defines for rffe_s#rffectlc */
#define RFFE_RFFECTLC_EN_OFFSET 0
#define RFFE_RFFECTLC_EN_SIZE 1
#define RFFE_RFFECTLC_CLRCMDFIFO_OFFSET 1
#define RFFE_RFFECTLC_CLRCMDFIFO_SIZE 1
#define RFFE_RFFECTLC_CLRRDATAFIFO_OFFSET 2
#define RFFE_RFFECTLC_CLRRDATAFIFO_SIZE 1
#define RFFE_RFFECTLC_INTMASK_OFFSET 3
#define RFFE_RFFECTLC_INTMASK_SIZE 5
/** Interrupt set when the command FIFO is emptied, but either of the Command or Last Command counters are non-zero.  There was a command programming error. */
#define RFFE_RFFECTLC_INTMASK_CMDERR_VALUE 16
/** Interrupt set on Command FIFO Overflow */
#define RFFE_RFFECTLC_INTMASK_CMDOFLOW_VALUE 2
/** Interrupt set when a TCU trigger occurs but there are no complete command sequences available to send. */
#define RFFE_RFFECTLC_INTMASK_MISSEDTRIG_VALUE 8
/** Interrupt set on Read Data FIFO Overflow */
#define RFFE_RFFECTLC_INTMASK_RDOFLOW_VALUE 4
/** Interrupt set on Pending Read */
#define RFFE_RFFECTLC_INTMASK_RDPEND_VALUE 1
#define RFFE_RFFECTLC_RDPARITYRPT_OFFSET 8
#define RFFE_RFFECTLC_RDPARITYRPT_SIZE 1
#define RFFE_RFFECTLC_HSE_OFFSET 9
#define RFFE_RFFECTLC_HSE_SIZE 1
#define RFFE_RFFECTLC_CLKDRIVEMODE_OFFSET 10
#define RFFE_RFFECTLC_CLKDRIVEMODE_SIZE 1
/** Always Drive SCLK */
#define RFFE_RFFECTLC_CLKDRIVEMODE_ALWAYS_VALUE 1
/** Drive when RffeCtlEn = 1 */
#define RFFE_RFFECTLC_CLKDRIVEMODE_USEEN_VALUE 0
#define RFFE_RFFECTLC_TRIGMODE_OFFSET 11
#define RFFE_RFFECTLC_TRIGMODE_SIZE 1

/** bit field defines for rffe_s#rffectl */
#define RFFE_RFFECTL_EN_OFFSET 0
#define RFFE_RFFECTL_EN_SIZE 1
#define RFFE_RFFECTL_CLRCMDFIFO_OFFSET 1
#define RFFE_RFFECTL_CLRCMDFIFO_SIZE 1
#define RFFE_RFFECTL_CLRRDATAFIFO_OFFSET 2
#define RFFE_RFFECTL_CLRRDATAFIFO_SIZE 1
#define RFFE_RFFECTL_INTMASK_OFFSET 3
#define RFFE_RFFECTL_INTMASK_SIZE 5
/** Interrupt set when the command FIFO is emptied, but either of the Command or Last Command counters are non-zero.  There was a command programming error. */
#define RFFE_RFFECTL_INTMASK_CMDERR_VALUE 16
/** Interrupt set on Command FIFO Overflow */
#define RFFE_RFFECTL_INTMASK_CMDOFLOW_VALUE 2
/** Interrupt set when a TCU trigger occurs but there are no complete command sequences available to send. */
#define RFFE_RFFECTL_INTMASK_MISSEDTRIG_VALUE 8
/** Interrupt set on Read Data FIFO Overflow */
#define RFFE_RFFECTL_INTMASK_RDOFLOW_VALUE 4
/** Interrupt set on Pending Read */
#define RFFE_RFFECTL_INTMASK_RDPEND_VALUE 1
#define RFFE_RFFECTL_RDPARITYRPT_OFFSET 8
#define RFFE_RFFECTL_RDPARITYRPT_SIZE 1
#define RFFE_RFFECTL_HSE_OFFSET 9
#define RFFE_RFFECTL_HSE_SIZE 1
#define RFFE_RFFECTL_CLKDRIVEMODE_OFFSET 10
#define RFFE_RFFECTL_CLKDRIVEMODE_SIZE 1
/** Always Drive SCLK */
#define RFFE_RFFECTL_CLKDRIVEMODE_ALWAYS_VALUE 1
/** Drive when RffeCtlEn = 1 */
#define RFFE_RFFECTL_CLKDRIVEMODE_USEEN_VALUE 0
#define RFFE_RFFECTL_TRIGMODE_OFFSET 11
#define RFFE_RFFECTL_TRIGMODE_SIZE 1

/** bit field defines for rffe_s#rffedbgs */
#define RFFE_RFFEDBGS_INVERTPARITY_OFFSET 0
#define RFFE_RFFEDBGS_INVERTPARITY_SIZE 1
#define RFFE_RFFEDBGS_DECCMDDIS_OFFSET 1
#define RFFE_RFFEDBGS_DECCMDDIS_SIZE 1
#define RFFE_RFFEDBGS_IGNCMDCNTR_OFFSET 2
#define RFFE_RFFEDBGS_IGNCMDCNTR_SIZE 1

/** bit field defines for rffe_s#rffedbgc */
#define RFFE_RFFEDBGC_INVERTPARITY_OFFSET 0
#define RFFE_RFFEDBGC_INVERTPARITY_SIZE 1
#define RFFE_RFFEDBGC_DECCMDDIS_OFFSET 1
#define RFFE_RFFEDBGC_DECCMDDIS_SIZE 1
#define RFFE_RFFEDBGC_IGNCMDCNTR_OFFSET 2
#define RFFE_RFFEDBGC_IGNCMDCNTR_SIZE 1

/** bit field defines for rffe_s#rffedbg */
#define RFFE_RFFEDBG_INVERTPARITY_OFFSET 0
#define RFFE_RFFEDBG_INVERTPARITY_SIZE 1
#define RFFE_RFFEDBG_DECCMDDIS_OFFSET 1
#define RFFE_RFFEDBG_DECCMDDIS_SIZE 1
#define RFFE_RFFEDBG_IGNCMDCNTR_OFFSET 2
#define RFFE_RFFEDBG_IGNCMDCNTR_SIZE 1

/** bit field defines for rffe_s#rffestatus */
#define RFFE_RFFESTATUS_STATEMPTY_OFFSET 0
#define RFFE_RFFESTATUS_STATEMPTY_SIZE 1
#define RFFE_RFFESTATUS_STATIDLE_OFFSET 1
#define RFFE_RFFESTATUS_STATIDLE_SIZE 1
#define RFFE_RFFESTATUS_STATWAITTRIG_OFFSET 2
#define RFFE_RFFESTATUS_STATWAITTRIG_SIZE 1
#define RFFE_RFFESTATUS_STATCMDEMPTY_OFFSET 3
#define RFFE_RFFESTATUS_STATCMDEMPTY_SIZE 1
#define RFFE_RFFESTATUS_STATCMDFULL_OFFSET 4
#define RFFE_RFFESTATUS_STATCMDFULL_SIZE 1
#define RFFE_RFFESTATUS_STATCMDOFLOW_OFFSET 5
#define RFFE_RFFESTATUS_STATCMDOFLOW_SIZE 1
#define RFFE_RFFESTATUS_STATRDEMPTY_OFFSET 6
#define RFFE_RFFESTATUS_STATRDEMPTY_SIZE 1
#define RFFE_RFFESTATUS_STATRDFULL_OFFSET 7
#define RFFE_RFFESTATUS_STATRDFULL_SIZE 1
#define RFFE_RFFESTATUS_STATRDOFLOW_OFFSET 8
#define RFFE_RFFESTATUS_STATRDOFLOW_SIZE 1
#define RFFE_RFFESTATUS_RDPEND_OFFSET 9
#define RFFE_RFFESTATUS_RDPEND_SIZE 1
#define RFFE_RFFESTATUS_CMDERROR_OFFSET 10
#define RFFE_RFFESTATUS_CMDERROR_SIZE 1
#define RFFE_RFFESTATUS_MISSEDTRIG_OFFSET 11
#define RFFE_RFFESTATUS_MISSEDTRIG_SIZE 1
#define RFFE_RFFESTATUS_CMDFIFONUMCMD_OFFSET 12
#define RFFE_RFFESTATUS_CMDFIFONUMCMD_SIZE 5
#define RFFE_RFFESTATUS_CMDFIFONUMLAST_OFFSET 17
#define RFFE_RFFESTATUS_CMDFIFONUMLAST_SIZE 5

/** bit field defines for rffe_s#rffeint */
#define RFFE_RFFEINT_INTPEND_OFFSET 0
#define RFFE_RFFEINT_INTPEND_SIZE 1
#define RFFE_RFFEINT_RDPEND_OFFSET 1
#define RFFE_RFFEINT_RDPEND_SIZE 1
#define RFFE_RFFEINT_CMDOFLOW_OFFSET 2
#define RFFE_RFFEINT_CMDOFLOW_SIZE 1
#define RFFE_RFFEINT_RDOFLOW_OFFSET 3
#define RFFE_RFFEINT_RDOFLOW_SIZE 1
#define RFFE_RFFEINT_MISSEDTRIG_OFFSET 4
#define RFFE_RFFEINT_MISSEDTRIG_SIZE 1
#define RFFE_RFFEINT_CMDERROR_OFFSET 5
#define RFFE_RFFEINT_CMDERROR_SIZE 1

/** bit field defines for rffe_s#rffetrig */
#define RFFE_RFFETRIG_RFFETCUTRIGSTAT_OFFSET 0
#define RFFE_RFFETRIG_RFFETCUTRIGSTAT_SIZE 1
#define RFFE_RFFETRIG_RFFETRIGMAN_OFFSET 1
#define RFFE_RFFETRIG_RFFETRIGMAN_SIZE 1

/** bit field defines for rffe_s#rffeclk */
#define RFFE_RFFECLK_DIV_OFFSET 0
#define RFFE_RFFECLK_DIV_SIZE 8

/** bit field defines for rffe_s#rffecmdfifo */
#define RFFE_RFFECMDFIFO_DATA0_OFFSET 0
#define RFFE_RFFECMDFIFO_DATA0_SIZE 12
#define RFFE_RFFECMDFIFO_CMDDATA0N_OFFSET 12
#define RFFE_RFFECMDFIFO_CMDDATA0N_SIZE 1
#define RFFE_RFFECMDFIFO_NEXT0_OFFSET 13
#define RFFE_RFFECMDFIFO_NEXT0_SIZE 1
#define RFFE_RFFECMDFIFO_LAST0_OFFSET 14
#define RFFE_RFFECMDFIFO_LAST0_SIZE 1
#define RFFE_RFFECMDFIFO_TRIG0_OFFSET 15
#define RFFE_RFFECMDFIFO_TRIG0_SIZE 1
#define RFFE_RFFECMDFIFO_DATA1_OFFSET 16
#define RFFE_RFFECMDFIFO_DATA1_SIZE 8
#define RFFE_RFFECMDFIFO_NEXT1_OFFSET 24
#define RFFE_RFFECMDFIFO_NEXT1_SIZE 1
#define RFFE_RFFECMDFIFO_LAST1_OFFSET 25
#define RFFE_RFFECMDFIFO_LAST1_SIZE 1
#define RFFE_RFFECMDFIFO_RDID_OFFSET 26
#define RFFE_RFFECMDFIFO_RDID_SIZE 4

/** bit field defines for rffe_s#rfferddatastatus */
#define RFFE_RFFERDDATASTATUS_COUNT_OFFSET 0
#define RFFE_RFFERDDATASTATUS_COUNT_SIZE 5

/** bit field defines for rffe_s#rfferddata */
#define RFFE_RFFERDDATA_NONE_OFFSET 0
#define RFFE_RFFERDDATA_NONE_SIZE 32

/** bit field defines for rffe_s#rffefifolevel */
#define RFFE_RFFEFIFOLEVEL_CMD_OFFSET 0
#define RFFE_RFFEFIFOLEVEL_CMD_SIZE 5
#define RFFE_RFFEFIFOLEVEL_RDDATA_OFFSET 8
#define RFFE_RFFEFIFOLEVEL_RDDATA_SIZE 5

/* EOF rffe.h */
#endif
