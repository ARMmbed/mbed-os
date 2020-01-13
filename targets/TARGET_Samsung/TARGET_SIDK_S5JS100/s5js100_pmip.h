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

#ifndef __S5JS100_PMIP_H__
#define __S5JS100_PMIP_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef __ASSEMBLY__

#if defined(__cplusplus)
extern "C" {
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef enum {
    PMIP_BUCK0,                 //1.35V, 1.0/1.2V LDO input
    PMIP_BUCK1,                 //1.95V, 1.8V LDO input
    PMIP_BUCKBOOST,             //3.3V, PA input
    PMIP_LDO0,                  //1.0V, Digital
    PMIP_LDO1,                  //1.0V, Analog
    PMIP_LDO2,                  //1.2V, Analog RF
    PMIP_LDO3,                  //1.8V, IO
    PMIP_LDO4,                  //1.8V, Analog RF
    PMIP_LDO5,                  //1.8V, Efuse
    PMIP_LDO6,                  //1.8V, TCXO
    PMIP_MAX
} PMIP_PORT;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5js100_pmip_initialize
 *
 * Description:
 *       Initialize the PMIP.
 *
 * Returned Value:
 *       a NULL on failure
 *
 ****************************************************************************/
extern int s5js100_pmip_initialize(void);
extern int s5js100_pmip_control_BuckBooster(int enable);
extern int s5js100_pmip_control_Ldo5_Efuse(int enable);
extern int s5js100_pmip_control_Ldo6_Tcxo(int enable);
extern int s5js100_pmip_control_powersave(int enable);

#if defined(__cplusplus)
}
#endif

#endif                          /* __ASSEMBLY__ */
#endif                          /* __ARCH_ARM_SRC_S5JS100_S5JS100_PMIP_H */
