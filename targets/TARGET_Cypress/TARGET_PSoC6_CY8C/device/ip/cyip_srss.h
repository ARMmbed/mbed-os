/***************************************************************************//**
* \file cyip_srss.h
*
* \brief
* SRSS IP definitions
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

#ifndef _CYIP_SRSS_H_
#define _CYIP_SRSS_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                           SRSS                                            ================ */
/* =========================================================================================================================== */

#define MCWDT_STRUCT_SECTION_SIZE   0x00000040UL
#define CLK_CSV_SECTION_SIZE        0x00000010UL
#define SRSS_SECTION_SIZE           0x00010000UL

/**
  * \brief Multi-Counter Watchdog Timer (MCWDT_STRUCT)
  */
typedef struct {
   __IM uint32_t RESERVED;
  __IOM uint32_t MCWDT_CNTLOW;                  /*!< 0x00000004 Multi-Counter Watchdog Sub-counters 0/1                        */
  __IOM uint32_t MCWDT_CNTHIGH;                 /*!< 0x00000008 Multi-Counter Watchdog Sub-counter 2                           */
  __IOM uint32_t MCWDT_MATCH;                   /*!< 0x0000000C Multi-Counter Watchdog Counter Match Register                  */
  __IOM uint32_t MCWDT_CONFIG;                  /*!< 0x00000010 Multi-Counter Watchdog Counter Configuration                   */
  __IOM uint32_t MCWDT_CTL;                     /*!< 0x00000014 Multi-Counter Watchdog Counter Control                         */
  __IOM uint32_t MCWDT_INTR;                    /*!< 0x00000018 Multi-Counter Watchdog Counter Interrupt Register              */
  __IOM uint32_t MCWDT_INTR_SET;                /*!< 0x0000001C Multi-Counter Watchdog Counter Interrupt Set Register          */
  __IOM uint32_t MCWDT_INTR_MASK;               /*!< 0x00000020 Multi-Counter Watchdog Counter Interrupt Mask Register         */
   __IM uint32_t MCWDT_INTR_MASKED;             /*!< 0x00000024 Multi-Counter Watchdog Counter Interrupt Masked Register       */
  __IOM uint32_t MCWDT_LOCK;                    /*!< 0x00000028 Multi-Counter Watchdog Counter Lock Register                   */
   __IM uint32_t RESERVED1[5];
} MCWDT_STRUCT_Type;                            /*!< Size = 44 (0x2C)                                                          */

/**
  * \brief SRSS Clock Supervisor (CSV) registers (CLK_CSV)
  */
typedef struct {
  __IOM uint32_t HF_CTL;                        /*!< 0x00000000 HFCLK Clock Supervision Control Register                       */
  __IOM uint32_t HF_LIMIT;                      /*!< 0x00000004 HFCLK Clock Supervision Limit Register                         */
   __IM uint32_t HF_CNT;                        /*!< 0x00000008 HFCLK Clock Supervision Count Register                         */
   __IM uint32_t RESERVED;
} CLK_CSV_Type;                                 /*!< Size = 12 (0xC)                                                           */

/**
  * \brief SRSS Core Registers (SRSS)
  */
typedef struct {                                /*!< SRSS Structure                                                            */
  __IOM uint32_t PWR_CTL;                       /*!< 0x00000000 Power Mode Control                                             */
  __IOM uint32_t PWR_HIBERNATE;                 /*!< 0x00000004 HIBERNATE Mode Register                                        */
  __IOM uint32_t PWR_LVD_CTL;                   /*!< 0x00000008 Low Voltage Detector (LVD) Configuration Register              */
   __IM uint32_t RESERVED[2];
  __IOM uint32_t PWR_BUCK_CTL;                  /*!< 0x00000014 SIMO Buck Control Register                                     */
  __IOM uint32_t PWR_BUCK_CTL2;                 /*!< 0x00000018 SIMO Buck Control Register 2                                   */
   __IM uint32_t PWR_LVD_STATUS;                /*!< 0x0000001C Low Voltage Detector (LVD) Status Register                     */
   __IM uint32_t RESERVED1[24];
  __IOM uint32_t PWR_HIB_DATA[16];              /*!< 0x00000080 HIBERNATE Data Register                                        */
   __IM uint32_t RESERVED2[48];
  __IOM uint32_t WDT_CTL;                       /*!< 0x00000180 Watchdog Counter Control Register                              */
  __IOM uint32_t WDT_CNT;                       /*!< 0x00000184 Watchdog Counter Count Register                                */
  __IOM uint32_t WDT_MATCH;                     /*!< 0x00000188 Watchdog Counter Match Register                                */
   __IM uint32_t RESERVED3[29];
        MCWDT_STRUCT_Type MCWDT_STRUCT[4];      /*!< 0x00000200 Multi-Counter Watchdog Timer                                   */
  __IOM uint32_t CLK_DSI_SELECT[16];            /*!< 0x00000300 Clock DSI Select Register                                      */
  __IOM uint32_t CLK_PATH_SELECT[16];           /*!< 0x00000340 Clock Path Select Register                                     */
  __IOM uint32_t CLK_ROOT_SELECT[16];           /*!< 0x00000380 Clock Root Select Register                                     */
   __IM uint32_t RESERVED4[16];
        CLK_CSV_Type CLK_CSV[16];               /*!< 0x00000400 SRSS Clock Supervisor (CSV) registers                          */
  __IOM uint32_t CLK_SELECT;                    /*!< 0x00000500 Clock selection register                                       */
  __IOM uint32_t CLK_TIMER_CTL;                 /*!< 0x00000504 Timer Clock Control Register                                   */
  __IOM uint32_t CLK_CSV_WCO_CTL;               /*!< 0x00000508 WCO Clock Supervision Control Register                         */
  __IOM uint32_t CLK_ILO_CONFIG;                /*!< 0x0000050C HVILO Configuration                                            */
  __IOM uint32_t CLK_IMO_CONFIG;                /*!< 0x00000510 IMO Configuration                                              */
  __IOM uint32_t CLK_OUTPUT_FAST;               /*!< 0x00000514 Fast Clock Output Select Register                              */
  __IOM uint32_t CLK_OUTPUT_SLOW;               /*!< 0x00000518 Slow Clock Output Select Register                              */
  __IOM uint32_t CLK_CAL_CNT1;                  /*!< 0x0000051C Clock Calibration Counter 1                                    */
   __IM uint32_t CLK_CAL_CNT2;                  /*!< 0x00000520 Clock Calibration Counter 2                                    */
   __IM uint32_t RESERVED5[2];
  __IOM uint32_t CLK_ECO_CONFIG;                /*!< 0x0000052C ECO Configuration Register                                     */
   __IM uint32_t CLK_ECO_STATUS;                /*!< 0x00000530 ECO Status Register                                            */
   __IM uint32_t RESERVED6;
  __IOM uint32_t CLK_WCO_DPLL;                  /*!< 0x00000538 WCO DPLL Register                                              */
  __IOM uint32_t CLK_PILO_CONFIG;               /*!< 0x0000053C Precision ILO Configuration Register                           */
   __IM uint32_t RESERVED7[16];
  __IOM uint32_t CLK_FLL_CONFIG;                /*!< 0x00000580 FLL Configuration Register                                     */
  __IOM uint32_t CLK_FLL_CONFIG2;               /*!< 0x00000584 FLL Configuration Register 2                                   */
  __IOM uint32_t CLK_FLL_CONFIG3;               /*!< 0x00000588 FLL Configuration Register 3                                   */
  __IOM uint32_t CLK_FLL_CONFIG4;               /*!< 0x0000058C FLL Configuration Register 4                                   */
  __IOM uint32_t CLK_FLL_STATUS;                /*!< 0x00000590 FLL Status Register                                            */
   __IM uint32_t RESERVED8[27];
  __IOM uint32_t CLK_PLL_CONFIG[15];            /*!< 0x00000600 PLL Configuration Register                                     */
   __IM uint32_t RESERVED9;
  __IOM uint32_t CLK_PLL_STATUS[15];            /*!< 0x00000640 PLL Status Register                                            */
   __IM uint32_t RESERVED10[33];
  __IOM uint32_t SRSS_INTR;                     /*!< 0x00000700 SRSS Interrupt Register                                        */
  __IOM uint32_t SRSS_INTR_SET;                 /*!< 0x00000704 SRSS Interrupt Set Register                                    */
  __IOM uint32_t SRSS_INTR_MASK;                /*!< 0x00000708 SRSS Interrupt Mask Register                                   */
   __IM uint32_t SRSS_INTR_MASKED;              /*!< 0x0000070C SRSS Interrupt Masked Register                                 */
  __IOM uint32_t SRSS_INTR_CFG;                 /*!< 0x00000710 SRSS Interrupt Configuration Register                          */
   __IM uint32_t RESERVED11[59];
  __IOM uint32_t RES_CAUSE;                     /*!< 0x00000800 Reset Cause Observation Register                               */
  __IOM uint32_t RES_CAUSE2;                    /*!< 0x00000804 Reset Cause Observation Register 2                             */
   __IM uint32_t RESERVED12[7614];
  __IOM uint32_t PWR_TRIM_REF_CTL;              /*!< 0x00007F00 Reference Trim Register                                        */
  __IOM uint32_t PWR_TRIM_BODOVP_CTL;           /*!< 0x00007F04 BOD/OVP Trim Register                                          */
  __IOM uint32_t CLK_TRIM_CCO_CTL;              /*!< 0x00007F08 CCO Trim Register                                              */
  __IOM uint32_t CLK_TRIM_CCO_CTL2;             /*!< 0x00007F0C CCO Trim Register 2                                            */
   __IM uint32_t RESERVED13[8];
  __IOM uint32_t PWR_TRIM_WAKE_CTL;             /*!< 0x00007F30 Wakeup Trim Register                                           */
   __IM uint32_t RESERVED14[8183];
  __IOM uint32_t PWR_TRIM_LVD_CTL;              /*!< 0x0000FF10 LVD Trim Register                                              */
   __IM uint32_t RESERVED15;
  __IOM uint32_t CLK_TRIM_ILO_CTL;              /*!< 0x0000FF18 ILO Trim Register                                              */
  __IOM uint32_t PWR_TRIM_PWRSYS_CTL;           /*!< 0x0000FF1C Power System Trim Register                                     */
  __IOM uint32_t CLK_TRIM_ECO_CTL;              /*!< 0x0000FF20 ECO Trim Register                                              */
  __IOM uint32_t CLK_TRIM_PILO_CTL;             /*!< 0x0000FF24 PILO Trim Register                                             */
  __IOM uint32_t CLK_TRIM_PILO_CTL2;            /*!< 0x0000FF28 PILO Trim Register 2                                           */
  __IOM uint32_t CLK_TRIM_PILO_CTL3;            /*!< 0x0000FF2C PILO Trim Register 3                                           */
} SRSS_Type;                                    /*!< Size = 65328 (0xFF30)                                                     */


/* ===============================================  MCWDT_STRUCT.MCWDT_CNTLOW  =============================================== */
#define MCWDT_STRUCT_MCWDT_CNTLOW_WDT_CTR0_Pos (0UL)            /*!< MCWDT_STRUCT MCWDT_CNTLOW: WDT_CTR0 (Bit 0)               */
#define MCWDT_STRUCT_MCWDT_CNTLOW_WDT_CTR0_Msk (0xffffUL)       /*!< MCWDT_STRUCT MCWDT_CNTLOW: WDT_CTR0 (Bitfield-Mask: 0xffff)*/
#define MCWDT_STRUCT_MCWDT_CNTLOW_WDT_CTR1_Pos (16UL)           /*!< MCWDT_STRUCT MCWDT_CNTLOW: WDT_CTR1 (Bit 16)              */
#define MCWDT_STRUCT_MCWDT_CNTLOW_WDT_CTR1_Msk (0xffff0000UL)   /*!< MCWDT_STRUCT MCWDT_CNTLOW: WDT_CTR1 (Bitfield-Mask: 0xffff)*/
/* ==============================================  MCWDT_STRUCT.MCWDT_CNTHIGH  =============================================== */
#define MCWDT_STRUCT_MCWDT_CNTHIGH_WDT_CTR2_Pos (0UL)           /*!< MCWDT_STRUCT MCWDT_CNTHIGH: WDT_CTR2 (Bit 0)              */
#define MCWDT_STRUCT_MCWDT_CNTHIGH_WDT_CTR2_Msk (0xffffffffUL)  /*!< MCWDT_STRUCT MCWDT_CNTHIGH: WDT_CTR2 (Bitfield-Mask: 0xffffffff)*/
/* ===============================================  MCWDT_STRUCT.MCWDT_MATCH  ================================================ */
#define MCWDT_STRUCT_MCWDT_MATCH_WDT_MATCH0_Pos (0UL)           /*!< MCWDT_STRUCT MCWDT_MATCH: WDT_MATCH0 (Bit 0)              */
#define MCWDT_STRUCT_MCWDT_MATCH_WDT_MATCH0_Msk (0xffffUL)      /*!< MCWDT_STRUCT MCWDT_MATCH: WDT_MATCH0 (Bitfield-Mask: 0xffff)*/
#define MCWDT_STRUCT_MCWDT_MATCH_WDT_MATCH1_Pos (16UL)          /*!< MCWDT_STRUCT MCWDT_MATCH: WDT_MATCH1 (Bit 16)             */
#define MCWDT_STRUCT_MCWDT_MATCH_WDT_MATCH1_Msk (0xffff0000UL)  /*!< MCWDT_STRUCT MCWDT_MATCH: WDT_MATCH1 (Bitfield-Mask: 0xffff)*/
/* ===============================================  MCWDT_STRUCT.MCWDT_CONFIG  =============================================== */
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_MODE0_Pos (0UL)           /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_MODE0 (Bit 0)              */
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_MODE0_Msk (0x3UL)         /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_MODE0 (Bitfield-Mask: 0x03)*/
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_CLEAR0_Pos (2UL)          /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_CLEAR0 (Bit 2)             */
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_CLEAR0_Msk (0x4UL)        /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_CLEAR0 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_CASCADE0_1_Pos (3UL)      /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_CASCADE0_1 (Bit 3)         */
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_CASCADE0_1_Msk (0x8UL)    /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_CASCADE0_1 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_MODE1_Pos (8UL)           /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_MODE1 (Bit 8)              */
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_MODE1_Msk (0x300UL)       /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_MODE1 (Bitfield-Mask: 0x03)*/
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_CLEAR1_Pos (10UL)         /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_CLEAR1 (Bit 10)            */
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_CLEAR1_Msk (0x400UL)      /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_CLEAR1 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_CASCADE1_2_Pos (11UL)     /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_CASCADE1_2 (Bit 11)        */
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_CASCADE1_2_Msk (0x800UL)  /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_CASCADE1_2 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_MODE2_Pos (16UL)          /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_MODE2 (Bit 16)             */
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_MODE2_Msk (0x10000UL)     /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_MODE2 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_BITS2_Pos (24UL)          /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_BITS2 (Bit 24)             */
#define MCWDT_STRUCT_MCWDT_CONFIG_WDT_BITS2_Msk (0x1f000000UL)  /*!< MCWDT_STRUCT MCWDT_CONFIG: WDT_BITS2 (Bitfield-Mask: 0x1f)*/
/* ================================================  MCWDT_STRUCT.MCWDT_CTL  ================================================= */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLE0_Pos (0UL)            /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLE0 (Bit 0)               */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLE0_Msk (0x1UL)          /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLE0 (Bitfield-Mask: 0x01) */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLED0_Pos (1UL)           /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLED0 (Bit 1)              */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLED0_Msk (0x2UL)         /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLED0 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_CTL_WDT_RESET0_Pos (3UL)             /*!< MCWDT_STRUCT MCWDT_CTL: WDT_RESET0 (Bit 3)                */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_RESET0_Msk (0x8UL)           /*!< MCWDT_STRUCT MCWDT_CTL: WDT_RESET0 (Bitfield-Mask: 0x01)  */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLE1_Pos (8UL)            /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLE1 (Bit 8)               */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLE1_Msk (0x100UL)        /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLE1 (Bitfield-Mask: 0x01) */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLED1_Pos (9UL)           /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLED1 (Bit 9)              */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLED1_Msk (0x200UL)       /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLED1 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_CTL_WDT_RESET1_Pos (11UL)            /*!< MCWDT_STRUCT MCWDT_CTL: WDT_RESET1 (Bit 11)               */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_RESET1_Msk (0x800UL)         /*!< MCWDT_STRUCT MCWDT_CTL: WDT_RESET1 (Bitfield-Mask: 0x01)  */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLE2_Pos (16UL)           /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLE2 (Bit 16)              */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLE2_Msk (0x10000UL)      /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLE2 (Bitfield-Mask: 0x01) */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLED2_Pos (17UL)          /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLED2 (Bit 17)             */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_ENABLED2_Msk (0x20000UL)     /*!< MCWDT_STRUCT MCWDT_CTL: WDT_ENABLED2 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_CTL_WDT_RESET2_Pos (19UL)            /*!< MCWDT_STRUCT MCWDT_CTL: WDT_RESET2 (Bit 19)               */
#define MCWDT_STRUCT_MCWDT_CTL_WDT_RESET2_Msk (0x80000UL)       /*!< MCWDT_STRUCT MCWDT_CTL: WDT_RESET2 (Bitfield-Mask: 0x01)  */
/* ================================================  MCWDT_STRUCT.MCWDT_INTR  ================================================ */
#define MCWDT_STRUCT_MCWDT_INTR_MCWDT_INT0_Pos (0UL)            /*!< MCWDT_STRUCT MCWDT_INTR: MCWDT_INT0 (Bit 0)               */
#define MCWDT_STRUCT_MCWDT_INTR_MCWDT_INT0_Msk (0x1UL)          /*!< MCWDT_STRUCT MCWDT_INTR: MCWDT_INT0 (Bitfield-Mask: 0x01) */
#define MCWDT_STRUCT_MCWDT_INTR_MCWDT_INT1_Pos (1UL)            /*!< MCWDT_STRUCT MCWDT_INTR: MCWDT_INT1 (Bit 1)               */
#define MCWDT_STRUCT_MCWDT_INTR_MCWDT_INT1_Msk (0x2UL)          /*!< MCWDT_STRUCT MCWDT_INTR: MCWDT_INT1 (Bitfield-Mask: 0x01) */
#define MCWDT_STRUCT_MCWDT_INTR_MCWDT_INT2_Pos (2UL)            /*!< MCWDT_STRUCT MCWDT_INTR: MCWDT_INT2 (Bit 2)               */
#define MCWDT_STRUCT_MCWDT_INTR_MCWDT_INT2_Msk (0x4UL)          /*!< MCWDT_STRUCT MCWDT_INTR: MCWDT_INT2 (Bitfield-Mask: 0x01) */
/* ==============================================  MCWDT_STRUCT.MCWDT_INTR_SET  ============================================== */
#define MCWDT_STRUCT_MCWDT_INTR_SET_MCWDT_INT0_Pos (0UL)        /*!< MCWDT_STRUCT MCWDT_INTR_SET: MCWDT_INT0 (Bit 0)           */
#define MCWDT_STRUCT_MCWDT_INTR_SET_MCWDT_INT0_Msk (0x1UL)      /*!< MCWDT_STRUCT MCWDT_INTR_SET: MCWDT_INT0 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_INTR_SET_MCWDT_INT1_Pos (1UL)        /*!< MCWDT_STRUCT MCWDT_INTR_SET: MCWDT_INT1 (Bit 1)           */
#define MCWDT_STRUCT_MCWDT_INTR_SET_MCWDT_INT1_Msk (0x2UL)      /*!< MCWDT_STRUCT MCWDT_INTR_SET: MCWDT_INT1 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_INTR_SET_MCWDT_INT2_Pos (2UL)        /*!< MCWDT_STRUCT MCWDT_INTR_SET: MCWDT_INT2 (Bit 2)           */
#define MCWDT_STRUCT_MCWDT_INTR_SET_MCWDT_INT2_Msk (0x4UL)      /*!< MCWDT_STRUCT MCWDT_INTR_SET: MCWDT_INT2 (Bitfield-Mask: 0x01)*/
/* =============================================  MCWDT_STRUCT.MCWDT_INTR_MASK  ============================================== */
#define MCWDT_STRUCT_MCWDT_INTR_MASK_MCWDT_INT0_Pos (0UL)       /*!< MCWDT_STRUCT MCWDT_INTR_MASK: MCWDT_INT0 (Bit 0)          */
#define MCWDT_STRUCT_MCWDT_INTR_MASK_MCWDT_INT0_Msk (0x1UL)     /*!< MCWDT_STRUCT MCWDT_INTR_MASK: MCWDT_INT0 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_INTR_MASK_MCWDT_INT1_Pos (1UL)       /*!< MCWDT_STRUCT MCWDT_INTR_MASK: MCWDT_INT1 (Bit 1)          */
#define MCWDT_STRUCT_MCWDT_INTR_MASK_MCWDT_INT1_Msk (0x2UL)     /*!< MCWDT_STRUCT MCWDT_INTR_MASK: MCWDT_INT1 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_INTR_MASK_MCWDT_INT2_Pos (2UL)       /*!< MCWDT_STRUCT MCWDT_INTR_MASK: MCWDT_INT2 (Bit 2)          */
#define MCWDT_STRUCT_MCWDT_INTR_MASK_MCWDT_INT2_Msk (0x4UL)     /*!< MCWDT_STRUCT MCWDT_INTR_MASK: MCWDT_INT2 (Bitfield-Mask: 0x01)*/
/* ============================================  MCWDT_STRUCT.MCWDT_INTR_MASKED  ============================================= */
#define MCWDT_STRUCT_MCWDT_INTR_MASKED_MCWDT_INT0_Pos (0UL)     /*!< MCWDT_STRUCT MCWDT_INTR_MASKED: MCWDT_INT0 (Bit 0)        */
#define MCWDT_STRUCT_MCWDT_INTR_MASKED_MCWDT_INT0_Msk (0x1UL)   /*!< MCWDT_STRUCT MCWDT_INTR_MASKED: MCWDT_INT0 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_INTR_MASKED_MCWDT_INT1_Pos (1UL)     /*!< MCWDT_STRUCT MCWDT_INTR_MASKED: MCWDT_INT1 (Bit 1)        */
#define MCWDT_STRUCT_MCWDT_INTR_MASKED_MCWDT_INT1_Msk (0x2UL)   /*!< MCWDT_STRUCT MCWDT_INTR_MASKED: MCWDT_INT1 (Bitfield-Mask: 0x01)*/
#define MCWDT_STRUCT_MCWDT_INTR_MASKED_MCWDT_INT2_Pos (2UL)     /*!< MCWDT_STRUCT MCWDT_INTR_MASKED: MCWDT_INT2 (Bit 2)        */
#define MCWDT_STRUCT_MCWDT_INTR_MASKED_MCWDT_INT2_Msk (0x4UL)   /*!< MCWDT_STRUCT MCWDT_INTR_MASKED: MCWDT_INT2 (Bitfield-Mask: 0x01)*/
/* ================================================  MCWDT_STRUCT.MCWDT_LOCK  ================================================ */
#define MCWDT_STRUCT_MCWDT_LOCK_MCWDT_LOCK_Pos (30UL)           /*!< MCWDT_STRUCT MCWDT_LOCK: MCWDT_LOCK (Bit 30)              */
#define MCWDT_STRUCT_MCWDT_LOCK_MCWDT_LOCK_Msk (0xc0000000UL)   /*!< MCWDT_STRUCT MCWDT_LOCK: MCWDT_LOCK (Bitfield-Mask: 0x03) */


/* ====================================================  CLK_CSV.HF_CTL  ===================================================== */
#define CLK_CSV_HF_CTL_CSV_FREQ_WINDOW_Pos (0UL)                /*!< CLK_CSV HF_CTL: CSV_FREQ_WINDOW (Bit 0)                   */
#define CLK_CSV_HF_CTL_CSV_FREQ_WINDOW_Msk (0xffffUL)           /*!< CLK_CSV HF_CTL: CSV_FREQ_WINDOW (Bitfield-Mask: 0xffff)   */
#define CLK_CSV_HF_CTL_CSV_MUX_Pos        (16UL)                /*!< CLK_CSV HF_CTL: CSV_MUX (Bit 16)                          */
#define CLK_CSV_HF_CTL_CSV_MUX_Msk        (0x30000UL)           /*!< CLK_CSV HF_CTL: CSV_MUX (Bitfield-Mask: 0x03)             */
#define CLK_CSV_HF_CTL_CSV_LOSS_WINDOW_Pos (20UL)               /*!< CLK_CSV HF_CTL: CSV_LOSS_WINDOW (Bit 20)                  */
#define CLK_CSV_HF_CTL_CSV_LOSS_WINDOW_Msk (0x700000UL)         /*!< CLK_CSV HF_CTL: CSV_LOSS_WINDOW (Bitfield-Mask: 0x07)     */
#define CLK_CSV_HF_CTL_CSV_FREQ_ACTION_Pos (24UL)               /*!< CLK_CSV HF_CTL: CSV_FREQ_ACTION (Bit 24)                  */
#define CLK_CSV_HF_CTL_CSV_FREQ_ACTION_Msk (0x3000000UL)        /*!< CLK_CSV HF_CTL: CSV_FREQ_ACTION (Bitfield-Mask: 0x03)     */
#define CLK_CSV_HF_CTL_CSV_FREQ_ONESHOT_Pos (26UL)              /*!< CLK_CSV HF_CTL: CSV_FREQ_ONESHOT (Bit 26)                 */
#define CLK_CSV_HF_CTL_CSV_FREQ_ONESHOT_Msk (0x4000000UL)       /*!< CLK_CSV HF_CTL: CSV_FREQ_ONESHOT (Bitfield-Mask: 0x01)    */
#define CLK_CSV_HF_CTL_CSV_FREQ_EN_Pos    (27UL)                /*!< CLK_CSV HF_CTL: CSV_FREQ_EN (Bit 27)                      */
#define CLK_CSV_HF_CTL_CSV_FREQ_EN_Msk    (0x8000000UL)         /*!< CLK_CSV HF_CTL: CSV_FREQ_EN (Bitfield-Mask: 0x01)         */
#define CLK_CSV_HF_CTL_CSV_LOSS_ACTION_Pos (28UL)               /*!< CLK_CSV HF_CTL: CSV_LOSS_ACTION (Bit 28)                  */
#define CLK_CSV_HF_CTL_CSV_LOSS_ACTION_Msk (0x30000000UL)       /*!< CLK_CSV HF_CTL: CSV_LOSS_ACTION (Bitfield-Mask: 0x03)     */
#define CLK_CSV_HF_CTL_CSV_LOSS_EN_Pos    (31UL)                /*!< CLK_CSV HF_CTL: CSV_LOSS_EN (Bit 31)                      */
#define CLK_CSV_HF_CTL_CSV_LOSS_EN_Msk    (0x80000000UL)        /*!< CLK_CSV HF_CTL: CSV_LOSS_EN (Bitfield-Mask: 0x01)         */
/* ===================================================  CLK_CSV.HF_LIMIT  ==================================================== */
#define CLK_CSV_HF_LIMIT_LOWER_LIMIT_Pos  (0UL)                 /*!< CLK_CSV HF_LIMIT: LOWER_LIMIT (Bit 0)                     */
#define CLK_CSV_HF_LIMIT_LOWER_LIMIT_Msk  (0xffffUL)            /*!< CLK_CSV HF_LIMIT: LOWER_LIMIT (Bitfield-Mask: 0xffff)     */
#define CLK_CSV_HF_LIMIT_UPPER_LIMIT_Pos  (16UL)                /*!< CLK_CSV HF_LIMIT: UPPER_LIMIT (Bit 16)                    */
#define CLK_CSV_HF_LIMIT_UPPER_LIMIT_Msk  (0xffff0000UL)        /*!< CLK_CSV HF_LIMIT: UPPER_LIMIT (Bitfield-Mask: 0xffff)     */
/* ====================================================  CLK_CSV.HF_CNT  ===================================================== */
#define CLK_CSV_HF_CNT_CNT_Pos            (0UL)                 /*!< CLK_CSV HF_CNT: CNT (Bit 0)                               */
#define CLK_CSV_HF_CNT_CNT_Msk            (0xffffUL)            /*!< CLK_CSV HF_CNT: CNT (Bitfield-Mask: 0xffff)               */


/* =====================================================  SRSS.PWR_CTL  ====================================================== */
#define SRSS_PWR_CTL_POWER_MODE_Pos       (0UL)                 /*!< SRSS PWR_CTL: POWER_MODE (Bit 0)                          */
#define SRSS_PWR_CTL_POWER_MODE_Msk       (0x3UL)               /*!< SRSS PWR_CTL: POWER_MODE (Bitfield-Mask: 0x03)            */
#define SRSS_PWR_CTL_DEBUG_SESSION_Pos    (4UL)                 /*!< SRSS PWR_CTL: DEBUG_SESSION (Bit 4)                       */
#define SRSS_PWR_CTL_DEBUG_SESSION_Msk    (0x10UL)              /*!< SRSS PWR_CTL: DEBUG_SESSION (Bitfield-Mask: 0x01)         */
#define SRSS_PWR_CTL_LPM_READY_Pos        (5UL)                 /*!< SRSS PWR_CTL: LPM_READY (Bit 5)                           */
#define SRSS_PWR_CTL_LPM_READY_Msk        (0x20UL)              /*!< SRSS PWR_CTL: LPM_READY (Bitfield-Mask: 0x01)             */
#define SRSS_PWR_CTL_DPSLP_REG_DIS_Pos    (20UL)                /*!< SRSS PWR_CTL: DPSLP_REG_DIS (Bit 20)                      */
#define SRSS_PWR_CTL_DPSLP_REG_DIS_Msk    (0x100000UL)          /*!< SRSS PWR_CTL: DPSLP_REG_DIS (Bitfield-Mask: 0x01)         */
#define SRSS_PWR_CTL_RET_REG_DIS_Pos      (21UL)                /*!< SRSS PWR_CTL: RET_REG_DIS (Bit 21)                        */
#define SRSS_PWR_CTL_RET_REG_DIS_Msk      (0x200000UL)          /*!< SRSS PWR_CTL: RET_REG_DIS (Bitfield-Mask: 0x01)           */
#define SRSS_PWR_CTL_NWELL_REG_DIS_Pos    (22UL)                /*!< SRSS PWR_CTL: NWELL_REG_DIS (Bit 22)                      */
#define SRSS_PWR_CTL_NWELL_REG_DIS_Msk    (0x400000UL)          /*!< SRSS PWR_CTL: NWELL_REG_DIS (Bitfield-Mask: 0x01)         */
#define SRSS_PWR_CTL_LINREG_DIS_Pos       (23UL)                /*!< SRSS PWR_CTL: LINREG_DIS (Bit 23)                         */
#define SRSS_PWR_CTL_LINREG_DIS_Msk       (0x800000UL)          /*!< SRSS PWR_CTL: LINREG_DIS (Bitfield-Mask: 0x01)            */
#define SRSS_PWR_CTL_LINREG_LPMODE_Pos    (24UL)                /*!< SRSS PWR_CTL: LINREG_LPMODE (Bit 24)                      */
#define SRSS_PWR_CTL_LINREG_LPMODE_Msk    (0x1000000UL)         /*!< SRSS PWR_CTL: LINREG_LPMODE (Bitfield-Mask: 0x01)         */
#define SRSS_PWR_CTL_PORBOD_LPMODE_Pos    (25UL)                /*!< SRSS PWR_CTL: PORBOD_LPMODE (Bit 25)                      */
#define SRSS_PWR_CTL_PORBOD_LPMODE_Msk    (0x2000000UL)         /*!< SRSS PWR_CTL: PORBOD_LPMODE (Bitfield-Mask: 0x01)         */
#define SRSS_PWR_CTL_BGREF_LPMODE_Pos     (26UL)                /*!< SRSS PWR_CTL: BGREF_LPMODE (Bit 26)                       */
#define SRSS_PWR_CTL_BGREF_LPMODE_Msk     (0x4000000UL)         /*!< SRSS PWR_CTL: BGREF_LPMODE (Bitfield-Mask: 0x01)          */
#define SRSS_PWR_CTL_PLL_LS_BYPASS_Pos    (27UL)                /*!< SRSS PWR_CTL: PLL_LS_BYPASS (Bit 27)                      */
#define SRSS_PWR_CTL_PLL_LS_BYPASS_Msk    (0x8000000UL)         /*!< SRSS PWR_CTL: PLL_LS_BYPASS (Bitfield-Mask: 0x01)         */
#define SRSS_PWR_CTL_VREFBUF_LPMODE_Pos   (28UL)                /*!< SRSS PWR_CTL: VREFBUF_LPMODE (Bit 28)                     */
#define SRSS_PWR_CTL_VREFBUF_LPMODE_Msk   (0x10000000UL)        /*!< SRSS PWR_CTL: VREFBUF_LPMODE (Bitfield-Mask: 0x01)        */
#define SRSS_PWR_CTL_VREFBUF_DIS_Pos      (29UL)                /*!< SRSS PWR_CTL: VREFBUF_DIS (Bit 29)                        */
#define SRSS_PWR_CTL_VREFBUF_DIS_Msk      (0x20000000UL)        /*!< SRSS PWR_CTL: VREFBUF_DIS (Bitfield-Mask: 0x01)           */
#define SRSS_PWR_CTL_ACT_REF_DIS_Pos      (30UL)                /*!< SRSS PWR_CTL: ACT_REF_DIS (Bit 30)                        */
#define SRSS_PWR_CTL_ACT_REF_DIS_Msk      (0x40000000UL)        /*!< SRSS PWR_CTL: ACT_REF_DIS (Bitfield-Mask: 0x01)           */
#define SRSS_PWR_CTL_ACT_REF_OK_Pos       (31UL)                /*!< SRSS PWR_CTL: ACT_REF_OK (Bit 31)                         */
#define SRSS_PWR_CTL_ACT_REF_OK_Msk       (0x80000000UL)        /*!< SRSS PWR_CTL: ACT_REF_OK (Bitfield-Mask: 0x01)            */
/* ==================================================  SRSS.PWR_HIBERNATE  =================================================== */
#define SRSS_PWR_HIBERNATE_TOKEN_Pos      (0UL)                 /*!< SRSS PWR_HIBERNATE: TOKEN (Bit 0)                         */
#define SRSS_PWR_HIBERNATE_TOKEN_Msk      (0xffUL)              /*!< SRSS PWR_HIBERNATE: TOKEN (Bitfield-Mask: 0xff)           */
#define SRSS_PWR_HIBERNATE_UNLOCK_Pos     (8UL)                 /*!< SRSS PWR_HIBERNATE: UNLOCK (Bit 8)                        */
#define SRSS_PWR_HIBERNATE_UNLOCK_Msk     (0xff00UL)            /*!< SRSS PWR_HIBERNATE: UNLOCK (Bitfield-Mask: 0xff)          */
#define SRSS_PWR_HIBERNATE_FREEZE_Pos     (17UL)                /*!< SRSS PWR_HIBERNATE: FREEZE (Bit 17)                       */
#define SRSS_PWR_HIBERNATE_FREEZE_Msk     (0x20000UL)           /*!< SRSS PWR_HIBERNATE: FREEZE (Bitfield-Mask: 0x01)          */
#define SRSS_PWR_HIBERNATE_MASK_HIBALARM_Pos (18UL)             /*!< SRSS PWR_HIBERNATE: MASK_HIBALARM (Bit 18)                */
#define SRSS_PWR_HIBERNATE_MASK_HIBALARM_Msk (0x40000UL)        /*!< SRSS PWR_HIBERNATE: MASK_HIBALARM (Bitfield-Mask: 0x01)   */
#define SRSS_PWR_HIBERNATE_MASK_HIBWDT_Pos (19UL)               /*!< SRSS PWR_HIBERNATE: MASK_HIBWDT (Bit 19)                  */
#define SRSS_PWR_HIBERNATE_MASK_HIBWDT_Msk (0x80000UL)          /*!< SRSS PWR_HIBERNATE: MASK_HIBWDT (Bitfield-Mask: 0x01)     */
#define SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Pos (20UL)           /*!< SRSS PWR_HIBERNATE: POLARITY_HIBPIN (Bit 20)              */
#define SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Msk (0xf00000UL)     /*!< SRSS PWR_HIBERNATE: POLARITY_HIBPIN (Bitfield-Mask: 0x0f) */
#define SRSS_PWR_HIBERNATE_MASK_HIBPIN_Pos (24UL)               /*!< SRSS PWR_HIBERNATE: MASK_HIBPIN (Bit 24)                  */
#define SRSS_PWR_HIBERNATE_MASK_HIBPIN_Msk (0xf000000UL)        /*!< SRSS PWR_HIBERNATE: MASK_HIBPIN (Bitfield-Mask: 0x0f)     */
#define SRSS_PWR_HIBERNATE_HIBERNATE_DISABLE_Pos (30UL)         /*!< SRSS PWR_HIBERNATE: HIBERNATE_DISABLE (Bit 30)            */
#define SRSS_PWR_HIBERNATE_HIBERNATE_DISABLE_Msk (0x40000000UL) /*!< SRSS PWR_HIBERNATE: HIBERNATE_DISABLE (Bitfield-Mask: 0x01)*/
#define SRSS_PWR_HIBERNATE_HIBERNATE_Pos  (31UL)                /*!< SRSS PWR_HIBERNATE: HIBERNATE (Bit 31)                    */
#define SRSS_PWR_HIBERNATE_HIBERNATE_Msk  (0x80000000UL)        /*!< SRSS PWR_HIBERNATE: HIBERNATE (Bitfield-Mask: 0x01)       */
/* ===================================================  SRSS.PWR_LVD_CTL  ==================================================== */
#define SRSS_PWR_LVD_CTL_HVLVD1_TRIPSEL_Pos (0UL)               /*!< SRSS PWR_LVD_CTL: HVLVD1_TRIPSEL (Bit 0)                  */
#define SRSS_PWR_LVD_CTL_HVLVD1_TRIPSEL_Msk (0xfUL)             /*!< SRSS PWR_LVD_CTL: HVLVD1_TRIPSEL (Bitfield-Mask: 0x0f)    */
#define SRSS_PWR_LVD_CTL_HVLVD1_SRCSEL_Pos (4UL)                /*!< SRSS PWR_LVD_CTL: HVLVD1_SRCSEL (Bit 4)                   */
#define SRSS_PWR_LVD_CTL_HVLVD1_SRCSEL_Msk (0x70UL)             /*!< SRSS PWR_LVD_CTL: HVLVD1_SRCSEL (Bitfield-Mask: 0x07)     */
#define SRSS_PWR_LVD_CTL_HVLVD1_EN_Pos    (7UL)                 /*!< SRSS PWR_LVD_CTL: HVLVD1_EN (Bit 7)                       */
#define SRSS_PWR_LVD_CTL_HVLVD1_EN_Msk    (0x80UL)              /*!< SRSS PWR_LVD_CTL: HVLVD1_EN (Bitfield-Mask: 0x01)         */
/* ===================================================  SRSS.PWR_BUCK_CTL  =================================================== */
#define SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL_Pos (0UL)               /*!< SRSS PWR_BUCK_CTL: BUCK_OUT1_SEL (Bit 0)                  */
#define SRSS_PWR_BUCK_CTL_BUCK_OUT1_SEL_Msk (0x7UL)             /*!< SRSS PWR_BUCK_CTL: BUCK_OUT1_SEL (Bitfield-Mask: 0x07)    */
#define SRSS_PWR_BUCK_CTL_BUCK_EN_Pos     (30UL)                /*!< SRSS PWR_BUCK_CTL: BUCK_EN (Bit 30)                       */
#define SRSS_PWR_BUCK_CTL_BUCK_EN_Msk     (0x40000000UL)        /*!< SRSS PWR_BUCK_CTL: BUCK_EN (Bitfield-Mask: 0x01)          */
#define SRSS_PWR_BUCK_CTL_BUCK_OUT1_EN_Pos (31UL)               /*!< SRSS PWR_BUCK_CTL: BUCK_OUT1_EN (Bit 31)                  */
#define SRSS_PWR_BUCK_CTL_BUCK_OUT1_EN_Msk (0x80000000UL)       /*!< SRSS PWR_BUCK_CTL: BUCK_OUT1_EN (Bitfield-Mask: 0x01)     */
/* ==================================================  SRSS.PWR_BUCK_CTL2  =================================================== */
#define SRSS_PWR_BUCK_CTL2_BUCK_OUT2_SEL_Pos (0UL)              /*!< SRSS PWR_BUCK_CTL2: BUCK_OUT2_SEL (Bit 0)                 */
#define SRSS_PWR_BUCK_CTL2_BUCK_OUT2_SEL_Msk (0x7UL)            /*!< SRSS PWR_BUCK_CTL2: BUCK_OUT2_SEL (Bitfield-Mask: 0x07)   */
#define SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL_Pos (30UL)          /*!< SRSS PWR_BUCK_CTL2: BUCK_OUT2_HW_SEL (Bit 30)             */
#define SRSS_PWR_BUCK_CTL2_BUCK_OUT2_HW_SEL_Msk (0x40000000UL)  /*!< SRSS PWR_BUCK_CTL2: BUCK_OUT2_HW_SEL (Bitfield-Mask: 0x01)*/
#define SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN_Pos (31UL)              /*!< SRSS PWR_BUCK_CTL2: BUCK_OUT2_EN (Bit 31)                 */
#define SRSS_PWR_BUCK_CTL2_BUCK_OUT2_EN_Msk (0x80000000UL)      /*!< SRSS PWR_BUCK_CTL2: BUCK_OUT2_EN (Bitfield-Mask: 0x01)    */
/* ==================================================  SRSS.PWR_LVD_STATUS  ================================================== */
#define SRSS_PWR_LVD_STATUS_HVLVD1_OK_Pos (0UL)                 /*!< SRSS PWR_LVD_STATUS: HVLVD1_OK (Bit 0)                    */
#define SRSS_PWR_LVD_STATUS_HVLVD1_OK_Msk (0x1UL)               /*!< SRSS PWR_LVD_STATUS: HVLVD1_OK (Bitfield-Mask: 0x01)      */
/* ===================================================  SRSS.PWR_HIB_DATA  =================================================== */
#define SRSS_PWR_HIB_DATA_HIB_DATA_Pos    (0UL)                 /*!< SRSS PWR_HIB_DATA: HIB_DATA (Bit 0)                       */
#define SRSS_PWR_HIB_DATA_HIB_DATA_Msk    (0xffffffffUL)        /*!< SRSS PWR_HIB_DATA: HIB_DATA (Bitfield-Mask: 0xffffffff)   */
/* =====================================================  SRSS.WDT_CTL  ====================================================== */
#define SRSS_WDT_CTL_WDT_EN_Pos           (0UL)                 /*!< SRSS WDT_CTL: WDT_EN (Bit 0)                              */
#define SRSS_WDT_CTL_WDT_EN_Msk           (0x1UL)               /*!< SRSS WDT_CTL: WDT_EN (Bitfield-Mask: 0x01)                */
#define SRSS_WDT_CTL_WDT_LOCK_Pos         (30UL)                /*!< SRSS WDT_CTL: WDT_LOCK (Bit 30)                           */
#define SRSS_WDT_CTL_WDT_LOCK_Msk         (0xc0000000UL)        /*!< SRSS WDT_CTL: WDT_LOCK (Bitfield-Mask: 0x03)              */
/* =====================================================  SRSS.WDT_CNT  ====================================================== */
#define SRSS_WDT_CNT_COUNTER_Pos          (0UL)                 /*!< SRSS WDT_CNT: COUNTER (Bit 0)                             */
#define SRSS_WDT_CNT_COUNTER_Msk          (0xffffUL)            /*!< SRSS WDT_CNT: COUNTER (Bitfield-Mask: 0xffff)             */
/* ====================================================  SRSS.WDT_MATCH  ===================================================== */
#define SRSS_WDT_MATCH_MATCH_Pos          (0UL)                 /*!< SRSS WDT_MATCH: MATCH (Bit 0)                             */
#define SRSS_WDT_MATCH_MATCH_Msk          (0xffffUL)            /*!< SRSS WDT_MATCH: MATCH (Bitfield-Mask: 0xffff)             */
#define SRSS_WDT_MATCH_IGNORE_BITS_Pos    (16UL)                /*!< SRSS WDT_MATCH: IGNORE_BITS (Bit 16)                      */
#define SRSS_WDT_MATCH_IGNORE_BITS_Msk    (0xf0000UL)           /*!< SRSS WDT_MATCH: IGNORE_BITS (Bitfield-Mask: 0x0f)         */
/* ==================================================  SRSS.CLK_DSI_SELECT  ================================================== */
#define SRSS_CLK_DSI_SELECT_DSI_MUX_Pos   (0UL)                 /*!< SRSS CLK_DSI_SELECT: DSI_MUX (Bit 0)                      */
#define SRSS_CLK_DSI_SELECT_DSI_MUX_Msk   (0x1fUL)              /*!< SRSS CLK_DSI_SELECT: DSI_MUX (Bitfield-Mask: 0x1f)        */
/* =================================================  SRSS.CLK_PATH_SELECT  ================================================== */
#define SRSS_CLK_PATH_SELECT_PATH_MUX_Pos (0UL)                 /*!< SRSS CLK_PATH_SELECT: PATH_MUX (Bit 0)                    */
#define SRSS_CLK_PATH_SELECT_PATH_MUX_Msk (0x7UL)               /*!< SRSS CLK_PATH_SELECT: PATH_MUX (Bitfield-Mask: 0x07)      */
/* =================================================  SRSS.CLK_ROOT_SELECT  ================================================== */
#define SRSS_CLK_ROOT_SELECT_ROOT_MUX_Pos (0UL)                 /*!< SRSS CLK_ROOT_SELECT: ROOT_MUX (Bit 0)                    */
#define SRSS_CLK_ROOT_SELECT_ROOT_MUX_Msk (0xfUL)               /*!< SRSS CLK_ROOT_SELECT: ROOT_MUX (Bitfield-Mask: 0x0f)      */
#define SRSS_CLK_ROOT_SELECT_ROOT_DIV_Pos (4UL)                 /*!< SRSS CLK_ROOT_SELECT: ROOT_DIV (Bit 4)                    */
#define SRSS_CLK_ROOT_SELECT_ROOT_DIV_Msk (0x30UL)              /*!< SRSS CLK_ROOT_SELECT: ROOT_DIV (Bitfield-Mask: 0x03)      */
#define SRSS_CLK_ROOT_SELECT_ENABLE_Pos   (31UL)                /*!< SRSS CLK_ROOT_SELECT: ENABLE (Bit 31)                     */
#define SRSS_CLK_ROOT_SELECT_ENABLE_Msk   (0x80000000UL)        /*!< SRSS CLK_ROOT_SELECT: ENABLE (Bitfield-Mask: 0x01)        */
/* ====================================================  SRSS.CLK_SELECT  ==================================================== */
#define SRSS_CLK_SELECT_LFCLK_SEL_Pos     (0UL)                 /*!< SRSS CLK_SELECT: LFCLK_SEL (Bit 0)                        */
#define SRSS_CLK_SELECT_LFCLK_SEL_Msk     (0x3UL)               /*!< SRSS CLK_SELECT: LFCLK_SEL (Bitfield-Mask: 0x03)          */
#define SRSS_CLK_SELECT_PUMP_SEL_Pos      (8UL)                 /*!< SRSS CLK_SELECT: PUMP_SEL (Bit 8)                         */
#define SRSS_CLK_SELECT_PUMP_SEL_Msk      (0xf00UL)             /*!< SRSS CLK_SELECT: PUMP_SEL (Bitfield-Mask: 0x0f)           */
#define SRSS_CLK_SELECT_PUMP_DIV_Pos      (12UL)                /*!< SRSS CLK_SELECT: PUMP_DIV (Bit 12)                        */
#define SRSS_CLK_SELECT_PUMP_DIV_Msk      (0x7000UL)            /*!< SRSS CLK_SELECT: PUMP_DIV (Bitfield-Mask: 0x07)           */
#define SRSS_CLK_SELECT_PUMP_ENABLE_Pos   (15UL)                /*!< SRSS CLK_SELECT: PUMP_ENABLE (Bit 15)                     */
#define SRSS_CLK_SELECT_PUMP_ENABLE_Msk   (0x8000UL)            /*!< SRSS CLK_SELECT: PUMP_ENABLE (Bitfield-Mask: 0x01)        */
/* ==================================================  SRSS.CLK_TIMER_CTL  =================================================== */
#define SRSS_CLK_TIMER_CTL_TIMER_SEL_Pos  (0UL)                 /*!< SRSS CLK_TIMER_CTL: TIMER_SEL (Bit 0)                     */
#define SRSS_CLK_TIMER_CTL_TIMER_SEL_Msk  (0x1UL)               /*!< SRSS CLK_TIMER_CTL: TIMER_SEL (Bitfield-Mask: 0x01)       */
#define SRSS_CLK_TIMER_CTL_TIMER_HF0_DIV_Pos (8UL)              /*!< SRSS CLK_TIMER_CTL: TIMER_HF0_DIV (Bit 8)                 */
#define SRSS_CLK_TIMER_CTL_TIMER_HF0_DIV_Msk (0x300UL)          /*!< SRSS CLK_TIMER_CTL: TIMER_HF0_DIV (Bitfield-Mask: 0x03)   */
#define SRSS_CLK_TIMER_CTL_TIMER_DIV_Pos  (16UL)                /*!< SRSS CLK_TIMER_CTL: TIMER_DIV (Bit 16)                    */
#define SRSS_CLK_TIMER_CTL_TIMER_DIV_Msk  (0xff0000UL)          /*!< SRSS CLK_TIMER_CTL: TIMER_DIV (Bitfield-Mask: 0xff)       */
#define SRSS_CLK_TIMER_CTL_ENABLE_Pos     (31UL)                /*!< SRSS CLK_TIMER_CTL: ENABLE (Bit 31)                       */
#define SRSS_CLK_TIMER_CTL_ENABLE_Msk     (0x80000000UL)        /*!< SRSS CLK_TIMER_CTL: ENABLE (Bitfield-Mask: 0x01)          */
/* =================================================  SRSS.CLK_CSV_WCO_CTL  ================================================== */
#define SRSS_CLK_CSV_WCO_CTL_CSV_MUX_Pos  (16UL)                /*!< SRSS CLK_CSV_WCO_CTL: CSV_MUX (Bit 16)                    */
#define SRSS_CLK_CSV_WCO_CTL_CSV_MUX_Msk  (0x30000UL)           /*!< SRSS CLK_CSV_WCO_CTL: CSV_MUX (Bitfield-Mask: 0x03)       */
#define SRSS_CLK_CSV_WCO_CTL_CSV_LOSS_WINDOW_Pos (20UL)         /*!< SRSS CLK_CSV_WCO_CTL: CSV_LOSS_WINDOW (Bit 20)            */
#define SRSS_CLK_CSV_WCO_CTL_CSV_LOSS_WINDOW_Msk (0x700000UL)   /*!< SRSS CLK_CSV_WCO_CTL: CSV_LOSS_WINDOW (Bitfield-Mask: 0x07)*/
#define SRSS_CLK_CSV_WCO_CTL_CSV_LOSS_ACTION_Pos (28UL)         /*!< SRSS CLK_CSV_WCO_CTL: CSV_LOSS_ACTION (Bit 28)            */
#define SRSS_CLK_CSV_WCO_CTL_CSV_LOSS_ACTION_Msk (0x30000000UL) /*!< SRSS CLK_CSV_WCO_CTL: CSV_LOSS_ACTION (Bitfield-Mask: 0x03)*/
#define SRSS_CLK_CSV_WCO_CTL_CSV_LOSS_EN_Pos (31UL)             /*!< SRSS CLK_CSV_WCO_CTL: CSV_LOSS_EN (Bit 31)                */
#define SRSS_CLK_CSV_WCO_CTL_CSV_LOSS_EN_Msk (0x80000000UL)     /*!< SRSS CLK_CSV_WCO_CTL: CSV_LOSS_EN (Bitfield-Mask: 0x01)   */
/* ==================================================  SRSS.CLK_ILO_CONFIG  ================================================== */
#define SRSS_CLK_ILO_CONFIG_HVILO_BACKUP_Pos (0UL)              /*!< SRSS CLK_ILO_CONFIG: HVILO_BACKUP (Bit 0)                 */
#define SRSS_CLK_ILO_CONFIG_HVILO_BACKUP_Msk (0x1UL)            /*!< SRSS CLK_ILO_CONFIG: HVILO_BACKUP (Bitfield-Mask: 0x01)   */
#define SRSS_CLK_ILO_CONFIG_ENABLE_Pos    (31UL)                /*!< SRSS CLK_ILO_CONFIG: ENABLE (Bit 31)                      */
#define SRSS_CLK_ILO_CONFIG_ENABLE_Msk    (0x80000000UL)        /*!< SRSS CLK_ILO_CONFIG: ENABLE (Bitfield-Mask: 0x01)         */
/* ==================================================  SRSS.CLK_IMO_CONFIG  ================================================== */
#define SRSS_CLK_IMO_CONFIG_ENABLE_Pos    (31UL)                /*!< SRSS CLK_IMO_CONFIG: ENABLE (Bit 31)                      */
#define SRSS_CLK_IMO_CONFIG_ENABLE_Msk    (0x80000000UL)        /*!< SRSS CLK_IMO_CONFIG: ENABLE (Bitfield-Mask: 0x01)         */
/* =================================================  SRSS.CLK_OUTPUT_FAST  ================================================== */
#define SRSS_CLK_OUTPUT_FAST_FAST_SEL0_Pos (0UL)                /*!< SRSS CLK_OUTPUT_FAST: FAST_SEL0 (Bit 0)                   */
#define SRSS_CLK_OUTPUT_FAST_FAST_SEL0_Msk (0xfUL)              /*!< SRSS CLK_OUTPUT_FAST: FAST_SEL0 (Bitfield-Mask: 0x0f)     */
#define SRSS_CLK_OUTPUT_FAST_PATH_SEL0_Pos (4UL)                /*!< SRSS CLK_OUTPUT_FAST: PATH_SEL0 (Bit 4)                   */
#define SRSS_CLK_OUTPUT_FAST_PATH_SEL0_Msk (0xf0UL)             /*!< SRSS CLK_OUTPUT_FAST: PATH_SEL0 (Bitfield-Mask: 0x0f)     */
#define SRSS_CLK_OUTPUT_FAST_HFCLK_SEL0_Pos (8UL)               /*!< SRSS CLK_OUTPUT_FAST: HFCLK_SEL0 (Bit 8)                  */
#define SRSS_CLK_OUTPUT_FAST_HFCLK_SEL0_Msk (0xf00UL)           /*!< SRSS CLK_OUTPUT_FAST: HFCLK_SEL0 (Bitfield-Mask: 0x0f)    */
#define SRSS_CLK_OUTPUT_FAST_FAST_SEL1_Pos (16UL)               /*!< SRSS CLK_OUTPUT_FAST: FAST_SEL1 (Bit 16)                  */
#define SRSS_CLK_OUTPUT_FAST_FAST_SEL1_Msk (0xf0000UL)          /*!< SRSS CLK_OUTPUT_FAST: FAST_SEL1 (Bitfield-Mask: 0x0f)     */
#define SRSS_CLK_OUTPUT_FAST_PATH_SEL1_Pos (20UL)               /*!< SRSS CLK_OUTPUT_FAST: PATH_SEL1 (Bit 20)                  */
#define SRSS_CLK_OUTPUT_FAST_PATH_SEL1_Msk (0xf00000UL)         /*!< SRSS CLK_OUTPUT_FAST: PATH_SEL1 (Bitfield-Mask: 0x0f)     */
#define SRSS_CLK_OUTPUT_FAST_HFCLK_SEL1_Pos (24UL)              /*!< SRSS CLK_OUTPUT_FAST: HFCLK_SEL1 (Bit 24)                 */
#define SRSS_CLK_OUTPUT_FAST_HFCLK_SEL1_Msk (0xf000000UL)       /*!< SRSS CLK_OUTPUT_FAST: HFCLK_SEL1 (Bitfield-Mask: 0x0f)    */
/* =================================================  SRSS.CLK_OUTPUT_SLOW  ================================================== */
#define SRSS_CLK_OUTPUT_SLOW_SLOW_SEL0_Pos (0UL)                /*!< SRSS CLK_OUTPUT_SLOW: SLOW_SEL0 (Bit 0)                   */
#define SRSS_CLK_OUTPUT_SLOW_SLOW_SEL0_Msk (0xfUL)              /*!< SRSS CLK_OUTPUT_SLOW: SLOW_SEL0 (Bitfield-Mask: 0x0f)     */
#define SRSS_CLK_OUTPUT_SLOW_SLOW_SEL1_Pos (4UL)                /*!< SRSS CLK_OUTPUT_SLOW: SLOW_SEL1 (Bit 4)                   */
#define SRSS_CLK_OUTPUT_SLOW_SLOW_SEL1_Msk (0xf0UL)             /*!< SRSS CLK_OUTPUT_SLOW: SLOW_SEL1 (Bitfield-Mask: 0x0f)     */
/* ===================================================  SRSS.CLK_CAL_CNT1  =================================================== */
#define SRSS_CLK_CAL_CNT1_CAL_COUNTER1_Pos (0UL)                /*!< SRSS CLK_CAL_CNT1: CAL_COUNTER1 (Bit 0)                   */
#define SRSS_CLK_CAL_CNT1_CAL_COUNTER1_Msk (0xffffffUL)         /*!< SRSS CLK_CAL_CNT1: CAL_COUNTER1 (Bitfield-Mask: 0xffffff) */
#define SRSS_CLK_CAL_CNT1_CAL_COUNTER_DONE_Pos (31UL)           /*!< SRSS CLK_CAL_CNT1: CAL_COUNTER_DONE (Bit 31)              */
#define SRSS_CLK_CAL_CNT1_CAL_COUNTER_DONE_Msk (0x80000000UL)   /*!< SRSS CLK_CAL_CNT1: CAL_COUNTER_DONE (Bitfield-Mask: 0x01) */
/* ===================================================  SRSS.CLK_CAL_CNT2  =================================================== */
#define SRSS_CLK_CAL_CNT2_CAL_COUNTER2_Pos (0UL)                /*!< SRSS CLK_CAL_CNT2: CAL_COUNTER2 (Bit 0)                   */
#define SRSS_CLK_CAL_CNT2_CAL_COUNTER2_Msk (0xffffffUL)         /*!< SRSS CLK_CAL_CNT2: CAL_COUNTER2 (Bitfield-Mask: 0xffffff) */
/* ==================================================  SRSS.CLK_ECO_CONFIG  ================================================== */
#define SRSS_CLK_ECO_CONFIG_AGC_EN_Pos    (1UL)                 /*!< SRSS CLK_ECO_CONFIG: AGC_EN (Bit 1)                       */
#define SRSS_CLK_ECO_CONFIG_AGC_EN_Msk    (0x2UL)               /*!< SRSS CLK_ECO_CONFIG: AGC_EN (Bitfield-Mask: 0x01)         */
#define SRSS_CLK_ECO_CONFIG_ECO_EN_Pos    (31UL)                /*!< SRSS CLK_ECO_CONFIG: ECO_EN (Bit 31)                      */
#define SRSS_CLK_ECO_CONFIG_ECO_EN_Msk    (0x80000000UL)        /*!< SRSS CLK_ECO_CONFIG: ECO_EN (Bitfield-Mask: 0x01)         */
/* ==================================================  SRSS.CLK_ECO_STATUS  ================================================== */
#define SRSS_CLK_ECO_STATUS_ECO_OK_Pos    (0UL)                 /*!< SRSS CLK_ECO_STATUS: ECO_OK (Bit 0)                       */
#define SRSS_CLK_ECO_STATUS_ECO_OK_Msk    (0x1UL)               /*!< SRSS CLK_ECO_STATUS: ECO_OK (Bitfield-Mask: 0x01)         */
#define SRSS_CLK_ECO_STATUS_ECO_READY_Pos (1UL)                 /*!< SRSS CLK_ECO_STATUS: ECO_READY (Bit 1)                    */
#define SRSS_CLK_ECO_STATUS_ECO_READY_Msk (0x2UL)               /*!< SRSS CLK_ECO_STATUS: ECO_READY (Bitfield-Mask: 0x01)      */
/* ===================================================  SRSS.CLK_WCO_DPLL  =================================================== */
#define SRSS_CLK_WCO_DPLL_DPLL_MULT_Pos   (0UL)                 /*!< SRSS CLK_WCO_DPLL: DPLL_MULT (Bit 0)                      */
#define SRSS_CLK_WCO_DPLL_DPLL_MULT_Msk   (0x7ffUL)             /*!< SRSS CLK_WCO_DPLL: DPLL_MULT (Bitfield-Mask: 0x7ff)       */
#define SRSS_CLK_WCO_DPLL_DPLL_LF_IGAIN_Pos (16UL)              /*!< SRSS CLK_WCO_DPLL: DPLL_LF_IGAIN (Bit 16)                 */
#define SRSS_CLK_WCO_DPLL_DPLL_LF_IGAIN_Msk (0x70000UL)         /*!< SRSS CLK_WCO_DPLL: DPLL_LF_IGAIN (Bitfield-Mask: 0x07)    */
#define SRSS_CLK_WCO_DPLL_DPLL_LF_PGAIN_Pos (19UL)              /*!< SRSS CLK_WCO_DPLL: DPLL_LF_PGAIN (Bit 19)                 */
#define SRSS_CLK_WCO_DPLL_DPLL_LF_PGAIN_Msk (0x380000UL)        /*!< SRSS CLK_WCO_DPLL: DPLL_LF_PGAIN (Bitfield-Mask: 0x07)    */
#define SRSS_CLK_WCO_DPLL_DPLL_LF_LIMIT_Pos (22UL)              /*!< SRSS CLK_WCO_DPLL: DPLL_LF_LIMIT (Bit 22)                 */
#define SRSS_CLK_WCO_DPLL_DPLL_LF_LIMIT_Msk (0x3fc00000UL)      /*!< SRSS CLK_WCO_DPLL: DPLL_LF_LIMIT (Bitfield-Mask: 0xff)    */
#define SRSS_CLK_WCO_DPLL_DPLL_EN_Pos     (31UL)                /*!< SRSS CLK_WCO_DPLL: DPLL_EN (Bit 31)                       */
#define SRSS_CLK_WCO_DPLL_DPLL_EN_Msk     (0x80000000UL)        /*!< SRSS CLK_WCO_DPLL: DPLL_EN (Bitfield-Mask: 0x01)          */
/* =================================================  SRSS.CLK_PILO_CONFIG  ================================================== */
#define SRSS_CLK_PILO_CONFIG_PILO_FFREQ_Pos (0UL)               /*!< SRSS CLK_PILO_CONFIG: PILO_FFREQ (Bit 0)                  */
#define SRSS_CLK_PILO_CONFIG_PILO_FFREQ_Msk (0x3ffUL)           /*!< SRSS CLK_PILO_CONFIG: PILO_FFREQ (Bitfield-Mask: 0x3ff)   */
#define SRSS_CLK_PILO_CONFIG_PILO_CLK_EN_Pos (29UL)             /*!< SRSS CLK_PILO_CONFIG: PILO_CLK_EN (Bit 29)                */
#define SRSS_CLK_PILO_CONFIG_PILO_CLK_EN_Msk (0x20000000UL)     /*!< SRSS CLK_PILO_CONFIG: PILO_CLK_EN (Bitfield-Mask: 0x01)   */
#define SRSS_CLK_PILO_CONFIG_PILO_RESET_N_Pos (30UL)            /*!< SRSS CLK_PILO_CONFIG: PILO_RESET_N (Bit 30)               */
#define SRSS_CLK_PILO_CONFIG_PILO_RESET_N_Msk (0x40000000UL)    /*!< SRSS CLK_PILO_CONFIG: PILO_RESET_N (Bitfield-Mask: 0x01)  */
#define SRSS_CLK_PILO_CONFIG_PILO_EN_Pos  (31UL)                /*!< SRSS CLK_PILO_CONFIG: PILO_EN (Bit 31)                    */
#define SRSS_CLK_PILO_CONFIG_PILO_EN_Msk  (0x80000000UL)        /*!< SRSS CLK_PILO_CONFIG: PILO_EN (Bitfield-Mask: 0x01)       */
/* ==================================================  SRSS.CLK_FLL_CONFIG  ================================================== */
#define SRSS_CLK_FLL_CONFIG_FLL_MULT_Pos  (0UL)                 /*!< SRSS CLK_FLL_CONFIG: FLL_MULT (Bit 0)                     */
#define SRSS_CLK_FLL_CONFIG_FLL_MULT_Msk  (0x3ffffUL)           /*!< SRSS CLK_FLL_CONFIG: FLL_MULT (Bitfield-Mask: 0x3ffff)    */
#define SRSS_CLK_FLL_CONFIG_FLL_OUTPUT_DIV_Pos (24UL)           /*!< SRSS CLK_FLL_CONFIG: FLL_OUTPUT_DIV (Bit 24)              */
#define SRSS_CLK_FLL_CONFIG_FLL_OUTPUT_DIV_Msk (0x1000000UL)    /*!< SRSS CLK_FLL_CONFIG: FLL_OUTPUT_DIV (Bitfield-Mask: 0x01) */
#define SRSS_CLK_FLL_CONFIG_FLL_ENABLE_Pos (31UL)               /*!< SRSS CLK_FLL_CONFIG: FLL_ENABLE (Bit 31)                  */
#define SRSS_CLK_FLL_CONFIG_FLL_ENABLE_Msk (0x80000000UL)       /*!< SRSS CLK_FLL_CONFIG: FLL_ENABLE (Bitfield-Mask: 0x01)     */
/* =================================================  SRSS.CLK_FLL_CONFIG2  ================================================== */
#define SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV_Pos (0UL)              /*!< SRSS CLK_FLL_CONFIG2: FLL_REF_DIV (Bit 0)                 */
#define SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV_Msk (0x1fffUL)         /*!< SRSS CLK_FLL_CONFIG2: FLL_REF_DIV (Bitfield-Mask: 0x1fff) */
#define SRSS_CLK_FLL_CONFIG2_LOCK_TOL_Pos (16UL)                /*!< SRSS CLK_FLL_CONFIG2: LOCK_TOL (Bit 16)                   */
#define SRSS_CLK_FLL_CONFIG2_LOCK_TOL_Msk (0x1ff0000UL)         /*!< SRSS CLK_FLL_CONFIG2: LOCK_TOL (Bitfield-Mask: 0x1ff)     */
/* =================================================  SRSS.CLK_FLL_CONFIG3  ================================================== */
#define SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN_Pos (0UL)             /*!< SRSS CLK_FLL_CONFIG3: FLL_LF_IGAIN (Bit 0)                */
#define SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN_Msk (0xfUL)           /*!< SRSS CLK_FLL_CONFIG3: FLL_LF_IGAIN (Bitfield-Mask: 0x0f)  */
#define SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN_Pos (4UL)             /*!< SRSS CLK_FLL_CONFIG3: FLL_LF_PGAIN (Bit 4)                */
#define SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN_Msk (0xf0UL)          /*!< SRSS CLK_FLL_CONFIG3: FLL_LF_PGAIN (Bitfield-Mask: 0x0f)  */
#define SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT_Pos (8UL)           /*!< SRSS CLK_FLL_CONFIG3: SETTLING_COUNT (Bit 8)              */
#define SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT_Msk (0x1fff00UL)    /*!< SRSS CLK_FLL_CONFIG3: SETTLING_COUNT (Bitfield-Mask: 0x1fff)*/
#define SRSS_CLK_FLL_CONFIG3_BYPASS_SEL_Pos (28UL)              /*!< SRSS CLK_FLL_CONFIG3: BYPASS_SEL (Bit 28)                 */
#define SRSS_CLK_FLL_CONFIG3_BYPASS_SEL_Msk (0x30000000UL)      /*!< SRSS CLK_FLL_CONFIG3: BYPASS_SEL (Bitfield-Mask: 0x03)    */
/* =================================================  SRSS.CLK_FLL_CONFIG4  ================================================== */
#define SRSS_CLK_FLL_CONFIG4_CCO_LIMIT_Pos (0UL)                /*!< SRSS CLK_FLL_CONFIG4: CCO_LIMIT (Bit 0)                   */
#define SRSS_CLK_FLL_CONFIG4_CCO_LIMIT_Msk (0xffUL)             /*!< SRSS CLK_FLL_CONFIG4: CCO_LIMIT (Bitfield-Mask: 0xff)     */
#define SRSS_CLK_FLL_CONFIG4_CCO_RANGE_Pos (8UL)                /*!< SRSS CLK_FLL_CONFIG4: CCO_RANGE (Bit 8)                   */
#define SRSS_CLK_FLL_CONFIG4_CCO_RANGE_Msk (0x700UL)            /*!< SRSS CLK_FLL_CONFIG4: CCO_RANGE (Bitfield-Mask: 0x07)     */
#define SRSS_CLK_FLL_CONFIG4_CCO_FREQ_Pos (16UL)                /*!< SRSS CLK_FLL_CONFIG4: CCO_FREQ (Bit 16)                   */
#define SRSS_CLK_FLL_CONFIG4_CCO_FREQ_Msk (0x1ff0000UL)         /*!< SRSS CLK_FLL_CONFIG4: CCO_FREQ (Bitfield-Mask: 0x1ff)     */
#define SRSS_CLK_FLL_CONFIG4_CCO_ENABLE_Pos (31UL)              /*!< SRSS CLK_FLL_CONFIG4: CCO_ENABLE (Bit 31)                 */
#define SRSS_CLK_FLL_CONFIG4_CCO_ENABLE_Msk (0x80000000UL)      /*!< SRSS CLK_FLL_CONFIG4: CCO_ENABLE (Bitfield-Mask: 0x01)    */
/* ==================================================  SRSS.CLK_FLL_STATUS  ================================================== */
#define SRSS_CLK_FLL_STATUS_LOCKED_Pos    (0UL)                 /*!< SRSS CLK_FLL_STATUS: LOCKED (Bit 0)                       */
#define SRSS_CLK_FLL_STATUS_LOCKED_Msk    (0x1UL)               /*!< SRSS CLK_FLL_STATUS: LOCKED (Bitfield-Mask: 0x01)         */
#define SRSS_CLK_FLL_STATUS_UNLOCK_OCCURRED_Pos (1UL)           /*!< SRSS CLK_FLL_STATUS: UNLOCK_OCCURRED (Bit 1)              */
#define SRSS_CLK_FLL_STATUS_UNLOCK_OCCURRED_Msk (0x2UL)         /*!< SRSS CLK_FLL_STATUS: UNLOCK_OCCURRED (Bitfield-Mask: 0x01)*/
#define SRSS_CLK_FLL_STATUS_CCO_READY_Pos (2UL)                 /*!< SRSS CLK_FLL_STATUS: CCO_READY (Bit 2)                    */
#define SRSS_CLK_FLL_STATUS_CCO_READY_Msk (0x4UL)               /*!< SRSS CLK_FLL_STATUS: CCO_READY (Bitfield-Mask: 0x01)      */
/* ==================================================  SRSS.CLK_PLL_CONFIG  ================================================== */
#define SRSS_CLK_PLL_CONFIG_FEEDBACK_DIV_Pos (0UL)              /*!< SRSS CLK_PLL_CONFIG: FEEDBACK_DIV (Bit 0)                 */
#define SRSS_CLK_PLL_CONFIG_FEEDBACK_DIV_Msk (0x7fUL)           /*!< SRSS CLK_PLL_CONFIG: FEEDBACK_DIV (Bitfield-Mask: 0x7f)   */
#define SRSS_CLK_PLL_CONFIG_REFERENCE_DIV_Pos (8UL)             /*!< SRSS CLK_PLL_CONFIG: REFERENCE_DIV (Bit 8)                */
#define SRSS_CLK_PLL_CONFIG_REFERENCE_DIV_Msk (0x1f00UL)        /*!< SRSS CLK_PLL_CONFIG: REFERENCE_DIV (Bitfield-Mask: 0x1f)  */
#define SRSS_CLK_PLL_CONFIG_OUTPUT_DIV_Pos (16UL)               /*!< SRSS CLK_PLL_CONFIG: OUTPUT_DIV (Bit 16)                  */
#define SRSS_CLK_PLL_CONFIG_OUTPUT_DIV_Msk (0x1f0000UL)         /*!< SRSS CLK_PLL_CONFIG: OUTPUT_DIV (Bitfield-Mask: 0x1f)     */
#define SRSS_CLK_PLL_CONFIG_PLL_LF_MODE_Pos (27UL)              /*!< SRSS CLK_PLL_CONFIG: PLL_LF_MODE (Bit 27)                 */
#define SRSS_CLK_PLL_CONFIG_PLL_LF_MODE_Msk (0x8000000UL)       /*!< SRSS CLK_PLL_CONFIG: PLL_LF_MODE (Bitfield-Mask: 0x01)    */
#define SRSS_CLK_PLL_CONFIG_BYPASS_SEL_Pos (28UL)               /*!< SRSS CLK_PLL_CONFIG: BYPASS_SEL (Bit 28)                  */
#define SRSS_CLK_PLL_CONFIG_BYPASS_SEL_Msk (0x30000000UL)       /*!< SRSS CLK_PLL_CONFIG: BYPASS_SEL (Bitfield-Mask: 0x03)     */
#define SRSS_CLK_PLL_CONFIG_ENABLE_Pos    (31UL)                /*!< SRSS CLK_PLL_CONFIG: ENABLE (Bit 31)                      */
#define SRSS_CLK_PLL_CONFIG_ENABLE_Msk    (0x80000000UL)        /*!< SRSS CLK_PLL_CONFIG: ENABLE (Bitfield-Mask: 0x01)         */
/* ==================================================  SRSS.CLK_PLL_STATUS  ================================================== */
#define SRSS_CLK_PLL_STATUS_LOCKED_Pos    (0UL)                 /*!< SRSS CLK_PLL_STATUS: LOCKED (Bit 0)                       */
#define SRSS_CLK_PLL_STATUS_LOCKED_Msk    (0x1UL)               /*!< SRSS CLK_PLL_STATUS: LOCKED (Bitfield-Mask: 0x01)         */
#define SRSS_CLK_PLL_STATUS_UNLOCK_OCCURRED_Pos (1UL)           /*!< SRSS CLK_PLL_STATUS: UNLOCK_OCCURRED (Bit 1)              */
#define SRSS_CLK_PLL_STATUS_UNLOCK_OCCURRED_Msk (0x2UL)         /*!< SRSS CLK_PLL_STATUS: UNLOCK_OCCURRED (Bitfield-Mask: 0x01)*/
/* ====================================================  SRSS.SRSS_INTR  ===================================================== */
#define SRSS_SRSS_INTR_WDT_MATCH_Pos      (0UL)                 /*!< SRSS SRSS_INTR: WDT_MATCH (Bit 0)                         */
#define SRSS_SRSS_INTR_WDT_MATCH_Msk      (0x1UL)               /*!< SRSS SRSS_INTR: WDT_MATCH (Bitfield-Mask: 0x01)           */
#define SRSS_SRSS_INTR_HVLVD1_Pos         (1UL)                 /*!< SRSS SRSS_INTR: HVLVD1 (Bit 1)                            */
#define SRSS_SRSS_INTR_HVLVD1_Msk         (0x2UL)               /*!< SRSS SRSS_INTR: HVLVD1 (Bitfield-Mask: 0x01)              */
#define SRSS_SRSS_INTR_CLK_CAL_Pos        (5UL)                 /*!< SRSS SRSS_INTR: CLK_CAL (Bit 5)                           */
#define SRSS_SRSS_INTR_CLK_CAL_Msk        (0x20UL)              /*!< SRSS SRSS_INTR: CLK_CAL (Bitfield-Mask: 0x01)             */
/* ==================================================  SRSS.SRSS_INTR_SET  =================================================== */
#define SRSS_SRSS_INTR_SET_WDT_MATCH_Pos  (0UL)                 /*!< SRSS SRSS_INTR_SET: WDT_MATCH (Bit 0)                     */
#define SRSS_SRSS_INTR_SET_WDT_MATCH_Msk  (0x1UL)               /*!< SRSS SRSS_INTR_SET: WDT_MATCH (Bitfield-Mask: 0x01)       */
#define SRSS_SRSS_INTR_SET_HVLVD1_Pos     (1UL)                 /*!< SRSS SRSS_INTR_SET: HVLVD1 (Bit 1)                        */
#define SRSS_SRSS_INTR_SET_HVLVD1_Msk     (0x2UL)               /*!< SRSS SRSS_INTR_SET: HVLVD1 (Bitfield-Mask: 0x01)          */
#define SRSS_SRSS_INTR_SET_CLK_CAL_Pos    (5UL)                 /*!< SRSS SRSS_INTR_SET: CLK_CAL (Bit 5)                       */
#define SRSS_SRSS_INTR_SET_CLK_CAL_Msk    (0x20UL)              /*!< SRSS SRSS_INTR_SET: CLK_CAL (Bitfield-Mask: 0x01)         */
/* ==================================================  SRSS.SRSS_INTR_MASK  ================================================== */
#define SRSS_SRSS_INTR_MASK_WDT_MATCH_Pos (0UL)                 /*!< SRSS SRSS_INTR_MASK: WDT_MATCH (Bit 0)                    */
#define SRSS_SRSS_INTR_MASK_WDT_MATCH_Msk (0x1UL)               /*!< SRSS SRSS_INTR_MASK: WDT_MATCH (Bitfield-Mask: 0x01)      */
#define SRSS_SRSS_INTR_MASK_HVLVD1_Pos    (1UL)                 /*!< SRSS SRSS_INTR_MASK: HVLVD1 (Bit 1)                       */
#define SRSS_SRSS_INTR_MASK_HVLVD1_Msk    (0x2UL)               /*!< SRSS SRSS_INTR_MASK: HVLVD1 (Bitfield-Mask: 0x01)         */
#define SRSS_SRSS_INTR_MASK_CLK_CAL_Pos   (5UL)                 /*!< SRSS SRSS_INTR_MASK: CLK_CAL (Bit 5)                      */
#define SRSS_SRSS_INTR_MASK_CLK_CAL_Msk   (0x20UL)              /*!< SRSS SRSS_INTR_MASK: CLK_CAL (Bitfield-Mask: 0x01)        */
/* =================================================  SRSS.SRSS_INTR_MASKED  ================================================= */
#define SRSS_SRSS_INTR_MASKED_WDT_MATCH_Pos (0UL)               /*!< SRSS SRSS_INTR_MASKED: WDT_MATCH (Bit 0)                  */
#define SRSS_SRSS_INTR_MASKED_WDT_MATCH_Msk (0x1UL)             /*!< SRSS SRSS_INTR_MASKED: WDT_MATCH (Bitfield-Mask: 0x01)    */
#define SRSS_SRSS_INTR_MASKED_HVLVD1_Pos  (1UL)                 /*!< SRSS SRSS_INTR_MASKED: HVLVD1 (Bit 1)                     */
#define SRSS_SRSS_INTR_MASKED_HVLVD1_Msk  (0x2UL)               /*!< SRSS SRSS_INTR_MASKED: HVLVD1 (Bitfield-Mask: 0x01)       */
#define SRSS_SRSS_INTR_MASKED_CLK_CAL_Pos (5UL)                 /*!< SRSS SRSS_INTR_MASKED: CLK_CAL (Bit 5)                    */
#define SRSS_SRSS_INTR_MASKED_CLK_CAL_Msk (0x20UL)              /*!< SRSS SRSS_INTR_MASKED: CLK_CAL (Bitfield-Mask: 0x01)      */
/* ==================================================  SRSS.SRSS_INTR_CFG  =================================================== */
#define SRSS_SRSS_INTR_CFG_HVLVD1_EDGE_SEL_Pos (0UL)            /*!< SRSS SRSS_INTR_CFG: HVLVD1_EDGE_SEL (Bit 0)               */
#define SRSS_SRSS_INTR_CFG_HVLVD1_EDGE_SEL_Msk (0x3UL)          /*!< SRSS SRSS_INTR_CFG: HVLVD1_EDGE_SEL (Bitfield-Mask: 0x03) */
/* ====================================================  SRSS.RES_CAUSE  ===================================================== */
#define SRSS_RES_CAUSE_RESET_WDT_Pos      (0UL)                 /*!< SRSS RES_CAUSE: RESET_WDT (Bit 0)                         */
#define SRSS_RES_CAUSE_RESET_WDT_Msk      (0x1UL)               /*!< SRSS RES_CAUSE: RESET_WDT (Bitfield-Mask: 0x01)           */
#define SRSS_RES_CAUSE_RESET_ACT_FAULT_Pos (1UL)                /*!< SRSS RES_CAUSE: RESET_ACT_FAULT (Bit 1)                   */
#define SRSS_RES_CAUSE_RESET_ACT_FAULT_Msk (0x2UL)              /*!< SRSS RES_CAUSE: RESET_ACT_FAULT (Bitfield-Mask: 0x01)     */
#define SRSS_RES_CAUSE_RESET_DPSLP_FAULT_Pos (2UL)              /*!< SRSS RES_CAUSE: RESET_DPSLP_FAULT (Bit 2)                 */
#define SRSS_RES_CAUSE_RESET_DPSLP_FAULT_Msk (0x4UL)            /*!< SRSS RES_CAUSE: RESET_DPSLP_FAULT (Bitfield-Mask: 0x01)   */
#define SRSS_RES_CAUSE_RESET_CSV_WCO_LOSS_Pos (3UL)             /*!< SRSS RES_CAUSE: RESET_CSV_WCO_LOSS (Bit 3)                */
#define SRSS_RES_CAUSE_RESET_CSV_WCO_LOSS_Msk (0x8UL)           /*!< SRSS RES_CAUSE: RESET_CSV_WCO_LOSS (Bitfield-Mask: 0x01)  */
#define SRSS_RES_CAUSE_RESET_SOFT_Pos     (4UL)                 /*!< SRSS RES_CAUSE: RESET_SOFT (Bit 4)                        */
#define SRSS_RES_CAUSE_RESET_SOFT_Msk     (0x10UL)              /*!< SRSS RES_CAUSE: RESET_SOFT (Bitfield-Mask: 0x01)          */
#define SRSS_RES_CAUSE_RESET_MCWDT0_Pos   (5UL)                 /*!< SRSS RES_CAUSE: RESET_MCWDT0 (Bit 5)                      */
#define SRSS_RES_CAUSE_RESET_MCWDT0_Msk   (0x20UL)              /*!< SRSS RES_CAUSE: RESET_MCWDT0 (Bitfield-Mask: 0x01)        */
#define SRSS_RES_CAUSE_RESET_MCWDT1_Pos   (6UL)                 /*!< SRSS RES_CAUSE: RESET_MCWDT1 (Bit 6)                      */
#define SRSS_RES_CAUSE_RESET_MCWDT1_Msk   (0x40UL)              /*!< SRSS RES_CAUSE: RESET_MCWDT1 (Bitfield-Mask: 0x01)        */
#define SRSS_RES_CAUSE_RESET_MCWDT2_Pos   (7UL)                 /*!< SRSS RES_CAUSE: RESET_MCWDT2 (Bit 7)                      */
#define SRSS_RES_CAUSE_RESET_MCWDT2_Msk   (0x80UL)              /*!< SRSS RES_CAUSE: RESET_MCWDT2 (Bitfield-Mask: 0x01)        */
#define SRSS_RES_CAUSE_RESET_MCWDT3_Pos   (8UL)                 /*!< SRSS RES_CAUSE: RESET_MCWDT3 (Bit 8)                      */
#define SRSS_RES_CAUSE_RESET_MCWDT3_Msk   (0x100UL)             /*!< SRSS RES_CAUSE: RESET_MCWDT3 (Bitfield-Mask: 0x01)        */
/* ====================================================  SRSS.RES_CAUSE2  ==================================================== */
#define SRSS_RES_CAUSE2_RESET_CSV_HF_LOSS_Pos (0UL)             /*!< SRSS RES_CAUSE2: RESET_CSV_HF_LOSS (Bit 0)                */
#define SRSS_RES_CAUSE2_RESET_CSV_HF_LOSS_Msk (0xffffUL)        /*!< SRSS RES_CAUSE2: RESET_CSV_HF_LOSS (Bitfield-Mask: 0xffff)*/
#define SRSS_RES_CAUSE2_RESET_CSV_HF_FREQ_Pos (16UL)            /*!< SRSS RES_CAUSE2: RESET_CSV_HF_FREQ (Bit 16)               */
#define SRSS_RES_CAUSE2_RESET_CSV_HF_FREQ_Msk (0xffff0000UL)    /*!< SRSS RES_CAUSE2: RESET_CSV_HF_FREQ (Bitfield-Mask: 0xffff)*/
/* =================================================  SRSS.PWR_TRIM_REF_CTL  ================================================= */
#define SRSS_PWR_TRIM_REF_CTL_ACT_REF_TCTRIM_Pos (0UL)          /*!< SRSS PWR_TRIM_REF_CTL: ACT_REF_TCTRIM (Bit 0)             */
#define SRSS_PWR_TRIM_REF_CTL_ACT_REF_TCTRIM_Msk (0xfUL)        /*!< SRSS PWR_TRIM_REF_CTL: ACT_REF_TCTRIM (Bitfield-Mask: 0x0f)*/
#define SRSS_PWR_TRIM_REF_CTL_ACT_REF_ITRIM_Pos (4UL)           /*!< SRSS PWR_TRIM_REF_CTL: ACT_REF_ITRIM (Bit 4)              */
#define SRSS_PWR_TRIM_REF_CTL_ACT_REF_ITRIM_Msk (0xf0UL)        /*!< SRSS PWR_TRIM_REF_CTL: ACT_REF_ITRIM (Bitfield-Mask: 0x0f)*/
#define SRSS_PWR_TRIM_REF_CTL_ACT_REF_ABSTRIM_Pos (8UL)         /*!< SRSS PWR_TRIM_REF_CTL: ACT_REF_ABSTRIM (Bit 8)            */
#define SRSS_PWR_TRIM_REF_CTL_ACT_REF_ABSTRIM_Msk (0x1f00UL)    /*!< SRSS PWR_TRIM_REF_CTL: ACT_REF_ABSTRIM (Bitfield-Mask: 0x1f)*/
#define SRSS_PWR_TRIM_REF_CTL_ACT_REF_IBOOST_Pos (14UL)         /*!< SRSS PWR_TRIM_REF_CTL: ACT_REF_IBOOST (Bit 14)            */
#define SRSS_PWR_TRIM_REF_CTL_ACT_REF_IBOOST_Msk (0x4000UL)     /*!< SRSS PWR_TRIM_REF_CTL: ACT_REF_IBOOST (Bitfield-Mask: 0x01)*/
#define SRSS_PWR_TRIM_REF_CTL_DPSLP_REF_TCTRIM_Pos (16UL)       /*!< SRSS PWR_TRIM_REF_CTL: DPSLP_REF_TCTRIM (Bit 16)          */
#define SRSS_PWR_TRIM_REF_CTL_DPSLP_REF_TCTRIM_Msk (0xf0000UL)  /*!< SRSS PWR_TRIM_REF_CTL: DPSLP_REF_TCTRIM (Bitfield-Mask: 0x0f)*/
#define SRSS_PWR_TRIM_REF_CTL_DPSLP_REF_ABSTRIM_Pos (20UL)      /*!< SRSS PWR_TRIM_REF_CTL: DPSLP_REF_ABSTRIM (Bit 20)         */
#define SRSS_PWR_TRIM_REF_CTL_DPSLP_REF_ABSTRIM_Msk (0x1f00000UL) /*!< SRSS PWR_TRIM_REF_CTL: DPSLP_REF_ABSTRIM (Bitfield-Mask: 0x1f)*/
#define SRSS_PWR_TRIM_REF_CTL_DPSLP_REF_ITRIM_Pos (28UL)        /*!< SRSS PWR_TRIM_REF_CTL: DPSLP_REF_ITRIM (Bit 28)           */
#define SRSS_PWR_TRIM_REF_CTL_DPSLP_REF_ITRIM_Msk (0xf0000000UL) /*!< SRSS PWR_TRIM_REF_CTL: DPSLP_REF_ITRIM (Bitfield-Mask: 0x0f)*/
/* ===============================================  SRSS.PWR_TRIM_BODOVP_CTL  ================================================ */
#define SRSS_PWR_TRIM_BODOVP_CTL_HVPORBOD_TRIPSEL_Pos (0UL)     /*!< SRSS PWR_TRIM_BODOVP_CTL: HVPORBOD_TRIPSEL (Bit 0)        */
#define SRSS_PWR_TRIM_BODOVP_CTL_HVPORBOD_TRIPSEL_Msk (0x7UL)   /*!< SRSS PWR_TRIM_BODOVP_CTL: HVPORBOD_TRIPSEL (Bitfield-Mask: 0x07)*/
#define SRSS_PWR_TRIM_BODOVP_CTL_HVPORBOD_OFSTRIM_Pos (4UL)     /*!< SRSS PWR_TRIM_BODOVP_CTL: HVPORBOD_OFSTRIM (Bit 4)        */
#define SRSS_PWR_TRIM_BODOVP_CTL_HVPORBOD_OFSTRIM_Msk (0x70UL)  /*!< SRSS PWR_TRIM_BODOVP_CTL: HVPORBOD_OFSTRIM (Bitfield-Mask: 0x07)*/
#define SRSS_PWR_TRIM_BODOVP_CTL_HVPORBOD_ITRIM_Pos (7UL)       /*!< SRSS PWR_TRIM_BODOVP_CTL: HVPORBOD_ITRIM (Bit 7)          */
#define SRSS_PWR_TRIM_BODOVP_CTL_HVPORBOD_ITRIM_Msk (0x380UL)   /*!< SRSS PWR_TRIM_BODOVP_CTL: HVPORBOD_ITRIM (Bitfield-Mask: 0x07)*/
#define SRSS_PWR_TRIM_BODOVP_CTL_LVPORBOD_TRIPSEL_Pos (10UL)    /*!< SRSS PWR_TRIM_BODOVP_CTL: LVPORBOD_TRIPSEL (Bit 10)       */
#define SRSS_PWR_TRIM_BODOVP_CTL_LVPORBOD_TRIPSEL_Msk (0x1c00UL) /*!< SRSS PWR_TRIM_BODOVP_CTL: LVPORBOD_TRIPSEL (Bitfield-Mask: 0x07)*/
#define SRSS_PWR_TRIM_BODOVP_CTL_LVPORBOD_OFSTRIM_Pos (14UL)    /*!< SRSS PWR_TRIM_BODOVP_CTL: LVPORBOD_OFSTRIM (Bit 14)       */
#define SRSS_PWR_TRIM_BODOVP_CTL_LVPORBOD_OFSTRIM_Msk (0x1c000UL) /*!< SRSS PWR_TRIM_BODOVP_CTL: LVPORBOD_OFSTRIM (Bitfield-Mask: 0x07)*/
#define SRSS_PWR_TRIM_BODOVP_CTL_LVPORBOD_ITRIM_Pos (17UL)      /*!< SRSS PWR_TRIM_BODOVP_CTL: LVPORBOD_ITRIM (Bit 17)         */
#define SRSS_PWR_TRIM_BODOVP_CTL_LVPORBOD_ITRIM_Msk (0xe0000UL) /*!< SRSS PWR_TRIM_BODOVP_CTL: LVPORBOD_ITRIM (Bitfield-Mask: 0x07)*/
/* =================================================  SRSS.CLK_TRIM_CCO_CTL  ================================================= */
#define SRSS_CLK_TRIM_CCO_CTL_CCO_RCSTRIM_Pos (0UL)             /*!< SRSS CLK_TRIM_CCO_CTL: CCO_RCSTRIM (Bit 0)                */
#define SRSS_CLK_TRIM_CCO_CTL_CCO_RCSTRIM_Msk (0x3fUL)          /*!< SRSS CLK_TRIM_CCO_CTL: CCO_RCSTRIM (Bitfield-Mask: 0x3f)  */
#define SRSS_CLK_TRIM_CCO_CTL_CCO_STABLE_CNT_Pos (24UL)         /*!< SRSS CLK_TRIM_CCO_CTL: CCO_STABLE_CNT (Bit 24)            */
#define SRSS_CLK_TRIM_CCO_CTL_CCO_STABLE_CNT_Msk (0x3f000000UL) /*!< SRSS CLK_TRIM_CCO_CTL: CCO_STABLE_CNT (Bitfield-Mask: 0x3f)*/
#define SRSS_CLK_TRIM_CCO_CTL_ENABLE_CNT_Pos (31UL)             /*!< SRSS CLK_TRIM_CCO_CTL: ENABLE_CNT (Bit 31)                */
#define SRSS_CLK_TRIM_CCO_CTL_ENABLE_CNT_Msk (0x80000000UL)     /*!< SRSS CLK_TRIM_CCO_CTL: ENABLE_CNT (Bitfield-Mask: 0x01)   */
/* ================================================  SRSS.CLK_TRIM_CCO_CTL2  ================================================= */
#define SRSS_CLK_TRIM_CCO_CTL2_CCO_FCTRIM1_Pos (0UL)            /*!< SRSS CLK_TRIM_CCO_CTL2: CCO_FCTRIM1 (Bit 0)               */
#define SRSS_CLK_TRIM_CCO_CTL2_CCO_FCTRIM1_Msk (0x1fUL)         /*!< SRSS CLK_TRIM_CCO_CTL2: CCO_FCTRIM1 (Bitfield-Mask: 0x1f) */
#define SRSS_CLK_TRIM_CCO_CTL2_CCO_FCTRIM2_Pos (5UL)            /*!< SRSS CLK_TRIM_CCO_CTL2: CCO_FCTRIM2 (Bit 5)               */
#define SRSS_CLK_TRIM_CCO_CTL2_CCO_FCTRIM2_Msk (0x3e0UL)        /*!< SRSS CLK_TRIM_CCO_CTL2: CCO_FCTRIM2 (Bitfield-Mask: 0x1f) */
#define SRSS_CLK_TRIM_CCO_CTL2_CCO_FCTRIM3_Pos (10UL)           /*!< SRSS CLK_TRIM_CCO_CTL2: CCO_FCTRIM3 (Bit 10)              */
#define SRSS_CLK_TRIM_CCO_CTL2_CCO_FCTRIM3_Msk (0x7c00UL)       /*!< SRSS CLK_TRIM_CCO_CTL2: CCO_FCTRIM3 (Bitfield-Mask: 0x1f) */
#define SRSS_CLK_TRIM_CCO_CTL2_CCO_FCTRIM4_Pos (15UL)           /*!< SRSS CLK_TRIM_CCO_CTL2: CCO_FCTRIM4 (Bit 15)              */
#define SRSS_CLK_TRIM_CCO_CTL2_CCO_FCTRIM4_Msk (0xf8000UL)      /*!< SRSS CLK_TRIM_CCO_CTL2: CCO_FCTRIM4 (Bitfield-Mask: 0x1f) */
#define SRSS_CLK_TRIM_CCO_CTL2_CCO_FCTRIM5_Pos (20UL)           /*!< SRSS CLK_TRIM_CCO_CTL2: CCO_FCTRIM5 (Bit 20)              */
#define SRSS_CLK_TRIM_CCO_CTL2_CCO_FCTRIM5_Msk (0x1f00000UL)    /*!< SRSS CLK_TRIM_CCO_CTL2: CCO_FCTRIM5 (Bitfield-Mask: 0x1f) */
/* ================================================  SRSS.PWR_TRIM_WAKE_CTL  ================================================= */
#define SRSS_PWR_TRIM_WAKE_CTL_WAKE_DELAY_Pos (0UL)             /*!< SRSS PWR_TRIM_WAKE_CTL: WAKE_DELAY (Bit 0)                */
#define SRSS_PWR_TRIM_WAKE_CTL_WAKE_DELAY_Msk (0xffUL)          /*!< SRSS PWR_TRIM_WAKE_CTL: WAKE_DELAY (Bitfield-Mask: 0xff)  */
/* =================================================  SRSS.PWR_TRIM_LVD_CTL  ================================================= */
#define SRSS_PWR_TRIM_LVD_CTL_HVLVD1_OFSTRIM_Pos (0UL)          /*!< SRSS PWR_TRIM_LVD_CTL: HVLVD1_OFSTRIM (Bit 0)             */
#define SRSS_PWR_TRIM_LVD_CTL_HVLVD1_OFSTRIM_Msk (0x7UL)        /*!< SRSS PWR_TRIM_LVD_CTL: HVLVD1_OFSTRIM (Bitfield-Mask: 0x07)*/
#define SRSS_PWR_TRIM_LVD_CTL_HVLVD1_ITRIM_Pos (4UL)            /*!< SRSS PWR_TRIM_LVD_CTL: HVLVD1_ITRIM (Bit 4)               */
#define SRSS_PWR_TRIM_LVD_CTL_HVLVD1_ITRIM_Msk (0x70UL)         /*!< SRSS PWR_TRIM_LVD_CTL: HVLVD1_ITRIM (Bitfield-Mask: 0x07) */
/* =================================================  SRSS.CLK_TRIM_ILO_CTL  ================================================= */
#define SRSS_CLK_TRIM_ILO_CTL_ILO_FTRIM_Pos (0UL)               /*!< SRSS CLK_TRIM_ILO_CTL: ILO_FTRIM (Bit 0)                  */
#define SRSS_CLK_TRIM_ILO_CTL_ILO_FTRIM_Msk (0x3fUL)            /*!< SRSS CLK_TRIM_ILO_CTL: ILO_FTRIM (Bitfield-Mask: 0x3f)    */
/* ===============================================  SRSS.PWR_TRIM_PWRSYS_CTL  ================================================ */
#define SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM_Pos (0UL)         /*!< SRSS PWR_TRIM_PWRSYS_CTL: ACT_REG_TRIM (Bit 0)            */
#define SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_TRIM_Msk (0x1fUL)      /*!< SRSS PWR_TRIM_PWRSYS_CTL: ACT_REG_TRIM (Bitfield-Mask: 0x1f)*/
#define SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_BOOST_Pos (30UL)       /*!< SRSS PWR_TRIM_PWRSYS_CTL: ACT_REG_BOOST (Bit 30)          */
#define SRSS_PWR_TRIM_PWRSYS_CTL_ACT_REG_BOOST_Msk (0xc0000000UL) /*!< SRSS PWR_TRIM_PWRSYS_CTL: ACT_REG_BOOST (Bitfield-Mask: 0x03)*/
/* =================================================  SRSS.CLK_TRIM_ECO_CTL  ================================================= */
#define SRSS_CLK_TRIM_ECO_CTL_WDTRIM_Pos  (0UL)                 /*!< SRSS CLK_TRIM_ECO_CTL: WDTRIM (Bit 0)                     */
#define SRSS_CLK_TRIM_ECO_CTL_WDTRIM_Msk  (0x7UL)               /*!< SRSS CLK_TRIM_ECO_CTL: WDTRIM (Bitfield-Mask: 0x07)       */
#define SRSS_CLK_TRIM_ECO_CTL_ATRIM_Pos   (4UL)                 /*!< SRSS CLK_TRIM_ECO_CTL: ATRIM (Bit 4)                      */
#define SRSS_CLK_TRIM_ECO_CTL_ATRIM_Msk   (0xf0UL)              /*!< SRSS CLK_TRIM_ECO_CTL: ATRIM (Bitfield-Mask: 0x0f)        */
#define SRSS_CLK_TRIM_ECO_CTL_FTRIM_Pos   (8UL)                 /*!< SRSS CLK_TRIM_ECO_CTL: FTRIM (Bit 8)                      */
#define SRSS_CLK_TRIM_ECO_CTL_FTRIM_Msk   (0x300UL)             /*!< SRSS CLK_TRIM_ECO_CTL: FTRIM (Bitfield-Mask: 0x03)        */
#define SRSS_CLK_TRIM_ECO_CTL_RTRIM_Pos   (10UL)                /*!< SRSS CLK_TRIM_ECO_CTL: RTRIM (Bit 10)                     */
#define SRSS_CLK_TRIM_ECO_CTL_RTRIM_Msk   (0xc00UL)             /*!< SRSS CLK_TRIM_ECO_CTL: RTRIM (Bitfield-Mask: 0x03)        */
#define SRSS_CLK_TRIM_ECO_CTL_GTRIM_Pos   (12UL)                /*!< SRSS CLK_TRIM_ECO_CTL: GTRIM (Bit 12)                     */
#define SRSS_CLK_TRIM_ECO_CTL_GTRIM_Msk   (0x3000UL)            /*!< SRSS CLK_TRIM_ECO_CTL: GTRIM (Bitfield-Mask: 0x03)        */
#define SRSS_CLK_TRIM_ECO_CTL_ITRIM_Pos   (16UL)                /*!< SRSS CLK_TRIM_ECO_CTL: ITRIM (Bit 16)                     */
#define SRSS_CLK_TRIM_ECO_CTL_ITRIM_Msk   (0x3f0000UL)          /*!< SRSS CLK_TRIM_ECO_CTL: ITRIM (Bitfield-Mask: 0x3f)        */
/* ================================================  SRSS.CLK_TRIM_PILO_CTL  ================================================= */
#define SRSS_CLK_TRIM_PILO_CTL_PILO_CFREQ_Pos (0UL)             /*!< SRSS CLK_TRIM_PILO_CTL: PILO_CFREQ (Bit 0)                */
#define SRSS_CLK_TRIM_PILO_CTL_PILO_CFREQ_Msk (0x3fUL)          /*!< SRSS CLK_TRIM_PILO_CTL: PILO_CFREQ (Bitfield-Mask: 0x3f)  */
#define SRSS_CLK_TRIM_PILO_CTL_PILO_OSC_TRIM_Pos (12UL)         /*!< SRSS CLK_TRIM_PILO_CTL: PILO_OSC_TRIM (Bit 12)            */
#define SRSS_CLK_TRIM_PILO_CTL_PILO_OSC_TRIM_Msk (0x7000UL)     /*!< SRSS CLK_TRIM_PILO_CTL: PILO_OSC_TRIM (Bitfield-Mask: 0x07)*/
#define SRSS_CLK_TRIM_PILO_CTL_PILO_COMP_TRIM_Pos (16UL)        /*!< SRSS CLK_TRIM_PILO_CTL: PILO_COMP_TRIM (Bit 16)           */
#define SRSS_CLK_TRIM_PILO_CTL_PILO_COMP_TRIM_Msk (0x30000UL)   /*!< SRSS CLK_TRIM_PILO_CTL: PILO_COMP_TRIM (Bitfield-Mask: 0x03)*/
#define SRSS_CLK_TRIM_PILO_CTL_PILO_NBIAS_TRIM_Pos (18UL)       /*!< SRSS CLK_TRIM_PILO_CTL: PILO_NBIAS_TRIM (Bit 18)          */
#define SRSS_CLK_TRIM_PILO_CTL_PILO_NBIAS_TRIM_Msk (0xc0000UL)  /*!< SRSS CLK_TRIM_PILO_CTL: PILO_NBIAS_TRIM (Bitfield-Mask: 0x03)*/
#define SRSS_CLK_TRIM_PILO_CTL_PILO_RES_TRIM_Pos (20UL)         /*!< SRSS CLK_TRIM_PILO_CTL: PILO_RES_TRIM (Bit 20)            */
#define SRSS_CLK_TRIM_PILO_CTL_PILO_RES_TRIM_Msk (0x1f00000UL)  /*!< SRSS CLK_TRIM_PILO_CTL: PILO_RES_TRIM (Bitfield-Mask: 0x1f)*/
#define SRSS_CLK_TRIM_PILO_CTL_PILO_ISLOPE_TRIM_Pos (26UL)      /*!< SRSS CLK_TRIM_PILO_CTL: PILO_ISLOPE_TRIM (Bit 26)         */
#define SRSS_CLK_TRIM_PILO_CTL_PILO_ISLOPE_TRIM_Msk (0xc000000UL) /*!< SRSS CLK_TRIM_PILO_CTL: PILO_ISLOPE_TRIM (Bitfield-Mask: 0x03)*/
#define SRSS_CLK_TRIM_PILO_CTL_PILO_VTDIFF_TRIM_Pos (28UL)      /*!< SRSS CLK_TRIM_PILO_CTL: PILO_VTDIFF_TRIM (Bit 28)         */
#define SRSS_CLK_TRIM_PILO_CTL_PILO_VTDIFF_TRIM_Msk (0x70000000UL) /*!< SRSS CLK_TRIM_PILO_CTL: PILO_VTDIFF_TRIM (Bitfield-Mask: 0x07)*/
/* ================================================  SRSS.CLK_TRIM_PILO_CTL2  ================================================ */
#define SRSS_CLK_TRIM_PILO_CTL2_PILO_VREF_TRIM_Pos (0UL)        /*!< SRSS CLK_TRIM_PILO_CTL2: PILO_VREF_TRIM (Bit 0)           */
#define SRSS_CLK_TRIM_PILO_CTL2_PILO_VREF_TRIM_Msk (0xffUL)     /*!< SRSS CLK_TRIM_PILO_CTL2: PILO_VREF_TRIM (Bitfield-Mask: 0xff)*/
#define SRSS_CLK_TRIM_PILO_CTL2_PILO_IREFBM_TRIM_Pos (8UL)      /*!< SRSS CLK_TRIM_PILO_CTL2: PILO_IREFBM_TRIM (Bit 8)         */
#define SRSS_CLK_TRIM_PILO_CTL2_PILO_IREFBM_TRIM_Msk (0x1f00UL) /*!< SRSS CLK_TRIM_PILO_CTL2: PILO_IREFBM_TRIM (Bitfield-Mask: 0x1f)*/
#define SRSS_CLK_TRIM_PILO_CTL2_PILO_IREF_TRIM_Pos (16UL)       /*!< SRSS CLK_TRIM_PILO_CTL2: PILO_IREF_TRIM (Bit 16)          */
#define SRSS_CLK_TRIM_PILO_CTL2_PILO_IREF_TRIM_Msk (0xff0000UL) /*!< SRSS CLK_TRIM_PILO_CTL2: PILO_IREF_TRIM (Bitfield-Mask: 0xff)*/
/* ================================================  SRSS.CLK_TRIM_PILO_CTL3  ================================================ */
#define SRSS_CLK_TRIM_PILO_CTL3_PILO_ENGOPT_Pos (0UL)           /*!< SRSS CLK_TRIM_PILO_CTL3: PILO_ENGOPT (Bit 0)              */
#define SRSS_CLK_TRIM_PILO_CTL3_PILO_ENGOPT_Msk (0xffffUL)      /*!< SRSS CLK_TRIM_PILO_CTL3: PILO_ENGOPT (Bitfield-Mask: 0xffff)*/


#endif /* _CYIP_SRSS_H_ */


/* [] END OF FILE */
