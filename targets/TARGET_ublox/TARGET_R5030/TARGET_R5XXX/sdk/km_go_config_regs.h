#ifndef INCLUDED_KM_GO_CONFIG_REGS
#define INCLUDED_KM_GO_CONFIG_REGS
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** km_go_config_regs HAL Spreadsheet version number */
#define KM_GO_CONFIG_REGS_HAL_VERSION 4

/** KM GO RF Digital Configuration Registers
*/
struct km_go_config_regs_s {
   /** External GPIO Pin Connectivity Control at address offset 0x000, read-write */
   uint32_t congpio;
   /** Configure the GPO port as a GPI.  One bit per signal. at address offset 0x004, read-write */
   uint32_t congpiodir;
   /** Current Value of the GPI signal.  Only valid in sch_gpio mode when the associated bit in ConGPIODir is set to GPI  at address offset 0x008, read-only */
   uint32_t congpiostatus;
   /** Debug Mux configuration at address offset 0x00C, read-write */
   uint32_t condbgmux;
   /** DigIF Interface configuration at address offset 0x010, read-write */
   uint32_t condigifcfg;
   /** Configuration for the Interrupt Vector synchronisers to DigClk at address offset 0x014, read-write */
   uint32_t conirqcfg;
};

/** bit field defines for km_go_config_regs_s#congpio */
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN0_OFFSET 0
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN0_SIZE 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN1_OFFSET 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN1_SIZE 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN2_OFFSET 4
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN2_SIZE 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN3_OFFSET 6
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN3_SIZE 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN4_OFFSET 8
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN4_SIZE 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN5_OFFSET 10
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN5_SIZE 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN6_OFFSET 12
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN6_SIZE 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN7_OFFSET 14
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN7_SIZE 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN8_OFFSET 16
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN8_SIZE 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN9_OFFSET 18
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN9_SIZE 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN10_OFFSET 20
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN10_SIZE 2
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN11_OFFSET 22
#define KM_GO_CONFIG_REGS_CONGPIO_CFGPIN11_SIZE 2

/** bit field defines for km_go_config_regs_s#congpiodir */
#define KM_GO_CONFIG_REGS_CONGPIODIR_NONE_OFFSET 0
#define KM_GO_CONFIG_REGS_CONGPIODIR_NONE_SIZE 12
/** Per Bit set to GPI to make GPIO an Output when in GPO mode. */
#define KM_GO_CONFIG_REGS_CONGPIODIR_NONE_GPI_VALUE 0
/** Per Bit set to GPO to make GPIO an Output when in GPO mode. */
#define KM_GO_CONFIG_REGS_CONGPIODIR_NONE_GPO_VALUE 1

/** bit field defines for km_go_config_regs_s#congpiostatus */
#define KM_GO_CONFIG_REGS_CONGPIOSTATUS_NONE_OFFSET 0
#define KM_GO_CONFIG_REGS_CONGPIOSTATUS_NONE_SIZE 12

/** bit field defines for km_go_config_regs_s#condbgmux */
#define KM_GO_CONFIG_REGS_CONDBGMUX_SEL_OFFSET 0
#define KM_GO_CONFIG_REGS_CONDBGMUX_SEL_SIZE 5

/** bit field defines for km_go_config_regs_s#condigifcfg */
#define KM_GO_CONFIG_REGS_CONDIGIFCFG_RDHOLDCYC_OFFSET 0
#define KM_GO_CONFIG_REGS_CONDIGIFCFG_RDHOLDCYC_SIZE 1

/** bit field defines for km_go_config_regs_s#conirqcfg */
#define KM_GO_CONFIG_REGS_CONIRQCFG_PLLSYNCENABLE_OFFSET 0
#define KM_GO_CONFIG_REGS_CONIRQCFG_PLLSYNCENABLE_SIZE 1
#define KM_GO_CONFIG_REGS_CONIRQCFG_MDMSYNCENABLE_OFFSET 1
#define KM_GO_CONFIG_REGS_CONIRQCFG_MDMSYNCENABLE_SIZE 1

/* EOF km_go_config_regs.h */
#endif
