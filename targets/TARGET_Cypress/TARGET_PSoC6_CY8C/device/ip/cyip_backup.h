/***************************************************************************//**
* \file cyip_backup.h
*
* \brief
* BACKUP IP definitions
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

#ifndef _CYIP_BACKUP_H_
#define _CYIP_BACKUP_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                          BACKUP                                           ================ */
/* =========================================================================================================================== */

#define BACKUP_SECTION_SIZE         0x00010000UL

/**
  * \brief SRSS Backup Domain (BACKUP)
  */
typedef struct {                                /*!< BACKUP Structure                                                          */
  __IOM uint32_t CTL;                           /*!< 0x00000000 Control                                                        */
   __IM uint32_t RESERVED;
  __IOM uint32_t RTC_RW;                        /*!< 0x00000008 RTC Read Write register                                        */
  __IOM uint32_t CAL_CTL;                       /*!< 0x0000000C Oscillator calibration for absolute frequency                  */
   __IM uint32_t STATUS;                        /*!< 0x00000010 Status                                                         */
  __IOM uint32_t RTC_TIME;                      /*!< 0x00000014 Calendar Seconds, Minutes, Hours, Day of Week                  */
  __IOM uint32_t RTC_DATE;                      /*!< 0x00000018 Calendar Day of Month, Month,  Year                            */
  __IOM uint32_t ALM1_TIME;                     /*!< 0x0000001C Alarm 1 Seconds, Minute, Hours, Day of Week                    */
  __IOM uint32_t ALM1_DATE;                     /*!< 0x00000020 Alarm 1 Day of Month, Month                                    */
  __IOM uint32_t ALM2_TIME;                     /*!< 0x00000024 Alarm 2 Seconds, Minute, Hours, Day of Week                    */
  __IOM uint32_t ALM2_DATE;                     /*!< 0x00000028 Alarm 2 Day of Month, Month                                    */
  __IOM uint32_t INTR;                          /*!< 0x0000002C Interrupt request register                                     */
  __IOM uint32_t INTR_SET;                      /*!< 0x00000030 Interrupt set request register                                 */
  __IOM uint32_t INTR_MASK;                     /*!< 0x00000034 Interrupt mask register                                        */
   __IM uint32_t INTR_MASKED;                   /*!< 0x00000038 Interrupt masked request register                              */
   __IM uint32_t OSCCNT;                        /*!< 0x0000003C 32kHz oscillator counter                                       */
   __IM uint32_t TICKS;                         /*!< 0x00000040 128Hz tick counter                                             */
  __IOM uint32_t PMIC_CTL;                      /*!< 0x00000044 PMIC control register                                          */
   __IM uint32_t RESERVED1[1006];
  __IOM uint32_t BREG[8];                       /*!< 0x00001000 Backup register region                                         */
   __IM uint32_t RESERVED2[15288];
  __IOM uint32_t TRIM;                          /*!< 0x0000FF00 Trim Register                                                  */
} BACKUP_Type;                                  /*!< Size = 65284 (0xFF04)                                                     */


/* ======================================================  BACKUP.CTL  ======================================================= */
#define BACKUP_CTL_WCO_EN_Pos             (3UL)                 /*!< BACKUP CTL: WCO_EN (Bit 3)                                */
#define BACKUP_CTL_WCO_EN_Msk             (0x8UL)               /*!< BACKUP CTL: WCO_EN (Bitfield-Mask: 0x01)                  */
#define BACKUP_CTL_CLK_SEL_Pos            (8UL)                 /*!< BACKUP CTL: CLK_SEL (Bit 8)                               */
#define BACKUP_CTL_CLK_SEL_Msk            (0x300UL)             /*!< BACKUP CTL: CLK_SEL (Bitfield-Mask: 0x03)                 */
#define BACKUP_CTL_PRESCALER_Pos          (12UL)                /*!< BACKUP CTL: PRESCALER (Bit 12)                            */
#define BACKUP_CTL_PRESCALER_Msk          (0x3000UL)            /*!< BACKUP CTL: PRESCALER (Bitfield-Mask: 0x03)               */
#define BACKUP_CTL_WCO_BYPASS_Pos         (16UL)                /*!< BACKUP CTL: WCO_BYPASS (Bit 16)                           */
#define BACKUP_CTL_WCO_BYPASS_Msk         (0x10000UL)           /*!< BACKUP CTL: WCO_BYPASS (Bitfield-Mask: 0x01)              */
#define BACKUP_CTL_VDDBAK_CTL_Pos         (17UL)                /*!< BACKUP CTL: VDDBAK_CTL (Bit 17)                           */
#define BACKUP_CTL_VDDBAK_CTL_Msk         (0x60000UL)           /*!< BACKUP CTL: VDDBAK_CTL (Bitfield-Mask: 0x03)              */
#define BACKUP_CTL_VBACKUP_MEAS_Pos       (19UL)                /*!< BACKUP CTL: VBACKUP_MEAS (Bit 19)                         */
#define BACKUP_CTL_VBACKUP_MEAS_Msk       (0x80000UL)           /*!< BACKUP CTL: VBACKUP_MEAS (Bitfield-Mask: 0x01)            */
#define BACKUP_CTL_EN_CHARGE_KEY_Pos      (24UL)                /*!< BACKUP CTL: EN_CHARGE_KEY (Bit 24)                        */
#define BACKUP_CTL_EN_CHARGE_KEY_Msk      (0xff000000UL)        /*!< BACKUP CTL: EN_CHARGE_KEY (Bitfield-Mask: 0xff)           */
/* =====================================================  BACKUP.RTC_RW  ===================================================== */
#define BACKUP_RTC_RW_READ_Pos            (0UL)                 /*!< BACKUP RTC_RW: READ (Bit 0)                               */
#define BACKUP_RTC_RW_READ_Msk            (0x1UL)               /*!< BACKUP RTC_RW: READ (Bitfield-Mask: 0x01)                 */
#define BACKUP_RTC_RW_WRITE_Pos           (1UL)                 /*!< BACKUP RTC_RW: WRITE (Bit 1)                              */
#define BACKUP_RTC_RW_WRITE_Msk           (0x2UL)               /*!< BACKUP RTC_RW: WRITE (Bitfield-Mask: 0x01)                */
/* ====================================================  BACKUP.CAL_CTL  ===================================================== */
#define BACKUP_CAL_CTL_CALIB_VAL_Pos      (0UL)                 /*!< BACKUP CAL_CTL: CALIB_VAL (Bit 0)                         */
#define BACKUP_CAL_CTL_CALIB_VAL_Msk      (0x3fUL)              /*!< BACKUP CAL_CTL: CALIB_VAL (Bitfield-Mask: 0x3f)           */
#define BACKUP_CAL_CTL_CALIB_SIGN_Pos     (6UL)                 /*!< BACKUP CAL_CTL: CALIB_SIGN (Bit 6)                        */
#define BACKUP_CAL_CTL_CALIB_SIGN_Msk     (0x40UL)              /*!< BACKUP CAL_CTL: CALIB_SIGN (Bitfield-Mask: 0x01)          */
#define BACKUP_CAL_CTL_CAL_OUT_Pos        (31UL)                /*!< BACKUP CAL_CTL: CAL_OUT (Bit 31)                          */
#define BACKUP_CAL_CTL_CAL_OUT_Msk        (0x80000000UL)        /*!< BACKUP CAL_CTL: CAL_OUT (Bitfield-Mask: 0x01)             */
/* =====================================================  BACKUP.STATUS  ===================================================== */
#define BACKUP_STATUS_RTC_BUSY_Pos        (0UL)                 /*!< BACKUP STATUS: RTC_BUSY (Bit 0)                           */
#define BACKUP_STATUS_RTC_BUSY_Msk        (0x1UL)               /*!< BACKUP STATUS: RTC_BUSY (Bitfield-Mask: 0x01)             */
#define BACKUP_STATUS_WCO_OK_Pos          (2UL)                 /*!< BACKUP STATUS: WCO_OK (Bit 2)                             */
#define BACKUP_STATUS_WCO_OK_Msk          (0x4UL)               /*!< BACKUP STATUS: WCO_OK (Bitfield-Mask: 0x01)               */
/* ====================================================  BACKUP.RTC_TIME  ==================================================== */
#define BACKUP_RTC_TIME_RTC_SEC_Pos       (0UL)                 /*!< BACKUP RTC_TIME: RTC_SEC (Bit 0)                          */
#define BACKUP_RTC_TIME_RTC_SEC_Msk       (0x7fUL)              /*!< BACKUP RTC_TIME: RTC_SEC (Bitfield-Mask: 0x7f)            */
#define BACKUP_RTC_TIME_RTC_MIN_Pos       (8UL)                 /*!< BACKUP RTC_TIME: RTC_MIN (Bit 8)                          */
#define BACKUP_RTC_TIME_RTC_MIN_Msk       (0x7f00UL)            /*!< BACKUP RTC_TIME: RTC_MIN (Bitfield-Mask: 0x7f)            */
#define BACKUP_RTC_TIME_RTC_HOUR_Pos      (16UL)                /*!< BACKUP RTC_TIME: RTC_HOUR (Bit 16)                        */
#define BACKUP_RTC_TIME_RTC_HOUR_Msk      (0x3f0000UL)          /*!< BACKUP RTC_TIME: RTC_HOUR (Bitfield-Mask: 0x3f)           */
#define BACKUP_RTC_TIME_CTRL_12HR_Pos     (22UL)                /*!< BACKUP RTC_TIME: CTRL_12HR (Bit 22)                       */
#define BACKUP_RTC_TIME_CTRL_12HR_Msk     (0x400000UL)          /*!< BACKUP RTC_TIME: CTRL_12HR (Bitfield-Mask: 0x01)          */
#define BACKUP_RTC_TIME_RTC_DAY_Pos       (24UL)                /*!< BACKUP RTC_TIME: RTC_DAY (Bit 24)                         */
#define BACKUP_RTC_TIME_RTC_DAY_Msk       (0x7000000UL)         /*!< BACKUP RTC_TIME: RTC_DAY (Bitfield-Mask: 0x07)            */
/* ====================================================  BACKUP.RTC_DATE  ==================================================== */
#define BACKUP_RTC_DATE_RTC_DATE_Pos      (0UL)                 /*!< BACKUP RTC_DATE: RTC_DATE (Bit 0)                         */
#define BACKUP_RTC_DATE_RTC_DATE_Msk      (0x3fUL)              /*!< BACKUP RTC_DATE: RTC_DATE (Bitfield-Mask: 0x3f)           */
#define BACKUP_RTC_DATE_RTC_MON_Pos       (8UL)                 /*!< BACKUP RTC_DATE: RTC_MON (Bit 8)                          */
#define BACKUP_RTC_DATE_RTC_MON_Msk       (0x1f00UL)            /*!< BACKUP RTC_DATE: RTC_MON (Bitfield-Mask: 0x1f)            */
#define BACKUP_RTC_DATE_RTC_YEAR_Pos      (16UL)                /*!< BACKUP RTC_DATE: RTC_YEAR (Bit 16)                        */
#define BACKUP_RTC_DATE_RTC_YEAR_Msk      (0xff0000UL)          /*!< BACKUP RTC_DATE: RTC_YEAR (Bitfield-Mask: 0xff)           */
/* ===================================================  BACKUP.ALM1_TIME  ==================================================== */
#define BACKUP_ALM1_TIME_ALM_SEC_Pos      (0UL)                 /*!< BACKUP ALM1_TIME: ALM_SEC (Bit 0)                         */
#define BACKUP_ALM1_TIME_ALM_SEC_Msk      (0x7fUL)              /*!< BACKUP ALM1_TIME: ALM_SEC (Bitfield-Mask: 0x7f)           */
#define BACKUP_ALM1_TIME_ALM_SEC_EN_Pos   (7UL)                 /*!< BACKUP ALM1_TIME: ALM_SEC_EN (Bit 7)                      */
#define BACKUP_ALM1_TIME_ALM_SEC_EN_Msk   (0x80UL)              /*!< BACKUP ALM1_TIME: ALM_SEC_EN (Bitfield-Mask: 0x01)        */
#define BACKUP_ALM1_TIME_ALM_MIN_Pos      (8UL)                 /*!< BACKUP ALM1_TIME: ALM_MIN (Bit 8)                         */
#define BACKUP_ALM1_TIME_ALM_MIN_Msk      (0x7f00UL)            /*!< BACKUP ALM1_TIME: ALM_MIN (Bitfield-Mask: 0x7f)           */
#define BACKUP_ALM1_TIME_ALM_MIN_EN_Pos   (15UL)                /*!< BACKUP ALM1_TIME: ALM_MIN_EN (Bit 15)                     */
#define BACKUP_ALM1_TIME_ALM_MIN_EN_Msk   (0x8000UL)            /*!< BACKUP ALM1_TIME: ALM_MIN_EN (Bitfield-Mask: 0x01)        */
#define BACKUP_ALM1_TIME_ALM_HOUR_Pos     (16UL)                /*!< BACKUP ALM1_TIME: ALM_HOUR (Bit 16)                       */
#define BACKUP_ALM1_TIME_ALM_HOUR_Msk     (0x3f0000UL)          /*!< BACKUP ALM1_TIME: ALM_HOUR (Bitfield-Mask: 0x3f)          */
#define BACKUP_ALM1_TIME_ALM_HOUR_EN_Pos  (23UL)                /*!< BACKUP ALM1_TIME: ALM_HOUR_EN (Bit 23)                    */
#define BACKUP_ALM1_TIME_ALM_HOUR_EN_Msk  (0x800000UL)          /*!< BACKUP ALM1_TIME: ALM_HOUR_EN (Bitfield-Mask: 0x01)       */
#define BACKUP_ALM1_TIME_ALM_DAY_Pos      (24UL)                /*!< BACKUP ALM1_TIME: ALM_DAY (Bit 24)                        */
#define BACKUP_ALM1_TIME_ALM_DAY_Msk      (0x7000000UL)         /*!< BACKUP ALM1_TIME: ALM_DAY (Bitfield-Mask: 0x07)           */
#define BACKUP_ALM1_TIME_ALM_DAY_EN_Pos   (31UL)                /*!< BACKUP ALM1_TIME: ALM_DAY_EN (Bit 31)                     */
#define BACKUP_ALM1_TIME_ALM_DAY_EN_Msk   (0x80000000UL)        /*!< BACKUP ALM1_TIME: ALM_DAY_EN (Bitfield-Mask: 0x01)        */
/* ===================================================  BACKUP.ALM1_DATE  ==================================================== */
#define BACKUP_ALM1_DATE_ALM_DATE_Pos     (0UL)                 /*!< BACKUP ALM1_DATE: ALM_DATE (Bit 0)                        */
#define BACKUP_ALM1_DATE_ALM_DATE_Msk     (0x3fUL)              /*!< BACKUP ALM1_DATE: ALM_DATE (Bitfield-Mask: 0x3f)          */
#define BACKUP_ALM1_DATE_ALM_DATE_EN_Pos  (7UL)                 /*!< BACKUP ALM1_DATE: ALM_DATE_EN (Bit 7)                     */
#define BACKUP_ALM1_DATE_ALM_DATE_EN_Msk  (0x80UL)              /*!< BACKUP ALM1_DATE: ALM_DATE_EN (Bitfield-Mask: 0x01)       */
#define BACKUP_ALM1_DATE_ALM_MON_Pos      (8UL)                 /*!< BACKUP ALM1_DATE: ALM_MON (Bit 8)                         */
#define BACKUP_ALM1_DATE_ALM_MON_Msk      (0x1f00UL)            /*!< BACKUP ALM1_DATE: ALM_MON (Bitfield-Mask: 0x1f)           */
#define BACKUP_ALM1_DATE_ALM_MON_EN_Pos   (15UL)                /*!< BACKUP ALM1_DATE: ALM_MON_EN (Bit 15)                     */
#define BACKUP_ALM1_DATE_ALM_MON_EN_Msk   (0x8000UL)            /*!< BACKUP ALM1_DATE: ALM_MON_EN (Bitfield-Mask: 0x01)        */
#define BACKUP_ALM1_DATE_ALM_EN_Pos       (31UL)                /*!< BACKUP ALM1_DATE: ALM_EN (Bit 31)                         */
#define BACKUP_ALM1_DATE_ALM_EN_Msk       (0x80000000UL)        /*!< BACKUP ALM1_DATE: ALM_EN (Bitfield-Mask: 0x01)            */
/* ===================================================  BACKUP.ALM2_TIME  ==================================================== */
#define BACKUP_ALM2_TIME_ALM_SEC_Pos      (0UL)                 /*!< BACKUP ALM2_TIME: ALM_SEC (Bit 0)                         */
#define BACKUP_ALM2_TIME_ALM_SEC_Msk      (0x7fUL)              /*!< BACKUP ALM2_TIME: ALM_SEC (Bitfield-Mask: 0x7f)           */
#define BACKUP_ALM2_TIME_ALM_SEC_EN_Pos   (7UL)                 /*!< BACKUP ALM2_TIME: ALM_SEC_EN (Bit 7)                      */
#define BACKUP_ALM2_TIME_ALM_SEC_EN_Msk   (0x80UL)              /*!< BACKUP ALM2_TIME: ALM_SEC_EN (Bitfield-Mask: 0x01)        */
#define BACKUP_ALM2_TIME_ALM_MIN_Pos      (8UL)                 /*!< BACKUP ALM2_TIME: ALM_MIN (Bit 8)                         */
#define BACKUP_ALM2_TIME_ALM_MIN_Msk      (0x7f00UL)            /*!< BACKUP ALM2_TIME: ALM_MIN (Bitfield-Mask: 0x7f)           */
#define BACKUP_ALM2_TIME_ALM_MIN_EN_Pos   (15UL)                /*!< BACKUP ALM2_TIME: ALM_MIN_EN (Bit 15)                     */
#define BACKUP_ALM2_TIME_ALM_MIN_EN_Msk   (0x8000UL)            /*!< BACKUP ALM2_TIME: ALM_MIN_EN (Bitfield-Mask: 0x01)        */
#define BACKUP_ALM2_TIME_ALM_HOUR_Pos     (16UL)                /*!< BACKUP ALM2_TIME: ALM_HOUR (Bit 16)                       */
#define BACKUP_ALM2_TIME_ALM_HOUR_Msk     (0x3f0000UL)          /*!< BACKUP ALM2_TIME: ALM_HOUR (Bitfield-Mask: 0x3f)          */
#define BACKUP_ALM2_TIME_ALM_HOUR_EN_Pos  (23UL)                /*!< BACKUP ALM2_TIME: ALM_HOUR_EN (Bit 23)                    */
#define BACKUP_ALM2_TIME_ALM_HOUR_EN_Msk  (0x800000UL)          /*!< BACKUP ALM2_TIME: ALM_HOUR_EN (Bitfield-Mask: 0x01)       */
#define BACKUP_ALM2_TIME_ALM_DAY_Pos      (24UL)                /*!< BACKUP ALM2_TIME: ALM_DAY (Bit 24)                        */
#define BACKUP_ALM2_TIME_ALM_DAY_Msk      (0x7000000UL)         /*!< BACKUP ALM2_TIME: ALM_DAY (Bitfield-Mask: 0x07)           */
#define BACKUP_ALM2_TIME_ALM_DAY_EN_Pos   (31UL)                /*!< BACKUP ALM2_TIME: ALM_DAY_EN (Bit 31)                     */
#define BACKUP_ALM2_TIME_ALM_DAY_EN_Msk   (0x80000000UL)        /*!< BACKUP ALM2_TIME: ALM_DAY_EN (Bitfield-Mask: 0x01)        */
/* ===================================================  BACKUP.ALM2_DATE  ==================================================== */
#define BACKUP_ALM2_DATE_ALM_DATE_Pos     (0UL)                 /*!< BACKUP ALM2_DATE: ALM_DATE (Bit 0)                        */
#define BACKUP_ALM2_DATE_ALM_DATE_Msk     (0x3fUL)              /*!< BACKUP ALM2_DATE: ALM_DATE (Bitfield-Mask: 0x3f)          */
#define BACKUP_ALM2_DATE_ALM_DATE_EN_Pos  (7UL)                 /*!< BACKUP ALM2_DATE: ALM_DATE_EN (Bit 7)                     */
#define BACKUP_ALM2_DATE_ALM_DATE_EN_Msk  (0x80UL)              /*!< BACKUP ALM2_DATE: ALM_DATE_EN (Bitfield-Mask: 0x01)       */
#define BACKUP_ALM2_DATE_ALM_MON_Pos      (8UL)                 /*!< BACKUP ALM2_DATE: ALM_MON (Bit 8)                         */
#define BACKUP_ALM2_DATE_ALM_MON_Msk      (0x1f00UL)            /*!< BACKUP ALM2_DATE: ALM_MON (Bitfield-Mask: 0x1f)           */
#define BACKUP_ALM2_DATE_ALM_MON_EN_Pos   (15UL)                /*!< BACKUP ALM2_DATE: ALM_MON_EN (Bit 15)                     */
#define BACKUP_ALM2_DATE_ALM_MON_EN_Msk   (0x8000UL)            /*!< BACKUP ALM2_DATE: ALM_MON_EN (Bitfield-Mask: 0x01)        */
#define BACKUP_ALM2_DATE_ALM_EN_Pos       (31UL)                /*!< BACKUP ALM2_DATE: ALM_EN (Bit 31)                         */
#define BACKUP_ALM2_DATE_ALM_EN_Msk       (0x80000000UL)        /*!< BACKUP ALM2_DATE: ALM_EN (Bitfield-Mask: 0x01)            */
/* ======================================================  BACKUP.INTR  ====================================================== */
#define BACKUP_INTR_ALARM1_Pos            (0UL)                 /*!< BACKUP INTR: ALARM1 (Bit 0)                               */
#define BACKUP_INTR_ALARM1_Msk            (0x1UL)               /*!< BACKUP INTR: ALARM1 (Bitfield-Mask: 0x01)                 */
#define BACKUP_INTR_ALARM2_Pos            (1UL)                 /*!< BACKUP INTR: ALARM2 (Bit 1)                               */
#define BACKUP_INTR_ALARM2_Msk            (0x2UL)               /*!< BACKUP INTR: ALARM2 (Bitfield-Mask: 0x01)                 */
#define BACKUP_INTR_CENTURY_Pos           (2UL)                 /*!< BACKUP INTR: CENTURY (Bit 2)                              */
#define BACKUP_INTR_CENTURY_Msk           (0x4UL)               /*!< BACKUP INTR: CENTURY (Bitfield-Mask: 0x01)                */
/* ====================================================  BACKUP.INTR_SET  ==================================================== */
#define BACKUP_INTR_SET_ALARM1_Pos        (0UL)                 /*!< BACKUP INTR_SET: ALARM1 (Bit 0)                           */
#define BACKUP_INTR_SET_ALARM1_Msk        (0x1UL)               /*!< BACKUP INTR_SET: ALARM1 (Bitfield-Mask: 0x01)             */
#define BACKUP_INTR_SET_ALARM2_Pos        (1UL)                 /*!< BACKUP INTR_SET: ALARM2 (Bit 1)                           */
#define BACKUP_INTR_SET_ALARM2_Msk        (0x2UL)               /*!< BACKUP INTR_SET: ALARM2 (Bitfield-Mask: 0x01)             */
#define BACKUP_INTR_SET_CENTURY_Pos       (2UL)                 /*!< BACKUP INTR_SET: CENTURY (Bit 2)                          */
#define BACKUP_INTR_SET_CENTURY_Msk       (0x4UL)               /*!< BACKUP INTR_SET: CENTURY (Bitfield-Mask: 0x01)            */
/* ===================================================  BACKUP.INTR_MASK  ==================================================== */
#define BACKUP_INTR_MASK_ALARM1_Pos       (0UL)                 /*!< BACKUP INTR_MASK: ALARM1 (Bit 0)                          */
#define BACKUP_INTR_MASK_ALARM1_Msk       (0x1UL)               /*!< BACKUP INTR_MASK: ALARM1 (Bitfield-Mask: 0x01)            */
#define BACKUP_INTR_MASK_ALARM2_Pos       (1UL)                 /*!< BACKUP INTR_MASK: ALARM2 (Bit 1)                          */
#define BACKUP_INTR_MASK_ALARM2_Msk       (0x2UL)               /*!< BACKUP INTR_MASK: ALARM2 (Bitfield-Mask: 0x01)            */
#define BACKUP_INTR_MASK_CENTURY_Pos      (2UL)                 /*!< BACKUP INTR_MASK: CENTURY (Bit 2)                         */
#define BACKUP_INTR_MASK_CENTURY_Msk      (0x4UL)               /*!< BACKUP INTR_MASK: CENTURY (Bitfield-Mask: 0x01)           */
/* ==================================================  BACKUP.INTR_MASKED  =================================================== */
#define BACKUP_INTR_MASKED_ALARM1_Pos     (0UL)                 /*!< BACKUP INTR_MASKED: ALARM1 (Bit 0)                        */
#define BACKUP_INTR_MASKED_ALARM1_Msk     (0x1UL)               /*!< BACKUP INTR_MASKED: ALARM1 (Bitfield-Mask: 0x01)          */
#define BACKUP_INTR_MASKED_ALARM2_Pos     (1UL)                 /*!< BACKUP INTR_MASKED: ALARM2 (Bit 1)                        */
#define BACKUP_INTR_MASKED_ALARM2_Msk     (0x2UL)               /*!< BACKUP INTR_MASKED: ALARM2 (Bitfield-Mask: 0x01)          */
#define BACKUP_INTR_MASKED_CENTURY_Pos    (2UL)                 /*!< BACKUP INTR_MASKED: CENTURY (Bit 2)                       */
#define BACKUP_INTR_MASKED_CENTURY_Msk    (0x4UL)               /*!< BACKUP INTR_MASKED: CENTURY (Bitfield-Mask: 0x01)         */
/* =====================================================  BACKUP.OSCCNT  ===================================================== */
#define BACKUP_OSCCNT_CNT32KHZ_Pos        (0UL)                 /*!< BACKUP OSCCNT: CNT32KHZ (Bit 0)                           */
#define BACKUP_OSCCNT_CNT32KHZ_Msk        (0xffUL)              /*!< BACKUP OSCCNT: CNT32KHZ (Bitfield-Mask: 0xff)             */
/* =====================================================  BACKUP.TICKS  ====================================================== */
#define BACKUP_TICKS_CNT128HZ_Pos         (0UL)                 /*!< BACKUP TICKS: CNT128HZ (Bit 0)                            */
#define BACKUP_TICKS_CNT128HZ_Msk         (0x3fUL)              /*!< BACKUP TICKS: CNT128HZ (Bitfield-Mask: 0x3f)              */
/* ====================================================  BACKUP.PMIC_CTL  ==================================================== */
#define BACKUP_PMIC_CTL_UNLOCK_Pos        (8UL)                 /*!< BACKUP PMIC_CTL: UNLOCK (Bit 8)                           */
#define BACKUP_PMIC_CTL_UNLOCK_Msk        (0xff00UL)            /*!< BACKUP PMIC_CTL: UNLOCK (Bitfield-Mask: 0xff)             */
#define BACKUP_PMIC_CTL_POLARITY_Pos      (16UL)                /*!< BACKUP PMIC_CTL: POLARITY (Bit 16)                        */
#define BACKUP_PMIC_CTL_POLARITY_Msk      (0x10000UL)           /*!< BACKUP PMIC_CTL: POLARITY (Bitfield-Mask: 0x01)           */
#define BACKUP_PMIC_CTL_PMIC_EN_OUTEN_Pos (29UL)                /*!< BACKUP PMIC_CTL: PMIC_EN_OUTEN (Bit 29)                   */
#define BACKUP_PMIC_CTL_PMIC_EN_OUTEN_Msk (0x20000000UL)        /*!< BACKUP PMIC_CTL: PMIC_EN_OUTEN (Bitfield-Mask: 0x01)      */
#define BACKUP_PMIC_CTL_PMIC_ALWAYSEN_Pos (30UL)                /*!< BACKUP PMIC_CTL: PMIC_ALWAYSEN (Bit 30)                   */
#define BACKUP_PMIC_CTL_PMIC_ALWAYSEN_Msk (0x40000000UL)        /*!< BACKUP PMIC_CTL: PMIC_ALWAYSEN (Bitfield-Mask: 0x01)      */
#define BACKUP_PMIC_CTL_PMIC_EN_Pos       (31UL)                /*!< BACKUP PMIC_CTL: PMIC_EN (Bit 31)                         */
#define BACKUP_PMIC_CTL_PMIC_EN_Msk       (0x80000000UL)        /*!< BACKUP PMIC_CTL: PMIC_EN (Bitfield-Mask: 0x01)            */
/* ======================================================  BACKUP.BREG  ====================================================== */
#define BACKUP_BREG_BREG_Pos              (0UL)                 /*!< BACKUP BREG: BREG (Bit 0)                                 */
#define BACKUP_BREG_BREG_Msk              (0xffffffffUL)        /*!< BACKUP BREG: BREG (Bitfield-Mask: 0xffffffff)             */
/* ======================================================  BACKUP.TRIM  ====================================================== */
#define BACKUP_TRIM_TRIM_Pos              (0UL)                 /*!< BACKUP TRIM: TRIM (Bit 0)                                 */
#define BACKUP_TRIM_TRIM_Msk              (0x3fUL)              /*!< BACKUP TRIM: TRIM (Bitfield-Mask: 0x3f)                   */


#endif /* _CYIP_BACKUP_H_ */


/* [] END OF FILE */
