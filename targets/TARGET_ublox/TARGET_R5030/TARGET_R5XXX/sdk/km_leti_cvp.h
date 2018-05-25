#ifndef INCLUDED_KM_LETI_CVP
#define INCLUDED_KM_LETI_CVP
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** km_leti_cvp HAL Spreadsheet version number */
#define KM_LETI_CVP_HAL_VERSION 3

/** DVS CVP
*/
struct km_leti_cvp_s {
   /**  at address offset 0x000, read-only */
   uint32_t itc_it_flags;
   /**  at address offset 0x004, read-write */
   uint32_t itc_it_debug;
   /** Reserved space */
   uint8_t fill0[8];
   /**  at address offset 0x010, read-write */
   uint32_t itc_it0_mask;
   /**  at address offset 0x014, read-only */
   uint32_t itc_it0_pending;
   /** Reserved space */
   uint8_t fill1[4];
   /**  at address offset 0x01C, read-only */
   uint32_t itc_it0_clear_pending;
   /**  at address offset 0x020, read-write */
   uint32_t itc_it1_mask;
   /**  at address offset 0x024, read-only */
   uint32_t itc_it1_pending;
   /** Reserved space */
   uint8_t fill2[4];
   /**  at address offset 0x02C, read-only */
   uint32_t itc_it1_clear_pending;
   /**  at address offset 0x030, read-write */
   uint32_t itc_it_clk_safe_mask;
   /**  at address offset 0x034, read-only */
   uint32_t itc_it_clk_safe_pending;
   /** Reserved space */
   uint8_t fill3[72];
   /**  at address offset 0x080, read-only */
   uint32_t pd_app0_tmflt_status0;
   /**  at address offset 0x084, read-only */
   uint32_t pd_app0_tmflt_status1;
   /**  at address offset 0x088, read-only */
   uint32_t pd_app0_tmflt_status2;
   /**  at address offset 0x08C, read-only */
   uint32_t pd_app0_tmflt_status3;
   /**  at address offset 0x090, read-write */
   uint32_t pd_app0_tmflt_warning_mask0;
   /**  at address offset 0x094, read-write */
   uint32_t pd_app0_tmflt_warning_mask1;
   /**  at address offset 0x098, read-write */
   uint32_t pd_app0_tmflt_warning_mask2;
   /**  at address offset 0x09C, read-write */
   uint32_t pd_app0_tmflt_warning_mask3;
   /**  at address offset 0x0A0, read-write */
   uint32_t pd_app0_tmflt_control;
   /** Reserved space */
   uint8_t fill4[92];
   /**  at address offset 0x100, read-only */
   uint32_t pd_app1_tmflt_status0;
   /**  at address offset 0x104, read-only */
   uint32_t pd_app1_tmflt_status1;
   /**  at address offset 0x108, read-only */
   uint32_t pd_app1_tmflt_status2;
   /**  at address offset 0x10C, read-only */
   uint32_t pd_app1_tmflt_status3;
   /**  at address offset 0x110, read-write */
   uint32_t pd_app1_tmflt_warning_mask0;
   /**  at address offset 0x114, read-write */
   uint32_t pd_app1_tmflt_warning_mask1;
   /**  at address offset 0x118, read-write */
   uint32_t pd_app1_tmflt_warning_mask2;
   /**  at address offset 0x11C, read-write */
   uint32_t pd_app1_tmflt_warning_mask3;
   /**  at address offset 0x120, read-write */
   uint32_t pd_app1_tmflt_control;
   /** Reserved space */
   uint8_t fill5[92];
   /**  at address offset 0x180, read-only */
   uint32_t pd_modem0_tmflt_status0;
   /**  at address offset 0x184, read-only */
   uint32_t pd_modem0_tmflt_status1;
   /**  at address offset 0x188, read-only */
   uint32_t pd_modem0_tmflt_status2;
   /**  at address offset 0x18C, read-only */
   uint32_t pd_modem0_tmflt_status3;
   /**  at address offset 0x190, read-write */
   uint32_t pd_modem0_tmflt_warning_mask0;
   /**  at address offset 0x194, read-write */
   uint32_t pd_modem0_tmflt_warning_mask1;
   /**  at address offset 0x198, read-write */
   uint32_t pd_modem0_tmflt_warning_mask2;
   /**  at address offset 0x19C, read-write */
   uint32_t pd_modem0_tmflt_warning_mask3;
   /**  at address offset 0x1A0, read-write */
   uint32_t pd_modem0_tmflt_control;
   /** Reserved space */
   uint8_t fill6[92];
   /**  at address offset 0x200, read-only */
   uint32_t pd_modem1_tmflt_status0;
   /**  at address offset 0x204, read-only */
   uint32_t pd_modem1_tmflt_status1;
   /**  at address offset 0x208, read-only */
   uint32_t pd_modem1_tmflt_status2;
   /**  at address offset 0x20C, read-only */
   uint32_t pd_modem1_tmflt_status3;
   /**  at address offset 0x210, read-write */
   uint32_t pd_modem1_tmflt_warning_mask0;
   /**  at address offset 0x214, read-write */
   uint32_t pd_modem1_tmflt_warning_mask1;
   /**  at address offset 0x218, read-write */
   uint32_t pd_modem1_tmflt_warning_mask2;
   /**  at address offset 0x21C, read-write */
   uint32_t pd_modem1_tmflt_warning_mask3;
   /**  at address offset 0x220, read-write */
   uint32_t pd_modem1_tmflt_control;
   /** Reserved space */
   uint8_t fill7[92];
   /**  at address offset 0x280, read-write */
   uint32_t pd_app0_ctrl_power_mode0;
   /**  at address offset 0x284, read-write */
   uint32_t pd_app0_ctrl_power_mode1;
   /**  at address offset 0x288, read-write */
   uint32_t pd_app0_ctrl_power_mode2;
   /**  at address offset 0x28C, read-write */
   uint32_t pd_app0_ctrl_power_mode3;
   /**  at address offset 0x290, read-write */
   uint32_t pd_app0_ctrl_power_mode4;
   /**  at address offset 0x294, read-write */
   uint32_t pd_app0_ctrl_power_mode5;
   /**  at address offset 0x298, read-write */
   uint32_t pd_app0_ctrl_power_mode6;
   /**  at address offset 0x29C, read-write */
   uint32_t pd_app0_ctrl_power_mode7;
   /**  at address offset 0x2A0, read-only */
   uint32_t pd_app0_ring_signature;
   /**  at address offset 0x2A4, read-only */
   uint32_t pd_app0_ring_status;
   /**  at address offset 0x2A8, read-write */
   uint32_t pd_app0_ring_control;
   /**  at address offset 0x2AC, read-write */
   uint32_t pd_app0_ring_debug;
   /** Reserved space */
   uint8_t fill8[80];
   /**  at address offset 0x300, read-write */
   uint32_t pd_app1_ctrl_power_mode0;
   /**  at address offset 0x304, read-write */
   uint32_t pd_app1_ctrl_power_mode1;
   /**  at address offset 0x308, read-write */
   uint32_t pd_app1_ctrl_power_mode2;
   /**  at address offset 0x30C, read-write */
   uint32_t pd_app1_ctrl_power_mode3;
   /**  at address offset 0x310, read-write */
   uint32_t pd_app1_ctrl_power_mode4;
   /**  at address offset 0x314, read-write */
   uint32_t pd_app1_ctrl_power_mode5;
   /**  at address offset 0x318, read-write */
   uint32_t pd_app1_ctrl_power_mode6;
   /**  at address offset 0x31C, read-write */
   uint32_t pd_app1_ctrl_power_mode7;
   /**  at address offset 0x320, read-only */
   uint32_t pd_app1_ring_signature;
   /**  at address offset 0x324, read-only */
   uint32_t pd_app1_ring_status;
   /**  at address offset 0x328, read-write */
   uint32_t pd_app1_ring_control;
   /**  at address offset 0x32C, read-write */
   uint32_t pd_app1_ring_debug;
   /** Reserved space */
   uint8_t fill9[80];
   /**  at address offset 0x380, read-write */
   uint32_t pd_modem0_ctrl_power_mode0;
   /**  at address offset 0x384, read-write */
   uint32_t pd_modem0_ctrl_power_mode1;
   /**  at address offset 0x388, read-write */
   uint32_t pd_modem0_ctrl_power_mode2;
   /**  at address offset 0x38C, read-write */
   uint32_t pd_modem0_ctrl_power_mode3;
   /**  at address offset 0x390, read-write */
   uint32_t pd_modem0_ctrl_power_mode4;
   /**  at address offset 0x394, read-write */
   uint32_t pd_modem0_ctrl_power_mode5;
   /**  at address offset 0x398, read-write */
   uint32_t pd_modem0_ctrl_power_mode6;
   /**  at address offset 0x39C, read-write */
   uint32_t pd_modem0_ctrl_power_mode7;
   /**  at address offset 0x3A0, read-only */
   uint32_t pd_modem0_ring_signature;
   /**  at address offset 0x3A4, read-only */
   uint32_t pd_modem0_ring_status;
   /**  at address offset 0x3A8, read-write */
   uint32_t pd_modem0_ring_control;
   /**  at address offset 0x3AC, read-write */
   uint32_t pd_modem0_ring_debug;
   /** Reserved space */
   uint8_t fill10[80];
   /**  at address offset 0x400, read-write */
   uint32_t pd_modem1_ctrl_power_mode0;
   /**  at address offset 0x404, read-write */
   uint32_t pd_modem1_ctrl_power_mode1;
   /**  at address offset 0x408, read-write */
   uint32_t pd_modem1_ctrl_power_mode2;
   /**  at address offset 0x40C, read-write */
   uint32_t pd_modem1_ctrl_power_mode3;
   /**  at address offset 0x410, read-write */
   uint32_t pd_modem1_ctrl_power_mode4;
   /**  at address offset 0x414, read-write */
   uint32_t pd_modem1_ctrl_power_mode5;
   /**  at address offset 0x418, read-write */
   uint32_t pd_modem1_ctrl_power_mode6;
   /**  at address offset 0x41C, read-write */
   uint32_t pd_modem1_ctrl_power_mode7;
   /**  at address offset 0x420, read-only */
   uint32_t pd_modem1_ring_signature;
   /**  at address offset 0x424, read-only */
   uint32_t pd_modem1_ring_status;
   /**  at address offset 0x428, read-write */
   uint32_t pd_modem1_ring_control;
   /**  at address offset 0x42C, read-write */
   uint32_t pd_modem1_ring_debug;
   /** Reserved space */
   uint8_t fill11[80];
   /**  at address offset 0x480, read-write */
   uint32_t pd_modem2_ctrl_power_mode0;
   /**  at address offset 0x484, read-write */
   uint32_t pd_modem2_ctrl_power_mode1;
   /**  at address offset 0x488, read-write */
   uint32_t pd_modem2_ctrl_power_mode2;
   /**  at address offset 0x48C, read-write */
   uint32_t pd_modem2_ctrl_power_mode3;
   /**  at address offset 0x490, read-write */
   uint32_t pd_modem2_ctrl_power_mode4;
   /**  at address offset 0x494, read-write */
   uint32_t pd_modem2_ctrl_power_mode5;
   /**  at address offset 0x498, read-write */
   uint32_t pd_modem2_ctrl_power_mode6;
   /**  at address offset 0x49C, read-write */
   uint32_t pd_modem2_ctrl_power_mode7;
   /**  at address offset 0x4A0, read-only */
   uint32_t pd_modem2_ring_signature;
   /**  at address offset 0x4A4, read-only */
   uint32_t pd_modem2_ring_status;
   /**  at address offset 0x4A8, read-write */
   uint32_t pd_modem2_ring_control;
   /**  at address offset 0x4AC, read-write */
   uint32_t pd_modem2_ring_debug;
   /** Reserved space */
   uint8_t fill12[80];
   /**  at address offset 0x500, read-write */
   uint32_t pd_modem3_ctrl_power_mode0;
   /**  at address offset 0x504, read-write */
   uint32_t pd_modem3_ctrl_power_mode1;
   /**  at address offset 0x508, read-write */
   uint32_t pd_modem3_ctrl_power_mode2;
   /**  at address offset 0x50C, read-write */
   uint32_t pd_modem3_ctrl_power_mode3;
   /**  at address offset 0x510, read-write */
   uint32_t pd_modem3_ctrl_power_mode4;
   /**  at address offset 0x514, read-write */
   uint32_t pd_modem3_ctrl_power_mode5;
   /**  at address offset 0x518, read-write */
   uint32_t pd_modem3_ctrl_power_mode6;
   /**  at address offset 0x51C, read-write */
   uint32_t pd_modem3_ctrl_power_mode7;
   /**  at address offset 0x520, read-only */
   uint32_t pd_modem3_ring_signature;
   /**  at address offset 0x524, read-only */
   uint32_t pd_modem3_ring_status;
   /**  at address offset 0x528, read-write */
   uint32_t pd_modem3_ring_control;
   /**  at address offset 0x52C, read-write */
   uint32_t pd_modem3_ring_debug;
};

/** bit field defines for km_leti_cvp_s#itc_it_flags */
#define KM_LETI_CVP_ITC_IT_FLAGS_IT_LINES_OFFSET 0
#define KM_LETI_CVP_ITC_IT_FLAGS_IT_LINES_SIZE 32

/** bit field defines for km_leti_cvp_s#itc_it_debug */
#define KM_LETI_CVP_ITC_IT_DEBUG_IT_DEBUG_OFFSET 0
#define KM_LETI_CVP_ITC_IT_DEBUG_IT_DEBUG_SIZE 32

/** bit field defines for km_leti_cvp_s#itc_it0_mask */
#define KM_LETI_CVP_ITC_IT0_MASK_IT0_MASK_OFFSET 0
#define KM_LETI_CVP_ITC_IT0_MASK_IT0_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#itc_it0_pending */
#define KM_LETI_CVP_ITC_IT0_PENDING_IT0_PENDING_OFFSET 0
#define KM_LETI_CVP_ITC_IT0_PENDING_IT0_PENDING_SIZE 32

/** bit field defines for km_leti_cvp_s#itc_it0_clear_pending */
#define KM_LETI_CVP_ITC_IT0_CLEAR_PENDING_IT0_PENDING_CLEAR_OFFSET 0
#define KM_LETI_CVP_ITC_IT0_CLEAR_PENDING_IT0_PENDING_CLEAR_SIZE 32

/** bit field defines for km_leti_cvp_s#itc_it1_mask */
#define KM_LETI_CVP_ITC_IT1_MASK_IT1_MASK_OFFSET 0
#define KM_LETI_CVP_ITC_IT1_MASK_IT1_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#itc_it1_pending */
#define KM_LETI_CVP_ITC_IT1_PENDING_IT1_PENDING_OFFSET 0
#define KM_LETI_CVP_ITC_IT1_PENDING_IT1_PENDING_SIZE 32

/** bit field defines for km_leti_cvp_s#itc_it1_clear_pending */
#define KM_LETI_CVP_ITC_IT1_CLEAR_PENDING_IT1_PENDING_CLEAR_OFFSET 0
#define KM_LETI_CVP_ITC_IT1_CLEAR_PENDING_IT1_PENDING_CLEAR_SIZE 32

/** bit field defines for km_leti_cvp_s#itc_it_clk_safe_mask */
#define KM_LETI_CVP_ITC_IT_CLK_SAFE_MASK_IT_CLOCK_SAFE_MASK_OFFSET 0
#define KM_LETI_CVP_ITC_IT_CLK_SAFE_MASK_IT_CLOCK_SAFE_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#itc_it_clk_safe_pending */
#define KM_LETI_CVP_ITC_IT_CLK_SAFE_PENDING_IT_CLOCK_SAFE_PENDING_OFFSET 0
#define KM_LETI_CVP_ITC_IT_CLK_SAFE_PENDING_IT_CLOCK_SAFE_PENDING_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app0_tmflt_status0 */
#define KM_LETI_CVP_PD_APP0_TMFLT_STATUS0_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_APP0_TMFLT_STATUS0_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app0_tmflt_status1 */
#define KM_LETI_CVP_PD_APP0_TMFLT_STATUS1_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_APP0_TMFLT_STATUS1_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app0_tmflt_status2 */
#define KM_LETI_CVP_PD_APP0_TMFLT_STATUS2_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_APP0_TMFLT_STATUS2_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app0_tmflt_status3 */
#define KM_LETI_CVP_PD_APP0_TMFLT_STATUS3_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_APP0_TMFLT_STATUS3_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app0_tmflt_warning_mask0 */
#define KM_LETI_CVP_PD_APP0_TMFLT_WARNING_MASK0_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_APP0_TMFLT_WARNING_MASK0_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app0_tmflt_warning_mask1 */
#define KM_LETI_CVP_PD_APP0_TMFLT_WARNING_MASK1_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_APP0_TMFLT_WARNING_MASK1_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app0_tmflt_warning_mask2 */
#define KM_LETI_CVP_PD_APP0_TMFLT_WARNING_MASK2_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_APP0_TMFLT_WARNING_MASK2_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app0_tmflt_warning_mask3 */
#define KM_LETI_CVP_PD_APP0_TMFLT_WARNING_MASK3_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_APP0_TMFLT_WARNING_MASK3_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app0_tmflt_control */
#define KM_LETI_CVP_PD_APP0_TMFLT_CONTROL_RESET_TMFLT_OFFSET 0
#define KM_LETI_CVP_PD_APP0_TMFLT_CONTROL_RESET_TMFLT_SIZE 1
#define KM_LETI_CVP_PD_APP0_TMFLT_CONTROL_USE_AUTO_OFFSET 1
#define KM_LETI_CVP_PD_APP0_TMFLT_CONTROL_USE_AUTO_SIZE 1
#define KM_LETI_CVP_PD_APP0_TMFLT_CONTROL_IT_WARNING_STATUS_OFFSET 16
#define KM_LETI_CVP_PD_APP0_TMFLT_CONTROL_IT_WARNING_STATUS_SIZE 1
#define KM_LETI_CVP_PD_APP0_TMFLT_CONTROL_IT_ERROR_STATUS_OFFSET 17
#define KM_LETI_CVP_PD_APP0_TMFLT_CONTROL_IT_ERROR_STATUS_SIZE 1
#define KM_LETI_CVP_PD_APP0_TMFLT_CONTROL_IT_AUTO_STATUS_OFFSET 18
#define KM_LETI_CVP_PD_APP0_TMFLT_CONTROL_IT_AUTO_STATUS_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_app1_tmflt_status0 */
#define KM_LETI_CVP_PD_APP1_TMFLT_STATUS0_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_APP1_TMFLT_STATUS0_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app1_tmflt_status1 */
#define KM_LETI_CVP_PD_APP1_TMFLT_STATUS1_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_APP1_TMFLT_STATUS1_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app1_tmflt_status2 */
#define KM_LETI_CVP_PD_APP1_TMFLT_STATUS2_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_APP1_TMFLT_STATUS2_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app1_tmflt_status3 */
#define KM_LETI_CVP_PD_APP1_TMFLT_STATUS3_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_APP1_TMFLT_STATUS3_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app1_tmflt_warning_mask0 */
#define KM_LETI_CVP_PD_APP1_TMFLT_WARNING_MASK0_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_APP1_TMFLT_WARNING_MASK0_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app1_tmflt_warning_mask1 */
#define KM_LETI_CVP_PD_APP1_TMFLT_WARNING_MASK1_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_APP1_TMFLT_WARNING_MASK1_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app1_tmflt_warning_mask2 */
#define KM_LETI_CVP_PD_APP1_TMFLT_WARNING_MASK2_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_APP1_TMFLT_WARNING_MASK2_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app1_tmflt_warning_mask3 */
#define KM_LETI_CVP_PD_APP1_TMFLT_WARNING_MASK3_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_APP1_TMFLT_WARNING_MASK3_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app1_tmflt_control */
#define KM_LETI_CVP_PD_APP1_TMFLT_CONTROL_RESET_TMFLT_OFFSET 0
#define KM_LETI_CVP_PD_APP1_TMFLT_CONTROL_RESET_TMFLT_SIZE 1
#define KM_LETI_CVP_PD_APP1_TMFLT_CONTROL_USE_AUTO_OFFSET 1
#define KM_LETI_CVP_PD_APP1_TMFLT_CONTROL_USE_AUTO_SIZE 1
#define KM_LETI_CVP_PD_APP1_TMFLT_CONTROL_IT_WARNING_STATUS_OFFSET 16
#define KM_LETI_CVP_PD_APP1_TMFLT_CONTROL_IT_WARNING_STATUS_SIZE 1
#define KM_LETI_CVP_PD_APP1_TMFLT_CONTROL_IT_ERROR_STATUS_OFFSET 17
#define KM_LETI_CVP_PD_APP1_TMFLT_CONTROL_IT_ERROR_STATUS_SIZE 1
#define KM_LETI_CVP_PD_APP1_TMFLT_CONTROL_IT_AUTO_STATUS_OFFSET 18
#define KM_LETI_CVP_PD_APP1_TMFLT_CONTROL_IT_AUTO_STATUS_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem0_tmflt_status0 */
#define KM_LETI_CVP_PD_MODEM0_TMFLT_STATUS0_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_TMFLT_STATUS0_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem0_tmflt_status1 */
#define KM_LETI_CVP_PD_MODEM0_TMFLT_STATUS1_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_TMFLT_STATUS1_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem0_tmflt_status2 */
#define KM_LETI_CVP_PD_MODEM0_TMFLT_STATUS2_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_TMFLT_STATUS2_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem0_tmflt_status3 */
#define KM_LETI_CVP_PD_MODEM0_TMFLT_STATUS3_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_TMFLT_STATUS3_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem0_tmflt_warning_mask0 */
#define KM_LETI_CVP_PD_MODEM0_TMFLT_WARNING_MASK0_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_TMFLT_WARNING_MASK0_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem0_tmflt_warning_mask1 */
#define KM_LETI_CVP_PD_MODEM0_TMFLT_WARNING_MASK1_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_TMFLT_WARNING_MASK1_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem0_tmflt_warning_mask2 */
#define KM_LETI_CVP_PD_MODEM0_TMFLT_WARNING_MASK2_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_TMFLT_WARNING_MASK2_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem0_tmflt_warning_mask3 */
#define KM_LETI_CVP_PD_MODEM0_TMFLT_WARNING_MASK3_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_TMFLT_WARNING_MASK3_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem0_tmflt_control */
#define KM_LETI_CVP_PD_MODEM0_TMFLT_CONTROL_RESET_TMFLT_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_TMFLT_CONTROL_RESET_TMFLT_SIZE 1
#define KM_LETI_CVP_PD_MODEM0_TMFLT_CONTROL_USE_AUTO_OFFSET 1
#define KM_LETI_CVP_PD_MODEM0_TMFLT_CONTROL_USE_AUTO_SIZE 1
#define KM_LETI_CVP_PD_MODEM0_TMFLT_CONTROL_IT_WARNING_STATUS_OFFSET 16
#define KM_LETI_CVP_PD_MODEM0_TMFLT_CONTROL_IT_WARNING_STATUS_SIZE 1
#define KM_LETI_CVP_PD_MODEM0_TMFLT_CONTROL_IT_ERROR_STATUS_OFFSET 17
#define KM_LETI_CVP_PD_MODEM0_TMFLT_CONTROL_IT_ERROR_STATUS_SIZE 1
#define KM_LETI_CVP_PD_MODEM0_TMFLT_CONTROL_IT_AUTO_STATUS_OFFSET 18
#define KM_LETI_CVP_PD_MODEM0_TMFLT_CONTROL_IT_AUTO_STATUS_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem1_tmflt_status0 */
#define KM_LETI_CVP_PD_MODEM1_TMFLT_STATUS0_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_TMFLT_STATUS0_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem1_tmflt_status1 */
#define KM_LETI_CVP_PD_MODEM1_TMFLT_STATUS1_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_TMFLT_STATUS1_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem1_tmflt_status2 */
#define KM_LETI_CVP_PD_MODEM1_TMFLT_STATUS2_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_TMFLT_STATUS2_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem1_tmflt_status3 */
#define KM_LETI_CVP_PD_MODEM1_TMFLT_STATUS3_SENSOR_STATUS_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_TMFLT_STATUS3_SENSOR_STATUS_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem1_tmflt_warning_mask0 */
#define KM_LETI_CVP_PD_MODEM1_TMFLT_WARNING_MASK0_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_TMFLT_WARNING_MASK0_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem1_tmflt_warning_mask1 */
#define KM_LETI_CVP_PD_MODEM1_TMFLT_WARNING_MASK1_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_TMFLT_WARNING_MASK1_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem1_tmflt_warning_mask2 */
#define KM_LETI_CVP_PD_MODEM1_TMFLT_WARNING_MASK2_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_TMFLT_WARNING_MASK2_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem1_tmflt_warning_mask3 */
#define KM_LETI_CVP_PD_MODEM1_TMFLT_WARNING_MASK3_WARNING_MASK_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_TMFLT_WARNING_MASK3_WARNING_MASK_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem1_tmflt_control */
#define KM_LETI_CVP_PD_MODEM1_TMFLT_CONTROL_RESET_TMFLT_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_TMFLT_CONTROL_RESET_TMFLT_SIZE 1
#define KM_LETI_CVP_PD_MODEM1_TMFLT_CONTROL_USE_AUTO_OFFSET 1
#define KM_LETI_CVP_PD_MODEM1_TMFLT_CONTROL_USE_AUTO_SIZE 1
#define KM_LETI_CVP_PD_MODEM1_TMFLT_CONTROL_IT_WARNING_STATUS_OFFSET 16
#define KM_LETI_CVP_PD_MODEM1_TMFLT_CONTROL_IT_WARNING_STATUS_SIZE 1
#define KM_LETI_CVP_PD_MODEM1_TMFLT_CONTROL_IT_ERROR_STATUS_OFFSET 17
#define KM_LETI_CVP_PD_MODEM1_TMFLT_CONTROL_IT_ERROR_STATUS_SIZE 1
#define KM_LETI_CVP_PD_MODEM1_TMFLT_CONTROL_IT_AUTO_STATUS_OFFSET 18
#define KM_LETI_CVP_PD_MODEM1_TMFLT_CONTROL_IT_AUTO_STATUS_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_app0_ctrl_power_mode0 */
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE0_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE0_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE0_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE0_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE0_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE0_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app0_ctrl_power_mode1 */
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE1_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE1_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE1_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE1_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE1_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE1_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app0_ctrl_power_mode2 */
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE2_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE2_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE2_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE2_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE2_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE2_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app0_ctrl_power_mode3 */
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE3_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE3_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE3_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE3_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE3_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE3_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app0_ctrl_power_mode4 */
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE4_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE4_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE4_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE4_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE4_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE4_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app0_ctrl_power_mode5 */
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE5_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE5_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE5_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE5_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE5_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE5_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app0_ctrl_power_mode6 */
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE6_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE6_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE6_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE6_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE6_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE6_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app0_ctrl_power_mode7 */
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE7_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE7_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE7_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE7_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE7_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP0_CTRL_POWER_MODE7_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app0_ring_signature */
#define KM_LETI_CVP_PD_APP0_RING_SIGNATURE_RING_SIGNATURE_OFFSET 0
#define KM_LETI_CVP_PD_APP0_RING_SIGNATURE_RING_SIGNATURE_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app0_ring_status */
#define KM_LETI_CVP_PD_APP0_RING_STATUS_LOWEST_RING_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_APP0_RING_STATUS_LOWEST_RING_VALUE_SIZE 5
#define KM_LETI_CVP_PD_APP0_RING_STATUS_IT_ERROR_STATUS_OFFSET 24
#define KM_LETI_CVP_PD_APP0_RING_STATUS_IT_ERROR_STATUS_SIZE 1
#define KM_LETI_CVP_PD_APP0_RING_STATUS_IT_WARNING_STATUS_OFFSET 25
#define KM_LETI_CVP_PD_APP0_RING_STATUS_IT_WARNING_STATUS_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_app0_ring_control */
#define KM_LETI_CVP_PD_APP0_RING_CONTROL_ENABLE_RING_OFFSET 16
#define KM_LETI_CVP_PD_APP0_RING_CONTROL_ENABLE_RING_SIZE 1
#define KM_LETI_CVP_PD_APP0_RING_CONTROL_CLEAR_LOWEST_VALUE_OFFSET 24
#define KM_LETI_CVP_PD_APP0_RING_CONTROL_CLEAR_LOWEST_VALUE_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_app0_ring_debug */
#define KM_LETI_CVP_PD_APP0_RING_DEBUG_FORCE_POWER_MODE_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_APP0_RING_DEBUG_FORCE_POWER_MODE_VALUE_SIZE 3
#define KM_LETI_CVP_PD_APP0_RING_DEBUG_FORCE_POWER_MODE_OFFSET 3
#define KM_LETI_CVP_PD_APP0_RING_DEBUG_FORCE_POWER_MODE_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_app1_ctrl_power_mode0 */
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE0_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE0_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE0_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE0_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE0_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE0_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app1_ctrl_power_mode1 */
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE1_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE1_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE1_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE1_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE1_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE1_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app1_ctrl_power_mode2 */
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE2_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE2_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE2_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE2_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE2_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE2_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app1_ctrl_power_mode3 */
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE3_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE3_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE3_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE3_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE3_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE3_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app1_ctrl_power_mode4 */
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE4_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE4_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE4_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE4_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE4_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE4_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app1_ctrl_power_mode5 */
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE5_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE5_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE5_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE5_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE5_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE5_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app1_ctrl_power_mode6 */
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE6_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE6_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE6_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE6_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE6_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE6_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app1_ctrl_power_mode7 */
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE7_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE7_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE7_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE7_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE7_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_APP1_CTRL_POWER_MODE7_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_app1_ring_signature */
#define KM_LETI_CVP_PD_APP1_RING_SIGNATURE_RING_SIGNATURE_OFFSET 0
#define KM_LETI_CVP_PD_APP1_RING_SIGNATURE_RING_SIGNATURE_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_app1_ring_status */
#define KM_LETI_CVP_PD_APP1_RING_STATUS_LOWEST_RING_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_APP1_RING_STATUS_LOWEST_RING_VALUE_SIZE 5
#define KM_LETI_CVP_PD_APP1_RING_STATUS_IT_ERROR_STATUS_OFFSET 24
#define KM_LETI_CVP_PD_APP1_RING_STATUS_IT_ERROR_STATUS_SIZE 1
#define KM_LETI_CVP_PD_APP1_RING_STATUS_IT_WARNING_STATUS_OFFSET 25
#define KM_LETI_CVP_PD_APP1_RING_STATUS_IT_WARNING_STATUS_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_app1_ring_control */
#define KM_LETI_CVP_PD_APP1_RING_CONTROL_ENABLE_RING_OFFSET 16
#define KM_LETI_CVP_PD_APP1_RING_CONTROL_ENABLE_RING_SIZE 1
#define KM_LETI_CVP_PD_APP1_RING_CONTROL_CLEAR_LOWEST_VALUE_OFFSET 24
#define KM_LETI_CVP_PD_APP1_RING_CONTROL_CLEAR_LOWEST_VALUE_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_app1_ring_debug */
#define KM_LETI_CVP_PD_APP1_RING_DEBUG_FORCE_POWER_MODE_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_APP1_RING_DEBUG_FORCE_POWER_MODE_VALUE_SIZE 3
#define KM_LETI_CVP_PD_APP1_RING_DEBUG_FORCE_POWER_MODE_OFFSET 3
#define KM_LETI_CVP_PD_APP1_RING_DEBUG_FORCE_POWER_MODE_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem0_ctrl_power_mode0 */
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE0_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE0_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE0_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE0_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE0_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE0_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem0_ctrl_power_mode1 */
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE1_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE1_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE1_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE1_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE1_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE1_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem0_ctrl_power_mode2 */
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE2_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE2_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE2_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE2_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE2_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE2_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem0_ctrl_power_mode3 */
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE3_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE3_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE3_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE3_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE3_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE3_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem0_ctrl_power_mode4 */
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE4_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE4_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE4_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE4_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE4_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE4_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem0_ctrl_power_mode5 */
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE5_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE5_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE5_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE5_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE5_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE5_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem0_ctrl_power_mode6 */
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE6_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE6_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE6_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE6_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE6_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE6_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem0_ctrl_power_mode7 */
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE7_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE7_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE7_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE7_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE7_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM0_CTRL_POWER_MODE7_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem0_ring_signature */
#define KM_LETI_CVP_PD_MODEM0_RING_SIGNATURE_RING_SIGNATURE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_RING_SIGNATURE_RING_SIGNATURE_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem0_ring_status */
#define KM_LETI_CVP_PD_MODEM0_RING_STATUS_LOWEST_RING_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_RING_STATUS_LOWEST_RING_VALUE_SIZE 5
#define KM_LETI_CVP_PD_MODEM0_RING_STATUS_IT_ERROR_STATUS_OFFSET 24
#define KM_LETI_CVP_PD_MODEM0_RING_STATUS_IT_ERROR_STATUS_SIZE 1
#define KM_LETI_CVP_PD_MODEM0_RING_STATUS_IT_WARNING_STATUS_OFFSET 25
#define KM_LETI_CVP_PD_MODEM0_RING_STATUS_IT_WARNING_STATUS_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem0_ring_control */
#define KM_LETI_CVP_PD_MODEM0_RING_CONTROL_ENABLE_RING_OFFSET 16
#define KM_LETI_CVP_PD_MODEM0_RING_CONTROL_ENABLE_RING_SIZE 1
#define KM_LETI_CVP_PD_MODEM0_RING_CONTROL_CLEAR_LOWEST_VALUE_OFFSET 24
#define KM_LETI_CVP_PD_MODEM0_RING_CONTROL_CLEAR_LOWEST_VALUE_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem0_ring_debug */
#define KM_LETI_CVP_PD_MODEM0_RING_DEBUG_FORCE_POWER_MODE_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM0_RING_DEBUG_FORCE_POWER_MODE_VALUE_SIZE 3
#define KM_LETI_CVP_PD_MODEM0_RING_DEBUG_FORCE_POWER_MODE_OFFSET 3
#define KM_LETI_CVP_PD_MODEM0_RING_DEBUG_FORCE_POWER_MODE_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem1_ctrl_power_mode0 */
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE0_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE0_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE0_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE0_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE0_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE0_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem1_ctrl_power_mode1 */
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE1_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE1_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE1_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE1_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE1_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE1_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem1_ctrl_power_mode2 */
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE2_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE2_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE2_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE2_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE2_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE2_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem1_ctrl_power_mode3 */
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE3_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE3_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE3_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE3_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE3_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE3_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem1_ctrl_power_mode4 */
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE4_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE4_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE4_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE4_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE4_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE4_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem1_ctrl_power_mode5 */
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE5_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE5_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE5_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE5_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE5_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE5_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem1_ctrl_power_mode6 */
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE6_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE6_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE6_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE6_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE6_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE6_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem1_ctrl_power_mode7 */
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE7_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE7_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE7_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE7_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE7_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM1_CTRL_POWER_MODE7_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem1_ring_signature */
#define KM_LETI_CVP_PD_MODEM1_RING_SIGNATURE_RING_SIGNATURE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_RING_SIGNATURE_RING_SIGNATURE_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem1_ring_status */
#define KM_LETI_CVP_PD_MODEM1_RING_STATUS_LOWEST_RING_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_RING_STATUS_LOWEST_RING_VALUE_SIZE 5
#define KM_LETI_CVP_PD_MODEM1_RING_STATUS_IT_ERROR_STATUS_OFFSET 24
#define KM_LETI_CVP_PD_MODEM1_RING_STATUS_IT_ERROR_STATUS_SIZE 1
#define KM_LETI_CVP_PD_MODEM1_RING_STATUS_IT_WARNING_STATUS_OFFSET 25
#define KM_LETI_CVP_PD_MODEM1_RING_STATUS_IT_WARNING_STATUS_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem1_ring_control */
#define KM_LETI_CVP_PD_MODEM1_RING_CONTROL_ENABLE_RING_OFFSET 16
#define KM_LETI_CVP_PD_MODEM1_RING_CONTROL_ENABLE_RING_SIZE 1
#define KM_LETI_CVP_PD_MODEM1_RING_CONTROL_CLEAR_LOWEST_VALUE_OFFSET 24
#define KM_LETI_CVP_PD_MODEM1_RING_CONTROL_CLEAR_LOWEST_VALUE_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem1_ring_debug */
#define KM_LETI_CVP_PD_MODEM1_RING_DEBUG_FORCE_POWER_MODE_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM1_RING_DEBUG_FORCE_POWER_MODE_VALUE_SIZE 3
#define KM_LETI_CVP_PD_MODEM1_RING_DEBUG_FORCE_POWER_MODE_OFFSET 3
#define KM_LETI_CVP_PD_MODEM1_RING_DEBUG_FORCE_POWER_MODE_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem2_ctrl_power_mode0 */
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE0_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE0_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE0_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE0_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE0_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE0_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem2_ctrl_power_mode1 */
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE1_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE1_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE1_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE1_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE1_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE1_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem2_ctrl_power_mode2 */
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE2_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE2_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE2_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE2_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE2_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE2_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem2_ctrl_power_mode3 */
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE3_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE3_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE3_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE3_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE3_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE3_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem2_ctrl_power_mode4 */
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE4_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE4_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE4_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE4_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE4_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE4_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem2_ctrl_power_mode5 */
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE5_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE5_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE5_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE5_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE5_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE5_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem2_ctrl_power_mode6 */
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE6_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE6_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE6_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE6_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE6_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE6_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem2_ctrl_power_mode7 */
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE7_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE7_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE7_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE7_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE7_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM2_CTRL_POWER_MODE7_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem2_ring_signature */
#define KM_LETI_CVP_PD_MODEM2_RING_SIGNATURE_RING_SIGNATURE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM2_RING_SIGNATURE_RING_SIGNATURE_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem2_ring_status */
#define KM_LETI_CVP_PD_MODEM2_RING_STATUS_LOWEST_RING_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM2_RING_STATUS_LOWEST_RING_VALUE_SIZE 5
#define KM_LETI_CVP_PD_MODEM2_RING_STATUS_IT_ERROR_STATUS_OFFSET 24
#define KM_LETI_CVP_PD_MODEM2_RING_STATUS_IT_ERROR_STATUS_SIZE 1
#define KM_LETI_CVP_PD_MODEM2_RING_STATUS_IT_WARNING_STATUS_OFFSET 25
#define KM_LETI_CVP_PD_MODEM2_RING_STATUS_IT_WARNING_STATUS_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem2_ring_control */
#define KM_LETI_CVP_PD_MODEM2_RING_CONTROL_ENABLE_RING_OFFSET 16
#define KM_LETI_CVP_PD_MODEM2_RING_CONTROL_ENABLE_RING_SIZE 1
#define KM_LETI_CVP_PD_MODEM2_RING_CONTROL_CLEAR_LOWEST_VALUE_OFFSET 24
#define KM_LETI_CVP_PD_MODEM2_RING_CONTROL_CLEAR_LOWEST_VALUE_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem2_ring_debug */
#define KM_LETI_CVP_PD_MODEM2_RING_DEBUG_FORCE_POWER_MODE_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM2_RING_DEBUG_FORCE_POWER_MODE_VALUE_SIZE 3
#define KM_LETI_CVP_PD_MODEM2_RING_DEBUG_FORCE_POWER_MODE_OFFSET 3
#define KM_LETI_CVP_PD_MODEM2_RING_DEBUG_FORCE_POWER_MODE_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem3_ctrl_power_mode0 */
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE0_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE0_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE0_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE0_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE0_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE0_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem3_ctrl_power_mode1 */
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE1_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE1_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE1_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE1_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE1_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE1_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem3_ctrl_power_mode2 */
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE2_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE2_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE2_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE2_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE2_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE2_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem3_ctrl_power_mode3 */
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE3_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE3_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE3_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE3_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE3_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE3_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem3_ctrl_power_mode4 */
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE4_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE4_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE4_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE4_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE4_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE4_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem3_ctrl_power_mode5 */
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE5_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE5_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE5_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE5_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE5_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE5_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem3_ctrl_power_mode6 */
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE6_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE6_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE6_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE6_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE6_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE6_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem3_ctrl_power_mode7 */
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE7_COARSE_SELECTOR_OFFSET 0
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE7_COARSE_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE7_WARNING_SELECTOR_OFFSET 8
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE7_WARNING_SELECTOR_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE7_ERROR_SELECTOR_OFFSET 16
#define KM_LETI_CVP_PD_MODEM3_CTRL_POWER_MODE7_ERROR_SELECTOR_SIZE 5

/** bit field defines for km_leti_cvp_s#pd_modem3_ring_signature */
#define KM_LETI_CVP_PD_MODEM3_RING_SIGNATURE_RING_SIGNATURE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM3_RING_SIGNATURE_RING_SIGNATURE_SIZE 32

/** bit field defines for km_leti_cvp_s#pd_modem3_ring_status */
#define KM_LETI_CVP_PD_MODEM3_RING_STATUS_LOWEST_RING_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM3_RING_STATUS_LOWEST_RING_VALUE_SIZE 5
#define KM_LETI_CVP_PD_MODEM3_RING_STATUS_IT_ERROR_STATUS_OFFSET 24
#define KM_LETI_CVP_PD_MODEM3_RING_STATUS_IT_ERROR_STATUS_SIZE 1
#define KM_LETI_CVP_PD_MODEM3_RING_STATUS_IT_WARNING_STATUS_OFFSET 25
#define KM_LETI_CVP_PD_MODEM3_RING_STATUS_IT_WARNING_STATUS_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem3_ring_control */
#define KM_LETI_CVP_PD_MODEM3_RING_CONTROL_ENABLE_RING_OFFSET 16
#define KM_LETI_CVP_PD_MODEM3_RING_CONTROL_ENABLE_RING_SIZE 1
#define KM_LETI_CVP_PD_MODEM3_RING_CONTROL_CLEAR_LOWEST_VALUE_OFFSET 24
#define KM_LETI_CVP_PD_MODEM3_RING_CONTROL_CLEAR_LOWEST_VALUE_SIZE 1

/** bit field defines for km_leti_cvp_s#pd_modem3_ring_debug */
#define KM_LETI_CVP_PD_MODEM3_RING_DEBUG_FORCE_POWER_MODE_VALUE_OFFSET 0
#define KM_LETI_CVP_PD_MODEM3_RING_DEBUG_FORCE_POWER_MODE_VALUE_SIZE 3
#define KM_LETI_CVP_PD_MODEM3_RING_DEBUG_FORCE_POWER_MODE_OFFSET 3
#define KM_LETI_CVP_PD_MODEM3_RING_DEBUG_FORCE_POWER_MODE_SIZE 1

/* EOF km_leti_cvp.h */
#endif
