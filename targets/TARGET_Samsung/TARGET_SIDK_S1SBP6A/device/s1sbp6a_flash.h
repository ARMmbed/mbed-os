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

#ifndef __S1SBP6A_FLASH_H
#define __S1SBP6A_FLASH_H

#define FLASH_PAGE_SIZE                         2048

#define UFC_FRWCON0_OFFSET                      0x0000
#define UFC_FSHSTAT_OFFSET                      0x0004
#define UFC_PROTCON_OFFSET                      0x0010
#define UFC_PROTMON_OFFSET                      0x0014
#define UFC_DCYCRDCON_OFFSET                    0x0020
#define UFC_LDTCON_OFFSET                       0x0030
#define UFC_FSHCON0_OFFSET                      0x0034
#define UFC_FECCCON_OFFSET                      0x0038
#define UFC_FECCFADR_OFFSET                     0x003C
#define UFC_DCT0_OFFSET                         0x0040
#define UFC_DCT1_OFFSET                         0x0044
#define UFC_LDT0_OFFSET                         0x0050
#define UFC_LDT1_OFFSET                         0x0054
#define UFC_LDT2_OFFSET                         0x0058
#define UFC_LDT3_OFFSET                         0x005C
#define UFC_MODE_OFFSET                         0x0060
#define UFC_CPA0_OFFSET                         0x0064
#define UFC_RED0_OFFSET                         0x0090
#define UFC_RED0_MBIT_OFFSET                    0x0094
#define UFC_RED1_OFFSET                         0x0098
#define UFC_RED1_MBIT_OFFSET                    0x009C
#define PAGE_ERASE                              (0)
#define SECTOR_ERASE                            (1)
#define MAT_ERASE                               (2)
#define UFC_FRWCON_PROG_EN_SHIFT                (0)
#define UFC_FRWCON_ERASE_MODE_SHIFT             (2)
#define UFC_FRWCON_ECC_DATA_SHIFT               (3)
#define UFC_FRWCON_ERASE_WRITE_SEL_SHIFT        (8)
#define UFC_FRWCON_ECC_FAIL_SHIFT               (15)

#define UFC_FRWCON_WRMD_SHIFT                   (0)
#define UFC_FRWCON_HVEN_SHIFT                   (2)
#define UFC_FRWCON_ERSMD_SHIFT                  (3)
#define UFC_FRWCON_BURSTPRM_SHIFT               (8)
#define UFC_FRWCON_HVERSFAIL_SHIFT              (10)
#define UFC_FRWCON_RED_ADDR_EN_SHIFT            (11)
#define UFC_FRWCON_ECC_FAIL_SHIFT               (15)

#define UFC_FRWCON_WRMD_MASK                    (0x03<<UFC_FRWCON_WRMD_SHIFT)
#define UFC_FRWCON_HVEN_MASK                    (0x01<<UFC_FRWCON_HVEN_SHIFT)
#define UFC_FRWCON_ERSMD_MASK                   (0x03<<UFC_FRWCON_ERSMD_SHIFT)
#define UFC_FRWCON_BURSTPRM_MASK                (0x01<<UFC_FRWCON_BURSTPRM_SHIFT)
#define UFC_FRWCON_HVERSFAIL_MASK               (0x01<<UFC_FRWCON_HVERSFAIL_SHIFT)
#define UFC_FRWCON_RED_ADDR_EN_MASK             (0x01<<UFC_FRWCON_RED_ADDR_EN_SHIFT)
#define UFC_FRWCON_ECC_FAIL_MASK                (0x01<<UFC_FRWCON_ECC_FAIL_SHIFT)

#define UFC_FRWCON_WRMD(c)                      ((c)<<UFC_FRWCON_WRMD_SHIFT)
#define UFC_FRWCON_HVEN(c)                      ((c)<<UFC_FRWCON_HVEN_SHIFT)
#define UFC_FRWCON_ERSMD(c)                     ((c)<<UFC_FRWCON_ERSMD_SHIFT)
#define UFC_FRWCON_BURSTPRM(c)                  ((c)<<UFC_FRWCON_BURSTPRM_SHIFT)
#define UFC_FRWCON_HVERSFAIL(c)                 ((c)<<UFC_FRWCON_HVERSFAIL_SHIFT)
#define UFC_FRWCON_RED_ADDR_EN(c)               ((c)<<UFC_FRWCON_RED_ADDR_EN_SHIFT)
#define UFC_FRWCON_ECC_FAILT(c)                 ((c)<<UFC_FRWCON_ECC_FAIL_SHIFT)

#define PAGE_ERASE_MODE         (0)
#define SECTOR_ERASE_MODE       (1)
#define MAT_ERASE_MODE          (2)

#define NO_OP_MODE              (0)
#define ERASE_MODE              (1)
#define WRITE_MODE              (2)

#endif /* __S1SBP6A_FLASH_H */
