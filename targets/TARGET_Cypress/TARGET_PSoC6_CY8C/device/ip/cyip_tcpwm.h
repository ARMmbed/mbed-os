/***************************************************************************//**
* \file cyip_tcpwm.h
*
* \brief
* TCPWM IP definitions
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

#ifndef _CYIP_TCPWM_H_
#define _CYIP_TCPWM_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                           TCPWM                                           ================ */
/* =========================================================================================================================== */

#define TCPWM_CNT_SECTION_SIZE      0x00000040UL
#define TCPWM_SECTION_SIZE          0x00010000UL

/**
  * \brief Timer/Counter/PWM Counter Module (TCPWM_CNT)
  */
typedef struct {
  __IOM uint32_t CTRL;                          /*!< 0x00000000 Counter control register                                       */
   __IM uint32_t STATUS;                        /*!< 0x00000004 Counter status register                                        */
  __IOM uint32_t COUNTER;                       /*!< 0x00000008 Counter count register                                         */
  __IOM uint32_t CC;                            /*!< 0x0000000C Counter compare/capture register                               */
  __IOM uint32_t CC_BUFF;                       /*!< 0x00000010 Counter buffered compare/capture register                      */
  __IOM uint32_t PERIOD;                        /*!< 0x00000014 Counter period register                                        */
  __IOM uint32_t PERIOD_BUFF;                   /*!< 0x00000018 Counter buffered period register                               */
   __IM uint32_t RESERVED;
  __IOM uint32_t TR_CTRL0;                      /*!< 0x00000020 Counter trigger control register 0                             */
  __IOM uint32_t TR_CTRL1;                      /*!< 0x00000024 Counter trigger control register 1                             */
  __IOM uint32_t TR_CTRL2;                      /*!< 0x00000028 Counter trigger control register 2                             */
   __IM uint32_t RESERVED1;
  __IOM uint32_t INTR;                          /*!< 0x00000030 Interrupt request register                                     */
  __IOM uint32_t INTR_SET;                      /*!< 0x00000034 Interrupt set request register                                 */
  __IOM uint32_t INTR_MASK;                     /*!< 0x00000038 Interrupt mask register                                        */
   __IM uint32_t INTR_MASKED;                   /*!< 0x0000003C Interrupt masked request register                              */
} TCPWM_CNT_Type;                               /*!< Size = 64 (0x40)                                                          */

/**
  * \brief Timer/Counter/PWM (TCPWM)
  */
typedef struct {                                /*!< TCPWM Structure                                                           */
  __IOM uint32_t CTRL;                          /*!< 0x00000000 TCPWM control register                                         */
  __IOM uint32_t CTRL_CLR;                      /*!< 0x00000004 TCPWM control clear register                                   */
  __IOM uint32_t CTRL_SET;                      /*!< 0x00000008 TCPWM control set register                                     */
  __IOM uint32_t CMD_CAPTURE;                   /*!< 0x0000000C TCPWM capture command register                                 */
  __IOM uint32_t CMD_RELOAD;                    /*!< 0x00000010 TCPWM reload command register                                  */
  __IOM uint32_t CMD_STOP;                      /*!< 0x00000014 TCPWM stop command register                                    */
  __IOM uint32_t CMD_START;                     /*!< 0x00000018 TCPWM start command register                                   */
   __IM uint32_t INTR_CAUSE;                    /*!< 0x0000001C TCPWM Counter interrupt cause register                         */
   __IM uint32_t RESERVED[56];
        TCPWM_CNT_Type CNT[32];                 /*!< 0x00000100 Timer/Counter/PWM Counter Module                               */
} TCPWM_Type;                                   /*!< Size = 2304 (0x900)                                                       */


/* ====================================================  TCPWM_CNT.CTRL  ===================================================== */
#define TCPWM_CNT_CTRL_AUTO_RELOAD_CC_Pos (0UL)                 /*!< TCPWM_CNT CTRL: AUTO_RELOAD_CC (Bit 0)                    */
#define TCPWM_CNT_CTRL_AUTO_RELOAD_CC_Msk (0x1UL)               /*!< TCPWM_CNT CTRL: AUTO_RELOAD_CC (Bitfield-Mask: 0x01)      */
#define TCPWM_CNT_CTRL_AUTO_RELOAD_PERIOD_Pos (1UL)             /*!< TCPWM_CNT CTRL: AUTO_RELOAD_PERIOD (Bit 1)                */
#define TCPWM_CNT_CTRL_AUTO_RELOAD_PERIOD_Msk (0x2UL)           /*!< TCPWM_CNT CTRL: AUTO_RELOAD_PERIOD (Bitfield-Mask: 0x01)  */
#define TCPWM_CNT_CTRL_PWM_SYNC_KILL_Pos  (2UL)                 /*!< TCPWM_CNT CTRL: PWM_SYNC_KILL (Bit 2)                     */
#define TCPWM_CNT_CTRL_PWM_SYNC_KILL_Msk  (0x4UL)               /*!< TCPWM_CNT CTRL: PWM_SYNC_KILL (Bitfield-Mask: 0x01)       */
#define TCPWM_CNT_CTRL_PWM_STOP_ON_KILL_Pos (3UL)               /*!< TCPWM_CNT CTRL: PWM_STOP_ON_KILL (Bit 3)                  */
#define TCPWM_CNT_CTRL_PWM_STOP_ON_KILL_Msk (0x8UL)             /*!< TCPWM_CNT CTRL: PWM_STOP_ON_KILL (Bitfield-Mask: 0x01)    */
#define TCPWM_CNT_CTRL_GENERIC_Pos        (8UL)                 /*!< TCPWM_CNT CTRL: GENERIC (Bit 8)                           */
#define TCPWM_CNT_CTRL_GENERIC_Msk        (0xff00UL)            /*!< TCPWM_CNT CTRL: GENERIC (Bitfield-Mask: 0xff)             */
#define TCPWM_CNT_CTRL_UP_DOWN_MODE_Pos   (16UL)                /*!< TCPWM_CNT CTRL: UP_DOWN_MODE (Bit 16)                     */
#define TCPWM_CNT_CTRL_UP_DOWN_MODE_Msk   (0x30000UL)           /*!< TCPWM_CNT CTRL: UP_DOWN_MODE (Bitfield-Mask: 0x03)        */
#define TCPWM_CNT_CTRL_ONE_SHOT_Pos       (18UL)                /*!< TCPWM_CNT CTRL: ONE_SHOT (Bit 18)                         */
#define TCPWM_CNT_CTRL_ONE_SHOT_Msk       (0x40000UL)           /*!< TCPWM_CNT CTRL: ONE_SHOT (Bitfield-Mask: 0x01)            */
#define TCPWM_CNT_CTRL_QUADRATURE_MODE_Pos (20UL)               /*!< TCPWM_CNT CTRL: QUADRATURE_MODE (Bit 20)                  */
#define TCPWM_CNT_CTRL_QUADRATURE_MODE_Msk (0x300000UL)         /*!< TCPWM_CNT CTRL: QUADRATURE_MODE (Bitfield-Mask: 0x03)     */
#define TCPWM_CNT_CTRL_MODE_Pos           (24UL)                /*!< TCPWM_CNT CTRL: MODE (Bit 24)                             */
#define TCPWM_CNT_CTRL_MODE_Msk           (0x7000000UL)         /*!< TCPWM_CNT CTRL: MODE (Bitfield-Mask: 0x07)                */
/* ===================================================  TCPWM_CNT.STATUS  ==================================================== */
#define TCPWM_CNT_STATUS_DOWN_Pos         (0UL)                 /*!< TCPWM_CNT STATUS: DOWN (Bit 0)                            */
#define TCPWM_CNT_STATUS_DOWN_Msk         (0x1UL)               /*!< TCPWM_CNT STATUS: DOWN (Bitfield-Mask: 0x01)              */
#define TCPWM_CNT_STATUS_GENERIC_Pos      (8UL)                 /*!< TCPWM_CNT STATUS: GENERIC (Bit 8)                         */
#define TCPWM_CNT_STATUS_GENERIC_Msk      (0xff00UL)            /*!< TCPWM_CNT STATUS: GENERIC (Bitfield-Mask: 0xff)           */
#define TCPWM_CNT_STATUS_RUNNING_Pos      (31UL)                /*!< TCPWM_CNT STATUS: RUNNING (Bit 31)                        */
#define TCPWM_CNT_STATUS_RUNNING_Msk      (0x80000000UL)        /*!< TCPWM_CNT STATUS: RUNNING (Bitfield-Mask: 0x01)           */
/* ===================================================  TCPWM_CNT.COUNTER  =================================================== */
#define TCPWM_CNT_COUNTER_COUNTER_Pos     (0UL)                 /*!< TCPWM_CNT COUNTER: COUNTER (Bit 0)                        */
#define TCPWM_CNT_COUNTER_COUNTER_Msk     (0xffffffffUL)        /*!< TCPWM_CNT COUNTER: COUNTER (Bitfield-Mask: 0xffffffff)    */
/* =====================================================  TCPWM_CNT.CC  ====================================================== */
#define TCPWM_CNT_CC_CC_Pos               (0UL)                 /*!< TCPWM_CNT CC: CC (Bit 0)                                  */
#define TCPWM_CNT_CC_CC_Msk               (0xffffffffUL)        /*!< TCPWM_CNT CC: CC (Bitfield-Mask: 0xffffffff)              */
/* ===================================================  TCPWM_CNT.CC_BUFF  =================================================== */
#define TCPWM_CNT_CC_BUFF_CC_Pos          (0UL)                 /*!< TCPWM_CNT CC_BUFF: CC (Bit 0)                             */
#define TCPWM_CNT_CC_BUFF_CC_Msk          (0xffffffffUL)        /*!< TCPWM_CNT CC_BUFF: CC (Bitfield-Mask: 0xffffffff)         */
/* ===================================================  TCPWM_CNT.PERIOD  ==================================================== */
#define TCPWM_CNT_PERIOD_PERIOD_Pos       (0UL)                 /*!< TCPWM_CNT PERIOD: PERIOD (Bit 0)                          */
#define TCPWM_CNT_PERIOD_PERIOD_Msk       (0xffffffffUL)        /*!< TCPWM_CNT PERIOD: PERIOD (Bitfield-Mask: 0xffffffff)      */
/* =================================================  TCPWM_CNT.PERIOD_BUFF  ================================================= */
#define TCPWM_CNT_PERIOD_BUFF_PERIOD_Pos  (0UL)                 /*!< TCPWM_CNT PERIOD_BUFF: PERIOD (Bit 0)                     */
#define TCPWM_CNT_PERIOD_BUFF_PERIOD_Msk  (0xffffffffUL)        /*!< TCPWM_CNT PERIOD_BUFF: PERIOD (Bitfield-Mask: 0xffffffff) */
/* ==================================================  TCPWM_CNT.TR_CTRL0  =================================================== */
#define TCPWM_CNT_TR_CTRL0_CAPTURE_SEL_Pos (0UL)                /*!< TCPWM_CNT TR_CTRL0: CAPTURE_SEL (Bit 0)                   */
#define TCPWM_CNT_TR_CTRL0_CAPTURE_SEL_Msk (0xfUL)              /*!< TCPWM_CNT TR_CTRL0: CAPTURE_SEL (Bitfield-Mask: 0x0f)     */
#define TCPWM_CNT_TR_CTRL0_COUNT_SEL_Pos  (4UL)                 /*!< TCPWM_CNT TR_CTRL0: COUNT_SEL (Bit 4)                     */
#define TCPWM_CNT_TR_CTRL0_COUNT_SEL_Msk  (0xf0UL)              /*!< TCPWM_CNT TR_CTRL0: COUNT_SEL (Bitfield-Mask: 0x0f)       */
#define TCPWM_CNT_TR_CTRL0_RELOAD_SEL_Pos (8UL)                 /*!< TCPWM_CNT TR_CTRL0: RELOAD_SEL (Bit 8)                    */
#define TCPWM_CNT_TR_CTRL0_RELOAD_SEL_Msk (0xf00UL)             /*!< TCPWM_CNT TR_CTRL0: RELOAD_SEL (Bitfield-Mask: 0x0f)      */
#define TCPWM_CNT_TR_CTRL0_STOP_SEL_Pos   (12UL)                /*!< TCPWM_CNT TR_CTRL0: STOP_SEL (Bit 12)                     */
#define TCPWM_CNT_TR_CTRL0_STOP_SEL_Msk   (0xf000UL)            /*!< TCPWM_CNT TR_CTRL0: STOP_SEL (Bitfield-Mask: 0x0f)        */
#define TCPWM_CNT_TR_CTRL0_START_SEL_Pos  (16UL)                /*!< TCPWM_CNT TR_CTRL0: START_SEL (Bit 16)                    */
#define TCPWM_CNT_TR_CTRL0_START_SEL_Msk  (0xf0000UL)           /*!< TCPWM_CNT TR_CTRL0: START_SEL (Bitfield-Mask: 0x0f)       */
/* ==================================================  TCPWM_CNT.TR_CTRL1  =================================================== */
#define TCPWM_CNT_TR_CTRL1_CAPTURE_EDGE_Pos (0UL)               /*!< TCPWM_CNT TR_CTRL1: CAPTURE_EDGE (Bit 0)                  */
#define TCPWM_CNT_TR_CTRL1_CAPTURE_EDGE_Msk (0x3UL)             /*!< TCPWM_CNT TR_CTRL1: CAPTURE_EDGE (Bitfield-Mask: 0x03)    */
#define TCPWM_CNT_TR_CTRL1_COUNT_EDGE_Pos (2UL)                 /*!< TCPWM_CNT TR_CTRL1: COUNT_EDGE (Bit 2)                    */
#define TCPWM_CNT_TR_CTRL1_COUNT_EDGE_Msk (0xcUL)               /*!< TCPWM_CNT TR_CTRL1: COUNT_EDGE (Bitfield-Mask: 0x03)      */
#define TCPWM_CNT_TR_CTRL1_RELOAD_EDGE_Pos (4UL)                /*!< TCPWM_CNT TR_CTRL1: RELOAD_EDGE (Bit 4)                   */
#define TCPWM_CNT_TR_CTRL1_RELOAD_EDGE_Msk (0x30UL)             /*!< TCPWM_CNT TR_CTRL1: RELOAD_EDGE (Bitfield-Mask: 0x03)     */
#define TCPWM_CNT_TR_CTRL1_STOP_EDGE_Pos  (6UL)                 /*!< TCPWM_CNT TR_CTRL1: STOP_EDGE (Bit 6)                     */
#define TCPWM_CNT_TR_CTRL1_STOP_EDGE_Msk  (0xc0UL)              /*!< TCPWM_CNT TR_CTRL1: STOP_EDGE (Bitfield-Mask: 0x03)       */
#define TCPWM_CNT_TR_CTRL1_START_EDGE_Pos (8UL)                 /*!< TCPWM_CNT TR_CTRL1: START_EDGE (Bit 8)                    */
#define TCPWM_CNT_TR_CTRL1_START_EDGE_Msk (0x300UL)             /*!< TCPWM_CNT TR_CTRL1: START_EDGE (Bitfield-Mask: 0x03)      */
/* ==================================================  TCPWM_CNT.TR_CTRL2  =================================================== */
#define TCPWM_CNT_TR_CTRL2_CC_MATCH_MODE_Pos (0UL)              /*!< TCPWM_CNT TR_CTRL2: CC_MATCH_MODE (Bit 0)                 */
#define TCPWM_CNT_TR_CTRL2_CC_MATCH_MODE_Msk (0x3UL)            /*!< TCPWM_CNT TR_CTRL2: CC_MATCH_MODE (Bitfield-Mask: 0x03)   */
#define TCPWM_CNT_TR_CTRL2_OVERFLOW_MODE_Pos (2UL)              /*!< TCPWM_CNT TR_CTRL2: OVERFLOW_MODE (Bit 2)                 */
#define TCPWM_CNT_TR_CTRL2_OVERFLOW_MODE_Msk (0xcUL)            /*!< TCPWM_CNT TR_CTRL2: OVERFLOW_MODE (Bitfield-Mask: 0x03)   */
#define TCPWM_CNT_TR_CTRL2_UNDERFLOW_MODE_Pos (4UL)             /*!< TCPWM_CNT TR_CTRL2: UNDERFLOW_MODE (Bit 4)                */
#define TCPWM_CNT_TR_CTRL2_UNDERFLOW_MODE_Msk (0x30UL)          /*!< TCPWM_CNT TR_CTRL2: UNDERFLOW_MODE (Bitfield-Mask: 0x03)  */
/* ====================================================  TCPWM_CNT.INTR  ===================================================== */
#define TCPWM_CNT_INTR_TC_Pos             (0UL)                 /*!< TCPWM_CNT INTR: TC (Bit 0)                                */
#define TCPWM_CNT_INTR_TC_Msk             (0x1UL)               /*!< TCPWM_CNT INTR: TC (Bitfield-Mask: 0x01)                  */
#define TCPWM_CNT_INTR_CC_MATCH_Pos       (1UL)                 /*!< TCPWM_CNT INTR: CC_MATCH (Bit 1)                          */
#define TCPWM_CNT_INTR_CC_MATCH_Msk       (0x2UL)               /*!< TCPWM_CNT INTR: CC_MATCH (Bitfield-Mask: 0x01)            */
/* ==================================================  TCPWM_CNT.INTR_SET  =================================================== */
#define TCPWM_CNT_INTR_SET_TC_Pos         (0UL)                 /*!< TCPWM_CNT INTR_SET: TC (Bit 0)                            */
#define TCPWM_CNT_INTR_SET_TC_Msk         (0x1UL)               /*!< TCPWM_CNT INTR_SET: TC (Bitfield-Mask: 0x01)              */
#define TCPWM_CNT_INTR_SET_CC_MATCH_Pos   (1UL)                 /*!< TCPWM_CNT INTR_SET: CC_MATCH (Bit 1)                      */
#define TCPWM_CNT_INTR_SET_CC_MATCH_Msk   (0x2UL)               /*!< TCPWM_CNT INTR_SET: CC_MATCH (Bitfield-Mask: 0x01)        */
/* ==================================================  TCPWM_CNT.INTR_MASK  ================================================== */
#define TCPWM_CNT_INTR_MASK_TC_Pos        (0UL)                 /*!< TCPWM_CNT INTR_MASK: TC (Bit 0)                           */
#define TCPWM_CNT_INTR_MASK_TC_Msk        (0x1UL)               /*!< TCPWM_CNT INTR_MASK: TC (Bitfield-Mask: 0x01)             */
#define TCPWM_CNT_INTR_MASK_CC_MATCH_Pos  (1UL)                 /*!< TCPWM_CNT INTR_MASK: CC_MATCH (Bit 1)                     */
#define TCPWM_CNT_INTR_MASK_CC_MATCH_Msk  (0x2UL)               /*!< TCPWM_CNT INTR_MASK: CC_MATCH (Bitfield-Mask: 0x01)       */
/* =================================================  TCPWM_CNT.INTR_MASKED  ================================================= */
#define TCPWM_CNT_INTR_MASKED_TC_Pos      (0UL)                 /*!< TCPWM_CNT INTR_MASKED: TC (Bit 0)                         */
#define TCPWM_CNT_INTR_MASKED_TC_Msk      (0x1UL)               /*!< TCPWM_CNT INTR_MASKED: TC (Bitfield-Mask: 0x01)           */
#define TCPWM_CNT_INTR_MASKED_CC_MATCH_Pos (1UL)                /*!< TCPWM_CNT INTR_MASKED: CC_MATCH (Bit 1)                   */
#define TCPWM_CNT_INTR_MASKED_CC_MATCH_Msk (0x2UL)              /*!< TCPWM_CNT INTR_MASKED: CC_MATCH (Bitfield-Mask: 0x01)     */


/* ======================================================  TCPWM.CTRL  ======================================================= */
#define TCPWM_CTRL_COUNTER_ENABLED_Pos    (0UL)                 /*!< TCPWM CTRL: COUNTER_ENABLED (Bit 0)                       */
#define TCPWM_CTRL_COUNTER_ENABLED_Msk    (0xffffffffUL)        /*!< TCPWM CTRL: COUNTER_ENABLED (Bitfield-Mask: 0xffffffff)   */
/* ====================================================  TCPWM.CTRL_CLR  ===================================================== */
#define TCPWM_CTRL_CLR_COUNTER_ENABLED_Pos (0UL)                /*!< TCPWM CTRL_CLR: COUNTER_ENABLED (Bit 0)                   */
#define TCPWM_CTRL_CLR_COUNTER_ENABLED_Msk (0xffffffffUL)       /*!< TCPWM CTRL_CLR: COUNTER_ENABLED (Bitfield-Mask: 0xffffffff)*/
/* ====================================================  TCPWM.CTRL_SET  ===================================================== */
#define TCPWM_CTRL_SET_COUNTER_ENABLED_Pos (0UL)                /*!< TCPWM CTRL_SET: COUNTER_ENABLED (Bit 0)                   */
#define TCPWM_CTRL_SET_COUNTER_ENABLED_Msk (0xffffffffUL)       /*!< TCPWM CTRL_SET: COUNTER_ENABLED (Bitfield-Mask: 0xffffffff)*/
/* ===================================================  TCPWM.CMD_CAPTURE  =================================================== */
#define TCPWM_CMD_CAPTURE_COUNTER_CAPTURE_Pos (0UL)             /*!< TCPWM CMD_CAPTURE: COUNTER_CAPTURE (Bit 0)                */
#define TCPWM_CMD_CAPTURE_COUNTER_CAPTURE_Msk (0xffffffffUL)    /*!< TCPWM CMD_CAPTURE: COUNTER_CAPTURE (Bitfield-Mask: 0xffffffff)*/
/* ===================================================  TCPWM.CMD_RELOAD  ==================================================== */
#define TCPWM_CMD_RELOAD_COUNTER_RELOAD_Pos (0UL)               /*!< TCPWM CMD_RELOAD: COUNTER_RELOAD (Bit 0)                  */
#define TCPWM_CMD_RELOAD_COUNTER_RELOAD_Msk (0xffffffffUL)      /*!< TCPWM CMD_RELOAD: COUNTER_RELOAD (Bitfield-Mask: 0xffffffff)*/
/* ====================================================  TCPWM.CMD_STOP  ===================================================== */
#define TCPWM_CMD_STOP_COUNTER_STOP_Pos   (0UL)                 /*!< TCPWM CMD_STOP: COUNTER_STOP (Bit 0)                      */
#define TCPWM_CMD_STOP_COUNTER_STOP_Msk   (0xffffffffUL)        /*!< TCPWM CMD_STOP: COUNTER_STOP (Bitfield-Mask: 0xffffffff)  */
/* ====================================================  TCPWM.CMD_START  ==================================================== */
#define TCPWM_CMD_START_COUNTER_START_Pos (0UL)                 /*!< TCPWM CMD_START: COUNTER_START (Bit 0)                    */
#define TCPWM_CMD_START_COUNTER_START_Msk (0xffffffffUL)        /*!< TCPWM CMD_START: COUNTER_START (Bitfield-Mask: 0xffffffff)*/
/* ===================================================  TCPWM.INTR_CAUSE  ==================================================== */
#define TCPWM_INTR_CAUSE_COUNTER_INT_Pos  (0UL)                 /*!< TCPWM INTR_CAUSE: COUNTER_INT (Bit 0)                     */
#define TCPWM_INTR_CAUSE_COUNTER_INT_Msk  (0xffffffffUL)        /*!< TCPWM INTR_CAUSE: COUNTER_INT (Bitfield-Mask: 0xffffffff) */


#endif /* _CYIP_TCPWM_H_ */


/* [] END OF FILE */
