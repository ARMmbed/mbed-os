#ifndef INCLUDED_PIO_ONFI_CAL
#define INCLUDED_PIO_ONFI_CAL
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** pio_onfi_cal HAL Spreadsheet version number */
#define PIO_ONFI_CAL_HAL_VERSION 1

/** Control for the Aragio ONFI Pad Calibration Cell and additional Aragio ONFI pad control
*/
struct pio_onfi_cal_s {
   /** ONFI Cal FSM Control Reg at address offset 0x000, read-write */
   uint32_t onficalctrl;
   /** ONFI Cal FSM status at address offset 0x004, read-write */
   uint32_t onficalstat;
   /** Returned Calibration Values at address offset 0x008, read-only */
   uint32_t onficalval;
   /** Calibration value to send to ONFI pads at address offset 0x00C, read-write */
   uint32_t onficalset;
   /** Interrupt Mask at address offset 0x010, read-write */
   uint32_t onficalintmask;
   /** Timout control for the Cal FSM at address offset 0x014, read-write */
   uint32_t onficaltimeout;
   /** ONFI pad misc control at address offset 0x018, read-write */
   uint32_t onfipad;
};

/** bit field defines for pio_onfi_cal_s#onficalctrl */
#define PIO_ONFI_CAL_ONFICALCTRL_START_OFFSET 0
#define PIO_ONFI_CAL_ONFICALCTRL_START_SIZE 1
#define PIO_ONFI_CAL_ONFICALCTRL_ABORT_OFFSET 1
#define PIO_ONFI_CAL_ONFICALCTRL_ABORT_SIZE 1

/** bit field defines for pio_onfi_cal_s#onficalstat */
#define PIO_ONFI_CAL_ONFICALSTAT_DONE_OFFSET 0
#define PIO_ONFI_CAL_ONFICALSTAT_DONE_SIZE 1
#define PIO_ONFI_CAL_ONFICALSTAT_TIMEOUT_OFFSET 1
#define PIO_ONFI_CAL_ONFICALSTAT_TIMEOUT_SIZE 1

/** bit field defines for pio_onfi_cal_s#onficalval */
#define PIO_ONFI_CAL_ONFICALVAL_CAL_DRP_OFFSET 0
#define PIO_ONFI_CAL_ONFICALVAL_CAL_DRP_SIZE 4
#define PIO_ONFI_CAL_ONFICALVAL_CAL_DRN_OFFSET 4
#define PIO_ONFI_CAL_ONFICALVAL_CAL_DRN_SIZE 4

/** bit field defines for pio_onfi_cal_s#onficalset */
#define PIO_ONFI_CAL_ONFICALSET_CAL_DWHVP_OFFSET 0
#define PIO_ONFI_CAL_ONFICALSET_CAL_DWHVP_SIZE 4
#define PIO_ONFI_CAL_ONFICALSET_CAL_DWHVN_OFFSET 4
#define PIO_ONFI_CAL_ONFICALSET_CAL_DWHVN_SIZE 4

/** bit field defines for pio_onfi_cal_s#onficalintmask */
#define PIO_ONFI_CAL_ONFICALINTMASK_DONE_OFFSET 0
#define PIO_ONFI_CAL_ONFICALINTMASK_DONE_SIZE 1
#define PIO_ONFI_CAL_ONFICALINTMASK_TIMEOUT_OFFSET 1
#define PIO_ONFI_CAL_ONFICALINTMASK_TIMEOUT_SIZE 1

/** bit field defines for pio_onfi_cal_s#onficaltimeout */
#define PIO_ONFI_CAL_ONFICALTIMEOUT_VAL_OFFSET 0
#define PIO_ONFI_CAL_ONFICALTIMEOUT_VAL_SIZE 12

/** bit field defines for pio_onfi_cal_s#onfipad */
#define PIO_ONFI_CAL_ONFIPAD_GPO_OFFSET 0
#define PIO_ONFI_CAL_ONFIPAD_GPO_SIZE 32

/* EOF pio_onfi_cal.h */
#endif
