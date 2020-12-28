/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/* Copyright (c) 2020 Renesas Electronics Corporation.
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
/**********************************************************************************************************************
 * File Name    : r_octabus_lld_rza2m_api.h
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

#include "r_typedefs.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

#ifndef SC_DRIVERS_R_OCTABUS_INC_R_OCTABUS_LLD_RZA2M_API_H_
#define SC_DRIVERS_R_OCTABUS_INC_R_OCTABUS_LLD_RZA2M_API_H_

/* Version Number of API */

#define STDIO_OCTABUS_RZ_LLD_DRV_NAME ("LLD RZA2M.OCTABUS")

/** Major Version Number of API */
#define STDIO_OCTABUS_RZ_LLD_VERSION_MAJOR      (1)
/** Minor Version Number of API */
#define STDIO_OCTABUS_RZ_LLD_VERSION_MINOR      (0)
/** Minor Version Number of API */
#define STDIO_OCTABUS_RZ_LLD_BUILD_NUM          (0)
/** Unique ID */
#define STDIO_OCTABUS_RZ_LLD_UID                (0)


/*!
 * @enum           e_octabus_init_control_t
 *                 emum of Execute initialize function project setting
 */
typedef enum {
    OCTABUS_NO_INIT        = (0), /*!< Not execute initialize function*/
    OCTABUS_INIT_AT_LOADER = (1), /*!< Execute initialize function on loader */
    OCTABUS_INIT_AT_APP    = (2), /*!< Execute initialize function on application */
} e_octabus_init_control_t;

/*!
 * @enum           e_octabus_precycle_t
 *                 emum of DVnPC bit of CDSR
 */
typedef enum {
    OCTABUS_PRECYCLE_DISABLE = (0), /*!< (default value) */
    OCTABUS_PRECYCLE_ENABLE  = (1), /*!<  */
} e_octabus_precycle_t;

/*!
 * @enum           e_octabus_ttype_t
 *                 emum of DVnTTYP bit of CDSR
 */
typedef enum {
    OCTABUS_TTYPE_SPI  = (0), /*!<  */
    OCTABUS_TTYPE_SOPI = (1), /*!<  */
    OCTABUS_TTYPE_DOPI = (2), /*!<  (default value) */
} e_octabus_ttype_t;


/*!
 * @enum           e_octabus_dqs_ena_cnt_t
 *                 emum of DQSExxx bit of MDTR
 */
typedef enum {
    OCTABUS_DQSENA_1_CYCLE  = (0x0), /*!<  */
    OCTABUS_DQSENA_2_CYCLE  = (0x1), /*!<  */
    OCTABUS_DQSENA_3_CYCLE  = (0x2), /*!<  */
    OCTABUS_DQSENA_4_CYCLE  = (0x3), /*!<  */
    OCTABUS_DQSENA_5_CYCLE  = (0x4), /*!<  */
    OCTABUS_DQSENA_6_CYCLE  = (0x5), /*!<  */
    OCTABUS_DQSENA_7_CYCLE  = (0x6), /*!< (default value) */
    OCTABUS_DQSENA_8_CYCLE  = (0x7), /*!<  */
    OCTABUS_DQSENA_9_CYCLE  = (0x8), /*!<  */
    OCTABUS_DQSENA_10_CYCLE  = (0x9), /*!<  */
    OCTABUS_DQSENA_11_CYCLE  = (0xA), /*!<  */
    OCTABUS_DQSENA_12_CYCLE  = (0xB), /*!<  */
    OCTABUS_DQSENA_13_CYCLE  = (0xC), /*!<  */
    OCTABUS_DQSENA_14_CYCLE  = (0xD), /*!<  */
    OCTABUS_DQSENA_15_CYCLE  = (0xE), /*!<  */
    OCTABUS_DQSENA_16_CYCLE  = (0xF), /*!<  */
} e_octabus_dqs_ena_cnt_t;

/*!
 * @enum           e_octabus_dvrdlo_t
 *                 emum of DVRDLOn field of DRCSTR
 */
typedef enum {
    OCTABUS_DVRDLO_DOPI_1P5_CYCLE  = (0),  /*!<  1.5 clock cycle */
    OCTABUS_DVRDLO_DOPI_2P5_CYCLE  = (1),  /*!<  2.5 clock cycle */
    OCTABUS_DVRDLO_DOPI_3P5_CYCLE  = (2),  /*!<  3.5 clock cycle */
    OCTABUS_DVRDLO_DOPI_4P5_CYCLE  = (3),  /*!<  1.5 clock cycle */
    OCTABUS_DVRDLO_OTHER_2_CYCLE   = (0),  /*!<  2 clock cycle */
    OCTABUS_DVRDLO_OTHER_3_CYCLE   = (1),  /*!<  3 clock cycle */
    OCTABUS_DVRDLO_OTHER_4_CYCLE   = (2),  /*!<  4 clock cycle */
    OCTABUS_DVRDLO_OTHER_5_CYCLE   = (3),  /*!<  5 clock cycle */
} e_octabus_dvrdlo_t;

/*!
 * @enum           e_octabus_dvrdhi_t
 *                 emum of DVRDHIn field of DRCSTR
 */
typedef enum {
    OCTABUS_DVRDHI_DOPI_6P5_CYCLE  = (5),  /*!<  6.5 clock cycle */
    OCTABUS_DVRDHI_DOPI_7P5_CYCLE  = (6),  /*!<  7.5 clock cycle */
    OCTABUS_DVRDHI_DOPI_8P5_CYCLE  = (7),  /*!<  8.5 clock cycle */
    OCTABUS_DVRDHI_OTHER_2_CYCLE   = (0),  /*!<  2 clock cycle */
    OCTABUS_DVRDHI_OTHER_3_CYCLE   = (1),  /*!<  3 clock cycle */
    OCTABUS_DVRDHI_OTHER_4_CYCLE   = (2),  /*!<  4 clock cycle */
    OCTABUS_DVRDHI_OTHER_5_CYCLE   = (3),  /*!<  5 clock cycle */
    OCTABUS_DVRDHI_OTHER_6_CYCLE   = (4),  /*!<  6 clock cycle */
    OCTABUS_DVRDHI_OTHER_7_CYCLE   = (5),  /*!<  7 clock cycle */
    OCTABUS_DVRDHI_OTHER_8_CYCLE   = (6),  /*!<  8 clock cycle */
    OCTABUS_DVRDHI_OTHER_9_CYCLE   = (7),  /*!<  9 clock cycle */
} e_octabus_dvrdhi_t;

/*!
 * @enum           e_octabus_dvrcmd_t
 *                 emum of DVRDCMDn field of DRCSTR
 */
typedef enum {
    OCTABUS_DVRDCMD_2_CYCLE    = (0),  /*!<  2 clock cycle */
    OCTABUS_DVRDCMD_5_CYCLE    = (1),  /*!<  5 clock cycle */
    OCTABUS_DVRDCMD_7_CYCLE    = (2),  /*!<  7 clock cycle */
    OCTABUS_DVRDCMD_9_CYCLE    = (3),  /*!<  9 clock cycle */
    OCTABUS_DVRDCMD_11_CYCLE   = (4),  /*!<  11 clock cycle */
    OCTABUS_DVRDCMD_13_CYCLE   = (5),  /*!<  13 clock cycle */
    OCTABUS_DVRDCMD_15_CYCLE   = (6),  /*!<  15 clock cycle */
    OCTABUS_DVRDCMD_17_CYCLE   = (7),  /*!<  17 clock cycle */
} e_octabus_dvrdcmd_t;

/*!
 * @enum           e_octabus_dvwlo_t
 *                 emum of DVWLOn field of DWCSTR
 */
typedef enum {
    OCTABUS_DVWLO_DOPI_1P5_CYCLE  = (0),  /*!<  1.5 clock cycle */
    OCTABUS_DVWLO_DOPI_2P5_CYCLE  = (1),  /*!<  2.5 clock cycle */
    OCTABUS_DVWLO_DOPI_3P5_CYCLE  = (2),  /*!<  3.5 clock cycle */
    OCTABUS_DVWLO_DOPI_4P5_CYCLE  = (3),  /*!<  1.5 clock cycle */
    OCTABUS_DVWLO_OTHER_2_CYCLE   = (0),  /*!<  2 clock cycle */
    OCTABUS_DVWLO_OTHER_3_CYCLE   = (1),  /*!<  3 clock cycle */
    OCTABUS_DVWLO_OTHER_4_CYCLE   = (2),  /*!<  4 clock cycle */
    OCTABUS_DVWLO_OTHER_5_CYCLE   = (3),  /*!<  5 clock cycle */
} e_octabus_dvwlo_t;

/*!
 * @enum           e_octabus_dvwhi_t
 *                 emum of DVWHIn field of DWCSTR
 */
typedef enum {
    OCTABUS_DVWHI_DOPI_1P5_CYCLE  = (0),  /*!<  1.5 clock cycle */
    OCTABUS_DVWHI_DOPI_2P5_CYCLE  = (1),  /*!<  2.5 clock cycle */
    OCTABUS_DVWHI_DOPI_3P5_CYCLE  = (2),  /*!<  3.5 clock cycle */
    OCTABUS_DVWHI_DOPI_4P5_CYCLE  = (3),  /*!<  4.5 clock cycle */
    OCTABUS_DVWHI_DOPI_5P5_CYCLE  = (4),  /*!<  5.5 clock cycle */
    OCTABUS_DVWHI_DOPI_6P5_CYCLE  = (5),  /*!<  6.5 clock cycle */
    OCTABUS_DVWHI_DOPI_7P5_CYCLE  = (6),  /*!<  7.5 clock cycle */
    OCTABUS_DVWHI_DOPI_8P5_CYCLE  = (7),  /*!<  8.5 clock cycle */
    OCTABUS_DVWHI_OTHER_2_CYCLE   = (0),  /*!<  2 clock cycle */
    OCTABUS_DVWHI_OTHER_3_CYCLE   = (1),  /*!<  3 clock cycle */
    OCTABUS_DVWHI_OTHER_4_CYCLE   = (2),  /*!<  4 clock cycle */
    OCTABUS_DVWHI_OTHER_5_CYCLE   = (3),  /*!<  5 clock cycle */
    OCTABUS_DVWHI_OTHER_6_CYCLE   = (4),  /*!<  6 clock cycle */
    OCTABUS_DVWHI_OTHER_7_CYCLE   = (5),  /*!<  7 clock cycle */
    OCTABUS_DVWHI_OTHER_8_CYCLE   = (6),  /*!<  8 clock cycle */
    OCTABUS_DVWHI_OTHER_9_CYCLE   = (7),  /*!<  9 clock cycle */
} e_octabus_dvwhi_t;

/*!
 * @enum           e_octabus_dvwcmd_t
 *                 emum of DVWCMDn field of DWCSTR
 */
typedef enum {
    OCTABUS_DVWCMD_2_CYCLE    = (0),  /*!<  2 clock cycle */
    OCTABUS_DVWCMD_5_CYCLE    = (1),  /*!<  5 clock cycle */
    OCTABUS_DVWCMD_7_CYCLE    = (2),  /*!<  7 clock cycle */
    OCTABUS_DVWCMD_9_CYCLE    = (3),  /*!<  9 clock cycle */
    OCTABUS_DVWCMD_11_CYCLE   = (4),  /*!<  11 clock cycle */
    OCTABUS_DVWCMD_13_CYCLE   = (5),  /*!<  13 clock cycle */
    OCTABUS_DVWCMD_15_CYCLE   = (6),  /*!<  15 clock cycle */
    OCTABUS_DVWCMD_17_CYCLE   = (7),  /*!<  17 clock cycle */
} e_octabus_dvwcmd_t;

/*!
 * @enum           e_octabus_byte_order_t
 *                 emum of MxOn field of MRWCSR
 */
typedef enum {
    OCTABUS_BYTE_ORDER_B0B1B2B3    = (0),  /*!<  Byte order is byte0, byte1, byte2, byte3 */
    OCTABUS_BYTE_ORDER_B1B0B3B2    = (1),  /*!<  Byte order is byte1, byte0, byte3, byte2 */
} e_octabus_byte_order_t;



typedef struct {
    e_octabus_init_control_t init_flag0;    /*!< Initialize procedure excute project          */
    e_octabus_init_control_t init_flag1;    /*!< Initialize procedure excute project          */
    uint32_t                 devsize1;      /*!< Size of RAM(Byte) */
    e_octabus_precycle_t     precycle1;     /*!< precycle of RAM */
    e_octabus_ttype_t        ttype1;        /*!< transfer type of RAM */
    uint8_t                  w_dmy_len1;    /*!< Write dummy length of RAM */
    uint8_t                  r_dmy_len1;    /*!< Read dummy length of RAM */
    uint8_t                  dopi_delay1;   /*!< Delay of RAM */
    e_octabus_dqs_ena_cnt_t  dqs_ena_cnt1;  /*!< OM_DQS enable counter of RAM */
    e_octabus_dvrdlo_t       dvrdlo1;       /*!< select signal pull-down timing of RAM read */
    e_octabus_dvrdhi_t       dvrdhi1;       /*!< select signal pull-up timing of RAM read */
    e_octabus_dvrdcmd_t      dvrdcmd1;      /*!< Command execution interval of RAM read */
    e_octabus_dvwlo_t        dvwlo1;        /*!< select signal pull-down timing of RAM write */
    e_octabus_dvwhi_t        dvwhi1;        /*!< select signal pull-up timing of RAM write */
    e_octabus_dvwcmd_t       dvwcmd1;       /*!< Command execution interval of RAM write */
    uint8_t                  d1mwcmd1;      /*!< Memory map write command 1 for RAM */
    uint8_t                  d1mwcmd0;      /*!< Memory map write command 0 for RAM */
    uint8_t                  d1mrcmd1;      /*!< Memory map read command 1 for RAM */
    uint8_t                  d1mrcmd0;      /*!< Memory map read command 0 for RAM */
    e_octabus_byte_order_t   mwo1;          /*!< write order of RAM */
    uint8_t                  mwcl1;         /*!< write command length of RAM */
    uint8_t                  mwal1;         /*!< write address length of RAM */
    e_octabus_byte_order_t   mro1;          /*!< read order of RAM */
    uint8_t                  mrcl1;         /*!< read command length of RAM */
    uint8_t                  mral1;         /*!< read address length of RAM */
    uint32_t                 actr;          /*!< Auto-Calibration Timer value of RAM */
    uint32_t                 acar1;         /*!< Auto-Calibration Address value of RAM */
} st_octabus_cfg_t;

typedef struct {
    uint32_t dcr_value;
    uint32_t dar_value;
    uint32_t dcsr_value;
    uint32_t dcstr_value;
} st_octabus_configmode_t;

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @fn         R_OCTABUS_Setup
 *
 * @brief      Initialize Octabus controller and connected device via user code
 *
 * @param[in]  none
 * @retval     none
 */
void R_OCTABUS_Setup(void);

/**
 * @fn         R_OCTABUS_Init
 *
 * @brief      Initialize Octabus controller
 *
 * @param[in]  p_cfg    : register setting data for configuration mode
 * @retval     DRV_SUCCESS : successful operation (always)
 */
int_t R_OCTABUS_Init(const st_octabus_cfg_t *p_cfg);

/**
 * @fn         R_OCTABUS_ManualCalib
 *
 * @brief      Manual calibration dqs delay value of Octabus controller
 *
 * @param[in]  p_cfg       : register setting data for configuration mode
 * @retval     DRV_SUCCESS : successful operation (always)
 * @retval     DRV_ERROR   : unsuccessful operation (always)
 */
int_t R_OCTABUS_ManualCalib(const st_octabus_cfg_t *p_cfg);

/**
 * @fn         R_OCTABUS_AutoCalib
 *
 * @brief      Auto calibration dqs delay value of Octabus controller
 *
 * @param[in]  p_cfg       : register setting data for configuration mode
 * @retval     DRV_SUCCESS : successful operation (always)
 * @retval     DRV_ERROR   : unsuccessful operation (always)
 */
int_t R_OCTABUS_AutoCalib(const st_octabus_cfg_t *p_cfg);

/**
 * @fn         R_OCTABUS_WriteConfigMode
 *
 * @brief      Write data in configuration mode

 * @param[in]  p_config    : register setting data for configuration mode
 * @param[in]  write_value : write data
 * @retval     none
 */
void R_OCTABUS_WriteConfigMode(st_octabus_configmode_t *p_config, uint32_t write_value);

/**
 * @fn         R_OCTABUS_ReadConfigMode
 *
 * @brief      Read data in configuration mode
 *
 * @param[in]  p_config    : register setting data for configuration mode
 * @retval     read value
 */
uint32_t R_OCTABUS_ReadConfigMode(st_octabus_configmode_t *p_config);

/**
 * @fn         OctaBus_UserConfig
 *
 * @brief      User defined device initialisation routine interface
 *
 * @param[in]  p_cfg       : Pointer to the configuration table
 *
 * @retval     DRV_SUCCESS   Successful operation.
 * @retval     DRV_ERROR     Error condition occurs.
 */
extern int_t OctaBus_UserConfig(const st_octabus_cfg_t *p_cfg) __attribute__((weak));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SC_DRIVERS_R_OCTABUS_INC_R_OCTABUS_LLD_RZA2M_API_H_ */
