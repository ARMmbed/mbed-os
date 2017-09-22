#ifndef INCLUDED_KM_PSS
#define INCLUDED_KM_PSS
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** km_pss HAL Spreadsheet version number */
#define KM_PSS_HAL_VERSION 1

/** This is the PSS register module
*/
struct km_pss_s {
   /** Top-level Control for pss_control at address offset 0x000, read-write */
   uint32_t commandctrl;
   /** Status for pss_control at address offset 0x004, read-only */
   uint32_t commandstatus;
   /**  at address offset 0x008, read-write */
   uint32_t commandparam1_0;
   /**  at address offset 0x00C, read-write */
   uint32_t commandparam2_0;
   /**  at address offset 0x010, read-write */
   uint32_t commandparam3_0;
   /**  at address offset 0x014, read-write */
   uint32_t commandparam1_1;
   /**  at address offset 0x018, read-write */
   uint32_t commandparam2_1;
   /**  at address offset 0x01C, read-write */
   uint32_t commandparam3_1;
   /**  at address offset 0x020, read-write */
   uint32_t commandparam1_2;
   /**  at address offset 0x024, read-write */
   uint32_t commandparam2_2;
   /**  at address offset 0x028, read-write */
   uint32_t commandparam3_2;
   /**  at address offset 0x02C, read-write */
   uint32_t commandparam1_3;
   /**  at address offset 0x030, read-write */
   uint32_t commandparam2_3;
   /**  at address offset 0x034, read-write */
   uint32_t commandparam3_3;
   /**  at address offset 0x038, read-write */
   uint32_t commandparam1_4;
   /**  at address offset 0x03C, read-write */
   uint32_t commandparam2_4;
   /**  at address offset 0x040, read-write */
   uint32_t commandparam3_4;
   /**  at address offset 0x044, read-write */
   uint32_t commandparam1_5;
   /**  at address offset 0x048, read-write */
   uint32_t commandparam2_5;
   /**  at address offset 0x04C, read-write */
   uint32_t commandparam3_5;
   /**  at address offset 0x050, read-write */
   uint32_t commandparam1_6;
   /**  at address offset 0x054, read-write */
   uint32_t commandparam2_6;
   /**  at address offset 0x058, read-write */
   uint32_t commandparam3_6;
   /**  at address offset 0x05C, read-write */
   uint32_t commandparam1_7;
   /**  at address offset 0x060, read-write */
   uint32_t commandparam2_7;
   /**  at address offset 0x064, read-write */
   uint32_t commandparam3_7;
   /** Correlator common configuration settings at address offset 0x068, read-write */
   uint32_t commonparam;
   /** Correlator per-configuration context settings at address offset 0x06C, read-write */
   uint32_t corrconfigparam_0;
   /** Correlator per-configuration context settings at address offset 0x070, read-write */
   uint32_t corrconfigparam_1;
   /** Correlator per-configuration context settings at address offset 0x074, read-write */
   uint32_t corrconfigparam_2;
   /** Correlator per-configuration context settings at address offset 0x078, read-write */
   uint32_t corrconfigparam_3;
   /**  at address offset 0x07C, read-only */
   uint32_t fbufstate;
   /**  at address offset 0x080, read-only */
   uint32_t fbufphasestate;
   /**  at address offset 0x084, read-only */
   uint32_t accumcorrstate;
   /**  at address offset 0x088, read-only */
   uint32_t sorterxferstate;
   /**  at address offset 0x08C, read-only */
   uint32_t ramxferstate;
   /**  at address offset 0x090, read-only */
   uint32_t axixferstate;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x094, read-write */
   uint32_t accumnbiot_0;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x098, read-write */
   uint32_t accumnbiot_1;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x09C, read-write */
   uint32_t accumnbiot_2;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x0A0, read-write */
   uint32_t accumnbiot_3;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x0A4, read-write */
   uint32_t accumnbiot_4;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x0A8, read-write */
   uint32_t accumnbiot_5;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x0AC, read-write */
   uint32_t accumnbiot_6;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x0B0, read-write */
   uint32_t accumnbiot_7;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x0B4, read-write */
   uint32_t accumnbiot_8;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x0B8, read-write */
   uint32_t accumnbiot_9;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x0BC, read-write */
   uint32_t accumnbiot_10;
   /** Settings associated with the 12 accumulation buffers used for NB-IoT-PSS at address offset 0x0C0, read-write */
   uint32_t accumnbiot_11;
   /** Settings associated with the 3 accumulation buffers used for LTE-PSS (2 sets of 3, the second set should use a different ConfigContext, can be used for neighbours, with different bandwidth/sorterId values) at address offset 0x0C4, read-write */
   uint32_t accumlte_0;
   /** Settings associated with the 3 accumulation buffers used for LTE-PSS (2 sets of 3, the second set should use a different ConfigContext, can be used for neighbours, with different bandwidth/sorterId values) at address offset 0x0C8, read-write */
   uint32_t accumlte_1;
   /** Settings associated with the 3 accumulation buffers used for LTE-PSS (2 sets of 3, the second set should use a different ConfigContext, can be used for neighbours, with different bandwidth/sorterId values) at address offset 0x0CC, read-write */
   uint32_t accumlte_2;
   /** Settings associated with the 3 accumulation buffers used for LTE-PSS (2 sets of 3, the second set should use a different ConfigContext, can be used for neighbours, with different bandwidth/sorterId values) at address offset 0x0D0, read-write */
   uint32_t accumlte_3;
   /** Settings associated with the 3 accumulation buffers used for LTE-PSS (2 sets of 3, the second set should use a different ConfigContext, can be used for neighbours, with different bandwidth/sorterId values) at address offset 0x0D4, read-write */
   uint32_t accumlte_4;
   /** Settings associated with the 3 accumulation buffers used for LTE-PSS (2 sets of 3, the second set should use a different ConfigContext, can be used for neighbours, with different bandwidth/sorterId values) at address offset 0x0D8, read-write */
   uint32_t accumlte_5;
   /** Sorter configuration (per sorterId).  SSS sorter memory occupies 7936 bytes total, enough to store 31 x 64-sample blocks at address offset 0x0DC, read-write */
   uint32_t sorterconfig_0;
   /** Sorter configuration (per sorterId).  SSS sorter memory occupies 7936 bytes total, enough to store 31 x 64-sample blocks at address offset 0x0E0, read-write */
   uint32_t sorterconfig_1;
   /** Sorter configuration (per sorterId).  SSS sorter memory occupies 7936 bytes total, enough to store 31 x 64-sample blocks at address offset 0x0E4, read-write */
   uint32_t sorterconfig_2;
   /** Sorter configuration (per sorterId).  SSS sorter memory occupies 7936 bytes total, enough to store 31 x 64-sample blocks at address offset 0x0E8, read-write */
   uint32_t sorterconfig_3;
   /** Sorter configuration (per sorterId).  SSS sorter memory occupies 7936 bytes total, enough to store 31 x 64-sample blocks at address offset 0x0EC, read-write */
   uint32_t sorterconfig_4;
   /** Sorter configuration (per sorterId).  SSS sorter memory occupies 7936 bytes total, enough to store 31 x 64-sample blocks at address offset 0x0F0, read-write */
   uint32_t sorterconfig_5;
   /** Sorter configuration (per sorterId).  SSS sorter memory occupies 7936 bytes total, enough to store 31 x 64-sample blocks at address offset 0x0F4, read-write */
   uint32_t sorterconfig_6;
   /** Sorter configuration (per sorterId).  SSS sorter memory occupies 7936 bytes total, enough to store 31 x 64-sample blocks at address offset 0x0F8, read-write */
   uint32_t sorterconfig_7;
   /** Sorter configuration (per sorterId).  SSS sorter config can occupy up to 15 x 64-sample blocks at address offset 0x0FC, read-only */
   uint32_t sorterstatus_0;
   /** Sorter configuration (per sorterId).  SSS sorter config can occupy up to 15 x 64-sample blocks at address offset 0x100, read-only */
   uint32_t sorterstatus_1;
   /** Sorter configuration (per sorterId).  SSS sorter config can occupy up to 15 x 64-sample blocks at address offset 0x104, read-only */
   uint32_t sorterstatus_2;
   /** Sorter configuration (per sorterId).  SSS sorter config can occupy up to 15 x 64-sample blocks at address offset 0x108, read-only */
   uint32_t sorterstatus_3;
   /** Sorter configuration (per sorterId).  SSS sorter config can occupy up to 15 x 64-sample blocks at address offset 0x10C, read-only */
   uint32_t sorterstatus_4;
   /** Sorter configuration (per sorterId).  SSS sorter config can occupy up to 15 x 64-sample blocks at address offset 0x110, read-only */
   uint32_t sorterstatus_5;
   /** Sorter configuration (per sorterId).  SSS sorter config can occupy up to 15 x 64-sample blocks at address offset 0x114, read-only */
   uint32_t sorterstatus_6;
   /** Sorter configuration (per sorterId).  SSS sorter config can occupy up to 15 x 64-sample blocks at address offset 0x118, read-only */
   uint32_t sorterstatus_7;
   /**  at address offset 0x11C, read-write */
   uint32_t sortertransfer;
   /**  at address offset 0x120, read-write */
   uint32_t accumtransfer;
   /**  at address offset 0x124, read-write */
   uint32_t accumswap;
   /** Accum swap memory size, in 2400B units (24 * 2400B = 57600B).  The HW will automatically wrap-around any accumSwap read/write accesses which exceed this size at address offset 0x128, read-write */
   uint32_t accumswapsize;
   /**  at address offset 0x12C, read-write */
   uint32_t accumtransferctrl;
   /** Latched status bits: set by h/w;  cleared by s/w; each bit is set upon completion of the associated command at address offset 0x130, read-write */
   uint32_t eventstatus;
   /** Interrupts are levels, each bit is set upon completion of the associated command at address offset 0x134, read-write */
   uint32_t interruptmask;
   /** Write to EventStatus to clear. at address offset 0x138, read-only */
   uint32_t interruptstatus;
   /** An external event pulse can be generated based on certain state transitions at address offset 0x13C, read-write */
   uint32_t exteventpulse;
   /**  at address offset 0x140, read-write */
   uint32_t hwobs;
};

/** bit field defines for km_pss_s#commandctrl */
#define KM_PSS_COMMANDCTRL_NEXTIDX_OFFSET 0
#define KM_PSS_COMMANDCTRL_NEXTIDX_SIZE 3
#define KM_PSS_COMMANDCTRL_DONEXT_OFFSET 3
#define KM_PSS_COMMANDCTRL_DONEXT_SIZE 1
#define KM_PSS_COMMANDCTRL_RESET_OFFSET 4
#define KM_PSS_COMMANDCTRL_RESET_SIZE 1

/** bit field defines for km_pss_s#commandstatus */
#define KM_PSS_COMMANDSTATUS_NEXTIDX_OFFSET 0
#define KM_PSS_COMMANDSTATUS_NEXTIDX_SIZE 3
#define KM_PSS_COMMANDSTATUS_RUNNINGIDX_OFFSET 3
#define KM_PSS_COMMANDSTATUS_RUNNINGIDX_SIZE 3
#define KM_PSS_COMMANDSTATUS_CURRENTFUNCTION_OFFSET 6
#define KM_PSS_COMMANDSTATUS_CURRENTFUNCTION_SIZE 2

/** bit field defines for km_pss_s#commandparam1_0 */
#define KM_PSS_COMMANDPARAM1_0_FBUFBLOCKSDURATION_OFFSET 0
#define KM_PSS_COMMANDPARAM1_0_FBUFBLOCKSDURATION_SIZE 8
#define KM_PSS_COMMANDPARAM1_0_FBUFPTRSTART_OFFSET 8
#define KM_PSS_COMMANDPARAM1_0_FBUFPTRSTART_SIZE 16

/** bit field defines for km_pss_s#commandparam2_0 */
#define KM_PSS_COMMANDPARAM2_0_ACCUMOFFSET_OFFSET 0
#define KM_PSS_COMMANDPARAM2_0_ACCUMOFFSET_SIZE 9
#define KM_PSS_COMMANDPARAM2_0_ACCUMRESET_OFFSET 9
#define KM_PSS_COMMANDPARAM2_0_ACCUMRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_0_DRX_OFFSET 10
#define KM_PSS_COMMANDPARAM2_0_DRX_SIZE 1
#define KM_PSS_COMMANDPARAM2_0_CONFIGCONTEXT_OFFSET 11
#define KM_PSS_COMMANDPARAM2_0_CONFIGCONTEXT_SIZE 2
#define KM_PSS_COMMANDPARAM2_0_SORTERENABLE_OFFSET 13
#define KM_PSS_COMMANDPARAM2_0_SORTERENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM2_0_SORTERRESET_OFFSET 14
#define KM_PSS_COMMANDPARAM2_0_SORTERRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_0_SORTERTRANSFER_OFFSET 15
#define KM_PSS_COMMANDPARAM2_0_SORTERTRANSFER_SIZE 1
#define KM_PSS_COMMANDPARAM2_0_ACCUMTRANSFER_OFFSET 16
#define KM_PSS_COMMANDPARAM2_0_ACCUMTRANSFER_SIZE 1

/** bit field defines for km_pss_s#commandparam3_0 */
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPEXTREADENABLE_OFFSET 0
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPEXTREADENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPEXTWRITEENABLE_OFFSET 1
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPEXTWRITEENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPINTOFFSET_OFFSET 2
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPINTOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPEXTREADOFFSET_OFFSET 7
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPEXTREADOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPEXTWRITEOFFSET_OFFSET 12
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPEXTWRITEOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPEXTBANKID_OFFSET 17
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPEXTBANKID_SIZE 1
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPXFERSIZE_OFFSET 18
#define KM_PSS_COMMANDPARAM3_0_ACCUMSWAPXFERSIZE_SIZE 5

/** bit field defines for km_pss_s#commandparam1_1 */
#define KM_PSS_COMMANDPARAM1_1_FBUFBLOCKSDURATION_OFFSET 0
#define KM_PSS_COMMANDPARAM1_1_FBUFBLOCKSDURATION_SIZE 8
#define KM_PSS_COMMANDPARAM1_1_FBUFPTRSTART_OFFSET 8
#define KM_PSS_COMMANDPARAM1_1_FBUFPTRSTART_SIZE 16

/** bit field defines for km_pss_s#commandparam2_1 */
#define KM_PSS_COMMANDPARAM2_1_ACCUMOFFSET_OFFSET 0
#define KM_PSS_COMMANDPARAM2_1_ACCUMOFFSET_SIZE 9
#define KM_PSS_COMMANDPARAM2_1_ACCUMRESET_OFFSET 9
#define KM_PSS_COMMANDPARAM2_1_ACCUMRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_1_DRX_OFFSET 10
#define KM_PSS_COMMANDPARAM2_1_DRX_SIZE 1
#define KM_PSS_COMMANDPARAM2_1_CONFIGCONTEXT_OFFSET 11
#define KM_PSS_COMMANDPARAM2_1_CONFIGCONTEXT_SIZE 2
#define KM_PSS_COMMANDPARAM2_1_SORTERENABLE_OFFSET 13
#define KM_PSS_COMMANDPARAM2_1_SORTERENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM2_1_SORTERRESET_OFFSET 14
#define KM_PSS_COMMANDPARAM2_1_SORTERRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_1_SORTERTRANSFER_OFFSET 15
#define KM_PSS_COMMANDPARAM2_1_SORTERTRANSFER_SIZE 1
#define KM_PSS_COMMANDPARAM2_1_ACCUMTRANSFER_OFFSET 16
#define KM_PSS_COMMANDPARAM2_1_ACCUMTRANSFER_SIZE 1

/** bit field defines for km_pss_s#commandparam3_1 */
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPEXTREADENABLE_OFFSET 0
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPEXTREADENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPEXTWRITEENABLE_OFFSET 1
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPEXTWRITEENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPINTOFFSET_OFFSET 2
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPINTOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPEXTREADOFFSET_OFFSET 7
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPEXTREADOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPEXTWRITEOFFSET_OFFSET 12
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPEXTWRITEOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPEXTBANKID_OFFSET 17
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPEXTBANKID_SIZE 1
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPXFERSIZE_OFFSET 18
#define KM_PSS_COMMANDPARAM3_1_ACCUMSWAPXFERSIZE_SIZE 5

/** bit field defines for km_pss_s#commandparam1_2 */
#define KM_PSS_COMMANDPARAM1_2_FBUFBLOCKSDURATION_OFFSET 0
#define KM_PSS_COMMANDPARAM1_2_FBUFBLOCKSDURATION_SIZE 8
#define KM_PSS_COMMANDPARAM1_2_FBUFPTRSTART_OFFSET 8
#define KM_PSS_COMMANDPARAM1_2_FBUFPTRSTART_SIZE 16

/** bit field defines for km_pss_s#commandparam2_2 */
#define KM_PSS_COMMANDPARAM2_2_ACCUMOFFSET_OFFSET 0
#define KM_PSS_COMMANDPARAM2_2_ACCUMOFFSET_SIZE 9
#define KM_PSS_COMMANDPARAM2_2_ACCUMRESET_OFFSET 9
#define KM_PSS_COMMANDPARAM2_2_ACCUMRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_2_DRX_OFFSET 10
#define KM_PSS_COMMANDPARAM2_2_DRX_SIZE 1
#define KM_PSS_COMMANDPARAM2_2_CONFIGCONTEXT_OFFSET 11
#define KM_PSS_COMMANDPARAM2_2_CONFIGCONTEXT_SIZE 2
#define KM_PSS_COMMANDPARAM2_2_SORTERENABLE_OFFSET 13
#define KM_PSS_COMMANDPARAM2_2_SORTERENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM2_2_SORTERRESET_OFFSET 14
#define KM_PSS_COMMANDPARAM2_2_SORTERRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_2_SORTERTRANSFER_OFFSET 15
#define KM_PSS_COMMANDPARAM2_2_SORTERTRANSFER_SIZE 1
#define KM_PSS_COMMANDPARAM2_2_ACCUMTRANSFER_OFFSET 16
#define KM_PSS_COMMANDPARAM2_2_ACCUMTRANSFER_SIZE 1

/** bit field defines for km_pss_s#commandparam3_2 */
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPEXTREADENABLE_OFFSET 0
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPEXTREADENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPEXTWRITEENABLE_OFFSET 1
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPEXTWRITEENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPINTOFFSET_OFFSET 2
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPINTOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPEXTREADOFFSET_OFFSET 7
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPEXTREADOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPEXTWRITEOFFSET_OFFSET 12
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPEXTWRITEOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPEXTBANKID_OFFSET 17
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPEXTBANKID_SIZE 1
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPXFERSIZE_OFFSET 18
#define KM_PSS_COMMANDPARAM3_2_ACCUMSWAPXFERSIZE_SIZE 5

/** bit field defines for km_pss_s#commandparam1_3 */
#define KM_PSS_COMMANDPARAM1_3_FBUFBLOCKSDURATION_OFFSET 0
#define KM_PSS_COMMANDPARAM1_3_FBUFBLOCKSDURATION_SIZE 8
#define KM_PSS_COMMANDPARAM1_3_FBUFPTRSTART_OFFSET 8
#define KM_PSS_COMMANDPARAM1_3_FBUFPTRSTART_SIZE 16

/** bit field defines for km_pss_s#commandparam2_3 */
#define KM_PSS_COMMANDPARAM2_3_ACCUMOFFSET_OFFSET 0
#define KM_PSS_COMMANDPARAM2_3_ACCUMOFFSET_SIZE 9
#define KM_PSS_COMMANDPARAM2_3_ACCUMRESET_OFFSET 9
#define KM_PSS_COMMANDPARAM2_3_ACCUMRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_3_DRX_OFFSET 10
#define KM_PSS_COMMANDPARAM2_3_DRX_SIZE 1
#define KM_PSS_COMMANDPARAM2_3_CONFIGCONTEXT_OFFSET 11
#define KM_PSS_COMMANDPARAM2_3_CONFIGCONTEXT_SIZE 2
#define KM_PSS_COMMANDPARAM2_3_SORTERENABLE_OFFSET 13
#define KM_PSS_COMMANDPARAM2_3_SORTERENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM2_3_SORTERRESET_OFFSET 14
#define KM_PSS_COMMANDPARAM2_3_SORTERRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_3_SORTERTRANSFER_OFFSET 15
#define KM_PSS_COMMANDPARAM2_3_SORTERTRANSFER_SIZE 1
#define KM_PSS_COMMANDPARAM2_3_ACCUMTRANSFER_OFFSET 16
#define KM_PSS_COMMANDPARAM2_3_ACCUMTRANSFER_SIZE 1

/** bit field defines for km_pss_s#commandparam3_3 */
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPEXTREADENABLE_OFFSET 0
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPEXTREADENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPEXTWRITEENABLE_OFFSET 1
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPEXTWRITEENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPINTOFFSET_OFFSET 2
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPINTOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPEXTREADOFFSET_OFFSET 7
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPEXTREADOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPEXTWRITEOFFSET_OFFSET 12
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPEXTWRITEOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPEXTBANKID_OFFSET 17
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPEXTBANKID_SIZE 1
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPXFERSIZE_OFFSET 18
#define KM_PSS_COMMANDPARAM3_3_ACCUMSWAPXFERSIZE_SIZE 5

/** bit field defines for km_pss_s#commandparam1_4 */
#define KM_PSS_COMMANDPARAM1_4_FBUFBLOCKSDURATION_OFFSET 0
#define KM_PSS_COMMANDPARAM1_4_FBUFBLOCKSDURATION_SIZE 8
#define KM_PSS_COMMANDPARAM1_4_FBUFPTRSTART_OFFSET 8
#define KM_PSS_COMMANDPARAM1_4_FBUFPTRSTART_SIZE 16

/** bit field defines for km_pss_s#commandparam2_4 */
#define KM_PSS_COMMANDPARAM2_4_ACCUMOFFSET_OFFSET 0
#define KM_PSS_COMMANDPARAM2_4_ACCUMOFFSET_SIZE 9
#define KM_PSS_COMMANDPARAM2_4_ACCUMRESET_OFFSET 9
#define KM_PSS_COMMANDPARAM2_4_ACCUMRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_4_DRX_OFFSET 10
#define KM_PSS_COMMANDPARAM2_4_DRX_SIZE 1
#define KM_PSS_COMMANDPARAM2_4_CONFIGCONTEXT_OFFSET 11
#define KM_PSS_COMMANDPARAM2_4_CONFIGCONTEXT_SIZE 2
#define KM_PSS_COMMANDPARAM2_4_SORTERENABLE_OFFSET 13
#define KM_PSS_COMMANDPARAM2_4_SORTERENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM2_4_SORTERRESET_OFFSET 14
#define KM_PSS_COMMANDPARAM2_4_SORTERRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_4_SORTERTRANSFER_OFFSET 15
#define KM_PSS_COMMANDPARAM2_4_SORTERTRANSFER_SIZE 1
#define KM_PSS_COMMANDPARAM2_4_ACCUMTRANSFER_OFFSET 16
#define KM_PSS_COMMANDPARAM2_4_ACCUMTRANSFER_SIZE 1

/** bit field defines for km_pss_s#commandparam3_4 */
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPEXTREADENABLE_OFFSET 0
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPEXTREADENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPEXTWRITEENABLE_OFFSET 1
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPEXTWRITEENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPINTOFFSET_OFFSET 2
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPINTOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPEXTREADOFFSET_OFFSET 7
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPEXTREADOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPEXTWRITEOFFSET_OFFSET 12
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPEXTWRITEOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPEXTBANKID_OFFSET 17
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPEXTBANKID_SIZE 1
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPXFERSIZE_OFFSET 18
#define KM_PSS_COMMANDPARAM3_4_ACCUMSWAPXFERSIZE_SIZE 5

/** bit field defines for km_pss_s#commandparam1_5 */
#define KM_PSS_COMMANDPARAM1_5_FBUFBLOCKSDURATION_OFFSET 0
#define KM_PSS_COMMANDPARAM1_5_FBUFBLOCKSDURATION_SIZE 8
#define KM_PSS_COMMANDPARAM1_5_FBUFPTRSTART_OFFSET 8
#define KM_PSS_COMMANDPARAM1_5_FBUFPTRSTART_SIZE 16

/** bit field defines for km_pss_s#commandparam2_5 */
#define KM_PSS_COMMANDPARAM2_5_ACCUMOFFSET_OFFSET 0
#define KM_PSS_COMMANDPARAM2_5_ACCUMOFFSET_SIZE 9
#define KM_PSS_COMMANDPARAM2_5_ACCUMRESET_OFFSET 9
#define KM_PSS_COMMANDPARAM2_5_ACCUMRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_5_DRX_OFFSET 10
#define KM_PSS_COMMANDPARAM2_5_DRX_SIZE 1
#define KM_PSS_COMMANDPARAM2_5_CONFIGCONTEXT_OFFSET 11
#define KM_PSS_COMMANDPARAM2_5_CONFIGCONTEXT_SIZE 2
#define KM_PSS_COMMANDPARAM2_5_SORTERENABLE_OFFSET 13
#define KM_PSS_COMMANDPARAM2_5_SORTERENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM2_5_SORTERRESET_OFFSET 14
#define KM_PSS_COMMANDPARAM2_5_SORTERRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_5_SORTERTRANSFER_OFFSET 15
#define KM_PSS_COMMANDPARAM2_5_SORTERTRANSFER_SIZE 1
#define KM_PSS_COMMANDPARAM2_5_ACCUMTRANSFER_OFFSET 16
#define KM_PSS_COMMANDPARAM2_5_ACCUMTRANSFER_SIZE 1

/** bit field defines for km_pss_s#commandparam3_5 */
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPEXTREADENABLE_OFFSET 0
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPEXTREADENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPEXTWRITEENABLE_OFFSET 1
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPEXTWRITEENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPINTOFFSET_OFFSET 2
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPINTOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPEXTREADOFFSET_OFFSET 7
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPEXTREADOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPEXTWRITEOFFSET_OFFSET 12
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPEXTWRITEOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPEXTBANKID_OFFSET 17
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPEXTBANKID_SIZE 1
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPXFERSIZE_OFFSET 18
#define KM_PSS_COMMANDPARAM3_5_ACCUMSWAPXFERSIZE_SIZE 5

/** bit field defines for km_pss_s#commandparam1_6 */
#define KM_PSS_COMMANDPARAM1_6_FBUFBLOCKSDURATION_OFFSET 0
#define KM_PSS_COMMANDPARAM1_6_FBUFBLOCKSDURATION_SIZE 8
#define KM_PSS_COMMANDPARAM1_6_FBUFPTRSTART_OFFSET 8
#define KM_PSS_COMMANDPARAM1_6_FBUFPTRSTART_SIZE 16

/** bit field defines for km_pss_s#commandparam2_6 */
#define KM_PSS_COMMANDPARAM2_6_ACCUMOFFSET_OFFSET 0
#define KM_PSS_COMMANDPARAM2_6_ACCUMOFFSET_SIZE 9
#define KM_PSS_COMMANDPARAM2_6_ACCUMRESET_OFFSET 9
#define KM_PSS_COMMANDPARAM2_6_ACCUMRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_6_DRX_OFFSET 10
#define KM_PSS_COMMANDPARAM2_6_DRX_SIZE 1
#define KM_PSS_COMMANDPARAM2_6_CONFIGCONTEXT_OFFSET 11
#define KM_PSS_COMMANDPARAM2_6_CONFIGCONTEXT_SIZE 2
#define KM_PSS_COMMANDPARAM2_6_SORTERENABLE_OFFSET 13
#define KM_PSS_COMMANDPARAM2_6_SORTERENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM2_6_SORTERRESET_OFFSET 14
#define KM_PSS_COMMANDPARAM2_6_SORTERRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_6_SORTERTRANSFER_OFFSET 15
#define KM_PSS_COMMANDPARAM2_6_SORTERTRANSFER_SIZE 1
#define KM_PSS_COMMANDPARAM2_6_ACCUMTRANSFER_OFFSET 16
#define KM_PSS_COMMANDPARAM2_6_ACCUMTRANSFER_SIZE 1

/** bit field defines for km_pss_s#commandparam3_6 */
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPEXTREADENABLE_OFFSET 0
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPEXTREADENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPEXTWRITEENABLE_OFFSET 1
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPEXTWRITEENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPINTOFFSET_OFFSET 2
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPINTOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPEXTREADOFFSET_OFFSET 7
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPEXTREADOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPEXTWRITEOFFSET_OFFSET 12
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPEXTWRITEOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPEXTBANKID_OFFSET 17
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPEXTBANKID_SIZE 1
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPXFERSIZE_OFFSET 18
#define KM_PSS_COMMANDPARAM3_6_ACCUMSWAPXFERSIZE_SIZE 5

/** bit field defines for km_pss_s#commandparam1_7 */
#define KM_PSS_COMMANDPARAM1_7_FBUFBLOCKSDURATION_OFFSET 0
#define KM_PSS_COMMANDPARAM1_7_FBUFBLOCKSDURATION_SIZE 8
#define KM_PSS_COMMANDPARAM1_7_FBUFPTRSTART_OFFSET 8
#define KM_PSS_COMMANDPARAM1_7_FBUFPTRSTART_SIZE 16

/** bit field defines for km_pss_s#commandparam2_7 */
#define KM_PSS_COMMANDPARAM2_7_ACCUMOFFSET_OFFSET 0
#define KM_PSS_COMMANDPARAM2_7_ACCUMOFFSET_SIZE 9
#define KM_PSS_COMMANDPARAM2_7_ACCUMRESET_OFFSET 9
#define KM_PSS_COMMANDPARAM2_7_ACCUMRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_7_DRX_OFFSET 10
#define KM_PSS_COMMANDPARAM2_7_DRX_SIZE 1
#define KM_PSS_COMMANDPARAM2_7_CONFIGCONTEXT_OFFSET 11
#define KM_PSS_COMMANDPARAM2_7_CONFIGCONTEXT_SIZE 2
#define KM_PSS_COMMANDPARAM2_7_SORTERENABLE_OFFSET 13
#define KM_PSS_COMMANDPARAM2_7_SORTERENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM2_7_SORTERRESET_OFFSET 14
#define KM_PSS_COMMANDPARAM2_7_SORTERRESET_SIZE 1
#define KM_PSS_COMMANDPARAM2_7_SORTERTRANSFER_OFFSET 15
#define KM_PSS_COMMANDPARAM2_7_SORTERTRANSFER_SIZE 1
#define KM_PSS_COMMANDPARAM2_7_ACCUMTRANSFER_OFFSET 16
#define KM_PSS_COMMANDPARAM2_7_ACCUMTRANSFER_SIZE 1

/** bit field defines for km_pss_s#commandparam3_7 */
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPEXTREADENABLE_OFFSET 0
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPEXTREADENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPEXTWRITEENABLE_OFFSET 1
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPEXTWRITEENABLE_SIZE 1
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPINTOFFSET_OFFSET 2
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPINTOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPEXTREADOFFSET_OFFSET 7
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPEXTREADOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPEXTWRITEOFFSET_OFFSET 12
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPEXTWRITEOFFSET_SIZE 5
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPEXTBANKID_OFFSET 17
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPEXTBANKID_SIZE 1
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPXFERSIZE_OFFSET 18
#define KM_PSS_COMMANDPARAM3_7_ACCUMSWAPXFERSIZE_SIZE 5

/** bit field defines for km_pss_s#commonparam */
#define KM_PSS_COMMONPARAM_CORRMODENBIOT_OFFSET 0
#define KM_PSS_COMMONPARAM_CORRMODENBIOT_SIZE 1
#define KM_PSS_COMMONPARAM_DAGCMAXLOG2_OFFSET 1
#define KM_PSS_COMMONPARAM_DAGCMAXLOG2_SIZE 4
#define KM_PSS_COMMONPARAM_ACCUMLO_OFFSET 5
#define KM_PSS_COMMONPARAM_ACCUMLO_SIZE 3

/** bit field defines for km_pss_s#corrconfigparam_0 */
#define KM_PSS_CORRCONFIGPARAM_0_FREQOFFSET_OFFSET 0
#define KM_PSS_CORRCONFIGPARAM_0_FREQOFFSET_SIZE 16
#define KM_PSS_CORRCONFIGPARAM_0_NBIOTSEGMENTSIZE_OFFSET 16
#define KM_PSS_CORRCONFIGPARAM_0_NBIOTSEGMENTSIZE_SIZE 1

/** bit field defines for km_pss_s#corrconfigparam_1 */
#define KM_PSS_CORRCONFIGPARAM_1_FREQOFFSET_OFFSET 0
#define KM_PSS_CORRCONFIGPARAM_1_FREQOFFSET_SIZE 16
#define KM_PSS_CORRCONFIGPARAM_1_NBIOTSEGMENTSIZE_OFFSET 16
#define KM_PSS_CORRCONFIGPARAM_1_NBIOTSEGMENTSIZE_SIZE 1

/** bit field defines for km_pss_s#corrconfigparam_2 */
#define KM_PSS_CORRCONFIGPARAM_2_FREQOFFSET_OFFSET 0
#define KM_PSS_CORRCONFIGPARAM_2_FREQOFFSET_SIZE 16
#define KM_PSS_CORRCONFIGPARAM_2_NBIOTSEGMENTSIZE_OFFSET 16
#define KM_PSS_CORRCONFIGPARAM_2_NBIOTSEGMENTSIZE_SIZE 1

/** bit field defines for km_pss_s#corrconfigparam_3 */
#define KM_PSS_CORRCONFIGPARAM_3_FREQOFFSET_OFFSET 0
#define KM_PSS_CORRCONFIGPARAM_3_FREQOFFSET_SIZE 16
#define KM_PSS_CORRCONFIGPARAM_3_NBIOTSEGMENTSIZE_OFFSET 16
#define KM_PSS_CORRCONFIGPARAM_3_NBIOTSEGMENTSIZE_SIZE 1

/** bit field defines for km_pss_s#fbufstate */
#define KM_PSS_FBUFSTATE_READPTR_OFFSET 0
#define KM_PSS_FBUFSTATE_READPTR_SIZE 16
#define KM_PSS_FBUFSTATE_BLOCKSREMAINING_OFFSET 16
#define KM_PSS_FBUFSTATE_BLOCKSREMAINING_SIZE 8

/** bit field defines for km_pss_s#fbufphasestate */
#define KM_PSS_FBUFPHASESTATE_START_OFFSET 0
#define KM_PSS_FBUFPHASESTATE_START_SIZE 16
#define KM_PSS_FBUFPHASESTATE_DELTA_OFFSET 16
#define KM_PSS_FBUFPHASESTATE_DELTA_SIZE 16

/** bit field defines for km_pss_s#accumcorrstate */
#define KM_PSS_ACCUMCORRSTATE_BUFFIDX_OFFSET 2
#define KM_PSS_ACCUMCORRSTATE_BUFFIDX_SIZE 12

/** bit field defines for km_pss_s#sorterxferstate */
#define KM_PSS_SORTERXFERSTATE_RESULTMASK_OFFSET 0
#define KM_PSS_SORTERXFERSTATE_RESULTMASK_SIZE 8
#define KM_PSS_SORTERXFERSTATE_CURRENTIDX_OFFSET 8
#define KM_PSS_SORTERXFERSTATE_CURRENTIDX_SIZE 4

/** bit field defines for km_pss_s#ramxferstate */
#define KM_PSS_RAMXFERSTATE_READADDRINT_OFFSET 0
#define KM_PSS_RAMXFERSTATE_READADDRINT_SIZE 13
#define KM_PSS_RAMXFERSTATE_WRITEADDRINT_OFFSET 16
#define KM_PSS_RAMXFERSTATE_WRITEADDRINT_SIZE 13
#define KM_PSS_RAMXFERSTATE_CURRENTFUNCTION_OFFSET 30
#define KM_PSS_RAMXFERSTATE_CURRENTFUNCTION_SIZE 2

/** bit field defines for km_pss_s#axixferstate */
#define KM_PSS_AXIXFERSTATE_WRITESDONE_OFFSET 0
#define KM_PSS_AXIXFERSTATE_WRITESDONE_SIZE 14
#define KM_PSS_AXIXFERSTATE_READSDONE_OFFSET 16
#define KM_PSS_AXIXFERSTATE_READSDONE_SIZE 14
#define KM_PSS_AXIXFERSTATE_CURRENTFUNCTION_OFFSET 30
#define KM_PSS_AXIXFERSTATE_CURRENTFUNCTION_SIZE 2

/** bit field defines for km_pss_s#accumnbiot_0 */
#define KM_PSS_ACCUMNBIOT_0_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_0_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_0_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_0_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_0_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_0_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_0_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_0_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumnbiot_1 */
#define KM_PSS_ACCUMNBIOT_1_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_1_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_1_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_1_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_1_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_1_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_1_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_1_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumnbiot_2 */
#define KM_PSS_ACCUMNBIOT_2_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_2_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_2_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_2_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_2_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_2_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_2_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_2_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumnbiot_3 */
#define KM_PSS_ACCUMNBIOT_3_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_3_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_3_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_3_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_3_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_3_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_3_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_3_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumnbiot_4 */
#define KM_PSS_ACCUMNBIOT_4_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_4_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_4_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_4_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_4_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_4_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_4_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_4_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumnbiot_5 */
#define KM_PSS_ACCUMNBIOT_5_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_5_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_5_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_5_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_5_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_5_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_5_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_5_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumnbiot_6 */
#define KM_PSS_ACCUMNBIOT_6_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_6_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_6_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_6_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_6_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_6_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_6_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_6_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumnbiot_7 */
#define KM_PSS_ACCUMNBIOT_7_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_7_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_7_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_7_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_7_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_7_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_7_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_7_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumnbiot_8 */
#define KM_PSS_ACCUMNBIOT_8_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_8_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_8_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_8_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_8_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_8_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_8_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_8_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumnbiot_9 */
#define KM_PSS_ACCUMNBIOT_9_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_9_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_9_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_9_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_9_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_9_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_9_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_9_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumnbiot_10 */
#define KM_PSS_ACCUMNBIOT_10_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_10_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_10_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_10_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_10_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_10_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_10_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_10_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumnbiot_11 */
#define KM_PSS_ACCUMNBIOT_11_BANDEDGELEFT_OFFSET 0
#define KM_PSS_ACCUMNBIOT_11_BANDEDGELEFT_SIZE 7
#define KM_PSS_ACCUMNBIOT_11_BANDWIDTH_OFFSET 7
#define KM_PSS_ACCUMNBIOT_11_BANDWIDTH_SIZE 7
#define KM_PSS_ACCUMNBIOT_11_CONFIGCONTEXT_OFFSET 14
#define KM_PSS_ACCUMNBIOT_11_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMNBIOT_11_SORTERID_OFFSET 16
#define KM_PSS_ACCUMNBIOT_11_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumlte_0 */
#define KM_PSS_ACCUMLTE_0_NID2_OFFSET 0
#define KM_PSS_ACCUMLTE_0_NID2_SIZE 2
#define KM_PSS_ACCUMLTE_0_BANDWIDTH_OFFSET 2
#define KM_PSS_ACCUMLTE_0_BANDWIDTH_SIZE 2
#define KM_PSS_ACCUMLTE_0_CONFIGCONTEXT_OFFSET 4
#define KM_PSS_ACCUMLTE_0_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMLTE_0_SORTERID_OFFSET 6
#define KM_PSS_ACCUMLTE_0_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumlte_1 */
#define KM_PSS_ACCUMLTE_1_NID2_OFFSET 0
#define KM_PSS_ACCUMLTE_1_NID2_SIZE 2
#define KM_PSS_ACCUMLTE_1_BANDWIDTH_OFFSET 2
#define KM_PSS_ACCUMLTE_1_BANDWIDTH_SIZE 2
#define KM_PSS_ACCUMLTE_1_CONFIGCONTEXT_OFFSET 4
#define KM_PSS_ACCUMLTE_1_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMLTE_1_SORTERID_OFFSET 6
#define KM_PSS_ACCUMLTE_1_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumlte_2 */
#define KM_PSS_ACCUMLTE_2_NID2_OFFSET 0
#define KM_PSS_ACCUMLTE_2_NID2_SIZE 2
#define KM_PSS_ACCUMLTE_2_BANDWIDTH_OFFSET 2
#define KM_PSS_ACCUMLTE_2_BANDWIDTH_SIZE 2
#define KM_PSS_ACCUMLTE_2_CONFIGCONTEXT_OFFSET 4
#define KM_PSS_ACCUMLTE_2_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMLTE_2_SORTERID_OFFSET 6
#define KM_PSS_ACCUMLTE_2_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumlte_3 */
#define KM_PSS_ACCUMLTE_3_NID2_OFFSET 0
#define KM_PSS_ACCUMLTE_3_NID2_SIZE 2
#define KM_PSS_ACCUMLTE_3_BANDWIDTH_OFFSET 2
#define KM_PSS_ACCUMLTE_3_BANDWIDTH_SIZE 2
#define KM_PSS_ACCUMLTE_3_CONFIGCONTEXT_OFFSET 4
#define KM_PSS_ACCUMLTE_3_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMLTE_3_SORTERID_OFFSET 6
#define KM_PSS_ACCUMLTE_3_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumlte_4 */
#define KM_PSS_ACCUMLTE_4_NID2_OFFSET 0
#define KM_PSS_ACCUMLTE_4_NID2_SIZE 2
#define KM_PSS_ACCUMLTE_4_BANDWIDTH_OFFSET 2
#define KM_PSS_ACCUMLTE_4_BANDWIDTH_SIZE 2
#define KM_PSS_ACCUMLTE_4_CONFIGCONTEXT_OFFSET 4
#define KM_PSS_ACCUMLTE_4_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMLTE_4_SORTERID_OFFSET 6
#define KM_PSS_ACCUMLTE_4_SORTERID_SIZE 3

/** bit field defines for km_pss_s#accumlte_5 */
#define KM_PSS_ACCUMLTE_5_NID2_OFFSET 0
#define KM_PSS_ACCUMLTE_5_NID2_SIZE 2
#define KM_PSS_ACCUMLTE_5_BANDWIDTH_OFFSET 2
#define KM_PSS_ACCUMLTE_5_BANDWIDTH_SIZE 2
#define KM_PSS_ACCUMLTE_5_CONFIGCONTEXT_OFFSET 4
#define KM_PSS_ACCUMLTE_5_CONFIGCONTEXT_SIZE 2
#define KM_PSS_ACCUMLTE_5_SORTERID_OFFSET 6
#define KM_PSS_ACCUMLTE_5_SORTERID_SIZE 3

/** bit field defines for km_pss_s#sorterconfig_0 */
#define KM_PSS_SORTERCONFIG_0_NUMBLOCKS_OFFSET 0
#define KM_PSS_SORTERCONFIG_0_NUMBLOCKS_SIZE 4
#define KM_PSS_SORTERCONFIG_0_BLOCKOFFSET_OFFSET 4
#define KM_PSS_SORTERCONFIG_0_BLOCKOFFSET_SIZE 5
#define KM_PSS_SORTERCONFIG_0_BLOCKOFFSETTRANSFER_OFFSET 9
#define KM_PSS_SORTERCONFIG_0_BLOCKOFFSETTRANSFER_SIZE 5

/** bit field defines for km_pss_s#sorterconfig_1 */
#define KM_PSS_SORTERCONFIG_1_NUMBLOCKS_OFFSET 0
#define KM_PSS_SORTERCONFIG_1_NUMBLOCKS_SIZE 4
#define KM_PSS_SORTERCONFIG_1_BLOCKOFFSET_OFFSET 4
#define KM_PSS_SORTERCONFIG_1_BLOCKOFFSET_SIZE 5
#define KM_PSS_SORTERCONFIG_1_BLOCKOFFSETTRANSFER_OFFSET 9
#define KM_PSS_SORTERCONFIG_1_BLOCKOFFSETTRANSFER_SIZE 5

/** bit field defines for km_pss_s#sorterconfig_2 */
#define KM_PSS_SORTERCONFIG_2_NUMBLOCKS_OFFSET 0
#define KM_PSS_SORTERCONFIG_2_NUMBLOCKS_SIZE 4
#define KM_PSS_SORTERCONFIG_2_BLOCKOFFSET_OFFSET 4
#define KM_PSS_SORTERCONFIG_2_BLOCKOFFSET_SIZE 5
#define KM_PSS_SORTERCONFIG_2_BLOCKOFFSETTRANSFER_OFFSET 9
#define KM_PSS_SORTERCONFIG_2_BLOCKOFFSETTRANSFER_SIZE 5

/** bit field defines for km_pss_s#sorterconfig_3 */
#define KM_PSS_SORTERCONFIG_3_NUMBLOCKS_OFFSET 0
#define KM_PSS_SORTERCONFIG_3_NUMBLOCKS_SIZE 4
#define KM_PSS_SORTERCONFIG_3_BLOCKOFFSET_OFFSET 4
#define KM_PSS_SORTERCONFIG_3_BLOCKOFFSET_SIZE 5
#define KM_PSS_SORTERCONFIG_3_BLOCKOFFSETTRANSFER_OFFSET 9
#define KM_PSS_SORTERCONFIG_3_BLOCKOFFSETTRANSFER_SIZE 5

/** bit field defines for km_pss_s#sorterconfig_4 */
#define KM_PSS_SORTERCONFIG_4_NUMBLOCKS_OFFSET 0
#define KM_PSS_SORTERCONFIG_4_NUMBLOCKS_SIZE 4
#define KM_PSS_SORTERCONFIG_4_BLOCKOFFSET_OFFSET 4
#define KM_PSS_SORTERCONFIG_4_BLOCKOFFSET_SIZE 5
#define KM_PSS_SORTERCONFIG_4_BLOCKOFFSETTRANSFER_OFFSET 9
#define KM_PSS_SORTERCONFIG_4_BLOCKOFFSETTRANSFER_SIZE 5

/** bit field defines for km_pss_s#sorterconfig_5 */
#define KM_PSS_SORTERCONFIG_5_NUMBLOCKS_OFFSET 0
#define KM_PSS_SORTERCONFIG_5_NUMBLOCKS_SIZE 4
#define KM_PSS_SORTERCONFIG_5_BLOCKOFFSET_OFFSET 4
#define KM_PSS_SORTERCONFIG_5_BLOCKOFFSET_SIZE 5
#define KM_PSS_SORTERCONFIG_5_BLOCKOFFSETTRANSFER_OFFSET 9
#define KM_PSS_SORTERCONFIG_5_BLOCKOFFSETTRANSFER_SIZE 5

/** bit field defines for km_pss_s#sorterconfig_6 */
#define KM_PSS_SORTERCONFIG_6_NUMBLOCKS_OFFSET 0
#define KM_PSS_SORTERCONFIG_6_NUMBLOCKS_SIZE 4
#define KM_PSS_SORTERCONFIG_6_BLOCKOFFSET_OFFSET 4
#define KM_PSS_SORTERCONFIG_6_BLOCKOFFSET_SIZE 5
#define KM_PSS_SORTERCONFIG_6_BLOCKOFFSETTRANSFER_OFFSET 9
#define KM_PSS_SORTERCONFIG_6_BLOCKOFFSETTRANSFER_SIZE 5

/** bit field defines for km_pss_s#sorterconfig_7 */
#define KM_PSS_SORTERCONFIG_7_NUMBLOCKS_OFFSET 0
#define KM_PSS_SORTERCONFIG_7_NUMBLOCKS_SIZE 4
#define KM_PSS_SORTERCONFIG_7_BLOCKOFFSET_OFFSET 4
#define KM_PSS_SORTERCONFIG_7_BLOCKOFFSET_SIZE 5
#define KM_PSS_SORTERCONFIG_7_BLOCKOFFSETTRANSFER_OFFSET 9
#define KM_PSS_SORTERCONFIG_7_BLOCKOFFSETTRANSFER_SIZE 5

/** bit field defines for km_pss_s#sorterstatus_0 */
#define KM_PSS_SORTERSTATUS_0_NUMSAMPLESDIV4_OFFSET 0
#define KM_PSS_SORTERSTATUS_0_NUMSAMPLESDIV4_SIZE 8

/** bit field defines for km_pss_s#sorterstatus_1 */
#define KM_PSS_SORTERSTATUS_1_NUMSAMPLESDIV4_OFFSET 0
#define KM_PSS_SORTERSTATUS_1_NUMSAMPLESDIV4_SIZE 8

/** bit field defines for km_pss_s#sorterstatus_2 */
#define KM_PSS_SORTERSTATUS_2_NUMSAMPLESDIV4_OFFSET 0
#define KM_PSS_SORTERSTATUS_2_NUMSAMPLESDIV4_SIZE 8

/** bit field defines for km_pss_s#sorterstatus_3 */
#define KM_PSS_SORTERSTATUS_3_NUMSAMPLESDIV4_OFFSET 0
#define KM_PSS_SORTERSTATUS_3_NUMSAMPLESDIV4_SIZE 8

/** bit field defines for km_pss_s#sorterstatus_4 */
#define KM_PSS_SORTERSTATUS_4_NUMSAMPLESDIV4_OFFSET 0
#define KM_PSS_SORTERSTATUS_4_NUMSAMPLESDIV4_SIZE 8

/** bit field defines for km_pss_s#sorterstatus_5 */
#define KM_PSS_SORTERSTATUS_5_NUMSAMPLESDIV4_OFFSET 0
#define KM_PSS_SORTERSTATUS_5_NUMSAMPLESDIV4_SIZE 8

/** bit field defines for km_pss_s#sorterstatus_6 */
#define KM_PSS_SORTERSTATUS_6_NUMSAMPLESDIV4_OFFSET 0
#define KM_PSS_SORTERSTATUS_6_NUMSAMPLESDIV4_SIZE 8

/** bit field defines for km_pss_s#sorterstatus_7 */
#define KM_PSS_SORTERSTATUS_7_NUMSAMPLESDIV4_OFFSET 0
#define KM_PSS_SORTERSTATUS_7_NUMSAMPLESDIV4_SIZE 8

/** bit field defines for km_pss_s#sortertransfer */
#define KM_PSS_SORTERTRANSFER_BASEADDR_OFFSET 6
#define KM_PSS_SORTERTRANSFER_BASEADDR_SIZE 26

/** bit field defines for km_pss_s#accumtransfer */
#define KM_PSS_ACCUMTRANSFER_BASEADDR_OFFSET 6
#define KM_PSS_ACCUMTRANSFER_BASEADDR_SIZE 26

/** bit field defines for km_pss_s#accumswap */
#define KM_PSS_ACCUMSWAP_BASEADDR_OFFSET 6
#define KM_PSS_ACCUMSWAP_BASEADDR_SIZE 26

/** bit field defines for km_pss_s#accumswapsize */
#define KM_PSS_ACCUMSWAPSIZE_NONE_OFFSET 0
#define KM_PSS_ACCUMSWAPSIZE_NONE_SIZE 5

/** bit field defines for km_pss_s#accumtransferctrl */
#define KM_PSS_ACCUMTRANSFERCTRL_ENABLE_OFFSET 0
#define KM_PSS_ACCUMTRANSFERCTRL_ENABLE_SIZE 12

/** bit field defines for km_pss_s#eventstatus */
#define KM_PSS_EVENTSTATUS_COMMANDCOMPLETE_OFFSET 0
#define KM_PSS_EVENTSTATUS_COMMANDCOMPLETE_SIZE 8
#define KM_PSS_EVENTSTATUS_ACCUMTRANSFEROVR_OFFSET 8
#define KM_PSS_EVENTSTATUS_ACCUMTRANSFEROVR_SIZE 1

/** bit field defines for km_pss_s#interruptmask */
#define KM_PSS_INTERRUPTMASK_COMMANDCOMPLETE_OFFSET 0
#define KM_PSS_INTERRUPTMASK_COMMANDCOMPLETE_SIZE 8
#define KM_PSS_INTERRUPTMASK_ACCUMTRANSFEROVR_OFFSET 8
#define KM_PSS_INTERRUPTMASK_ACCUMTRANSFEROVR_SIZE 1

/** bit field defines for km_pss_s#interruptstatus */
#define KM_PSS_INTERRUPTSTATUS_COMMANDCOMPLETE_OFFSET 0
#define KM_PSS_INTERRUPTSTATUS_COMMANDCOMPLETE_SIZE 8
#define KM_PSS_INTERRUPTSTATUS_ACCUMTRANSFEROVR_OFFSET 8
#define KM_PSS_INTERRUPTSTATUS_ACCUMTRANSFEROVR_SIZE 1

/** bit field defines for km_pss_s#exteventpulse */
#define KM_PSS_EXTEVENTPULSE_COMMANDCOMPLETE_OFFSET 0
#define KM_PSS_EXTEVENTPULSE_COMMANDCOMPLETE_SIZE 8

/** bit field defines for km_pss_s#hwobs */
#define KM_PSS_HWOBS_SEL_OFFSET 0
#define KM_PSS_HWOBS_SEL_SIZE 3

/* EOF km_pss.h */
#endif
