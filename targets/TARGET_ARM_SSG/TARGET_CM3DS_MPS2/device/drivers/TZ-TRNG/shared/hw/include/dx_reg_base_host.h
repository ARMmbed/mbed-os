/******************************************************************************
 * Copyright (c) 2017-2017, ARM, All Rights Reserved                           *
 * SPDX-License-Identifier: Apache-2.0                                         *
 *                                                                             *
 * Licensed under the Apache License, Version 2.0 (the "License");             *
 * you may not use this file except in compliance with the License.            *
 *                                                                             *
 * You may obtain a copy of the License at                                     *
 * http://www.apache.org/licenses/LICENSE-2.0                                  *
 *                                                                             *
 * Unless required by applicable law or agreed to in writing, software         *
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT   *
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.            *
 *                                                                             *
 * See the License for the specific language governing permissions and         *
 * limitations under the License.                                              *
 ******************************************************************************/

#ifndef __DX_REG_BASE_HOST_H__
#define __DX_REG_BASE_HOST_H__

/*! 
@file
@brief This file contains general HW related definitions. 
*/

/*! Identify platform: Xilinx Zynq7000 ZC706 */
#define DX_PLAT_ZYNQ7000 1
#define DX_PLAT_ZYNQ7000_ZC706 1

/*! SEP core clock frequency in MHz */
#define DX_SEP_FREQ_MHZ 50

/*! Base address for Sansa Silicon-Secure registers. It must be modified to match the mapping of Sansa Silicon-Secure on the customer's *  
*   platform.*/
#define DX_BASE_RNG 0x40000000

/*! Base address for FPGA environment registers */
#define DX_BASE_ENV_REGS 0x80008000

#endif /*__DX_REG_BASE_HOST_H__*/
