#ifndef INCLUDED_ARM_CRYPTO310
#define INCLUDED_ARM_CRYPTO310
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** arm_crypto310 HAL Spreadsheet version number */
#define ARM_CRYPTO310_HAL_VERSION 3

/**  ARM CryptoCell 310 Register.  Taken from ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual v1.2, dx_crys_kernel.h and dx_rng.h.  Please note that the 0x1000 offset seen in the documentation shall be added as part of the base address.
*/
struct arm_crypto310_s {
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x000, read-write */
   uint32_t memory_map0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x004, read-write */
   uint32_t memory_map1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x008, read-write */
   uint32_t memory_map2;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x00C, read-write */
   uint32_t memory_map3;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x010, read-write */
   uint32_t memory_map4;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x014, read-write */
   uint32_t memory_map5;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x018, read-write */
   uint32_t memory_map6;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x01C, read-write */
   uint32_t memory_map7;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x020, read-write */
   uint32_t memory_map8;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x024, read-write */
   uint32_t memory_map9;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x028, read-write */
   uint32_t memory_map10;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x02C, read-write */
   uint32_t memory_map11;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x030, read-write */
   uint32_t memory_map12;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x034, read-write */
   uint32_t memory_map13;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x038, read-write */
   uint32_t memory_map14;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x03C, read-write */
   uint32_t memory_map15;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x040, read-write */
   uint32_t memory_map16;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x044, read-write */
   uint32_t memory_map17;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x048, read-write */
   uint32_t memory_map18;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x04C, read-write */
   uint32_t memory_map19;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x050, read-write */
   uint32_t memory_map20;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x054, read-write */
   uint32_t memory_map21;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x058, read-write */
   uint32_t memory_map22;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x05C, read-write */
   uint32_t memory_map23;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x060, read-write */
   uint32_t memory_map24;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x064, read-write */
   uint32_t memory_map25;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x068, read-write */
   uint32_t memory_map26;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x06C, read-write */
   uint32_t memory_map27;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x070, read-write */
   uint32_t memory_map28;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x074, read-write */
   uint32_t memory_map29;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x078, read-write */
   uint32_t memory_map30;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x07C, read-write */
   uint32_t memory_map31;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x080, read-write */
   uint32_t opcode;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x084, read-write */
   uint32_t n_np_t0_t1_addr;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x088, read-only */
   uint32_t pka_status;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x08C, write-only */
   uint32_t pka_sw_reset;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x090, read-write */
   uint32_t pka_l0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x094, read-write */
   uint32_t pka_l1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x098, read-write */
   uint32_t pka_l2;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x09C, read-write */
   uint32_t pka_l3;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0A0, read-write */
   uint32_t pka_l4;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0A4, read-write */
   uint32_t pka_l5;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0A8, read-write */
   uint32_t pka_l6;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0AC, read-write */
   uint32_t pka_l7;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0B0, read-only */
   uint32_t pka_pipe_rdy;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0B4, read-only */
   uint32_t pka_done;
   /** Reserved space */
   uint8_t fill0[12];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0C4, read-only */
   uint32_t pka_version;
   /** Reserved space */
   uint8_t fill1[4];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0CC, read-write */
   uint32_t pka_msb_addr;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0D0, read-write */
   uint32_t pka_mon_read;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0D4, write-only */
   uint32_t pka_sram_addr;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0D8, write-only */
   uint32_t pka_sram_wdata;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0DC, read-only */
   uint32_t pka_sram_rdata;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0E0, write-only */
   uint32_t pka_sram_wr_clr;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0E4, write-only */
   uint32_t pka_sram_raddr;
   /** Reserved space */
   uint8_t fill2[8];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0F0, read-write */
   uint32_t pka_word_access;
   /** Reserved space */
   uint8_t fill3[4];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x0F8, write-only */
   uint32_t pka_buff_addr;
   /** Reserved space */
   uint8_t fill4[4];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x100, read-write */
   uint32_t rng_imr;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x104, read-only */
   uint32_t rng_isr;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x108, write-only */
   uint32_t rng_icr;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x10C, read-write */
   uint32_t rng_trng_config;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x110, read-only */
   uint32_t rng_trng_valid;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x114, read-only */
   uint32_t rng_ehr_data_0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x118, read-only */
   uint32_t rng_ehr_data_1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x11C, read-only */
   uint32_t rng_ehr_data_2;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x120, read-only */
   uint32_t rng_ehr_data_3;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x124, read-only */
   uint32_t rng_ehr_data_4;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x128, read-only */
   uint32_t rng_ehr_data_5;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x12C, read-write */
   uint32_t rng_rnd_source_enable;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x130, read-write */
   uint32_t rng_sample_cnt1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x134, read-write */
   uint32_t rng_autocorr_statistic;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x138, read-write */
   uint32_t rng_trng_debug_control;
   /** Reserved space */
   uint8_t fill5[4];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x140, write-only */
   uint32_t rng_sw_reset;
   /** Reserved space */
   uint8_t fill6[112];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x1B4, read-only */
   uint32_t rng_debug_en_input;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x1B8, read-only */
   uint32_t rng_busy;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x1BC, write-only */
   uint32_t rst_bits_counter;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x1C0, read-only */
   uint32_t rng_version;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x1C4, read-write */
   uint32_t rng_clk_enable;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x1C8, read-write */
   uint32_t rng_dma_enable;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x1CC, read-write */
   uint32_t rng_dma_src_mask;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x1D0, read-write */
   uint32_t rng_dma_sram_addr;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x1D4, read-write */
   uint32_t rng_dma_samples_num;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x1D8, read-write */
   uint32_t rng_watchdog_val;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x1DC, read-only */
   uint32_t rng_dma_status;
   /** Reserved space */
   uint8_t fill7[416];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x380, read-write */
   uint32_t chacha_control_reg;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x384, read-only */
   uint32_t chacha_version;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x388, write-only */
   uint32_t chacha_key0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x38C, write-only */
   uint32_t chacha_key1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x390, write-only */
   uint32_t chacha_key2;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x394, write-only */
   uint32_t chacha_key3;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x398, write-only */
   uint32_t chacha_key4;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x39C, write-only */
   uint32_t chacha_key5;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3A0, write-only */
   uint32_t chacha_key6;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3A4, write-only */
   uint32_t chacha_key7;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3A8, read-write */
   uint32_t chacha_iv_0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3AC, read-write */
   uint32_t chacha_iv_1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3B0, read-only */
   uint32_t chacha_busy;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3B4, read-only */
   uint32_t chacha_hw_flags;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3B8, read-write */
   uint32_t chacha_block_cnt_lsb;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3BC, read-write */
   uint32_t chacha_block_cnt_msb;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3C0, read-write */
   uint32_t chacha_sw_reset;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3C4, read-only */
   uint32_t chacha_for_poly_key0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3C8, read-only */
   uint32_t chacha_for_poly_key1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3CC, read-only */
   uint32_t chacha_for_poly_key2;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3D0, read-only */
   uint32_t chacha_for_poly_key3;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3D4, read-only */
   uint32_t chacha_for_poly_key4;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3D8, read-only */
   uint32_t chacha_for_poly_key5;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3DC, read-only */
   uint32_t chacha_for_poly_key6;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3E0, read-only */
   uint32_t chacha_for_poly_key7;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3E4, read-write */
   uint32_t chacha_byte_word_order_cntl_reg;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x3E8, read-only */
   uint32_t chacha_debug_reg;
   /** Reserved space */
   uint8_t fill8[20];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x400, read-write */
   uint32_t aes_key_0_0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x404, read-write */
   uint32_t aes_key_0_1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x408, read-write */
   uint32_t aes_key_0_2;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x40C, read-write */
   uint32_t aes_key_0_3;
   /** Reserved space */
   uint8_t fill9[48];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x440, read-write */
   uint32_t aes_iv_0_0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x444, read-write */
   uint32_t aes_iv_0_1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x448, read-write */
   uint32_t aes_iv_0_2;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x44C, read-write */
   uint32_t aes_iv_0_3;
   /** Reserved space */
   uint8_t fill10[16];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x460, read-write */
   uint32_t aes_ctr_0_0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x464, read-write */
   uint32_t aes_ctr_0_1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x468, read-write */
   uint32_t aes_ctr_0_2;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x46C, read-write */
   uint32_t aes_ctr_0_3;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x470, read-only */
   uint32_t aes_busy;
   /** Reserved space */
   uint8_t fill11[4];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x478, read-write */
   uint32_t aes_sk;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x47C, write-only */
   uint32_t aes_cmac_init;
   /** Reserved space */
   uint8_t fill12[60];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x4BC, read-write */
   uint32_t aes_remaining_bytes;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x4C0, read-write */
   uint32_t aes_control;
   /** Reserved space */
   uint8_t fill13[4];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x4C8, read-only */
   uint32_t aes_hw_flags;
   /** Reserved space */
   uint8_t fill14[12];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x4D8, read-write */
   uint32_t aes_ctr_no_increment;
   /** Reserved space */
   uint8_t fill15[24];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x4F4, write-only */
   uint32_t aes_sw_reset;
   /** Reserved space */
   uint8_t fill16[44];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x524, read-write */
   uint32_t aes_cmac_size0_kick;
   /** Reserved space */
   uint8_t fill17[280];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x640, read-write */
   uint32_t hash_h0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x644, read-write */
   uint32_t hash_h1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x648, read-write */
   uint32_t hash_h2;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x64C, read-write */
   uint32_t hash_h3;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x650, read-write */
   uint32_t hash_h4;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x654, read-write */
   uint32_t hash_h5;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x658, read-write */
   uint32_t hash_h6;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x65C, read-write */
   uint32_t hash_h7;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x660, read-write */
   uint32_t hash_h8;
   /** Reserved space */
   uint8_t fill18[32];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x684, write-only */
   uint32_t auto_hw_padding;
   /** Reserved space */
   uint8_t fill19[12];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x694, read-write */
   uint32_t load_init_state;
   /** Reserved space */
   uint8_t fill20[280];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x7B0, read-only */
   uint32_t hash_version;
   /** Reserved space */
   uint8_t fill21[12];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x7C0, read-write */
   uint32_t hash_control;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x7C4, read-write */
   uint32_t hash_pad_en;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x7C8, read-write */
   uint32_t hash_pad_cfg;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x7CC, read-write */
   uint32_t hash_cur_len_0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x7D0, read-write */
   uint32_t hash_cur_len_1;
   /** Reserved space */
   uint8_t fill22[8];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x7DC, read-only */
   uint32_t hash_param;
   /** Reserved space */
   uint8_t fill23[4];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x7E4, write-only */
   uint32_t hash_aes_sw_reset;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x7E8, read-write */
   uint32_t hash_endianess;
   /** Reserved space */
   uint8_t fill24[36];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x810, write-only */
   uint32_t aes_clk_enable;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x814, write-only */
   uint32_t des_clk_enable;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x818, write-only */
   uint32_t hash_clk_enable;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x81C, write-only */
   uint32_t pka_clk_enable;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x820, write-only */
   uint32_t dma_clk_enable;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x824, read-only */
   uint32_t clk_status;
   /** Reserved space */
   uint8_t fill25[48];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x858, write-only */
   uint32_t chacha_clk_enable;
   /** Reserved space */
   uint8_t fill26[164];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x900, write-only */
   uint32_t crypto_ctl;
   /** Reserved space */
   uint8_t fill27[12];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x910, read-only */
   uint32_t crypto_busy;
   /** Reserved space */
   uint8_t fill28[8];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x91C, read-only */
   uint32_t hash_busy;
   /** Reserved space */
   uint8_t fill29[8];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0x928, read-only */
   uint32_t version;
   /** Reserved space */
   uint8_t fill30[212];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA00, read-only */
   uint32_t host_rgf_irr;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA04, read-write */
   uint32_t host_rgf_imr;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA08, write-only */
   uint32_t host_rgf_icr;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA0C, read-write */
   uint32_t host_rgf_endian;
   /** Reserved space */
   uint8_t fill31[16];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA20, write-only */
   uint32_t host_rgf_cc_sw_rst;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA24, read-only */
   uint32_t host_rgf_signature;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA28, read-only */
   uint32_t host_boot;
   /** Reserved space */
   uint8_t fill32[12];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA38, read-write */
   uint32_t host_cryptokey_sel;
   /** Reserved space */
   uint8_t fill33[16];
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA4C, read-write */
   uint32_t host_iot_kprtl_lock;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA50, read-write */
   uint32_t host_iot_kdr0;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA54, read-write */
   uint32_t host_iot_kdr1;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA58, read-write */
   uint32_t host_iot_kdr2;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA5C, read-write */
   uint32_t host_iot_kdr3;
   /** See ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual  at address offset 0xA60, read-write */
   uint32_t host_iot_lcs;
   /** Reserved space */
   uint8_t fill34[412];
   /**  at address offset 0xC00, write-only */
   uint32_t din_buffer;
   /** Reserved space */
   uint8_t fill35[28];
   /**  at address offset 0xC20, read-only */
   uint32_t din_mem_dma_busy;
   /**  at address offset 0xC24, read-write */
   uint32_t src_lli_sram_addr;
   /**  at address offset 0xC28, write-only */
   uint32_t src_lli_word0;
   /**  at address offset 0xC2C, write-only */
   uint32_t src_lli_word1;
   /**  at address offset 0xC30, read-write */
   uint32_t sram_src_addr;
   /**  at address offset 0xC34, write-only */
   uint32_t din_sram_bytes_len;
   /**  at address offset 0xC38, read-only */
   uint32_t din_sram_dma_busy;
   /**  at address offset 0xC3C, read-write */
   uint32_t din_sram_endianness;
   /** Reserved space */
   uint8_t fill36[4];
   /**  at address offset 0xC44, write-only */
   uint32_t din_sw_reset;
   /**  at address offset 0xC48, write-only */
   uint32_t din_cpu_data_size;
   /**  at address offset 0xC4C, write-only */
   uint32_t write_align_last;
   /**  at address offset 0xC50, read-only */
   uint32_t fifo_in_empty;
   /**  at address offset 0xC54, read-write */
   uint32_t disable_outstd_req;
   /**  at address offset 0xC58, write-only */
   uint32_t din_fifo_rst_pntr;
   /** Reserved space */
   uint8_t fill37[164];
   /**  at address offset 0xD00, read-only */
   uint32_t dout_buffer;
   /** Reserved space */
   uint8_t fill38[28];
   /**  at address offset 0xD20, read-only */
   uint32_t dout_mem_dma_busy;
   /**  at address offset 0xD24, read-write */
   uint32_t dst_lli_sram_addr;
   /**  at address offset 0xD28, write-only */
   uint32_t dst_lli_word0;
   /**  at address offset 0xD2C, write-only */
   uint32_t dst_lli_word1;
   /**  at address offset 0xD30, read-write */
   uint32_t sram_dest_addr;
   /**  at address offset 0xD34, write-only */
   uint32_t dout_sram_bytes_len;
   /**  at address offset 0xD38, read-only */
   uint32_t dout_sram_dma_busy;
   /**  at address offset 0xD3C, read-write */
   uint32_t dout_sram_endianness;
   /** Reserved space */
   uint8_t fill39[4];
   /**  at address offset 0xD44, write-only */
   uint32_t read_align_last;
   /** Reserved space */
   uint8_t fill40[8];
   /**  at address offset 0xD50, read-only */
   uint32_t dout_fifo_empty;
   /** Reserved space */
   uint8_t fill41[4];
   /**  at address offset 0xD58, write-only */
   uint32_t dout_sw_reset;
   /** Reserved space */
   uint8_t fill42[420];
   /**  at address offset 0xF00, read-write */
   uint32_t sram_data;
   /**  at address offset 0xF04, write-only */
   uint32_t sram_addr;
   /**  at address offset 0xF08, read-only */
   uint32_t sram_data_ready;
};

/** bit field defines for arm_crypto310_s#memory_map0 */
#define ARM_CRYPTO310_MEMORY_MAP0_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP0_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map1 */
#define ARM_CRYPTO310_MEMORY_MAP1_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP1_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map2 */
#define ARM_CRYPTO310_MEMORY_MAP2_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP2_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map3 */
#define ARM_CRYPTO310_MEMORY_MAP3_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP3_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map4 */
#define ARM_CRYPTO310_MEMORY_MAP4_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP4_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map5 */
#define ARM_CRYPTO310_MEMORY_MAP5_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP5_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map6 */
#define ARM_CRYPTO310_MEMORY_MAP6_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP6_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map7 */
#define ARM_CRYPTO310_MEMORY_MAP7_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP7_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map8 */
#define ARM_CRYPTO310_MEMORY_MAP8_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP8_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map9 */
#define ARM_CRYPTO310_MEMORY_MAP9_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP9_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map10 */
#define ARM_CRYPTO310_MEMORY_MAP10_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP10_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map11 */
#define ARM_CRYPTO310_MEMORY_MAP11_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP11_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map12 */
#define ARM_CRYPTO310_MEMORY_MAP12_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP12_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map13 */
#define ARM_CRYPTO310_MEMORY_MAP13_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP13_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map14 */
#define ARM_CRYPTO310_MEMORY_MAP14_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP14_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map15 */
#define ARM_CRYPTO310_MEMORY_MAP15_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP15_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map16 */
#define ARM_CRYPTO310_MEMORY_MAP16_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP16_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map17 */
#define ARM_CRYPTO310_MEMORY_MAP17_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP17_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map18 */
#define ARM_CRYPTO310_MEMORY_MAP18_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP18_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map19 */
#define ARM_CRYPTO310_MEMORY_MAP19_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP19_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map20 */
#define ARM_CRYPTO310_MEMORY_MAP20_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP20_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map21 */
#define ARM_CRYPTO310_MEMORY_MAP21_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP21_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map22 */
#define ARM_CRYPTO310_MEMORY_MAP22_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP22_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map23 */
#define ARM_CRYPTO310_MEMORY_MAP23_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP23_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map24 */
#define ARM_CRYPTO310_MEMORY_MAP24_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP24_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map25 */
#define ARM_CRYPTO310_MEMORY_MAP25_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP25_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map26 */
#define ARM_CRYPTO310_MEMORY_MAP26_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP26_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map27 */
#define ARM_CRYPTO310_MEMORY_MAP27_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP27_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map28 */
#define ARM_CRYPTO310_MEMORY_MAP28_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP28_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map29 */
#define ARM_CRYPTO310_MEMORY_MAP29_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP29_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map30 */
#define ARM_CRYPTO310_MEMORY_MAP30_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP30_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#memory_map31 */
#define ARM_CRYPTO310_MEMORY_MAP31_VALUE_OFFSET 1
#define ARM_CRYPTO310_MEMORY_MAP31_VALUE_SIZE 9

/** bit field defines for arm_crypto310_s#opcode */
#define ARM_CRYPTO310_OPCODE_TAG_OFFSET 0
#define ARM_CRYPTO310_OPCODE_TAG_SIZE 6
#define ARM_CRYPTO310_OPCODE_REG_R_OFFSET 6
#define ARM_CRYPTO310_OPCODE_REG_R_SIZE 6
#define ARM_CRYPTO310_OPCODE_REG_B_OFFSET 12
#define ARM_CRYPTO310_OPCODE_REG_B_SIZE 6
#define ARM_CRYPTO310_OPCODE_REG_A_OFFSET 18
#define ARM_CRYPTO310_OPCODE_REG_A_SIZE 6
#define ARM_CRYPTO310_OPCODE_LEN_OFFSET 24
#define ARM_CRYPTO310_OPCODE_LEN_SIZE 3
#define ARM_CRYPTO310_OPCODE_OPCODE_OFFSET 27
#define ARM_CRYPTO310_OPCODE_OPCODE_SIZE 5

/** bit field defines for arm_crypto310_s#n_np_t0_t1_addr */
#define ARM_CRYPTO310_N_NP_T0_T1_ADDR_N_VIRTUAL_ADDR_OFFSET 0
#define ARM_CRYPTO310_N_NP_T0_T1_ADDR_N_VIRTUAL_ADDR_SIZE 5
#define ARM_CRYPTO310_N_NP_T0_T1_ADDR_NP_VIRTUAL_ADDR_OFFSET 5
#define ARM_CRYPTO310_N_NP_T0_T1_ADDR_NP_VIRTUAL_ADDR_SIZE 5
#define ARM_CRYPTO310_N_NP_T0_T1_ADDR_T0_VIRTUAL_ADDR_OFFSET 10
#define ARM_CRYPTO310_N_NP_T0_T1_ADDR_T0_VIRTUAL_ADDR_SIZE 5
#define ARM_CRYPTO310_N_NP_T0_T1_ADDR_T1_VIRTUAL_ADDR_OFFSET 15
#define ARM_CRYPTO310_N_NP_T0_T1_ADDR_T1_VIRTUAL_ADDR_SIZE 5

/** bit field defines for arm_crypto310_s#pka_status */
#define ARM_CRYPTO310_PKA_STATUS_ALU_MSB_4BITS_OFFSET 0
#define ARM_CRYPTO310_PKA_STATUS_ALU_MSB_4BITS_SIZE 4
#define ARM_CRYPTO310_PKA_STATUS_ALU_LSB_4BITS_OFFSET 4
#define ARM_CRYPTO310_PKA_STATUS_ALU_LSB_4BITS_SIZE 4
#define ARM_CRYPTO310_PKA_STATUS_ALU_SIGN_OUT_OFFSET 8
#define ARM_CRYPTO310_PKA_STATUS_ALU_SIGN_OUT_SIZE 1
#define ARM_CRYPTO310_PKA_STATUS_ALU_CARRY_OFFSET 9
#define ARM_CRYPTO310_PKA_STATUS_ALU_CARRY_SIZE 1
#define ARM_CRYPTO310_PKA_STATUS_ALU_CARRY_MOD_OFFSET 10
#define ARM_CRYPTO310_PKA_STATUS_ALU_CARRY_MOD_SIZE 1
#define ARM_CRYPTO310_PKA_STATUS_ALU_SUB_IS_ZERO_OFFSET 11
#define ARM_CRYPTO310_PKA_STATUS_ALU_SUB_IS_ZERO_SIZE 1
#define ARM_CRYPTO310_PKA_STATUS_ALU_OUT_ZERO_OFFSET 12
#define ARM_CRYPTO310_PKA_STATUS_ALU_OUT_ZERO_SIZE 1
#define ARM_CRYPTO310_PKA_STATUS_ALU_MODOVRFLW_OFFSET 13
#define ARM_CRYPTO310_PKA_STATUS_ALU_MODOVRFLW_SIZE 1
#define ARM_CRYPTO310_PKA_STATUS_DIV_BY_ZERO_OFFSET 14
#define ARM_CRYPTO310_PKA_STATUS_DIV_BY_ZERO_SIZE 1
#define ARM_CRYPTO310_PKA_STATUS_MODINV_OF_ZERO_OFFSET 15
#define ARM_CRYPTO310_PKA_STATUS_MODINV_OF_ZERO_SIZE 1
#define ARM_CRYPTO310_PKA_STATUS_OPCODE_OFFSET 16
#define ARM_CRYPTO310_PKA_STATUS_OPCODE_SIZE 5

/** bit field defines for arm_crypto310_s#pka_sw_reset */
#define ARM_CRYPTO310_PKA_SW_RESET_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_SW_RESET_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#pka_l0 */
#define ARM_CRYPTO310_PKA_L0_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_L0_VALUE_SIZE 13

/** bit field defines for arm_crypto310_s#pka_l1 */
#define ARM_CRYPTO310_PKA_L1_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_L1_VALUE_SIZE 13

/** bit field defines for arm_crypto310_s#pka_l2 */
#define ARM_CRYPTO310_PKA_L2_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_L2_VALUE_SIZE 13

/** bit field defines for arm_crypto310_s#pka_l3 */
#define ARM_CRYPTO310_PKA_L3_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_L3_VALUE_SIZE 13

/** bit field defines for arm_crypto310_s#pka_l4 */
#define ARM_CRYPTO310_PKA_L4_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_L4_VALUE_SIZE 13

/** bit field defines for arm_crypto310_s#pka_l5 */
#define ARM_CRYPTO310_PKA_L5_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_L5_VALUE_SIZE 13

/** bit field defines for arm_crypto310_s#pka_l6 */
#define ARM_CRYPTO310_PKA_L6_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_L6_VALUE_SIZE 13

/** bit field defines for arm_crypto310_s#pka_l7 */
#define ARM_CRYPTO310_PKA_L7_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_L7_VALUE_SIZE 13

/** bit field defines for arm_crypto310_s#pka_pipe_rdy */
#define ARM_CRYPTO310_PKA_PIPE_RDY_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_PIPE_RDY_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#pka_done */
#define ARM_CRYPTO310_PKA_DONE_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_DONE_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#pka_version */
#define ARM_CRYPTO310_PKA_VERSION_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_VERSION_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#pka_msb_addr */
#define ARM_CRYPTO310_PKA_MSB_ADDR_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_MSB_ADDR_VALUE_SIZE 2

/** bit field defines for arm_crypto310_s#pka_mon_read */
#define ARM_CRYPTO310_PKA_MON_READ_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_MON_READ_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#pka_sram_addr */
#define ARM_CRYPTO310_PKA_SRAM_ADDR_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_SRAM_ADDR_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#pka_sram_wdata */
#define ARM_CRYPTO310_PKA_SRAM_WDATA_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_SRAM_WDATA_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#pka_sram_rdata */
#define ARM_CRYPTO310_PKA_SRAM_RDATA_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_SRAM_RDATA_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#pka_sram_wr_clr */
#define ARM_CRYPTO310_PKA_SRAM_WR_CLR_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_SRAM_WR_CLR_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#pka_sram_raddr */
#define ARM_CRYPTO310_PKA_SRAM_RADDR_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_SRAM_RADDR_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#pka_word_access */
#define ARM_CRYPTO310_PKA_WORD_ACCESS_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_WORD_ACCESS_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#pka_buff_addr */
#define ARM_CRYPTO310_PKA_BUFF_ADDR_PKA_BUF_ADDR_OFFSET 0
#define ARM_CRYPTO310_PKA_BUFF_ADDR_PKA_BUF_ADDR_SIZE 12
#define ARM_CRYPTO310_PKA_BUFF_ADDR_RESEREVED1_OFFSET 12
#define ARM_CRYPTO310_PKA_BUFF_ADDR_RESEREVED1_SIZE 20

/** bit field defines for arm_crypto310_s#rng_imr */
#define ARM_CRYPTO310_RNG_IMR_EHR_VALID_INT_MASK_OFFSET 0
#define ARM_CRYPTO310_RNG_IMR_EHR_VALID_INT_MASK_SIZE 1
#define ARM_CRYPTO310_RNG_IMR_AUTOCORR_ERR_INT_MASK_OFFSET 1
#define ARM_CRYPTO310_RNG_IMR_AUTOCORR_ERR_INT_MASK_SIZE 1
#define ARM_CRYPTO310_RNG_IMR_CRNGT_ERR_INT_MASK_OFFSET 2
#define ARM_CRYPTO310_RNG_IMR_CRNGT_ERR_INT_MASK_SIZE 1
#define ARM_CRYPTO310_RNG_IMR_VN_ERR_INT_MASK_OFFSET 3
#define ARM_CRYPTO310_RNG_IMR_VN_ERR_INT_MASK_SIZE 1
#define ARM_CRYPTO310_RNG_IMR_WATCHDOG_INT_MASK_OFFSET 4
#define ARM_CRYPTO310_RNG_IMR_WATCHDOG_INT_MASK_SIZE 1
#define ARM_CRYPTO310_RNG_IMR_RNG_DMA_DONE_INT_OFFSET 5
#define ARM_CRYPTO310_RNG_IMR_RNG_DMA_DONE_INT_SIZE 1

/** bit field defines for arm_crypto310_s#rng_isr */
#define ARM_CRYPTO310_RNG_ISR_EHR_VALID_OFFSET 0
#define ARM_CRYPTO310_RNG_ISR_EHR_VALID_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_AUTOCORR_ERR_OFFSET 1
#define ARM_CRYPTO310_RNG_ISR_AUTOCORR_ERR_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_CRNGT_ERR_OFFSET 2
#define ARM_CRYPTO310_RNG_ISR_CRNGT_ERR_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_VN_ERR_OFFSET 3
#define ARM_CRYPTO310_RNG_ISR_VN_ERR_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_RNG_DMA_DONE_OFFSET 5
#define ARM_CRYPTO310_RNG_ISR_RNG_DMA_DONE_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_RESEEDING_DONE_OFFSET 16
#define ARM_CRYPTO310_RNG_ISR_RESEEDING_DONE_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_INSTANTIATION_DONE_OFFSET 17
#define ARM_CRYPTO310_RNG_ISR_INSTANTIATION_DONE_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_FINAL_UPDATE_DONE_OFFSET 18
#define ARM_CRYPTO310_RNG_ISR_FINAL_UPDATE_DONE_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_OUTPUT_READY_OFFSET 19
#define ARM_CRYPTO310_RNG_ISR_OUTPUT_READY_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_RESEED_CNTR_FULL_OFFSET 20
#define ARM_CRYPTO310_RNG_ISR_RESEED_CNTR_FULL_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_RESEED_CNTR_TOP_40_OFFSET 21
#define ARM_CRYPTO310_RNG_ISR_RESEED_CNTR_TOP_40_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_PRNG_CRNGT_ERR_OFFSET 22
#define ARM_CRYPTO310_RNG_ISR_PRNG_CRNGT_ERR_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_REQ_SIZE_OFFSET 23
#define ARM_CRYPTO310_RNG_ISR_REQ_SIZE_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_KAT_ERR_OFFSET 24
#define ARM_CRYPTO310_RNG_ISR_KAT_ERR_SIZE 1
#define ARM_CRYPTO310_RNG_ISR_WHICH_KAT_ERR_OFFSET 25
#define ARM_CRYPTO310_RNG_ISR_WHICH_KAT_ERR_SIZE 2

/** bit field defines for arm_crypto310_s#rng_icr */
#define ARM_CRYPTO310_RNG_ICR_EHR_VALID_OFFSET 0
#define ARM_CRYPTO310_RNG_ICR_EHR_VALID_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_AUTOCORR_ERR_OFFSET 1
#define ARM_CRYPTO310_RNG_ICR_AUTOCORR_ERR_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_CRNGT_ERR_OFFSET 2
#define ARM_CRYPTO310_RNG_ICR_CRNGT_ERR_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_VN_ERR_OFFSET 3
#define ARM_CRYPTO310_RNG_ICR_VN_ERR_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_RNG_WATCHDOG_OFFSET 4
#define ARM_CRYPTO310_RNG_ICR_RNG_WATCHDOG_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_RNG_DMA_DONE_OFFSET 5
#define ARM_CRYPTO310_RNG_ICR_RNG_DMA_DONE_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_RESEEDING_DONE_OFFSET 16
#define ARM_CRYPTO310_RNG_ICR_RESEEDING_DONE_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_INSTANTIATION_DONE_OFFSET 17
#define ARM_CRYPTO310_RNG_ICR_INSTANTIATION_DONE_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_FINAL_UPDATE_DONE_OFFSET 18
#define ARM_CRYPTO310_RNG_ICR_FINAL_UPDATE_DONE_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_OUTPUT_READY_OFFSET 19
#define ARM_CRYPTO310_RNG_ICR_OUTPUT_READY_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_RESEED_CNTR_FULL_OFFSET 20
#define ARM_CRYPTO310_RNG_ICR_RESEED_CNTR_FULL_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_RESEED_CNTR_TOP_40_OFFSET 21
#define ARM_CRYPTO310_RNG_ICR_RESEED_CNTR_TOP_40_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_PRNG_CRNGT_ERR_OFFSET 22
#define ARM_CRYPTO310_RNG_ICR_PRNG_CRNGT_ERR_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_REQ_SIZE_OFFSET 23
#define ARM_CRYPTO310_RNG_ICR_REQ_SIZE_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_KAT_ERR_OFFSET 24
#define ARM_CRYPTO310_RNG_ICR_KAT_ERR_SIZE 1
#define ARM_CRYPTO310_RNG_ICR_WHICH_KAT_ERR_OFFSET 25
#define ARM_CRYPTO310_RNG_ICR_WHICH_KAT_ERR_SIZE 2

/** bit field defines for arm_crypto310_s#rng_trng_config */
#define ARM_CRYPTO310_RNG_TRNG_CONFIG_RND_SRC_SEL_OFFSET 0
#define ARM_CRYPTO310_RNG_TRNG_CONFIG_RND_SRC_SEL_SIZE 2
#define ARM_CRYPTO310_RNG_TRNG_CONFIG_SOP_SEL_OFFSET 2
#define ARM_CRYPTO310_RNG_TRNG_CONFIG_SOP_SEL_SIZE 1

/** bit field defines for arm_crypto310_s#rng_trng_valid */
#define ARM_CRYPTO310_RNG_TRNG_VALID_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_TRNG_VALID_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#rng_ehr_data_0 */
#define ARM_CRYPTO310_RNG_EHR_DATA_0_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_EHR_DATA_0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#rng_ehr_data_1 */
#define ARM_CRYPTO310_RNG_EHR_DATA_1_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_EHR_DATA_1_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#rng_ehr_data_2 */
#define ARM_CRYPTO310_RNG_EHR_DATA_2_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_EHR_DATA_2_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#rng_ehr_data_3 */
#define ARM_CRYPTO310_RNG_EHR_DATA_3_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_EHR_DATA_3_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#rng_ehr_data_4 */
#define ARM_CRYPTO310_RNG_EHR_DATA_4_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_EHR_DATA_4_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#rng_ehr_data_5 */
#define ARM_CRYPTO310_RNG_EHR_DATA_5_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_EHR_DATA_5_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#rng_rnd_source_enable */
#define ARM_CRYPTO310_RNG_RND_SOURCE_ENABLE_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_RND_SOURCE_ENABLE_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#rng_sample_cnt1 */
#define ARM_CRYPTO310_RNG_SAMPLE_CNT1_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_SAMPLE_CNT1_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#rng_autocorr_statistic */
#define ARM_CRYPTO310_RNG_AUTOCORR_STATISTIC_AUTOCORR_TRYS_OFFSET 0
#define ARM_CRYPTO310_RNG_AUTOCORR_STATISTIC_AUTOCORR_TRYS_SIZE 14
#define ARM_CRYPTO310_RNG_AUTOCORR_STATISTIC_AUTOCORR_FAILS_OFFSET 14
#define ARM_CRYPTO310_RNG_AUTOCORR_STATISTIC_AUTOCORR_FAILS_SIZE 8

/** bit field defines for arm_crypto310_s#rng_trng_debug_control */
#define ARM_CRYPTO310_RNG_TRNG_DEBUG_CONTROL_VNC_BYPASS_OFFSET 1
#define ARM_CRYPTO310_RNG_TRNG_DEBUG_CONTROL_VNC_BYPASS_SIZE 1
#define ARM_CRYPTO310_RNG_TRNG_DEBUG_CONTROL_TRNG_CRNGT_BYPASS_OFFSET 2
#define ARM_CRYPTO310_RNG_TRNG_DEBUG_CONTROL_TRNG_CRNGT_BYPASS_SIZE 1
#define ARM_CRYPTO310_RNG_TRNG_DEBUG_CONTROL_AUTO_CORRELATE_BYPASS_OFFSET 3
#define ARM_CRYPTO310_RNG_TRNG_DEBUG_CONTROL_AUTO_CORRELATE_BYPASS_SIZE 1

/** bit field defines for arm_crypto310_s#rng_sw_reset */
#define ARM_CRYPTO310_RNG_SW_RESET_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_SW_RESET_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#rng_debug_en_input */
#define ARM_CRYPTO310_RNG_DEBUG_EN_INPUT_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_DEBUG_EN_INPUT_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#rng_busy */
#define ARM_CRYPTO310_RNG_BUSY_RNG_BUSY_OFFSET 0
#define ARM_CRYPTO310_RNG_BUSY_RNG_BUSY_SIZE 1
#define ARM_CRYPTO310_RNG_BUSY_TRNG_BUSY_OFFSET 1
#define ARM_CRYPTO310_RNG_BUSY_TRNG_BUSY_SIZE 1
#define ARM_CRYPTO310_RNG_BUSY_PRNG_BUSY_OFFSET 2
#define ARM_CRYPTO310_RNG_BUSY_PRNG_BUSY_SIZE 1

/** bit field defines for arm_crypto310_s#rst_bits_counter */
#define ARM_CRYPTO310_RST_BITS_COUNTER_VALUE_OFFSET 0
#define ARM_CRYPTO310_RST_BITS_COUNTER_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#rng_version */
#define ARM_CRYPTO310_RNG_VERSION_EHR_WIDTH_192_OFFSET 0
#define ARM_CRYPTO310_RNG_VERSION_EHR_WIDTH_192_SIZE 1
#define ARM_CRYPTO310_RNG_VERSION_CRNGT_EXISTS_OFFSET 1
#define ARM_CRYPTO310_RNG_VERSION_CRNGT_EXISTS_SIZE 1
#define ARM_CRYPTO310_RNG_VERSION_AUTOCORR_EXISTS_OFFSET 2
#define ARM_CRYPTO310_RNG_VERSION_AUTOCORR_EXISTS_SIZE 1
#define ARM_CRYPTO310_RNG_VERSION_TRNG_TESTS_BYPASS_EN_OFFSET 3
#define ARM_CRYPTO310_RNG_VERSION_TRNG_TESTS_BYPASS_EN_SIZE 1
#define ARM_CRYPTO310_RNG_VERSION_PRNG_EXISTS_OFFSET 4
#define ARM_CRYPTO310_RNG_VERSION_PRNG_EXISTS_SIZE 1
#define ARM_CRYPTO310_RNG_VERSION_KAT_EXISTS_OFFSET 5
#define ARM_CRYPTO310_RNG_VERSION_KAT_EXISTS_SIZE 1
#define ARM_CRYPTO310_RNG_VERSION_RESEEDING_EXISTS_OFFSET 6
#define ARM_CRYPTO310_RNG_VERSION_RESEEDING_EXISTS_SIZE 1
#define ARM_CRYPTO310_RNG_VERSION_RNG_USE_5_SBOXES_OFFSET 7
#define ARM_CRYPTO310_RNG_VERSION_RNG_USE_5_SBOXES_SIZE 1

/** bit field defines for arm_crypto310_s#rng_clk_enable */
#define ARM_CRYPTO310_RNG_CLK_ENABLE_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_CLK_ENABLE_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#rng_dma_enable */
#define ARM_CRYPTO310_RNG_DMA_ENABLE_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_DMA_ENABLE_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#rng_dma_src_mask */
#define ARM_CRYPTO310_RNG_DMA_SRC_MASK_EN_SRC_SEL0_OFFSET 0
#define ARM_CRYPTO310_RNG_DMA_SRC_MASK_EN_SRC_SEL0_SIZE 1
#define ARM_CRYPTO310_RNG_DMA_SRC_MASK_EN_SRC_SEL1_OFFSET 1
#define ARM_CRYPTO310_RNG_DMA_SRC_MASK_EN_SRC_SEL1_SIZE 1
#define ARM_CRYPTO310_RNG_DMA_SRC_MASK_EN_SRC_SEL2_OFFSET 2
#define ARM_CRYPTO310_RNG_DMA_SRC_MASK_EN_SRC_SEL2_SIZE 1
#define ARM_CRYPTO310_RNG_DMA_SRC_MASK_EN_SRC_SEL3_OFFSET 3
#define ARM_CRYPTO310_RNG_DMA_SRC_MASK_EN_SRC_SEL3_SIZE 1

/** bit field defines for arm_crypto310_s#rng_dma_sram_addr */
#define ARM_CRYPTO310_RNG_DMA_SRAM_ADDR_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_DMA_SRAM_ADDR_VALUE_SIZE 11

/** bit field defines for arm_crypto310_s#rng_dma_samples_num */
#define ARM_CRYPTO310_RNG_DMA_SAMPLES_NUM_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_DMA_SAMPLES_NUM_VALUE_SIZE 8

/** bit field defines for arm_crypto310_s#rng_watchdog_val */
#define ARM_CRYPTO310_RNG_WATCHDOG_VAL_VALUE_OFFSET 0
#define ARM_CRYPTO310_RNG_WATCHDOG_VAL_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#rng_dma_status */
#define ARM_CRYPTO310_RNG_DMA_STATUS_RNG_DMA_BUSY_OFFSET 0
#define ARM_CRYPTO310_RNG_DMA_STATUS_RNG_DMA_BUSY_SIZE 1
#define ARM_CRYPTO310_RNG_DMA_STATUS_DMA_SRC_SEL_OFFSET 1
#define ARM_CRYPTO310_RNG_DMA_STATUS_DMA_SRC_SEL_SIZE 2
#define ARM_CRYPTO310_RNG_DMA_STATUS_NUM_OF_SAMPLES_OFFSET 3
#define ARM_CRYPTO310_RNG_DMA_STATUS_NUM_OF_SAMPLES_SIZE 8

/** bit field defines for arm_crypto310_s#chacha_control_reg */
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_CHACHA_OR_SALSA_OFFSET 0
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_CHACHA_OR_SALSA_SIZE 1
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_INIT_FROM_HOST_OFFSET 1
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_INIT_FROM_HOST_SIZE 1
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_CALC_KEY_FOR_POLY1305_OFFSET 2
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_CALC_KEY_FOR_POLY1305_SIZE 1
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_KEY_LEN_OFFSET 3
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_KEY_LEN_SIZE 1
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_NUM_OF_ROUNDS_OFFSET 4
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_NUM_OF_ROUNDS_SIZE 2
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_RESET_BLOCK_CNT_OFFSET 9
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_RESET_BLOCK_CNT_SIZE 1
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_USE_IV_96BIT_OFFSET 10
#define ARM_CRYPTO310_CHACHA_CONTROL_REG_USE_IV_96BIT_SIZE 1

/** bit field defines for arm_crypto310_s#chacha_version */
#define ARM_CRYPTO310_CHACHA_VERSION_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_VERSION_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_key0 */
#define ARM_CRYPTO310_CHACHA_KEY0_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_KEY0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_key1 */
#define ARM_CRYPTO310_CHACHA_KEY1_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_KEY1_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_key2 */
#define ARM_CRYPTO310_CHACHA_KEY2_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_KEY2_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_key3 */
#define ARM_CRYPTO310_CHACHA_KEY3_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_KEY3_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_key4 */
#define ARM_CRYPTO310_CHACHA_KEY4_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_KEY4_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_key5 */
#define ARM_CRYPTO310_CHACHA_KEY5_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_KEY5_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_key6 */
#define ARM_CRYPTO310_CHACHA_KEY6_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_KEY6_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_key7 */
#define ARM_CRYPTO310_CHACHA_KEY7_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_KEY7_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_iv_0 */
#define ARM_CRYPTO310_CHACHA_IV_0_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_IV_0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_iv_1 */
#define ARM_CRYPTO310_CHACHA_IV_1_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_IV_1_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_busy */
#define ARM_CRYPTO310_CHACHA_BUSY_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_BUSY_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#chacha_hw_flags */
#define ARM_CRYPTO310_CHACHA_HW_FLAGS_CHACHA_EXISTS_OFFSET 0
#define ARM_CRYPTO310_CHACHA_HW_FLAGS_CHACHA_EXISTS_SIZE 1
#define ARM_CRYPTO310_CHACHA_HW_FLAGS_SALSA_EXISTS_OFFSET 1
#define ARM_CRYPTO310_CHACHA_HW_FLAGS_SALSA_EXISTS_SIZE 1
#define ARM_CRYPTO310_CHACHA_HW_FLAGS_FAST_CHACHA_OFFSET 2
#define ARM_CRYPTO310_CHACHA_HW_FLAGS_FAST_CHACHA_SIZE 1

/** bit field defines for arm_crypto310_s#chacha_block_cnt_lsb */
#define ARM_CRYPTO310_CHACHA_BLOCK_CNT_LSB_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_BLOCK_CNT_LSB_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_block_cnt_msb */
#define ARM_CRYPTO310_CHACHA_BLOCK_CNT_MSB_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_BLOCK_CNT_MSB_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_sw_reset */
#define ARM_CRYPTO310_CHACHA_SW_RESET_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_SW_RESET_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#chacha_for_poly_key0 */
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY0_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_for_poly_key1 */
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY1_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY1_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_for_poly_key2 */
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY2_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY2_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_for_poly_key3 */
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY3_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY3_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_for_poly_key4 */
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY4_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY4_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_for_poly_key5 */
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY5_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY5_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_for_poly_key6 */
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY6_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY6_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_for_poly_key7 */
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY7_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_FOR_POLY_KEY7_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#chacha_byte_word_order_cntl_reg */
#define ARM_CRYPTO310_CHACHA_BYTE_WORD_ORDER_CNTL_REG_CHACHA_DIN_WORD_ORDER_OFFSET 0
#define ARM_CRYPTO310_CHACHA_BYTE_WORD_ORDER_CNTL_REG_CHACHA_DIN_WORD_ORDER_SIZE 1
#define ARM_CRYPTO310_CHACHA_BYTE_WORD_ORDER_CNTL_REG_CHACHA_DIN_BYTE_ORDER_OFFSET 1
#define ARM_CRYPTO310_CHACHA_BYTE_WORD_ORDER_CNTL_REG_CHACHA_DIN_BYTE_ORDER_SIZE 1
#define ARM_CRYPTO310_CHACHA_BYTE_WORD_ORDER_CNTL_REG_CHACHA_CORE_MATRIX_LBE_ORDER_OFFSET 2
#define ARM_CRYPTO310_CHACHA_BYTE_WORD_ORDER_CNTL_REG_CHACHA_CORE_MATRIX_LBE_ORDER_SIZE 1
#define ARM_CRYPTO310_CHACHA_BYTE_WORD_ORDER_CNTL_REG_CHACHA_DOUT_WORD_ORDER_OFFSET 3
#define ARM_CRYPTO310_CHACHA_BYTE_WORD_ORDER_CNTL_REG_CHACHA_DOUT_WORD_ORDER_SIZE 1
#define ARM_CRYPTO310_CHACHA_BYTE_WORD_ORDER_CNTL_REG_CHACHA_DOUT_BYTE_ORDER_OFFSET 4
#define ARM_CRYPTO310_CHACHA_BYTE_WORD_ORDER_CNTL_REG_CHACHA_DOUT_BYTE_ORDER_SIZE 1

/** bit field defines for arm_crypto310_s#chacha_debug_reg */
#define ARM_CRYPTO310_CHACHA_DEBUG_REG_CHACHA_DEBUG_FSM_STATE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_DEBUG_REG_CHACHA_DEBUG_FSM_STATE_SIZE 3
#define ARM_CRYPTO310_CHACHA_DEBUG_REG_CHACHA_DEBUG_ERR_OFFSET 3
#define ARM_CRYPTO310_CHACHA_DEBUG_REG_CHACHA_DEBUG_ERR_SIZE 1

/** bit field defines for arm_crypto310_s#aes_key_0_0 */
#define ARM_CRYPTO310_AES_KEY_0_0_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_KEY_0_0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_key_0_1 */
#define ARM_CRYPTO310_AES_KEY_0_1_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_KEY_0_1_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_key_0_2 */
#define ARM_CRYPTO310_AES_KEY_0_2_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_KEY_0_2_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_key_0_3 */
#define ARM_CRYPTO310_AES_KEY_0_3_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_KEY_0_3_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_iv_0_0 */
#define ARM_CRYPTO310_AES_IV_0_0_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_IV_0_0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_iv_0_1 */
#define ARM_CRYPTO310_AES_IV_0_1_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_IV_0_1_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_iv_0_2 */
#define ARM_CRYPTO310_AES_IV_0_2_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_IV_0_2_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_iv_0_3 */
#define ARM_CRYPTO310_AES_IV_0_3_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_IV_0_3_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_ctr_0_0 */
#define ARM_CRYPTO310_AES_CTR_0_0_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_CTR_0_0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_ctr_0_1 */
#define ARM_CRYPTO310_AES_CTR_0_1_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_CTR_0_1_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_ctr_0_2 */
#define ARM_CRYPTO310_AES_CTR_0_2_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_CTR_0_2_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_ctr_0_3 */
#define ARM_CRYPTO310_AES_CTR_0_3_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_CTR_0_3_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_busy */
#define ARM_CRYPTO310_AES_BUSY_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_BUSY_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#aes_sk */
#define ARM_CRYPTO310_AES_SK_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_SK_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#aes_cmac_init */
#define ARM_CRYPTO310_AES_CMAC_INIT_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_CMAC_INIT_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#aes_remaining_bytes */
#define ARM_CRYPTO310_AES_REMAINING_BYTES_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_REMAINING_BYTES_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#aes_control */
#define ARM_CRYPTO310_AES_CONTROL_DEC_KEY0_OFFSET 0
#define ARM_CRYPTO310_AES_CONTROL_DEC_KEY0_SIZE 1
#define ARM_CRYPTO310_AES_CONTROL_MODE_KEY0_OFFSET 2
#define ARM_CRYPTO310_AES_CONTROL_MODE_KEY0_SIZE 3
#define ARM_CRYPTO310_AES_CONTROL_NK_KEY0_OFFSET 12
#define ARM_CRYPTO310_AES_CONTROL_NK_KEY0_SIZE 2
#define ARM_CRYPTO310_AES_CONTROL_AES_XOR_CRYPTOKEY_OFFSET 29
#define ARM_CRYPTO310_AES_CONTROL_AES_XOR_CRYPTOKEY_SIZE 1
#define ARM_CRYPTO310_AES_CONTROL_DIRECT_ACCESS_OFFSET 31
#define ARM_CRYPTO310_AES_CONTROL_DIRECT_ACCESS_SIZE 1

/** bit field defines for arm_crypto310_s#aes_hw_flags */
#define ARM_CRYPTO310_AES_HW_FLAGS_SUPPORT_256_192_KEY_OFFSET 0
#define ARM_CRYPTO310_AES_HW_FLAGS_SUPPORT_256_192_KEY_SIZE 1
#define ARM_CRYPTO310_AES_HW_FLAGS_AES_LARGE_RKEK_OFFSET 1
#define ARM_CRYPTO310_AES_HW_FLAGS_AES_LARGE_RKEK_SIZE 1
#define ARM_CRYPTO310_AES_HW_FLAGS_DPA_CNTRMSR_EXIST_OFFSET 2
#define ARM_CRYPTO310_AES_HW_FLAGS_DPA_CNTRMSR_EXIST_SIZE 1
#define ARM_CRYPTO310_AES_HW_FLAGS_CTR_EXIST_OFFSET 3
#define ARM_CRYPTO310_AES_HW_FLAGS_CTR_EXIST_SIZE 1
#define ARM_CRYPTO310_AES_HW_FLAGS_ONLY_ENCRYPT_OFFSET 4
#define ARM_CRYPTO310_AES_HW_FLAGS_ONLY_ENCRYPT_SIZE 1
#define ARM_CRYPTO310_AES_HW_FLAGS_USE_SBOX_TABLE_OFFSET 5
#define ARM_CRYPTO310_AES_HW_FLAGS_USE_SBOX_TABLE_SIZE 1
#define ARM_CRYPTO310_AES_HW_FLAGS_USE_5_SBOXES_OFFSET 8
#define ARM_CRYPTO310_AES_HW_FLAGS_USE_5_SBOXES_SIZE 1
#define ARM_CRYPTO310_AES_HW_FLAGS_AES_SUPPORT_PREV_IV_OFFSET 9
#define ARM_CRYPTO310_AES_HW_FLAGS_AES_SUPPORT_PREV_IV_SIZE 1
#define ARM_CRYPTO310_AES_HW_FLAGS_AES_TUNNEL_EXISTS_OFFSET 10
#define ARM_CRYPTO310_AES_HW_FLAGS_AES_TUNNEL_EXISTS_SIZE 1
#define ARM_CRYPTO310_AES_HW_FLAGS_SECOND_REGS_SET_EXIST_OFFSET 11
#define ARM_CRYPTO310_AES_HW_FLAGS_SECOND_REGS_SET_EXIST_SIZE 1
#define ARM_CRYPTO310_AES_HW_FLAGS_DFA_CNTRMSR_EXIST_OFFSET 12
#define ARM_CRYPTO310_AES_HW_FLAGS_DFA_CNTRMSR_EXIST_SIZE 1

/** bit field defines for arm_crypto310_s#aes_ctr_no_increment */
#define ARM_CRYPTO310_AES_CTR_NO_INCREMENT_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_CTR_NO_INCREMENT_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#aes_sw_reset */
#define ARM_CRYPTO310_AES_SW_RESET_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_SW_RESET_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#aes_cmac_size0_kick */
#define ARM_CRYPTO310_AES_CMAC_SIZE0_KICK_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_CMAC_SIZE0_KICK_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#hash_h0 */
#define ARM_CRYPTO310_HASH_H0_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_H0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#hash_h1 */
#define ARM_CRYPTO310_HASH_H1_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_H1_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#hash_h2 */
#define ARM_CRYPTO310_HASH_H2_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_H2_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#hash_h3 */
#define ARM_CRYPTO310_HASH_H3_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_H3_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#hash_h4 */
#define ARM_CRYPTO310_HASH_H4_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_H4_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#hash_h5 */
#define ARM_CRYPTO310_HASH_H5_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_H5_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#hash_h6 */
#define ARM_CRYPTO310_HASH_H6_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_H6_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#hash_h7 */
#define ARM_CRYPTO310_HASH_H7_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_H7_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#hash_h8 */
#define ARM_CRYPTO310_HASH_H8_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_H8_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#auto_hw_padding */
#define ARM_CRYPTO310_AUTO_HW_PADDING_VALUE_OFFSET 0
#define ARM_CRYPTO310_AUTO_HW_PADDING_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#load_init_state */
#define ARM_CRYPTO310_LOAD_INIT_STATE_VALUE_OFFSET 0
#define ARM_CRYPTO310_LOAD_INIT_STATE_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#hash_version */
#define ARM_CRYPTO310_HASH_VERSION_FIXES_OFFSET 0
#define ARM_CRYPTO310_HASH_VERSION_FIXES_SIZE 8
#define ARM_CRYPTO310_HASH_VERSION_MINOR_VERSION_NUMBER_OFFSET 8
#define ARM_CRYPTO310_HASH_VERSION_MINOR_VERSION_NUMBER_SIZE 4
#define ARM_CRYPTO310_HASH_VERSION_MAJOR_VERSION_NUMBER_OFFSET 12
#define ARM_CRYPTO310_HASH_VERSION_MAJOR_VERSION_NUMBER_SIZE 4

/** bit field defines for arm_crypto310_s#hash_control */
#define ARM_CRYPTO310_HASH_CONTROL_MODE_0_1_OFFSET 0
#define ARM_CRYPTO310_HASH_CONTROL_MODE_0_1_SIZE 2
#define ARM_CRYPTO310_HASH_CONTROL_MODE_3_OFFSET 3
#define ARM_CRYPTO310_HASH_CONTROL_MODE_3_SIZE 1

/** bit field defines for arm_crypto310_s#hash_pad_en */
#define ARM_CRYPTO310_HASH_PAD_EN_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_PAD_EN_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#hash_pad_cfg */
#define ARM_CRYPTO310_HASH_PAD_CFG_VALUE_OFFSET 2
#define ARM_CRYPTO310_HASH_PAD_CFG_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#hash_cur_len_0 */
#define ARM_CRYPTO310_HASH_CUR_LEN_0_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_CUR_LEN_0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#hash_cur_len_1 */
#define ARM_CRYPTO310_HASH_CUR_LEN_1_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_CUR_LEN_1_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#hash_param */
#define ARM_CRYPTO310_HASH_PARAM_CW_OFFSET 0
#define ARM_CRYPTO310_HASH_PARAM_CW_SIZE 4
#define ARM_CRYPTO310_HASH_PARAM_CH_OFFSET 4
#define ARM_CRYPTO310_HASH_PARAM_CH_SIZE 4
#define ARM_CRYPTO310_HASH_PARAM_DW_OFFSET 8
#define ARM_CRYPTO310_HASH_PARAM_DW_SIZE 4
#define ARM_CRYPTO310_HASH_PARAM_SHA_512_EXISTS_OFFSET 12
#define ARM_CRYPTO310_HASH_PARAM_SHA_512_EXISTS_SIZE 1
#define ARM_CRYPTO310_HASH_PARAM_PAD_EXISTS_OFFSET 13
#define ARM_CRYPTO310_HASH_PARAM_PAD_EXISTS_SIZE 1
#define ARM_CRYPTO310_HASH_PARAM_MD5_EXISTS_OFFSET 14
#define ARM_CRYPTO310_HASH_PARAM_MD5_EXISTS_SIZE 1
#define ARM_CRYPTO310_HASH_PARAM_HMAC_EXISTS_OFFSET 15
#define ARM_CRYPTO310_HASH_PARAM_HMAC_EXISTS_SIZE 1
#define ARM_CRYPTO310_HASH_PARAM_SHA_256_EXISTS_OFFSET 16
#define ARM_CRYPTO310_HASH_PARAM_SHA_256_EXISTS_SIZE 1
#define ARM_CRYPTO310_HASH_PARAM_HASH_COMPARE_EXISTS_OFFSET 17
#define ARM_CRYPTO310_HASH_PARAM_HASH_COMPARE_EXISTS_SIZE 1
#define ARM_CRYPTO310_HASH_PARAM_DUMP_HASH_TO_DOUT_EXISTS_OFFSET 18
#define ARM_CRYPTO310_HASH_PARAM_DUMP_HASH_TO_DOUT_EXISTS_SIZE 1

/** bit field defines for arm_crypto310_s#hash_aes_sw_reset */
#define ARM_CRYPTO310_HASH_AES_SW_RESET_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_AES_SW_RESET_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#hash_endianess */
#define ARM_CRYPTO310_HASH_ENDIANESS_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_ENDIANESS_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#aes_clk_enable */
#define ARM_CRYPTO310_AES_CLK_ENABLE_VALUE_OFFSET 0
#define ARM_CRYPTO310_AES_CLK_ENABLE_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#des_clk_enable */
#define ARM_CRYPTO310_DES_CLK_ENABLE_VALUE_OFFSET 0
#define ARM_CRYPTO310_DES_CLK_ENABLE_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#hash_clk_enable */
#define ARM_CRYPTO310_HASH_CLK_ENABLE_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_CLK_ENABLE_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#pka_clk_enable */
#define ARM_CRYPTO310_PKA_CLK_ENABLE_VALUE_OFFSET 0
#define ARM_CRYPTO310_PKA_CLK_ENABLE_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#dma_clk_enable */
#define ARM_CRYPTO310_DMA_CLK_ENABLE_VALUE_OFFSET 0
#define ARM_CRYPTO310_DMA_CLK_ENABLE_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#clk_status */
#define ARM_CRYPTO310_CLK_STATUS_AES_CLK_STATUS_OFFSET 0
#define ARM_CRYPTO310_CLK_STATUS_AES_CLK_STATUS_SIZE 1
#define ARM_CRYPTO310_CLK_STATUS_DES_CLK_STATUS_OFFSET 1
#define ARM_CRYPTO310_CLK_STATUS_DES_CLK_STATUS_SIZE 1
#define ARM_CRYPTO310_CLK_STATUS_HASH_CLK_STATUS_OFFSET 2
#define ARM_CRYPTO310_CLK_STATUS_HASH_CLK_STATUS_SIZE 1
#define ARM_CRYPTO310_CLK_STATUS_PKA_CLK_STATUS_OFFSET 3
#define ARM_CRYPTO310_CLK_STATUS_PKA_CLK_STATUS_SIZE 1
#define ARM_CRYPTO310_CLK_STATUS_DMA_CLK_STATUS_OFFSET 8
#define ARM_CRYPTO310_CLK_STATUS_DMA_CLK_STATUS_SIZE 1

/** bit field defines for arm_crypto310_s#chacha_clk_enable */
#define ARM_CRYPTO310_CHACHA_CLK_ENABLE_VALUE_OFFSET 0
#define ARM_CRYPTO310_CHACHA_CLK_ENABLE_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#crypto_ctl */
#define ARM_CRYPTO310_CRYPTO_CTL_VALUE_OFFSET 0
#define ARM_CRYPTO310_CRYPTO_CTL_VALUE_SIZE 5

/** bit field defines for arm_crypto310_s#crypto_busy */
#define ARM_CRYPTO310_CRYPTO_BUSY_VALUE_OFFSET 0
#define ARM_CRYPTO310_CRYPTO_BUSY_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#hash_busy */
#define ARM_CRYPTO310_HASH_BUSY_VALUE_OFFSET 0
#define ARM_CRYPTO310_HASH_BUSY_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#version */
#define ARM_CRYPTO310_VERSION_VALUE_OFFSET 0
#define ARM_CRYPTO310_VERSION_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#host_rgf_irr */
#define ARM_CRYPTO310_HOST_RGF_IRR_SRAM_TO_DIN_INT_OFFSET 4
#define ARM_CRYPTO310_HOST_RGF_IRR_SRAM_TO_DIN_INT_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IRR_DOUT_TO_SRAM_INT_OFFSET 5
#define ARM_CRYPTO310_HOST_RGF_IRR_DOUT_TO_SRAM_INT_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IRR_MEM_TO_DIN_INT_OFFSET 6
#define ARM_CRYPTO310_HOST_RGF_IRR_MEM_TO_DIN_INT_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IRR_DOUT_TO_MEM_INT_OFFSET 7
#define ARM_CRYPTO310_HOST_RGF_IRR_DOUT_TO_MEM_INT_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IRR_AHB_ERR_INT_OFFSET 8
#define ARM_CRYPTO310_HOST_RGF_IRR_AHB_ERR_INT_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IRR_PKA_EXP_INT_OFFSET 9
#define ARM_CRYPTO310_HOST_RGF_IRR_PKA_EXP_INT_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IRR_RNG_INT_OFFSET 10
#define ARM_CRYPTO310_HOST_RGF_IRR_RNG_INT_SIZE 1

/** bit field defines for arm_crypto310_s#host_rgf_imr */
#define ARM_CRYPTO310_HOST_RGF_IMR_SRAM_TO_DIN_MASK_OFFSET 4
#define ARM_CRYPTO310_HOST_RGF_IMR_SRAM_TO_DIN_MASK_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IMR_DOUT_TO_SRAM_MASK_OFFSET 5
#define ARM_CRYPTO310_HOST_RGF_IMR_DOUT_TO_SRAM_MASK_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IMR_MEM_TO_DIN_MASK_OFFSET 6
#define ARM_CRYPTO310_HOST_RGF_IMR_MEM_TO_DIN_MASK_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IMR_DOUT_TO_MEM_MASK_OFFSET 7
#define ARM_CRYPTO310_HOST_RGF_IMR_DOUT_TO_MEM_MASK_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IMR_AHB_ERR_MASK_OFFSET 8
#define ARM_CRYPTO310_HOST_RGF_IMR_AHB_ERR_MASK_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IMR_PKA_EXP_MASK_OFFSET 9
#define ARM_CRYPTO310_HOST_RGF_IMR_PKA_EXP_MASK_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_IMR_RNG_INT_MASK_OFFSET 10
#define ARM_CRYPTO310_HOST_RGF_IMR_RNG_INT_MASK_SIZE 1

/** bit field defines for arm_crypto310_s#host_rgf_icr */
#define ARM_CRYPTO310_HOST_RGF_ICR_SRAM_TO_DIN_CLEAR_OFFSET 4
#define ARM_CRYPTO310_HOST_RGF_ICR_SRAM_TO_DIN_CLEAR_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_ICR_DOUT_TO_SRAM_CLEAR_OFFSET 5
#define ARM_CRYPTO310_HOST_RGF_ICR_DOUT_TO_SRAM_CLEAR_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_ICR_MEM_TO_DIN_CLEAR_OFFSET 6
#define ARM_CRYPTO310_HOST_RGF_ICR_MEM_TO_DIN_CLEAR_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_ICR_DOUT_TO_MEM_CLEAR_OFFSET 7
#define ARM_CRYPTO310_HOST_RGF_ICR_DOUT_TO_MEM_CLEAR_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_ICR_AHB_ERR_CLEAR_OFFSET 8
#define ARM_CRYPTO310_HOST_RGF_ICR_AHB_ERR_CLEAR_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_ICR_PKA_EXP_CLEAR_OFFSET 9
#define ARM_CRYPTO310_HOST_RGF_ICR_PKA_EXP_CLEAR_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_ICR_RNG_INT_CLEAR_OFFSET 10
#define ARM_CRYPTO310_HOST_RGF_ICR_RNG_INT_CLEAR_SIZE 1

/** bit field defines for arm_crypto310_s#host_rgf_endian */
#define ARM_CRYPTO310_HOST_RGF_ENDIAN_DOUT_WR_BG_OFFSET 3
#define ARM_CRYPTO310_HOST_RGF_ENDIAN_DOUT_WR_BG_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_ENDIAN_DIN_RD_BG_OFFSET 7
#define ARM_CRYPTO310_HOST_RGF_ENDIAN_DIN_RD_BG_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_ENDIAN_DOUT_WR_WBG_OFFSET 11
#define ARM_CRYPTO310_HOST_RGF_ENDIAN_DOUT_WR_WBG_SIZE 1
#define ARM_CRYPTO310_HOST_RGF_ENDIAN_DIN_RD_WBG_OFFSET 15
#define ARM_CRYPTO310_HOST_RGF_ENDIAN_DIN_RD_WBG_SIZE 1

/** bit field defines for arm_crypto310_s#host_rgf_cc_sw_rst */
#define ARM_CRYPTO310_HOST_RGF_CC_SW_RST_CC_SW_RST_REQ_OFFSET 0
#define ARM_CRYPTO310_HOST_RGF_CC_SW_RST_CC_SW_RST_REQ_SIZE 1

/** bit field defines for arm_crypto310_s#host_rgf_signature */
#define ARM_CRYPTO310_HOST_RGF_SIGNATURE_VALUE_OFFSET 0
#define ARM_CRYPTO310_HOST_RGF_SIGNATURE_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#host_boot */
#define ARM_CRYPTO310_HOST_BOOT_SYNTHESIS_CONFIG_OFFSET 0
#define ARM_CRYPTO310_HOST_BOOT_SYNTHESIS_CONFIG_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_LARGE_RKEK_LOCAL_OFFSET 1
#define ARM_CRYPTO310_HOST_BOOT_LARGE_RKEK_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_HASH_IN_FUSES_LOCAL_OFFSET 2
#define ARM_CRYPTO310_HOST_BOOT_HASH_IN_FUSES_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_EXT_MEM_SECURED_LOCAL_OFFSET 3
#define ARM_CRYPTO310_HOST_BOOT_EXT_MEM_SECURED_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_RKEK_ECC_EXISTS_LOCAL_N_OFFSET 5
#define ARM_CRYPTO310_HOST_BOOT_RKEK_ECC_EXISTS_LOCAL_N_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_SRAM_SIZE_LOCAL_OFFSET 6
#define ARM_CRYPTO310_HOST_BOOT_SRAM_SIZE_LOCAL_SIZE 2
#define ARM_CRYPTO310_HOST_BOOT_DSCRPTR_EXISTS_LOCAL_OFFSET 9
#define ARM_CRYPTO310_HOST_BOOT_DSCRPTR_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_PAU_EXISTS_LOCAL_OFFSET 10
#define ARM_CRYPTO310_HOST_BOOT_PAU_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_RNG_EXISTS_LOCAL_OFFSET 11
#define ARM_CRYPTO310_HOST_BOOT_RNG_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_PKA_EXISTS_LOCAL_OFFSET 12
#define ARM_CRYPTO310_HOST_BOOT_PKA_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_RC4_EXISTS_LOCAL_OFFSET 13
#define ARM_CRYPTO310_HOST_BOOT_RC4_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_SHA_512_PRSNT_LOCAL_OFFSET 14
#define ARM_CRYPTO310_HOST_BOOT_SHA_512_PRSNT_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_SHA_256_PRSNT_LOCAL_OFFSET 15
#define ARM_CRYPTO310_HOST_BOOT_SHA_256_PRSNT_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_MD5_PRSNT_LOCAL_OFFSET 16
#define ARM_CRYPTO310_HOST_BOOT_MD5_PRSNT_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_HASH_EXISTS_LOCAL_OFFSET 17
#define ARM_CRYPTO310_HOST_BOOT_HASH_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_C2_EXISTS_LOCAL_OFFSET 18
#define ARM_CRYPTO310_HOST_BOOT_C2_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_DES_EXISTS_LOCAL_OFFSET 19
#define ARM_CRYPTO310_HOST_BOOT_DES_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_AES_XCBC_MAC_EXISTS_LOCAL_OFFSET 20
#define ARM_CRYPTO310_HOST_BOOT_AES_XCBC_MAC_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_AES_CMAC_EXISTS_LOCAL_OFFSET 21
#define ARM_CRYPTO310_HOST_BOOT_AES_CMAC_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_AES_CCM_EXISTS_LOCAL_OFFSET 22
#define ARM_CRYPTO310_HOST_BOOT_AES_CCM_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_AES_XEX_HW_T_CALC_LOCAL_OFFSET 23
#define ARM_CRYPTO310_HOST_BOOT_AES_XEX_HW_T_CALC_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_AES_XEX_EXISTS_LOCAL_OFFSET 24
#define ARM_CRYPTO310_HOST_BOOT_AES_XEX_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_CTR_EXISTS_LOCAL_OFFSET 25
#define ARM_CRYPTO310_HOST_BOOT_CTR_EXISTS_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_AES_DIN_BYTE_RESOLUTION_LOCAL_OFFSET 26
#define ARM_CRYPTO310_HOST_BOOT_AES_DIN_BYTE_RESOLUTION_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_TUNNELING_ENB_LOCAL_OFFSET 27
#define ARM_CRYPTO310_HOST_BOOT_TUNNELING_ENB_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_SUPPORT_256_192_KEY_LOCAL_OFFSET 28
#define ARM_CRYPTO310_HOST_BOOT_SUPPORT_256_192_KEY_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_ONLY_ENCRYPT_LOCAL_OFFSET 29
#define ARM_CRYPTO310_HOST_BOOT_ONLY_ENCRYPT_LOCAL_SIZE 1
#define ARM_CRYPTO310_HOST_BOOT_AES_EXISTS_LOCAL_OFFSET 30
#define ARM_CRYPTO310_HOST_BOOT_AES_EXISTS_LOCAL_SIZE 1

/** bit field defines for arm_crypto310_s#host_cryptokey_sel */
#define ARM_CRYPTO310_HOST_CRYPTOKEY_SEL_SEL_CRYPTO_KEY_OFFSET 0
#define ARM_CRYPTO310_HOST_CRYPTOKEY_SEL_SEL_CRYPTO_KEY_SIZE 2

/** bit field defines for arm_crypto310_s#host_iot_kprtl_lock */
#define ARM_CRYPTO310_HOST_IOT_KPRTL_LOCK_VALUE_OFFSET 0
#define ARM_CRYPTO310_HOST_IOT_KPRTL_LOCK_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#host_iot_kdr0 */
#define ARM_CRYPTO310_HOST_IOT_KDR0_VALUE_OFFSET 0
#define ARM_CRYPTO310_HOST_IOT_KDR0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#host_iot_kdr1 */
#define ARM_CRYPTO310_HOST_IOT_KDR1_VALUE_OFFSET 0
#define ARM_CRYPTO310_HOST_IOT_KDR1_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#host_iot_kdr2 */
#define ARM_CRYPTO310_HOST_IOT_KDR2_VALUE_OFFSET 0
#define ARM_CRYPTO310_HOST_IOT_KDR2_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#host_iot_kdr3 */
#define ARM_CRYPTO310_HOST_IOT_KDR3_VALUE_OFFSET 0
#define ARM_CRYPTO310_HOST_IOT_KDR3_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#host_iot_lcs */
#define ARM_CRYPTO310_HOST_IOT_LCS_LCS_OFFSET 0
#define ARM_CRYPTO310_HOST_IOT_LCS_LCS_SIZE 3
#define ARM_CRYPTO310_HOST_IOT_LCS_LCS_IS_VALID_OFFSET 8
#define ARM_CRYPTO310_HOST_IOT_LCS_LCS_IS_VALID_SIZE 1

/** bit field defines for arm_crypto310_s#din_buffer */
#define ARM_CRYPTO310_DIN_BUFFER_VALUE_OFFSET 0
#define ARM_CRYPTO310_DIN_BUFFER_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#din_mem_dma_busy */
#define ARM_CRYPTO310_DIN_MEM_DMA_BUSY_VALUE_OFFSET 0
#define ARM_CRYPTO310_DIN_MEM_DMA_BUSY_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#src_lli_sram_addr */
#define ARM_CRYPTO310_SRC_LLI_SRAM_ADDR_VALUE_OFFSET 0
#define ARM_CRYPTO310_SRC_LLI_SRAM_ADDR_VALUE_SIZE 15

/** bit field defines for arm_crypto310_s#src_lli_word0 */
#define ARM_CRYPTO310_SRC_LLI_WORD0_VALUE_OFFSET 0
#define ARM_CRYPTO310_SRC_LLI_WORD0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#src_lli_word1 */
#define ARM_CRYPTO310_SRC_LLI_WORD1_BYTES_NUM_OFFSET 0
#define ARM_CRYPTO310_SRC_LLI_WORD1_BYTES_NUM_SIZE 30
#define ARM_CRYPTO310_SRC_LLI_WORD1_FIRST_OFFSET 30
#define ARM_CRYPTO310_SRC_LLI_WORD1_FIRST_SIZE 1
#define ARM_CRYPTO310_SRC_LLI_WORD1_LAST_OFFSET 31
#define ARM_CRYPTO310_SRC_LLI_WORD1_LAST_SIZE 1

/** bit field defines for arm_crypto310_s#sram_src_addr */
#define ARM_CRYPTO310_SRAM_SRC_ADDR_VALUE_OFFSET 0
#define ARM_CRYPTO310_SRAM_SRC_ADDR_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#din_sram_bytes_len */
#define ARM_CRYPTO310_DIN_SRAM_BYTES_LEN_VALUE_OFFSET 0
#define ARM_CRYPTO310_DIN_SRAM_BYTES_LEN_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#din_sram_dma_busy */
#define ARM_CRYPTO310_DIN_SRAM_DMA_BUSY_VALUE_OFFSET 0
#define ARM_CRYPTO310_DIN_SRAM_DMA_BUSY_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#din_sram_endianness */
#define ARM_CRYPTO310_DIN_SRAM_ENDIANNESS_VALUE_OFFSET 0
#define ARM_CRYPTO310_DIN_SRAM_ENDIANNESS_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#din_sw_reset */
#define ARM_CRYPTO310_DIN_SW_RESET_VALUE_OFFSET 0
#define ARM_CRYPTO310_DIN_SW_RESET_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#din_cpu_data_size */
#define ARM_CRYPTO310_DIN_CPU_DATA_SIZE_VALUE_OFFSET 0
#define ARM_CRYPTO310_DIN_CPU_DATA_SIZE_VALUE_SIZE 16

/** bit field defines for arm_crypto310_s#write_align_last */
#define ARM_CRYPTO310_WRITE_ALIGN_LAST_VALUE_OFFSET 0
#define ARM_CRYPTO310_WRITE_ALIGN_LAST_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#fifo_in_empty */
#define ARM_CRYPTO310_FIFO_IN_EMPTY_VALUE_OFFSET 0
#define ARM_CRYPTO310_FIFO_IN_EMPTY_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#disable_outstd_req */
#define ARM_CRYPTO310_DISABLE_OUTSTD_REQ_VALUE_OFFSET 0
#define ARM_CRYPTO310_DISABLE_OUTSTD_REQ_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#din_fifo_rst_pntr */
#define ARM_CRYPTO310_DIN_FIFO_RST_PNTR_VALUE_OFFSET 0
#define ARM_CRYPTO310_DIN_FIFO_RST_PNTR_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#dout_buffer */
#define ARM_CRYPTO310_DOUT_BUFFER_VALUE_OFFSET 0
#define ARM_CRYPTO310_DOUT_BUFFER_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#dout_mem_dma_busy */
#define ARM_CRYPTO310_DOUT_MEM_DMA_BUSY_VALUE_OFFSET 0
#define ARM_CRYPTO310_DOUT_MEM_DMA_BUSY_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#dst_lli_sram_addr */
#define ARM_CRYPTO310_DST_LLI_SRAM_ADDR_VALUE_OFFSET 0
#define ARM_CRYPTO310_DST_LLI_SRAM_ADDR_VALUE_SIZE 15

/** bit field defines for arm_crypto310_s#dst_lli_word0 */
#define ARM_CRYPTO310_DST_LLI_WORD0_VALUE_OFFSET 0
#define ARM_CRYPTO310_DST_LLI_WORD0_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#dst_lli_word1 */
#define ARM_CRYPTO310_DST_LLI_WORD1_BYTES_NUM_OFFSET 0
#define ARM_CRYPTO310_DST_LLI_WORD1_BYTES_NUM_SIZE 30
#define ARM_CRYPTO310_DST_LLI_WORD1_FIRST_OFFSET 30
#define ARM_CRYPTO310_DST_LLI_WORD1_FIRST_SIZE 1
#define ARM_CRYPTO310_DST_LLI_WORD1_LAST_OFFSET 31
#define ARM_CRYPTO310_DST_LLI_WORD1_LAST_SIZE 1

/** bit field defines for arm_crypto310_s#sram_dest_addr */
#define ARM_CRYPTO310_SRAM_DEST_ADDR_VALUE_OFFSET 0
#define ARM_CRYPTO310_SRAM_DEST_ADDR_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#dout_sram_bytes_len */
#define ARM_CRYPTO310_DOUT_SRAM_BYTES_LEN_VALUE_OFFSET 0
#define ARM_CRYPTO310_DOUT_SRAM_BYTES_LEN_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#dout_sram_dma_busy */
#define ARM_CRYPTO310_DOUT_SRAM_DMA_BUSY_VALUE_OFFSET 0
#define ARM_CRYPTO310_DOUT_SRAM_DMA_BUSY_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#dout_sram_endianness */
#define ARM_CRYPTO310_DOUT_SRAM_ENDIANNESS_VALUE_OFFSET 0
#define ARM_CRYPTO310_DOUT_SRAM_ENDIANNESS_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#read_align_last */
#define ARM_CRYPTO310_READ_ALIGN_LAST_VALUE_OFFSET 0
#define ARM_CRYPTO310_READ_ALIGN_LAST_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#dout_fifo_empty */
#define ARM_CRYPTO310_DOUT_FIFO_EMPTY_VALUE_OFFSET 0
#define ARM_CRYPTO310_DOUT_FIFO_EMPTY_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#dout_sw_reset */
#define ARM_CRYPTO310_DOUT_SW_RESET_VALUE_OFFSET 0
#define ARM_CRYPTO310_DOUT_SW_RESET_VALUE_SIZE 1

/** bit field defines for arm_crypto310_s#sram_data */
#define ARM_CRYPTO310_SRAM_DATA_VALUE_OFFSET 0
#define ARM_CRYPTO310_SRAM_DATA_VALUE_SIZE 32

/** bit field defines for arm_crypto310_s#sram_addr */
#define ARM_CRYPTO310_SRAM_ADDR_VALUE_OFFSET 0
#define ARM_CRYPTO310_SRAM_ADDR_VALUE_SIZE 15

/** bit field defines for arm_crypto310_s#sram_data_ready */
#define ARM_CRYPTO310_SRAM_DATA_READY_VALUE_OFFSET 0
#define ARM_CRYPTO310_SRAM_DATA_READY_VALUE_SIZE 1

/* EOF arm_crypto310.h */
#endif
