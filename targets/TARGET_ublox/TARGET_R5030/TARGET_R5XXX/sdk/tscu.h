#ifndef INCLUDED_TSCU
#define INCLUDED_TSCU
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define TSCU_PERIPHERALID4 0x07
#define TSCU_PERIPHERALID5 0x00
#define TSCU_PERIPHERALID6 0x00
#define TSCU_PERIPHERALID7 0x00
#define TSCU_PERIPHERALID0 0x03
#define TSCU_PERIPHERALID1 0xDA
#define TSCU_PERIPHERALID2 0x08
#define TSCU_PERIPHERALID3 0x00
#define TSCU_COMPONENTID0 0x0D
#define TSCU_COMPONENTID1 0xF0
#define TSCU_COMPONENTID2 0x05
#define TSCU_COMPONENTID3 0xB1

/** Time Control Unit 0 and 1 Control Interface
*/
struct tscu_s {
   /** Control Register at address offset 0x000, write-only */
   uint32_t tcu0_ctrl;
   /** Reserved space */
   uint8_t fill0[4];
   /** Number of Slow Clock cycles to sleep at address offset 0x008, read-write */
   uint32_t tcu0_ssleepset;
   /** Number of Fast Clock cycles to sleep at address offset 0x00C, read-write */
   uint32_t tcu0_fsleepset;
   /** Number of Slow Clock cycles required to power-up the Fast Clock at address offset 0x010, read-write */
   uint32_t tcu0_spwrupdelayset;
   /** Instruction to be executed after assert at address offset 0x014, read-write */
   uint32_t tcu0_definstr;
   /** Trigger an early wake-up assert at address offset 0x018, write-only */
   uint32_t tcu0_assert;
   /** software abort at address offset 0x01C, write-only */
   uint32_t tcu0_abort;
   /** Reserved space */
   uint8_t fill1[224];
   /** Status register at address offset 0x100, read-only */
   uint32_t tcu0_status;
   /** Number of Slow Clock cycles slept at address offset 0x104, read-only */
   uint32_t tcu0_ssleepstatus;
   /** Number of Fast Clock cycles slept at address offset 0x108, read-only */
   uint32_t tcu0_fsleepstatus;
   /** Number of Fast Clock cycles during slow clock calibration period at address offset 0x10C, read-only */
   uint32_t tcu0_calcnt;
   /** Reserved space */
   uint8_t fill2[16];
   /** Sequence FIFO near empty watermark.  As the FIFO is filled in 16-bit increments, the watermarks should be 2-byte aligned values: the lower 1 bit should be zero. Non-zero bit 0 will be ignored by the hardware. at address offset 0x120, read-write */
   uint32_t tcu0_sfnemptywm;
   /** Sequence FIFO near full watermark.  As the FIFO is filled in 16-bit increments, the watermarks should be 2-byte aligned values: the lower 1 bit should be zero. Non-zero bit 0 will be ignored by the hardware. at address offset 0x124, read-write */
   uint32_t tcu0_sfnfullwm;
   /** Reserved space */
   uint8_t fill3[472];
   /** Clear Fault Status.  Write one to a specific bit to clear the corresponding fault. at address offset 0x300, write-only */
   uint32_t tcu0_fltclear;
   /** Fault Status Register at address offset 0x304, read-only */
   uint32_t tcu0_fltstatus;
   /** Raw Fault Status Register at address offset 0x308, read-only */
   uint32_t tcu0_fltrawstatus;
   /** Fault Mask Clear.  Write one to a specific bit to clear the corresponding mask bit. at address offset 0x30C, write-only */
   uint32_t tcu0_fltmaskclr;
   /** Fault Mask Set.  Write one to a specific bit to set the corresponding mask bit. at address offset 0x310, write-only */
   uint32_t tcu0_fltmaskset;
   /** Fault Mask Status at address offset 0x314, read-only */
   uint32_t tcu0_fltmaskstatus;
   /** Reserved space */
   uint8_t fill4[232];
   /** Override Event Outputs Set at address offset 0x400, write-only */
   uint32_t tcu0_setevent;
   /** Override Event Outputs Clear at address offset 0x404, write-only */
   uint32_t tcu0_clrevent;
   /** Override Event Outputs Pulse at address offset 0x408, write-only */
   uint32_t tcu0_pulseevent;
   /** Event output status at address offset 0x40C, read-only */
   uint32_t tcu0_eventstat;
   /** Mask to apply when TSCU is halted at address offset 0x410, read-write */
   uint32_t tcu0_eventhalt;
   /** Reserved space */
   uint8_t fill5[236];
   /** Configured FIFO size in bytes at address offset 0x500, read-only */
   uint32_t tcu0_config;
   /** Reserved space */
   uint8_t fill6[252];
   /** Control Register at address offset 0x600, write-only */
   uint32_t tcu1_ctrl;
   /** Reserved space */
   uint8_t fill7[24];
   /** software abort at address offset 0x61C, write-only */
   uint32_t tcu1_abort;
   /** Reserved space */
   uint8_t fill8[224];
   /** Status register at address offset 0x700, read-only */
   uint32_t tcu1_status;
   /** Reserved space */
   uint8_t fill9[28];
   /** Sequence FIFO near empty watermark.  As the FIFO is filled in 16-bit increments, the watermarks should be 2-byte aligned values: the lower 1 bit should be zero. Non-zero bit 0 will be ignored by the hardware. at address offset 0x720, read-write */
   uint32_t tcu1_sfnemptywm;
   /** Sequence FIFO near full watermark.  As the FIFO is filled in 16-bit increments, the watermarks should be 2-byte aligned values: the lower 1 bit should be zero. Non-zero bit 0 will be ignored by the hardware. at address offset 0x724, read-write */
   uint32_t tcu1_sfnfullwm;
   /** Reserved space */
   uint8_t fill10[472];
   /** Clear Fault Status.  Write one to a specific bit to clear the corresponding fault. at address offset 0x900, write-only */
   uint32_t tcu1_fltclear;
   /** Fault Status Register at address offset 0x904, read-only */
   uint32_t tcu1_fltstatus;
   /** Raw Fault Status Register at address offset 0x908, read-only */
   uint32_t tcu1_fltrawstatus;
   /** Fault Mask Clear.  Write one to a specific bit to clear the corresponding mask bit. at address offset 0x90C, write-only */
   uint32_t tcu1_fltmaskclr;
   /** Fault Mask Set.  Write one to a specific bit to set the corresponding mask bit. at address offset 0x910, write-only */
   uint32_t tcu1_fltmaskset;
   /** Fault Mask Status at address offset 0x914, read-only */
   uint32_t tcu1_fltmaskstatus;
   /** Reserved space */
   uint8_t fill11[232];
   /** Override Event Outputs Set at address offset 0xA00, write-only */
   uint32_t tcu1_setevent;
   /** Override Event Outputs Clear at address offset 0xA04, write-only */
   uint32_t tcu1_clrevent;
   /** Override Event Outputs Pulse at address offset 0xA08, write-only */
   uint32_t tcu1_pulseevent;
   /** Event output status at address offset 0xA0C, read-only */
   uint32_t tcu1_eventstat;
   /** Mask to apply when TSCU is halted at address offset 0xA10, read-write */
   uint32_t tcu1_eventhalt;
   /** Reserved space */
   uint8_t fill12[236];
   /** Configured FIFO size in bytes at address offset 0xB00, read-only */
   uint32_t tcu1_config;
   /** Reserved space */
   uint8_t fill13[252];
   /** Numerator of DPLL fractional divider at address offset 0xC00, read-write */
   uint32_t dpll_x_div;
   /** Denominator of DPLL fractional divider at address offset 0xC04, read-write */
   uint32_t dpll_y_div;
   /** Reserved space */
   uint8_t fill14[968];
   /** Peripheral ID4 Register at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** Peripheral ID5 Register at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID6 Register at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID7 Register at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID0 Register at address offset 0xFE0, read-only constant 0x00000003 */
   uint32_t peripheralid0;
   /** Peripheral ID1 Register at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** Peripheral ID2 Register at address offset 0xFE8, read-only constant 0x00000008 */
   uint32_t peripheralid2;
   /** Peripheral ID3 Register at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Primecell ID register 0 at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Primecell ID register 1 at address offset 0xFF4, read-only constant 0x000000F0 */
   uint32_t componentid1;
   /** Primecell ID register 2 at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Primecell ID register 3 at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for tscu_s#tcu0_ctrl */
#define TSCU_TCU0_CTRL_FIFO_ENABLE_OFFSET 0
#define TSCU_TCU0_CTRL_FIFO_ENABLE_SIZE 1
#define TSCU_TCU0_CTRL_POWER_IF_ENABLE_OFFSET 1
#define TSCU_TCU0_CTRL_POWER_IF_ENABLE_SIZE 1
#define TSCU_TCU0_CTRL_DPLL_ENABLE_OFFSET 2
#define TSCU_TCU0_CTRL_DPLL_ENABLE_SIZE 1
#define TSCU_TCU0_CTRL_TIMING_UNIT_ENABLED_OFFSET 3
#define TSCU_TCU0_CTRL_TIMING_UNIT_ENABLED_SIZE 1

/** bit field defines for tscu_s#tcu0_ssleepset */
#define TSCU_TCU0_SSLEEPSET_TCU0_SSLEEPSET_OFFSET 0
#define TSCU_TCU0_SSLEEPSET_TCU0_SSLEEPSET_SIZE 20

/** bit field defines for tscu_s#tcu0_fsleepset */
#define TSCU_TCU0_FSLEEPSET_TCU0_FSLEEPSET_OFFSET 0
#define TSCU_TCU0_FSLEEPSET_TCU0_FSLEEPSET_SIZE 16

/** bit field defines for tscu_s#tcu0_spwrupdelayset */
#define TSCU_TCU0_SPWRUPDELAYSET_TCU0_SPWRUPDELAYSET_OFFSET 0
#define TSCU_TCU0_SPWRUPDELAYSET_TCU0_SPWRUPDELAYSET_SIZE 20

/** bit field defines for tscu_s#tcu0_definstr */
#define TSCU_TCU0_DEFINSTR_TCU0_DEFINSTR_OFFSET 0
#define TSCU_TCU0_DEFINSTR_TCU0_DEFINSTR_SIZE 16

/** bit field defines for tscu_s#tcu0_assert */
#define TSCU_TCU0_ASSERT_TCU0_ASSERT_OFFSET 0
#define TSCU_TCU0_ASSERT_TCU0_ASSERT_SIZE 1

/** bit field defines for tscu_s#tcu0_abort */
#define TSCU_TCU0_ABORT_ABORT_OFFSET 16
#define TSCU_TCU0_ABORT_ABORT_SIZE 1
#define TSCU_TCU0_ABORT_DPLL_RESET_OFFSET 17
#define TSCU_TCU0_ABORT_DPLL_RESET_SIZE 1

/** bit field defines for tscu_s#tcu0_status */
#define TSCU_TCU0_STATUS_FIFO_ENABLED_OFFSET 0
#define TSCU_TCU0_STATUS_FIFO_ENABLED_SIZE 1
#define TSCU_TCU0_STATUS_POWER_IF_ENABLED_OFFSET 1
#define TSCU_TCU0_STATUS_POWER_IF_ENABLED_SIZE 1
#define TSCU_TCU0_STATUS_DPLL_ENABLED_OFFSET 2
#define TSCU_TCU0_STATUS_DPLL_ENABLED_SIZE 1
#define TSCU_TCU0_STATUS_TIMING_UNIT_ENABLED_OFFSET 3
#define TSCU_TCU0_STATUS_TIMING_UNIT_ENABLED_SIZE 1
#define TSCU_TCU0_STATUS_ABORT_BUSY_OFFSET 16
#define TSCU_TCU0_STATUS_ABORT_BUSY_SIZE 1
#define TSCU_TCU0_STATUS_ASSERT_BUSY_OFFSET 24
#define TSCU_TCU0_STATUS_ASSERT_BUSY_SIZE 1
#define TSCU_TCU0_STATUS_CALIBRATION_BUSY_OFFSET 25
#define TSCU_TCU0_STATUS_CALIBRATION_BUSY_SIZE 1

/** bit field defines for tscu_s#tcu0_ssleepstatus */
#define TSCU_TCU0_SSLEEPSTATUS_TCU0_SSLEEPSTATUS_OFFSET 0
#define TSCU_TCU0_SSLEEPSTATUS_TCU0_SSLEEPSTATUS_SIZE 20

/** bit field defines for tscu_s#tcu0_fsleepstatus */
#define TSCU_TCU0_FSLEEPSTATUS_TCU0_FSLEEPSTATUS_OFFSET 0
#define TSCU_TCU0_FSLEEPSTATUS_TCU0_FSLEEPSTATUS_SIZE 16

/** bit field defines for tscu_s#tcu0_calcnt */
#define TSCU_TCU0_CALCNT_TCU0_CALCNT_OFFSET 0
#define TSCU_TCU0_CALCNT_TCU0_CALCNT_SIZE 16

/** bit field defines for tscu_s#tcu0_sfnemptywm */
#define TSCU_TCU0_SFNEMPTYWM_SFNEMPTYWM_OFFSET 1
#define TSCU_TCU0_SFNEMPTYWM_SFNEMPTYWM_SIZE 7

/** bit field defines for tscu_s#tcu0_sfnfullwm */
#define TSCU_TCU0_SFNFULLWM_SFNFULLWM_OFFSET 1
#define TSCU_TCU0_SFNFULLWM_SFNFULLWM_SIZE 7

/** bit field defines for tscu_s#tcu0_fltclear */
#define TSCU_TCU0_FLTCLEAR_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU0_FLTCLEAR_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU0_FLTCLEAR_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU0_FLTCLEAR_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTCLEAR_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU0_FLTCLEAR_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTCLEAR_SYSTEM_CLOCK_MISSING_OFFSET 3
#define TSCU_TCU0_FLTCLEAR_SYSTEM_CLOCK_MISSING_SIZE 1
#define TSCU_TCU0_FLTCLEAR_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU0_FLTCLEAR_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTCLEAR_CALIBRATION_OVERFLOW_OFFSET 5
#define TSCU_TCU0_FLTCLEAR_CALIBRATION_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTCLEAR_AXI_ERROR_OFFSET 6
#define TSCU_TCU0_FLTCLEAR_AXI_ERROR_SIZE 1
#define TSCU_TCU0_FLTCLEAR_SPREADER_OVERFLOW_OFFSET 7
#define TSCU_TCU0_FLTCLEAR_SPREADER_OVERFLOW_SIZE 1

/** bit field defines for tscu_s#tcu0_fltstatus */
#define TSCU_TCU0_FLTSTATUS_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU0_FLTSTATUS_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU0_FLTSTATUS_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU0_FLTSTATUS_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTSTATUS_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU0_FLTSTATUS_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTSTATUS_SYSTEM_CLOCK_MISSING_OFFSET 3
#define TSCU_TCU0_FLTSTATUS_SYSTEM_CLOCK_MISSING_SIZE 1
#define TSCU_TCU0_FLTSTATUS_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU0_FLTSTATUS_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTSTATUS_CALIBRATION_OVERFLOW_OFFSET 5
#define TSCU_TCU0_FLTSTATUS_CALIBRATION_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTSTATUS_AXI_ERROR_OFFSET 6
#define TSCU_TCU0_FLTSTATUS_AXI_ERROR_SIZE 1
#define TSCU_TCU0_FLTSTATUS_SPREADER_OVERFLOW_OFFSET 7
#define TSCU_TCU0_FLTSTATUS_SPREADER_OVERFLOW_SIZE 1

/** bit field defines for tscu_s#tcu0_fltrawstatus */
#define TSCU_TCU0_FLTRAWSTATUS_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU0_FLTRAWSTATUS_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU0_FLTRAWSTATUS_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU0_FLTRAWSTATUS_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTRAWSTATUS_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU0_FLTRAWSTATUS_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTRAWSTATUS_SYSTEM_CLOCK_MISSING_OFFSET 3
#define TSCU_TCU0_FLTRAWSTATUS_SYSTEM_CLOCK_MISSING_SIZE 1
#define TSCU_TCU0_FLTRAWSTATUS_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU0_FLTRAWSTATUS_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTRAWSTATUS_CALIBRATION_OVERFLOW_OFFSET 5
#define TSCU_TCU0_FLTRAWSTATUS_CALIBRATION_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTRAWSTATUS_AXI_ERROR_OFFSET 6
#define TSCU_TCU0_FLTRAWSTATUS_AXI_ERROR_SIZE 1
#define TSCU_TCU0_FLTRAWSTATUS_SPREADER_OVERFLOW_OFFSET 7
#define TSCU_TCU0_FLTRAWSTATUS_SPREADER_OVERFLOW_SIZE 1

/** bit field defines for tscu_s#tcu0_fltmaskclr */
#define TSCU_TCU0_FLTMASKCLR_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU0_FLTMASKCLR_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU0_FLTMASKCLR_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU0_FLTMASKCLR_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKCLR_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU0_FLTMASKCLR_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKCLR_SYSTEM_CLOCK_MISSING_OFFSET 3
#define TSCU_TCU0_FLTMASKCLR_SYSTEM_CLOCK_MISSING_SIZE 1
#define TSCU_TCU0_FLTMASKCLR_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU0_FLTMASKCLR_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKCLR_CALIBRATION_OVERFLOW_OFFSET 5
#define TSCU_TCU0_FLTMASKCLR_CALIBRATION_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKCLR_AXI_ERROR_OFFSET 6
#define TSCU_TCU0_FLTMASKCLR_AXI_ERROR_SIZE 1
#define TSCU_TCU0_FLTMASKCLR_SPREADER_OVERFLOW_OFFSET 7
#define TSCU_TCU0_FLTMASKCLR_SPREADER_OVERFLOW_SIZE 1

/** bit field defines for tscu_s#tcu0_fltmaskset */
#define TSCU_TCU0_FLTMASKSET_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU0_FLTMASKSET_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU0_FLTMASKSET_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU0_FLTMASKSET_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKSET_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU0_FLTMASKSET_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKSET_SYSTEM_CLOCK_MISSING_OFFSET 3
#define TSCU_TCU0_FLTMASKSET_SYSTEM_CLOCK_MISSING_SIZE 1
#define TSCU_TCU0_FLTMASKSET_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU0_FLTMASKSET_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKSET_CALIBRATION_OVERFLOW_OFFSET 5
#define TSCU_TCU0_FLTMASKSET_CALIBRATION_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKSET_AXI_ERROR_OFFSET 6
#define TSCU_TCU0_FLTMASKSET_AXI_ERROR_SIZE 1
#define TSCU_TCU0_FLTMASKSET_SPREADER_OVERFLOW_OFFSET 7
#define TSCU_TCU0_FLTMASKSET_SPREADER_OVERFLOW_SIZE 1

/** bit field defines for tscu_s#tcu0_fltmaskstatus */
#define TSCU_TCU0_FLTMASKSTATUS_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU0_FLTMASKSTATUS_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU0_FLTMASKSTATUS_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU0_FLTMASKSTATUS_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKSTATUS_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU0_FLTMASKSTATUS_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKSTATUS_SYSTEM_CLOCK_MISSING_OFFSET 3
#define TSCU_TCU0_FLTMASKSTATUS_SYSTEM_CLOCK_MISSING_SIZE 1
#define TSCU_TCU0_FLTMASKSTATUS_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU0_FLTMASKSTATUS_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKSTATUS_CALIBRATION_OVERFLOW_OFFSET 5
#define TSCU_TCU0_FLTMASKSTATUS_CALIBRATION_OVERFLOW_SIZE 1
#define TSCU_TCU0_FLTMASKSTATUS_AXI_ERROR_OFFSET 6
#define TSCU_TCU0_FLTMASKSTATUS_AXI_ERROR_SIZE 1
#define TSCU_TCU0_FLTMASKSTATUS_SPREADER_OVERFLOW_OFFSET 7
#define TSCU_TCU0_FLTMASKSTATUS_SPREADER_OVERFLOW_SIZE 1

/** bit field defines for tscu_s#tcu0_setevent */
#define TSCU_TCU0_SETEVENT_SETFEVENT_OFFSET 0
#define TSCU_TCU0_SETEVENT_SETFEVENT_SIZE 22

/** bit field defines for tscu_s#tcu0_clrevent */
#define TSCU_TCU0_CLREVENT_CLRFEVENT_OFFSET 0
#define TSCU_TCU0_CLREVENT_CLRFEVENT_SIZE 22

/** bit field defines for tscu_s#tcu0_pulseevent */
#define TSCU_TCU0_PULSEEVENT_PULSEFEVENT_OFFSET 0
#define TSCU_TCU0_PULSEEVENT_PULSEFEVENT_SIZE 22
#define TSCU_TCU0_PULSEEVENT_PULSEFEVENT_SLEEPSTATE_REQ_OFFSET 22
#define TSCU_TCU0_PULSEEVENT_PULSEFEVENT_SLEEPSTATE_REQ_SIZE 1
#define TSCU_TCU0_PULSEEVENT_PULSEFEVENT_CALIBRATE_REQ_OFFSET 23
#define TSCU_TCU0_PULSEEVENT_PULSEFEVENT_CALIBRATE_REQ_SIZE 1

/** bit field defines for tscu_s#tcu0_eventstat */
#define TSCU_TCU0_EVENTSTAT_FEVENTSTAT_OFFSET 0
#define TSCU_TCU0_EVENTSTAT_FEVENTSTAT_SIZE 22

/** bit field defines for tscu_s#tcu0_eventhalt */
#define TSCU_TCU0_EVENTHALT_HALTFEVENT_OFFSET 0
#define TSCU_TCU0_EVENTHALT_HALTFEVENT_SIZE 22

/** bit field defines for tscu_s#tcu0_config */
#define TSCU_TCU0_CONFIG_TCU0_CONFIG_OFFSET 0
#define TSCU_TCU0_CONFIG_TCU0_CONFIG_SIZE 16

/** bit field defines for tscu_s#tcu1_ctrl */
#define TSCU_TCU1_CTRL_FIFO_ENABLE_OFFSET 0
#define TSCU_TCU1_CTRL_FIFO_ENABLE_SIZE 1

/** bit field defines for tscu_s#tcu1_abort */
#define TSCU_TCU1_ABORT_ABORT_OFFSET 16
#define TSCU_TCU1_ABORT_ABORT_SIZE 1

/** bit field defines for tscu_s#tcu1_status */
#define TSCU_TCU1_STATUS_FIFO_ENABLED_OFFSET 0
#define TSCU_TCU1_STATUS_FIFO_ENABLED_SIZE 1
#define TSCU_TCU1_STATUS_ABORT_BUSY_OFFSET 16
#define TSCU_TCU1_STATUS_ABORT_BUSY_SIZE 1

/** bit field defines for tscu_s#tcu1_sfnemptywm */
#define TSCU_TCU1_SFNEMPTYWM_SFNEMPTYWM_OFFSET 1
#define TSCU_TCU1_SFNEMPTYWM_SFNEMPTYWM_SIZE 7

/** bit field defines for tscu_s#tcu1_sfnfullwm */
#define TSCU_TCU1_SFNFULLWM_SFNFULLWM_OFFSET 1
#define TSCU_TCU1_SFNFULLWM_SFNFULLWM_SIZE 7

/** bit field defines for tscu_s#tcu1_fltclear */
#define TSCU_TCU1_FLTCLEAR_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU1_FLTCLEAR_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU1_FLTCLEAR_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU1_FLTCLEAR_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTCLEAR_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU1_FLTCLEAR_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTCLEAR_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU1_FLTCLEAR_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTCLEAR_AXI_ERROR_OFFSET 6
#define TSCU_TCU1_FLTCLEAR_AXI_ERROR_SIZE 1

/** bit field defines for tscu_s#tcu1_fltstatus */
#define TSCU_TCU1_FLTSTATUS_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU1_FLTSTATUS_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU1_FLTSTATUS_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU1_FLTSTATUS_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTSTATUS_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU1_FLTSTATUS_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTSTATUS_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU1_FLTSTATUS_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTSTATUS_AXI_ERROR_OFFSET 6
#define TSCU_TCU1_FLTSTATUS_AXI_ERROR_SIZE 1

/** bit field defines for tscu_s#tcu1_fltrawstatus */
#define TSCU_TCU1_FLTRAWSTATUS_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU1_FLTRAWSTATUS_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU1_FLTRAWSTATUS_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU1_FLTRAWSTATUS_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTRAWSTATUS_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU1_FLTRAWSTATUS_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTRAWSTATUS_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU1_FLTRAWSTATUS_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTRAWSTATUS_AXI_ERROR_OFFSET 6
#define TSCU_TCU1_FLTRAWSTATUS_AXI_ERROR_SIZE 1

/** bit field defines for tscu_s#tcu1_fltmaskclr */
#define TSCU_TCU1_FLTMASKCLR_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU1_FLTMASKCLR_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU1_FLTMASKCLR_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU1_FLTMASKCLR_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTMASKCLR_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU1_FLTMASKCLR_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTMASKCLR_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU1_FLTMASKCLR_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTMASKCLR_AXI_ERROR_OFFSET 6
#define TSCU_TCU1_FLTMASKCLR_AXI_ERROR_SIZE 1

/** bit field defines for tscu_s#tcu1_fltmaskset */
#define TSCU_TCU1_FLTMASKSET_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU1_FLTMASKSET_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU1_FLTMASKSET_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU1_FLTMASKSET_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTMASKSET_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU1_FLTMASKSET_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTMASKSET_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU1_FLTMASKSET_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTMASKSET_AXI_ERROR_OFFSET 6
#define TSCU_TCU1_FLTMASKSET_AXI_ERROR_SIZE 1

/** bit field defines for tscu_s#tcu1_fltmaskstatus */
#define TSCU_TCU1_FLTMASKSTATUS_SEQUENCE_UNDERRUN_OFFSET 0
#define TSCU_TCU1_FLTMASKSTATUS_SEQUENCE_UNDERRUN_SIZE 1
#define TSCU_TCU1_FLTMASKSTATUS_SEQUENCE_OVERFLOW_OFFSET 1
#define TSCU_TCU1_FLTMASKSTATUS_SEQUENCE_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTMASKSTATUS_FIFO_OVERFLOW_OFFSET 2
#define TSCU_TCU1_FLTMASKSTATUS_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTMASKSTATUS_APB_FIFO_OVERFLOW_OFFSET 4
#define TSCU_TCU1_FLTMASKSTATUS_APB_FIFO_OVERFLOW_SIZE 1
#define TSCU_TCU1_FLTMASKSTATUS_AXI_ERROR_OFFSET 6
#define TSCU_TCU1_FLTMASKSTATUS_AXI_ERROR_SIZE 1

/** bit field defines for tscu_s#tcu1_setevent */
#define TSCU_TCU1_SETEVENT_SETFEVENT_OFFSET 0
#define TSCU_TCU1_SETEVENT_SETFEVENT_SIZE 16

/** bit field defines for tscu_s#tcu1_clrevent */
#define TSCU_TCU1_CLREVENT_CLRFEVENT_OFFSET 0
#define TSCU_TCU1_CLREVENT_CLRFEVENT_SIZE 16

/** bit field defines for tscu_s#tcu1_pulseevent */
#define TSCU_TCU1_PULSEEVENT_PULSEFEVENT_OFFSET 0
#define TSCU_TCU1_PULSEEVENT_PULSEFEVENT_SIZE 16

/** bit field defines for tscu_s#tcu1_eventstat */
#define TSCU_TCU1_EVENTSTAT_FEVENTSTAT_OFFSET 0
#define TSCU_TCU1_EVENTSTAT_FEVENTSTAT_SIZE 16

/** bit field defines for tscu_s#tcu1_eventhalt */
#define TSCU_TCU1_EVENTHALT_HALTFEVENT_OFFSET 0
#define TSCU_TCU1_EVENTHALT_HALTFEVENT_SIZE 16

/** bit field defines for tscu_s#tcu1_config */
#define TSCU_TCU1_CONFIG_TCU1_CONFIG_OFFSET 0
#define TSCU_TCU1_CONFIG_TCU1_CONFIG_SIZE 16

/** bit field defines for tscu_s#dpll_x_div */
#define TSCU_DPLL_X_DIV_X_DIV_OFFSET 0
#define TSCU_DPLL_X_DIV_X_DIV_SIZE 24

/** bit field defines for tscu_s#dpll_y_div */
#define TSCU_DPLL_Y_DIV_Y_DIV_OFFSET 0
#define TSCU_DPLL_Y_DIV_Y_DIV_SIZE 24

/** bit field defines for tscu_s#peripheralid4 */
#define TSCU_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define TSCU_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for tscu_s#peripheralid5 */
#define TSCU_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define TSCU_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for tscu_s#peripheralid6 */
#define TSCU_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define TSCU_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for tscu_s#peripheralid7 */
#define TSCU_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define TSCU_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for tscu_s#peripheralid0 */
#define TSCU_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define TSCU_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for tscu_s#peripheralid1 */
#define TSCU_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define TSCU_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for tscu_s#peripheralid2 */
#define TSCU_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define TSCU_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for tscu_s#peripheralid3 */
#define TSCU_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define TSCU_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for tscu_s#componentid0 */
#define TSCU_COMPONENTID0_COMPONENTID0_OFFSET 0
#define TSCU_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for tscu_s#componentid1 */
#define TSCU_COMPONENTID1_COMPONENTID1_OFFSET 0
#define TSCU_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for tscu_s#componentid2 */
#define TSCU_COMPONENTID2_COMPONENTID2_OFFSET 0
#define TSCU_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for tscu_s#componentid3 */
#define TSCU_COMPONENTID3_COMPONENTID3_OFFSET 0
#define TSCU_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF tscu.h */
#endif
