/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef SSS_MAP_H
#define SSS_MAP_H

/*************** Include Files ********************************************/

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/
#define SSS_CMD_GET_INFO                (0x101)
#define SSS_CMD_SUCCESS                 (0xA1)
#define SSS_CMD_FAIL                    (0xF1)

#define SSS_SRAM_LOCK_EN                (0x01)
#define SSS_DEBUG_LOCK_EN               (0x08)

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/
/////////////////////////////////////////////////////////
// SSS Address in S5J_S100 and S5JT100 //////////////////
/////////////////////////////////////////////////////////
#if defined(TARGET_S5JS100)
#define SSS_REG_BASE                    (0x83100000)
#define MAILBOX_BASE                    (0x83110000)
#define SSS_SRAM_BASE                   (0x83108000)
#define SSS_TRNG_BASE                   (0x83180000)
#endif

#if defined(TARGET_S3JT100)
#define SSS_REG_BASE                    (0x400C0000)
#define MAILBOX_BASE                    (0x400E0000)
#define SSS_SRAM_BASE                   (0x400C8000)
#define SSS_TRNG_BASE                   (0x400C1400)
#endif

/////////////////////////////////////////////////////////
// SSS MAP //////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define SSS_FEEDER_BASE                 (SSS_REG_BASE)
#define SSS_DATA_BASE                   (SSS_SRAM_BASE + 0x6F00)

#define SSS_CM0_LP_CON                  (*(volatile unsigned int *)(SSS_FEEDER_BASE + 0x001C))
#define SSS_LOW_POWER_MODE_EN           (1<<0)
#define SSS_CM0_RESET                   (*(volatile unsigned int *)(MAILBOX_BASE + 0x0004))
#define SSS_CM0_HRESET                  (1<<0)
#define SSS_CM0_SRAM_ACCESS_CONTROL     (*(volatile unsigned int *)(MAILBOX_BASE + 0x0008))
#define SSS_CM0_DEBUG_CONTROL           (*(volatile unsigned int *)(MAILBOX_BASE + 0x0038))

#define SSS_MB_STATUS                   (*(volatile unsigned int *)(MAILBOX_BASE + 0x0000))
#define SSS_CM0_BUSY                    (1<<0)
#define SSS_CTRL_FIELD_BASE             (MAILBOX_BASE + 0x0100)
#define SSS_DATA_FIELD_BASE             (MAILBOX_BASE + 0x0110)

#define SSS_TRNG_CLKDIV                 (*(volatile unsigned int *)(SSS_REG_BASE + 0x1400))
#define SSS_TRNG_STARTUP_CTRL           (*(volatile unsigned int *)(SSS_REG_BASE + 0x144C))
#define SSS_STARTUP_HTPASS              (1<<0)
#define SSS_STARTUP_HTPASS_CLR          (1<<0)
#define SSS_TRNG_CTRL                   (*(volatile unsigned int *)(SSS_REG_BASE + 0x1420))
#define SSS_RNGEN                       (0x80000000)
#define SSS_TRNG_TEST_CTRL              (*(volatile unsigned int *)(SSS_REG_BASE + 0x1440))
#define SSS_HTEN                        (1<<1)
#define SSS_TRNG_TEST_DONE              (*(volatile unsigned int *)(SSS_REG_BASE + 0x1460))
#define SSS_HTDONE                      (1<<1)
#define SSS_KATDONE                     (1<<2)
#define SSS_TRNG_TEST_STAT              (*(volatile unsigned int *)(SSS_REG_BASE + 0x1444))
#define SSS_HTERR                       (1<<2)
#define SSS_KAT_PPERR                   (1<<3)
#define SSS_KAT_CRNGTERR                (1<<4)
#define SSS_TRNG_FIFO_CTRL              (*(volatile unsigned int *)(SSS_REG_BASE + 0x1450))
#define SSS_GEN_1_BYTE                  (1)
#define SSS_GEN_2_BYTE                  (2)
#define SSS_GEN_32_BYTE                 (32)



#if defined(TARGET_S5JS100)
#ifdef OTP_BANK
#define OTP_BASE                            (0x80000000)
#define SSS_ROOT_ENCRYPTION_KEY_BASE        (OTP_BASE + 0x000)
#define SSS_ROOT_PRIVATE_KEY_BASE           (OTP_BASE + 0x020)
#define SSS_FW_ENCRYPTION_KEY_BASE          (OTP_BASE + 0x080)

#define SSS_KM_DIAG_DISABLE                 (OTP_BASE + 0x0A0)
#define SSS_CM0_DEBUG_DISABLE               (OTP_BASE + 0x0A4)
#define SSS_CM0_SRAM_BOOT_DISABLE           (OTP_BASE + 0x0A8)
#define SSS_CM0_SRAM_READ_DISABLE           (OTP_BASE + 0x0AC)
#define SSS_CM0_ANTI_ROLLBACK_COUNT         (OTP_BASE + 0x0B0)
#define SSS_CM0_SECURE_BOOT_KEY_BASE        (OTP_BASE + 0x0C0)
#define SSS_TRANSFER_KEY1_BASE              (OTP_BASE + 0x100)
#define SSS_TRANSFER_KEY2_BASE              (OTP_BASE + 0x110)
#define SSS_TRANSFER_KEY3_BASE              (OTP_BASE + 0x120)
#define SSS_TRANSFER_KEY4_BASE              (OTP_BASE + 0x130)

#define PUF_KEY_BASE                        (OTP_BASE + 0x140)
#define PUF_KEY_VALID                       (OTP_BASE + 0x160)

#define SSS_SW_POR                          (OTP_BASE + 0x200)
#endif //OTP_BANK
#endif //TARGET_S5JS100

#endif  /* SSS_MAP_H */
