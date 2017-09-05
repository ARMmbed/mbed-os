#ifndef INCLUDED_DISPLAY_TOP
#define INCLUDED_DISPLAY_TOP
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** display_top HAL Spreadsheet version number */
#define DISPLAY_TOP_HAL_VERSION 1

/** This is the Display IF register file.  This configurable engine can convey data to an external display using either a serial interface or i8080 parallel interface.
*/
struct display_top_s {
   /** Write to the Data FIFO setting Cs0 and A0 = 0 at address offset 0x000, read-write */
   uint32_t fifowrcs0a0;
   /** Write to the Data FIFO setting Cs0 and A0 = 1 at address offset 0x004, read-write */
   uint32_t fifowrcs0a1;
   /** Write to the Data FIFO setting Cs1 and A0 = 0 at address offset 0x008, read-write */
   uint32_t fifowrcs1a0;
   /** Write to the Data FIFO setting Cs1 and A0 = 1 at address offset 0x00C, read-write */
   uint32_t fifowrcs1a1;
   /**  at address offset 0x010, read-write */
   uint32_t displaycfg;
   /**  at address offset 0x014, read-write */
   uint32_t readcfg;
   /** Set when the read data is valid at address offset 0x018, read-only */
   uint32_t readdatavalid;
   /** Read data register (parallel or serial) at address offset 0x01C, read-only */
   uint32_t readdatareg;
   /**  at address offset 0x020, read-write */
   uint32_t serialenginecfg;
   /** When this bit is set we start a DMA write transaction at address offset 0x024, read-write */
   uint32_t startdmawr;
   /**  at address offset 0x028, read-write */
   uint32_t dmacfg;
   /** Total number of packets to transfer at address offset 0x02C, read-write */
   uint32_t numberofpackets;
   /**  at address offset 0x030, read-write */
   uint32_t timingcontrolreg1;
   /**  at address offset 0x034, read-write */
   uint32_t timingcontrolreg2;
   /**  at address offset 0x038, read-write */
   uint32_t polarityreg;
   /**  at address offset 0x03C, read-only */
   uint32_t displaystatusreg;
};

/** bit field defines for display_top_s#fifowrcs0a0 */
#define DISPLAY_TOP_FIFOWRCS0A0_NONE_OFFSET 0
#define DISPLAY_TOP_FIFOWRCS0A0_NONE_SIZE 32

/** bit field defines for display_top_s#fifowrcs0a1 */
#define DISPLAY_TOP_FIFOWRCS0A1_NONE_OFFSET 0
#define DISPLAY_TOP_FIFOWRCS0A1_NONE_SIZE 32

/** bit field defines for display_top_s#fifowrcs1a0 */
#define DISPLAY_TOP_FIFOWRCS1A0_NONE_OFFSET 0
#define DISPLAY_TOP_FIFOWRCS1A0_NONE_SIZE 32

/** bit field defines for display_top_s#fifowrcs1a1 */
#define DISPLAY_TOP_FIFOWRCS1A1_NONE_OFFSET 0
#define DISPLAY_TOP_FIFOWRCS1A1_NONE_SIZE 32

/** bit field defines for display_top_s#displaycfg */
#define DISPLAY_TOP_DISPLAYCFG_ENGINESELECT_OFFSET 0
#define DISPLAY_TOP_DISPLAYCFG_ENGINESELECT_SIZE 1
/** Selected engine: parallel */
#define DISPLAY_TOP_DISPLAYCFG_ENGINESELECT_PARALLEL_VALUE 1
/** Selected engine: serial */
#define DISPLAY_TOP_DISPLAYCFG_ENGINESELECT_SERIAL_VALUE 0
#define DISPLAY_TOP_DISPLAYCFG_LSBALIGNMENT_OFFSET 1
#define DISPLAY_TOP_DISPLAYCFG_LSBALIGNMENT_SIZE 1
/** Data shifted out LSB first */
#define DISPLAY_TOP_DISPLAYCFG_LSBALIGNMENT_LSB_VALUE 1
/** Data shifted out MSB first */
#define DISPLAY_TOP_DISPLAYCFG_LSBALIGNMENT_MSB_VALUE 0
#define DISPLAY_TOP_DISPLAYCFG_PARALLELMODE_OFFSET 2
#define DISPLAY_TOP_DISPLAYCFG_PARALLELMODE_SIZE 1
/** Driven mode of operation */
#define DISPLAY_TOP_DISPLAYCFG_PARALLELMODE_DRIVEN_VALUE 0
/** Tri-state mode of operation */
#define DISPLAY_TOP_DISPLAYCFG_PARALLELMODE_TRISTATE_VALUE 1
#define DISPLAY_TOP_DISPLAYCFG_USEPCS_OFFSET 4
#define DISPLAY_TOP_DISPLAYCFG_USEPCS_SIZE 1

/** bit field defines for display_top_s#readcfg */
#define DISPLAY_TOP_READCFG_READTRANSACTIONSTART_OFFSET 0
#define DISPLAY_TOP_READCFG_READTRANSACTIONSTART_SIZE 1
#define DISPLAY_TOP_READCFG_READCSA0_OFFSET 1
#define DISPLAY_TOP_READCFG_READCSA0_SIZE 2

/** bit field defines for display_top_s#readdatavalid */
#define DISPLAY_TOP_READDATAVALID_NONE_OFFSET 0
#define DISPLAY_TOP_READDATAVALID_NONE_SIZE 1

/** bit field defines for display_top_s#readdatareg */
#define DISPLAY_TOP_READDATAREG_NONE_OFFSET 0
#define DISPLAY_TOP_READDATAREG_NONE_SIZE 32

/** bit field defines for display_top_s#serialenginecfg */
#define DISPLAY_TOP_SERIALENGINECFG_NUMBEROFCLOCKS_OFFSET 0
#define DISPLAY_TOP_SERIALENGINECFG_NUMBEROFCLOCKS_SIZE 9
#define DISPLAY_TOP_SERIALENGINECFG_NUMBEROFWRITECYCLES_OFFSET 9
#define DISPLAY_TOP_SERIALENGINECFG_NUMBEROFWRITECYCLES_SIZE 9
#define DISPLAY_TOP_SERIALENGINECFG_BIDIRENABLE_OFFSET 20
#define DISPLAY_TOP_SERIALENGINECFG_BIDIRENABLE_SIZE 1
#define DISPLAY_TOP_SERIALENGINECFG_USESTOUT_OFFSET 24
#define DISPLAY_TOP_SERIALENGINECFG_USESTOUT_SIZE 1

/** bit field defines for display_top_s#startdmawr */
#define DISPLAY_TOP_STARTDMAWR_NONE_OFFSET 0
#define DISPLAY_TOP_STARTDMAWR_NONE_SIZE 1

/** bit field defines for display_top_s#dmacfg */
#define DISPLAY_TOP_DMACFG_BURSTSIZE_OFFSET 0
#define DISPLAY_TOP_DMACFG_BURSTSIZE_SIZE 2
#define DISPLAY_TOP_DMACFG_NUMBEROFBITSPERPACKET_OFFSET 2
#define DISPLAY_TOP_DMACFG_NUMBEROFBITSPERPACKET_SIZE 9

/** bit field defines for display_top_s#numberofpackets */
#define DISPLAY_TOP_NUMBEROFPACKETS_NONE_OFFSET 0
#define DISPLAY_TOP_NUMBEROFPACKETS_NONE_SIZE 18

/** bit field defines for display_top_s#timingcontrolreg1 */
#define DISPLAY_TOP_TIMINGCONTROLREG1_TTRIG_OFFSET 0
#define DISPLAY_TOP_TIMINGCONTROLREG1_TTRIG_SIZE 4
#define DISPLAY_TOP_TIMINGCONTROLREG1_TSETUP_OFFSET 4
#define DISPLAY_TOP_TIMINGCONTROLREG1_TSETUP_SIZE 5
#define DISPLAY_TOP_TIMINGCONTROLREG1_TACTIVE_OFFSET 9
#define DISPLAY_TOP_TIMINGCONTROLREG1_TACTIVE_SIZE 4
#define DISPLAY_TOP_TIMINGCONTROLREG1_THOLD_OFFSET 13
#define DISPLAY_TOP_TIMINGCONTROLREG1_THOLD_SIZE 5
#define DISPLAY_TOP_TIMINGCONTROLREG1_TGAP_OFFSET 18
#define DISPLAY_TOP_TIMINGCONTROLREG1_TGAP_SIZE 5

/** bit field defines for display_top_s#timingcontrolreg2 */
#define DISPLAY_TOP_TIMINGCONTROLREG2_TCLK1_2_OFFSET 0
#define DISPLAY_TOP_TIMINGCONTROLREG2_TCLK1_2_SIZE 5
#define DISPLAY_TOP_TIMINGCONTROLREG2_TOUTDELAY_OFFSET 5
#define DISPLAY_TOP_TIMINGCONTROLREG2_TOUTDELAY_SIZE 6
#define DISPLAY_TOP_TIMINGCONTROLREG2_TOUTACTIVE_OFFSET 11
#define DISPLAY_TOP_TIMINGCONTROLREG2_TOUTACTIVE_SIZE 6

/** bit field defines for display_top_s#polarityreg */
#define DISPLAY_TOP_POLARITYREG_CHIPSELECTPOLARITY_OFFSET 0
#define DISPLAY_TOP_POLARITYREG_CHIPSELECTPOLARITY_SIZE 1
#define DISPLAY_TOP_POLARITYREG_CLKPOLARITY_OFFSET 1
#define DISPLAY_TOP_POLARITYREG_CLKPOLARITY_SIZE 1
#define DISPLAY_TOP_POLARITYREG_WRITESTROBEPOLARITY_OFFSET 2
#define DISPLAY_TOP_POLARITYREG_WRITESTROBEPOLARITY_SIZE 1
#define DISPLAY_TOP_POLARITYREG_READSTROBEPOLARITY_OFFSET 3
#define DISPLAY_TOP_POLARITYREG_READSTROBEPOLARITY_SIZE 1
#define DISPLAY_TOP_POLARITYREG_TIMINGINPUTSIGNALPOLARITY_OFFSET 4
#define DISPLAY_TOP_POLARITYREG_TIMINGINPUTSIGNALPOLARITY_SIZE 2
#define DISPLAY_TOP_POLARITYREG_TIMINGOUTPUTSIGNALPOLARITY_OFFSET 6
#define DISPLAY_TOP_POLARITYREG_TIMINGOUTPUTSIGNALPOLARITY_SIZE 1

/** bit field defines for display_top_s#displaystatusreg */
#define DISPLAY_TOP_DISPLAYSTATUSREG_IDLESTATE_OFFSET 0
#define DISPLAY_TOP_DISPLAYSTATUSREG_IDLESTATE_SIZE 1
#define DISPLAY_TOP_DISPLAYSTATUSREG_FIFOEMPTY_OFFSET 4
#define DISPLAY_TOP_DISPLAYSTATUSREG_FIFOEMPTY_SIZE 1
#define DISPLAY_TOP_DISPLAYSTATUSREG_WAITINGFORDATA_OFFSET 8
#define DISPLAY_TOP_DISPLAYSTATUSREG_WAITINGFORDATA_SIZE 1

/* EOF display_top.h */
#endif
