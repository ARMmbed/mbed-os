/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#ifndef _MXC_PMU_REGS_H_
#define _MXC_PMU_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
    If types are not defined elsewhere (CMSIS) define them here
*/
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif


typedef struct {
    __IO uint32_t start_opcode[32];
    __IO uint32_t enable;
    __IO uint32_t rsvd0;
    __IO uint32_t ll_stopped;
    __IO uint32_t manual;
    __IO uint32_t bus_error;
    __IO uint32_t rsvd1;
    __IO uint32_t to_stat;
    __IO uint32_t rsvd2[4];
    __IO uint32_t to_sel[3];
    __IO uint32_t ps_sel[2];
    __IO uint32_t interrupt;
    __IO uint32_t int_enable;
    __IO uint32_t rsvd3[6];
    __IO uint32_t burst_size[5];
    __IO uint32_t rsvd4[3];
    __IO uint32_t padding[192]; /* Offset to next channel */
} mxc_pmu_bits_t;


/*
   Typedefed structure(s) for module registers (per instance or section) with direct 32-bit
   access to each register in module.
*/

/*                                                          Offset          Register Description
                                                            =============   ============================================================================ */
typedef struct {
    __IO uint32_t dscadr;                               /*  0x0000          PMU Channel Next Descriptor Address                                          */
    __IO uint32_t cfg;                                  /*  0x0004          PMU Channel Configuration                                                    */
    __IO uint32_t loop;                                 /*  0x0008          PMU Channel Loop Counters                                                    */
    __IO uint32_t op;                                   /*  0x000C          PMU Channel Current Descriptor DWORD 0 [INTERNAL TEST ONLY]                  */
    __IO uint32_t dsc1;                                 /*  0x0010          PMU Channel Current Descriptor DWORD 1 [INTERNAL TEST ONLY]                  */
    __IO uint32_t dsc2;                                 /*  0x0014          PMU Channel Current Descriptor DWORD 2 [INTERNAL TEST ONLY]                  */
    __IO uint32_t dsc3;                                 /*  0x0018          PMU Channel Current Descriptor DWORD 3 [INTERNAL TEST ONLY]                  */
    __IO uint32_t dsc4;                                 /*  0x001C          PMU Channel Current Descriptor DWORD 4 [INTERNAL TEST ONLY]                  */
} mxc_pmu_regs_t;


/*
   Register offsets for module PMU.
*/

#define MXC_R_PMU_OFFS_DSCADR                               ((uint32_t)0x00000000UL)
#define MXC_R_PMU_OFFS_CFG                                  ((uint32_t)0x00000004UL)
#define MXC_R_PMU_OFFS_LOOP                                 ((uint32_t)0x00000008UL)
#define MXC_R_PMU_OFFS_OP                                   ((uint32_t)0x0000000CUL)
#define MXC_R_PMU_OFFS_DSC1                                 ((uint32_t)0x00000010UL)
#define MXC_R_PMU_OFFS_DSC2                                 ((uint32_t)0x00000014UL)
#define MXC_R_PMU_OFFS_DSC3                                 ((uint32_t)0x00000018UL)
#define MXC_R_PMU_OFFS_DSC4                                 ((uint32_t)0x0000001CUL)


/*
   Field positions and masks for module PMU.
*/

#define MXC_F_PMU_CFG_ENABLE_POS                            0
#define MXC_F_PMU_CFG_ENABLE                                ((uint32_t)(0x00000001UL << MXC_F_PMU_CFG_ENABLE_POS))
#define MXC_F_PMU_CFG_LL_STOPPED_POS                        2
#define MXC_F_PMU_CFG_LL_STOPPED                            ((uint32_t)(0x00000001UL << MXC_F_PMU_CFG_LL_STOPPED_POS))
#define MXC_F_PMU_CFG_MANUAL_POS                            3
#define MXC_F_PMU_CFG_MANUAL                                ((uint32_t)(0x00000001UL << MXC_F_PMU_CFG_MANUAL_POS))
#define MXC_F_PMU_CFG_BUS_ERROR_POS                         4
#define MXC_F_PMU_CFG_BUS_ERROR                             ((uint32_t)(0x00000001UL << MXC_F_PMU_CFG_BUS_ERROR_POS))
#define MXC_F_PMU_CFG_TO_STAT_POS                           6
#define MXC_F_PMU_CFG_TO_STAT                               ((uint32_t)(0x00000001UL << MXC_F_PMU_CFG_TO_STAT_POS))
#define MXC_F_PMU_CFG_TO_SEL_POS                            11
#define MXC_F_PMU_CFG_TO_SEL                                ((uint32_t)(0x00000007UL << MXC_F_PMU_CFG_TO_SEL_POS))
#define MXC_F_PMU_CFG_PS_SEL_POS                            14
#define MXC_F_PMU_CFG_PS_SEL                                ((uint32_t)(0x00000003UL << MXC_F_PMU_CFG_PS_SEL_POS))
#define MXC_F_PMU_CFG_INTERRUPT_POS                         16
#define MXC_F_PMU_CFG_INTERRUPT                             ((uint32_t)(0x00000001UL << MXC_F_PMU_CFG_INTERRUPT_POS))
#define MXC_F_PMU_CFG_INT_EN_POS                            17
#define MXC_F_PMU_CFG_INT_EN                                ((uint32_t)(0x00000001UL << MXC_F_PMU_CFG_INT_EN_POS))
#define MXC_F_PMU_CFG_BURST_SIZE_POS                        24
#define MXC_F_PMU_CFG_BURST_SIZE                            ((uint32_t)(0x0000001FUL << MXC_F_PMU_CFG_BURST_SIZE_POS))

#define MXC_F_PMU_LOOP_COUNTER_0_POS                        0
#define MXC_F_PMU_LOOP_COUNTER_0                            ((uint32_t)(0x0000FFFFUL << MXC_F_PMU_LOOP_COUNTER_0_POS))
#define MXC_F_PMU_LOOP_COUNTER_1_POS                        16
#define MXC_F_PMU_LOOP_COUNTER_1                            ((uint32_t)(0x0000FFFFUL << MXC_F_PMU_LOOP_COUNTER_1_POS))



#ifdef __cplusplus
}
#endif

#endif   /* _MXC_PMU_REGS_H_ */

