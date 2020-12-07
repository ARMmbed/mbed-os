/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019-2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/* Copyright (c) 2019-2020 Renesas Electronics Corporation.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/***********************************************************************************************************************
* File Name    : r_can_rz_config.h
* Description  : The RZ CAN config label.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 20.09.2019 1.00 First Release
***********************************************************************************************************************/
#ifndef CAN_CONFIG_HEADER_FILE
#define CAN_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes>, "Project Includes"
***********************************************************************************************************************/
#include "r_can_rz_if.h"
#include "iobitmasks/rcanfd_iobitmask.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Max loops to poll a CAN mode bit for expected value.
Maximum mode transition time is 2times of CAN frame.
This value depends on the communication bit rate */
#define MAX_CAN_SW_DELAY      (0x30000)

/* Max loops to wait for completion inter-channel communication.
This value is used for R_CAN_CrcTest function.
This value depends on the communication bit rate */
#define MAX_CAN_ICC_DELAY     (0x30000)


/*****************************************************************
                R Z   C A N   A P I   C O N F I G
******************************************************************/

/**************************************************
* R_CAN_Create function config
***************************************************/
/*=====================================*/
/* Global Function config              */
/*=====================================*/
/* RSCFDnCFDGCFG : Global Configuration Register */
#define    GCFG_ITRCP         (0x0000)      /* Interval Timer Prescaler Set bit                */
#define    GCFG_TSBTCS        (0x00)        /* Timestamp Clock Source Select bit               */
#define    GCFG_TSSS          (0x00)        /* Timestamp Source Select bit                     */
#define    GCFG_TSP           (0x00)        /* Timestamp Clock Source Division bit             */
#define    GCFG_CMPOC         (0x00)        /* Payload Overflow Mode Select bit                */
#define    GCFG_DCS           (0x00)        /* CAN Clock Source Select bit                     */
#define    GCFG_MME           (0x00)        /* Mirror Function Enable bit                      */
#define    GCFG_DRE           (0x00)        /* DLC Replacement Enable                          */
#define    GCFG_DCE           (0x00)        /* DLC Check Enable bit                            */
#define    GCFG_TPRI          (0x00)        /* Transmit Priority Select bit                    */

/* RSCFDnCFDGFDCFG : Global FD Configuration Register */
#define    GFDCFG_TSCCFG      (0x00)        /* Timestamp capture setting bit                   */
#define    GFDCFG_RPED        (0x00)        /* Protocol exception event detection disable bit  */


/*=====================================*/
/*  Transmit/Receive FIFO config       */
/*  (FIFO number 0~5)                  */
/*=====================================*/
/* TR_FIFO_0 */
/* RSCFD0CFDCFCC0 : Transmit/Receive FIFO Buffer Configuration and Control Register */
#define    CFCC0_CFITT        (0x00)        /* Set a message transmission interval bit                       */
#define    CFCC0_CFTML        (0x00)        /* Transmit Buffer Link Configuration bit                        */
#define    CFCC0_CFITR        (0x00)        /* Transmit/Receive FIFO Interval Timer Resolution bit           */
#define    CFCC0_CFITSS       (0x00)        /* Transmit/Receive FIFO Interval Timer Clock Source Select bit  */
#define    CFCC0_CFM          (0x01)        /* Transmit/Receive FIFO Mode Select bit                         */
#define    CFCC0_CFIGCV       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Request Timing Select */
#define    CFCC0_CFIM         (0x00)        /* Transmit/Receive FIFO Interrupt Source Select bit             */
#define    CFCC0_CFDC         (0x02)        /* Transmit/Receive FIFO Buffer Depth Configuration bit          */
#define    CFCC0_CFPLS        (0x07)        /* Transmit/Receive FIFO Buffer Payload Storage Size Select bit  */
#define    CFCC0_CFTXIE       (0x00)        /* Transmit/Receive FIFO Transmit Interrupt Enable bit           */
#define    CFCC0_CFRXIE       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Enable bit            */

/* TR_FIFO_1 */
/* RSCFD0CFDCFCC1 : Transmit/Receive FIFO Buffer Configuration and Control Register */
#define    CFCC1_CFITT        (0x00)        /* Set a message transmission interval bit                       */
#define    CFCC1_CFTML        (0x00)        /* Transmit Buffer Link Configuration bit                        */
#define    CFCC1_CFITR        (0x00)        /* Transmit/Receive FIFO Interval Timer Resolution bit           */
#define    CFCC1_CFITSS       (0x00)        /* Transmit/Receive FIFO Interval Timer Clock Source Select bit  */
#define    CFCC1_CFM          (0x00)        /* Transmit/Receive FIFO Mode Select bit                         */
#define    CFCC1_CFIGCV       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Request Timing Select */
#define    CFCC1_CFIM         (0x01)        /* Transmit/Receive FIFO Interrupt Source Select bit             */
#define    CFCC1_CFDC         (0x01)        /* Transmit/Receive FIFO Buffer Depth Configuration bit          */
#define    CFCC1_CFPLS        (0x07)        /* Transmit/Receive FIFO Buffer Payload Storage Size Select bit  */
#define    CFCC1_CFTXIE       (0x00)        /* Transmit/Receive FIFO Transmit Interrupt Enable bit           */
#define    CFCC1_CFRXIE       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Enable bit            */

/* TR_FIFO_2 */
/* RSCFD0CFDCFCC2 : Transmit/Receive FIFO Buffer Configuration and Control Register */
#define    CFCC2_CFITT        (0x00)        /* Set a message transmission interval bit                       */
#define    CFCC2_CFTML        (0x00)        /* Transmit Buffer Link Configuration bit                        */
#define    CFCC2_CFITR        (0x00)        /* Transmit/Receive FIFO Interval Timer Resolution bit           */
#define    CFCC2_CFITSS       (0x00)        /* Transmit/Receive FIFO Interval Timer Clock Source Select bit  */
#define    CFCC2_CFM          (0x00)        /* Transmit/Receive FIFO Mode Select bit                         */
#define    CFCC2_CFIGCV       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Request Timing Select */
#define    CFCC2_CFIM         (0x00)        /* Transmit/Receive FIFO Interrupt Source Select bit             */
#define    CFCC2_CFDC         (0x00)        /* Transmit/Receive FIFO Buffer Depth Configuration bit          */
#define    CFCC2_CFPLS        (0x00)        /* Transmit/Receive FIFO Buffer Payload Storage Size Select bit  */
#define    CFCC2_CFTXIE       (0x00)        /* Transmit/Receive FIFO Transmit Interrupt Enable bit           */
#define    CFCC2_CFRXIE       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Enable bit            */

/* TR_FIFO_3 */
/* RSCFD0CFDCFCC3 : Transmit/Receive FIFO Buffer Configuration and Control Register */
#define    CFCC3_CFITT        (0x00)        /* Set a message transmission interval bit                       */
#define    CFCC3_CFTML        (0x00)        /* Transmit Buffer Link Configuration bit                        */
#define    CFCC3_CFITR        (0x00)        /* Transmit/Receive FIFO Interval Timer Resolution bit           */
#define    CFCC3_CFITSS       (0x00)        /* Transmit/Receive FIFO Interval Timer Clock Source Select bit  */
#define    CFCC3_CFM          (0x01)        /* Transmit/Receive FIFO Mode Select bit                         */
#define    CFCC3_CFIGCV       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Request Timing Select */
#define    CFCC3_CFIM         (0x00)        /* Transmit/Receive FIFO Interrupt Source Select bit             */
#define    CFCC3_CFDC         (0x02)        /* Transmit/Receive FIFO Buffer Depth Configuration bit          */
#define    CFCC3_CFPLS        (0x07)        /* Transmit/Receive FIFO Buffer Payload Storage Size Select bit  */
#define    CFCC3_CFTXIE       (0x00)        /* Transmit/Receive FIFO Transmit Interrupt Enable bit           */
#define    CFCC3_CFRXIE       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Enable bit            */

/* TR_FIFO_4 */
/* RSCFD0CFDCFCC4 : Transmit/Receive FIFO Buffer Configuration and Control Register */
#define    CFCC4_CFITT        (0x00)        /* Set a message transmission interval bit                       */
#define    CFCC4_CFTML        (0x00)        /* Transmit Buffer Link Configuration bit                        */
#define    CFCC4_CFITR        (0x00)        /* Transmit/Receive FIFO Interval Timer Resolution bit           */
#define    CFCC4_CFITSS       (0x00)        /* Transmit/Receive FIFO Interval Timer Clock Source Select bit  */
#define    CFCC4_CFM          (0x00)        /* Transmit/Receive FIFO Mode Select bit                         */
#define    CFCC4_CFIGCV       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Request Timing Select */
#define    CFCC4_CFIM         (0x01)        /* Transmit/Receive FIFO Interrupt Source Select bit             */
#define    CFCC4_CFDC         (0x01)        /* Transmit/Receive FIFO Buffer Depth Configuration bit          */
#define    CFCC4_CFPLS        (0x07)        /* Transmit/Receive FIFO Buffer Payload Storage Size Select bit  */
#define    CFCC4_CFTXIE       (0x00)        /* Transmit/Receive FIFO Transmit Interrupt Enable bit           */
#define    CFCC4_CFRXIE       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Enable bit            */

/* TR_FIFO_5 */
/* RSCFD0CFDCFCC5 : Transmit/Receive FIFO Buffer Configuration and Control Register */
#define    CFCC5_CFITT        (0x00)        /* Set a message transmission interval bit                       */
#define    CFCC5_CFTML        (0x00)        /* Transmit Buffer Link Configuration bit                        */
#define    CFCC5_CFITR        (0x00)        /* Transmit/Receive FIFO Interval Timer Resolution bit           */
#define    CFCC5_CFITSS       (0x00)        /* Transmit/Receive FIFO Interval Timer Clock Source Select bit  */
#define    CFCC5_CFM          (0x00)        /* Transmit/Receive FIFO Mode Select bit                         */
#define    CFCC5_CFIGCV       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Request Timing Select */
#define    CFCC5_CFIM         (0x00)        /* Transmit/Receive FIFO Interrupt Source Select bit             */
#define    CFCC5_CFDC         (0x00)        /* Transmit/Receive FIFO Buffer Depth Configuration bit          */
#define    CFCC5_CFPLS        (0x00)        /* Transmit/Receive FIFO Buffer Payload Storage Size Select bit  */
#define    CFCC5_CFTXIE       (0x00)        /* Transmit/Receive FIFO Transmit Interrupt Enable bit           */
#define    CFCC5_CFRXIE       (0x00)        /* Transmit/Receive FIFO Receive Interrupt Enable bit            */


/*=====================================*/
/*  Receive FIFO config                */
/*  (FIFO number 0~7)                  */
/*=====================================*/
/* RX_FIFO_0 */
/* RSCFD0CFDRFCC0 : Receive FIFO Buffer Configuration and Control Register */
#define    RFCC0_RFIGCV     (0x01)        /* Receive FIFO Interrupt Request Timing Select bit    */
#define    RFCC0_RFIM       (0x01)        /* Receive FIFO Interrupt Source Select bit            */
#define    RFCC0_RFDC       (0x02)        /* Receive FIFO Buffer Depth Configuration bit         */
#define    RFCC0_RFPLS      (0x07)        /* Receive FIFO Buffer Payload Storage Size Select bit */
#define    RFCC0_RFIE       (0x00)        /* Receive FIFO Interrupt Enable bit                   */

/* RX_FIFO_1 */
/* RSCFD0CFDRFCC1 : Receive FIFO Buffer Configuration and Control Register */
#define    RFCC1_RFIGCV     (0x00)        /* Receive FIFO Interrupt Request Timing Select bit    */
#define    RFCC1_RFIM       (0x00)        /* Receive FIFO Interrupt Source Select bit            */
#define    RFCC1_RFDC       (0x00)        /* Receive FIFO Buffer Depth Configuration bit         */
#define    RFCC1_RFPLS      (0x00)        /* Receive FIFO Buffer Payload Storage Size Select bit */
#define    RFCC1_RFIE       (0x00)        /* Receive FIFO Interrupt Enable bit                   */

/* RX_FIFO_2 */
/* RSCFD0CFDRFCC2 : Receive FIFO Buffer Configuration and Control Register */
#define    RFCC2_RFIGCV     (0x00)        /* Receive FIFO Interrupt Request Timing Select bit    */
#define    RFCC2_RFIM       (0x00)        /* Receive FIFO Interrupt Source Select bit            */
#define    RFCC2_RFDC       (0x00)        /* Receive FIFO Buffer Depth Configuration bit         */
#define    RFCC2_RFPLS      (0x00)        /* Receive FIFO Buffer Payload Storage Size Select bit */
#define    RFCC2_RFIE       (0x00)        /* Receive FIFO Interrupt Enable bit                   */

/* RX_FIFO_3 */
/* RSCFD0CFDRFCC3 : Receive FIFO Buffer Configuration and Control Register */
#define    RFCC3_RFIGCV     (0x00)        /* Receive FIFO Interrupt Request Timing Select bit    */
#define    RFCC3_RFIM       (0x00)        /* Receive FIFO Interrupt Source Select bit            */
#define    RFCC3_RFDC       (0x00)        /* Receive FIFO Buffer Depth Configuration bit         */
#define    RFCC3_RFPLS      (0x00)        /* Receive FIFO Buffer Payload Storage Size Select bit */
#define    RFCC3_RFIE       (0x00)        /* Receive FIFO Interrupt Enable bit                   */

/* RX_FIFO_4 */
/* RSCFD0CFDRFCC4 : Receive FIFO Buffer Configuration and Control Register */
#define    RFCC4_RFIGCV     (0x01)        /* Receive FIFO Interrupt Request Timing Select bit    */
#define    RFCC4_RFIM       (0x01)        /* Receive FIFO Interrupt Source Select bit            */
#define    RFCC4_RFDC       (0x02)        /* Receive FIFO Buffer Depth Configuration bit         */
#define    RFCC4_RFPLS      (0x07)        /* Receive FIFO Buffer Payload Storage Size Select bit */
#define    RFCC4_RFIE       (0x00)        /* Receive FIFO Interrupt Enable bit                   */

/* RX_FIFO_5 */
/* RSCFD0CFDRFCC5 : Receive FIFO Buffer Configuration and Control Register */
#define    RFCC5_RFIGCV     (0x00)        /* Receive FIFO Interrupt Request Timing Select bit    */
#define    RFCC5_RFIM       (0x00)        /* Receive FIFO Interrupt Source Select bit            */
#define    RFCC5_RFDC       (0x00)        /* Receive FIFO Buffer Depth Configuration bit         */
#define    RFCC5_RFPLS      (0x00)        /* Receive FIFO Buffer Payload Storage Size Select bit */
#define    RFCC5_RFIE       (0x00)        /* Receive FIFO Interrupt Enable bit                   */

/* RX_FIFO_6 */
/* RSCFD0CFDRFCC6 : Receive FIFO Buffer Configuration and Control Register */
#define    RFCC6_RFIGCV     (0x00)        /* Receive FIFO Interrupt Request Timing Select bit    */
#define    RFCC6_RFIM       (0x00)        /* Receive FIFO Interrupt Source Select bit            */
#define    RFCC6_RFDC       (0x00)        /* Receive FIFO Buffer Depth Configuration bit         */
#define    RFCC6_RFPLS      (0x00)        /* Receive FIFO Buffer Payload Storage Size Select bit */
#define    RFCC6_RFIE       (0x00)        /* Receive FIFO Interrupt Enable bit                   */

/* RX_FIFO_7 */
/* RSCFD0CFDRFCC7 : Receive FIFO Buffer Configuration and Control Register */
#define    RFCC7_RFIGCV     (0x00)        /* Receive FIFO Interrupt Request Timing Select bit    */
#define    RFCC7_RFIM       (0x00)        /* Receive FIFO Interrupt Source Select bit            */
#define    RFCC7_RFDC       (0x00)        /* Receive FIFO Buffer Depth Configuration bit         */
#define    RFCC7_RFPLS      (0x00)        /* Receive FIFO Buffer Payload Storage Size Select bit */
#define    RFCC7_RFIE       (0x00)        /* Receive FIFO Interrupt Enable bit                   */


/*=====================================*/
/*  Receive Buffer config              */
/*=====================================*/
/* RSCFDnCFDRMNB : Receive Buffer Number Register */
#define    RMNB_RMPLS     (0x00)        /* Receive Buffer Payload Storage Size Select bit */
#define    RMNB_NRXMB     (0x00)        /* Receive Buffer Number Configuration bit        */


/*=====================================*/
/*  Transmit Buffer config             */
/*=====================================*/
/* RSCFDnCFDTMIECy : Transmit Buffer Interrupt Enable Configuration Register */
#define    TMIEC0_TMIE31     (0x00)        /* Transmit Buffer Interrupt Enable bit31     */
#define    TMIEC0_TMIE30     (0x00)        /* Transmit Buffer Interrupt Enable bit30     */
#define    TMIEC0_TMIE29     (0x00)        /* Transmit Buffer Interrupt Enable bit29     */
#define    TMIEC0_TMIE28     (0x00)        /* Transmit Buffer Interrupt Enable bit28     */
#define    TMIEC0_TMIE27     (0x00)        /* Transmit Buffer Interrupt Enable bit27     */
#define    TMIEC0_TMIE26     (0x00)        /* Transmit Buffer Interrupt Enable bit26     */
#define    TMIEC0_TMIE25     (0x00)        /* Transmit Buffer Interrupt Enable bit25     */
#define    TMIEC0_TMIE24     (0x00)        /* Transmit Buffer Interrupt Enable bit24     */
#define    TMIEC0_TMIE23     (0x00)        /* Transmit Buffer Interrupt Enable bit23     */
#define    TMIEC0_TMIE22     (0x00)        /* Transmit Buffer Interrupt Enable bit22     */
#define    TMIEC0_TMIE21     (0x00)        /* Transmit Buffer Interrupt Enable bit21     */
#define    TMIEC0_TMIE20     (0x00)        /* Transmit Buffer Interrupt Enable bit20     */
#define    TMIEC0_TMIE19     (0x00)        /* Transmit Buffer Interrupt Enable bit19     */
#define    TMIEC0_TMIE18     (0x00)        /* Transmit Buffer Interrupt Enable bit18     */
#define    TMIEC0_TMIE17     (0x00)        /* Transmit Buffer Interrupt Enable bit17     */
#define    TMIEC0_TMIE16     (0x00)        /* Transmit Buffer Interrupt Enable bit16     */
#define    TMIEC0_TMIE15     (0x00)        /* Transmit Buffer Interrupt Enable bit15     */
#define    TMIEC0_TMIE14     (0x00)        /* Transmit Buffer Interrupt Enable bit14     */
#define    TMIEC0_TMIE13     (0x00)        /* Transmit Buffer Interrupt Enable bit13     */
#define    TMIEC0_TMIE12     (0x00)        /* Transmit Buffer Interrupt Enable bit12     */
#define    TMIEC0_TMIE11     (0x00)        /* Transmit Buffer Interrupt Enable bit11     */
#define    TMIEC0_TMIE10     (0x00)        /* Transmit Buffer Interrupt Enable bit10     */
#define    TMIEC0_TMIE9      (0x00)        /* Transmit Buffer Interrupt Enable bit09     */
#define    TMIEC0_TMIE8      (0x00)        /* Transmit Buffer Interrupt Enable bit08     */
#define    TMIEC0_TMIE7      (0x00)        /* Transmit Buffer Interrupt Enable bit07     */
#define    TMIEC0_TMIE6      (0x00)        /* Transmit Buffer Interrupt Enable bit06     */
#define    TMIEC0_TMIE5      (0x00)        /* Transmit Buffer Interrupt Enable bit05     */
#define    TMIEC0_TMIE4      (0x00)        /* Transmit Buffer Interrupt Enable bit04     */
#define    TMIEC0_TMIE3      (0x00)        /* Transmit Buffer Interrupt Enable bit03     */
#define    TMIEC0_TMIE2      (0x00)        /* Transmit Buffer Interrupt Enable bit02     */
#define    TMIEC0_TMIE1      (0x00)        /* Transmit Buffer Interrupt Enable bit01     */
#define    TMIEC0_TMIE0      (0x00)        /* Transmit Buffer Interrupt Enable bit00     */


/*=====================================*/
/*  Transmit Queue config              */
/*=====================================*/
/* When using transmit queue, The macro "GCFG_TPRI" must be set to 0 */

/* For channel 0 */
/* RSCFD0CFDTXQCC0 : Transmit Queue Configuration and Control Register */
#define    TXQCC0_TXQIM        (0x00)        /* Transmit Queue Interrupt Source Select bit */
#define    TXQCC0_TXQIE        (0x00)        /* Transmit Queue Interrupt Enable bit        */
#define    TXQCC0_TXQDC        (0x00)        /* Transmit Queue Depth Configuration bit     */

/* For channel 1 */
/* RSCFD0CFDTXQCC1 : Transmit Queue Configuration and Control Register */
#define    TXQCC1_TXQIM        (0x00)        /* Transmit Queue Interrupt Source Select bit */
#define    TXQCC1_TXQIE        (0x00)        /* Transmit Queue Interrupt Enable bit        */
#define    TXQCC1_TXQDC        (0x00)        /* Transmit Queue Depth Configuration bit     */


/*=====================================*/
/*  Transmit History function config   */
/*=====================================*/
/* For channel 0 */
/* RSCFD0CFDTHLCC0 : Transmit History List Configuration and Control Register */
#define    THLCC0_THLDTE        (0x00)        /* Transmit History Target Buffer Select bit    */
#define    THLCC0_THLIM         (0x00)        /* Transmit History Interrupt Source Select bit */
#define    THLCC0_THLIE         (0x00)        /* Transmit History Interrupt Enable bit        */

/* For channel 1 */
/* RSCFD0CFDTHLCC1 : Transmit History List Configuration and Control Register */
#define    THLCC1_THLDTE        (0x00)        /* Transmit History Target Buffer Select bit    */
#define    THLCC1_THLIM         (0x00)        /* Transmit History Interrupt Source Select bit */
#define    THLCC1_THLIE         (0x00)        /* Transmit History Interrupt Enable bit        */


/*=====================================*/
/*  Global Error Interrupt config      */
/*=====================================*/
/* RSCFDnCFDGCTR : Global Control Register */
#define    GCTR_CMPOFIE   (0x00)        /* Payload Overflow Interrupt Enable bit                 */
#define    GCTR_THLEIE    (0x00)        /* Transmit History Buffer Overflow Interrupt Enable bit */
#define    GCTR_MEIE      (0x00)        /* FIFO Message Lost Interrupt Enable bit                */
#define    GCTR_DEIE      (0x00)        /* DLC Error Interrupt Enable bit                        */


/*=====================================*/
/*  Channel config                     */
/*    Restricted Operation Mode        */
/*    Bus Off Recovery Mode            */
/*    Error Display Mode               */
/*=====================================*/
/* For channel 0 */
/* RSCFD0CFDC0CTR : Channel Control Register */
#define    C0CTR_ROM         (0x00)     /* Restricted Operation Mode Enable bit                          */
#define    C0CTR_BOM         (0x00)     /* Bus Off Recovery Mode Select bit                              */
#define    C0CTR_ERRD        (0x00)     /* Error Display Mode Select bit                                 */

/* For channel 1 */
/* RSCFD0CFDC1CTR : Channel Control Register */
#define    C1CTR_ROM         (0x00)     /* Restricted Operation Mode Enable bit                          */
#define    C1CTR_BOM         (0x00)     /* Bus Off Recovery Mode Select bit                              */
#define    C1CTR_ERRD        (0x00)     /* Error Display Mode Select bit                                 */


/*=====================================*/
/*  Channel Interrupt config           */
/*=====================================*/
/* For channel 0 */
/* RSCFD0CFDC0CTR : Channel Control Register */
#define    C0CTR_TDCVFIE     (0x00)     /* Transmitter Delay Compensation Violation Interrupt Enable bit */
#define    C0CTR_SOCOIE      (0x00)     /* Successful Occurrence Counter Overflow Interrupt Enable bit   */
#define    C0CTR_EOCOIE      (0x00)     /* Error Occurrence Counter Overflow Interrupt Enable bit        */
#define    C0CTR_TAIE        (0x00)     /* Transmit Abort Interrupt Enable bit                           */
#define    C0CTR_ALIE        (0x00)     /* Arbitration Lost Interrupt Enable bit                         */
#define    C0CTR_BLIE        (0x00)     /* Bus Lock Interrupt Enable bit                                 */
#define    C0CTR_OLIE        (0x00)     /* Overload Frame Transmit Interrupt Enable bit                  */
#define    C0CTR_BORIE       (0x00)     /* Bus Off Recovery Interrupt Enable bit                         */
#define    C0CTR_BOEIE       (0x00)     /* Bus Off Entry Interrupt Enable bit                            */
#define    C0CTR_EPIE        (0x00)     /* Error Passive Interrupt Enable bit                            */
#define    C0CTR_EWIE        (0x00)     /* Error Warning Interrupt Enable bit                            */
#define    C0CTR_BEIE        (0x00)     /* Bus Error Interrupt Enable bit                                */

/* For channel 1 */
/* RSCFD0CFDC1CTR : Channel Control Register */
#define    C1CTR_TDCVFIE     (0x00)     /* Transmitter Delay Compensation Violation Interrupt Enable bit */
#define    C1CTR_SOCOIE      (0x00)     /* Successful Occurrence Counter Overflow Interrupt Enable bit   */
#define    C1CTR_EOCOIE      (0x00)     /* Error Occurrence Counter Overflow Interrupt Enable bit        */
#define    C1CTR_TAIE        (0x00)     /* Transmit Abort Interrupt Enable bit                           */
#define    C1CTR_ALIE        (0x00)     /* Arbitration Lost Interrupt Enable bit                         */
#define    C1CTR_BLIE        (0x00)     /* Bus Lock Interrupt Enable bit                                 */
#define    C1CTR_OLIE        (0x00)     /* Overload Frame Transmit Interrupt Enable bit                  */
#define    C1CTR_BORIE       (0x00)     /* Bus Off Recovery Interrupt Enable bit                         */
#define    C1CTR_BOEIE       (0x00)     /* Bus Off Entry Interrupt Enable bit                            */
#define    C1CTR_EPIE        (0x00)     /* Error Passive Interrupt Enable bit                            */
#define    C1CTR_EWIE        (0x00)     /* Error Warning Interrupt Enable bit                            */
#define    C1CTR_BEIE        (0x00)     /* Bus Error Interrupt Enable bit                                */


/*=====================================*/
/* Channel CAN FD config               */
/*=====================================*/
/* For channel 0 */
/* RSCFD0CFDC0FDCFG : Channel CAN FD Configuration Register */
#define    C0FDCFG_REFE        (0x00)        /* Reception data edge filter enable bit       */
#define    C0FDCFG_FDOE        (0x00)        /* FD only enable bit                          */
#define    C0FDCFG_TMME        (0x00)        /* Transmit Buffer Merge Mode Enable bit       */
#define    C0FDCFG_GWBRS       (0x00)        /* Gateway BRS bit                             */
#define    C0FDCFG_GWFDF       (0x00)        /* Gateway FDF bit                             */
#define    C0FDCFG_GWEN        (0x00)        /* CAN-CAN FD Gateway Enable bit               */
#define    C0FDCFG_ESIC        (0x00)        /* Error State Display Mode Select bit         */
#define    C0FDCFG_EOCCFG      (0x00)        /* Error Occurrence Counting Method Select bit */

/* For channel 1 */
/* RSCFD0CFDC1FDCFG : Channel CAN FD Configuration Register */
#define    C1FDCFG_REFE        (0x00)        /* Reception data edge filter enable bit       */
#define    C1FDCFG_FDOE        (0x00)        /* FD only enable bit                          */
#define    C1FDCFG_TMME        (0x00)        /* Transmit Buffer Merge Mode Enable bit       */
#define    C1FDCFG_GWBRS       (0x00)        /* Gateway BRS bit                             */
#define    C1FDCFG_GWFDF       (0x00)        /* Gateway FDF bit                             */
#define    C1FDCFG_GWEN        (0x00)        /* CAN-CAN FD Gateway Enable bit               */
#define    C1FDCFG_ESIC        (0x00)        /* Error State Display Mode Select bit         */
#define    C1FDCFG_EOCCFG      (0x00)        /* Error Occurrence Counting Method Select bit */


/*=====================================*/
/* Receive Rule config                 */
/*=====================================*/
#define CAN0_RX_RULE_NUM    (16)   /* Set the number of receive rules exclusively used for channel 0. */
#define CAN1_RX_RULE_NUM    (16)   /* Set the number of receive rules exclusively used for channel 1. */

/*
   Set receive rules of each channel in succession.
   It is not possible to set channel 0 receive rules and channel 1 receive rules alternately.
*/

/*****************/
/* Rule No. 000  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_000           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_000           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_000            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_000            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_000           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_000           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_000            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_000           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_000           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_000           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_000          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_000    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_000    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_000           ((GAFLP1_GAFLFDP_TRFIFO_000 << 8) | GAFLP1_GAFLFDP_RXFIFO_000)

/*****************/
/* Rule No. 001  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_001           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_001           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_001            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_001            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_001           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_001           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_001            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_001           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_001           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_001           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_001          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_001    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_001    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_001           ((GAFLP1_GAFLFDP_TRFIFO_001 << 8) | GAFLP1_GAFLFDP_RXFIFO_001)

/*****************/
/* Rule No. 002  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_002           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_002           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_002            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_002            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_002           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_002           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_002            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_002           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_002           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_002           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_002          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_002    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_002    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_002           ((GAFLP1_GAFLFDP_TRFIFO_002 << 8) | GAFLP1_GAFLFDP_RXFIFO_002)

/*****************/
/* Rule No. 003  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_003           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_003           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_003            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_003            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_003           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_003           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_003            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_003           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_003           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_003           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_003          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_003    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_003    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_003           ((GAFLP1_GAFLFDP_TRFIFO_003 << 8) | GAFLP1_GAFLFDP_RXFIFO_003)

/*****************/
/* Rule No. 004  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_004           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_004           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_004            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_004            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_004           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_004           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_004            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_004           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_004           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_004           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_004          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_004    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_004    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_004           ((GAFLP1_GAFLFDP_TRFIFO_004 << 8) | GAFLP1_GAFLFDP_RXFIFO_004)

/*****************/
/* Rule No. 005  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_005           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_005           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_005            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_005            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_005           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_005           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_005            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_005           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_005           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_005           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_005          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_005    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_005    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_005           ((GAFLP1_GAFLFDP_TRFIFO_005 << 8) | GAFLP1_GAFLFDP_RXFIFO_005)

/*****************/
/* Rule No. 006  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_006           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_006           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_006            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_006            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_006           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_006           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_006            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_006           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_006           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_006           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_006          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_006    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_006    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_006           ((GAFLP1_GAFLFDP_TRFIFO_006 << 8) | GAFLP1_GAFLFDP_RXFIFO_006)

/*****************/
/* Rule No. 007  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_007           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_007           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_007            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_007            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_007           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_007           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_007            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_007           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_007           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_007           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_007          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_007    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_007    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_007           ((GAFLP1_GAFLFDP_TRFIFO_007 << 8) | GAFLP1_GAFLFDP_RXFIFO_007)

/*****************/
/* Rule No. 008  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_008           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_008           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_008            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_008            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_008           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_008           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_008            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_008           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_008           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_008           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_008          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_008    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_008    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_008           ((GAFLP1_GAFLFDP_TRFIFO_008 << 8) | GAFLP1_GAFLFDP_RXFIFO_008)

/*****************/
/* Rule No. 009  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_009           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_009           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_009            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_009            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_009           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_009           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_009            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_009           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_009           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_009           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_009          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_009    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_009    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_009           ((GAFLP1_GAFLFDP_TRFIFO_009 << 8) | GAFLP1_GAFLFDP_RXFIFO_009)

/*****************/
/* Rule No. 010  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_010           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_010           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_010            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_010            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_010           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_010           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_010            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_010           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_010           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_010           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_010          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_010    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_010    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_010           ((GAFLP1_GAFLFDP_TRFIFO_010 << 8) | GAFLP1_GAFLFDP_RXFIFO_010)

/*****************/
/* Rule No. 011  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_011           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_011           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_011            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_011            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_011           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_011           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_011            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_011           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_011           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_011           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_011          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_011    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_011    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_011           ((GAFLP1_GAFLFDP_TRFIFO_011 << 8) | GAFLP1_GAFLFDP_RXFIFO_011)

/*****************/
/* Rule No. 012  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_012           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_012           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_012            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_012            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_012           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_012           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_012            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_012           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_012           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_012           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_012          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_012    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_012    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_012           ((GAFLP1_GAFLFDP_TRFIFO_012 << 8) | GAFLP1_GAFLFDP_RXFIFO_012)

/*****************/
/* Rule No. 013  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_013           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_013           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_013            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_013            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_013           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_013           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_013            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_013           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_013           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_013           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_013          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_013    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_013    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_013           ((GAFLP1_GAFLFDP_TRFIFO_013 << 8) | GAFLP1_GAFLFDP_RXFIFO_013)

/*****************/
/* Rule No. 014  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_014           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_014           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_014            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_014            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_014           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_014           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_014            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_014           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_014           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_014           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_014          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_014    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_014    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_014           ((GAFLP1_GAFLFDP_TRFIFO_014 << 8) | GAFLP1_GAFLFDP_RXFIFO_014)

/*****************/
/* Rule No. 015  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_015           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_015           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_015            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_015            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_015           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_015           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_015            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_015           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_015           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_015           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_015          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_015    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_015    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_015           ((GAFLP1_GAFLFDP_TRFIFO_015 << 8) | GAFLP1_GAFLFDP_RXFIFO_015)

/*****************/
/* Rule No. 016  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_016           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_016           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_016            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_016            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_016           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_016           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_016            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_016           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_016           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_016           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_016          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_016    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_016    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_016           ((GAFLP1_GAFLFDP_TRFIFO_016 << 8) | GAFLP1_GAFLFDP_RXFIFO_016)

/*****************/
/* Rule No. 017  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_017           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_017           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_017            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_017            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_017           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_017           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_017            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_017           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_017           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_017           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_017          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_017    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_017    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_017           ((GAFLP1_GAFLFDP_TRFIFO_017 << 8) | GAFLP1_GAFLFDP_RXFIFO_017)

/*****************/
/* Rule No. 018  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_018           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_018           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_018            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_018            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_018           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_018           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_018            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_018           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_018           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_018           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_018          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_018    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_018    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_018           ((GAFLP1_GAFLFDP_TRFIFO_018 << 8) | GAFLP1_GAFLFDP_RXFIFO_018)

/*****************/
/* Rule No. 019  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_019           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_019           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_019            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_019            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_019           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_019           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_019            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_019           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_019           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_019           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_019          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_019    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_019    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_019           ((GAFLP1_GAFLFDP_TRFIFO_019 << 8) | GAFLP1_GAFLFDP_RXFIFO_019)

/*****************/
/* Rule No. 020  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_020           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_020           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_020            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_020            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_020           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_020           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_020            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_020           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_020           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_020           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_020          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_020    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_020    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_020           ((GAFLP1_GAFLFDP_TRFIFO_020 << 8) | GAFLP1_GAFLFDP_RXFIFO_020)

/*****************/
/* Rule No. 021  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_021           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_021           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_021            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_021            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_021           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_021           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_021            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_021           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_021           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_021           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_021          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_021    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_021    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_021           ((GAFLP1_GAFLFDP_TRFIFO_021 << 8) | GAFLP1_GAFLFDP_RXFIFO_021)

/*****************/
/* Rule No. 022  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_022           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_022           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_022            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_022            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_022           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_022           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_022            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_022           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_022           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_022           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_022          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_022    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_022    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_022           ((GAFLP1_GAFLFDP_TRFIFO_022 << 8) | GAFLP1_GAFLFDP_RXFIFO_022)

/*****************/
/* Rule No. 023  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_023           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_023           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_023            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_023            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_023           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_023           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_023            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_023           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_023           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_023           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_023          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_023    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_023    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_023           ((GAFLP1_GAFLFDP_TRFIFO_023 << 8) | GAFLP1_GAFLFDP_RXFIFO_023)

/*****************/
/* Rule No. 024  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_024           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_024           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_024            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_024            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_024           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_024           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_024            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_024           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_024           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_024           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_024          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_024    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_024    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_024           ((GAFLP1_GAFLFDP_TRFIFO_024 << 8) | GAFLP1_GAFLFDP_RXFIFO_024)

/*****************/
/* Rule No. 025  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_025           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_025           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_025            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_025            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_025           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_025           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_025            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_025           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_025           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_025           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_025          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_025    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_025    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_025           ((GAFLP1_GAFLFDP_TRFIFO_025 << 8) | GAFLP1_GAFLFDP_RXFIFO_025)

/*****************/
/* Rule No. 026  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_026           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_026           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_026            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_026            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_026           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_026           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_026            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_026           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_026           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_026           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_026          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_026    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_026    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_026           ((GAFLP1_GAFLFDP_TRFIFO_026 << 8) | GAFLP1_GAFLFDP_RXFIFO_026)

/*****************/
/* Rule No. 027  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_027           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_027           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_027            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_027            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_027           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_027           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_027            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_027           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_027           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_027           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_027          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_027    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_027    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_027           ((GAFLP1_GAFLFDP_TRFIFO_027 << 8) | GAFLP1_GAFLFDP_RXFIFO_027)

/*****************/
/* Rule No. 028  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_028           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_028           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_028            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_028            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_028           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_028           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_028            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_028           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_028           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_028           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_028          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_028    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_028    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_028           ((GAFLP1_GAFLFDP_TRFIFO_028 << 8) | GAFLP1_GAFLFDP_RXFIFO_028)

/*****************/
/* Rule No. 029  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_029           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_029           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_029            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_029            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_029           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_029           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_029            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_029           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_029           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_029           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_029          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_029    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_029    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_029           ((GAFLP1_GAFLFDP_TRFIFO_029 << 8) | GAFLP1_GAFLFDP_RXFIFO_029)

/*****************/
/* Rule No. 030  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_030           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_030           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_030            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_030            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_030           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_030           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_030            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_030           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_030           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_030           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_030          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_030    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_030    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_030           ((GAFLP1_GAFLFDP_TRFIFO_030 << 8) | GAFLP1_GAFLFDP_RXFIFO_030)

/*****************/
/* Rule No. 031  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_031           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_031           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_031            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_031            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_031           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_031           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_031            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_031           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_031           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_031           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_031          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_031    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_031    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_031           ((GAFLP1_GAFLFDP_TRFIFO_031 << 8) | GAFLP1_GAFLFDP_RXFIFO_031)

/*****************/
/* Rule No. 032  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_032           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_032           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_032            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_032            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_032           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_032           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_032            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_032           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_032           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_032           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_032          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_032    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_032    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_032           ((GAFLP1_GAFLFDP_TRFIFO_032 << 8) | GAFLP1_GAFLFDP_RXFIFO_032)

/*****************/
/* Rule No. 033  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_033           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_033           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_033            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_033            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_033           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_033           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_033            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_033           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_033           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_033           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_033          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_033    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_033    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_033           ((GAFLP1_GAFLFDP_TRFIFO_033 << 8) | GAFLP1_GAFLFDP_RXFIFO_033)

/*****************/
/* Rule No. 034  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_034           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_034           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_034            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_034            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_034           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_034           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_034            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_034           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_034           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_034           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_034          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_034    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_034    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_034           ((GAFLP1_GAFLFDP_TRFIFO_034 << 8) | GAFLP1_GAFLFDP_RXFIFO_034)

/*****************/
/* Rule No. 035  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_035           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_035           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_035            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_035            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_035           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_035           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_035            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_035           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_035           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_035           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_035          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_035    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_035    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_035           ((GAFLP1_GAFLFDP_TRFIFO_035 << 8) | GAFLP1_GAFLFDP_RXFIFO_035)

/*****************/
/* Rule No. 036  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_036           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_036           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_036            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_036            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_036           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_036           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_036            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_036           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_036           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_036           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_036          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_036    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_036    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_036           ((GAFLP1_GAFLFDP_TRFIFO_036 << 8) | GAFLP1_GAFLFDP_RXFIFO_036)

/*****************/
/* Rule No. 037  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_037           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_037           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_037            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_037            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_037           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_037           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_037            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_037           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_037           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_037           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_037          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_037    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_037    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_037           ((GAFLP1_GAFLFDP_TRFIFO_037 << 8) | GAFLP1_GAFLFDP_RXFIFO_037)

/*****************/
/* Rule No. 038  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_038           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_038           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_038            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_038            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_038           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_038           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_038            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_038           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_038           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_038           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_038          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_038    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_038    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_038           ((GAFLP1_GAFLFDP_TRFIFO_038 << 8) | GAFLP1_GAFLFDP_RXFIFO_038)

/*****************/
/* Rule No. 039  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_039           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_039           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_039            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_039            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_039           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_039           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_039            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_039           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_039           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_039           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_039          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_039    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_039    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_039           ((GAFLP1_GAFLFDP_TRFIFO_039 << 8) | GAFLP1_GAFLFDP_RXFIFO_039)

/*****************/
/* Rule No. 040  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_040           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_040           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_040            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_040            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_040           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_040           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_040            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_040           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_040           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_040           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_040          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_040    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_040    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_040           ((GAFLP1_GAFLFDP_TRFIFO_040 << 8) | GAFLP1_GAFLFDP_RXFIFO_040)

/*****************/
/* Rule No. 041  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_041           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_041           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_041            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_041            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_041           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_041           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_041            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_041           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_041           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_041           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_041          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_041    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_041    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_041           ((GAFLP1_GAFLFDP_TRFIFO_041 << 8) | GAFLP1_GAFLFDP_RXFIFO_041)

/*****************/
/* Rule No. 042  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_042           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_042           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_042            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_042            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_042           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_042           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_042            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_042           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_042           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_042           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_042          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_042    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_042    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_042           ((GAFLP1_GAFLFDP_TRFIFO_042 << 8) | GAFLP1_GAFLFDP_RXFIFO_042)

/*****************/
/* Rule No. 043  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_043           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_043           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_043            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_043            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_043           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_043           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_043            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_043           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_043           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_043           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_043          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_043    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_043    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_043           ((GAFLP1_GAFLFDP_TRFIFO_043 << 8) | GAFLP1_GAFLFDP_RXFIFO_043)

/*****************/
/* Rule No. 044  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_044           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_044           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_044            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_044            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_044           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_044           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_044            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_044           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_044           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_044           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_044          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_044    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_044    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_044           ((GAFLP1_GAFLFDP_TRFIFO_044 << 8) | GAFLP1_GAFLFDP_RXFIFO_044)

/*****************/
/* Rule No. 045  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_045           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_045           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_045            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_045            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_045           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_045           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_045            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_045           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_045           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_045           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_045          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_045    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_045    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_045           ((GAFLP1_GAFLFDP_TRFIFO_045 << 8) | GAFLP1_GAFLFDP_RXFIFO_045)

/*****************/
/* Rule No. 046  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_046           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_046           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_046            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_046            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_046           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_046           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_046            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_046           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_046           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_046           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_046          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_046    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_046    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_046           ((GAFLP1_GAFLFDP_TRFIFO_046 << 8) | GAFLP1_GAFLFDP_RXFIFO_046)

/*****************/
/* Rule No. 047  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_047           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_047           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_047            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_047            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_047           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_047           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_047            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_047           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_047           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_047           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_047          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_047    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_047    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_047           ((GAFLP1_GAFLFDP_TRFIFO_047 << 8) | GAFLP1_GAFLFDP_RXFIFO_047)

/*****************/
/* Rule No. 048  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_048           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_048           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_048            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_048            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_048           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_048           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_048            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_048           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_048           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_048           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_048          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_048    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_048    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_048           ((GAFLP1_GAFLFDP_TRFIFO_048 << 8) | GAFLP1_GAFLFDP_RXFIFO_048)

/*****************/
/* Rule No. 049  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_049           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_049           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_049            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_049            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_049           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_049           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_049            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_049           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_049           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_049           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_049          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_049    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_049    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_049           ((GAFLP1_GAFLFDP_TRFIFO_049 << 8) | GAFLP1_GAFLFDP_RXFIFO_049)

/*****************/
/* Rule No. 050  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_050           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_050           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_050            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_050            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_050           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_050           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_050            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_050           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_050           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_050           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_050          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_050    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_050    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_050           ((GAFLP1_GAFLFDP_TRFIFO_050 << 8) | GAFLP1_GAFLFDP_RXFIFO_050)

/*****************/
/* Rule No. 051  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_051           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_051           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_051            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_051            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_051           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_051           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_051            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_051           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_051           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_051           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_051          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_051    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_051    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_051           ((GAFLP1_GAFLFDP_TRFIFO_051 << 8) | GAFLP1_GAFLFDP_RXFIFO_051)

/*****************/
/* Rule No. 052  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_052           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_052           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_052            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_052            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_052           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_052           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_052            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_052           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_052           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_052           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_052          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_052    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_052    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_052           ((GAFLP1_GAFLFDP_TRFIFO_052 << 8) | GAFLP1_GAFLFDP_RXFIFO_052)

/*****************/
/* Rule No. 053  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_053           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_053           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_053            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_053            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_053           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_053           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_053            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_053           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_053           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_053           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_053          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_053    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_053    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_053           ((GAFLP1_GAFLFDP_TRFIFO_053 << 8) | GAFLP1_GAFLFDP_RXFIFO_053)

/*****************/
/* Rule No. 054  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_054           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_054           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_054            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_054            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_054           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_054           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_054            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_054           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_054           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_054           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_054          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_054    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_054    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_054           ((GAFLP1_GAFLFDP_TRFIFO_054 << 8) | GAFLP1_GAFLFDP_RXFIFO_054)

/*****************/
/* Rule No. 055  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_055           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_055           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_055            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_055            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_055           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_055           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_055            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_055           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_055           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_055           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_055          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_055    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_055    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_055           ((GAFLP1_GAFLFDP_TRFIFO_055 << 8) | GAFLP1_GAFLFDP_RXFIFO_055)

/*****************/
/* Rule No. 056  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_056           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_056           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_056            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_056            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_056           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_056           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_056            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_056           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_056           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_056           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_056          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_056    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_056    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_056           ((GAFLP1_GAFLFDP_TRFIFO_056 << 8) | GAFLP1_GAFLFDP_RXFIFO_056)

/*****************/
/* Rule No. 057  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_057           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_057           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_057            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_057            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_057           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_057           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_057            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_057           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_057           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_057           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_057          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_057    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_057    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_057           ((GAFLP1_GAFLFDP_TRFIFO_057 << 8) | GAFLP1_GAFLFDP_RXFIFO_057)

/*****************/
/* Rule No. 058  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_058           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_058           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_058            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_058            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_058           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_058           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_058            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_058           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_058           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_058           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_058          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_058    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_058    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_058           ((GAFLP1_GAFLFDP_TRFIFO_058 << 8) | GAFLP1_GAFLFDP_RXFIFO_058)

/*****************/
/* Rule No. 059  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_059           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_059           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_059            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_059            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_059           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_059           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_059            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_059           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_059           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_059           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_059          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_059    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_059    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_059           ((GAFLP1_GAFLFDP_TRFIFO_059 << 8) | GAFLP1_GAFLFDP_RXFIFO_059)

/*****************/
/* Rule No. 060  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_060           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_060           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_060            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_060            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_060           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_060           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_060            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_060           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_060           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_060           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_060          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_060    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_060    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_060           ((GAFLP1_GAFLFDP_TRFIFO_060 << 8) | GAFLP1_GAFLFDP_RXFIFO_060)

/*****************/
/* Rule No. 061  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_061           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_061           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_061            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_061            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_061           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_061           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_061            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_061           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_061           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_061           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_061          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_061    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_061    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_061           ((GAFLP1_GAFLFDP_TRFIFO_061 << 8) | GAFLP1_GAFLFDP_RXFIFO_061)

/*****************/
/* Rule No. 062  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_062           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_062           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_062            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_062            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_062           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_062           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_062            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_062           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_062           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_062           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_062          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_062    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_062    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_062           ((GAFLP1_GAFLFDP_TRFIFO_062 << 8) | GAFLP1_GAFLFDP_RXFIFO_062)

/*****************/
/* Rule No. 063  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_063           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_063           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_063            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_063            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_063           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_063           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_063            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_063           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_063           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_063           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_063          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_063    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_063    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_063           ((GAFLP1_GAFLFDP_TRFIFO_063 << 8) | GAFLP1_GAFLFDP_RXFIFO_063)

/*****************/
/* Rule No. 064  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_064           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_064           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_064            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_064            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_064           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_064           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_064            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_064           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_064           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_064           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_064          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_064    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_064    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_064           ((GAFLP1_GAFLFDP_TRFIFO_064 << 8) | GAFLP1_GAFLFDP_RXFIFO_064)

/*****************/
/* Rule No. 065  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_065           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_065           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_065            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_065            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_065           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_065           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_065            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_065           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_065           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_065           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_065          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_065    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_065    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_065           ((GAFLP1_GAFLFDP_TRFIFO_065 << 8) | GAFLP1_GAFLFDP_RXFIFO_065)

/*****************/
/* Rule No. 066  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_066           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_066           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_066            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_066            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_066           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_066           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_066            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_066           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_066           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_066           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_066          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_066    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_066    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_066           ((GAFLP1_GAFLFDP_TRFIFO_066 << 8) | GAFLP1_GAFLFDP_RXFIFO_066)

/*****************/
/* Rule No. 067  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_067           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_067           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_067            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_067            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_067           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_067           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_067            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_067           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_067           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_067           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_067          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_067    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_067    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_067           ((GAFLP1_GAFLFDP_TRFIFO_067 << 8) | GAFLP1_GAFLFDP_RXFIFO_067)

/*****************/
/* Rule No. 068  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_068           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_068           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_068            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_068            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_068           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_068           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_068            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_068           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_068           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_068           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_068          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_068    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_068    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_068           ((GAFLP1_GAFLFDP_TRFIFO_068 << 8) | GAFLP1_GAFLFDP_RXFIFO_068)

/*****************/
/* Rule No. 069  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_069           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_069           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_069            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_069            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_069           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_069           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_069            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_069           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_069           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_069           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_069          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_069    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_069    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_069           ((GAFLP1_GAFLFDP_TRFIFO_069 << 8) | GAFLP1_GAFLFDP_RXFIFO_069)

/*****************/
/* Rule No. 070  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_070           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_070           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_070            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_070            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_070           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_070           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_070            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_070           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_070           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_070           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_070          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_070    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_070    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_070           ((GAFLP1_GAFLFDP_TRFIFO_070 << 8) | GAFLP1_GAFLFDP_RXFIFO_070)

/*****************/
/* Rule No. 071  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_071           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_071           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_071            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_071            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_071           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_071           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_071            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_071           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_071           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_071           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_071          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_071    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_071    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_071           ((GAFLP1_GAFLFDP_TRFIFO_071 << 8) | GAFLP1_GAFLFDP_RXFIFO_071)

/*****************/
/* Rule No. 072  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_072           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_072           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_072            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_072            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_072           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_072           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_072            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_072           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_072           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_072           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_072          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_072    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_072    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_072           ((GAFLP1_GAFLFDP_TRFIFO_072 << 8) | GAFLP1_GAFLFDP_RXFIFO_072)

/*****************/
/* Rule No. 073  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_073           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_073           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_073            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_073            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_073           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_073           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_073            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_073           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_073           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_073           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_073          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_073    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_073    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_073           ((GAFLP1_GAFLFDP_TRFIFO_073 << 8) | GAFLP1_GAFLFDP_RXFIFO_073)

/*****************/
/* Rule No. 074  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_074           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_074           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_074            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_074            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_074           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_074           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_074            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_074           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_074           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_074           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_074          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_074    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_074    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_074           ((GAFLP1_GAFLFDP_TRFIFO_074 << 8) | GAFLP1_GAFLFDP_RXFIFO_074)

/*****************/
/* Rule No. 075  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_075           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_075           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_075            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_075            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_075           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_075           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_075            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_075           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_075           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_075           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_075          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_075    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_075    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_075           ((GAFLP1_GAFLFDP_TRFIFO_075 << 8) | GAFLP1_GAFLFDP_RXFIFO_075)

/*****************/
/* Rule No. 076  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_076           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_076           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_076            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_076            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_076           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_076           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_076            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_076           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_076           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_076           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_076          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_076    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_076    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_076           ((GAFLP1_GAFLFDP_TRFIFO_076 << 8) | GAFLP1_GAFLFDP_RXFIFO_076)

/*****************/
/* Rule No. 077  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_077           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_077           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_077            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_077            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_077           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_077           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_077            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_077           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_077           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_077           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_077          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_077    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_077    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_077           ((GAFLP1_GAFLFDP_TRFIFO_077 << 8) | GAFLP1_GAFLFDP_RXFIFO_077)

/*****************/
/* Rule No. 078  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_078           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_078           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_078            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_078            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_078           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_078           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_078            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_078           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_078           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_078           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_078          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_078    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_078    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_078           ((GAFLP1_GAFLFDP_TRFIFO_078 << 8) | GAFLP1_GAFLFDP_RXFIFO_078)

/*****************/
/* Rule No. 079  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_079           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_079           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_079            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_079            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_079           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_079           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_079            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_079           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_079           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_079           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_079          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_079    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_079    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_079           ((GAFLP1_GAFLFDP_TRFIFO_079 << 8) | GAFLP1_GAFLFDP_RXFIFO_079)

/*****************/
/* Rule No. 080  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_080           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_080           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_080            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_080            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_080           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_080           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_080            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_080           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_080           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_080           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_080          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_080    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_080    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_080           ((GAFLP1_GAFLFDP_TRFIFO_080 << 8) | GAFLP1_GAFLFDP_RXFIFO_080)

/*****************/
/* Rule No. 081  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_081           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_081           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_081            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_081            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_081           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_081           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_081            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_081           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_081           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_081           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_081          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_081    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_081    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_081           ((GAFLP1_GAFLFDP_TRFIFO_081 << 8) | GAFLP1_GAFLFDP_RXFIFO_081)

/*****************/
/* Rule No. 082  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_082           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_082           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_082            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_082            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_082           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_082           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_082            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_082           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_082           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_082           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_082          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_082    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_082    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_082           ((GAFLP1_GAFLFDP_TRFIFO_082 << 8) | GAFLP1_GAFLFDP_RXFIFO_082)

/*****************/
/* Rule No. 083  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_083           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_083           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_083            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_083            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_083           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_083           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_083            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_083           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_083           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_083           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_083          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_083    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_083    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_083           ((GAFLP1_GAFLFDP_TRFIFO_083 << 8) | GAFLP1_GAFLFDP_RXFIFO_083)

/*****************/
/* Rule No. 084  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_084           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_084           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_084            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_084            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_084           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_084           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_084            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_084           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_084           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_084           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_084          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_084    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_084    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_084           ((GAFLP1_GAFLFDP_TRFIFO_084 << 8) | GAFLP1_GAFLFDP_RXFIFO_084)

/*****************/
/* Rule No. 085  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_085           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_085           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_085            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_085            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_085           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_085           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_085            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_085           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_085           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_085           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_085          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_085    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_085    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_085           ((GAFLP1_GAFLFDP_TRFIFO_085 << 8) | GAFLP1_GAFLFDP_RXFIFO_085)

/*****************/
/* Rule No. 086  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_086           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_086           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_086            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_086            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_086           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_086           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_086            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_086           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_086           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_086           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_086          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_086    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_086    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_086           ((GAFLP1_GAFLFDP_TRFIFO_086 << 8) | GAFLP1_GAFLFDP_RXFIFO_086)

/*****************/
/* Rule No. 087  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_087           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_087           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_087            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_087            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_087           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_087           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_087            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_087           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_087           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_087           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_087          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_087    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_087    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_087           ((GAFLP1_GAFLFDP_TRFIFO_087 << 8) | GAFLP1_GAFLFDP_RXFIFO_087)

/*****************/
/* Rule No. 088  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_088           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_088           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_088            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_088            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_088           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_088           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_088            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_088           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_088           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_088           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_088          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_088    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_088    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_088           ((GAFLP1_GAFLFDP_TRFIFO_088 << 8) | GAFLP1_GAFLFDP_RXFIFO_088)

/*****************/
/* Rule No. 089  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_089           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_089           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_089            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_089            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_089           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_089           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_089            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_089           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_089           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_089           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_089          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_089    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_089    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_089           ((GAFLP1_GAFLFDP_TRFIFO_089 << 8) | GAFLP1_GAFLFDP_RXFIFO_089)

/*****************/
/* Rule No. 090  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_090           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_090           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_090            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_090            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_090           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_090           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_090            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_090           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_090           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_090           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_090          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_090    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_090    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_090           ((GAFLP1_GAFLFDP_TRFIFO_090 << 8) | GAFLP1_GAFLFDP_RXFIFO_090)

/*****************/
/* Rule No. 091  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_091           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_091           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_091            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_091            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_091           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_091           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_091            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_091           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_091           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_091           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_091          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_091    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_091    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_091           ((GAFLP1_GAFLFDP_TRFIFO_091 << 8) | GAFLP1_GAFLFDP_RXFIFO_091)

/*****************/
/* Rule No. 092  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_092           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_092           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_092            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_092            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_092           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_092           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_092            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_092           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_092           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_092           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_092          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_092    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_092    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_092           ((GAFLP1_GAFLFDP_TRFIFO_092 << 8) | GAFLP1_GAFLFDP_RXFIFO_092)

/*****************/
/* Rule No. 093  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_093           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_093           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_093            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_093            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_093           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_093           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_093            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_093           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_093           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_093           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_093          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_093    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_093    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_093           ((GAFLP1_GAFLFDP_TRFIFO_093 << 8) | GAFLP1_GAFLFDP_RXFIFO_093)

/*****************/
/* Rule No. 094  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_094           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_094           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_094            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_094            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_094           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_094           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_094            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_094           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_094           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_094           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_094          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_094    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_094    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_094           ((GAFLP1_GAFLFDP_TRFIFO_094 << 8) | GAFLP1_GAFLFDP_RXFIFO_094)

/*****************/
/* Rule No. 095  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_095           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_095           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_095            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_095            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_095           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_095           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_095            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_095           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_095           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_095           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_095          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_095    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_095    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_095           ((GAFLP1_GAFLFDP_TRFIFO_095 << 8) | GAFLP1_GAFLFDP_RXFIFO_095)

/*****************/
/* Rule No. 096  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_096           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_096           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_096            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_096            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_096           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_096           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_096            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_096           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_096           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_096           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_096          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_096    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_096    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_096           ((GAFLP1_GAFLFDP_TRFIFO_096 << 8) | GAFLP1_GAFLFDP_RXFIFO_096)

/*****************/
/* Rule No. 097  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_097           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_097           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_097            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_097            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_097           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_097           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_097            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_097           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_097           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_097           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_097          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_097    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_097    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_097           ((GAFLP1_GAFLFDP_TRFIFO_097 << 8) | GAFLP1_GAFLFDP_RXFIFO_097)

/*****************/
/* Rule No. 098  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_098           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_098           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_098            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_098            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_098           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_098           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_098            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_098           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_098           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_098           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_098          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_098    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_098    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_098           ((GAFLP1_GAFLFDP_TRFIFO_098 << 8) | GAFLP1_GAFLFDP_RXFIFO_098)

/*****************/
/* Rule No. 099  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_099           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_099           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_099            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_099            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_099           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_099           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_099            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_099           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_099           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_099           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_099          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_099    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_099    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_099           ((GAFLP1_GAFLFDP_TRFIFO_099 << 8) | GAFLP1_GAFLFDP_RXFIFO_099)

/*****************/
/* Rule No. 100  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_100           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_100           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_100            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_100            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_100           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_100           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_100            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_100           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_100           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_100           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_100          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_100    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_100    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_100           ((GAFLP1_GAFLFDP_TRFIFO_100 << 8) | GAFLP1_GAFLFDP_RXFIFO_100)

/*****************/
/* Rule No. 101  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_101           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_101           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_101            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_101            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_101           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_101           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_101            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_101           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_101           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_101           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_101          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_101    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_101    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_101           ((GAFLP1_GAFLFDP_TRFIFO_101 << 8) | GAFLP1_GAFLFDP_RXFIFO_101)

/*****************/
/* Rule No. 102  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_102           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_102           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_102            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_102            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_102           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_102           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_102            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_102           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_102           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_102           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_102          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_102    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_102    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_102           ((GAFLP1_GAFLFDP_TRFIFO_102 << 8) | GAFLP1_GAFLFDP_RXFIFO_102)

/*****************/
/* Rule No. 103  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_103           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_103           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_103            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_103            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_103           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_103           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_103            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_103           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_103           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_103           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_103          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_103    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_103    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_103           ((GAFLP1_GAFLFDP_TRFIFO_103 << 8) | GAFLP1_GAFLFDP_RXFIFO_103)

/*****************/
/* Rule No. 104  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_104           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_104           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_104            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_104            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_104           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_104           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_104            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_104           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_104           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_104           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_104          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_104    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_104    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_104           ((GAFLP1_GAFLFDP_TRFIFO_104 << 8) | GAFLP1_GAFLFDP_RXFIFO_104)

/*****************/
/* Rule No. 105  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_105           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_105           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_105            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_105            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_105           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_105           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_105            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_105           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_105           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_105           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_105          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_105    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_105    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_105           ((GAFLP1_GAFLFDP_TRFIFO_105 << 8) | GAFLP1_GAFLFDP_RXFIFO_105)

/*****************/
/* Rule No. 106  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_106           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_106           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_106            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_106            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_106           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_106           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_106            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_106           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_106           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_106           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_106          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_106    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_106    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_106           ((GAFLP1_GAFLFDP_TRFIFO_106 << 8) | GAFLP1_GAFLFDP_RXFIFO_106)

/*****************/
/* Rule No. 107  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_107           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_107           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_107            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_107            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_107           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_107           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_107            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_107           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_107           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_107           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_107          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_107    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_107    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_107           ((GAFLP1_GAFLFDP_TRFIFO_107 << 8) | GAFLP1_GAFLFDP_RXFIFO_107)

/*****************/
/* Rule No. 108  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_108           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_108           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_108            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_108            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_108           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_108           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_108            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_108           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_108           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_108           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_108          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_108    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_108    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_108           ((GAFLP1_GAFLFDP_TRFIFO_108 << 8) | GAFLP1_GAFLFDP_RXFIFO_108)

/*****************/
/* Rule No. 109  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_109           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_109           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_109            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_109            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_109           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_109           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_109            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_109           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_109           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_109           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_109          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_109    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_109    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_109           ((GAFLP1_GAFLFDP_TRFIFO_109 << 8) | GAFLP1_GAFLFDP_RXFIFO_109)

/*****************/
/* Rule No. 110  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_110           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_110           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_110            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_110            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_110           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_110           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_110            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_110           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_110           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_110           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_110          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_110    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_110    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_110           ((GAFLP1_GAFLFDP_TRFIFO_110 << 8) | GAFLP1_GAFLFDP_RXFIFO_110)

/*****************/
/* Rule No. 111  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_111           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_111           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_111            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_111            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_111           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_111           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_111            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_111           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_111           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_111           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_111          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_111    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_111    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_111           ((GAFLP1_GAFLFDP_TRFIFO_111 << 8) | GAFLP1_GAFLFDP_RXFIFO_111)

/*****************/
/* Rule No. 112  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_112           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_112           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_112            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_112            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_112           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_112           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_112            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_112           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_112           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_112           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_112          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_112    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_112    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_112           ((GAFLP1_GAFLFDP_TRFIFO_112 << 8) | GAFLP1_GAFLFDP_RXFIFO_112)

/*****************/
/* Rule No. 113  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_113           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_113           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_113            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_113            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_113           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_113           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_113            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_113           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_113           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_113           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_113          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_113    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_113    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_113           ((GAFLP1_GAFLFDP_TRFIFO_113 << 8) | GAFLP1_GAFLFDP_RXFIFO_113)

/*****************/
/* Rule No. 114  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_114           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_114           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_114            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_114            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_114           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_114           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_114            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_114           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_114           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_114           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_114          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_114    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_114    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_114           ((GAFLP1_GAFLFDP_TRFIFO_114 << 8) | GAFLP1_GAFLFDP_RXFIFO_114)

/*****************/
/* Rule No. 115  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_115           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_115           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_115            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_115            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_115           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_115           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_115            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_115           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_115           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_115           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_115          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_115    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_115    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_115           ((GAFLP1_GAFLFDP_TRFIFO_115 << 8) | GAFLP1_GAFLFDP_RXFIFO_115)

/*****************/
/* Rule No. 116  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_116           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_116           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_116            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_116            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_116           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_116           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_116            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_116           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_116           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_116           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_116          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_116    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_116    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_116           ((GAFLP1_GAFLFDP_TRFIFO_116 << 8) | GAFLP1_GAFLFDP_RXFIFO_116)

/*****************/
/* Rule No. 117  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_117           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_117           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_117            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_117            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_117           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_117           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_117            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_117           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_117           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_117           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_117          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_117    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_117    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_117           ((GAFLP1_GAFLFDP_TRFIFO_117 << 8) | GAFLP1_GAFLFDP_RXFIFO_117)

/*****************/
/* Rule No. 118  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_118           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_118           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_118            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_118            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_118           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_118           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_118            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_118           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_118           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_118           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_118          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_118    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_118    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_118           ((GAFLP1_GAFLFDP_TRFIFO_118 << 8) | GAFLP1_GAFLFDP_RXFIFO_118)

/*****************/
/* Rule No. 119  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_119           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_119           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_119            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_119            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_119           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_119           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_119            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_119           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_119           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_119           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_119          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_119    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_119    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_119           ((GAFLP1_GAFLFDP_TRFIFO_119 << 8) | GAFLP1_GAFLFDP_RXFIFO_119)

/*****************/
/* Rule No. 120  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_120           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_120           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_120            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_120            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_120           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_120           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_120            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_120           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_120           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_120           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_120          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_120    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_120    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_120           ((GAFLP1_GAFLFDP_TRFIFO_120 << 8) | GAFLP1_GAFLFDP_RXFIFO_120)

/*****************/
/* Rule No. 121  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_121           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_121           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_121            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_121            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_121           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_121           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_121            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_121           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_121           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_121           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_121          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_121    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_121    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_121           ((GAFLP1_GAFLFDP_TRFIFO_121 << 8) | GAFLP1_GAFLFDP_RXFIFO_121)

/*****************/
/* Rule No. 122  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_122           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_122           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_122            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_122            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_122           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_122           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_122            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_122           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_122           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_122           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_122          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_122    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_122    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_122           ((GAFLP1_GAFLFDP_TRFIFO_122 << 8) | GAFLP1_GAFLFDP_RXFIFO_122)

/*****************/
/* Rule No. 123  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_123           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_123           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_123            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_123            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_123           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_123           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_123            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_123           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_123           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_123           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_123          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_123    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_123    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_123           ((GAFLP1_GAFLFDP_TRFIFO_123 << 8) | GAFLP1_GAFLFDP_RXFIFO_123)

/*****************/
/* Rule No. 124  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_124           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_124           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_124            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_124            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_124           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_124           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_124            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_124           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_124           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_124           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_124          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_124    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_124    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_124           ((GAFLP1_GAFLFDP_TRFIFO_124 << 8) | GAFLP1_GAFLFDP_RXFIFO_124)

/*****************/
/* Rule No. 125  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_125           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_125           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_125            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_125            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_125           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_125           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_125            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_125           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_125           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_125           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_125          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_125    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_125    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_125           ((GAFLP1_GAFLFDP_TRFIFO_125 << 8) | GAFLP1_GAFLFDP_RXFIFO_125)

/*****************/
/* Rule No. 126  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_126           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_126           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_126            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_126            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_126           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_126           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_126            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_126           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_126           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_126           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_126          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_126    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_126    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_126           ((GAFLP1_GAFLFDP_TRFIFO_126 << 8) | GAFLP1_GAFLFDP_RXFIFO_126)

/*****************/
/* Rule No. 127  */
/*****************/
/* RSCFDnCFDGAFLIDj : Receive Rule ID Register */
#define    GAFLID_GAFLIDE_127           (0x00)        /* IDE Select bit                            */
#define    GAFLID_GAFLRTR_127           (0x00)        /* RTR Select bit                            */
#define    GAFLID_GAFLLB_127            (0x00)        /* Receive Rule Target Message Select bit    */
#define    GAFLID_GAFLID_127            (0x00000000)  /* ID bit (0x0 - 0x1FFFFFFF)                 */
/* RSCFDnCFDGAFLMj : Receive Rule Mask Register */
#define    GAFLM_GAFLIDEM_127           (0x00)        /* IDE Mask bit                              */
#define    GAFLM_GAFLRTRM_127           (0x00)        /* RTR Mask bit                              */
#define    GAFLM_GAFLIDM_127            (0x00000000)  /* ID Mask bit (0x0 - 0x1FFFFFFF)            */
/* RSCFDnCFDGAFLP0_j : Receive Rule Pointer 0 Register */
#define    GAFLP0_GAFLDLC_127           (0x00)        /* Receive Rule DLC bit                      */
#define    GAFLP0_GAFLPTR_127           (0x0000)      /* Receive Rule Label bit                    */
#define    GAFLP0_GAFLRMV_127           (0x00)        /* Receive Buffer Enable bit                 */
#define    GAFLP0_GAFLRMDP_127          (0x00)        /* Receive Buffer Number Select bit          */
/* RSCFDnCFDGAFLP1_j : Receive Rule Pointer 1 Register */
#define    GAFLP1_GAFLFDP_TRFIFO_127    (0x00)        /* Transmit/Receive FIFO Buffer k Select bit */
#define    GAFLP1_GAFLFDP_RXFIFO_127    (0x00)        /* Receive FIFO Buffer x Select bit          */
#define    GAFLP1_GAFLFDP_127           ((GAFLP1_GAFLFDP_TRFIFO_127 << 8) | GAFLP1_GAFLFDP_RXFIFO_127)



/**************************************************
* R_CAN_SetBitrate function config
***************************************************/
/*=====================================*/
/*  Bitrate config                     */
/*=====================================*/

/**************************************************************************************
*
*  Bitrate config
*  See 'CAN Communication Speed Setting', and 'Bit Rate' sections in HW-manual.
*
*  FCAN = clkc or clk_xincan.
*  FCANTQ = FCAN / CnNCFG_mBRP (n:0, 1, m:N, D)
*  P = value selected in NBRP[9:0] bits in BCR (P = 0 to 1023). P + 1 = CnNCFG_mBRP.
*  mTQTOT =  Nr CAN clocks in one CAN bit = FCANTQ/BITRATE.
*
*  mTQTOT = FCAN/(CnNCFG_mBRP * BITRATE)          (Eq. 1)
*  Also,
*  mTQTOT = CnNCFG_mTSEG1 + CnNCFG_mTSEG2 + SS    (Eq. 2)
*   (condition: CnNCFG_mTSEG1 > CnNCFG_mTSEG2)
*
***************************************************************************************/
/*  Example A). Desired Nominal baudrate 1 Mbps.
 *  Try C0NCFG_NBRP = 0(= 1). Equation 1:
 *  TQTOT = (36000000)/(1000000 * 1) = 36.
 *
 *  Try Equation 2:
 *      SS = 1 Tq always.
 *      C1NCFG_NTSEG1 = 17 (= 18 Tq)
 *      C1NCFG_NTSEG2 = 12 (= 13 Tq)
 *      ============
 *      SUM = 36 Tq
 */

/* For channel 0 */
/* RSCFD0CFDC0NCFG : Channel Nominal Bit Rate Configuration Register */
#define    C0NCFG_NTSEG2       (0x0C)         /* Nominal Bit Rate Time Segment 2 Control bit               */ /* Changed */
#define    C0NCFG_NTSEG1       (0x11)         /* Nominal Bit Rate Time Segment 1 Control bit               */ /* Changed */
#define    C0NCFG_NSJW         (0x02)         /* Nominal Bit Rate Resynchronization Jump Width Control bit */ /* Changed */
#define    C0NCFG_NBRP         (0x0000)       /* Nominal Bit Rate Prescaler Division Ratio Setting bit     */

/* RSCFD0CFDC0DCFG : Channel Data Bit Rate Configuration Register */
#define    C0DCFG_DSJW         (0x02)         /* Data Bit Rate Resynchronization Jump Width Control bit     */ /* Changed */
#define    C0DCFG_DTSEG2       (0x05)         /* Data Bit Rate Time Segment 2 Control bit                   */ /* Changed */
#define    C0DCFG_DTSEG1       (0x08)         /* Data Bit Rate Time Segment 1 Control bit                   */ /* Changed */
#define    C0DCFG_DBRP         (0x00)         /* Data Bit Rate Prescaler Division Ratio Setting bit         */

/* For channel 1 */
/* RSCFD0CFDC1NCFG : Channel Nominal Bit Rate Configuration Register */
#define    C1NCFG_NTSEG2       (0x0C)         /* Nominal Bit Rate Time Segment 2 Control bit                */
#define    C1NCFG_NTSEG1       (0x11)         /* Nominal Bit Rate Time Segment 1 Control bit                */
#define    C1NCFG_NSJW         (0x02)         /* Nominal Bit Rate Resynchronization Jump Width Control bit  */
#define    C1NCFG_NBRP         (0x0000)       /* Nominal Bit Rate Prescaler Division Ratio Setting bit      */

/* RSCFD0CFDC0DCFG : Channel Data Bit Rate Configuration Register */
#define    C1DCFG_DSJW         (0x02)         /* Data Bit Rate Resynchronization Jump Width Control bit     */
#define    C1DCFG_DTSEG2       (0x05)         /* Data Bit Rate Time Segment 2 Control bit                   */
#define    C1DCFG_DTSEG1       (0x08)         /* Data Bit Rate Time Segment 1 Control bit                   */
#define    C1DCFG_DBRP         (0x00)         /* Data Bit Rate Prescaler Division Ratio Setting bit         */


/*========================================*/
/*  Transmitter Delay Compensation config */
/*========================================*/
/* For channel 0 */
/* RSCFD0CFDC0FDCFG : Channel CAN FD Configuration Register */
#define    C0FDCFG_TDCO        (0x00)         /* Transmitter Delay Compensation Offset bit                  */
#define    C0FDCFG_TDCE        (0x00)         /* Transmitter Delay Compensation Enable bit                  */
#define    C0FDCFG_TDCOC       (0x00)         /* Transmitter Delay Compensation Measurement Select bit      */

/* For channel 1 */
/* RSCFD0CFDC1FDCFG : Channel CAN FD Configuration Register */
#define    C1FDCFG_TDCO        (0x00)         /* Transmitter Delay Compensation Offset bit                  */
#define    C1FDCFG_TDCE        (0x00)         /* Transmitter Delay Compensation Enable bit                  */
#define    C1FDCFG_TDCOC       (0x00)         /* Transmitter Delay Compensation Measurement Select bit      */



/* Need not change the following code */
#define    CAN_GCFG        (\
    (GCFG_ITRCP    << RCANFD_RSCFD0CFDGCFG_ITRCP_SHIFT)  |\
    (GCFG_TSBTCS   << RCANFD_RSCFD0CFDGCFG_TSBTCS_SHIFT) |\
    (GCFG_TSSS     << RCANFD_RSCFD0CFDGCFG_TSSS_SHIFT)   |\
    (GCFG_TSP      << RCANFD_RSCFD0CFDGCFG_TSP_SHIFT)    |\
    (GCFG_CMPOC    << RCANFD_RSCFD0CFDGCFG_CMPOC_SHIFT)  |\
    (GCFG_DCS      << RCANFD_RSCFD0CFDGCFG_DCS_SHIFT)    |\
    (GCFG_MME      << RCANFD_RSCFD0CFDGCFG_MME_SHIFT)    |\
    (GCFG_DRE      << RCANFD_RSCFD0CFDGCFG_DRE_SHIFT)    |\
    (GCFG_DCE      << RCANFD_RSCFD0CFDGCFG_DCE_SHIFT)    |\
    (GCFG_TPRI     << RCANFD_RSCFD0CFDGCFG_TPRI_SHIFT)    \
)

#define    CAN_GFDCFG        (\
    (GFDCFG_TSCCFG     << RCANFD_RSCFD0CFDGFDCFG_TSCCFG_SHIFT) |\
    (GFDCFG_RPED       << RCANFD_RSCFD0CFDGFDCFG_RPED_SHIFT) \
)

#define    CAN_C0NCFG        (\
    (C0NCFG_NTSEG2   << RCANFD_RSCFD0CFDC0NCFG_NTSEG2_SHIFT) |\
    (C0NCFG_NTSEG1   << RCANFD_RSCFD0CFDC0NCFG_NTSEG1_SHIFT) |\
    (C0NCFG_NSJW     << RCANFD_RSCFD0CFDC0NCFG_NSJW_SHIFT)   |\
    (C0NCFG_NBRP     << RCANFD_RSCFD0CFDC0NCFG_NBRP_SHIFT)    \
)

#define    CAN_C1NCFG        (\
    (C1NCFG_NTSEG2   << RCANFD_RSCFD0CFDC1NCFG_NTSEG2_SHIFT) |\
    (C1NCFG_NTSEG1   << RCANFD_RSCFD0CFDC1NCFG_NTSEG1_SHIFT) |\
    (C1NCFG_NSJW     << RCANFD_RSCFD0CFDC1NCFG_NSJW_SHIFT)   |\
    (C1NCFG_NBRP     << RCANFD_RSCFD0CFDC1NCFG_NBRP_SHIFT)    \
)

#define    CAN_C0DCFG        (\
    (C0DCFG_DSJW    << RCANFD_RSCFD0CFDC0DCFG_DSJW_SHIFT)   |\
    (C0DCFG_DTSEG2  << RCANFD_RSCFD0CFDC0DCFG_DTSEG2_SHIFT) |\
    (C0DCFG_DTSEG1  << RCANFD_RSCFD0CFDC0DCFG_DTSEG1_SHIFT) |\
    (C0DCFG_DBRP    << RCANFD_RSCFD0CFDC0DCFG_DBRP_SHIFT)    \
)

#define    CAN_C1DCFG        (\
    (C1DCFG_DSJW    << RCANFD_RSCFD0CFDC1DCFG_DSJW_SHIFT)   |\
    (C1DCFG_DTSEG2  << RCANFD_RSCFD0CFDC1DCFG_DTSEG2_SHIFT) |\
    (C1DCFG_DTSEG1  << RCANFD_RSCFD0CFDC1DCFG_DTSEG1_SHIFT) |\
    (C1DCFG_DBRP    << RCANFD_RSCFD0CFDC1DCFG_DBRP_SHIFT)    \
)

#define    CAN_C0FDCFG        (\
    (C0FDCFG_REFE   << RCANFD_RSCFD0CFDC0FDCFG_REFE_SHIFT)  |\
    (C0FDCFG_FDOE   << RCANFD_RSCFD0CFDC0FDCFG_FDOE_SHIFT)  |\
    (C0FDCFG_TMME   << RCANFD_RSCFD0CFDC0FDCFG_TMME_SHIFT)  |\
    (C0FDCFG_GWBRS  << RCANFD_RSCFD0CFDC0FDCFG_GWBRS_SHIFT) |\
    (C0FDCFG_GWFDF  << RCANFD_RSCFD0CFDC0FDCFG_GWFDF_SHIFT) |\
    (C0FDCFG_GWEN   << RCANFD_RSCFD0CFDC0FDCFG_GWEN_SHIFT)  |\
    (C0FDCFG_TDCO   << RCANFD_RSCFD0CFDC0FDCFG_TDCO_SHIFT)  |\
    (C0FDCFG_ESIC   << RCANFD_RSCFD0CFDC0FDCFG_ESIC_SHIFT)  |\
    (C0FDCFG_TDCE   << RCANFD_RSCFD0CFDC0FDCFG_TDCE_SHIFT)  |\
    (C0FDCFG_TDCOC  << RCANFD_RSCFD0CFDC0FDCFG_TDCOC_SHIFT) |\
    (C0FDCFG_EOCCFG << RCANFD_RSCFD0CFDC0FDCFG_EOCCFG_SHIFT) \
)

#define    CAN_C1FDCFG        (\
    (C1FDCFG_REFE   << RCANFD_RSCFD0CFDC1FDCFG_REFE_SHIFT)  |\
    (C1FDCFG_FDOE   << RCANFD_RSCFD0CFDC1FDCFG_FDOE_SHIFT)  |\
    (C1FDCFG_TMME   << RCANFD_RSCFD0CFDC1FDCFG_TMME_SHIFT)  |\
    (C1FDCFG_GWBRS  << RCANFD_RSCFD0CFDC1FDCFG_GWBRS_SHIFT) |\
    (C1FDCFG_GWFDF  << RCANFD_RSCFD0CFDC1FDCFG_GWFDF_SHIFT) |\
    (C1FDCFG_GWEN   << RCANFD_RSCFD0CFDC1FDCFG_GWEN_SHIFT)  |\
    (C1FDCFG_TDCO   << RCANFD_RSCFD0CFDC1FDCFG_TDCO_SHIFT)  |\
    (C1FDCFG_ESIC   << RCANFD_RSCFD0CFDC1FDCFG_ESIC_SHIFT)  |\
    (C1FDCFG_TDCE   << RCANFD_RSCFD0CFDC1FDCFG_TDCE_SHIFT)  |\
    (C1FDCFG_TDCOC  << RCANFD_RSCFD0CFDC1FDCFG_TDCOC_SHIFT) |\
    (C1FDCFG_EOCCFG << RCANFD_RSCFD0CFDC1FDCFG_EOCCFG_SHIFT) \
)


#define    CAN_GAFLID_RULE(idx)        (\
    (GAFLID_GAFLIDE_##idx   << RCANFD_RSCFD0CFDGAFLID0_GAFLIDE_SHIFT) |\
    (GAFLID_GAFLRTR_##idx   << RCANFD_RSCFD0CFDGAFLID0_GAFLRTR_SHIFT) |\
    (GAFLID_GAFLLB_##idx    << RCANFD_RSCFD0CFDGAFLID0_GAFLLB_SHIFT)  |\
    (GAFLID_GAFLID_##idx << RCANFD_RSCFD0CFDGAFLID0_GAFLID_SHIFT)   \
)

#define    CAN_GAFLM_RULE(idx)        (\
    (GAFLM_GAFLIDEM_##idx   << RCANFD_RSCFD0CFDGAFLM0_GAFLIDEM_SHIFT) |\
    (GAFLM_GAFLRTRM_##idx   << RCANFD_RSCFD0CFDGAFLM0_GAFLRTRM_SHIFT) |\
    (GAFLM_GAFLIDM_##idx    << RCANFD_RSCFD0CFDGAFLM0_GAFLIDM_SHIFT)   \
)

#define    CAN_GAFLP0_RULE(idx)   (\
    (GAFLP0_GAFLDLC_##idx    << RCANFD_RSCFD0CFDGAFLP0_0_GAFLDLC_SHIFT) |\
    (GAFLP0_GAFLPTR_##idx    << RCANFD_RSCFD0CFDGAFLP0_0_GAFLPTR_SHIFT) |\
    (GAFLP0_GAFLRMV_##idx    << RCANFD_RSCFD0CFDGAFLP0_0_GAFLRMV_SHIFT) |\
    (GAFLP0_GAFLRMDP_##idx   << RCANFD_RSCFD0CFDGAFLP0_0_GAFLRMDP_SHIFT) \
)

#define    CAN_GAFLP1_RULE(idx)   (\
    (GAFLP1_GAFLFDP_##idx    << RCANFD_RSCFD0CFDGAFLP1_0_GAFLFDP_SHIFT) \
)


#define    CAN_CFCC0   (\
    (CFCC0_CFITT    << RCANFD_RSCFD0CFDCFCC0_CFITT_SHIFT) |\
    (CFCC0_CFTML    << RCANFD_RSCFD0CFDCFCC0_CFTML_SHIFT) |\
    (CFCC0_CFITR    << RCANFD_RSCFD0CFDCFCC0_CFITR_SHIFT) |\
    (CFCC0_CFITSS   << RCANFD_RSCFD0CFDCFCC0_CFITSS_SHIFT)|\
    (CFCC0_CFM      << RCANFD_RSCFD0CFDCFCC0_CFM_SHIFT)   |\
    (CFCC0_CFIGCV   << RCANFD_RSCFD0CFDCFCC0_CFIGCV_SHIFT)|\
    (CFCC0_CFIM     << RCANFD_RSCFD0CFDCFCC0_CFIM_SHIFT)  |\
    (CFCC0_CFDC     << RCANFD_RSCFD0CFDCFCC0_CFDC_SHIFT)  |\
    (CFCC0_CFPLS    << RCANFD_RSCFD0CFDCFCC0_CFPLS_SHIFT) |\
    (CFCC0_CFTXIE   << RCANFD_RSCFD0CFDCFCC0_CFTXIE_SHIFT)|\
    (CFCC0_CFRXIE   << RCANFD_RSCFD0CFDCFCC0_CFRXIE_SHIFT)\
)

#define    CAN_CFCC1   (\
    (CFCC1_CFITT    << RCANFD_RSCFD0CFDCFCC1_CFITT_SHIFT) |\
    (CFCC1_CFTML    << RCANFD_RSCFD0CFDCFCC1_CFTML_SHIFT) |\
    (CFCC1_CFITR    << RCANFD_RSCFD0CFDCFCC1_CFITR_SHIFT) |\
    (CFCC1_CFITSS   << RCANFD_RSCFD0CFDCFCC1_CFITSS_SHIFT)|\
    (CFCC1_CFM      << RCANFD_RSCFD0CFDCFCC1_CFM_SHIFT)   |\
    (CFCC1_CFIGCV   << RCANFD_RSCFD0CFDCFCC1_CFIGCV_SHIFT)|\
    (CFCC1_CFIM     << RCANFD_RSCFD0CFDCFCC1_CFIM_SHIFT)  |\
    (CFCC1_CFDC     << RCANFD_RSCFD0CFDCFCC1_CFDC_SHIFT)  |\
    (CFCC1_CFPLS    << RCANFD_RSCFD0CFDCFCC1_CFPLS_SHIFT) |\
    (CFCC1_CFTXIE   << RCANFD_RSCFD0CFDCFCC1_CFTXIE_SHIFT)|\
    (CFCC1_CFRXIE   << RCANFD_RSCFD0CFDCFCC1_CFRXIE_SHIFT)\
)

#define    CAN_CFCC2   (\
    (CFCC2_CFITT    << RCANFD_RSCFD0CFDCFCC2_CFITT_SHIFT) |\
    (CFCC2_CFTML    << RCANFD_RSCFD0CFDCFCC2_CFTML_SHIFT) |\
    (CFCC2_CFITR    << RCANFD_RSCFD0CFDCFCC2_CFITR_SHIFT) |\
    (CFCC2_CFITSS   << RCANFD_RSCFD0CFDCFCC2_CFITSS_SHIFT)|\
    (CFCC2_CFM      << RCANFD_RSCFD0CFDCFCC2_CFM_SHIFT)   |\
    (CFCC2_CFIGCV   << RCANFD_RSCFD0CFDCFCC2_CFIGCV_SHIFT)|\
    (CFCC2_CFIM     << RCANFD_RSCFD0CFDCFCC2_CFIM_SHIFT)  |\
    (CFCC2_CFDC     << RCANFD_RSCFD0CFDCFCC2_CFDC_SHIFT)  |\
    (CFCC2_CFPLS    << RCANFD_RSCFD0CFDCFCC2_CFPLS_SHIFT) |\
    (CFCC2_CFTXIE   << RCANFD_RSCFD0CFDCFCC2_CFTXIE_SHIFT)|\
    (CFCC2_CFRXIE   << RCANFD_RSCFD0CFDCFCC2_CFRXIE_SHIFT)\
)

#define    CAN_CFCC3   (\
    (CFCC3_CFITT    << RCANFD_RSCFD0CFDCFCC3_CFITT_SHIFT) |\
    (CFCC3_CFTML    << RCANFD_RSCFD0CFDCFCC3_CFTML_SHIFT) |\
    (CFCC3_CFITR    << RCANFD_RSCFD0CFDCFCC3_CFITR_SHIFT) |\
    (CFCC3_CFITSS   << RCANFD_RSCFD0CFDCFCC3_CFITSS_SHIFT)|\
    (CFCC3_CFM      << RCANFD_RSCFD0CFDCFCC3_CFM_SHIFT)   |\
    (CFCC3_CFIGCV   << RCANFD_RSCFD0CFDCFCC3_CFIGCV_SHIFT)|\
    (CFCC3_CFIM     << RCANFD_RSCFD0CFDCFCC3_CFIM_SHIFT)  |\
    (CFCC3_CFDC     << RCANFD_RSCFD0CFDCFCC3_CFDC_SHIFT)  |\
    (CFCC3_CFPLS    << RCANFD_RSCFD0CFDCFCC3_CFPLS_SHIFT) |\
    (CFCC3_CFTXIE   << RCANFD_RSCFD0CFDCFCC3_CFTXIE_SHIFT)|\
    (CFCC3_CFRXIE   << RCANFD_RSCFD0CFDCFCC3_CFRXIE_SHIFT)\
)

#define    CAN_CFCC4   (\
    (CFCC4_CFITT    << RCANFD_RSCFD0CFDCFCC4_CFITT_SHIFT) |\
    (CFCC4_CFTML    << RCANFD_RSCFD0CFDCFCC4_CFTML_SHIFT) |\
    (CFCC4_CFITR    << RCANFD_RSCFD0CFDCFCC4_CFITR_SHIFT) |\
    (CFCC4_CFITSS   << RCANFD_RSCFD0CFDCFCC4_CFITSS_SHIFT)|\
    (CFCC4_CFM      << RCANFD_RSCFD0CFDCFCC4_CFM_SHIFT)   |\
    (CFCC4_CFIGCV   << RCANFD_RSCFD0CFDCFCC4_CFIGCV_SHIFT)|\
    (CFCC4_CFIM     << RCANFD_RSCFD0CFDCFCC4_CFIM_SHIFT)  |\
    (CFCC4_CFDC     << RCANFD_RSCFD0CFDCFCC4_CFDC_SHIFT)  |\
    (CFCC4_CFPLS    << RCANFD_RSCFD0CFDCFCC4_CFPLS_SHIFT) |\
    (CFCC4_CFTXIE   << RCANFD_RSCFD0CFDCFCC4_CFTXIE_SHIFT)|\
    (CFCC4_CFRXIE   << RCANFD_RSCFD0CFDCFCC4_CFRXIE_SHIFT)\
)

#define    CAN_CFCC5   (\
    (CFCC5_CFITT    << RCANFD_RSCFD0CFDCFCC5_CFITT_SHIFT) |\
    (CFCC5_CFTML    << RCANFD_RSCFD0CFDCFCC5_CFTML_SHIFT) |\
    (CFCC5_CFITR    << RCANFD_RSCFD0CFDCFCC5_CFITR_SHIFT) |\
    (CFCC5_CFITSS   << RCANFD_RSCFD0CFDCFCC5_CFITSS_SHIFT)|\
    (CFCC5_CFM      << RCANFD_RSCFD0CFDCFCC5_CFM_SHIFT)   |\
    (CFCC5_CFIGCV   << RCANFD_RSCFD0CFDCFCC5_CFIGCV_SHIFT)|\
    (CFCC5_CFIM     << RCANFD_RSCFD0CFDCFCC5_CFIM_SHIFT)  |\
    (CFCC5_CFDC     << RCANFD_RSCFD0CFDCFCC5_CFDC_SHIFT)  |\
    (CFCC5_CFPLS    << RCANFD_RSCFD0CFDCFCC5_CFPLS_SHIFT) |\
    (CFCC5_CFTXIE   << RCANFD_RSCFD0CFDCFCC5_CFTXIE_SHIFT)|\
    (CFCC5_CFRXIE   << RCANFD_RSCFD0CFDCFCC5_CFRXIE_SHIFT)\
)


#define    CAN_RFCC0   (\
    (RFCC0_RFIGCV    << RCANFD_RSCFD0CFDRFCC0_RFIGCV_SHIFT)|\
    (RFCC0_RFIM      << RCANFD_RSCFD0CFDRFCC0_RFIM_SHIFT)  |\
    (RFCC0_RFDC      << RCANFD_RSCFD0CFDRFCC0_RFDC_SHIFT)  |\
    (RFCC0_RFPLS     << RCANFD_RSCFD0CFDRFCC0_RFPLS_SHIFT) |\
    (RFCC0_RFIE      << RCANFD_RSCFD0CFDRFCC0_RFIE_SHIFT)  \
)

#define    CAN_RFCC1   (\
    (RFCC1_RFIGCV    << RCANFD_RSCFD0CFDRFCC1_RFIGCV_SHIFT)|\
    (RFCC1_RFIM      << RCANFD_RSCFD0CFDRFCC1_RFIM_SHIFT)  |\
    (RFCC1_RFDC      << RCANFD_RSCFD0CFDRFCC1_RFDC_SHIFT)  |\
    (RFCC1_RFPLS     << RCANFD_RSCFD0CFDRFCC1_RFPLS_SHIFT) |\
    (RFCC1_RFIE      << RCANFD_RSCFD0CFDRFCC1_RFIE_SHIFT)  \
)

#define    CAN_RFCC2   (\
    (RFCC2_RFIGCV    << RCANFD_RSCFD0CFDRFCC2_RFIGCV_SHIFT)|\
    (RFCC2_RFIM      << RCANFD_RSCFD0CFDRFCC2_RFIM_SHIFT)  |\
    (RFCC2_RFDC      << RCANFD_RSCFD0CFDRFCC2_RFDC_SHIFT)  |\
    (RFCC2_RFPLS     << RCANFD_RSCFD0CFDRFCC2_RFPLS_SHIFT) |\
    (RFCC2_RFIE      << RCANFD_RSCFD0CFDRFCC2_RFIE_SHIFT)  \
)

#define    CAN_RFCC3   (\
    (RFCC3_RFIGCV    << RCANFD_RSCFD0CFDRFCC3_RFIGCV_SHIFT)|\
    (RFCC3_RFIM      << RCANFD_RSCFD0CFDRFCC3_RFIM_SHIFT)  |\
    (RFCC3_RFDC      << RCANFD_RSCFD0CFDRFCC3_RFDC_SHIFT)  |\
    (RFCC3_RFPLS     << RCANFD_RSCFD0CFDRFCC3_RFPLS_SHIFT) |\
    (RFCC3_RFIE      << RCANFD_RSCFD0CFDRFCC3_RFIE_SHIFT)  \
)

#define    CAN_RFCC4   (\
    (RFCC4_RFIGCV    << RCANFD_RSCFD0CFDRFCC4_RFIGCV_SHIFT)|\
    (RFCC4_RFIM      << RCANFD_RSCFD0CFDRFCC4_RFIM_SHIFT)  |\
    (RFCC4_RFDC      << RCANFD_RSCFD0CFDRFCC4_RFDC_SHIFT)  |\
    (RFCC4_RFPLS     << RCANFD_RSCFD0CFDRFCC4_RFPLS_SHIFT) |\
    (RFCC4_RFIE      << RCANFD_RSCFD0CFDRFCC4_RFIE_SHIFT)  \
)

#define    CAN_RFCC5   (\
    (RFCC5_RFIGCV    << RCANFD_RSCFD0CFDRFCC5_RFIGCV_SHIFT)|\
    (RFCC5_RFIM      << RCANFD_RSCFD0CFDRFCC5_RFIM_SHIFT)  |\
    (RFCC5_RFDC      << RCANFD_RSCFD0CFDRFCC5_RFDC_SHIFT)  |\
    (RFCC5_RFPLS     << RCANFD_RSCFD0CFDRFCC5_RFPLS_SHIFT) |\
    (RFCC5_RFIE      << RCANFD_RSCFD0CFDRFCC5_RFIE_SHIFT)  \
)

#define    CAN_RFCC6   (\
    (RFCC6_RFIGCV    << RCANFD_RSCFD0CFDRFCC6_RFIGCV_SHIFT)|\
    (RFCC6_RFIM      << RCANFD_RSCFD0CFDRFCC6_RFIM_SHIFT)  |\
    (RFCC6_RFDC      << RCANFD_RSCFD0CFDRFCC6_RFDC_SHIFT)  |\
    (RFCC6_RFPLS     << RCANFD_RSCFD0CFDRFCC6_RFPLS_SHIFT) |\
    (RFCC6_RFIE      << RCANFD_RSCFD0CFDRFCC6_RFIE_SHIFT)  \
)

#define    CAN_RFCC7   (\
    (RFCC7_RFIGCV    << RCANFD_RSCFD0CFDRFCC7_RFIGCV_SHIFT)|\
    (RFCC7_RFIM      << RCANFD_RSCFD0CFDRFCC7_RFIM_SHIFT)  |\
    (RFCC7_RFDC      << RCANFD_RSCFD0CFDRFCC7_RFDC_SHIFT)  |\
    (RFCC7_RFPLS     << RCANFD_RSCFD0CFDRFCC7_RFPLS_SHIFT) |\
    (RFCC7_RFIE      << RCANFD_RSCFD0CFDRFCC7_RFIE_SHIFT)  \
)

#define    CAN_RMNB        (\
    (RMNB_RMPLS   << RCANFD_RSCFD0CFDRMNB_RMPLS_SHIFT) |\
    (RMNB_NRXMB   << RCANFD_RSCFD0CFDRMNB_NRXMB_SHIFT) \
)

#define    CAN_TMIEC0   (\
    (TMIEC0_TMIE31   << RCANFD_RSCFD0CFDTMIEC0_TMIE31_SHIFT) |\
    (TMIEC0_TMIE30   << RCANFD_RSCFD0CFDTMIEC0_TMIE30_SHIFT) |\
    (TMIEC0_TMIE29   << RCANFD_RSCFD0CFDTMIEC0_TMIE29_SHIFT) |\
    (TMIEC0_TMIE28   << RCANFD_RSCFD0CFDTMIEC0_TMIE28_SHIFT) |\
    (TMIEC0_TMIE27   << RCANFD_RSCFD0CFDTMIEC0_TMIE27_SHIFT) |\
    (TMIEC0_TMIE26   << RCANFD_RSCFD0CFDTMIEC0_TMIE26_SHIFT) |\
    (TMIEC0_TMIE25   << RCANFD_RSCFD0CFDTMIEC0_TMIE25_SHIFT) |\
    (TMIEC0_TMIE24   << RCANFD_RSCFD0CFDTMIEC0_TMIE24_SHIFT) |\
    (TMIEC0_TMIE23   << RCANFD_RSCFD0CFDTMIEC0_TMIE23_SHIFT) |\
    (TMIEC0_TMIE22   << RCANFD_RSCFD0CFDTMIEC0_TMIE22_SHIFT) |\
    (TMIEC0_TMIE21   << RCANFD_RSCFD0CFDTMIEC0_TMIE21_SHIFT) |\
    (TMIEC0_TMIE20   << RCANFD_RSCFD0CFDTMIEC0_TMIE20_SHIFT) |\
    (TMIEC0_TMIE19   << RCANFD_RSCFD0CFDTMIEC0_TMIE19_SHIFT) |\
    (TMIEC0_TMIE18   << RCANFD_RSCFD0CFDTMIEC0_TMIE18_SHIFT) |\
    (TMIEC0_TMIE17   << RCANFD_RSCFD0CFDTMIEC0_TMIE17_SHIFT) |\
    (TMIEC0_TMIE16   << RCANFD_RSCFD0CFDTMIEC0_TMIE16_SHIFT) |\
    (TMIEC0_TMIE15   << RCANFD_RSCFD0CFDTMIEC0_TMIE15_SHIFT) |\
    (TMIEC0_TMIE14   << RCANFD_RSCFD0CFDTMIEC0_TMIE14_SHIFT) |\
    (TMIEC0_TMIE13   << RCANFD_RSCFD0CFDTMIEC0_TMIE13_SHIFT) |\
    (TMIEC0_TMIE12   << RCANFD_RSCFD0CFDTMIEC0_TMIE12_SHIFT) |\
    (TMIEC0_TMIE11   << RCANFD_RSCFD0CFDTMIEC0_TMIE11_SHIFT) |\
    (TMIEC0_TMIE10   << RCANFD_RSCFD0CFDTMIEC0_TMIE10_SHIFT) |\
    (TMIEC0_TMIE9    << RCANFD_RSCFD0CFDTMIEC0_TMIE9_SHIFT)  |\
    (TMIEC0_TMIE8    << RCANFD_RSCFD0CFDTMIEC0_TMIE8_SHIFT)  |\
    (TMIEC0_TMIE7    << RCANFD_RSCFD0CFDTMIEC0_TMIE7_SHIFT)  |\
    (TMIEC0_TMIE6    << RCANFD_RSCFD0CFDTMIEC0_TMIE6_SHIFT)  |\
    (TMIEC0_TMIE5    << RCANFD_RSCFD0CFDTMIEC0_TMIE5_SHIFT)  |\
    (TMIEC0_TMIE4    << RCANFD_RSCFD0CFDTMIEC0_TMIE4_SHIFT)  |\
    (TMIEC0_TMIE3    << RCANFD_RSCFD0CFDTMIEC0_TMIE3_SHIFT)  |\
    (TMIEC0_TMIE2    << RCANFD_RSCFD0CFDTMIEC0_TMIE2_SHIFT)  |\
    (TMIEC0_TMIE1    << RCANFD_RSCFD0CFDTMIEC0_TMIE1_SHIFT)  |\
    (TMIEC0_TMIE0    << RCANFD_RSCFD0CFDTMIEC0_TMIE0_SHIFT)   \
)


#define    CAN_TXQCC0   (\
    (TXQCC0_TXQIM   << RCANFD_RSCFD0CFDTXQCC0_TXQIM_SHIFT) |\
    (TXQCC0_TXQIE   << RCANFD_RSCFD0CFDTXQCC0_TXQIE_SHIFT) |\
    (TXQCC0_TXQDC   << RCANFD_RSCFD0CFDTXQCC0_TXQDC_SHIFT) \
)

#define    CAN_TXQCC1   (\
    (TXQCC1_TXQIM   << RCANFD_RSCFD0CFDTXQCC1_TXQIM_SHIFT) |\
    (TXQCC1_TXQIE   << RCANFD_RSCFD0CFDTXQCC1_TXQIE_SHIFT) |\
    (TXQCC1_TXQDC   << RCANFD_RSCFD0CFDTXQCC1_TXQDC_SHIFT) \
)


#define    CAN_THLCC0   (\
    (THLCC0_THLDTE   << RCANFD_RSCFD0CFDTHLCC0_THLDTE_SHIFT) |\
    (THLCC0_THLIM    << RCANFD_RSCFD0CFDTHLCC0_THLIM_SHIFT)  |\
    (THLCC0_THLIE    << RCANFD_RSCFD0CFDTHLCC0_THLIE_SHIFT)  \
)

#define    CAN_THLCC1   (\
    (THLCC1_THLDTE   << RCANFD_RSCFD0CFDTHLCC1_THLDTE_SHIFT) |\
    (THLCC1_THLIM    << RCANFD_RSCFD0CFDTHLCC1_THLIM_SHIFT)  |\
    (THLCC1_THLIE    << RCANFD_RSCFD0CFDTHLCC1_THLIE_SHIFT)  \
)

#define    CAN_GCTR        (\
    (GCTR_CMPOFIE   << RCANFD_RSCFD0CFDGCTR_CMPOFIE_SHIFT) |\
    (GCTR_THLEIE    << RCANFD_RSCFD0CFDGCTR_THLEIE_SHIFT)  |\
    (GCTR_MEIE      << RCANFD_RSCFD0CFDGCTR_MEIE_SHIFT)    |\
    (GCTR_DEIE      << RCANFD_RSCFD0CFDGCTR_DEIE_SHIFT)    \
)

#define    CAN_C0CTR        (\
    (C0CTR_ROM      << RCANFD_RSCFD0CFDC0CTR_ROM_SHIFT)     |\
    (C0CTR_ERRD     << RCANFD_RSCFD0CFDC0CTR_ERRD_SHIFT)    |\
    (C0CTR_BOM      << RCANFD_RSCFD0CFDC0CTR_BOM_SHIFT)     |\
    (C0CTR_TDCVFIE  << RCANFD_RSCFD0CFDC0CTR_TDCVFIE_SHIFT) |\
    (C0CTR_SOCOIE   << RCANFD_RSCFD0CFDC0CTR_SOCOIE_SHIFT)  |\
    (C0CTR_EOCOIE   << RCANFD_RSCFD0CFDC0CTR_EOCOIE_SHIFT)  |\
    (C0CTR_TAIE     << RCANFD_RSCFD0CFDC0CTR_TAIE_SHIFT)    |\
    (C0CTR_ALIE     << RCANFD_RSCFD0CFDC0CTR_ALIE_SHIFT)    |\
    (C0CTR_BLIE     << RCANFD_RSCFD0CFDC0CTR_BLIE_SHIFT)    |\
    (C0CTR_OLIE     << RCANFD_RSCFD0CFDC0CTR_OLIE_SHIFT)    |\
    (C0CTR_BORIE    << RCANFD_RSCFD0CFDC0CTR_BORIE_SHIFT)   |\
    (C0CTR_BOEIE    << RCANFD_RSCFD0CFDC0CTR_BOEIE_SHIFT)   |\
    (C0CTR_EPIE     << RCANFD_RSCFD0CFDC0CTR_EPIE_SHIFT)    |\
    (C0CTR_EWIE     << RCANFD_RSCFD0CFDC0CTR_EWIE_SHIFT)    |\
    (C0CTR_BEIE     << RCANFD_RSCFD0CFDC0CTR_BEIE_SHIFT)    \
)

#define    CAN_C1CTR        (\
    (C1CTR_ROM      << RCANFD_RSCFD0CFDC1CTR_ROM_SHIFT)     |\
    (C1CTR_ERRD     << RCANFD_RSCFD0CFDC1CTR_ERRD_SHIFT)    |\
    (C1CTR_BOM      << RCANFD_RSCFD0CFDC1CTR_BOM_SHIFT)     |\
    (C1CTR_TDCVFIE  << RCANFD_RSCFD0CFDC1CTR_TDCVFIE_SHIFT) |\
    (C1CTR_SOCOIE   << RCANFD_RSCFD0CFDC1CTR_SOCOIE_SHIFT)  |\
    (C1CTR_EOCOIE   << RCANFD_RSCFD0CFDC1CTR_EOCOIE_SHIFT)  |\
    (C1CTR_TAIE     << RCANFD_RSCFD0CFDC1CTR_TAIE_SHIFT)    |\
    (C1CTR_ALIE     << RCANFD_RSCFD0CFDC1CTR_ALIE_SHIFT)    |\
    (C1CTR_BLIE     << RCANFD_RSCFD0CFDC1CTR_BLIE_SHIFT)    |\
    (C1CTR_OLIE     << RCANFD_RSCFD0CFDC1CTR_OLIE_SHIFT)    |\
    (C1CTR_BORIE    << RCANFD_RSCFD0CFDC1CTR_BORIE_SHIFT)   |\
    (C1CTR_BOEIE    << RCANFD_RSCFD0CFDC1CTR_BOEIE_SHIFT)   |\
    (C1CTR_EPIE     << RCANFD_RSCFD0CFDC1CTR_EPIE_SHIFT)    |\
    (C1CTR_EWIE     << RCANFD_RSCFD0CFDC1CTR_EWIE_SHIFT)    |\
    (C1CTR_BEIE     << RCANFD_RSCFD0CFDC1CTR_BEIE_SHIFT)    \
)

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
extern const uint32_t g_BitrateTable[MAX_CHANNELS][3];
extern const uint32_t g_RxRuleTable[CAN0_RX_RULE_NUM + CAN1_RX_RULE_NUM][4];

#endif    /* CAN_CONFIG_HEADER_FILE */

