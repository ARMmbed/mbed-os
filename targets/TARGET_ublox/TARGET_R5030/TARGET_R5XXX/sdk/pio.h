#ifndef INCLUDED_PIO
#define INCLUDED_PIO
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** pio HAL Spreadsheet version number */
#define PIO_HAL_VERSION 4

/** Peripheral IO Controller Module
*/
struct pio_s {
   /** PIO Enable Register at address offset 0x000, write-only */
   uint32_t pio_per_0;
   /** PIO Disable Register at address offset 0x004, write-only */
   uint32_t pio_pdr_0;
   /** PIO Status Register at address offset 0x008, read-only */
   uint32_t pio_psr_0;
   /** Reserved space */
   uint8_t fill0[4];
   /** Output Enable Register at address offset 0x010, write-only */
   uint32_t pio_oer_0;
   /** Output Disable Register at address offset 0x014, write-only */
   uint32_t pio_odr_0;
   /** Output Status Register at address offset 0x018, read-only */
   uint32_t pio_osr_0;
   /** Output Direction Register (negated) at address offset 0x01C, read-only */
   uint32_t pio_posr_0;
   /** Input Filter Enable Register at address offset 0x020, write-only */
   uint32_t pio_ifer_0;
   /** Input Filter Disable Register at address offset 0x024, write-only */
   uint32_t pio_ifdr_0;
   /** Input Filter Status Register (selector 1) at address offset 0x028, read-only */
   uint32_t pio_ifsr_0;
   /** Reserved space */
   uint8_t fill1[4];
   /** Set Output Data Register at address offset 0x030, write-only */
   uint32_t pio_sodr_0;
   /** Clear Output Data Register at address offset 0x034, write-only */
   uint32_t pio_codr_0;
   /** Output Data Status Register at address offset 0x038, read-write */
   uint32_t pio_odsr_0;
   /** Pin Data Status Register at address offset 0x03C, read-only */
   uint32_t pio_pdsr_0;
   /** 1st Interrupt Enable Register at address offset 0x040, write-only */
   uint32_t pio_ier1_0;
   /** 1st Interrupt Disable Register at address offset 0x044, write-only */
   uint32_t pio_idr1_0;
   /** 1st Interrupt Mask Register at address offset 0x048, read-only */
   uint32_t pio_imr1_0;
   /** Reserved space */
   uint8_t fill2[4];
   /** 2nd Interrupt Enable Register at address offset 0x050, write-only */
   uint32_t pio_ier2_0;
   /** 2nd Interrupt Disable Register at address offset 0x054, write-only */
   uint32_t pio_idr2_0;
   /** 2nd Interrupt Mask Register at address offset 0x058, read-only */
   uint32_t pio_imr2_0;
   /** Interrupt Status Register at address offset 0x05C, read-write */
   uint32_t pio_isr_0;
   /** Multi-Driver Enable Register at address offset 0x060, write-only */
   uint32_t pio_mder_0;
   /** Multi-Driver Disable Register at address offset 0x064, write-only */
   uint32_t pio_mddr_0;
   /** Multi-Driver Status Register at address offset 0x068, read-only */
   uint32_t pio_mdsr_0;
   /** Reserved space */
   uint8_t fill3[4];
   /** PIO check in Register at address offset 0x070, read-only */
   uint32_t pio_ckin_0;
   /** PIO general purpose Register at address offset 0x074, read-write */
   uint32_t pio_gpsr_0;
   /** Select A Register at address offset 0x078, write-only */
   uint32_t pio_asr_0;
   /** Select B Register at address offset 0x07C, write-only */
   uint32_t pio_bsr_0;
   /** Select C Register at address offset 0x080, write-only */
   uint32_t pio_csr_0;
   /** Select D Register at address offset 0x084, write-only */
   uint32_t pio_dsr_0;
   /** MSB Select Status Register at address offset 0x088, read-only */
   uint32_t pio_mselsr_0;
   /** LSB Select Status Register at address offset 0x08C, read-only */
   uint32_t pio_lselsr_0;
   /** MSB Drive Strength Enable Register at address offset 0x090, write-only */
   uint32_t pio_mdser_0;
   /** MSB Drive Strength Disable Register at address offset 0x094, write-only */
   uint32_t pio_mdsdr_0;
   /** MSB Drive Strength Status Register at address offset 0x098, read-only */
   uint32_t pio_mdssr_0;
   /** Reserved space */
   uint8_t fill4[4];
   /** LSB Drive Strength Enable Register at address offset 0x0A0, write-only */
   uint32_t pio_ldser_0;
   /** LSB Drive Strength Disable Register at address offset 0x0A4, write-only */
   uint32_t pio_ldsdr_0;
   /** LSB Drive Strength Status Register at address offset 0x0A8, read-only */
   uint32_t pio_ldssr_0;
   /** Reserved space */
   uint8_t fill5[4];
   /** Slew Rate Enable Register at address offset 0x0B0, write-only */
   uint32_t pio_srer_0;
   /** Slew Rate Disable Register at address offset 0x0B4, write-only */
   uint32_t pio_srdr_0;
   /** Slew Rate Status Register at address offset 0x0B8, read-only */
   uint32_t pio_srsr_0;
   /** Reserved space */
   uint8_t fill6[4];
   /** Pull to High Level Enable Register at address offset 0x0C0, write-only */
   uint32_t pio_pher_0;
   /** Pull to High Level Disable Register at address offset 0x0C4, write-only */
   uint32_t pio_phdr_0;
   /** Pull to High Level Status Register at address offset 0x0C8, read-only */
   uint32_t pio_phsr_0;
   /** Reserved space */
   uint8_t fill7[4];
   /** Pull to Low Level Enable Register at address offset 0x0D0, write-only */
   uint32_t pio_pler_0;
   /** Pull to Low Level Disable Register at address offset 0x0D4, write-only */
   uint32_t pio_pldr_0;
   /** Pull to Low Level Status Register at address offset 0x0D8, read-only */
   uint32_t pio_plsr_0;
   /** Reserved space */
   uint8_t fill8[4];
   /** Schmitt trigger Enable Register at address offset 0x0E0, write-only */
   uint32_t pio_ster_0;
   /** Schmitt trigger Disable Register at address offset 0x0E4, write-only */
   uint32_t pio_stdr_0;
   /** Schmitt trigger Status Register at address offset 0x0E8, read-only */
   uint32_t pio_stsr_0;
   /** Reserved space */
   uint8_t fill9[4];
   /** Input Enable Register at address offset 0x0F0, write-only */
   uint32_t pio_iner_0;
   /** Input Disable Register at address offset 0x0F4, write-only */
   uint32_t pio_indr_0;
   /** Input Enable Status Register at address offset 0x0F8, read-only */
   uint32_t pio_insr_0;
   /** Reserved space */
   uint8_t fill10[4];
   /** Periph Ctrl PU/PD Enable Register at address offset 0x100, write-only */
   uint32_t pio_percper_0;
   /** Periph Ctrl PU/PD Disable Register at address offset 0x104, write-only */
   uint32_t pio_percpdr_0;
   /** Periph Ctrl PU/PD Status Register at address offset 0x108, read-only */
   uint32_t pio_percpsr_0;
   /** Reserved space */
   uint8_t fill11[4];
   /** HW Observation Selector Status Register at address offset 0x110, read-write */
   uint32_t pio_hwobssr_0;
   /** Input Filter Status Register (selector 2) at address offset 0x114, read-write */
   uint32_t pio_if2sr_0;
   /** Input Filter Status Register (selector 3) at address offset 0x118, read-write */
   uint32_t pio_if3sr_0;
   /** Reserved space */
   uint8_t fill12[4];
   /** Interrupt Event Enable Register at address offset 0x120, write-only */
   uint32_t pio_ever_0;
   /** Interrupt Event Disable Register at address offset 0x124, write-only */
   uint32_t pio_evdr_0;
   /** Interrupt Event Status Register at address offset 0x128, read-only */
   uint32_t pio_evsr_0;
   /** Reserved space */
   uint8_t fill13[4];
   /** Interrupt Event Type Enable Register at address offset 0x130, write-only */
   uint32_t pio_evter_0;
   /** Interrupt Event Type Disable Register at address offset 0x134, write-only */
   uint32_t pio_evtdr_0;
   /** Interrupt Event Type Status Register at address offset 0x138, read-only */
   uint32_t pio_evtsr_0;
   /** Interrupt Double Edge Register at address offset 0x13C, read-write */
   uint32_t pio_deevsr_0;
   /** Wakeup Source Enable Register at address offset 0x140, write-only */
   uint32_t pio_wser_0;
   /** Wakeup Source Disable Register at address offset 0x144, write-only */
   uint32_t pio_wsdr_0;
   /** Wakeup Source Mask Register at address offset 0x148, read-only */
   uint32_t pio_wsmr_0;
   /** Wakeup Source Status Register at address offset 0x14C, read-write */
   uint32_t pio_wssr_0;
   /** PIO Wakeup Source Enable Register at address offset 0x150, write-only */
   uint32_t pio_pwser_0;
   /** PIO Wakeup Source Disable Register at address offset 0x154, write-only */
   uint32_t pio_pwsdr_0;
   /** PIO Wakeup Source Mask Register at address offset 0x158, read-only */
   uint32_t pio_pwsmr_0;
   /** PIO Wakeup Source Status Register at address offset 0x15C, read-write */
   uint32_t pio_pwssr_0;
   /** Wakeup Event Enable Register at address offset 0x160, write-only */
   uint32_t pio_weer_0;
   /** Wakeup Event Disable Register at address offset 0x164, write-only */
   uint32_t pio_wedr_0;
   /** Wakeup Event Status Register at address offset 0x168, read-only */
   uint32_t pio_wesr_0;
   /** Wakeup Data Status Register at address offset 0x16C, read-only */
   uint32_t pio_wdsr_0;
   /** Wakeup Event Type Enable Register at address offset 0x170, write-only */
   uint32_t pio_weter_0;
   /** Wakeup Event Type Disable Register at address offset 0x174, write-only */
   uint32_t pio_wetdr_0;
   /** Wakeup Event Type Status Register at address offset 0x178, read-only */
   uint32_t pio_wetsr_0;
   /** Wakeup Event Double Edge Register at address offset 0x17C, read-write */
   uint32_t pio_wedesr_0;
   /** PIO Wakeup Event Enable Register at address offset 0x180, write-only */
   uint32_t pio_pweer_0;
   /** PIO Wakeup Event Disable Register at address offset 0x184, write-only */
   uint32_t pio_pwedr_0;
   /** PIO Wakeup Event Status Register at address offset 0x188, read-only */
   uint32_t pio_pwesr_0;
   /** Reserved space */
   uint8_t fill14[4];
   /** PIO Wakeup Event Type Enable Register at address offset 0x190, write-only */
   uint32_t pio_pweter_0;
   /** PIO Wakeup Event Type Disable Register at address offset 0x194, write-only */
   uint32_t pio_pwetdr_0;
   /** PIO Wakeup Event Type Status Register at address offset 0x198, read-only */
   uint32_t pio_pwetsr_0;
   /** PIO Wakeup Event Double Edge Register at address offset 0x19C, read-write */
   uint32_t pio_pwedesr_0;
   /** WakeUp Redux 0-3 Status Register at address offset 0x1A0, read-write */
   uint32_t pio_wur0sr_0;
   /** WakeUp Redux 4-7 Status Register at address offset 0x1A4, read-write */
   uint32_t pio_wur1sr_0;
   /** WakeUp Redux 8-11 Status Register at address offset 0x1A8, read-write */
   uint32_t pio_wur2sr_0;
   /** WakeUp Redux 12-15 Status Register at address offset 0x1AC, read-write */
   uint32_t pio_wur3sr_0;
   /** WakeUp Redux 16-19 Status Register at address offset 0x1B0, read-write */
   uint32_t pio_wur4sr_0;
   /** WakeUp Redux 20-23 Status Register at address offset 0x1B4, read-write */
   uint32_t pio_wur5sr_0;
   /** WakeUp Redux 24-27 Status Register at address offset 0x1B8, read-write */
   uint32_t pio_wur6sr_0;
   /** WakeUp Redux 28-31 Status Register at address offset 0x1BC, read-write */
   uint32_t pio_wur7sr_0;
   /** Reserved space */
   uint8_t fill15[96];
   /** PIO Enable Register at address offset 0x220, write-only */
   uint32_t pio_per_1;
   /** PIO Disable Register at address offset 0x224, write-only */
   uint32_t pio_pdr_1;
   /** PIO Status Register at address offset 0x228, read-only */
   uint32_t pio_psr_1;
   /** Reserved space */
   uint8_t fill16[4];
   /** Output Enable Register at address offset 0x230, write-only */
   uint32_t pio_oer_1;
   /** Output Disable Register at address offset 0x234, write-only */
   uint32_t pio_odr_1;
   /** Output Status Register at address offset 0x238, read-only */
   uint32_t pio_osr_1;
   /** Output Direction Register (negated) at address offset 0x23C, read-only */
   uint32_t pio_posr_1;
   /** Input Filter Enable Register at address offset 0x240, write-only */
   uint32_t pio_ifer_1;
   /** Input Filter Disable Register at address offset 0x244, write-only */
   uint32_t pio_ifdr_1;
   /** Input Filter Status Register (selector 1) at address offset 0x248, read-only */
   uint32_t pio_ifsr_1;
   /** Reserved space */
   uint8_t fill17[4];
   /** Set Output Data Register at address offset 0x250, write-only */
   uint32_t pio_sodr_1;
   /** Clear Output Data Register at address offset 0x254, write-only */
   uint32_t pio_codr_1;
   /** Output Data Status Register at address offset 0x258, read-write */
   uint32_t pio_odsr_1;
   /** Pin Data Status Register at address offset 0x25C, read-only */
   uint32_t pio_pdsr_1;
   /** 1st Interrupt Enable Register at address offset 0x260, write-only */
   uint32_t pio_ier1_1;
   /** 1st Interrupt Disable Register at address offset 0x264, write-only */
   uint32_t pio_idr1_1;
   /** 1st Interrupt Mask Register at address offset 0x268, read-only */
   uint32_t pio_imr1_1;
   /** Reserved space */
   uint8_t fill18[4];
   /** 2nd Interrupt Enable Register at address offset 0x270, write-only */
   uint32_t pio_ier2_1;
   /** 2nd Interrupt Disable Register at address offset 0x274, write-only */
   uint32_t pio_idr2_1;
   /** 2nd Interrupt Mask Register at address offset 0x278, read-only */
   uint32_t pio_imr2_1;
   /** Interrupt Status Register at address offset 0x27C, read-write */
   uint32_t pio_isr_1;
   /** Multi-Driver Enable Register at address offset 0x280, write-only */
   uint32_t pio_mder_1;
   /** Multi-Driver Disable Register at address offset 0x284, write-only */
   uint32_t pio_mddr_1;
   /** Multi-Driver Status Register at address offset 0x288, read-only */
   uint32_t pio_mdsr_1;
   /** Reserved space */
   uint8_t fill19[4];
   /** PIO check in Register at address offset 0x290, read-only */
   uint32_t pio_ckin_1;
   /** PIO general purpose Register at address offset 0x294, read-write */
   uint32_t pio_gpsr_1;
   /** Select A Register at address offset 0x298, write-only */
   uint32_t pio_asr_1;
   /** Select B Register at address offset 0x29C, write-only */
   uint32_t pio_bsr_1;
   /** Select C Register at address offset 0x2A0, write-only */
   uint32_t pio_csr_1;
   /** Select D Register at address offset 0x2A4, write-only */
   uint32_t pio_dsr_1;
   /** MSB Select Status Register at address offset 0x2A8, read-only */
   uint32_t pio_mselsr_1;
   /** LSB Select Status Register at address offset 0x2AC, read-only */
   uint32_t pio_lselsr_1;
   /** MSB Drive Strength Enable Register at address offset 0x2B0, write-only */
   uint32_t pio_mdser_1;
   /** MSB Drive Strength Disable Register at address offset 0x2B4, write-only */
   uint32_t pio_mdsdr_1;
   /** MSB Drive Strength Status Register at address offset 0x2B8, read-only */
   uint32_t pio_mdssr_1;
   /** Reserved space */
   uint8_t fill20[4];
   /** LSB Drive Strength Enable Register at address offset 0x2C0, write-only */
   uint32_t pio_ldser_1;
   /** LSB Drive Strength Disable Register at address offset 0x2C4, write-only */
   uint32_t pio_ldsdr_1;
   /** LSB Drive Strength Status Register at address offset 0x2C8, read-only */
   uint32_t pio_ldssr_1;
   /** Reserved space */
   uint8_t fill21[4];
   /** Slew Rate Enable Register at address offset 0x2D0, write-only */
   uint32_t pio_srer_1;
   /** Slew Rate Disable Register at address offset 0x2D4, write-only */
   uint32_t pio_srdr_1;
   /** Slew Rate Status Register at address offset 0x2D8, read-only */
   uint32_t pio_srsr_1;
   /** Reserved space */
   uint8_t fill22[4];
   /** Pull to High Level Enable Register at address offset 0x2E0, write-only */
   uint32_t pio_pher_1;
   /** Pull to High Level Disable Register at address offset 0x2E4, write-only */
   uint32_t pio_phdr_1;
   /** Pull to High Level Status Register at address offset 0x2E8, read-only */
   uint32_t pio_phsr_1;
   /** Reserved space */
   uint8_t fill23[4];
   /** Pull to Low Level Enable Register at address offset 0x2F0, write-only */
   uint32_t pio_pler_1;
   /** Pull to Low Level Disable Register at address offset 0x2F4, write-only */
   uint32_t pio_pldr_1;
   /** Pull to Low Level Status Register at address offset 0x2F8, read-only */
   uint32_t pio_plsr_1;
   /** Reserved space */
   uint8_t fill24[4];
   /** Schmitt trigger Enable Register at address offset 0x300, write-only */
   uint32_t pio_ster_1;
   /** Schmitt trigger Disable Register at address offset 0x304, write-only */
   uint32_t pio_stdr_1;
   /** Schmitt trigger Status Register at address offset 0x308, read-only */
   uint32_t pio_stsr_1;
   /** Reserved space */
   uint8_t fill25[4];
   /** Input Enable Register at address offset 0x310, write-only */
   uint32_t pio_iner_1;
   /** Input Disable Register at address offset 0x314, write-only */
   uint32_t pio_indr_1;
   /** Input Enable Status Register at address offset 0x318, read-only */
   uint32_t pio_insr_1;
   /** Reserved space */
   uint8_t fill26[4];
   /** Periph Ctrl PU/PD Enable Register at address offset 0x320, write-only */
   uint32_t pio_percper_1;
   /** Periph Ctrl PU/PD Disable Register at address offset 0x324, write-only */
   uint32_t pio_percpdr_1;
   /** Periph Ctrl PU/PD Status Register at address offset 0x328, read-only */
   uint32_t pio_percpsr_1;
   /** Reserved space */
   uint8_t fill27[4];
   /** HW Observation Selector Status Register at address offset 0x330, read-write */
   uint32_t pio_hwobssr_1;
   /** Input Filter Status Register (selector 2) at address offset 0x334, read-write */
   uint32_t pio_if2sr_1;
   /** Input Filter Status Register (selector 3) at address offset 0x338, read-write */
   uint32_t pio_if3sr_1;
   /** Reserved space */
   uint8_t fill28[4];
   /** Interrupt Event Enable Register at address offset 0x340, write-only */
   uint32_t pio_ever_1;
   /** Interrupt Event Disable Register at address offset 0x344, write-only */
   uint32_t pio_evdr_1;
   /** Interrupt Event Status Register at address offset 0x348, read-only */
   uint32_t pio_evsr_1;
   /** Reserved space */
   uint8_t fill29[4];
   /** Interrupt Event Type Enable Register at address offset 0x350, write-only */
   uint32_t pio_evter_1;
   /** Interrupt Event Type Disable Register at address offset 0x354, write-only */
   uint32_t pio_evtdr_1;
   /** Interrupt Event Type Status Register at address offset 0x358, read-only */
   uint32_t pio_evtsr_1;
   /** Interrupt Double Edge Register at address offset 0x35C, read-write */
   uint32_t pio_deevsr_1;
   /** Wakeup Source Enable Register at address offset 0x360, write-only */
   uint32_t pio_wser_1;
   /** Wakeup Source Disable Register at address offset 0x364, write-only */
   uint32_t pio_wsdr_1;
   /** Wakeup Source Mask Register at address offset 0x368, read-only */
   uint32_t pio_wsmr_1;
   /** Wakeup Source Status Register at address offset 0x36C, read-write */
   uint32_t pio_wssr_1;
   /** PIO Wakeup Source Enable Register at address offset 0x370, write-only */
   uint32_t pio_pwser_1;
   /** PIO Wakeup Source Disable Register at address offset 0x374, write-only */
   uint32_t pio_pwsdr_1;
   /** PIO Wakeup Source Mask Register at address offset 0x378, read-only */
   uint32_t pio_pwsmr_1;
   /** PIO Wakeup Source Status Register at address offset 0x37C, read-write */
   uint32_t pio_pwssr_1;
   /** Wakeup Event Enable Register at address offset 0x380, write-only */
   uint32_t pio_weer_1;
   /** Wakeup Event Disable Register at address offset 0x384, write-only */
   uint32_t pio_wedr_1;
   /** Wakeup Event Status Register at address offset 0x388, read-only */
   uint32_t pio_wesr_1;
   /** Wakeup Data Status Register at address offset 0x38C, read-only */
   uint32_t pio_wdsr_1;
   /** Wakeup Event Type Enable Register at address offset 0x390, write-only */
   uint32_t pio_weter_1;
   /** Wakeup Event Type Disable Register at address offset 0x394, write-only */
   uint32_t pio_wetdr_1;
   /** Wakeup Event Type Status Register at address offset 0x398, read-only */
   uint32_t pio_wetsr_1;
   /** Wakeup Event Double Edge Register at address offset 0x39C, read-write */
   uint32_t pio_wedesr_1;
   /** PIO Wakeup Event Enable Register at address offset 0x3A0, write-only */
   uint32_t pio_pweer_1;
   /** PIO Wakeup Event Disable Register at address offset 0x3A4, write-only */
   uint32_t pio_pwedr_1;
   /** PIO Wakeup Event Status Register at address offset 0x3A8, read-only */
   uint32_t pio_pwesr_1;
   /** Reserved space */
   uint8_t fill30[4];
   /** PIO Wakeup Event Type Enable Register at address offset 0x3B0, write-only */
   uint32_t pio_pweter_1;
   /** PIO Wakeup Event Type Disable Register at address offset 0x3B4, write-only */
   uint32_t pio_pwetdr_1;
   /** PIO Wakeup Event Type Status Register at address offset 0x3B8, read-only */
   uint32_t pio_pwetsr_1;
   /** PIO Wakeup Event Double Edge Register at address offset 0x3BC, read-write */
   uint32_t pio_pwedesr_1;
   /** WakeUp Redux 0-3 Status Register at address offset 0x3C0, read-write */
   uint32_t pio_wur0sr_1;
   /** WakeUp Redux 4-7 Status Register at address offset 0x3C4, read-write */
   uint32_t pio_wur1sr_1;
   /** WakeUp Redux 8-11 Status Register at address offset 0x3C8, read-write */
   uint32_t pio_wur2sr_1;
   /** WakeUp Redux 12-15 Status Register at address offset 0x3CC, read-write */
   uint32_t pio_wur3sr_1;
   /** WakeUp Redux 16-19 Status Register at address offset 0x3D0, read-write */
   uint32_t pio_wur4sr_1;
   /** WakeUp Redux 20-23 Status Register at address offset 0x3D4, read-write */
   uint32_t pio_wur5sr_1;
   /** WakeUp Redux 24-27 Status Register at address offset 0x3D8, read-write */
   uint32_t pio_wur6sr_1;
   /** WakeUp Redux 28-31 Status Register at address offset 0x3DC, read-write */
   uint32_t pio_wur7sr_1;
   /** Reserved space */
   uint8_t fill31[96];
   /** PIO Enable Register at address offset 0x440, write-only */
   uint32_t pio_per_2;
   /** PIO Disable Register at address offset 0x444, write-only */
   uint32_t pio_pdr_2;
   /** PIO Status Register at address offset 0x448, read-only */
   uint32_t pio_psr_2;
   /** Reserved space */
   uint8_t fill32[4];
   /** Output Enable Register at address offset 0x450, write-only */
   uint32_t pio_oer_2;
   /** Output Disable Register at address offset 0x454, write-only */
   uint32_t pio_odr_2;
   /** Output Status Register at address offset 0x458, read-only */
   uint32_t pio_osr_2;
   /** Output Direction Register (negated) at address offset 0x45C, read-only */
   uint32_t pio_posr_2;
   /** Input Filter Enable Register at address offset 0x460, write-only */
   uint32_t pio_ifer_2;
   /** Input Filter Disable Register at address offset 0x464, write-only */
   uint32_t pio_ifdr_2;
   /** Input Filter Status Register (selector 1) at address offset 0x468, read-only */
   uint32_t pio_ifsr_2;
   /** Reserved space */
   uint8_t fill33[4];
   /** Set Output Data Register at address offset 0x470, write-only */
   uint32_t pio_sodr_2;
   /** Clear Output Data Register at address offset 0x474, write-only */
   uint32_t pio_codr_2;
   /** Output Data Status Register at address offset 0x478, read-write */
   uint32_t pio_odsr_2;
   /** Pin Data Status Register at address offset 0x47C, read-only */
   uint32_t pio_pdsr_2;
   /** 1st Interrupt Enable Register at address offset 0x480, write-only */
   uint32_t pio_ier1_2;
   /** 1st Interrupt Disable Register at address offset 0x484, write-only */
   uint32_t pio_idr1_2;
   /** 1st Interrupt Mask Register at address offset 0x488, read-only */
   uint32_t pio_imr1_2;
   /** Reserved space */
   uint8_t fill34[4];
   /** 2nd Interrupt Enable Register at address offset 0x490, write-only */
   uint32_t pio_ier2_2;
   /** 2nd Interrupt Disable Register at address offset 0x494, write-only */
   uint32_t pio_idr2_2;
   /** 2nd Interrupt Mask Register at address offset 0x498, read-only */
   uint32_t pio_imr2_2;
   /** Interrupt Status Register at address offset 0x49C, read-write */
   uint32_t pio_isr_2;
   /** Multi-Driver Enable Register at address offset 0x4A0, write-only */
   uint32_t pio_mder_2;
   /** Multi-Driver Disable Register at address offset 0x4A4, write-only */
   uint32_t pio_mddr_2;
   /** Multi-Driver Status Register at address offset 0x4A8, read-only */
   uint32_t pio_mdsr_2;
   /** Reserved space */
   uint8_t fill35[4];
   /** PIO check in Register at address offset 0x4B0, read-only */
   uint32_t pio_ckin_2;
   /** PIO general purpose Register at address offset 0x4B4, read-write */
   uint32_t pio_gpsr_2;
   /** Select A Register at address offset 0x4B8, write-only */
   uint32_t pio_asr_2;
   /** Select B Register at address offset 0x4BC, write-only */
   uint32_t pio_bsr_2;
   /** Select C Register at address offset 0x4C0, write-only */
   uint32_t pio_csr_2;
   /** Select D Register at address offset 0x4C4, write-only */
   uint32_t pio_dsr_2;
   /** MSB Select Status Register at address offset 0x4C8, read-only */
   uint32_t pio_mselsr_2;
   /** LSB Select Status Register at address offset 0x4CC, read-only */
   uint32_t pio_lselsr_2;
   /** MSB Drive Strength Enable Register at address offset 0x4D0, write-only */
   uint32_t pio_mdser_2;
   /** MSB Drive Strength Disable Register at address offset 0x4D4, write-only */
   uint32_t pio_mdsdr_2;
   /** MSB Drive Strength Status Register at address offset 0x4D8, read-only */
   uint32_t pio_mdssr_2;
   /** Reserved space */
   uint8_t fill36[4];
   /** LSB Drive Strength Enable Register at address offset 0x4E0, write-only */
   uint32_t pio_ldser_2;
   /** LSB Drive Strength Disable Register at address offset 0x4E4, write-only */
   uint32_t pio_ldsdr_2;
   /** LSB Drive Strength Status Register at address offset 0x4E8, read-only */
   uint32_t pio_ldssr_2;
   /** Reserved space */
   uint8_t fill37[4];
   /** Slew Rate Enable Register at address offset 0x4F0, write-only */
   uint32_t pio_srer_2;
   /** Slew Rate Disable Register at address offset 0x4F4, write-only */
   uint32_t pio_srdr_2;
   /** Slew Rate Status Register at address offset 0x4F8, read-only */
   uint32_t pio_srsr_2;
   /** Reserved space */
   uint8_t fill38[4];
   /** Pull to High Level Enable Register at address offset 0x500, write-only */
   uint32_t pio_pher_2;
   /** Pull to High Level Disable Register at address offset 0x504, write-only */
   uint32_t pio_phdr_2;
   /** Pull to High Level Status Register at address offset 0x508, read-only */
   uint32_t pio_phsr_2;
   /** Reserved space */
   uint8_t fill39[4];
   /** Pull to Low Level Enable Register at address offset 0x510, write-only */
   uint32_t pio_pler_2;
   /** Pull to Low Level Disable Register at address offset 0x514, write-only */
   uint32_t pio_pldr_2;
   /** Pull to Low Level Status Register at address offset 0x518, read-only */
   uint32_t pio_plsr_2;
   /** Reserved space */
   uint8_t fill40[4];
   /** Schmitt trigger Enable Register at address offset 0x520, write-only */
   uint32_t pio_ster_2;
   /** Schmitt trigger Disable Register at address offset 0x524, write-only */
   uint32_t pio_stdr_2;
   /** Schmitt trigger Status Register at address offset 0x528, read-only */
   uint32_t pio_stsr_2;
   /** Reserved space */
   uint8_t fill41[4];
   /** Input Enable Register at address offset 0x530, write-only */
   uint32_t pio_iner_2;
   /** Input Disable Register at address offset 0x534, write-only */
   uint32_t pio_indr_2;
   /** Input Enable Status Register at address offset 0x538, read-only */
   uint32_t pio_insr_2;
   /** Reserved space */
   uint8_t fill42[4];
   /** Periph Ctrl PU/PD Enable Register at address offset 0x540, write-only */
   uint32_t pio_percper_2;
   /** Periph Ctrl PU/PD Disable Register at address offset 0x544, write-only */
   uint32_t pio_percpdr_2;
   /** Periph Ctrl PU/PD Status Register at address offset 0x548, read-only */
   uint32_t pio_percpsr_2;
   /** Reserved space */
   uint8_t fill43[4];
   /** HW Observation Selector Status Register at address offset 0x550, read-write */
   uint32_t pio_hwobssr_2;
   /** Input Filter Status Register (selector 2) at address offset 0x554, read-write */
   uint32_t pio_if2sr_2;
   /** Input Filter Status Register (selector 3) at address offset 0x558, read-write */
   uint32_t pio_if3sr_2;
   /** Reserved space */
   uint8_t fill44[4];
   /** Interrupt Event Enable Register at address offset 0x560, write-only */
   uint32_t pio_ever_2;
   /** Interrupt Event Disable Register at address offset 0x564, write-only */
   uint32_t pio_evdr_2;
   /** Interrupt Event Status Register at address offset 0x568, read-only */
   uint32_t pio_evsr_2;
   /** Reserved space */
   uint8_t fill45[4];
   /** Interrupt Event Type Enable Register at address offset 0x570, write-only */
   uint32_t pio_evter_2;
   /** Interrupt Event Type Disable Register at address offset 0x574, write-only */
   uint32_t pio_evtdr_2;
   /** Interrupt Event Type Status Register at address offset 0x578, read-only */
   uint32_t pio_evtsr_2;
   /** Interrupt Double Edge Register at address offset 0x57C, read-write */
   uint32_t pio_deevsr_2;
   /** Wakeup Source Enable Register at address offset 0x580, write-only */
   uint32_t pio_wser_2;
   /** Wakeup Source Disable Register at address offset 0x584, write-only */
   uint32_t pio_wsdr_2;
   /** Wakeup Source Mask Register at address offset 0x588, read-only */
   uint32_t pio_wsmr_2;
   /** Wakeup Source Status Register at address offset 0x58C, read-write */
   uint32_t pio_wssr_2;
   /** PIO Wakeup Source Enable Register at address offset 0x590, write-only */
   uint32_t pio_pwser_2;
   /** PIO Wakeup Source Disable Register at address offset 0x594, write-only */
   uint32_t pio_pwsdr_2;
   /** PIO Wakeup Source Mask Register at address offset 0x598, read-only */
   uint32_t pio_pwsmr_2;
   /** PIO Wakeup Source Status Register at address offset 0x59C, read-write */
   uint32_t pio_pwssr_2;
   /** Wakeup Event Enable Register at address offset 0x5A0, write-only */
   uint32_t pio_weer_2;
   /** Wakeup Event Disable Register at address offset 0x5A4, write-only */
   uint32_t pio_wedr_2;
   /** Wakeup Event Status Register at address offset 0x5A8, read-only */
   uint32_t pio_wesr_2;
   /** Wakeup Data Status Register at address offset 0x5AC, read-only */
   uint32_t pio_wdsr_2;
   /** Wakeup Event Type Enable Register at address offset 0x5B0, write-only */
   uint32_t pio_weter_2;
   /** Wakeup Event Type Disable Register at address offset 0x5B4, write-only */
   uint32_t pio_wetdr_2;
   /** Wakeup Event Type Status Register at address offset 0x5B8, read-only */
   uint32_t pio_wetsr_2;
   /** Wakeup Event Double Edge Register at address offset 0x5BC, read-write */
   uint32_t pio_wedesr_2;
   /** PIO Wakeup Event Enable Register at address offset 0x5C0, write-only */
   uint32_t pio_pweer_2;
   /** PIO Wakeup Event Disable Register at address offset 0x5C4, write-only */
   uint32_t pio_pwedr_2;
   /** PIO Wakeup Event Status Register at address offset 0x5C8, read-only */
   uint32_t pio_pwesr_2;
   /** Reserved space */
   uint8_t fill46[4];
   /** PIO Wakeup Event Type Enable Register at address offset 0x5D0, write-only */
   uint32_t pio_pweter_2;
   /** PIO Wakeup Event Type Disable Register at address offset 0x5D4, write-only */
   uint32_t pio_pwetdr_2;
   /** PIO Wakeup Event Type Status Register at address offset 0x5D8, read-only */
   uint32_t pio_pwetsr_2;
   /** PIO Wakeup Event Double Edge Register at address offset 0x5DC, read-write */
   uint32_t pio_pwedesr_2;
   /** WakeUp Redux 0-3 Status Register at address offset 0x5E0, read-write */
   uint32_t pio_wur0sr_2;
   /** WakeUp Redux 4-7 Status Register at address offset 0x5E4, read-write */
   uint32_t pio_wur1sr_2;
   /** WakeUp Redux 8-11 Status Register at address offset 0x5E8, read-write */
   uint32_t pio_wur2sr_2;
   /** WakeUp Redux 12-15 Status Register at address offset 0x5EC, read-write */
   uint32_t pio_wur3sr_2;
   /** WakeUp Redux 16-19 Status Register at address offset 0x5F0, read-write */
   uint32_t pio_wur4sr_2;
   /** WakeUp Redux 20-23 Status Register at address offset 0x5F4, read-write */
   uint32_t pio_wur5sr_2;
   /** WakeUp Redux 24-27 Status Register at address offset 0x5F8, read-write */
   uint32_t pio_wur6sr_2;
   /** WakeUp Redux 28-31 Status Register at address offset 0x5FC, read-write */
   uint32_t pio_wur7sr_2;
   /** Reserved space */
   uint8_t fill47[96];
   /** PIO Enable Register at address offset 0x660, write-only */
   uint32_t pio_per_3;
   /** PIO Disable Register at address offset 0x664, write-only */
   uint32_t pio_pdr_3;
   /** PIO Status Register at address offset 0x668, read-only */
   uint32_t pio_psr_3;
   /** Reserved space */
   uint8_t fill48[4];
   /** Output Enable Register at address offset 0x670, write-only */
   uint32_t pio_oer_3;
   /** Output Disable Register at address offset 0x674, write-only */
   uint32_t pio_odr_3;
   /** Output Status Register at address offset 0x678, read-only */
   uint32_t pio_osr_3;
   /** Output Direction Register (negated) at address offset 0x67C, read-only */
   uint32_t pio_posr_3;
   /** Input Filter Enable Register at address offset 0x680, write-only */
   uint32_t pio_ifer_3;
   /** Input Filter Disable Register at address offset 0x684, write-only */
   uint32_t pio_ifdr_3;
   /** Input Filter Status Register (selector 1) at address offset 0x688, read-only */
   uint32_t pio_ifsr_3;
   /** Reserved space */
   uint8_t fill49[4];
   /** Set Output Data Register at address offset 0x690, write-only */
   uint32_t pio_sodr_3;
   /** Clear Output Data Register at address offset 0x694, write-only */
   uint32_t pio_codr_3;
   /** Output Data Status Register at address offset 0x698, read-write */
   uint32_t pio_odsr_3;
   /** Pin Data Status Register at address offset 0x69C, read-only */
   uint32_t pio_pdsr_3;
   /** 1st Interrupt Enable Register at address offset 0x6A0, write-only */
   uint32_t pio_ier1_3;
   /** 1st Interrupt Disable Register at address offset 0x6A4, write-only */
   uint32_t pio_idr1_3;
   /** 1st Interrupt Mask Register at address offset 0x6A8, read-only */
   uint32_t pio_imr1_3;
   /** Reserved space */
   uint8_t fill50[4];
   /** 2nd Interrupt Enable Register at address offset 0x6B0, write-only */
   uint32_t pio_ier2_3;
   /** 2nd Interrupt Disable Register at address offset 0x6B4, write-only */
   uint32_t pio_idr2_3;
   /** 2nd Interrupt Mask Register at address offset 0x6B8, read-only */
   uint32_t pio_imr2_3;
   /** Interrupt Status Register at address offset 0x6BC, read-write */
   uint32_t pio_isr_3;
   /** Multi-Driver Enable Register at address offset 0x6C0, write-only */
   uint32_t pio_mder_3;
   /** Multi-Driver Disable Register at address offset 0x6C4, write-only */
   uint32_t pio_mddr_3;
   /** Multi-Driver Status Register at address offset 0x6C8, read-only */
   uint32_t pio_mdsr_3;
   /** Reserved space */
   uint8_t fill51[4];
   /** PIO check in Register at address offset 0x6D0, read-only */
   uint32_t pio_ckin_3;
   /** PIO general purpose Register at address offset 0x6D4, read-write */
   uint32_t pio_gpsr_3;
   /** Select A Register at address offset 0x6D8, write-only */
   uint32_t pio_asr_3;
   /** Select B Register at address offset 0x6DC, write-only */
   uint32_t pio_bsr_3;
   /** Select C Register at address offset 0x6E0, write-only */
   uint32_t pio_csr_3;
   /** Select D Register at address offset 0x6E4, write-only */
   uint32_t pio_dsr_3;
   /** MSB Select Status Register at address offset 0x6E8, read-only */
   uint32_t pio_mselsr_3;
   /** LSB Select Status Register at address offset 0x6EC, read-only */
   uint32_t pio_lselsr_3;
   /** MSB Drive Strength Enable Register at address offset 0x6F0, write-only */
   uint32_t pio_mdser_3;
   /** MSB Drive Strength Disable Register at address offset 0x6F4, write-only */
   uint32_t pio_mdsdr_3;
   /** MSB Drive Strength Status Register at address offset 0x6F8, read-only */
   uint32_t pio_mdssr_3;
   /** Reserved space */
   uint8_t fill52[4];
   /** LSB Drive Strength Enable Register at address offset 0x700, write-only */
   uint32_t pio_ldser_3;
   /** LSB Drive Strength Disable Register at address offset 0x704, write-only */
   uint32_t pio_ldsdr_3;
   /** LSB Drive Strength Status Register at address offset 0x708, read-only */
   uint32_t pio_ldssr_3;
   /** Reserved space */
   uint8_t fill53[4];
   /** Slew Rate Enable Register at address offset 0x710, write-only */
   uint32_t pio_srer_3;
   /** Slew Rate Disable Register at address offset 0x714, write-only */
   uint32_t pio_srdr_3;
   /** Slew Rate Status Register at address offset 0x718, read-only */
   uint32_t pio_srsr_3;
   /** Reserved space */
   uint8_t fill54[4];
   /** Pull to High Level Enable Register at address offset 0x720, write-only */
   uint32_t pio_pher_3;
   /** Pull to High Level Disable Register at address offset 0x724, write-only */
   uint32_t pio_phdr_3;
   /** Pull to High Level Status Register at address offset 0x728, read-only */
   uint32_t pio_phsr_3;
   /** Reserved space */
   uint8_t fill55[4];
   /** Pull to Low Level Enable Register at address offset 0x730, write-only */
   uint32_t pio_pler_3;
   /** Pull to Low Level Disable Register at address offset 0x734, write-only */
   uint32_t pio_pldr_3;
   /** Pull to Low Level Status Register at address offset 0x738, read-only */
   uint32_t pio_plsr_3;
   /** Reserved space */
   uint8_t fill56[4];
   /** Schmitt trigger Enable Register at address offset 0x740, write-only */
   uint32_t pio_ster_3;
   /** Schmitt trigger Disable Register at address offset 0x744, write-only */
   uint32_t pio_stdr_3;
   /** Schmitt trigger Status Register at address offset 0x748, read-only */
   uint32_t pio_stsr_3;
   /** Reserved space */
   uint8_t fill57[4];
   /** Input Enable Register at address offset 0x750, write-only */
   uint32_t pio_iner_3;
   /** Input Disable Register at address offset 0x754, write-only */
   uint32_t pio_indr_3;
   /** Input Enable Status Register at address offset 0x758, read-only */
   uint32_t pio_insr_3;
   /** Reserved space */
   uint8_t fill58[4];
   /** Periph Ctrl PU/PD Enable Register at address offset 0x760, write-only */
   uint32_t pio_percper_3;
   /** Periph Ctrl PU/PD Disable Register at address offset 0x764, write-only */
   uint32_t pio_percpdr_3;
   /** Periph Ctrl PU/PD Status Register at address offset 0x768, read-only */
   uint32_t pio_percpsr_3;
   /** Reserved space */
   uint8_t fill59[4];
   /** HW Observation Selector Status Register at address offset 0x770, read-write */
   uint32_t pio_hwobssr_3;
   /** Input Filter Status Register (selector 2) at address offset 0x774, read-write */
   uint32_t pio_if2sr_3;
   /** Input Filter Status Register (selector 3) at address offset 0x778, read-write */
   uint32_t pio_if3sr_3;
   /** Reserved space */
   uint8_t fill60[4];
   /** Interrupt Event Enable Register at address offset 0x780, write-only */
   uint32_t pio_ever_3;
   /** Interrupt Event Disable Register at address offset 0x784, write-only */
   uint32_t pio_evdr_3;
   /** Interrupt Event Status Register at address offset 0x788, read-only */
   uint32_t pio_evsr_3;
   /** Reserved space */
   uint8_t fill61[4];
   /** Interrupt Event Type Enable Register at address offset 0x790, write-only */
   uint32_t pio_evter_3;
   /** Interrupt Event Type Disable Register at address offset 0x794, write-only */
   uint32_t pio_evtdr_3;
   /** Interrupt Event Type Status Register at address offset 0x798, read-only */
   uint32_t pio_evtsr_3;
   /** Interrupt Double Edge Register at address offset 0x79C, read-write */
   uint32_t pio_deevsr_3;
   /** Wakeup Source Enable Register at address offset 0x7A0, write-only */
   uint32_t pio_wser_3;
   /** Wakeup Source Disable Register at address offset 0x7A4, write-only */
   uint32_t pio_wsdr_3;
   /** Wakeup Source Mask Register at address offset 0x7A8, read-only */
   uint32_t pio_wsmr_3;
   /** Wakeup Source Status Register at address offset 0x7AC, read-write */
   uint32_t pio_wssr_3;
   /** PIO Wakeup Source Enable Register at address offset 0x7B0, write-only */
   uint32_t pio_pwser_3;
   /** PIO Wakeup Source Disable Register at address offset 0x7B4, write-only */
   uint32_t pio_pwsdr_3;
   /** PIO Wakeup Source Mask Register at address offset 0x7B8, read-only */
   uint32_t pio_pwsmr_3;
   /** PIO Wakeup Source Status Register at address offset 0x7BC, read-write */
   uint32_t pio_pwssr_3;
   /** Wakeup Event Enable Register at address offset 0x7C0, write-only */
   uint32_t pio_weer_3;
   /** Wakeup Event Disable Register at address offset 0x7C4, write-only */
   uint32_t pio_wedr_3;
   /** Wakeup Event Status Register at address offset 0x7C8, read-only */
   uint32_t pio_wesr_3;
   /** Wakeup Data Status Register at address offset 0x7CC, read-only */
   uint32_t pio_wdsr_3;
   /** Wakeup Event Type Enable Register at address offset 0x7D0, write-only */
   uint32_t pio_weter_3;
   /** Wakeup Event Type Disable Register at address offset 0x7D4, write-only */
   uint32_t pio_wetdr_3;
   /** Wakeup Event Type Status Register at address offset 0x7D8, read-only */
   uint32_t pio_wetsr_3;
   /** Wakeup Event Double Edge Register at address offset 0x7DC, read-write */
   uint32_t pio_wedesr_3;
   /** PIO Wakeup Event Enable Register at address offset 0x7E0, write-only */
   uint32_t pio_pweer_3;
   /** PIO Wakeup Event Disable Register at address offset 0x7E4, write-only */
   uint32_t pio_pwedr_3;
   /** PIO Wakeup Event Status Register at address offset 0x7E8, read-only */
   uint32_t pio_pwesr_3;
   /** Reserved space */
   uint8_t fill62[4];
   /** PIO Wakeup Event Type Enable Register at address offset 0x7F0, write-only */
   uint32_t pio_pweter_3;
   /** PIO Wakeup Event Type Disable Register at address offset 0x7F4, write-only */
   uint32_t pio_pwetdr_3;
   /** PIO Wakeup Event Type Status Register at address offset 0x7F8, read-only */
   uint32_t pio_pwetsr_3;
   /** PIO Wakeup Event Double Edge Register at address offset 0x7FC, read-write */
   uint32_t pio_pwedesr_3;
   /** WakeUp Redux 0-3 Status Register at address offset 0x800, read-write */
   uint32_t pio_wur0sr_3;
   /** WakeUp Redux 4-7 Status Register at address offset 0x804, read-write */
   uint32_t pio_wur1sr_3;
   /** WakeUp Redux 8-11 Status Register at address offset 0x808, read-write */
   uint32_t pio_wur2sr_3;
   /** WakeUp Redux 12-15 Status Register at address offset 0x80C, read-write */
   uint32_t pio_wur3sr_3;
   /** WakeUp Redux 16-19 Status Register at address offset 0x810, read-write */
   uint32_t pio_wur4sr_3;
   /** WakeUp Redux 20-23 Status Register at address offset 0x814, read-write */
   uint32_t pio_wur5sr_3;
   /** WakeUp Redux 24-27 Status Register at address offset 0x818, read-write */
   uint32_t pio_wur6sr_3;
   /** WakeUp Redux 28-31 Status Register at address offset 0x81C, read-write */
   uint32_t pio_wur7sr_3;
};

/** bit field defines for pio_s#pio_per_0 */
#define PIO_PIO_PER_0_NONE_OFFSET 0
#define PIO_PIO_PER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pdr_0 */
#define PIO_PIO_PDR_0_NONE_OFFSET 0
#define PIO_PIO_PDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_psr_0 */
#define PIO_PIO_PSR_0_NONE_OFFSET 0
#define PIO_PIO_PSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_oer_0 */
#define PIO_PIO_OER_0_NONE_OFFSET 0
#define PIO_PIO_OER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_odr_0 */
#define PIO_PIO_ODR_0_NONE_OFFSET 0
#define PIO_PIO_ODR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_osr_0 */
#define PIO_PIO_OSR_0_NONE_OFFSET 0
#define PIO_PIO_OSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_posr_0 */
#define PIO_PIO_POSR_0_NONE_OFFSET 0
#define PIO_PIO_POSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_ifer_0 */
#define PIO_PIO_IFER_0_NONE_OFFSET 0
#define PIO_PIO_IFER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_ifdr_0 */
#define PIO_PIO_IFDR_0_NONE_OFFSET 0
#define PIO_PIO_IFDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_ifsr_0 */
#define PIO_PIO_IFSR_0_NONE_OFFSET 0
#define PIO_PIO_IFSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_sodr_0 */
#define PIO_PIO_SODR_0_NONE_OFFSET 0
#define PIO_PIO_SODR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_codr_0 */
#define PIO_PIO_CODR_0_NONE_OFFSET 0
#define PIO_PIO_CODR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_odsr_0 */
#define PIO_PIO_ODSR_0_NONE_OFFSET 0
#define PIO_PIO_ODSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pdsr_0 */
#define PIO_PIO_PDSR_0_NONE_OFFSET 0
#define PIO_PIO_PDSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_ier1_0 */
#define PIO_PIO_IER1_0_NONE_OFFSET 0
#define PIO_PIO_IER1_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_idr1_0 */
#define PIO_PIO_IDR1_0_NONE_OFFSET 0
#define PIO_PIO_IDR1_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_imr1_0 */
#define PIO_PIO_IMR1_0_NONE_OFFSET 0
#define PIO_PIO_IMR1_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_ier2_0 */
#define PIO_PIO_IER2_0_NONE_OFFSET 0
#define PIO_PIO_IER2_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_idr2_0 */
#define PIO_PIO_IDR2_0_NONE_OFFSET 0
#define PIO_PIO_IDR2_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_imr2_0 */
#define PIO_PIO_IMR2_0_NONE_OFFSET 0
#define PIO_PIO_IMR2_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_isr_0 */
#define PIO_PIO_ISR_0_NONE_OFFSET 0
#define PIO_PIO_ISR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_mder_0 */
#define PIO_PIO_MDER_0_NONE_OFFSET 0
#define PIO_PIO_MDER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_mddr_0 */
#define PIO_PIO_MDDR_0_NONE_OFFSET 0
#define PIO_PIO_MDDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdsr_0 */
#define PIO_PIO_MDSR_0_NONE_OFFSET 0
#define PIO_PIO_MDSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_ckin_0 */
#define PIO_PIO_CKIN_0_NONE_OFFSET 0
#define PIO_PIO_CKIN_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_gpsr_0 */
#define PIO_PIO_GPSR_0_NONE_OFFSET 0
#define PIO_PIO_GPSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_asr_0 */
#define PIO_PIO_ASR_0_NONE_OFFSET 0
#define PIO_PIO_ASR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_bsr_0 */
#define PIO_PIO_BSR_0_NONE_OFFSET 0
#define PIO_PIO_BSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_csr_0 */
#define PIO_PIO_CSR_0_NONE_OFFSET 0
#define PIO_PIO_CSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_dsr_0 */
#define PIO_PIO_DSR_0_NONE_OFFSET 0
#define PIO_PIO_DSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_mselsr_0 */
#define PIO_PIO_MSELSR_0_NONE_OFFSET 0
#define PIO_PIO_MSELSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_lselsr_0 */
#define PIO_PIO_LSELSR_0_NONE_OFFSET 0
#define PIO_PIO_LSELSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdser_0 */
#define PIO_PIO_MDSER_0_NONE_OFFSET 0
#define PIO_PIO_MDSER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdsdr_0 */
#define PIO_PIO_MDSDR_0_NONE_OFFSET 0
#define PIO_PIO_MDSDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdssr_0 */
#define PIO_PIO_MDSSR_0_NONE_OFFSET 0
#define PIO_PIO_MDSSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_ldser_0 */
#define PIO_PIO_LDSER_0_NONE_OFFSET 0
#define PIO_PIO_LDSER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_ldsdr_0 */
#define PIO_PIO_LDSDR_0_NONE_OFFSET 0
#define PIO_PIO_LDSDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_ldssr_0 */
#define PIO_PIO_LDSSR_0_NONE_OFFSET 0
#define PIO_PIO_LDSSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_srer_0 */
#define PIO_PIO_SRER_0_NONE_OFFSET 0
#define PIO_PIO_SRER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_srdr_0 */
#define PIO_PIO_SRDR_0_NONE_OFFSET 0
#define PIO_PIO_SRDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_srsr_0 */
#define PIO_PIO_SRSR_0_NONE_OFFSET 0
#define PIO_PIO_SRSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pher_0 */
#define PIO_PIO_PHER_0_NONE_OFFSET 0
#define PIO_PIO_PHER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_phdr_0 */
#define PIO_PIO_PHDR_0_NONE_OFFSET 0
#define PIO_PIO_PHDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_phsr_0 */
#define PIO_PIO_PHSR_0_NONE_OFFSET 0
#define PIO_PIO_PHSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pler_0 */
#define PIO_PIO_PLER_0_NONE_OFFSET 0
#define PIO_PIO_PLER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pldr_0 */
#define PIO_PIO_PLDR_0_NONE_OFFSET 0
#define PIO_PIO_PLDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_plsr_0 */
#define PIO_PIO_PLSR_0_NONE_OFFSET 0
#define PIO_PIO_PLSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_ster_0 */
#define PIO_PIO_STER_0_NONE_OFFSET 0
#define PIO_PIO_STER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_stdr_0 */
#define PIO_PIO_STDR_0_NONE_OFFSET 0
#define PIO_PIO_STDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_stsr_0 */
#define PIO_PIO_STSR_0_NONE_OFFSET 0
#define PIO_PIO_STSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_iner_0 */
#define PIO_PIO_INER_0_NONE_OFFSET 0
#define PIO_PIO_INER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_indr_0 */
#define PIO_PIO_INDR_0_NONE_OFFSET 0
#define PIO_PIO_INDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_insr_0 */
#define PIO_PIO_INSR_0_NONE_OFFSET 0
#define PIO_PIO_INSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_percper_0 */
#define PIO_PIO_PERCPER_0_NONE_OFFSET 0
#define PIO_PIO_PERCPER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_percpdr_0 */
#define PIO_PIO_PERCPDR_0_NONE_OFFSET 0
#define PIO_PIO_PERCPDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_percpsr_0 */
#define PIO_PIO_PERCPSR_0_NONE_OFFSET 0
#define PIO_PIO_PERCPSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_hwobssr_0 */
#define PIO_PIO_HWOBSSR_0_NONE_OFFSET 0
#define PIO_PIO_HWOBSSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_if2sr_0 */
#define PIO_PIO_IF2SR_0_NONE_OFFSET 0
#define PIO_PIO_IF2SR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_if3sr_0 */
#define PIO_PIO_IF3SR_0_NONE_OFFSET 0
#define PIO_PIO_IF3SR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_ever_0 */
#define PIO_PIO_EVER_0_NONE_OFFSET 0
#define PIO_PIO_EVER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_evdr_0 */
#define PIO_PIO_EVDR_0_NONE_OFFSET 0
#define PIO_PIO_EVDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_evsr_0 */
#define PIO_PIO_EVSR_0_NONE_OFFSET 0
#define PIO_PIO_EVSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_evter_0 */
#define PIO_PIO_EVTER_0_NONE_OFFSET 0
#define PIO_PIO_EVTER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_evtdr_0 */
#define PIO_PIO_EVTDR_0_NONE_OFFSET 0
#define PIO_PIO_EVTDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_evtsr_0 */
#define PIO_PIO_EVTSR_0_NONE_OFFSET 0
#define PIO_PIO_EVTSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_deevsr_0 */
#define PIO_PIO_DEEVSR_0_NONE_OFFSET 0
#define PIO_PIO_DEEVSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wser_0 */
#define PIO_PIO_WSER_0_NONE_OFFSET 0
#define PIO_PIO_WSER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wsdr_0 */
#define PIO_PIO_WSDR_0_NONE_OFFSET 0
#define PIO_PIO_WSDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wsmr_0 */
#define PIO_PIO_WSMR_0_NONE_OFFSET 0
#define PIO_PIO_WSMR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wssr_0 */
#define PIO_PIO_WSSR_0_NONE_OFFSET 0
#define PIO_PIO_WSSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwser_0 */
#define PIO_PIO_PWSER_0_NONE_OFFSET 0
#define PIO_PIO_PWSER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwsdr_0 */
#define PIO_PIO_PWSDR_0_NONE_OFFSET 0
#define PIO_PIO_PWSDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwsmr_0 */
#define PIO_PIO_PWSMR_0_NONE_OFFSET 0
#define PIO_PIO_PWSMR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwssr_0 */
#define PIO_PIO_PWSSR_0_NONE_OFFSET 0
#define PIO_PIO_PWSSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_weer_0 */
#define PIO_PIO_WEER_0_NONE_OFFSET 0
#define PIO_PIO_WEER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wedr_0 */
#define PIO_PIO_WEDR_0_NONE_OFFSET 0
#define PIO_PIO_WEDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wesr_0 */
#define PIO_PIO_WESR_0_NONE_OFFSET 0
#define PIO_PIO_WESR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wdsr_0 */
#define PIO_PIO_WDSR_0_NONE_OFFSET 0
#define PIO_PIO_WDSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_weter_0 */
#define PIO_PIO_WETER_0_NONE_OFFSET 0
#define PIO_PIO_WETER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wetdr_0 */
#define PIO_PIO_WETDR_0_NONE_OFFSET 0
#define PIO_PIO_WETDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wetsr_0 */
#define PIO_PIO_WETSR_0_NONE_OFFSET 0
#define PIO_PIO_WETSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wedesr_0 */
#define PIO_PIO_WEDESR_0_NONE_OFFSET 0
#define PIO_PIO_WEDESR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pweer_0 */
#define PIO_PIO_PWEER_0_NONE_OFFSET 0
#define PIO_PIO_PWEER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwedr_0 */
#define PIO_PIO_PWEDR_0_NONE_OFFSET 0
#define PIO_PIO_PWEDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwesr_0 */
#define PIO_PIO_PWESR_0_NONE_OFFSET 0
#define PIO_PIO_PWESR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pweter_0 */
#define PIO_PIO_PWETER_0_NONE_OFFSET 0
#define PIO_PIO_PWETER_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwetdr_0 */
#define PIO_PIO_PWETDR_0_NONE_OFFSET 0
#define PIO_PIO_PWETDR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwetsr_0 */
#define PIO_PIO_PWETSR_0_NONE_OFFSET 0
#define PIO_PIO_PWETSR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwedesr_0 */
#define PIO_PIO_PWEDESR_0_NONE_OFFSET 0
#define PIO_PIO_PWEDESR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur0sr_0 */
#define PIO_PIO_WUR0SR_0_NONE_OFFSET 0
#define PIO_PIO_WUR0SR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur1sr_0 */
#define PIO_PIO_WUR1SR_0_NONE_OFFSET 0
#define PIO_PIO_WUR1SR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur2sr_0 */
#define PIO_PIO_WUR2SR_0_NONE_OFFSET 0
#define PIO_PIO_WUR2SR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur3sr_0 */
#define PIO_PIO_WUR3SR_0_NONE_OFFSET 0
#define PIO_PIO_WUR3SR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur4sr_0 */
#define PIO_PIO_WUR4SR_0_NONE_OFFSET 0
#define PIO_PIO_WUR4SR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur5sr_0 */
#define PIO_PIO_WUR5SR_0_NONE_OFFSET 0
#define PIO_PIO_WUR5SR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur6sr_0 */
#define PIO_PIO_WUR6SR_0_NONE_OFFSET 0
#define PIO_PIO_WUR6SR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur7sr_0 */
#define PIO_PIO_WUR7SR_0_NONE_OFFSET 0
#define PIO_PIO_WUR7SR_0_NONE_SIZE 32

/** bit field defines for pio_s#pio_per_1 */
#define PIO_PIO_PER_1_NONE_OFFSET 0
#define PIO_PIO_PER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pdr_1 */
#define PIO_PIO_PDR_1_NONE_OFFSET 0
#define PIO_PIO_PDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_psr_1 */
#define PIO_PIO_PSR_1_NONE_OFFSET 0
#define PIO_PIO_PSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_oer_1 */
#define PIO_PIO_OER_1_NONE_OFFSET 0
#define PIO_PIO_OER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_odr_1 */
#define PIO_PIO_ODR_1_NONE_OFFSET 0
#define PIO_PIO_ODR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_osr_1 */
#define PIO_PIO_OSR_1_NONE_OFFSET 0
#define PIO_PIO_OSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_posr_1 */
#define PIO_PIO_POSR_1_NONE_OFFSET 0
#define PIO_PIO_POSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_ifer_1 */
#define PIO_PIO_IFER_1_NONE_OFFSET 0
#define PIO_PIO_IFER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_ifdr_1 */
#define PIO_PIO_IFDR_1_NONE_OFFSET 0
#define PIO_PIO_IFDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_ifsr_1 */
#define PIO_PIO_IFSR_1_NONE_OFFSET 0
#define PIO_PIO_IFSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_sodr_1 */
#define PIO_PIO_SODR_1_NONE_OFFSET 0
#define PIO_PIO_SODR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_codr_1 */
#define PIO_PIO_CODR_1_NONE_OFFSET 0
#define PIO_PIO_CODR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_odsr_1 */
#define PIO_PIO_ODSR_1_NONE_OFFSET 0
#define PIO_PIO_ODSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pdsr_1 */
#define PIO_PIO_PDSR_1_NONE_OFFSET 0
#define PIO_PIO_PDSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_ier1_1 */
#define PIO_PIO_IER1_1_NONE_OFFSET 0
#define PIO_PIO_IER1_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_idr1_1 */
#define PIO_PIO_IDR1_1_NONE_OFFSET 0
#define PIO_PIO_IDR1_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_imr1_1 */
#define PIO_PIO_IMR1_1_NONE_OFFSET 0
#define PIO_PIO_IMR1_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_ier2_1 */
#define PIO_PIO_IER2_1_NONE_OFFSET 0
#define PIO_PIO_IER2_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_idr2_1 */
#define PIO_PIO_IDR2_1_NONE_OFFSET 0
#define PIO_PIO_IDR2_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_imr2_1 */
#define PIO_PIO_IMR2_1_NONE_OFFSET 0
#define PIO_PIO_IMR2_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_isr_1 */
#define PIO_PIO_ISR_1_NONE_OFFSET 0
#define PIO_PIO_ISR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_mder_1 */
#define PIO_PIO_MDER_1_NONE_OFFSET 0
#define PIO_PIO_MDER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_mddr_1 */
#define PIO_PIO_MDDR_1_NONE_OFFSET 0
#define PIO_PIO_MDDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdsr_1 */
#define PIO_PIO_MDSR_1_NONE_OFFSET 0
#define PIO_PIO_MDSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_ckin_1 */
#define PIO_PIO_CKIN_1_NONE_OFFSET 0
#define PIO_PIO_CKIN_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_gpsr_1 */
#define PIO_PIO_GPSR_1_NONE_OFFSET 0
#define PIO_PIO_GPSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_asr_1 */
#define PIO_PIO_ASR_1_NONE_OFFSET 0
#define PIO_PIO_ASR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_bsr_1 */
#define PIO_PIO_BSR_1_NONE_OFFSET 0
#define PIO_PIO_BSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_csr_1 */
#define PIO_PIO_CSR_1_NONE_OFFSET 0
#define PIO_PIO_CSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_dsr_1 */
#define PIO_PIO_DSR_1_NONE_OFFSET 0
#define PIO_PIO_DSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_mselsr_1 */
#define PIO_PIO_MSELSR_1_NONE_OFFSET 0
#define PIO_PIO_MSELSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_lselsr_1 */
#define PIO_PIO_LSELSR_1_NONE_OFFSET 0
#define PIO_PIO_LSELSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdser_1 */
#define PIO_PIO_MDSER_1_NONE_OFFSET 0
#define PIO_PIO_MDSER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdsdr_1 */
#define PIO_PIO_MDSDR_1_NONE_OFFSET 0
#define PIO_PIO_MDSDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdssr_1 */
#define PIO_PIO_MDSSR_1_NONE_OFFSET 0
#define PIO_PIO_MDSSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_ldser_1 */
#define PIO_PIO_LDSER_1_NONE_OFFSET 0
#define PIO_PIO_LDSER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_ldsdr_1 */
#define PIO_PIO_LDSDR_1_NONE_OFFSET 0
#define PIO_PIO_LDSDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_ldssr_1 */
#define PIO_PIO_LDSSR_1_NONE_OFFSET 0
#define PIO_PIO_LDSSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_srer_1 */
#define PIO_PIO_SRER_1_NONE_OFFSET 0
#define PIO_PIO_SRER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_srdr_1 */
#define PIO_PIO_SRDR_1_NONE_OFFSET 0
#define PIO_PIO_SRDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_srsr_1 */
#define PIO_PIO_SRSR_1_NONE_OFFSET 0
#define PIO_PIO_SRSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pher_1 */
#define PIO_PIO_PHER_1_NONE_OFFSET 0
#define PIO_PIO_PHER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_phdr_1 */
#define PIO_PIO_PHDR_1_NONE_OFFSET 0
#define PIO_PIO_PHDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_phsr_1 */
#define PIO_PIO_PHSR_1_NONE_OFFSET 0
#define PIO_PIO_PHSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pler_1 */
#define PIO_PIO_PLER_1_NONE_OFFSET 0
#define PIO_PIO_PLER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pldr_1 */
#define PIO_PIO_PLDR_1_NONE_OFFSET 0
#define PIO_PIO_PLDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_plsr_1 */
#define PIO_PIO_PLSR_1_NONE_OFFSET 0
#define PIO_PIO_PLSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_ster_1 */
#define PIO_PIO_STER_1_NONE_OFFSET 0
#define PIO_PIO_STER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_stdr_1 */
#define PIO_PIO_STDR_1_NONE_OFFSET 0
#define PIO_PIO_STDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_stsr_1 */
#define PIO_PIO_STSR_1_NONE_OFFSET 0
#define PIO_PIO_STSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_iner_1 */
#define PIO_PIO_INER_1_NONE_OFFSET 0
#define PIO_PIO_INER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_indr_1 */
#define PIO_PIO_INDR_1_NONE_OFFSET 0
#define PIO_PIO_INDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_insr_1 */
#define PIO_PIO_INSR_1_NONE_OFFSET 0
#define PIO_PIO_INSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_percper_1 */
#define PIO_PIO_PERCPER_1_NONE_OFFSET 0
#define PIO_PIO_PERCPER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_percpdr_1 */
#define PIO_PIO_PERCPDR_1_NONE_OFFSET 0
#define PIO_PIO_PERCPDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_percpsr_1 */
#define PIO_PIO_PERCPSR_1_NONE_OFFSET 0
#define PIO_PIO_PERCPSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_hwobssr_1 */
#define PIO_PIO_HWOBSSR_1_NONE_OFFSET 0
#define PIO_PIO_HWOBSSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_if2sr_1 */
#define PIO_PIO_IF2SR_1_NONE_OFFSET 0
#define PIO_PIO_IF2SR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_if3sr_1 */
#define PIO_PIO_IF3SR_1_NONE_OFFSET 0
#define PIO_PIO_IF3SR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_ever_1 */
#define PIO_PIO_EVER_1_NONE_OFFSET 0
#define PIO_PIO_EVER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_evdr_1 */
#define PIO_PIO_EVDR_1_NONE_OFFSET 0
#define PIO_PIO_EVDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_evsr_1 */
#define PIO_PIO_EVSR_1_NONE_OFFSET 0
#define PIO_PIO_EVSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_evter_1 */
#define PIO_PIO_EVTER_1_NONE_OFFSET 0
#define PIO_PIO_EVTER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_evtdr_1 */
#define PIO_PIO_EVTDR_1_NONE_OFFSET 0
#define PIO_PIO_EVTDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_evtsr_1 */
#define PIO_PIO_EVTSR_1_NONE_OFFSET 0
#define PIO_PIO_EVTSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_deevsr_1 */
#define PIO_PIO_DEEVSR_1_NONE_OFFSET 0
#define PIO_PIO_DEEVSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wser_1 */
#define PIO_PIO_WSER_1_NONE_OFFSET 0
#define PIO_PIO_WSER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wsdr_1 */
#define PIO_PIO_WSDR_1_NONE_OFFSET 0
#define PIO_PIO_WSDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wsmr_1 */
#define PIO_PIO_WSMR_1_NONE_OFFSET 0
#define PIO_PIO_WSMR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wssr_1 */
#define PIO_PIO_WSSR_1_NONE_OFFSET 0
#define PIO_PIO_WSSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwser_1 */
#define PIO_PIO_PWSER_1_NONE_OFFSET 0
#define PIO_PIO_PWSER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwsdr_1 */
#define PIO_PIO_PWSDR_1_NONE_OFFSET 0
#define PIO_PIO_PWSDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwsmr_1 */
#define PIO_PIO_PWSMR_1_NONE_OFFSET 0
#define PIO_PIO_PWSMR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwssr_1 */
#define PIO_PIO_PWSSR_1_NONE_OFFSET 0
#define PIO_PIO_PWSSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_weer_1 */
#define PIO_PIO_WEER_1_NONE_OFFSET 0
#define PIO_PIO_WEER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wedr_1 */
#define PIO_PIO_WEDR_1_NONE_OFFSET 0
#define PIO_PIO_WEDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wesr_1 */
#define PIO_PIO_WESR_1_NONE_OFFSET 0
#define PIO_PIO_WESR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wdsr_1 */
#define PIO_PIO_WDSR_1_NONE_OFFSET 0
#define PIO_PIO_WDSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_weter_1 */
#define PIO_PIO_WETER_1_NONE_OFFSET 0
#define PIO_PIO_WETER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wetdr_1 */
#define PIO_PIO_WETDR_1_NONE_OFFSET 0
#define PIO_PIO_WETDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wetsr_1 */
#define PIO_PIO_WETSR_1_NONE_OFFSET 0
#define PIO_PIO_WETSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wedesr_1 */
#define PIO_PIO_WEDESR_1_NONE_OFFSET 0
#define PIO_PIO_WEDESR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pweer_1 */
#define PIO_PIO_PWEER_1_NONE_OFFSET 0
#define PIO_PIO_PWEER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwedr_1 */
#define PIO_PIO_PWEDR_1_NONE_OFFSET 0
#define PIO_PIO_PWEDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwesr_1 */
#define PIO_PIO_PWESR_1_NONE_OFFSET 0
#define PIO_PIO_PWESR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pweter_1 */
#define PIO_PIO_PWETER_1_NONE_OFFSET 0
#define PIO_PIO_PWETER_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwetdr_1 */
#define PIO_PIO_PWETDR_1_NONE_OFFSET 0
#define PIO_PIO_PWETDR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwetsr_1 */
#define PIO_PIO_PWETSR_1_NONE_OFFSET 0
#define PIO_PIO_PWETSR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwedesr_1 */
#define PIO_PIO_PWEDESR_1_NONE_OFFSET 0
#define PIO_PIO_PWEDESR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur0sr_1 */
#define PIO_PIO_WUR0SR_1_NONE_OFFSET 0
#define PIO_PIO_WUR0SR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur1sr_1 */
#define PIO_PIO_WUR1SR_1_NONE_OFFSET 0
#define PIO_PIO_WUR1SR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur2sr_1 */
#define PIO_PIO_WUR2SR_1_NONE_OFFSET 0
#define PIO_PIO_WUR2SR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur3sr_1 */
#define PIO_PIO_WUR3SR_1_NONE_OFFSET 0
#define PIO_PIO_WUR3SR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur4sr_1 */
#define PIO_PIO_WUR4SR_1_NONE_OFFSET 0
#define PIO_PIO_WUR4SR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur5sr_1 */
#define PIO_PIO_WUR5SR_1_NONE_OFFSET 0
#define PIO_PIO_WUR5SR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur6sr_1 */
#define PIO_PIO_WUR6SR_1_NONE_OFFSET 0
#define PIO_PIO_WUR6SR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur7sr_1 */
#define PIO_PIO_WUR7SR_1_NONE_OFFSET 0
#define PIO_PIO_WUR7SR_1_NONE_SIZE 32

/** bit field defines for pio_s#pio_per_2 */
#define PIO_PIO_PER_2_NONE_OFFSET 0
#define PIO_PIO_PER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pdr_2 */
#define PIO_PIO_PDR_2_NONE_OFFSET 0
#define PIO_PIO_PDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_psr_2 */
#define PIO_PIO_PSR_2_NONE_OFFSET 0
#define PIO_PIO_PSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_oer_2 */
#define PIO_PIO_OER_2_NONE_OFFSET 0
#define PIO_PIO_OER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_odr_2 */
#define PIO_PIO_ODR_2_NONE_OFFSET 0
#define PIO_PIO_ODR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_osr_2 */
#define PIO_PIO_OSR_2_NONE_OFFSET 0
#define PIO_PIO_OSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_posr_2 */
#define PIO_PIO_POSR_2_NONE_OFFSET 0
#define PIO_PIO_POSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_ifer_2 */
#define PIO_PIO_IFER_2_NONE_OFFSET 0
#define PIO_PIO_IFER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_ifdr_2 */
#define PIO_PIO_IFDR_2_NONE_OFFSET 0
#define PIO_PIO_IFDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_ifsr_2 */
#define PIO_PIO_IFSR_2_NONE_OFFSET 0
#define PIO_PIO_IFSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_sodr_2 */
#define PIO_PIO_SODR_2_NONE_OFFSET 0
#define PIO_PIO_SODR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_codr_2 */
#define PIO_PIO_CODR_2_NONE_OFFSET 0
#define PIO_PIO_CODR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_odsr_2 */
#define PIO_PIO_ODSR_2_NONE_OFFSET 0
#define PIO_PIO_ODSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pdsr_2 */
#define PIO_PIO_PDSR_2_NONE_OFFSET 0
#define PIO_PIO_PDSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_ier1_2 */
#define PIO_PIO_IER1_2_NONE_OFFSET 0
#define PIO_PIO_IER1_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_idr1_2 */
#define PIO_PIO_IDR1_2_NONE_OFFSET 0
#define PIO_PIO_IDR1_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_imr1_2 */
#define PIO_PIO_IMR1_2_NONE_OFFSET 0
#define PIO_PIO_IMR1_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_ier2_2 */
#define PIO_PIO_IER2_2_NONE_OFFSET 0
#define PIO_PIO_IER2_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_idr2_2 */
#define PIO_PIO_IDR2_2_NONE_OFFSET 0
#define PIO_PIO_IDR2_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_imr2_2 */
#define PIO_PIO_IMR2_2_NONE_OFFSET 0
#define PIO_PIO_IMR2_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_isr_2 */
#define PIO_PIO_ISR_2_NONE_OFFSET 0
#define PIO_PIO_ISR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_mder_2 */
#define PIO_PIO_MDER_2_NONE_OFFSET 0
#define PIO_PIO_MDER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_mddr_2 */
#define PIO_PIO_MDDR_2_NONE_OFFSET 0
#define PIO_PIO_MDDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdsr_2 */
#define PIO_PIO_MDSR_2_NONE_OFFSET 0
#define PIO_PIO_MDSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_ckin_2 */
#define PIO_PIO_CKIN_2_NONE_OFFSET 0
#define PIO_PIO_CKIN_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_gpsr_2 */
#define PIO_PIO_GPSR_2_NONE_OFFSET 0
#define PIO_PIO_GPSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_asr_2 */
#define PIO_PIO_ASR_2_NONE_OFFSET 0
#define PIO_PIO_ASR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_bsr_2 */
#define PIO_PIO_BSR_2_NONE_OFFSET 0
#define PIO_PIO_BSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_csr_2 */
#define PIO_PIO_CSR_2_NONE_OFFSET 0
#define PIO_PIO_CSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_dsr_2 */
#define PIO_PIO_DSR_2_NONE_OFFSET 0
#define PIO_PIO_DSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_mselsr_2 */
#define PIO_PIO_MSELSR_2_NONE_OFFSET 0
#define PIO_PIO_MSELSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_lselsr_2 */
#define PIO_PIO_LSELSR_2_NONE_OFFSET 0
#define PIO_PIO_LSELSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdser_2 */
#define PIO_PIO_MDSER_2_NONE_OFFSET 0
#define PIO_PIO_MDSER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdsdr_2 */
#define PIO_PIO_MDSDR_2_NONE_OFFSET 0
#define PIO_PIO_MDSDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_mdssr_2 */
#define PIO_PIO_MDSSR_2_NONE_OFFSET 0
#define PIO_PIO_MDSSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_ldser_2 */
#define PIO_PIO_LDSER_2_NONE_OFFSET 0
#define PIO_PIO_LDSER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_ldsdr_2 */
#define PIO_PIO_LDSDR_2_NONE_OFFSET 0
#define PIO_PIO_LDSDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_ldssr_2 */
#define PIO_PIO_LDSSR_2_NONE_OFFSET 0
#define PIO_PIO_LDSSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_srer_2 */
#define PIO_PIO_SRER_2_NONE_OFFSET 0
#define PIO_PIO_SRER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_srdr_2 */
#define PIO_PIO_SRDR_2_NONE_OFFSET 0
#define PIO_PIO_SRDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_srsr_2 */
#define PIO_PIO_SRSR_2_NONE_OFFSET 0
#define PIO_PIO_SRSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pher_2 */
#define PIO_PIO_PHER_2_NONE_OFFSET 0
#define PIO_PIO_PHER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_phdr_2 */
#define PIO_PIO_PHDR_2_NONE_OFFSET 0
#define PIO_PIO_PHDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_phsr_2 */
#define PIO_PIO_PHSR_2_NONE_OFFSET 0
#define PIO_PIO_PHSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pler_2 */
#define PIO_PIO_PLER_2_NONE_OFFSET 0
#define PIO_PIO_PLER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pldr_2 */
#define PIO_PIO_PLDR_2_NONE_OFFSET 0
#define PIO_PIO_PLDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_plsr_2 */
#define PIO_PIO_PLSR_2_NONE_OFFSET 0
#define PIO_PIO_PLSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_ster_2 */
#define PIO_PIO_STER_2_NONE_OFFSET 0
#define PIO_PIO_STER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_stdr_2 */
#define PIO_PIO_STDR_2_NONE_OFFSET 0
#define PIO_PIO_STDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_stsr_2 */
#define PIO_PIO_STSR_2_NONE_OFFSET 0
#define PIO_PIO_STSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_iner_2 */
#define PIO_PIO_INER_2_NONE_OFFSET 0
#define PIO_PIO_INER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_indr_2 */
#define PIO_PIO_INDR_2_NONE_OFFSET 0
#define PIO_PIO_INDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_insr_2 */
#define PIO_PIO_INSR_2_NONE_OFFSET 0
#define PIO_PIO_INSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_percper_2 */
#define PIO_PIO_PERCPER_2_NONE_OFFSET 0
#define PIO_PIO_PERCPER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_percpdr_2 */
#define PIO_PIO_PERCPDR_2_NONE_OFFSET 0
#define PIO_PIO_PERCPDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_percpsr_2 */
#define PIO_PIO_PERCPSR_2_NONE_OFFSET 0
#define PIO_PIO_PERCPSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_hwobssr_2 */
#define PIO_PIO_HWOBSSR_2_NONE_OFFSET 0
#define PIO_PIO_HWOBSSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_if2sr_2 */
#define PIO_PIO_IF2SR_2_NONE_OFFSET 0
#define PIO_PIO_IF2SR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_if3sr_2 */
#define PIO_PIO_IF3SR_2_NONE_OFFSET 0
#define PIO_PIO_IF3SR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_ever_2 */
#define PIO_PIO_EVER_2_NONE_OFFSET 0
#define PIO_PIO_EVER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_evdr_2 */
#define PIO_PIO_EVDR_2_NONE_OFFSET 0
#define PIO_PIO_EVDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_evsr_2 */
#define PIO_PIO_EVSR_2_NONE_OFFSET 0
#define PIO_PIO_EVSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_evter_2 */
#define PIO_PIO_EVTER_2_NONE_OFFSET 0
#define PIO_PIO_EVTER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_evtdr_2 */
#define PIO_PIO_EVTDR_2_NONE_OFFSET 0
#define PIO_PIO_EVTDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_evtsr_2 */
#define PIO_PIO_EVTSR_2_NONE_OFFSET 0
#define PIO_PIO_EVTSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_deevsr_2 */
#define PIO_PIO_DEEVSR_2_NONE_OFFSET 0
#define PIO_PIO_DEEVSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wser_2 */
#define PIO_PIO_WSER_2_NONE_OFFSET 0
#define PIO_PIO_WSER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wsdr_2 */
#define PIO_PIO_WSDR_2_NONE_OFFSET 0
#define PIO_PIO_WSDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wsmr_2 */
#define PIO_PIO_WSMR_2_NONE_OFFSET 0
#define PIO_PIO_WSMR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wssr_2 */
#define PIO_PIO_WSSR_2_NONE_OFFSET 0
#define PIO_PIO_WSSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwser_2 */
#define PIO_PIO_PWSER_2_NONE_OFFSET 0
#define PIO_PIO_PWSER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwsdr_2 */
#define PIO_PIO_PWSDR_2_NONE_OFFSET 0
#define PIO_PIO_PWSDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwsmr_2 */
#define PIO_PIO_PWSMR_2_NONE_OFFSET 0
#define PIO_PIO_PWSMR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwssr_2 */
#define PIO_PIO_PWSSR_2_NONE_OFFSET 0
#define PIO_PIO_PWSSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_weer_2 */
#define PIO_PIO_WEER_2_NONE_OFFSET 0
#define PIO_PIO_WEER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wedr_2 */
#define PIO_PIO_WEDR_2_NONE_OFFSET 0
#define PIO_PIO_WEDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wesr_2 */
#define PIO_PIO_WESR_2_NONE_OFFSET 0
#define PIO_PIO_WESR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wdsr_2 */
#define PIO_PIO_WDSR_2_NONE_OFFSET 0
#define PIO_PIO_WDSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_weter_2 */
#define PIO_PIO_WETER_2_NONE_OFFSET 0
#define PIO_PIO_WETER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wetdr_2 */
#define PIO_PIO_WETDR_2_NONE_OFFSET 0
#define PIO_PIO_WETDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wetsr_2 */
#define PIO_PIO_WETSR_2_NONE_OFFSET 0
#define PIO_PIO_WETSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wedesr_2 */
#define PIO_PIO_WEDESR_2_NONE_OFFSET 0
#define PIO_PIO_WEDESR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pweer_2 */
#define PIO_PIO_PWEER_2_NONE_OFFSET 0
#define PIO_PIO_PWEER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwedr_2 */
#define PIO_PIO_PWEDR_2_NONE_OFFSET 0
#define PIO_PIO_PWEDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwesr_2 */
#define PIO_PIO_PWESR_2_NONE_OFFSET 0
#define PIO_PIO_PWESR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pweter_2 */
#define PIO_PIO_PWETER_2_NONE_OFFSET 0
#define PIO_PIO_PWETER_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwetdr_2 */
#define PIO_PIO_PWETDR_2_NONE_OFFSET 0
#define PIO_PIO_PWETDR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwetsr_2 */
#define PIO_PIO_PWETSR_2_NONE_OFFSET 0
#define PIO_PIO_PWETSR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_pwedesr_2 */
#define PIO_PIO_PWEDESR_2_NONE_OFFSET 0
#define PIO_PIO_PWEDESR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur0sr_2 */
#define PIO_PIO_WUR0SR_2_NONE_OFFSET 0
#define PIO_PIO_WUR0SR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur1sr_2 */
#define PIO_PIO_WUR1SR_2_NONE_OFFSET 0
#define PIO_PIO_WUR1SR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur2sr_2 */
#define PIO_PIO_WUR2SR_2_NONE_OFFSET 0
#define PIO_PIO_WUR2SR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur3sr_2 */
#define PIO_PIO_WUR3SR_2_NONE_OFFSET 0
#define PIO_PIO_WUR3SR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur4sr_2 */
#define PIO_PIO_WUR4SR_2_NONE_OFFSET 0
#define PIO_PIO_WUR4SR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur5sr_2 */
#define PIO_PIO_WUR5SR_2_NONE_OFFSET 0
#define PIO_PIO_WUR5SR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur6sr_2 */
#define PIO_PIO_WUR6SR_2_NONE_OFFSET 0
#define PIO_PIO_WUR6SR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_wur7sr_2 */
#define PIO_PIO_WUR7SR_2_NONE_OFFSET 0
#define PIO_PIO_WUR7SR_2_NONE_SIZE 32

/** bit field defines for pio_s#pio_per_3 */
#define PIO_PIO_PER_3_NONE_OFFSET 0
#define PIO_PIO_PER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pdr_3 */
#define PIO_PIO_PDR_3_NONE_OFFSET 0
#define PIO_PIO_PDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_psr_3 */
#define PIO_PIO_PSR_3_NONE_OFFSET 0
#define PIO_PIO_PSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_oer_3 */
#define PIO_PIO_OER_3_NONE_OFFSET 0
#define PIO_PIO_OER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_odr_3 */
#define PIO_PIO_ODR_3_NONE_OFFSET 0
#define PIO_PIO_ODR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_osr_3 */
#define PIO_PIO_OSR_3_NONE_OFFSET 0
#define PIO_PIO_OSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_posr_3 */
#define PIO_PIO_POSR_3_NONE_OFFSET 0
#define PIO_PIO_POSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_ifer_3 */
#define PIO_PIO_IFER_3_NONE_OFFSET 0
#define PIO_PIO_IFER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_ifdr_3 */
#define PIO_PIO_IFDR_3_NONE_OFFSET 0
#define PIO_PIO_IFDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_ifsr_3 */
#define PIO_PIO_IFSR_3_NONE_OFFSET 0
#define PIO_PIO_IFSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_sodr_3 */
#define PIO_PIO_SODR_3_NONE_OFFSET 0
#define PIO_PIO_SODR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_codr_3 */
#define PIO_PIO_CODR_3_NONE_OFFSET 0
#define PIO_PIO_CODR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_odsr_3 */
#define PIO_PIO_ODSR_3_NONE_OFFSET 0
#define PIO_PIO_ODSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pdsr_3 */
#define PIO_PIO_PDSR_3_NONE_OFFSET 0
#define PIO_PIO_PDSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_ier1_3 */
#define PIO_PIO_IER1_3_NONE_OFFSET 0
#define PIO_PIO_IER1_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_idr1_3 */
#define PIO_PIO_IDR1_3_NONE_OFFSET 0
#define PIO_PIO_IDR1_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_imr1_3 */
#define PIO_PIO_IMR1_3_NONE_OFFSET 0
#define PIO_PIO_IMR1_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_ier2_3 */
#define PIO_PIO_IER2_3_NONE_OFFSET 0
#define PIO_PIO_IER2_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_idr2_3 */
#define PIO_PIO_IDR2_3_NONE_OFFSET 0
#define PIO_PIO_IDR2_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_imr2_3 */
#define PIO_PIO_IMR2_3_NONE_OFFSET 0
#define PIO_PIO_IMR2_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_isr_3 */
#define PIO_PIO_ISR_3_NONE_OFFSET 0
#define PIO_PIO_ISR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_mder_3 */
#define PIO_PIO_MDER_3_NONE_OFFSET 0
#define PIO_PIO_MDER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_mddr_3 */
#define PIO_PIO_MDDR_3_NONE_OFFSET 0
#define PIO_PIO_MDDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_mdsr_3 */
#define PIO_PIO_MDSR_3_NONE_OFFSET 0
#define PIO_PIO_MDSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_ckin_3 */
#define PIO_PIO_CKIN_3_NONE_OFFSET 0
#define PIO_PIO_CKIN_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_gpsr_3 */
#define PIO_PIO_GPSR_3_NONE_OFFSET 0
#define PIO_PIO_GPSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_asr_3 */
#define PIO_PIO_ASR_3_NONE_OFFSET 0
#define PIO_PIO_ASR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_bsr_3 */
#define PIO_PIO_BSR_3_NONE_OFFSET 0
#define PIO_PIO_BSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_csr_3 */
#define PIO_PIO_CSR_3_NONE_OFFSET 0
#define PIO_PIO_CSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_dsr_3 */
#define PIO_PIO_DSR_3_NONE_OFFSET 0
#define PIO_PIO_DSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_mselsr_3 */
#define PIO_PIO_MSELSR_3_NONE_OFFSET 0
#define PIO_PIO_MSELSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_lselsr_3 */
#define PIO_PIO_LSELSR_3_NONE_OFFSET 0
#define PIO_PIO_LSELSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_mdser_3 */
#define PIO_PIO_MDSER_3_NONE_OFFSET 0
#define PIO_PIO_MDSER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_mdsdr_3 */
#define PIO_PIO_MDSDR_3_NONE_OFFSET 0
#define PIO_PIO_MDSDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_mdssr_3 */
#define PIO_PIO_MDSSR_3_NONE_OFFSET 0
#define PIO_PIO_MDSSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_ldser_3 */
#define PIO_PIO_LDSER_3_NONE_OFFSET 0
#define PIO_PIO_LDSER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_ldsdr_3 */
#define PIO_PIO_LDSDR_3_NONE_OFFSET 0
#define PIO_PIO_LDSDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_ldssr_3 */
#define PIO_PIO_LDSSR_3_NONE_OFFSET 0
#define PIO_PIO_LDSSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_srer_3 */
#define PIO_PIO_SRER_3_NONE_OFFSET 0
#define PIO_PIO_SRER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_srdr_3 */
#define PIO_PIO_SRDR_3_NONE_OFFSET 0
#define PIO_PIO_SRDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_srsr_3 */
#define PIO_PIO_SRSR_3_NONE_OFFSET 0
#define PIO_PIO_SRSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pher_3 */
#define PIO_PIO_PHER_3_NONE_OFFSET 0
#define PIO_PIO_PHER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_phdr_3 */
#define PIO_PIO_PHDR_3_NONE_OFFSET 0
#define PIO_PIO_PHDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_phsr_3 */
#define PIO_PIO_PHSR_3_NONE_OFFSET 0
#define PIO_PIO_PHSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pler_3 */
#define PIO_PIO_PLER_3_NONE_OFFSET 0
#define PIO_PIO_PLER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pldr_3 */
#define PIO_PIO_PLDR_3_NONE_OFFSET 0
#define PIO_PIO_PLDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_plsr_3 */
#define PIO_PIO_PLSR_3_NONE_OFFSET 0
#define PIO_PIO_PLSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_ster_3 */
#define PIO_PIO_STER_3_NONE_OFFSET 0
#define PIO_PIO_STER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_stdr_3 */
#define PIO_PIO_STDR_3_NONE_OFFSET 0
#define PIO_PIO_STDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_stsr_3 */
#define PIO_PIO_STSR_3_NONE_OFFSET 0
#define PIO_PIO_STSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_iner_3 */
#define PIO_PIO_INER_3_NONE_OFFSET 0
#define PIO_PIO_INER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_indr_3 */
#define PIO_PIO_INDR_3_NONE_OFFSET 0
#define PIO_PIO_INDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_insr_3 */
#define PIO_PIO_INSR_3_NONE_OFFSET 0
#define PIO_PIO_INSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_percper_3 */
#define PIO_PIO_PERCPER_3_NONE_OFFSET 0
#define PIO_PIO_PERCPER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_percpdr_3 */
#define PIO_PIO_PERCPDR_3_NONE_OFFSET 0
#define PIO_PIO_PERCPDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_percpsr_3 */
#define PIO_PIO_PERCPSR_3_NONE_OFFSET 0
#define PIO_PIO_PERCPSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_hwobssr_3 */
#define PIO_PIO_HWOBSSR_3_NONE_OFFSET 0
#define PIO_PIO_HWOBSSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_if2sr_3 */
#define PIO_PIO_IF2SR_3_NONE_OFFSET 0
#define PIO_PIO_IF2SR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_if3sr_3 */
#define PIO_PIO_IF3SR_3_NONE_OFFSET 0
#define PIO_PIO_IF3SR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_ever_3 */
#define PIO_PIO_EVER_3_NONE_OFFSET 0
#define PIO_PIO_EVER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_evdr_3 */
#define PIO_PIO_EVDR_3_NONE_OFFSET 0
#define PIO_PIO_EVDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_evsr_3 */
#define PIO_PIO_EVSR_3_NONE_OFFSET 0
#define PIO_PIO_EVSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_evter_3 */
#define PIO_PIO_EVTER_3_NONE_OFFSET 0
#define PIO_PIO_EVTER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_evtdr_3 */
#define PIO_PIO_EVTDR_3_NONE_OFFSET 0
#define PIO_PIO_EVTDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_evtsr_3 */
#define PIO_PIO_EVTSR_3_NONE_OFFSET 0
#define PIO_PIO_EVTSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_deevsr_3 */
#define PIO_PIO_DEEVSR_3_NONE_OFFSET 0
#define PIO_PIO_DEEVSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wser_3 */
#define PIO_PIO_WSER_3_NONE_OFFSET 0
#define PIO_PIO_WSER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wsdr_3 */
#define PIO_PIO_WSDR_3_NONE_OFFSET 0
#define PIO_PIO_WSDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wsmr_3 */
#define PIO_PIO_WSMR_3_NONE_OFFSET 0
#define PIO_PIO_WSMR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wssr_3 */
#define PIO_PIO_WSSR_3_NONE_OFFSET 0
#define PIO_PIO_WSSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pwser_3 */
#define PIO_PIO_PWSER_3_NONE_OFFSET 0
#define PIO_PIO_PWSER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pwsdr_3 */
#define PIO_PIO_PWSDR_3_NONE_OFFSET 0
#define PIO_PIO_PWSDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pwsmr_3 */
#define PIO_PIO_PWSMR_3_NONE_OFFSET 0
#define PIO_PIO_PWSMR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pwssr_3 */
#define PIO_PIO_PWSSR_3_NONE_OFFSET 0
#define PIO_PIO_PWSSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_weer_3 */
#define PIO_PIO_WEER_3_NONE_OFFSET 0
#define PIO_PIO_WEER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wedr_3 */
#define PIO_PIO_WEDR_3_NONE_OFFSET 0
#define PIO_PIO_WEDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wesr_3 */
#define PIO_PIO_WESR_3_NONE_OFFSET 0
#define PIO_PIO_WESR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wdsr_3 */
#define PIO_PIO_WDSR_3_NONE_OFFSET 0
#define PIO_PIO_WDSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_weter_3 */
#define PIO_PIO_WETER_3_NONE_OFFSET 0
#define PIO_PIO_WETER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wetdr_3 */
#define PIO_PIO_WETDR_3_NONE_OFFSET 0
#define PIO_PIO_WETDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wetsr_3 */
#define PIO_PIO_WETSR_3_NONE_OFFSET 0
#define PIO_PIO_WETSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wedesr_3 */
#define PIO_PIO_WEDESR_3_NONE_OFFSET 0
#define PIO_PIO_WEDESR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pweer_3 */
#define PIO_PIO_PWEER_3_NONE_OFFSET 0
#define PIO_PIO_PWEER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pwedr_3 */
#define PIO_PIO_PWEDR_3_NONE_OFFSET 0
#define PIO_PIO_PWEDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pwesr_3 */
#define PIO_PIO_PWESR_3_NONE_OFFSET 0
#define PIO_PIO_PWESR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pweter_3 */
#define PIO_PIO_PWETER_3_NONE_OFFSET 0
#define PIO_PIO_PWETER_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pwetdr_3 */
#define PIO_PIO_PWETDR_3_NONE_OFFSET 0
#define PIO_PIO_PWETDR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pwetsr_3 */
#define PIO_PIO_PWETSR_3_NONE_OFFSET 0
#define PIO_PIO_PWETSR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_pwedesr_3 */
#define PIO_PIO_PWEDESR_3_NONE_OFFSET 0
#define PIO_PIO_PWEDESR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wur0sr_3 */
#define PIO_PIO_WUR0SR_3_NONE_OFFSET 0
#define PIO_PIO_WUR0SR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wur1sr_3 */
#define PIO_PIO_WUR1SR_3_NONE_OFFSET 0
#define PIO_PIO_WUR1SR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wur2sr_3 */
#define PIO_PIO_WUR2SR_3_NONE_OFFSET 0
#define PIO_PIO_WUR2SR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wur3sr_3 */
#define PIO_PIO_WUR3SR_3_NONE_OFFSET 0
#define PIO_PIO_WUR3SR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wur4sr_3 */
#define PIO_PIO_WUR4SR_3_NONE_OFFSET 0
#define PIO_PIO_WUR4SR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wur5sr_3 */
#define PIO_PIO_WUR5SR_3_NONE_OFFSET 0
#define PIO_PIO_WUR5SR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wur6sr_3 */
#define PIO_PIO_WUR6SR_3_NONE_OFFSET 0
#define PIO_PIO_WUR6SR_3_NONE_SIZE 22

/** bit field defines for pio_s#pio_wur7sr_3 */
#define PIO_PIO_WUR7SR_3_NONE_OFFSET 0
#define PIO_PIO_WUR7SR_3_NONE_SIZE 22

/* EOF pio.h */
#endif
