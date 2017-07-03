/***************************************************************************//**
* \file cyip_flashc.h
*
* \brief
* FLASHC IP definitions
*
* \note
* Generated 2/9/2017 by CyDeviceHeaderGenerator v1.1.0.47
* from the register map configuration rev#961378
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CYIP_FLASHC_H_
#define _CYIP_FLASHC_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                          FLASHC                                           ================ */
/* =========================================================================================================================== */

#define FLASHC_FM_CTL_SECTION_SIZE  0x00001000UL
#define FLASHC_SECTION_SIZE         0x00010000UL

/**
  * \brief Flash Macro Registers (FLASHC_FM_CTL)
  */
typedef struct {
  __IOM uint32_t FM_CTL;                        /*!< 0x00000000 Flash macro control                                            */
   __IM uint32_t STATUS;                        /*!< 0x00000004 Status                                                         */
  __IOM uint32_t FM_ADDR;                       /*!< 0x00000008 Flash macro  address                                           */
   __IM uint32_t GEOMETRY;                      /*!< 0x0000000C Regular flash geometry                                         */
   __IM uint32_t GEOMETRY_SUPERVISORY;          /*!< 0x00000010 Supervisory flash geometry                                     */
  __IOM uint32_t TIMER_CTL;                     /*!< 0x00000014 Timer control                                                  */
  __IOM uint32_t ANA_CTL0;                      /*!< 0x00000018 Analog control 0                                               */
  __IOM uint32_t ANA_CTL1;                      /*!< 0x0000001C Analog control 1                                               */
   __IM uint32_t GEOMETRY_GEN;                  /*!< 0x00000020 General FM Geometry Info                                       */
  __IOM uint32_t TEST_CTL;                      /*!< 0x00000024 Test mode control                                              */
  __IOM uint32_t WAIT_CTL;                      /*!< 0x00000028 Wiat State control                                             */
   __IM uint32_t MONITOR_STATUS;                /*!< 0x0000002C Monitor Status                                                 */
  __IOM uint32_t SCRATCH_CTL;                   /*!< 0x00000030 Scratch Control                                                */
  __IOM uint32_t HV_CTL;                        /*!< 0x00000034 High voltage control                                           */
   __OM uint32_t ACLK_CTL;                      /*!< 0x00000038 Aclk control                                                   */
  __IOM uint32_t INTR;                          /*!< 0x0000003C Interrupt                                                      */
  __IOM uint32_t INTR_SET;                      /*!< 0x00000040 Interrupt set                                                  */
  __IOM uint32_t INTR_MASK;                     /*!< 0x00000044 Interrupt mask                                                 */
   __IM uint32_t INTR_MASKED;                   /*!< 0x00000048 Interrupt masked                                               */
   __OM uint32_t FM_HV_DATA_ALL;                /*!< 0x0000004C Flash macro high Voltage page latches data (for all page
                                                                latches)                                                       */
  __IOM uint32_t CAL_CTL0;                      /*!< 0x00000050 Cal control BG LO trim bits                                    */
  __IOM uint32_t CAL_CTL1;                      /*!< 0x00000054 Cal control  BG HI trim bits                                   */
  __IOM uint32_t CAL_CTL2;                      /*!< 0x00000058 Cal control BG LO&HI ipref trim, ref sel, fm_active, turbo_ext */
  __IOM uint32_t CAL_CTL3;                      /*!< 0x0000005C Cal control osc trim bits, idac, sdac, itim, bdac.             */
   __OM uint32_t BOOKMARK;                      /*!< 0x00000060 Bookmark register - keeps the current FW HV seq                */
   __IM uint32_t RESERVED[7];
  __IOM uint32_t RED_CTL01;                     /*!< 0x00000080 Redundancy Control normal sectors 0,1                          */
  __IOM uint32_t RED_CTL23;                     /*!< 0x00000084 Redundancy Controll normal sectors 2,3                         */
  __IOM uint32_t RED_CTL45;                     /*!< 0x00000088 Redundancy Controll normal sectors 4,5                         */
  __IOM uint32_t RED_CTL67;                     /*!< 0x0000008C Redundancy Controll normal sectors 6,7                         */
  __IOM uint32_t RED_CTL_SM01;                  /*!< 0x00000090 Redundancy Controll special sectors 0,1                        */
   __IM uint32_t RESERVED1[27];
   __IM uint32_t TM_CMPR[32];                   /*!< 0x00000100 Comparison between FM data out and Page Latches on each
                                                                column.                                                        */
   __IM uint32_t RESERVED2[416];
  __IOM uint32_t FM_HV_DATA[256];               /*!< 0x00000800 Flash macro high Voltage page latches data                     */
   __IM uint32_t FM_MEM_DATA[256];              /*!< 0x00000C00 Flash macro memory sense amplifier and column decoder data     */
} FLASHC_FM_CTL_Type;                           /*!< Size = 4096 (0x1000)                                                      */

/**
  * \brief Flash controller (FLASHC)
  */
typedef struct {                                /*!< FLASHC Structure                                                          */
  __IOM uint32_t FLASH_CTL;                     /*!< 0x00000000 Control                                                        */
  __IOM uint32_t FLASH_PWR_CTL;                 /*!< 0x00000004 Flash power control                                            */
  __IOM uint32_t FLASH_CMD;                     /*!< 0x00000008 Command                                                        */
   __IM uint32_t RESERVED[253];
  __IOM uint32_t CM0_CA_CTL0;                   /*!< 0x00000400 CM0+ cache control                                             */
  __IOM uint32_t CM0_CA_CTL1;                   /*!< 0x00000404 CM0+ cache control                                             */
  __IOM uint32_t CM0_CA_CMD;                    /*!< 0x00000408 CM0+ cache command                                             */
   __IM uint32_t RESERVED1[13];
   __IM uint32_t CM0_CA_STATUS0;                /*!< 0x00000440 CM0+ cache status 0                                            */
   __IM uint32_t CM0_CA_STATUS1;                /*!< 0x00000444 CM0+ cache status 1                                            */
   __IM uint32_t CM0_CA_STATUS2;                /*!< 0x00000448 CM0+ cache status 2                                            */
   __IM uint32_t RESERVED2[13];
  __IOM uint32_t CM4_CA_CTL0;                   /*!< 0x00000480 CM4 cache control                                              */
  __IOM uint32_t CM4_CA_CTL1;                   /*!< 0x00000484 CM0+ cache control                                             */
  __IOM uint32_t CM4_CA_CMD;                    /*!< 0x00000488 CM4 cache command                                              */
   __IM uint32_t RESERVED3[13];
   __IM uint32_t CM4_CA_STATUS0;                /*!< 0x000004C0 CM4 cache status 0                                             */
   __IM uint32_t CM4_CA_STATUS1;                /*!< 0x000004C4 CM4 cache status 1                                             */
   __IM uint32_t CM4_CA_STATUS2;                /*!< 0x000004C8 CM4 cache status 2                                             */
   __IM uint32_t RESERVED4[13];
  __IOM uint32_t CRYPTO_BUFF_CTL;               /*!< 0x00000500 Cryptography buffer control                                    */
   __IM uint32_t RESERVED5;
  __IOM uint32_t CRYPTO_BUFF_CMD;               /*!< 0x00000508 Cryptography buffer command                                    */
   __IM uint32_t RESERVED6[29];
  __IOM uint32_t DW0_BUFF_CTL;                  /*!< 0x00000580 Datawire 0 buffer control                                      */
   __IM uint32_t RESERVED7;
  __IOM uint32_t DW0_BUFF_CMD;                  /*!< 0x00000588 Datawire 0 buffer command                                      */
   __IM uint32_t RESERVED8[29];
  __IOM uint32_t DW1_BUFF_CTL;                  /*!< 0x00000600 Datawire 1 buffer control                                      */
   __IM uint32_t RESERVED9;
  __IOM uint32_t DW1_BUFF_CMD;                  /*!< 0x00000608 Datawire 1 buffer command                                      */
   __IM uint32_t RESERVED10[29];
  __IOM uint32_t DAP_BUFF_CTL;                  /*!< 0x00000680 Debug access port buffer control                               */
   __IM uint32_t RESERVED11;
  __IOM uint32_t DAP_BUFF_CMD;                  /*!< 0x00000688 Debug access port buffer command                               */
   __IM uint32_t RESERVED12[29];
  __IOM uint32_t EXT_MS0_BUFF_CTL;              /*!< 0x00000700 External master 0 buffer control                               */
   __IM uint32_t RESERVED13;
  __IOM uint32_t EXT_MS0_BUFF_CMD;              /*!< 0x00000708 External master 0 buffer command                               */
   __IM uint32_t RESERVED14[29];
  __IOM uint32_t EXT_MS1_BUFF_CTL;              /*!< 0x00000780 External master 1 buffer control                               */
   __IM uint32_t RESERVED15;
  __IOM uint32_t EXT_MS1_BUFF_CMD;              /*!< 0x00000788 External master 1 buffer command                               */
   __IM uint32_t RESERVED16[14877];
        FLASHC_FM_CTL_Type FM_CTL;              /*!< 0x0000F000 Flash Macro Registers                                          */
} FLASHC_Type;                                  /*!< Size = 65536 (0x10000)                                                    */


/* =================================================  FLASHC_FM_CTL.FM_CTL  ================================================== */
#define FLASHC_FM_CTL_FM_CTL_FM_MODE_Pos  (0UL)                 /*!< FLASHC_FM_CTL FM_CTL: FM_MODE (Bit 0)                     */
#define FLASHC_FM_CTL_FM_CTL_FM_MODE_Msk  (0xfUL)               /*!< FLASHC_FM_CTL FM_CTL: FM_MODE (Bitfield-Mask: 0x0f)       */
#define FLASHC_FM_CTL_FM_CTL_FM_SEQ_Pos   (8UL)                 /*!< FLASHC_FM_CTL FM_CTL: FM_SEQ (Bit 8)                      */
#define FLASHC_FM_CTL_FM_CTL_FM_SEQ_Msk   (0x300UL)             /*!< FLASHC_FM_CTL FM_CTL: FM_SEQ (Bitfield-Mask: 0x03)        */
#define FLASHC_FM_CTL_FM_CTL_DAA_MUX_SEL_Pos (16UL)             /*!< FLASHC_FM_CTL FM_CTL: DAA_MUX_SEL (Bit 16)                */
#define FLASHC_FM_CTL_FM_CTL_DAA_MUX_SEL_Msk (0x7f0000UL)       /*!< FLASHC_FM_CTL FM_CTL: DAA_MUX_SEL (Bitfield-Mask: 0x7f)   */
#define FLASHC_FM_CTL_FM_CTL_IF_SEL_Pos   (24UL)                /*!< FLASHC_FM_CTL FM_CTL: IF_SEL (Bit 24)                     */
#define FLASHC_FM_CTL_FM_CTL_IF_SEL_Msk   (0x1000000UL)         /*!< FLASHC_FM_CTL FM_CTL: IF_SEL (Bitfield-Mask: 0x01)        */
#define FLASHC_FM_CTL_FM_CTL_WR_EN_Pos    (25UL)                /*!< FLASHC_FM_CTL FM_CTL: WR_EN (Bit 25)                      */
#define FLASHC_FM_CTL_FM_CTL_WR_EN_Msk    (0x2000000UL)         /*!< FLASHC_FM_CTL FM_CTL: WR_EN (Bitfield-Mask: 0x01)         */
/* =================================================  FLASHC_FM_CTL.STATUS  ================================================== */
#define FLASHC_FM_CTL_STATUS_HV_TIMER_RUNNING_Pos (0UL)         /*!< FLASHC_FM_CTL STATUS: HV_TIMER_RUNNING (Bit 0)            */
#define FLASHC_FM_CTL_STATUS_HV_TIMER_RUNNING_Msk (0x1UL)       /*!< FLASHC_FM_CTL STATUS: HV_TIMER_RUNNING (Bitfield-Mask: 0x01)*/
#define FLASHC_FM_CTL_STATUS_HV_REGS_ISOLATED_Pos (1UL)         /*!< FLASHC_FM_CTL STATUS: HV_REGS_ISOLATED (Bit 1)            */
#define FLASHC_FM_CTL_STATUS_HV_REGS_ISOLATED_Msk (0x2UL)       /*!< FLASHC_FM_CTL STATUS: HV_REGS_ISOLATED (Bitfield-Mask: 0x01)*/
#define FLASHC_FM_CTL_STATUS_ILLEGAL_HVOP_Pos (2UL)             /*!< FLASHC_FM_CTL STATUS: ILLEGAL_HVOP (Bit 2)                */
#define FLASHC_FM_CTL_STATUS_ILLEGAL_HVOP_Msk (0x4UL)           /*!< FLASHC_FM_CTL STATUS: ILLEGAL_HVOP (Bitfield-Mask: 0x01)  */
#define FLASHC_FM_CTL_STATUS_TURBO_N_Pos  (3UL)                 /*!< FLASHC_FM_CTL STATUS: TURBO_N (Bit 3)                     */
#define FLASHC_FM_CTL_STATUS_TURBO_N_Msk  (0x8UL)               /*!< FLASHC_FM_CTL STATUS: TURBO_N (Bitfield-Mask: 0x01)       */
#define FLASHC_FM_CTL_STATUS_WR_EN_MON_Pos (4UL)                /*!< FLASHC_FM_CTL STATUS: WR_EN_MON (Bit 4)                   */
#define FLASHC_FM_CTL_STATUS_WR_EN_MON_Msk (0x10UL)             /*!< FLASHC_FM_CTL STATUS: WR_EN_MON (Bitfield-Mask: 0x01)     */
#define FLASHC_FM_CTL_STATUS_IF_SEL_MON_Pos (5UL)               /*!< FLASHC_FM_CTL STATUS: IF_SEL_MON (Bit 5)                  */
#define FLASHC_FM_CTL_STATUS_IF_SEL_MON_Msk (0x20UL)            /*!< FLASHC_FM_CTL STATUS: IF_SEL_MON (Bitfield-Mask: 0x01)    */
/* =================================================  FLASHC_FM_CTL.FM_ADDR  ================================================= */
#define FLASHC_FM_CTL_FM_ADDR_RA_Pos      (0UL)                 /*!< FLASHC_FM_CTL FM_ADDR: RA (Bit 0)                         */
#define FLASHC_FM_CTL_FM_ADDR_RA_Msk      (0xffffUL)            /*!< FLASHC_FM_CTL FM_ADDR: RA (Bitfield-Mask: 0xffff)         */
#define FLASHC_FM_CTL_FM_ADDR_BA_Pos      (16UL)                /*!< FLASHC_FM_CTL FM_ADDR: BA (Bit 16)                        */
#define FLASHC_FM_CTL_FM_ADDR_BA_Msk      (0xff0000UL)          /*!< FLASHC_FM_CTL FM_ADDR: BA (Bitfield-Mask: 0xff)           */
#define FLASHC_FM_CTL_FM_ADDR_AXA_Pos     (24UL)                /*!< FLASHC_FM_CTL FM_ADDR: AXA (Bit 24)                       */
#define FLASHC_FM_CTL_FM_ADDR_AXA_Msk     (0x1000000UL)         /*!< FLASHC_FM_CTL FM_ADDR: AXA (Bitfield-Mask: 0x01)          */
/* ================================================  FLASHC_FM_CTL.GEOMETRY  ================================================= */
#define FLASHC_FM_CTL_GEOMETRY_WORD_SIZE_LOG2_Pos (0UL)         /*!< FLASHC_FM_CTL GEOMETRY: WORD_SIZE_LOG2 (Bit 0)            */
#define FLASHC_FM_CTL_GEOMETRY_WORD_SIZE_LOG2_Msk (0xfUL)       /*!< FLASHC_FM_CTL GEOMETRY: WORD_SIZE_LOG2 (Bitfield-Mask: 0x0f)*/
#define FLASHC_FM_CTL_GEOMETRY_PAGE_SIZE_LOG2_Pos (4UL)         /*!< FLASHC_FM_CTL GEOMETRY: PAGE_SIZE_LOG2 (Bit 4)            */
#define FLASHC_FM_CTL_GEOMETRY_PAGE_SIZE_LOG2_Msk (0xf0UL)      /*!< FLASHC_FM_CTL GEOMETRY: PAGE_SIZE_LOG2 (Bitfield-Mask: 0x0f)*/
#define FLASHC_FM_CTL_GEOMETRY_ROW_COUNT_Pos (8UL)              /*!< FLASHC_FM_CTL GEOMETRY: ROW_COUNT (Bit 8)                 */
#define FLASHC_FM_CTL_GEOMETRY_ROW_COUNT_Msk (0xffff00UL)       /*!< FLASHC_FM_CTL GEOMETRY: ROW_COUNT (Bitfield-Mask: 0xffff) */
#define FLASHC_FM_CTL_GEOMETRY_BANK_COUNT_Pos (24UL)            /*!< FLASHC_FM_CTL GEOMETRY: BANK_COUNT (Bit 24)               */
#define FLASHC_FM_CTL_GEOMETRY_BANK_COUNT_Msk (0xff000000UL)    /*!< FLASHC_FM_CTL GEOMETRY: BANK_COUNT (Bitfield-Mask: 0xff)  */
/* ==========================================  FLASHC_FM_CTL.GEOMETRY_SUPERVISORY  =========================================== */
#define FLASHC_FM_CTL_GEOMETRY_SUPERVISORY_WORD_SIZE_LOG2_Pos (0UL) /*!< FLASHC_FM_CTL GEOMETRY_SUPERVISORY: WORD_SIZE_LOG2 (Bit 0)*/
#define FLASHC_FM_CTL_GEOMETRY_SUPERVISORY_WORD_SIZE_LOG2_Msk (0xfUL) /*!< FLASHC_FM_CTL GEOMETRY_SUPERVISORY: WORD_SIZE_LOG2 (Bitfield-Mask: 0x0f)*/
#define FLASHC_FM_CTL_GEOMETRY_SUPERVISORY_PAGE_SIZE_LOG2_Pos (4UL) /*!< FLASHC_FM_CTL GEOMETRY_SUPERVISORY: PAGE_SIZE_LOG2 (Bit 4)*/
#define FLASHC_FM_CTL_GEOMETRY_SUPERVISORY_PAGE_SIZE_LOG2_Msk (0xf0UL) /*!< FLASHC_FM_CTL GEOMETRY_SUPERVISORY: PAGE_SIZE_LOG2 (Bitfield-Mask: 0x0f)*/
#define FLASHC_FM_CTL_GEOMETRY_SUPERVISORY_ROW_COUNT_Pos (8UL)  /*!< FLASHC_FM_CTL GEOMETRY_SUPERVISORY: ROW_COUNT (Bit 8)     */
#define FLASHC_FM_CTL_GEOMETRY_SUPERVISORY_ROW_COUNT_Msk (0xffff00UL) /*!< FLASHC_FM_CTL GEOMETRY_SUPERVISORY: ROW_COUNT (Bitfield-Mask: 0xffff)*/
#define FLASHC_FM_CTL_GEOMETRY_SUPERVISORY_BANK_COUNT_Pos (24UL) /*!< FLASHC_FM_CTL GEOMETRY_SUPERVISORY: BANK_COUNT (Bit 24)  */
#define FLASHC_FM_CTL_GEOMETRY_SUPERVISORY_BANK_COUNT_Msk (0xff000000UL) /*!< FLASHC_FM_CTL GEOMETRY_SUPERVISORY: BANK_COUNT (Bitfield-Mask: 0xff)*/
/* ================================================  FLASHC_FM_CTL.TIMER_CTL  ================================================ */
#define FLASHC_FM_CTL_TIMER_CTL_PERIOD_Pos (0UL)                /*!< FLASHC_FM_CTL TIMER_CTL: PERIOD (Bit 0)                   */
#define FLASHC_FM_CTL_TIMER_CTL_PERIOD_Msk (0xffffUL)           /*!< FLASHC_FM_CTL TIMER_CTL: PERIOD (Bitfield-Mask: 0xffff)   */
#define FLASHC_FM_CTL_TIMER_CTL_SCALE_Pos (16UL)                /*!< FLASHC_FM_CTL TIMER_CTL: SCALE (Bit 16)                   */
#define FLASHC_FM_CTL_TIMER_CTL_SCALE_Msk (0x10000UL)           /*!< FLASHC_FM_CTL TIMER_CTL: SCALE (Bitfield-Mask: 0x01)      */
#define FLASHC_FM_CTL_TIMER_CTL_PUMP_CLOCK_SEL_Pos (24UL)       /*!< FLASHC_FM_CTL TIMER_CTL: PUMP_CLOCK_SEL (Bit 24)          */
#define FLASHC_FM_CTL_TIMER_CTL_PUMP_CLOCK_SEL_Msk (0x1000000UL) /*!< FLASHC_FM_CTL TIMER_CTL: PUMP_CLOCK_SEL (Bitfield-Mask: 0x01)*/
#define FLASHC_FM_CTL_TIMER_CTL_PRE_PROG_Pos (25UL)             /*!< FLASHC_FM_CTL TIMER_CTL: PRE_PROG (Bit 25)                */
#define FLASHC_FM_CTL_TIMER_CTL_PRE_PROG_Msk (0x2000000UL)      /*!< FLASHC_FM_CTL TIMER_CTL: PRE_PROG (Bitfield-Mask: 0x01)   */
#define FLASHC_FM_CTL_TIMER_CTL_PRE_PROG_CSL_Pos (26UL)         /*!< FLASHC_FM_CTL TIMER_CTL: PRE_PROG_CSL (Bit 26)            */
#define FLASHC_FM_CTL_TIMER_CTL_PRE_PROG_CSL_Msk (0x4000000UL)  /*!< FLASHC_FM_CTL TIMER_CTL: PRE_PROG_CSL (Bitfield-Mask: 0x01)*/
#define FLASHC_FM_CTL_TIMER_CTL_PUMP_EN_Pos (29UL)              /*!< FLASHC_FM_CTL TIMER_CTL: PUMP_EN (Bit 29)                 */
#define FLASHC_FM_CTL_TIMER_CTL_PUMP_EN_Msk (0x20000000UL)      /*!< FLASHC_FM_CTL TIMER_CTL: PUMP_EN (Bitfield-Mask: 0x01)    */
#define FLASHC_FM_CTL_TIMER_CTL_ACLK_EN_Pos (30UL)              /*!< FLASHC_FM_CTL TIMER_CTL: ACLK_EN (Bit 30)                 */
#define FLASHC_FM_CTL_TIMER_CTL_ACLK_EN_Msk (0x40000000UL)      /*!< FLASHC_FM_CTL TIMER_CTL: ACLK_EN (Bitfield-Mask: 0x01)    */
#define FLASHC_FM_CTL_TIMER_CTL_TIMER_EN_Pos (31UL)             /*!< FLASHC_FM_CTL TIMER_CTL: TIMER_EN (Bit 31)                */
#define FLASHC_FM_CTL_TIMER_CTL_TIMER_EN_Msk (0x80000000UL)     /*!< FLASHC_FM_CTL TIMER_CTL: TIMER_EN (Bitfield-Mask: 0x01)   */
/* ================================================  FLASHC_FM_CTL.ANA_CTL0  ================================================= */
#define FLASHC_FM_CTL_ANA_CTL0_CSLDAC_Pos (8UL)                 /*!< FLASHC_FM_CTL ANA_CTL0: CSLDAC (Bit 8)                    */
#define FLASHC_FM_CTL_ANA_CTL0_CSLDAC_Msk (0x700UL)             /*!< FLASHC_FM_CTL ANA_CTL0: CSLDAC (Bitfield-Mask: 0x07)      */
#define FLASHC_FM_CTL_ANA_CTL0_VCC_SEL_Pos (24UL)               /*!< FLASHC_FM_CTL ANA_CTL0: VCC_SEL (Bit 24)                  */
#define FLASHC_FM_CTL_ANA_CTL0_VCC_SEL_Msk (0x1000000UL)        /*!< FLASHC_FM_CTL ANA_CTL0: VCC_SEL (Bitfield-Mask: 0x01)     */
#define FLASHC_FM_CTL_ANA_CTL0_FLIP_AMUXBUS_AB_Pos (27UL)       /*!< FLASHC_FM_CTL ANA_CTL0: FLIP_AMUXBUS_AB (Bit 27)          */
#define FLASHC_FM_CTL_ANA_CTL0_FLIP_AMUXBUS_AB_Msk (0x8000000UL) /*!< FLASHC_FM_CTL ANA_CTL0: FLIP_AMUXBUS_AB (Bitfield-Mask: 0x01)*/
/* ================================================  FLASHC_FM_CTL.ANA_CTL1  ================================================= */
#define FLASHC_FM_CTL_ANA_CTL1_MDAC_Pos   (0UL)                 /*!< FLASHC_FM_CTL ANA_CTL1: MDAC (Bit 0)                      */
#define FLASHC_FM_CTL_ANA_CTL1_MDAC_Msk   (0xffUL)              /*!< FLASHC_FM_CTL ANA_CTL1: MDAC (Bitfield-Mask: 0xff)        */
#define FLASHC_FM_CTL_ANA_CTL1_PDAC_Pos   (16UL)                /*!< FLASHC_FM_CTL ANA_CTL1: PDAC (Bit 16)                     */
#define FLASHC_FM_CTL_ANA_CTL1_PDAC_Msk   (0xf0000UL)           /*!< FLASHC_FM_CTL ANA_CTL1: PDAC (Bitfield-Mask: 0x0f)        */
#define FLASHC_FM_CTL_ANA_CTL1_NDAC_Pos   (24UL)                /*!< FLASHC_FM_CTL ANA_CTL1: NDAC (Bit 24)                     */
#define FLASHC_FM_CTL_ANA_CTL1_NDAC_Msk   (0xf000000UL)         /*!< FLASHC_FM_CTL ANA_CTL1: NDAC (Bitfield-Mask: 0x0f)        */
#define FLASHC_FM_CTL_ANA_CTL1_VPROT_OVERRIDE_Pos (28UL)        /*!< FLASHC_FM_CTL ANA_CTL1: VPROT_OVERRIDE (Bit 28)           */
#define FLASHC_FM_CTL_ANA_CTL1_VPROT_OVERRIDE_Msk (0x10000000UL) /*!< FLASHC_FM_CTL ANA_CTL1: VPROT_OVERRIDE (Bitfield-Mask: 0x01)*/
#define FLASHC_FM_CTL_ANA_CTL1_SPARE2_Pos (29UL)                /*!< FLASHC_FM_CTL ANA_CTL1: SPARE2 (Bit 29)                   */
#define FLASHC_FM_CTL_ANA_CTL1_SPARE2_Msk (0x20000000UL)        /*!< FLASHC_FM_CTL ANA_CTL1: SPARE2 (Bitfield-Mask: 0x01)      */
#define FLASHC_FM_CTL_ANA_CTL1_RST_SFT_HVPL_Pos (30UL)          /*!< FLASHC_FM_CTL ANA_CTL1: RST_SFT_HVPL (Bit 30)             */
#define FLASHC_FM_CTL_ANA_CTL1_RST_SFT_HVPL_Msk (0x40000000UL)  /*!< FLASHC_FM_CTL ANA_CTL1: RST_SFT_HVPL (Bitfield-Mask: 0x01)*/
/* ==============================================  FLASHC_FM_CTL.GEOMETRY_GEN  =============================================== */
#define FLASHC_FM_CTL_GEOMETRY_GEN_RWW_Pos (1UL)                /*!< FLASHC_FM_CTL GEOMETRY_GEN: RWW (Bit 1)                   */
#define FLASHC_FM_CTL_GEOMETRY_GEN_RWW_Msk (0x2UL)              /*!< FLASHC_FM_CTL GEOMETRY_GEN: RWW (Bitfield-Mask: 0x01)     */
#define FLASHC_FM_CTL_GEOMETRY_GEN_MAX_DOUT_WIDTH_Pos (2UL)     /*!< FLASHC_FM_CTL GEOMETRY_GEN: MAX_DOUT_WIDTH (Bit 2)        */
#define FLASHC_FM_CTL_GEOMETRY_GEN_MAX_DOUT_WIDTH_Msk (0x4UL)   /*!< FLASHC_FM_CTL GEOMETRY_GEN: MAX_DOUT_WIDTH (Bitfield-Mask: 0x01)*/
#define FLASHC_FM_CTL_GEOMETRY_GEN_SECTOR0_SR_Pos (3UL)         /*!< FLASHC_FM_CTL GEOMETRY_GEN: SECTOR0_SR (Bit 3)            */
#define FLASHC_FM_CTL_GEOMETRY_GEN_SECTOR0_SR_Msk (0x8UL)       /*!< FLASHC_FM_CTL GEOMETRY_GEN: SECTOR0_SR (Bitfield-Mask: 0x01)*/
/* ================================================  FLASHC_FM_CTL.TEST_CTL  ================================================= */
#define FLASHC_FM_CTL_TEST_CTL_TEST_MODE_Pos (0UL)              /*!< FLASHC_FM_CTL TEST_CTL: TEST_MODE (Bit 0)                 */
#define FLASHC_FM_CTL_TEST_CTL_TEST_MODE_Msk (0x1fUL)           /*!< FLASHC_FM_CTL TEST_CTL: TEST_MODE (Bitfield-Mask: 0x1f)   */
#define FLASHC_FM_CTL_TEST_CTL_PN_CTL_Pos (8UL)                 /*!< FLASHC_FM_CTL TEST_CTL: PN_CTL (Bit 8)                    */
#define FLASHC_FM_CTL_TEST_CTL_PN_CTL_Msk (0x100UL)             /*!< FLASHC_FM_CTL TEST_CTL: PN_CTL (Bitfield-Mask: 0x01)      */
#define FLASHC_FM_CTL_TEST_CTL_TM_PE_Pos  (9UL)                 /*!< FLASHC_FM_CTL TEST_CTL: TM_PE (Bit 9)                     */
#define FLASHC_FM_CTL_TEST_CTL_TM_PE_Msk  (0x200UL)             /*!< FLASHC_FM_CTL TEST_CTL: TM_PE (Bitfield-Mask: 0x01)       */
#define FLASHC_FM_CTL_TEST_CTL_TM_DISPOS_Pos (10UL)             /*!< FLASHC_FM_CTL TEST_CTL: TM_DISPOS (Bit 10)                */
#define FLASHC_FM_CTL_TEST_CTL_TM_DISPOS_Msk (0x400UL)          /*!< FLASHC_FM_CTL TEST_CTL: TM_DISPOS (Bitfield-Mask: 0x01)   */
#define FLASHC_FM_CTL_TEST_CTL_TM_DISNEG_Pos (11UL)             /*!< FLASHC_FM_CTL TEST_CTL: TM_DISNEG (Bit 11)                */
#define FLASHC_FM_CTL_TEST_CTL_TM_DISNEG_Msk (0x800UL)          /*!< FLASHC_FM_CTL TEST_CTL: TM_DISNEG (Bitfield-Mask: 0x01)   */
#define FLASHC_FM_CTL_TEST_CTL_EN_CLK_MON_Pos (16UL)            /*!< FLASHC_FM_CTL TEST_CTL: EN_CLK_MON (Bit 16)               */
#define FLASHC_FM_CTL_TEST_CTL_EN_CLK_MON_Msk (0x10000UL)       /*!< FLASHC_FM_CTL TEST_CTL: EN_CLK_MON (Bitfield-Mask: 0x01)  */
#define FLASHC_FM_CTL_TEST_CTL_CSL_DEBUG_Pos (17UL)             /*!< FLASHC_FM_CTL TEST_CTL: CSL_DEBUG (Bit 17)                */
#define FLASHC_FM_CTL_TEST_CTL_CSL_DEBUG_Msk (0x20000UL)        /*!< FLASHC_FM_CTL TEST_CTL: CSL_DEBUG (Bitfield-Mask: 0x01)   */
#define FLASHC_FM_CTL_TEST_CTL_ENABLE_OSC_Pos (18UL)            /*!< FLASHC_FM_CTL TEST_CTL: ENABLE_OSC (Bit 18)               */
#define FLASHC_FM_CTL_TEST_CTL_ENABLE_OSC_Msk (0x40000UL)       /*!< FLASHC_FM_CTL TEST_CTL: ENABLE_OSC (Bitfield-Mask: 0x01)  */
#define FLASHC_FM_CTL_TEST_CTL_UNSCRAMBLE_WA_Pos (31UL)         /*!< FLASHC_FM_CTL TEST_CTL: UNSCRAMBLE_WA (Bit 31)            */
#define FLASHC_FM_CTL_TEST_CTL_UNSCRAMBLE_WA_Msk (0x80000000UL) /*!< FLASHC_FM_CTL TEST_CTL: UNSCRAMBLE_WA (Bitfield-Mask: 0x01)*/
/* ================================================  FLASHC_FM_CTL.WAIT_CTL  ================================================= */
#define FLASHC_FM_CTL_WAIT_CTL_WAIT_FM_MEM_RD_Pos (0UL)         /*!< FLASHC_FM_CTL WAIT_CTL: WAIT_FM_MEM_RD (Bit 0)            */
#define FLASHC_FM_CTL_WAIT_CTL_WAIT_FM_MEM_RD_Msk (0xfUL)       /*!< FLASHC_FM_CTL WAIT_CTL: WAIT_FM_MEM_RD (Bitfield-Mask: 0x0f)*/
#define FLASHC_FM_CTL_WAIT_CTL_WAIT_FM_HV_RD_Pos (8UL)          /*!< FLASHC_FM_CTL WAIT_CTL: WAIT_FM_HV_RD (Bit 8)             */
#define FLASHC_FM_CTL_WAIT_CTL_WAIT_FM_HV_RD_Msk (0xf00UL)      /*!< FLASHC_FM_CTL WAIT_CTL: WAIT_FM_HV_RD (Bitfield-Mask: 0x0f)*/
#define FLASHC_FM_CTL_WAIT_CTL_WAIT_FM_HV_WR_Pos (16UL)         /*!< FLASHC_FM_CTL WAIT_CTL: WAIT_FM_HV_WR (Bit 16)            */
#define FLASHC_FM_CTL_WAIT_CTL_WAIT_FM_HV_WR_Msk (0x70000UL)    /*!< FLASHC_FM_CTL WAIT_CTL: WAIT_FM_HV_WR (Bitfield-Mask: 0x07)*/
/* =============================================  FLASHC_FM_CTL.MONITOR_STATUS  ============================================== */
#define FLASHC_FM_CTL_MONITOR_STATUS_NEG_PUMP_VLO_Pos (1UL)     /*!< FLASHC_FM_CTL MONITOR_STATUS: NEG_PUMP_VLO (Bit 1)        */
#define FLASHC_FM_CTL_MONITOR_STATUS_NEG_PUMP_VLO_Msk (0x2UL)   /*!< FLASHC_FM_CTL MONITOR_STATUS: NEG_PUMP_VLO (Bitfield-Mask: 0x01)*/
#define FLASHC_FM_CTL_MONITOR_STATUS_POS_PUMP_VHI_Pos (2UL)     /*!< FLASHC_FM_CTL MONITOR_STATUS: POS_PUMP_VHI (Bit 2)        */
#define FLASHC_FM_CTL_MONITOR_STATUS_POS_PUMP_VHI_Msk (0x4UL)   /*!< FLASHC_FM_CTL MONITOR_STATUS: POS_PUMP_VHI (Bitfield-Mask: 0x01)*/
/* ===============================================  FLASHC_FM_CTL.SCRATCH_CTL  =============================================== */
#define FLASHC_FM_CTL_SCRATCH_CTL_DUMMY32_Pos (0UL)             /*!< FLASHC_FM_CTL SCRATCH_CTL: DUMMY32 (Bit 0)                */
#define FLASHC_FM_CTL_SCRATCH_CTL_DUMMY32_Msk (0xffffffffUL)    /*!< FLASHC_FM_CTL SCRATCH_CTL: DUMMY32 (Bitfield-Mask: 0xffffffff)*/
/* =================================================  FLASHC_FM_CTL.HV_CTL  ================================================== */
#define FLASHC_FM_CTL_HV_CTL_TIMER_CLOCK_FREQ_Pos (0UL)         /*!< FLASHC_FM_CTL HV_CTL: TIMER_CLOCK_FREQ (Bit 0)            */
#define FLASHC_FM_CTL_HV_CTL_TIMER_CLOCK_FREQ_Msk (0xffUL)      /*!< FLASHC_FM_CTL HV_CTL: TIMER_CLOCK_FREQ (Bitfield-Mask: 0xff)*/
/* ================================================  FLASHC_FM_CTL.ACLK_CTL  ================================================= */
#define FLASHC_FM_CTL_ACLK_CTL_ACLK_GEN_Pos (0UL)               /*!< FLASHC_FM_CTL ACLK_CTL: ACLK_GEN (Bit 0)                  */
#define FLASHC_FM_CTL_ACLK_CTL_ACLK_GEN_Msk (0x1UL)             /*!< FLASHC_FM_CTL ACLK_CTL: ACLK_GEN (Bitfield-Mask: 0x01)    */
/* ==================================================  FLASHC_FM_CTL.INTR  =================================================== */
#define FLASHC_FM_CTL_INTR_TIMER_EXPIRED_Pos (0UL)              /*!< FLASHC_FM_CTL INTR: TIMER_EXPIRED (Bit 0)                 */
#define FLASHC_FM_CTL_INTR_TIMER_EXPIRED_Msk (0x1UL)            /*!< FLASHC_FM_CTL INTR: TIMER_EXPIRED (Bitfield-Mask: 0x01)   */
/* ================================================  FLASHC_FM_CTL.INTR_SET  ================================================= */
#define FLASHC_FM_CTL_INTR_SET_TIMER_EXPIRED_Pos (0UL)          /*!< FLASHC_FM_CTL INTR_SET: TIMER_EXPIRED (Bit 0)             */
#define FLASHC_FM_CTL_INTR_SET_TIMER_EXPIRED_Msk (0x1UL)        /*!< FLASHC_FM_CTL INTR_SET: TIMER_EXPIRED (Bitfield-Mask: 0x01)*/
/* ================================================  FLASHC_FM_CTL.INTR_MASK  ================================================ */
#define FLASHC_FM_CTL_INTR_MASK_TIMER_EXPIRED_Pos (0UL)         /*!< FLASHC_FM_CTL INTR_MASK: TIMER_EXPIRED (Bit 0)            */
#define FLASHC_FM_CTL_INTR_MASK_TIMER_EXPIRED_Msk (0x1UL)       /*!< FLASHC_FM_CTL INTR_MASK: TIMER_EXPIRED (Bitfield-Mask: 0x01)*/
/* ===============================================  FLASHC_FM_CTL.INTR_MASKED  =============================================== */
#define FLASHC_FM_CTL_INTR_MASKED_TIMER_EXPIRED_Pos (0UL)       /*!< FLASHC_FM_CTL INTR_MASKED: TIMER_EXPIRED (Bit 0)          */
#define FLASHC_FM_CTL_INTR_MASKED_TIMER_EXPIRED_Msk (0x1UL)     /*!< FLASHC_FM_CTL INTR_MASKED: TIMER_EXPIRED (Bitfield-Mask: 0x01)*/
/* =============================================  FLASHC_FM_CTL.FM_HV_DATA_ALL  ============================================== */
#define FLASHC_FM_CTL_FM_HV_DATA_ALL_DATA32_Pos (0UL)           /*!< FLASHC_FM_CTL FM_HV_DATA_ALL: DATA32 (Bit 0)              */
#define FLASHC_FM_CTL_FM_HV_DATA_ALL_DATA32_Msk (0xffffffffUL)  /*!< FLASHC_FM_CTL FM_HV_DATA_ALL: DATA32 (Bitfield-Mask: 0xffffffff)*/
/* ================================================  FLASHC_FM_CTL.CAL_CTL0  ================================================= */
#define FLASHC_FM_CTL_CAL_CTL0_VCT_TRIM_LO_HV_Pos (0UL)         /*!< FLASHC_FM_CTL CAL_CTL0: VCT_TRIM_LO_HV (Bit 0)            */
#define FLASHC_FM_CTL_CAL_CTL0_VCT_TRIM_LO_HV_Msk (0x1fUL)      /*!< FLASHC_FM_CTL CAL_CTL0: VCT_TRIM_LO_HV (Bitfield-Mask: 0x1f)*/
#define FLASHC_FM_CTL_CAL_CTL0_CDAC_LO_HV_Pos (5UL)             /*!< FLASHC_FM_CTL CAL_CTL0: CDAC_LO_HV (Bit 5)                */
#define FLASHC_FM_CTL_CAL_CTL0_CDAC_LO_HV_Msk (0xe0UL)          /*!< FLASHC_FM_CTL CAL_CTL0: CDAC_LO_HV (Bitfield-Mask: 0x07)  */
#define FLASHC_FM_CTL_CAL_CTL0_VBG_TRIM_LO_HV_Pos (8UL)         /*!< FLASHC_FM_CTL CAL_CTL0: VBG_TRIM_LO_HV (Bit 8)            */
#define FLASHC_FM_CTL_CAL_CTL0_VBG_TRIM_LO_HV_Msk (0x1f00UL)    /*!< FLASHC_FM_CTL CAL_CTL0: VBG_TRIM_LO_HV (Bitfield-Mask: 0x1f)*/
#define FLASHC_FM_CTL_CAL_CTL0_VBG_TC_TRIM_LO_HV_Pos (13UL)     /*!< FLASHC_FM_CTL CAL_CTL0: VBG_TC_TRIM_LO_HV (Bit 13)        */
#define FLASHC_FM_CTL_CAL_CTL0_VBG_TC_TRIM_LO_HV_Msk (0xe000UL) /*!< FLASHC_FM_CTL CAL_CTL0: VBG_TC_TRIM_LO_HV (Bitfield-Mask: 0x07)*/
#define FLASHC_FM_CTL_CAL_CTL0_IPREF_TRIM_LO_HV_Pos (16UL)      /*!< FLASHC_FM_CTL CAL_CTL0: IPREF_TRIM_LO_HV (Bit 16)         */
#define FLASHC_FM_CTL_CAL_CTL0_IPREF_TRIM_LO_HV_Msk (0xf0000UL) /*!< FLASHC_FM_CTL CAL_CTL0: IPREF_TRIM_LO_HV (Bitfield-Mask: 0x0f)*/
/* ================================================  FLASHC_FM_CTL.CAL_CTL1  ================================================= */
#define FLASHC_FM_CTL_CAL_CTL1_VCT_TRIM_HI_HV_Pos (0UL)         /*!< FLASHC_FM_CTL CAL_CTL1: VCT_TRIM_HI_HV (Bit 0)            */
#define FLASHC_FM_CTL_CAL_CTL1_VCT_TRIM_HI_HV_Msk (0x1fUL)      /*!< FLASHC_FM_CTL CAL_CTL1: VCT_TRIM_HI_HV (Bitfield-Mask: 0x1f)*/
#define FLASHC_FM_CTL_CAL_CTL1_CDAC_HI_HV_Pos (5UL)             /*!< FLASHC_FM_CTL CAL_CTL1: CDAC_HI_HV (Bit 5)                */
#define FLASHC_FM_CTL_CAL_CTL1_CDAC_HI_HV_Msk (0xe0UL)          /*!< FLASHC_FM_CTL CAL_CTL1: CDAC_HI_HV (Bitfield-Mask: 0x07)  */
#define FLASHC_FM_CTL_CAL_CTL1_VBG_TRIM_HI_HV_Pos (8UL)         /*!< FLASHC_FM_CTL CAL_CTL1: VBG_TRIM_HI_HV (Bit 8)            */
#define FLASHC_FM_CTL_CAL_CTL1_VBG_TRIM_HI_HV_Msk (0x1f00UL)    /*!< FLASHC_FM_CTL CAL_CTL1: VBG_TRIM_HI_HV (Bitfield-Mask: 0x1f)*/
#define FLASHC_FM_CTL_CAL_CTL1_VBG_TC_TRIM_HI_HV_Pos (13UL)     /*!< FLASHC_FM_CTL CAL_CTL1: VBG_TC_TRIM_HI_HV (Bit 13)        */
#define FLASHC_FM_CTL_CAL_CTL1_VBG_TC_TRIM_HI_HV_Msk (0xe000UL) /*!< FLASHC_FM_CTL CAL_CTL1: VBG_TC_TRIM_HI_HV (Bitfield-Mask: 0x07)*/
#define FLASHC_FM_CTL_CAL_CTL1_IPREF_TRIM_HI_HV_Pos (16UL)      /*!< FLASHC_FM_CTL CAL_CTL1: IPREF_TRIM_HI_HV (Bit 16)         */
#define FLASHC_FM_CTL_CAL_CTL1_IPREF_TRIM_HI_HV_Msk (0xf0000UL) /*!< FLASHC_FM_CTL CAL_CTL1: IPREF_TRIM_HI_HV (Bitfield-Mask: 0x0f)*/
/* ================================================  FLASHC_FM_CTL.CAL_CTL2  ================================================= */
#define FLASHC_FM_CTL_CAL_CTL2_ICREF_TRIM_LO_HV_Pos (0UL)       /*!< FLASHC_FM_CTL CAL_CTL2: ICREF_TRIM_LO_HV (Bit 0)          */
#define FLASHC_FM_CTL_CAL_CTL2_ICREF_TRIM_LO_HV_Msk (0x1fUL)    /*!< FLASHC_FM_CTL CAL_CTL2: ICREF_TRIM_LO_HV (Bitfield-Mask: 0x1f)*/
#define FLASHC_FM_CTL_CAL_CTL2_ICREF_TC_TRIM_LO_HV_Pos (5UL)    /*!< FLASHC_FM_CTL CAL_CTL2: ICREF_TC_TRIM_LO_HV (Bit 5)       */
#define FLASHC_FM_CTL_CAL_CTL2_ICREF_TC_TRIM_LO_HV_Msk (0xe0UL) /*!< FLASHC_FM_CTL CAL_CTL2: ICREF_TC_TRIM_LO_HV (Bitfield-Mask: 0x07)*/
#define FLASHC_FM_CTL_CAL_CTL2_ICREF_TRIM_HI_HV_Pos (8UL)       /*!< FLASHC_FM_CTL CAL_CTL2: ICREF_TRIM_HI_HV (Bit 8)          */
#define FLASHC_FM_CTL_CAL_CTL2_ICREF_TRIM_HI_HV_Msk (0x1f00UL)  /*!< FLASHC_FM_CTL CAL_CTL2: ICREF_TRIM_HI_HV (Bitfield-Mask: 0x1f)*/
#define FLASHC_FM_CTL_CAL_CTL2_ICREF_TC_TRIM_HI_HV_Pos (13UL)   /*!< FLASHC_FM_CTL CAL_CTL2: ICREF_TC_TRIM_HI_HV (Bit 13)      */
#define FLASHC_FM_CTL_CAL_CTL2_ICREF_TC_TRIM_HI_HV_Msk (0xe000UL) /*!< FLASHC_FM_CTL CAL_CTL2: ICREF_TC_TRIM_HI_HV (Bitfield-Mask: 0x07)*/
#define FLASHC_FM_CTL_CAL_CTL2_VREF_SEL_HV_Pos (16UL)           /*!< FLASHC_FM_CTL CAL_CTL2: VREF_SEL_HV (Bit 16)              */
#define FLASHC_FM_CTL_CAL_CTL2_VREF_SEL_HV_Msk (0x10000UL)      /*!< FLASHC_FM_CTL CAL_CTL2: VREF_SEL_HV (Bitfield-Mask: 0x01) */
#define FLASHC_FM_CTL_CAL_CTL2_IREF_SEL_HV_Pos (17UL)           /*!< FLASHC_FM_CTL CAL_CTL2: IREF_SEL_HV (Bit 17)              */
#define FLASHC_FM_CTL_CAL_CTL2_IREF_SEL_HV_Msk (0x20000UL)      /*!< FLASHC_FM_CTL CAL_CTL2: IREF_SEL_HV (Bitfield-Mask: 0x01) */
#define FLASHC_FM_CTL_CAL_CTL2_FM_ACTIVE_HV_Pos (18UL)          /*!< FLASHC_FM_CTL CAL_CTL2: FM_ACTIVE_HV (Bit 18)             */
#define FLASHC_FM_CTL_CAL_CTL2_FM_ACTIVE_HV_Msk (0x40000UL)     /*!< FLASHC_FM_CTL CAL_CTL2: FM_ACTIVE_HV (Bitfield-Mask: 0x01)*/
#define FLASHC_FM_CTL_CAL_CTL2_TURBO_EXT_HV_Pos (19UL)          /*!< FLASHC_FM_CTL CAL_CTL2: TURBO_EXT_HV (Bit 19)             */
#define FLASHC_FM_CTL_CAL_CTL2_TURBO_EXT_HV_Msk (0x80000UL)     /*!< FLASHC_FM_CTL CAL_CTL2: TURBO_EXT_HV (Bitfield-Mask: 0x01)*/
/* ================================================  FLASHC_FM_CTL.CAL_CTL3  ================================================= */
#define FLASHC_FM_CTL_CAL_CTL3_OSC_TRIM_HV_Pos (0UL)            /*!< FLASHC_FM_CTL CAL_CTL3: OSC_TRIM_HV (Bit 0)               */
#define FLASHC_FM_CTL_CAL_CTL3_OSC_TRIM_HV_Msk (0xfUL)          /*!< FLASHC_FM_CTL CAL_CTL3: OSC_TRIM_HV (Bitfield-Mask: 0x0f) */
#define FLASHC_FM_CTL_CAL_CTL3_OSC_RANGE_TRIM_HV_Pos (4UL)      /*!< FLASHC_FM_CTL CAL_CTL3: OSC_RANGE_TRIM_HV (Bit 4)         */
#define FLASHC_FM_CTL_CAL_CTL3_OSC_RANGE_TRIM_HV_Msk (0x10UL)   /*!< FLASHC_FM_CTL CAL_CTL3: OSC_RANGE_TRIM_HV (Bitfield-Mask: 0x01)*/
#define FLASHC_FM_CTL_CAL_CTL3_IDAC_HV_Pos (5UL)                /*!< FLASHC_FM_CTL CAL_CTL3: IDAC_HV (Bit 5)                   */
#define FLASHC_FM_CTL_CAL_CTL3_IDAC_HV_Msk (0x1e0UL)            /*!< FLASHC_FM_CTL CAL_CTL3: IDAC_HV (Bitfield-Mask: 0x0f)     */
#define FLASHC_FM_CTL_CAL_CTL3_SDAC_HV_Pos (9UL)                /*!< FLASHC_FM_CTL CAL_CTL3: SDAC_HV (Bit 9)                   */
#define FLASHC_FM_CTL_CAL_CTL3_SDAC_HV_Msk (0x600UL)            /*!< FLASHC_FM_CTL CAL_CTL3: SDAC_HV (Bitfield-Mask: 0x03)     */
#define FLASHC_FM_CTL_CAL_CTL3_ITIM_HV_Pos (11UL)               /*!< FLASHC_FM_CTL CAL_CTL3: ITIM_HV (Bit 11)                  */
#define FLASHC_FM_CTL_CAL_CTL3_ITIM_HV_Msk (0x7800UL)           /*!< FLASHC_FM_CTL CAL_CTL3: ITIM_HV (Bitfield-Mask: 0x0f)     */
#define FLASHC_FM_CTL_CAL_CTL3_VDDHI_HV_Pos (15UL)              /*!< FLASHC_FM_CTL CAL_CTL3: VDDHI_HV (Bit 15)                 */
#define FLASHC_FM_CTL_CAL_CTL3_VDDHI_HV_Msk (0x8000UL)          /*!< FLASHC_FM_CTL CAL_CTL3: VDDHI_HV (Bitfield-Mask: 0x01)    */
#define FLASHC_FM_CTL_CAL_CTL3_TURBO_PULSEW_HV_Pos (16UL)       /*!< FLASHC_FM_CTL CAL_CTL3: TURBO_PULSEW_HV (Bit 16)          */
#define FLASHC_FM_CTL_CAL_CTL3_TURBO_PULSEW_HV_Msk (0x30000UL)  /*!< FLASHC_FM_CTL CAL_CTL3: TURBO_PULSEW_HV (Bitfield-Mask: 0x03)*/
#define FLASHC_FM_CTL_CAL_CTL3_BGLO_EN_HV_Pos (18UL)            /*!< FLASHC_FM_CTL CAL_CTL3: BGLO_EN_HV (Bit 18)               */
#define FLASHC_FM_CTL_CAL_CTL3_BGLO_EN_HV_Msk (0x40000UL)       /*!< FLASHC_FM_CTL CAL_CTL3: BGLO_EN_HV (Bitfield-Mask: 0x01)  */
#define FLASHC_FM_CTL_CAL_CTL3_BGHI_EN_HV_Pos (19UL)            /*!< FLASHC_FM_CTL CAL_CTL3: BGHI_EN_HV (Bit 19)               */
#define FLASHC_FM_CTL_CAL_CTL3_BGHI_EN_HV_Msk (0x80000UL)       /*!< FLASHC_FM_CTL CAL_CTL3: BGHI_EN_HV (Bitfield-Mask: 0x01)  */
/* ================================================  FLASHC_FM_CTL.BOOKMARK  ================================================= */
#define FLASHC_FM_CTL_BOOKMARK_BOOKMARK_Pos (0UL)               /*!< FLASHC_FM_CTL BOOKMARK: BOOKMARK (Bit 0)                  */
#define FLASHC_FM_CTL_BOOKMARK_BOOKMARK_Msk (0xffffffffUL)      /*!< FLASHC_FM_CTL BOOKMARK: BOOKMARK (Bitfield-Mask: 0xffffffff)*/
/* ================================================  FLASHC_FM_CTL.RED_CTL01  ================================================ */
#define FLASHC_FM_CTL_RED_CTL01_RED_ADDR_0_Pos (0UL)            /*!< FLASHC_FM_CTL RED_CTL01: RED_ADDR_0 (Bit 0)               */
#define FLASHC_FM_CTL_RED_CTL01_RED_ADDR_0_Msk (0xffUL)         /*!< FLASHC_FM_CTL RED_CTL01: RED_ADDR_0 (Bitfield-Mask: 0xff) */
#define FLASHC_FM_CTL_RED_CTL01_RED_EN_0_Pos (8UL)              /*!< FLASHC_FM_CTL RED_CTL01: RED_EN_0 (Bit 8)                 */
#define FLASHC_FM_CTL_RED_CTL01_RED_EN_0_Msk (0x100UL)          /*!< FLASHC_FM_CTL RED_CTL01: RED_EN_0 (Bitfield-Mask: 0x01)   */
#define FLASHC_FM_CTL_RED_CTL01_RED_ADDR_1_Pos (16UL)           /*!< FLASHC_FM_CTL RED_CTL01: RED_ADDR_1 (Bit 16)              */
#define FLASHC_FM_CTL_RED_CTL01_RED_ADDR_1_Msk (0xff0000UL)     /*!< FLASHC_FM_CTL RED_CTL01: RED_ADDR_1 (Bitfield-Mask: 0xff) */
#define FLASHC_FM_CTL_RED_CTL01_RED_EN_1_Pos (24UL)             /*!< FLASHC_FM_CTL RED_CTL01: RED_EN_1 (Bit 24)                */
#define FLASHC_FM_CTL_RED_CTL01_RED_EN_1_Msk (0x1000000UL)      /*!< FLASHC_FM_CTL RED_CTL01: RED_EN_1 (Bitfield-Mask: 0x01)   */
/* ================================================  FLASHC_FM_CTL.RED_CTL23  ================================================ */
#define FLASHC_FM_CTL_RED_CTL23_RED_ADDR_2_Pos (0UL)            /*!< FLASHC_FM_CTL RED_CTL23: RED_ADDR_2 (Bit 0)               */
#define FLASHC_FM_CTL_RED_CTL23_RED_ADDR_2_Msk (0xffUL)         /*!< FLASHC_FM_CTL RED_CTL23: RED_ADDR_2 (Bitfield-Mask: 0xff) */
#define FLASHC_FM_CTL_RED_CTL23_RED_EN_2_Pos (8UL)              /*!< FLASHC_FM_CTL RED_CTL23: RED_EN_2 (Bit 8)                 */
#define FLASHC_FM_CTL_RED_CTL23_RED_EN_2_Msk (0x100UL)          /*!< FLASHC_FM_CTL RED_CTL23: RED_EN_2 (Bitfield-Mask: 0x01)   */
#define FLASHC_FM_CTL_RED_CTL23_RED_ADDR_3_Pos (16UL)           /*!< FLASHC_FM_CTL RED_CTL23: RED_ADDR_3 (Bit 16)              */
#define FLASHC_FM_CTL_RED_CTL23_RED_ADDR_3_Msk (0xff0000UL)     /*!< FLASHC_FM_CTL RED_CTL23: RED_ADDR_3 (Bitfield-Mask: 0xff) */
#define FLASHC_FM_CTL_RED_CTL23_RED_EN_3_Pos (24UL)             /*!< FLASHC_FM_CTL RED_CTL23: RED_EN_3 (Bit 24)                */
#define FLASHC_FM_CTL_RED_CTL23_RED_EN_3_Msk (0x1000000UL)      /*!< FLASHC_FM_CTL RED_CTL23: RED_EN_3 (Bitfield-Mask: 0x01)   */
/* ================================================  FLASHC_FM_CTL.RED_CTL45  ================================================ */
#define FLASHC_FM_CTL_RED_CTL45_RED_ADDR_4_Pos (0UL)            /*!< FLASHC_FM_CTL RED_CTL45: RED_ADDR_4 (Bit 0)               */
#define FLASHC_FM_CTL_RED_CTL45_RED_ADDR_4_Msk (0xffUL)         /*!< FLASHC_FM_CTL RED_CTL45: RED_ADDR_4 (Bitfield-Mask: 0xff) */
#define FLASHC_FM_CTL_RED_CTL45_RED_EN_4_Pos (8UL)              /*!< FLASHC_FM_CTL RED_CTL45: RED_EN_4 (Bit 8)                 */
#define FLASHC_FM_CTL_RED_CTL45_RED_EN_4_Msk (0x100UL)          /*!< FLASHC_FM_CTL RED_CTL45: RED_EN_4 (Bitfield-Mask: 0x01)   */
#define FLASHC_FM_CTL_RED_CTL45_RED_ADDR_5_Pos (16UL)           /*!< FLASHC_FM_CTL RED_CTL45: RED_ADDR_5 (Bit 16)              */
#define FLASHC_FM_CTL_RED_CTL45_RED_ADDR_5_Msk (0xff0000UL)     /*!< FLASHC_FM_CTL RED_CTL45: RED_ADDR_5 (Bitfield-Mask: 0xff) */
#define FLASHC_FM_CTL_RED_CTL45_RED_EN_5_Pos (24UL)             /*!< FLASHC_FM_CTL RED_CTL45: RED_EN_5 (Bit 24)                */
#define FLASHC_FM_CTL_RED_CTL45_RED_EN_5_Msk (0x1000000UL)      /*!< FLASHC_FM_CTL RED_CTL45: RED_EN_5 (Bitfield-Mask: 0x01)   */
/* ================================================  FLASHC_FM_CTL.RED_CTL67  ================================================ */
#define FLASHC_FM_CTL_RED_CTL67_RED_ADDR_6_Pos (0UL)            /*!< FLASHC_FM_CTL RED_CTL67: RED_ADDR_6 (Bit 0)               */
#define FLASHC_FM_CTL_RED_CTL67_RED_ADDR_6_Msk (0xffUL)         /*!< FLASHC_FM_CTL RED_CTL67: RED_ADDR_6 (Bitfield-Mask: 0xff) */
#define FLASHC_FM_CTL_RED_CTL67_RED_EN_6_Pos (8UL)              /*!< FLASHC_FM_CTL RED_CTL67: RED_EN_6 (Bit 8)                 */
#define FLASHC_FM_CTL_RED_CTL67_RED_EN_6_Msk (0x100UL)          /*!< FLASHC_FM_CTL RED_CTL67: RED_EN_6 (Bitfield-Mask: 0x01)   */
#define FLASHC_FM_CTL_RED_CTL67_RED_ADDR_7_Pos (16UL)           /*!< FLASHC_FM_CTL RED_CTL67: RED_ADDR_7 (Bit 16)              */
#define FLASHC_FM_CTL_RED_CTL67_RED_ADDR_7_Msk (0xff0000UL)     /*!< FLASHC_FM_CTL RED_CTL67: RED_ADDR_7 (Bitfield-Mask: 0xff) */
#define FLASHC_FM_CTL_RED_CTL67_RED_EN_7_Pos (24UL)             /*!< FLASHC_FM_CTL RED_CTL67: RED_EN_7 (Bit 24)                */
#define FLASHC_FM_CTL_RED_CTL67_RED_EN_7_Msk (0x1000000UL)      /*!< FLASHC_FM_CTL RED_CTL67: RED_EN_7 (Bitfield-Mask: 0x01)   */
/* ==============================================  FLASHC_FM_CTL.RED_CTL_SM01  =============================================== */
#define FLASHC_FM_CTL_RED_CTL_SM01_RED_ADDR_SM0_Pos (0UL)       /*!< FLASHC_FM_CTL RED_CTL_SM01: RED_ADDR_SM0 (Bit 0)          */
#define FLASHC_FM_CTL_RED_CTL_SM01_RED_ADDR_SM0_Msk (0xffUL)    /*!< FLASHC_FM_CTL RED_CTL_SM01: RED_ADDR_SM0 (Bitfield-Mask: 0xff)*/
#define FLASHC_FM_CTL_RED_CTL_SM01_RED_EN_SM0_Pos (8UL)         /*!< FLASHC_FM_CTL RED_CTL_SM01: RED_EN_SM0 (Bit 8)            */
#define FLASHC_FM_CTL_RED_CTL_SM01_RED_EN_SM0_Msk (0x100UL)     /*!< FLASHC_FM_CTL RED_CTL_SM01: RED_EN_SM0 (Bitfield-Mask: 0x01)*/
#define FLASHC_FM_CTL_RED_CTL_SM01_RED_ADDR_SM1_Pos (16UL)      /*!< FLASHC_FM_CTL RED_CTL_SM01: RED_ADDR_SM1 (Bit 16)         */
#define FLASHC_FM_CTL_RED_CTL_SM01_RED_ADDR_SM1_Msk (0xff0000UL) /*!< FLASHC_FM_CTL RED_CTL_SM01: RED_ADDR_SM1 (Bitfield-Mask: 0xff)*/
#define FLASHC_FM_CTL_RED_CTL_SM01_RED_EN_SM1_Pos (24UL)        /*!< FLASHC_FM_CTL RED_CTL_SM01: RED_EN_SM1 (Bit 24)           */
#define FLASHC_FM_CTL_RED_CTL_SM01_RED_EN_SM1_Msk (0x1000000UL) /*!< FLASHC_FM_CTL RED_CTL_SM01: RED_EN_SM1 (Bitfield-Mask: 0x01)*/
#define FLASHC_FM_CTL_RED_CTL_SM01_TRKD_Pos (30UL)              /*!< FLASHC_FM_CTL RED_CTL_SM01: TRKD (Bit 30)                 */
#define FLASHC_FM_CTL_RED_CTL_SM01_TRKD_Msk (0x40000000UL)      /*!< FLASHC_FM_CTL RED_CTL_SM01: TRKD (Bitfield-Mask: 0x01)    */
#define FLASHC_FM_CTL_RED_CTL_SM01_R_GRANT_EN_Pos (31UL)        /*!< FLASHC_FM_CTL RED_CTL_SM01: R_GRANT_EN (Bit 31)           */
#define FLASHC_FM_CTL_RED_CTL_SM01_R_GRANT_EN_Msk (0x80000000UL) /*!< FLASHC_FM_CTL RED_CTL_SM01: R_GRANT_EN (Bitfield-Mask: 0x01)*/
/* =================================================  FLASHC_FM_CTL.TM_CMPR  ================================================= */
#define FLASHC_FM_CTL_TM_CMPR_DATA_COMP_RESULT_Pos (0UL)        /*!< FLASHC_FM_CTL TM_CMPR: DATA_COMP_RESULT (Bit 0)           */
#define FLASHC_FM_CTL_TM_CMPR_DATA_COMP_RESULT_Msk (0x1UL)      /*!< FLASHC_FM_CTL TM_CMPR: DATA_COMP_RESULT (Bitfield-Mask: 0x01)*/
/* ===============================================  FLASHC_FM_CTL.FM_HV_DATA  ================================================ */
#define FLASHC_FM_CTL_FM_HV_DATA_DATA32_Pos (0UL)               /*!< FLASHC_FM_CTL FM_HV_DATA: DATA32 (Bit 0)                  */
#define FLASHC_FM_CTL_FM_HV_DATA_DATA32_Msk (0xffffffffUL)      /*!< FLASHC_FM_CTL FM_HV_DATA: DATA32 (Bitfield-Mask: 0xffffffff)*/
/* ===============================================  FLASHC_FM_CTL.FM_MEM_DATA  =============================================== */
#define FLASHC_FM_CTL_FM_MEM_DATA_DATA32_Pos (0UL)              /*!< FLASHC_FM_CTL FM_MEM_DATA: DATA32 (Bit 0)                 */
#define FLASHC_FM_CTL_FM_MEM_DATA_DATA32_Msk (0xffffffffUL)     /*!< FLASHC_FM_CTL FM_MEM_DATA: DATA32 (Bitfield-Mask: 0xffffffff)*/


/* ===================================================  FLASHC.FLASH_CTL  ==================================================== */
#define FLASHC_FLASH_CTL_MAIN_WS_Pos      (0UL)                 /*!< FLASHC FLASH_CTL: MAIN_WS (Bit 0)                         */
#define FLASHC_FLASH_CTL_MAIN_WS_Msk      (0xfUL)               /*!< FLASHC FLASH_CTL: MAIN_WS (Bitfield-Mask: 0x0f)           */
#define FLASHC_FLASH_CTL_REMAP_Pos        (8UL)                 /*!< FLASHC FLASH_CTL: REMAP (Bit 8)                           */
#define FLASHC_FLASH_CTL_REMAP_Msk        (0x100UL)             /*!< FLASHC FLASH_CTL: REMAP (Bitfield-Mask: 0x01)             */
/* =================================================  FLASHC.FLASH_PWR_CTL  ================================================== */
#define FLASHC_FLASH_PWR_CTL_ENABLE_Pos   (0UL)                 /*!< FLASHC FLASH_PWR_CTL: ENABLE (Bit 0)                      */
#define FLASHC_FLASH_PWR_CTL_ENABLE_Msk   (0x1UL)               /*!< FLASHC FLASH_PWR_CTL: ENABLE (Bitfield-Mask: 0x01)        */
#define FLASHC_FLASH_PWR_CTL_ENABLE_HV_Pos (1UL)                /*!< FLASHC FLASH_PWR_CTL: ENABLE_HV (Bit 1)                   */
#define FLASHC_FLASH_PWR_CTL_ENABLE_HV_Msk (0x2UL)              /*!< FLASHC FLASH_PWR_CTL: ENABLE_HV (Bitfield-Mask: 0x01)     */
/* ===================================================  FLASHC.FLASH_CMD  ==================================================== */
#define FLASHC_FLASH_CMD_INV_Pos          (0UL)                 /*!< FLASHC FLASH_CMD: INV (Bit 0)                             */
#define FLASHC_FLASH_CMD_INV_Msk          (0x1UL)               /*!< FLASHC FLASH_CMD: INV (Bitfield-Mask: 0x01)               */
/* ==================================================  FLASHC.CM0_CA_CTL0  =================================================== */
#define FLASHC_CM0_CA_CTL0_WAY_Pos        (16UL)                /*!< FLASHC CM0_CA_CTL0: WAY (Bit 16)                          */
#define FLASHC_CM0_CA_CTL0_WAY_Msk        (0x30000UL)           /*!< FLASHC CM0_CA_CTL0: WAY (Bitfield-Mask: 0x03)             */
#define FLASHC_CM0_CA_CTL0_SET_ADDR_Pos   (24UL)                /*!< FLASHC CM0_CA_CTL0: SET_ADDR (Bit 24)                     */
#define FLASHC_CM0_CA_CTL0_SET_ADDR_Msk   (0x7000000UL)         /*!< FLASHC CM0_CA_CTL0: SET_ADDR (Bitfield-Mask: 0x07)        */
#define FLASHC_CM0_CA_CTL0_PREF_EN_Pos    (30UL)                /*!< FLASHC CM0_CA_CTL0: PREF_EN (Bit 30)                      */
#define FLASHC_CM0_CA_CTL0_PREF_EN_Msk    (0x40000000UL)        /*!< FLASHC CM0_CA_CTL0: PREF_EN (Bitfield-Mask: 0x01)         */
#define FLASHC_CM0_CA_CTL0_ENABLED_Pos    (31UL)                /*!< FLASHC CM0_CA_CTL0: ENABLED (Bit 31)                      */
#define FLASHC_CM0_CA_CTL0_ENABLED_Msk    (0x80000000UL)        /*!< FLASHC CM0_CA_CTL0: ENABLED (Bitfield-Mask: 0x01)         */
/* ==================================================  FLASHC.CM0_CA_CTL1  =================================================== */
#define FLASHC_CM0_CA_CTL1_ISOLATE_Pos    (0UL)                 /*!< FLASHC CM0_CA_CTL1: ISOLATE (Bit 0)                       */
#define FLASHC_CM0_CA_CTL1_ISOLATE_Msk    (0x1UL)               /*!< FLASHC CM0_CA_CTL1: ISOLATE (Bitfield-Mask: 0x01)         */
#define FLASHC_CM0_CA_CTL1_RETAIN_Pos     (1UL)                 /*!< FLASHC CM0_CA_CTL1: RETAIN (Bit 1)                        */
#define FLASHC_CM0_CA_CTL1_RETAIN_Msk     (0x2UL)               /*!< FLASHC CM0_CA_CTL1: RETAIN (Bitfield-Mask: 0x01)          */
#define FLASHC_CM0_CA_CTL1_POWER_Pos      (2UL)                 /*!< FLASHC CM0_CA_CTL1: POWER (Bit 2)                         */
#define FLASHC_CM0_CA_CTL1_POWER_Msk      (0x4UL)               /*!< FLASHC CM0_CA_CTL1: POWER (Bitfield-Mask: 0x01)           */
/* ===================================================  FLASHC.CM0_CA_CMD  =================================================== */
#define FLASHC_CM0_CA_CMD_INV_Pos         (0UL)                 /*!< FLASHC CM0_CA_CMD: INV (Bit 0)                            */
#define FLASHC_CM0_CA_CMD_INV_Msk         (0x1UL)               /*!< FLASHC CM0_CA_CMD: INV (Bitfield-Mask: 0x01)              */
/* =================================================  FLASHC.CM0_CA_STATUS0  ================================================= */
#define FLASHC_CM0_CA_STATUS0_VALID16_Pos (0UL)                 /*!< FLASHC CM0_CA_STATUS0: VALID16 (Bit 0)                    */
#define FLASHC_CM0_CA_STATUS0_VALID16_Msk (0xffffUL)            /*!< FLASHC CM0_CA_STATUS0: VALID16 (Bitfield-Mask: 0xffff)    */
/* =================================================  FLASHC.CM0_CA_STATUS1  ================================================= */
#define FLASHC_CM0_CA_STATUS1_TAG_Pos     (0UL)                 /*!< FLASHC CM0_CA_STATUS1: TAG (Bit 0)                        */
#define FLASHC_CM0_CA_STATUS1_TAG_Msk     (0xffffffffUL)        /*!< FLASHC CM0_CA_STATUS1: TAG (Bitfield-Mask: 0xffffffff)    */
/* =================================================  FLASHC.CM0_CA_STATUS2  ================================================= */
#define FLASHC_CM0_CA_STATUS2_LRU_Pos     (0UL)                 /*!< FLASHC CM0_CA_STATUS2: LRU (Bit 0)                        */
#define FLASHC_CM0_CA_STATUS2_LRU_Msk     (0x3fUL)              /*!< FLASHC CM0_CA_STATUS2: LRU (Bitfield-Mask: 0x3f)          */
/* ==================================================  FLASHC.CM4_CA_CTL0  =================================================== */
#define FLASHC_CM4_CA_CTL0_WAY_Pos        (16UL)                /*!< FLASHC CM4_CA_CTL0: WAY (Bit 16)                          */
#define FLASHC_CM4_CA_CTL0_WAY_Msk        (0x30000UL)           /*!< FLASHC CM4_CA_CTL0: WAY (Bitfield-Mask: 0x03)             */
#define FLASHC_CM4_CA_CTL0_SET_ADDR_Pos   (24UL)                /*!< FLASHC CM4_CA_CTL0: SET_ADDR (Bit 24)                     */
#define FLASHC_CM4_CA_CTL0_SET_ADDR_Msk   (0x7000000UL)         /*!< FLASHC CM4_CA_CTL0: SET_ADDR (Bitfield-Mask: 0x07)        */
#define FLASHC_CM4_CA_CTL0_PREF_EN_Pos    (30UL)                /*!< FLASHC CM4_CA_CTL0: PREF_EN (Bit 30)                      */
#define FLASHC_CM4_CA_CTL0_PREF_EN_Msk    (0x40000000UL)        /*!< FLASHC CM4_CA_CTL0: PREF_EN (Bitfield-Mask: 0x01)         */
#define FLASHC_CM4_CA_CTL0_ENABLED_Pos    (31UL)                /*!< FLASHC CM4_CA_CTL0: ENABLED (Bit 31)                      */
#define FLASHC_CM4_CA_CTL0_ENABLED_Msk    (0x80000000UL)        /*!< FLASHC CM4_CA_CTL0: ENABLED (Bitfield-Mask: 0x01)         */
/* ==================================================  FLASHC.CM4_CA_CTL1  =================================================== */
#define FLASHC_CM4_CA_CTL1_ISOLATE_Pos    (0UL)                 /*!< FLASHC CM4_CA_CTL1: ISOLATE (Bit 0)                       */
#define FLASHC_CM4_CA_CTL1_ISOLATE_Msk    (0x1UL)               /*!< FLASHC CM4_CA_CTL1: ISOLATE (Bitfield-Mask: 0x01)         */
#define FLASHC_CM4_CA_CTL1_RETAIN_Pos     (1UL)                 /*!< FLASHC CM4_CA_CTL1: RETAIN (Bit 1)                        */
#define FLASHC_CM4_CA_CTL1_RETAIN_Msk     (0x2UL)               /*!< FLASHC CM4_CA_CTL1: RETAIN (Bitfield-Mask: 0x01)          */
#define FLASHC_CM4_CA_CTL1_POWER_Pos      (2UL)                 /*!< FLASHC CM4_CA_CTL1: POWER (Bit 2)                         */
#define FLASHC_CM4_CA_CTL1_POWER_Msk      (0x4UL)               /*!< FLASHC CM4_CA_CTL1: POWER (Bitfield-Mask: 0x01)           */
/* ===================================================  FLASHC.CM4_CA_CMD  =================================================== */
#define FLASHC_CM4_CA_CMD_INV_Pos         (0UL)                 /*!< FLASHC CM4_CA_CMD: INV (Bit 0)                            */
#define FLASHC_CM4_CA_CMD_INV_Msk         (0x1UL)               /*!< FLASHC CM4_CA_CMD: INV (Bitfield-Mask: 0x01)              */
/* =================================================  FLASHC.CM4_CA_STATUS0  ================================================= */
#define FLASHC_CM4_CA_STATUS0_VALID16_Pos (0UL)                 /*!< FLASHC CM4_CA_STATUS0: VALID16 (Bit 0)                    */
#define FLASHC_CM4_CA_STATUS0_VALID16_Msk (0xffffUL)            /*!< FLASHC CM4_CA_STATUS0: VALID16 (Bitfield-Mask: 0xffff)    */
/* =================================================  FLASHC.CM4_CA_STATUS1  ================================================= */
#define FLASHC_CM4_CA_STATUS1_TAG_Pos     (0UL)                 /*!< FLASHC CM4_CA_STATUS1: TAG (Bit 0)                        */
#define FLASHC_CM4_CA_STATUS1_TAG_Msk     (0xffffffffUL)        /*!< FLASHC CM4_CA_STATUS1: TAG (Bitfield-Mask: 0xffffffff)    */
/* =================================================  FLASHC.CM4_CA_STATUS2  ================================================= */
#define FLASHC_CM4_CA_STATUS2_LRU_Pos     (0UL)                 /*!< FLASHC CM4_CA_STATUS2: LRU (Bit 0)                        */
#define FLASHC_CM4_CA_STATUS2_LRU_Msk     (0x3fUL)              /*!< FLASHC CM4_CA_STATUS2: LRU (Bitfield-Mask: 0x3f)          */
/* ================================================  FLASHC.CRYPTO_BUFF_CTL  ================================================= */
#define FLASHC_CRYPTO_BUFF_CTL_PREF_EN_Pos (30UL)               /*!< FLASHC CRYPTO_BUFF_CTL: PREF_EN (Bit 30)                  */
#define FLASHC_CRYPTO_BUFF_CTL_PREF_EN_Msk (0x40000000UL)       /*!< FLASHC CRYPTO_BUFF_CTL: PREF_EN (Bitfield-Mask: 0x01)     */
#define FLASHC_CRYPTO_BUFF_CTL_ENABLED_Pos (31UL)               /*!< FLASHC CRYPTO_BUFF_CTL: ENABLED (Bit 31)                  */
#define FLASHC_CRYPTO_BUFF_CTL_ENABLED_Msk (0x80000000UL)       /*!< FLASHC CRYPTO_BUFF_CTL: ENABLED (Bitfield-Mask: 0x01)     */
/* ================================================  FLASHC.CRYPTO_BUFF_CMD  ================================================= */
#define FLASHC_CRYPTO_BUFF_CMD_INV_Pos    (0UL)                 /*!< FLASHC CRYPTO_BUFF_CMD: INV (Bit 0)                       */
#define FLASHC_CRYPTO_BUFF_CMD_INV_Msk    (0x1UL)               /*!< FLASHC CRYPTO_BUFF_CMD: INV (Bitfield-Mask: 0x01)         */
/* ==================================================  FLASHC.DW0_BUFF_CTL  ================================================== */
#define FLASHC_DW0_BUFF_CTL_PREF_EN_Pos   (30UL)                /*!< FLASHC DW0_BUFF_CTL: PREF_EN (Bit 30)                     */
#define FLASHC_DW0_BUFF_CTL_PREF_EN_Msk   (0x40000000UL)        /*!< FLASHC DW0_BUFF_CTL: PREF_EN (Bitfield-Mask: 0x01)        */
#define FLASHC_DW0_BUFF_CTL_ENABLED_Pos   (31UL)                /*!< FLASHC DW0_BUFF_CTL: ENABLED (Bit 31)                     */
#define FLASHC_DW0_BUFF_CTL_ENABLED_Msk   (0x80000000UL)        /*!< FLASHC DW0_BUFF_CTL: ENABLED (Bitfield-Mask: 0x01)        */
/* ==================================================  FLASHC.DW0_BUFF_CMD  ================================================== */
#define FLASHC_DW0_BUFF_CMD_INV_Pos       (0UL)                 /*!< FLASHC DW0_BUFF_CMD: INV (Bit 0)                          */
#define FLASHC_DW0_BUFF_CMD_INV_Msk       (0x1UL)               /*!< FLASHC DW0_BUFF_CMD: INV (Bitfield-Mask: 0x01)            */
/* ==================================================  FLASHC.DW1_BUFF_CTL  ================================================== */
#define FLASHC_DW1_BUFF_CTL_PREF_EN_Pos   (30UL)                /*!< FLASHC DW1_BUFF_CTL: PREF_EN (Bit 30)                     */
#define FLASHC_DW1_BUFF_CTL_PREF_EN_Msk   (0x40000000UL)        /*!< FLASHC DW1_BUFF_CTL: PREF_EN (Bitfield-Mask: 0x01)        */
#define FLASHC_DW1_BUFF_CTL_ENABLED_Pos   (31UL)                /*!< FLASHC DW1_BUFF_CTL: ENABLED (Bit 31)                     */
#define FLASHC_DW1_BUFF_CTL_ENABLED_Msk   (0x80000000UL)        /*!< FLASHC DW1_BUFF_CTL: ENABLED (Bitfield-Mask: 0x01)        */
/* ==================================================  FLASHC.DW1_BUFF_CMD  ================================================== */
#define FLASHC_DW1_BUFF_CMD_INV_Pos       (0UL)                 /*!< FLASHC DW1_BUFF_CMD: INV (Bit 0)                          */
#define FLASHC_DW1_BUFF_CMD_INV_Msk       (0x1UL)               /*!< FLASHC DW1_BUFF_CMD: INV (Bitfield-Mask: 0x01)            */
/* ==================================================  FLASHC.DAP_BUFF_CTL  ================================================== */
#define FLASHC_DAP_BUFF_CTL_PREF_EN_Pos   (30UL)                /*!< FLASHC DAP_BUFF_CTL: PREF_EN (Bit 30)                     */
#define FLASHC_DAP_BUFF_CTL_PREF_EN_Msk   (0x40000000UL)        /*!< FLASHC DAP_BUFF_CTL: PREF_EN (Bitfield-Mask: 0x01)        */
#define FLASHC_DAP_BUFF_CTL_ENABLED_Pos   (31UL)                /*!< FLASHC DAP_BUFF_CTL: ENABLED (Bit 31)                     */
#define FLASHC_DAP_BUFF_CTL_ENABLED_Msk   (0x80000000UL)        /*!< FLASHC DAP_BUFF_CTL: ENABLED (Bitfield-Mask: 0x01)        */
/* ==================================================  FLASHC.DAP_BUFF_CMD  ================================================== */
#define FLASHC_DAP_BUFF_CMD_INV_Pos       (0UL)                 /*!< FLASHC DAP_BUFF_CMD: INV (Bit 0)                          */
#define FLASHC_DAP_BUFF_CMD_INV_Msk       (0x1UL)               /*!< FLASHC DAP_BUFF_CMD: INV (Bitfield-Mask: 0x01)            */
/* ================================================  FLASHC.EXT_MS0_BUFF_CTL  ================================================ */
#define FLASHC_EXT_MS0_BUFF_CTL_PREF_EN_Pos (30UL)              /*!< FLASHC EXT_MS0_BUFF_CTL: PREF_EN (Bit 30)                 */
#define FLASHC_EXT_MS0_BUFF_CTL_PREF_EN_Msk (0x40000000UL)      /*!< FLASHC EXT_MS0_BUFF_CTL: PREF_EN (Bitfield-Mask: 0x01)    */
#define FLASHC_EXT_MS0_BUFF_CTL_ENABLED_Pos (31UL)              /*!< FLASHC EXT_MS0_BUFF_CTL: ENABLED (Bit 31)                 */
#define FLASHC_EXT_MS0_BUFF_CTL_ENABLED_Msk (0x80000000UL)      /*!< FLASHC EXT_MS0_BUFF_CTL: ENABLED (Bitfield-Mask: 0x01)    */
/* ================================================  FLASHC.EXT_MS0_BUFF_CMD  ================================================ */
#define FLASHC_EXT_MS0_BUFF_CMD_INV_Pos   (0UL)                 /*!< FLASHC EXT_MS0_BUFF_CMD: INV (Bit 0)                      */
#define FLASHC_EXT_MS0_BUFF_CMD_INV_Msk   (0x1UL)               /*!< FLASHC EXT_MS0_BUFF_CMD: INV (Bitfield-Mask: 0x01)        */
/* ================================================  FLASHC.EXT_MS1_BUFF_CTL  ================================================ */
#define FLASHC_EXT_MS1_BUFF_CTL_PREF_EN_Pos (30UL)              /*!< FLASHC EXT_MS1_BUFF_CTL: PREF_EN (Bit 30)                 */
#define FLASHC_EXT_MS1_BUFF_CTL_PREF_EN_Msk (0x40000000UL)      /*!< FLASHC EXT_MS1_BUFF_CTL: PREF_EN (Bitfield-Mask: 0x01)    */
#define FLASHC_EXT_MS1_BUFF_CTL_ENABLED_Pos (31UL)              /*!< FLASHC EXT_MS1_BUFF_CTL: ENABLED (Bit 31)                 */
#define FLASHC_EXT_MS1_BUFF_CTL_ENABLED_Msk (0x80000000UL)      /*!< FLASHC EXT_MS1_BUFF_CTL: ENABLED (Bitfield-Mask: 0x01)    */
/* ================================================  FLASHC.EXT_MS1_BUFF_CMD  ================================================ */
#define FLASHC_EXT_MS1_BUFF_CMD_INV_Pos   (0UL)                 /*!< FLASHC EXT_MS1_BUFF_CMD: INV (Bit 0)                      */
#define FLASHC_EXT_MS1_BUFF_CMD_INV_Msk   (0x1UL)               /*!< FLASHC EXT_MS1_BUFF_CMD: INV (Bitfield-Mask: 0x01)        */


#endif /* _CYIP_FLASHC_H_ */


/* [] END OF FILE */
