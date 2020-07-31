/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2016-2020 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/* Copyright (c) 2016-2020 Renesas Electronics Corporation.
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
/******************************************************************************
* File Name    : dev_drv.h
* $Rev: 127 $
* $Date:: 2016-09-20 11:34:44 +0900#$
* Description  : Device driver header
******************************************************************************/
#ifndef DEV_DRV_H
#define DEV_DRV_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== Arguments, Return values ==== */
#define DEVDRV_SUCCESS      (0)     /* Success */
#define DEVDRV_ERROR        (-1)    /* Failure */

/* ==== Flags ==== */
#define DEVDRV_FLAG_OFF     (0)     /* Flag OFF */
#define DEVDRV_FLAG_ON      (1)     /* Flag ON  */

/* ==== Channels ==== */
typedef enum devdrv_ch {
    DEVDRV_CH_0,            /* Channel 0  */
    DEVDRV_CH_1,            /* Channel 1  */
    DEVDRV_CH_2,            /* Channel 2  */
    DEVDRV_CH_3,            /* Channel 3  */
    DEVDRV_CH_4,            /* Channel 4  */
    DEVDRV_CH_5,            /* Channel 5  */
    DEVDRV_CH_6,            /* Channel 6  */
    DEVDRV_CH_7,            /* Channel 7  */
    DEVDRV_CH_8,            /* Channel 8  */
    DEVDRV_CH_9,            /* Channel 9  */
    DEVDRV_CH_10,           /* Channel 10 */
    DEVDRV_CH_11,           /* Channel 11 */
    DEVDRV_CH_12,           /* Channel 12 */
    DEVDRV_CH_13,           /* Channel 13 */
    DEVDRV_CH_14,           /* Channel 14 */
    DEVDRV_CH_15            /* Channel 15 */
} devdrv_ch_t;

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/

#endif  /* DEV_DRV_H */

/* End of File */
