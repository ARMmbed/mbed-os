#ifndef INCLUDED_PWM_TOP
#define INCLUDED_PWM_TOP
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** pwm_top HAL Spreadsheet version number */
#define PWM_TOP_HAL_VERSION 1

/** This is the PWM Top register file description
*/
struct pwm_top_s {
   /** Load the shadow registers at address offset 0x000, read-write */
   uint32_t tc_sh_reg_load;
   /** Counter Configuration at address offset 0x004, read-write */
   uint32_t tc_cfg;
   /** NCO Frequency Register 1 at address offset 0x008, read-write */
   uint32_t tp_freq;
   /**  at address offset 0x00C, read-write */
   uint32_t tp_ph;
   /**  at address offset 0x010, read-write */
   uint32_t tp_duty;
   /**  at address offset 0x014, read-write */
   uint32_t tp_ovf;
   /** NCO Frequency Register 2 at address offset 0x018, read-write */
   uint32_t tp_freq2;
   /**  at address offset 0x01C, read-write */
   uint32_t tp_ph2;
   /**  at address offset 0x020, read-write */
   uint32_t tp_duty2;
   /**  at address offset 0x024, read-write */
   uint32_t tp_ovf2;
   /** NCO Frequency Register 2 at address offset 0x028, read-write */
   uint32_t tp_freq3;
   /**  at address offset 0x02C, read-write */
   uint32_t tp_ph3;
   /**  at address offset 0x030, read-write */
   uint32_t tp_duty3;
   /**  at address offset 0x034, read-write */
   uint32_t tp_ovf3;
};

/** bit field defines for pwm_top_s#tc_sh_reg_load */
#define PWM_TOP_TC_SH_REG_LOAD_TC_SHADOW_REG_LOAD_OFFSET 0
#define PWM_TOP_TC_SH_REG_LOAD_TC_SHADOW_REG_LOAD_SIZE 1

/** bit field defines for pwm_top_s#tc_cfg */
#define PWM_TOP_TC_CFG_TC_VALUE_COUNT_OFFSET 0
#define PWM_TOP_TC_CFG_TC_VALUE_COUNT_SIZE 16
#define PWM_TOP_TC_CFG_TC_ENABLE_OFFSET 16
#define PWM_TOP_TC_CFG_TC_ENABLE_SIZE 1
#define PWM_TOP_TC_CFG_TC_RESETN_OFFSET 17
#define PWM_TOP_TC_CFG_TC_RESETN_SIZE 1
#define PWM_TOP_TC_CFG_TC_RELOAD_OFFSET 18
#define PWM_TOP_TC_CFG_TC_RELOAD_SIZE 1

/** bit field defines for pwm_top_s#tp_freq */
#define PWM_TOP_TP_FREQ_TP_FREQUENCY_OFFSET 0
#define PWM_TOP_TP_FREQ_TP_FREQUENCY_SIZE 18
#define PWM_TOP_TP_FREQ_TP_SIGMADELTA_OFFSET 28
#define PWM_TOP_TP_FREQ_TP_SIGMADELTA_SIZE 1
#define PWM_TOP_TP_FREQ_TP_POLARITY_OFFSET 29
#define PWM_TOP_TP_FREQ_TP_POLARITY_SIZE 1
#define PWM_TOP_TP_FREQ_TP_ENABLE_OFFSET 30
#define PWM_TOP_TP_FREQ_TP_ENABLE_SIZE 1

/** bit field defines for pwm_top_s#tp_ph */
#define PWM_TOP_TP_PH_TP_PHASE_OFFSET 0
#define PWM_TOP_TP_PH_TP_PHASE_SIZE 18

/** bit field defines for pwm_top_s#tp_duty */
#define PWM_TOP_TP_DUTY_TP_DUTYCYCLE_OFFSET 0
#define PWM_TOP_TP_DUTY_TP_DUTYCYCLE_SIZE 18

/** bit field defines for pwm_top_s#tp_ovf */
#define PWM_TOP_TP_OVF_TP_OVERFLOW_OFFSET 0
#define PWM_TOP_TP_OVF_TP_OVERFLOW_SIZE 18

/** bit field defines for pwm_top_s#tp_freq2 */
#define PWM_TOP_TP_FREQ2_TP_FREQUENCY2_OFFSET 0
#define PWM_TOP_TP_FREQ2_TP_FREQUENCY2_SIZE 18
#define PWM_TOP_TP_FREQ2_TP_SIGMADELTA2_OFFSET 28
#define PWM_TOP_TP_FREQ2_TP_SIGMADELTA2_SIZE 1
#define PWM_TOP_TP_FREQ2_TP_POLARITY2_OFFSET 29
#define PWM_TOP_TP_FREQ2_TP_POLARITY2_SIZE 1
#define PWM_TOP_TP_FREQ2_TP_ENABLE2_OFFSET 30
#define PWM_TOP_TP_FREQ2_TP_ENABLE2_SIZE 1

/** bit field defines for pwm_top_s#tp_ph2 */
#define PWM_TOP_TP_PH2_TP_PHASE2_OFFSET 0
#define PWM_TOP_TP_PH2_TP_PHASE2_SIZE 18

/** bit field defines for pwm_top_s#tp_duty2 */
#define PWM_TOP_TP_DUTY2_TP_DUTYCYCLE2_OFFSET 0
#define PWM_TOP_TP_DUTY2_TP_DUTYCYCLE2_SIZE 18

/** bit field defines for pwm_top_s#tp_ovf2 */
#define PWM_TOP_TP_OVF2_TP_OVERFLOW2_OFFSET 0
#define PWM_TOP_TP_OVF2_TP_OVERFLOW2_SIZE 18

/** bit field defines for pwm_top_s#tp_freq3 */
#define PWM_TOP_TP_FREQ3_TP_FREQUENCY3_OFFSET 0
#define PWM_TOP_TP_FREQ3_TP_FREQUENCY3_SIZE 18
#define PWM_TOP_TP_FREQ3_TP_SIGMADELTA3_OFFSET 28
#define PWM_TOP_TP_FREQ3_TP_SIGMADELTA3_SIZE 1
#define PWM_TOP_TP_FREQ3_TP_POLARITY3_OFFSET 29
#define PWM_TOP_TP_FREQ3_TP_POLARITY3_SIZE 1
#define PWM_TOP_TP_FREQ3_TP_ENABLE3_OFFSET 30
#define PWM_TOP_TP_FREQ3_TP_ENABLE3_SIZE 1

/** bit field defines for pwm_top_s#tp_ph3 */
#define PWM_TOP_TP_PH3_TP_PHASE3_OFFSET 0
#define PWM_TOP_TP_PH3_TP_PHASE3_SIZE 18

/** bit field defines for pwm_top_s#tp_duty3 */
#define PWM_TOP_TP_DUTY3_TP_DUTYCYCLE3_OFFSET 0
#define PWM_TOP_TP_DUTY3_TP_DUTYCYCLE3_SIZE 18

/** bit field defines for pwm_top_s#tp_ovf3 */
#define PWM_TOP_TP_OVF3_TP_OVERFLOW3_OFFSET 0
#define PWM_TOP_TP_OVF3_TP_OVERFLOW3_SIZE 18

/* EOF pwm_top.h */
#endif
