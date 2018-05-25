#ifndef INCLUDED_AKIRA_CM3_SS_PPB
#define INCLUDED_AKIRA_CM3_SS_PPB
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>


/** Private pripheral bus, light sleep controller slave
*/
struct akira_cm3_ss_ppb_s {
   /** Two to the power of the value of this register is number of clock cycles that will pass between light sleep being requested and the memory entering light sleep. at address offset 0x000, read-write */
   uint32_t ls_ctrl_threshold;
   /** Two to the power of the value of this register is the free running clock division factor while the CPU is sleeping. at address offset 0x004, read-write */
   uint32_t clk_lowering_div;
};

/** bit field defines for akira_cm3_ss_ppb_s#ls_ctrl_threshold */
#define AKIRA_CM3_SS_PPB_LS_CTRL_THRESHOLD_THRESHOLD_OFFSET 0
#define AKIRA_CM3_SS_PPB_LS_CTRL_THRESHOLD_THRESHOLD_SIZE 3

/** bit field defines for akira_cm3_ss_ppb_s#clk_lowering_div */
#define AKIRA_CM3_SS_PPB_CLK_LOWERING_DIV_DIV_FACTOR_OFFSET 0
#define AKIRA_CM3_SS_PPB_CLK_LOWERING_DIV_DIV_FACTOR_SIZE 3

/* EOF akira_cm3_ss_ppb.h */
#endif
