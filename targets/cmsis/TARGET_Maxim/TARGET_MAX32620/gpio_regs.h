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

#ifndef _MXC_GPIO_REGS_H_
#define _MXC_GPIO_REGS_H_

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


/*
   Typedefed structure(s) for module registers (per instance or section) with direct 32-bit
   access to each register in module.
*/

/*                                                          Offset          Register Description
                                                            =============   ============================================================================ */
typedef struct {
    __IO uint32_t rst_mode[7];                          /*  0x0000-0x0018   Port P[0..6] Default (Power-On Reset) Output Drive Mode                      */
    __I  uint32_t rsv01C[9];                            /*  0x001C-0x003C                                                                                */
    __IO uint32_t free[7];                              /*  0x0040-0x0058   Port P[0..6] Free for GPIO Operation Flags                                   */
    __I  uint32_t rsv05C[9];                            /*  0x005C-0x007C                                                                                */
    __IO uint32_t out_mode[7];                          /*  0x0080-0x0098   Port P[0..6] Output Drive Mode                                               */
    __I  uint32_t rsv09C[9];                            /*  0x009C-0x00BC                                                                                */
    __IO uint32_t out_val[7];                           /*  0x00C0-0x00D8   Port P[0..6] GPIO Output Value                                               */
    __I  uint32_t rsv0DC[9];                            /*  0x00DC-0x00FC                                                                                */
    __IO uint32_t func_sel[7];                          /*  0x0100-0x0118   Port P[0..6] GPIO Function Select                                            */
    __I  uint32_t rsv11C[9];                            /*  0x011C-0x013C                                                                                */
    __IO uint32_t in_mode[7];                           /*  0x0140-0x0158   Port P[0..6] GPIO Input Monitoring Mode                                      */
    __I  uint32_t rsv15C[9];                            /*  0x015C-0x017C                                                                                */
    __IO uint32_t in_val[7];                            /*  0x0180-0x0198   Port P[0..6] GPIO Input Value                                                */
    __I  uint32_t rsv19C[9];                            /*  0x019C-0x01BC                                                                                */
    __IO uint32_t int_mode[7];                          /*  0x01C0-0x01D8   Port P[0..6] Interrupt Detection Mode                                        */
    __I  uint32_t rsv1DC[9];                            /*  0x01DC-0x01FC                                                                                */
    __IO uint32_t intfl[7];                             /*  0x0200-0x0218   Port P[0..6] Interrupt Flags                                                 */
    __I  uint32_t rsv21C[9];                            /*  0x021C-0x023C                                                                                */
    __IO uint32_t inten[7];                             /*  0x0240-0x0258   Port P[0..6] Interrupt Enables                                               */
} mxc_gpio_regs_t;


/*
   Register offsets for module GPIO.
*/

#define MXC_R_GPIO_OFFS_RST_MODE_P0                         ((uint32_t)0x00000000UL)
#define MXC_R_GPIO_OFFS_RST_MODE_P1                         ((uint32_t)0x00000004UL)
#define MXC_R_GPIO_OFFS_RST_MODE_P2                         ((uint32_t)0x00000008UL)
#define MXC_R_GPIO_OFFS_RST_MODE_P3                         ((uint32_t)0x0000000CUL)
#define MXC_R_GPIO_OFFS_RST_MODE_P4                         ((uint32_t)0x00000010UL)
#define MXC_R_GPIO_OFFS_RST_MODE_P5                         ((uint32_t)0x00000014UL)
#define MXC_R_GPIO_OFFS_RST_MODE_P6                         ((uint32_t)0x00000018UL)
#define MXC_R_GPIO_OFFS_FREE_P0                             ((uint32_t)0x00000040UL)
#define MXC_R_GPIO_OFFS_FREE_P1                             ((uint32_t)0x00000044UL)
#define MXC_R_GPIO_OFFS_FREE_P2                             ((uint32_t)0x00000048UL)
#define MXC_R_GPIO_OFFS_FREE_P3                             ((uint32_t)0x0000004CUL)
#define MXC_R_GPIO_OFFS_FREE_P4                             ((uint32_t)0x00000050UL)
#define MXC_R_GPIO_OFFS_FREE_P5                             ((uint32_t)0x00000054UL)
#define MXC_R_GPIO_OFFS_FREE_P6                             ((uint32_t)0x00000058UL)
#define MXC_R_GPIO_OFFS_OUT_MODE_P0                         ((uint32_t)0x00000080UL)
#define MXC_R_GPIO_OFFS_OUT_MODE_P1                         ((uint32_t)0x00000084UL)
#define MXC_R_GPIO_OFFS_OUT_MODE_P2                         ((uint32_t)0x00000088UL)
#define MXC_R_GPIO_OFFS_OUT_MODE_P3                         ((uint32_t)0x0000008CUL)
#define MXC_R_GPIO_OFFS_OUT_MODE_P4                         ((uint32_t)0x00000090UL)
#define MXC_R_GPIO_OFFS_OUT_MODE_P5                         ((uint32_t)0x00000094UL)
#define MXC_R_GPIO_OFFS_OUT_MODE_P6                         ((uint32_t)0x00000098UL)
#define MXC_R_GPIO_OFFS_OUT_VAL_P0                          ((uint32_t)0x000000C0UL)
#define MXC_R_GPIO_OFFS_OUT_VAL_P1                          ((uint32_t)0x000000C4UL)
#define MXC_R_GPIO_OFFS_OUT_VAL_P2                          ((uint32_t)0x000000C8UL)
#define MXC_R_GPIO_OFFS_OUT_VAL_P3                          ((uint32_t)0x000000CCUL)
#define MXC_R_GPIO_OFFS_OUT_VAL_P4                          ((uint32_t)0x000000D0UL)
#define MXC_R_GPIO_OFFS_OUT_VAL_P5                          ((uint32_t)0x000000D4UL)
#define MXC_R_GPIO_OFFS_OUT_VAL_P6                          ((uint32_t)0x000000D8UL)
#define MXC_R_GPIO_OFFS_FUNC_SEL_P0                         ((uint32_t)0x00000100UL)
#define MXC_R_GPIO_OFFS_FUNC_SEL_P1                         ((uint32_t)0x00000104UL)
#define MXC_R_GPIO_OFFS_FUNC_SEL_P2                         ((uint32_t)0x00000108UL)
#define MXC_R_GPIO_OFFS_FUNC_SEL_P3                         ((uint32_t)0x0000010CUL)
#define MXC_R_GPIO_OFFS_FUNC_SEL_P4                         ((uint32_t)0x00000110UL)
#define MXC_R_GPIO_OFFS_FUNC_SEL_P5                         ((uint32_t)0x00000114UL)
#define MXC_R_GPIO_OFFS_FUNC_SEL_P6                         ((uint32_t)0x00000118UL)
#define MXC_R_GPIO_OFFS_IN_MODE_P0                          ((uint32_t)0x00000140UL)
#define MXC_R_GPIO_OFFS_IN_MODE_P1                          ((uint32_t)0x00000144UL)
#define MXC_R_GPIO_OFFS_IN_MODE_P2                          ((uint32_t)0x00000148UL)
#define MXC_R_GPIO_OFFS_IN_MODE_P3                          ((uint32_t)0x0000014CUL)
#define MXC_R_GPIO_OFFS_IN_MODE_P4                          ((uint32_t)0x00000150UL)
#define MXC_R_GPIO_OFFS_IN_MODE_P5                          ((uint32_t)0x00000154UL)
#define MXC_R_GPIO_OFFS_IN_MODE_P6                          ((uint32_t)0x00000158UL)
#define MXC_R_GPIO_OFFS_IN_VAL_P0                           ((uint32_t)0x00000180UL)
#define MXC_R_GPIO_OFFS_IN_VAL_P1                           ((uint32_t)0x00000184UL)
#define MXC_R_GPIO_OFFS_IN_VAL_P2                           ((uint32_t)0x00000188UL)
#define MXC_R_GPIO_OFFS_IN_VAL_P3                           ((uint32_t)0x0000018CUL)
#define MXC_R_GPIO_OFFS_IN_VAL_P4                           ((uint32_t)0x00000190UL)
#define MXC_R_GPIO_OFFS_IN_VAL_P5                           ((uint32_t)0x00000194UL)
#define MXC_R_GPIO_OFFS_IN_VAL_P6                           ((uint32_t)0x00000198UL)
#define MXC_R_GPIO_OFFS_INT_MODE_P0                         ((uint32_t)0x000001C0UL)
#define MXC_R_GPIO_OFFS_INT_MODE_P1                         ((uint32_t)0x000001C4UL)
#define MXC_R_GPIO_OFFS_INT_MODE_P2                         ((uint32_t)0x000001C8UL)
#define MXC_R_GPIO_OFFS_INT_MODE_P3                         ((uint32_t)0x000001CCUL)
#define MXC_R_GPIO_OFFS_INT_MODE_P4                         ((uint32_t)0x000001D0UL)
#define MXC_R_GPIO_OFFS_INT_MODE_P5                         ((uint32_t)0x000001D4UL)
#define MXC_R_GPIO_OFFS_INT_MODE_P6                         ((uint32_t)0x000001D8UL)
#define MXC_R_GPIO_OFFS_INTFL_P0                            ((uint32_t)0x00000200UL)
#define MXC_R_GPIO_OFFS_INTFL_P1                            ((uint32_t)0x00000204UL)
#define MXC_R_GPIO_OFFS_INTFL_P2                            ((uint32_t)0x00000208UL)
#define MXC_R_GPIO_OFFS_INTFL_P3                            ((uint32_t)0x0000020CUL)
#define MXC_R_GPIO_OFFS_INTFL_P4                            ((uint32_t)0x00000210UL)
#define MXC_R_GPIO_OFFS_INTFL_P5                            ((uint32_t)0x00000214UL)
#define MXC_R_GPIO_OFFS_INTFL_P6                            ((uint32_t)0x00000218UL)
#define MXC_R_GPIO_OFFS_INTEN_P0                            ((uint32_t)0x00000240UL)
#define MXC_R_GPIO_OFFS_INTEN_P1                            ((uint32_t)0x00000244UL)
#define MXC_R_GPIO_OFFS_INTEN_P2                            ((uint32_t)0x00000248UL)
#define MXC_R_GPIO_OFFS_INTEN_P3                            ((uint32_t)0x0000024CUL)
#define MXC_R_GPIO_OFFS_INTEN_P4                            ((uint32_t)0x00000250UL)
#define MXC_R_GPIO_OFFS_INTEN_P5                            ((uint32_t)0x00000254UL)
#define MXC_R_GPIO_OFFS_INTEN_P6                            ((uint32_t)0x00000258UL)


/*
   Field positions and masks for module GPIO.
*/

#define MXC_F_GPIO_RST_MODE_PIN0_POS                        0
#define MXC_F_GPIO_RST_MODE_PIN0                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_RST_MODE_PIN0_POS))
#define MXC_F_GPIO_RST_MODE_PIN1_POS                        4
#define MXC_F_GPIO_RST_MODE_PIN1                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_RST_MODE_PIN1_POS))
#define MXC_F_GPIO_RST_MODE_PIN2_POS                        8
#define MXC_F_GPIO_RST_MODE_PIN2                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_RST_MODE_PIN2_POS))
#define MXC_F_GPIO_RST_MODE_PIN3_POS                        12
#define MXC_F_GPIO_RST_MODE_PIN3                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_RST_MODE_PIN3_POS))
#define MXC_F_GPIO_RST_MODE_PIN4_POS                        16
#define MXC_F_GPIO_RST_MODE_PIN4                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_RST_MODE_PIN4_POS))
#define MXC_F_GPIO_RST_MODE_PIN5_POS                        20
#define MXC_F_GPIO_RST_MODE_PIN5                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_RST_MODE_PIN5_POS))
#define MXC_F_GPIO_RST_MODE_PIN6_POS                        24
#define MXC_F_GPIO_RST_MODE_PIN6                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_RST_MODE_PIN6_POS))
#define MXC_F_GPIO_RST_MODE_PIN7_POS                        28
#define MXC_F_GPIO_RST_MODE_PIN7                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_RST_MODE_PIN7_POS))

#define MXC_F_GPIO_FREE_PIN0_POS                            0
#define MXC_F_GPIO_FREE_PIN0                                ((uint32_t)(0x00000001UL << MXC_F_GPIO_FREE_PIN0_POS))
#define MXC_F_GPIO_FREE_PIN1_POS                            1
#define MXC_F_GPIO_FREE_PIN1                                ((uint32_t)(0x00000001UL << MXC_F_GPIO_FREE_PIN1_POS))
#define MXC_F_GPIO_FREE_PIN2_POS                            2
#define MXC_F_GPIO_FREE_PIN2                                ((uint32_t)(0x00000001UL << MXC_F_GPIO_FREE_PIN2_POS))
#define MXC_F_GPIO_FREE_PIN3_POS                            3
#define MXC_F_GPIO_FREE_PIN3                                ((uint32_t)(0x00000001UL << MXC_F_GPIO_FREE_PIN3_POS))
#define MXC_F_GPIO_FREE_PIN4_POS                            4
#define MXC_F_GPIO_FREE_PIN4                                ((uint32_t)(0x00000001UL << MXC_F_GPIO_FREE_PIN4_POS))
#define MXC_F_GPIO_FREE_PIN5_POS                            5
#define MXC_F_GPIO_FREE_PIN5                                ((uint32_t)(0x00000001UL << MXC_F_GPIO_FREE_PIN5_POS))
#define MXC_F_GPIO_FREE_PIN6_POS                            6
#define MXC_F_GPIO_FREE_PIN6                                ((uint32_t)(0x00000001UL << MXC_F_GPIO_FREE_PIN6_POS))
#define MXC_F_GPIO_FREE_PIN7_POS                            7
#define MXC_F_GPIO_FREE_PIN7                                ((uint32_t)(0x00000001UL << MXC_F_GPIO_FREE_PIN7_POS))

#define MXC_F_GPIO_OUT_MODE_PIN0_POS                        0
#define MXC_F_GPIO_OUT_MODE_PIN0                            ((uint32_t)(0x0000000FUL << MXC_F_GPIO_OUT_MODE_PIN0_POS))
#define MXC_F_GPIO_OUT_MODE_PIN1_POS                        4
#define MXC_F_GPIO_OUT_MODE_PIN1                            ((uint32_t)(0x0000000FUL << MXC_F_GPIO_OUT_MODE_PIN1_POS))
#define MXC_F_GPIO_OUT_MODE_PIN2_POS                        8
#define MXC_F_GPIO_OUT_MODE_PIN2                            ((uint32_t)(0x0000000FUL << MXC_F_GPIO_OUT_MODE_PIN2_POS))
#define MXC_F_GPIO_OUT_MODE_PIN3_POS                        12
#define MXC_F_GPIO_OUT_MODE_PIN3                            ((uint32_t)(0x0000000FUL << MXC_F_GPIO_OUT_MODE_PIN3_POS))
#define MXC_F_GPIO_OUT_MODE_PIN4_POS                        16
#define MXC_F_GPIO_OUT_MODE_PIN4                            ((uint32_t)(0x0000000FUL << MXC_F_GPIO_OUT_MODE_PIN4_POS))
#define MXC_F_GPIO_OUT_MODE_PIN5_POS                        20
#define MXC_F_GPIO_OUT_MODE_PIN5                            ((uint32_t)(0x0000000FUL << MXC_F_GPIO_OUT_MODE_PIN5_POS))
#define MXC_F_GPIO_OUT_MODE_PIN6_POS                        24
#define MXC_F_GPIO_OUT_MODE_PIN6                            ((uint32_t)(0x0000000FUL << MXC_F_GPIO_OUT_MODE_PIN6_POS))
#define MXC_F_GPIO_OUT_MODE_PIN7_POS                        28
#define MXC_F_GPIO_OUT_MODE_PIN7                            ((uint32_t)(0x0000000FUL << MXC_F_GPIO_OUT_MODE_PIN7_POS))

#define MXC_F_GPIO_OUT_VAL_PIN0_POS                         0
#define MXC_F_GPIO_OUT_VAL_PIN0                             ((uint32_t)(0x00000001UL << MXC_F_GPIO_OUT_VAL_PIN0_POS))
#define MXC_F_GPIO_OUT_VAL_PIN1_POS                         1
#define MXC_F_GPIO_OUT_VAL_PIN1                             ((uint32_t)(0x00000001UL << MXC_F_GPIO_OUT_VAL_PIN1_POS))
#define MXC_F_GPIO_OUT_VAL_PIN2_POS                         2
#define MXC_F_GPIO_OUT_VAL_PIN2                             ((uint32_t)(0x00000001UL << MXC_F_GPIO_OUT_VAL_PIN2_POS))
#define MXC_F_GPIO_OUT_VAL_PIN3_POS                         3
#define MXC_F_GPIO_OUT_VAL_PIN3                             ((uint32_t)(0x00000001UL << MXC_F_GPIO_OUT_VAL_PIN3_POS))
#define MXC_F_GPIO_OUT_VAL_PIN4_POS                         4
#define MXC_F_GPIO_OUT_VAL_PIN4                             ((uint32_t)(0x00000001UL << MXC_F_GPIO_OUT_VAL_PIN4_POS))
#define MXC_F_GPIO_OUT_VAL_PIN5_POS                         5
#define MXC_F_GPIO_OUT_VAL_PIN5                             ((uint32_t)(0x00000001UL << MXC_F_GPIO_OUT_VAL_PIN5_POS))
#define MXC_F_GPIO_OUT_VAL_PIN6_POS                         6
#define MXC_F_GPIO_OUT_VAL_PIN6                             ((uint32_t)(0x00000001UL << MXC_F_GPIO_OUT_VAL_PIN6_POS))
#define MXC_F_GPIO_OUT_VAL_PIN7_POS                         7
#define MXC_F_GPIO_OUT_VAL_PIN7                             ((uint32_t)(0x00000001UL << MXC_F_GPIO_OUT_VAL_PIN7_POS))

#define MXC_F_GPIO_FUNC_SEL_P0_PIN0_POS                     0
#define MXC_F_GPIO_FUNC_SEL_P0_PIN0                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P0_PIN0_POS))
#define MXC_F_GPIO_FUNC_SEL_P0_PIN1_POS                     4
#define MXC_F_GPIO_FUNC_SEL_P0_PIN1                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P0_PIN1_POS))
#define MXC_F_GPIO_FUNC_SEL_P0_PIN2_POS                     8
#define MXC_F_GPIO_FUNC_SEL_P0_PIN2                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P0_PIN2_POS))
#define MXC_F_GPIO_FUNC_SEL_P0_PIN3_POS                     12
#define MXC_F_GPIO_FUNC_SEL_P0_PIN3                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P0_PIN3_POS))
#define MXC_F_GPIO_FUNC_SEL_P0_PIN4_POS                     16
#define MXC_F_GPIO_FUNC_SEL_P0_PIN4                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P0_PIN4_POS))
#define MXC_F_GPIO_FUNC_SEL_P0_PIN5_POS                     20
#define MXC_F_GPIO_FUNC_SEL_P0_PIN5                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P0_PIN5_POS))
#define MXC_F_GPIO_FUNC_SEL_P0_PIN6_POS                     24
#define MXC_F_GPIO_FUNC_SEL_P0_PIN6                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P0_PIN6_POS))
#define MXC_F_GPIO_FUNC_SEL_P0_PIN7_POS                     28
#define MXC_F_GPIO_FUNC_SEL_P0_PIN7                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P0_PIN7_POS))

#define MXC_F_GPIO_FUNC_SEL_P1_PIN0_POS                     0
#define MXC_F_GPIO_FUNC_SEL_P1_PIN0                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P1_PIN0_POS))
#define MXC_F_GPIO_FUNC_SEL_P1_PIN1_POS                     4
#define MXC_F_GPIO_FUNC_SEL_P1_PIN1                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P1_PIN1_POS))
#define MXC_F_GPIO_FUNC_SEL_P1_PIN2_POS                     8
#define MXC_F_GPIO_FUNC_SEL_P1_PIN2                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P1_PIN2_POS))
#define MXC_F_GPIO_FUNC_SEL_P1_PIN3_POS                     12
#define MXC_F_GPIO_FUNC_SEL_P1_PIN3                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P1_PIN3_POS))
#define MXC_F_GPIO_FUNC_SEL_P1_PIN4_POS                     16
#define MXC_F_GPIO_FUNC_SEL_P1_PIN4                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P1_PIN4_POS))
#define MXC_F_GPIO_FUNC_SEL_P1_PIN5_POS                     20
#define MXC_F_GPIO_FUNC_SEL_P1_PIN5                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P1_PIN5_POS))
#define MXC_F_GPIO_FUNC_SEL_P1_PIN6_POS                     24
#define MXC_F_GPIO_FUNC_SEL_P1_PIN6                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P1_PIN6_POS))
#define MXC_F_GPIO_FUNC_SEL_P1_PIN7_POS                     28
#define MXC_F_GPIO_FUNC_SEL_P1_PIN7                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P1_PIN7_POS))

#define MXC_F_GPIO_FUNC_SEL_P2_PIN0_POS                     0
#define MXC_F_GPIO_FUNC_SEL_P2_PIN0                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P2_PIN0_POS))
#define MXC_F_GPIO_FUNC_SEL_P2_PIN1_POS                     4
#define MXC_F_GPIO_FUNC_SEL_P2_PIN1                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P2_PIN1_POS))
#define MXC_F_GPIO_FUNC_SEL_P2_PIN2_POS                     8
#define MXC_F_GPIO_FUNC_SEL_P2_PIN2                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P2_PIN2_POS))
#define MXC_F_GPIO_FUNC_SEL_P2_PIN3_POS                     12
#define MXC_F_GPIO_FUNC_SEL_P2_PIN3                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P2_PIN3_POS))
#define MXC_F_GPIO_FUNC_SEL_P2_PIN4_POS                     16
#define MXC_F_GPIO_FUNC_SEL_P2_PIN4                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P2_PIN4_POS))
#define MXC_F_GPIO_FUNC_SEL_P2_PIN5_POS                     20
#define MXC_F_GPIO_FUNC_SEL_P2_PIN5                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P2_PIN5_POS))
#define MXC_F_GPIO_FUNC_SEL_P2_PIN6_POS                     24
#define MXC_F_GPIO_FUNC_SEL_P2_PIN6                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P2_PIN6_POS))
#define MXC_F_GPIO_FUNC_SEL_P2_PIN7_POS                     28
#define MXC_F_GPIO_FUNC_SEL_P2_PIN7                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P2_PIN7_POS))

#define MXC_F_GPIO_FUNC_SEL_P3_PIN0_POS                     0
#define MXC_F_GPIO_FUNC_SEL_P3_PIN0                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P3_PIN0_POS))
#define MXC_F_GPIO_FUNC_SEL_P3_PIN1_POS                     4
#define MXC_F_GPIO_FUNC_SEL_P3_PIN1                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P3_PIN1_POS))
#define MXC_F_GPIO_FUNC_SEL_P3_PIN2_POS                     8
#define MXC_F_GPIO_FUNC_SEL_P3_PIN2                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P3_PIN2_POS))
#define MXC_F_GPIO_FUNC_SEL_P3_PIN3_POS                     12
#define MXC_F_GPIO_FUNC_SEL_P3_PIN3                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P3_PIN3_POS))
#define MXC_F_GPIO_FUNC_SEL_P3_PIN4_POS                     16
#define MXC_F_GPIO_FUNC_SEL_P3_PIN4                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P3_PIN4_POS))
#define MXC_F_GPIO_FUNC_SEL_P3_PIN5_POS                     20
#define MXC_F_GPIO_FUNC_SEL_P3_PIN5                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P3_PIN5_POS))
#define MXC_F_GPIO_FUNC_SEL_P3_PIN6_POS                     24
#define MXC_F_GPIO_FUNC_SEL_P3_PIN6                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P3_PIN6_POS))
#define MXC_F_GPIO_FUNC_SEL_P3_PIN7_POS                     28
#define MXC_F_GPIO_FUNC_SEL_P3_PIN7                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P3_PIN7_POS))

#define MXC_F_GPIO_FUNC_SEL_P4_PIN0_POS                     0
#define MXC_F_GPIO_FUNC_SEL_P4_PIN0                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P4_PIN0_POS))
#define MXC_F_GPIO_FUNC_SEL_P4_PIN1_POS                     4
#define MXC_F_GPIO_FUNC_SEL_P4_PIN1                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P4_PIN1_POS))
#define MXC_F_GPIO_FUNC_SEL_P4_PIN2_POS                     8
#define MXC_F_GPIO_FUNC_SEL_P4_PIN2                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P4_PIN2_POS))
#define MXC_F_GPIO_FUNC_SEL_P4_PIN3_POS                     12
#define MXC_F_GPIO_FUNC_SEL_P4_PIN3                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P4_PIN3_POS))
#define MXC_F_GPIO_FUNC_SEL_P4_PIN4_POS                     16
#define MXC_F_GPIO_FUNC_SEL_P4_PIN4                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P4_PIN4_POS))
#define MXC_F_GPIO_FUNC_SEL_P4_PIN5_POS                     20
#define MXC_F_GPIO_FUNC_SEL_P4_PIN5                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P4_PIN5_POS))
#define MXC_F_GPIO_FUNC_SEL_P4_PIN6_POS                     24
#define MXC_F_GPIO_FUNC_SEL_P4_PIN6                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P4_PIN6_POS))
#define MXC_F_GPIO_FUNC_SEL_P4_PIN7_POS                     28
#define MXC_F_GPIO_FUNC_SEL_P4_PIN7                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P4_PIN7_POS))

#define MXC_F_GPIO_FUNC_SEL_P5_PIN0_POS                     0
#define MXC_F_GPIO_FUNC_SEL_P5_PIN0                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P5_PIN0_POS))
#define MXC_F_GPIO_FUNC_SEL_P5_PIN1_POS                     4
#define MXC_F_GPIO_FUNC_SEL_P5_PIN1                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P5_PIN1_POS))
#define MXC_F_GPIO_FUNC_SEL_P5_PIN2_POS                     8
#define MXC_F_GPIO_FUNC_SEL_P5_PIN2                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P5_PIN2_POS))
#define MXC_F_GPIO_FUNC_SEL_P5_PIN3_POS                     12
#define MXC_F_GPIO_FUNC_SEL_P5_PIN3                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P5_PIN3_POS))
#define MXC_F_GPIO_FUNC_SEL_P5_PIN4_POS                     16
#define MXC_F_GPIO_FUNC_SEL_P5_PIN4                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P5_PIN4_POS))
#define MXC_F_GPIO_FUNC_SEL_P5_PIN5_POS                     20
#define MXC_F_GPIO_FUNC_SEL_P5_PIN5                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P5_PIN5_POS))
#define MXC_F_GPIO_FUNC_SEL_P5_PIN6_POS                     24
#define MXC_F_GPIO_FUNC_SEL_P5_PIN6                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P5_PIN6_POS))
#define MXC_F_GPIO_FUNC_SEL_P5_PIN7_POS                     28
#define MXC_F_GPIO_FUNC_SEL_P5_PIN7                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P5_PIN7_POS))

#define MXC_F_GPIO_FUNC_SEL_P6_PIN0_POS                     0
#define MXC_F_GPIO_FUNC_SEL_P6_PIN0                         ((uint32_t)(0x0000000FUL << MXC_F_GPIO_FUNC_SEL_P6_PIN0_POS))

#define MXC_F_GPIO_IN_MODE_PIN0_POS                         0
#define MXC_F_GPIO_IN_MODE_PIN0                             ((uint32_t)(0x00000003UL << MXC_F_GPIO_IN_MODE_PIN0_POS))
#define MXC_F_GPIO_IN_MODE_PIN1_POS                         4
#define MXC_F_GPIO_IN_MODE_PIN1                             ((uint32_t)(0x00000003UL << MXC_F_GPIO_IN_MODE_PIN1_POS))
#define MXC_F_GPIO_IN_MODE_PIN2_POS                         8
#define MXC_F_GPIO_IN_MODE_PIN2                             ((uint32_t)(0x00000003UL << MXC_F_GPIO_IN_MODE_PIN2_POS))
#define MXC_F_GPIO_IN_MODE_PIN3_POS                         12
#define MXC_F_GPIO_IN_MODE_PIN3                             ((uint32_t)(0x00000003UL << MXC_F_GPIO_IN_MODE_PIN3_POS))
#define MXC_F_GPIO_IN_MODE_PIN4_POS                         16
#define MXC_F_GPIO_IN_MODE_PIN4                             ((uint32_t)(0x00000003UL << MXC_F_GPIO_IN_MODE_PIN4_POS))
#define MXC_F_GPIO_IN_MODE_PIN5_POS                         20
#define MXC_F_GPIO_IN_MODE_PIN5                             ((uint32_t)(0x00000003UL << MXC_F_GPIO_IN_MODE_PIN5_POS))
#define MXC_F_GPIO_IN_MODE_PIN6_POS                         24
#define MXC_F_GPIO_IN_MODE_PIN6                             ((uint32_t)(0x00000003UL << MXC_F_GPIO_IN_MODE_PIN6_POS))
#define MXC_F_GPIO_IN_MODE_PIN7_POS                         28
#define MXC_F_GPIO_IN_MODE_PIN7                             ((uint32_t)(0x00000003UL << MXC_F_GPIO_IN_MODE_PIN7_POS))

#define MXC_F_GPIO_IN_VAL_PIN0_POS                          0
#define MXC_F_GPIO_IN_VAL_PIN0                              ((uint32_t)(0x00000001UL << MXC_F_GPIO_IN_VAL_PIN0_POS))
#define MXC_F_GPIO_IN_VAL_PIN1_POS                          1
#define MXC_F_GPIO_IN_VAL_PIN1                              ((uint32_t)(0x00000001UL << MXC_F_GPIO_IN_VAL_PIN1_POS))
#define MXC_F_GPIO_IN_VAL_PIN2_POS                          2
#define MXC_F_GPIO_IN_VAL_PIN2                              ((uint32_t)(0x00000001UL << MXC_F_GPIO_IN_VAL_PIN2_POS))
#define MXC_F_GPIO_IN_VAL_PIN3_POS                          3
#define MXC_F_GPIO_IN_VAL_PIN3                              ((uint32_t)(0x00000001UL << MXC_F_GPIO_IN_VAL_PIN3_POS))
#define MXC_F_GPIO_IN_VAL_PIN4_POS                          4
#define MXC_F_GPIO_IN_VAL_PIN4                              ((uint32_t)(0x00000001UL << MXC_F_GPIO_IN_VAL_PIN4_POS))
#define MXC_F_GPIO_IN_VAL_PIN5_POS                          5
#define MXC_F_GPIO_IN_VAL_PIN5                              ((uint32_t)(0x00000001UL << MXC_F_GPIO_IN_VAL_PIN5_POS))
#define MXC_F_GPIO_IN_VAL_PIN6_POS                          6
#define MXC_F_GPIO_IN_VAL_PIN6                              ((uint32_t)(0x00000001UL << MXC_F_GPIO_IN_VAL_PIN6_POS))
#define MXC_F_GPIO_IN_VAL_PIN7_POS                          7
#define MXC_F_GPIO_IN_VAL_PIN7                              ((uint32_t)(0x00000001UL << MXC_F_GPIO_IN_VAL_PIN7_POS))

#define MXC_F_GPIO_INT_MODE_PIN0_POS                        0
#define MXC_F_GPIO_INT_MODE_PIN0                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_INT_MODE_PIN0_POS))
#define MXC_F_GPIO_INT_MODE_PIN1_POS                        4
#define MXC_F_GPIO_INT_MODE_PIN1                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_INT_MODE_PIN1_POS))
#define MXC_F_GPIO_INT_MODE_PIN2_POS                        8
#define MXC_F_GPIO_INT_MODE_PIN2                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_INT_MODE_PIN2_POS))
#define MXC_F_GPIO_INT_MODE_PIN3_POS                        12
#define MXC_F_GPIO_INT_MODE_PIN3                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_INT_MODE_PIN3_POS))
#define MXC_F_GPIO_INT_MODE_PIN4_POS                        16
#define MXC_F_GPIO_INT_MODE_PIN4                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_INT_MODE_PIN4_POS))
#define MXC_F_GPIO_INT_MODE_PIN5_POS                        20
#define MXC_F_GPIO_INT_MODE_PIN5                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_INT_MODE_PIN5_POS))
#define MXC_F_GPIO_INT_MODE_PIN6_POS                        24
#define MXC_F_GPIO_INT_MODE_PIN6                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_INT_MODE_PIN6_POS))
#define MXC_F_GPIO_INT_MODE_PIN7_POS                        28
#define MXC_F_GPIO_INT_MODE_PIN7                            ((uint32_t)(0x00000007UL << MXC_F_GPIO_INT_MODE_PIN7_POS))

#define MXC_F_GPIO_INTFL_PIN0_POS                           0
#define MXC_F_GPIO_INTFL_PIN0                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTFL_PIN0_POS))
#define MXC_F_GPIO_INTFL_PIN1_POS                           1
#define MXC_F_GPIO_INTFL_PIN1                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTFL_PIN1_POS))
#define MXC_F_GPIO_INTFL_PIN2_POS                           2
#define MXC_F_GPIO_INTFL_PIN2                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTFL_PIN2_POS))
#define MXC_F_GPIO_INTFL_PIN3_POS                           3
#define MXC_F_GPIO_INTFL_PIN3                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTFL_PIN3_POS))
#define MXC_F_GPIO_INTFL_PIN4_POS                           4
#define MXC_F_GPIO_INTFL_PIN4                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTFL_PIN4_POS))
#define MXC_F_GPIO_INTFL_PIN5_POS                           5
#define MXC_F_GPIO_INTFL_PIN5                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTFL_PIN5_POS))
#define MXC_F_GPIO_INTFL_PIN6_POS                           6
#define MXC_F_GPIO_INTFL_PIN6                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTFL_PIN6_POS))
#define MXC_F_GPIO_INTFL_PIN7_POS                           7
#define MXC_F_GPIO_INTFL_PIN7                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTFL_PIN7_POS))

#define MXC_F_GPIO_INTEN_PIN0_POS                           0
#define MXC_F_GPIO_INTEN_PIN0                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTEN_PIN0_POS))
#define MXC_F_GPIO_INTEN_PIN1_POS                           1
#define MXC_F_GPIO_INTEN_PIN1                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTEN_PIN1_POS))
#define MXC_F_GPIO_INTEN_PIN2_POS                           2
#define MXC_F_GPIO_INTEN_PIN2                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTEN_PIN2_POS))
#define MXC_F_GPIO_INTEN_PIN3_POS                           3
#define MXC_F_GPIO_INTEN_PIN3                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTEN_PIN3_POS))
#define MXC_F_GPIO_INTEN_PIN4_POS                           4
#define MXC_F_GPIO_INTEN_PIN4                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTEN_PIN4_POS))
#define MXC_F_GPIO_INTEN_PIN5_POS                           5
#define MXC_F_GPIO_INTEN_PIN5                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTEN_PIN5_POS))
#define MXC_F_GPIO_INTEN_PIN6_POS                           6
#define MXC_F_GPIO_INTEN_PIN6                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTEN_PIN6_POS))
#define MXC_F_GPIO_INTEN_PIN7_POS                           7
#define MXC_F_GPIO_INTEN_PIN7                               ((uint32_t)(0x00000001UL << MXC_F_GPIO_INTEN_PIN7_POS))



/*
   Field values and shifted values for module GPIO.
*/

#define MXC_V_GPIO_RST_MODE_DRIVE_0                                             ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_RST_MODE_WEAK_PULLDOWN                                       ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_RST_MODE_WEAK_PULLUP                                         ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_RST_MODE_DRIVE_1                                             ((uint32_t)(0x00000003UL))
#define MXC_V_GPIO_RST_MODE_HIGH_Z                                              ((uint32_t)(0x00000004UL))

#define MXC_V_GPIO_FREE_PIN0_NOT_AVAILABLE                                      ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_FREE_PIN0_AVAILABLE                                          ((uint32_t)(0x00000001UL))

#define MXC_S_GPIO_FREE_PIN0_AVAILABLE                                          ((uint32_t)(MXC_V_GPIO_FREE_PIN0_AVAILABLE      << MXC_F_GPIO_FREE_PIN0_POS))

#define MXC_V_GPIO_FREE_PIN1_NOT_AVAILABLE                                      ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_FREE_PIN1_AVAILABLE                                          ((uint32_t)(0x00000001UL))

#define MXC_S_GPIO_FREE_PIN1_AVAILABLE                                          ((uint32_t)(MXC_V_GPIO_FREE_PIN1_AVAILABLE      << MXC_F_GPIO_FREE_PIN1_POS))

#define MXC_V_GPIO_FREE_PIN2_NOT_AVAILABLE                                      ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_FREE_PIN2_AVAILABLE                                          ((uint32_t)(0x00000001UL))

#define MXC_S_GPIO_FREE_PIN2_AVAILABLE                                          ((uint32_t)(MXC_V_GPIO_FREE_PIN2_AVAILABLE      << MXC_F_GPIO_FREE_PIN2_POS))

#define MXC_V_GPIO_FREE_PIN3_NOT_AVAILABLE                                      ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_FREE_PIN3_AVAILABLE                                          ((uint32_t)(0x00000001UL))

#define MXC_S_GPIO_FREE_PIN3_AVAILABLE                                          ((uint32_t)(MXC_V_GPIO_FREE_PIN3_AVAILABLE      << MXC_F_GPIO_FREE_PIN3_POS))

#define MXC_V_GPIO_FREE_PIN4_NOT_AVAILABLE                                      ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_FREE_PIN4_AVAILABLE                                          ((uint32_t)(0x00000001UL))

#define MXC_S_GPIO_FREE_PIN4_AVAILABLE                                          ((uint32_t)(MXC_V_GPIO_FREE_PIN4_AVAILABLE      << MXC_F_GPIO_FREE_PIN4_POS))

#define MXC_V_GPIO_FREE_PIN5_NOT_AVAILABLE                                      ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_FREE_PIN5_AVAILABLE                                          ((uint32_t)(0x00000001UL))

#define MXC_S_GPIO_FREE_PIN5_AVAILABLE                                          ((uint32_t)(MXC_V_GPIO_FREE_PIN5_AVAILABLE      << MXC_F_GPIO_FREE_PIN5_POS))

#define MXC_V_GPIO_FREE_PIN6_NOT_AVAILABLE                                      ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_FREE_PIN6_AVAILABLE                                          ((uint32_t)(0x00000001UL))

#define MXC_S_GPIO_FREE_PIN6_AVAILABLE                                          ((uint32_t)(MXC_V_GPIO_FREE_PIN6_AVAILABLE      << MXC_F_GPIO_FREE_PIN6_POS))

#define MXC_V_GPIO_FREE_PIN7_NOT_AVAILABLE                                      ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_FREE_PIN7_AVAILABLE                                          ((uint32_t)(0x00000001UL))

#define MXC_S_GPIO_FREE_PIN7_AVAILABLE                                          ((uint32_t)(MXC_V_GPIO_FREE_PIN7_AVAILABLE      << MXC_F_GPIO_FREE_PIN7_POS))

#define MXC_V_GPIO_OUT_MODE_HIGH_Z_WEAK_PULLUP                                  ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_OUT_MODE_OPEN_DRAIN                                          ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_OUT_MODE_OPEN_DRAIN_WEAK_PULLUP                              ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_OUT_MODE_NORMAL_HIGH_Z                                       ((uint32_t)(0x00000004UL))
#define MXC_V_GPIO_OUT_MODE_NORMAL                                              ((uint32_t)(0x00000005UL))
#define MXC_V_GPIO_OUT_MODE_SLOW_HIGH_Z                                         ((uint32_t)(0x00000006UL))
#define MXC_V_GPIO_OUT_MODE_SLOW_DRIVE                                          ((uint32_t)(0x00000007UL))
#define MXC_V_GPIO_OUT_MODE_FAST_HIGH_Z                                         ((uint32_t)(0x00000008UL))
#define MXC_V_GPIO_OUT_MODE_FAST_DRIVE                                          ((uint32_t)(0x00000009UL))
#define MXC_V_GPIO_OUT_MODE_HIGH_Z_WEAK_PULLDOWN                                ((uint32_t)(0x0000000AUL))
#define MXC_V_GPIO_OUT_MODE_OPEN_SOURCE                                         ((uint32_t)(0x0000000BUL))
#define MXC_V_GPIO_OUT_MODE_OPEN_SOURCE_WEAK_PULLDOWN                           ((uint32_t)(0x0000000CUL))
#define MXC_V_GPIO_OUT_MODE_HIGH_Z_INPUT_DISABLED                               ((uint32_t)(0x0000000FUL))

#define MXC_V_GPIO_IN_MODE_NORMAL                                               ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_IN_MODE_INVERTED                                             ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_IN_MODE_ALWAYS_ZERO                                          ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_IN_MODE_ALWAYS_ONE                                           ((uint32_t)(0x00000003UL))

#define MXC_V_GPIO_IN_MODE_PIN0_NORMAL                                          ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_IN_MODE_PIN0_INVERTED                                        ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_IN_MODE_PIN0_ALWAYS_ZERO                                     ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_IN_MODE_PIN0_ALWAYS_ONE                                      ((uint32_t)(0x00000003UL))

#define MXC_S_GPIO_IN_MODE_PIN0_ALWAYS_ONE                                      ((uint32_t)(MXC_V_GPIO_IN_MODE_PIN0_ALWAYS_ONE   << MXC_F_GPIO_IN_MODE_PIN0_POS))

#define MXC_V_GPIO_IN_MODE_PIN1_NORMAL                                          ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_IN_MODE_PIN1_INVERTED                                        ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_IN_MODE_PIN1_ALWAYS_ZERO                                     ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_IN_MODE_PIN1_ALWAYS_ONE                                      ((uint32_t)(0x00000003UL))

#define MXC_S_GPIO_IN_MODE_PIN1_ALWAYS_ONE                                      ((uint32_t)(MXC_V_GPIO_IN_MODE_PIN1_ALWAYS_ONE   << MXC_F_GPIO_IN_MODE_PIN1_POS))

#define MXC_V_GPIO_IN_MODE_PIN2_NORMAL                                          ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_IN_MODE_PIN2_INVERTED                                        ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_IN_MODE_PIN2_ALWAYS_ZERO                                     ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_IN_MODE_PIN2_ALWAYS_ONE                                      ((uint32_t)(0x00000003UL))

#define MXC_S_GPIO_IN_MODE_PIN2_ALWAYS_ONE                                      ((uint32_t)(MXC_V_GPIO_IN_MODE_PIN2_ALWAYS_ONE   << MXC_F_GPIO_IN_MODE_PIN2_POS))

#define MXC_V_GPIO_IN_MODE_PIN3_NORMAL                                          ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_IN_MODE_PIN3_INVERTED                                        ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_IN_MODE_PIN3_ALWAYS_ZERO                                     ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_IN_MODE_PIN3_ALWAYS_ONE                                      ((uint32_t)(0x00000003UL))

#define MXC_S_GPIO_IN_MODE_PIN3_ALWAYS_ONE                                      ((uint32_t)(MXC_V_GPIO_IN_MODE_PIN3_ALWAYS_ONE   << MXC_F_GPIO_IN_MODE_PIN3_POS))

#define MXC_V_GPIO_IN_MODE_PIN4_NORMAL                                          ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_IN_MODE_PIN4_INVERTED                                        ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_IN_MODE_PIN4_ALWAYS_ZERO                                     ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_IN_MODE_PIN4_ALWAYS_ONE                                      ((uint32_t)(0x00000003UL))

#define MXC_S_GPIO_IN_MODE_PIN4_ALWAYS_ONE                                      ((uint32_t)(MXC_V_GPIO_IN_MODE_PIN4_ALWAYS_ONE   << MXC_F_GPIO_IN_MODE_PIN4_POS))

#define MXC_V_GPIO_IN_MODE_PIN5_NORMAL                                          ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_IN_MODE_PIN5_INVERTED                                        ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_IN_MODE_PIN5_ALWAYS_ZERO                                     ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_IN_MODE_PIN5_ALWAYS_ONE                                      ((uint32_t)(0x00000003UL))

#define MXC_S_GPIO_IN_MODE_PIN5_ALWAYS_ONE                                      ((uint32_t)(MXC_V_GPIO_IN_MODE_PIN5_ALWAYS_ONE   << MXC_F_GPIO_IN_MODE_PIN5_POS))

#define MXC_V_GPIO_IN_MODE_PIN6_NORMAL                                          ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_IN_MODE_PIN6_INVERTED                                        ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_IN_MODE_PIN6_ALWAYS_ZERO                                     ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_IN_MODE_PIN6_ALWAYS_ONE                                      ((uint32_t)(0x00000003UL))

#define MXC_S_GPIO_IN_MODE_PIN6_ALWAYS_ONE                                      ((uint32_t)(MXC_V_GPIO_IN_MODE_PIN6_ALWAYS_ONE   << MXC_F_GPIO_IN_MODE_PIN6_POS))

#define MXC_V_GPIO_IN_MODE_PIN7_NORMAL                                          ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_IN_MODE_PIN7_INVERTED                                        ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_IN_MODE_PIN7_ALWAYS_ZERO                                     ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_IN_MODE_PIN7_ALWAYS_ONE                                      ((uint32_t)(0x00000003UL))

#define MXC_S_GPIO_IN_MODE_PIN7_ALWAYS_ONE                                      ((uint32_t)(MXC_V_GPIO_IN_MODE_PIN7_ALWAYS_ONE   << MXC_F_GPIO_IN_MODE_PIN7_POS))

#define MXC_V_GPIO_INT_MODE_DISABLE                                             ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_INT_MODE_FALLING_EDGE                                        ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_INT_MODE_RISING_EDGE                                         ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_INT_MODE_ANY_EDGE                                            ((uint32_t)(0x00000003UL))
#define MXC_V_GPIO_INT_MODE_LOW_LVL                                             ((uint32_t)(0x00000004UL))
#define MXC_V_GPIO_INT_MODE_HIGH_LVL                                            ((uint32_t)(0x00000005UL))

#define MXC_V_GPIO_INT_MODE_PIN0_DISABLE                                        ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_INT_MODE_PIN0_FALLING_EDGE                                   ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_INT_MODE_PIN0_RISING_EDGE                                    ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_INT_MODE_PIN0_ANY_EDGE                                       ((uint32_t)(0x00000003UL))
#define MXC_V_GPIO_INT_MODE_PIN0_LOW_LVL                                        ((uint32_t)(0x00000004UL))
#define MXC_V_GPIO_INT_MODE_PIN0_HIGH_LVL                                       ((uint32_t)(0x00000005UL))

#define MXC_S_GPIO_INT_MODE_PIN0_HIGH_LVL                                       ((uint32_t)(MXC_V_GPIO_INT_MODE_PIN0_HIGH_LVL      << MXC_F_GPIO_INT_MODE_PIN0_POS))

#define MXC_V_GPIO_INT_MODE_PIN1_DISABLE                                        ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_INT_MODE_PIN1_FALLING_EDGE                                   ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_INT_MODE_PIN1_RISING_EDGE                                    ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_INT_MODE_PIN1_ANY_EDGE                                       ((uint32_t)(0x00000003UL))
#define MXC_V_GPIO_INT_MODE_PIN1_LOW_LVL                                        ((uint32_t)(0x00000004UL))
#define MXC_V_GPIO_INT_MODE_PIN1_HIGH_LVL                                       ((uint32_t)(0x00000005UL))

#define MXC_S_GPIO_INT_MODE_PIN1_HIGH_LVL                                       ((uint32_t)(MXC_V_GPIO_INT_MODE_PIN1_HIGH_LVL      << MXC_F_GPIO_INT_MODE_PIN1_POS))

#define MXC_V_GPIO_INT_MODE_PIN2_DISABLE                                        ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_INT_MODE_PIN2_FALLING_EDGE                                   ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_INT_MODE_PIN2_RISING_EDGE                                    ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_INT_MODE_PIN2_ANY_EDGE                                       ((uint32_t)(0x00000003UL))
#define MXC_V_GPIO_INT_MODE_PIN2_LOW_LVL                                        ((uint32_t)(0x00000004UL))
#define MXC_V_GPIO_INT_MODE_PIN2_HIGH_LVL                                       ((uint32_t)(0x00000005UL))

#define MXC_S_GPIO_INT_MODE_PIN2_HIGH_LVL                                       ((uint32_t)(MXC_V_GPIO_INT_MODE_PIN2_HIGH_LVL      << MXC_F_GPIO_INT_MODE_PIN2_POS))

#define MXC_V_GPIO_INT_MODE_PIN3_DISABLE                                        ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_INT_MODE_PIN3_FALLING_EDGE                                   ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_INT_MODE_PIN3_RISING_EDGE                                    ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_INT_MODE_PIN3_ANY_EDGE                                       ((uint32_t)(0x00000003UL))
#define MXC_V_GPIO_INT_MODE_PIN3_LOW_LVL                                        ((uint32_t)(0x00000004UL))
#define MXC_V_GPIO_INT_MODE_PIN3_HIGH_LVL                                       ((uint32_t)(0x00000005UL))

#define MXC_S_GPIO_INT_MODE_PIN3_HIGH_LVL                                       ((uint32_t)(MXC_V_GPIO_INT_MODE_PIN3_HIGH_LVL      << MXC_F_GPIO_INT_MODE_PIN3_POS))

#define MXC_V_GPIO_INT_MODE_PIN4_DISABLE                                        ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_INT_MODE_PIN4_FALLING_EDGE                                   ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_INT_MODE_PIN4_RISING_EDGE                                    ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_INT_MODE_PIN4_ANY_EDGE                                       ((uint32_t)(0x00000003UL))
#define MXC_V_GPIO_INT_MODE_PIN4_LOW_LVL                                        ((uint32_t)(0x00000004UL))
#define MXC_V_GPIO_INT_MODE_PIN4_HIGH_LVL                                       ((uint32_t)(0x00000005UL))

#define MXC_S_GPIO_INT_MODE_PIN4_HIGH_LVL                                       ((uint32_t)(MXC_V_GPIO_INT_MODE_PIN4_HIGH_LVL      << MXC_F_GPIO_INT_MODE_PIN4_POS))

#define MXC_V_GPIO_INT_MODE_PIN5_DISABLE                                        ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_INT_MODE_PIN5_FALLING_EDGE                                   ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_INT_MODE_PIN5_RISING_EDGE                                    ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_INT_MODE_PIN5_ANY_EDGE                                       ((uint32_t)(0x00000003UL))
#define MXC_V_GPIO_INT_MODE_PIN5_LOW_LVL                                        ((uint32_t)(0x00000004UL))
#define MXC_V_GPIO_INT_MODE_PIN5_HIGH_LVL                                       ((uint32_t)(0x00000005UL))

#define MXC_S_GPIO_INT_MODE_PIN5_HIGH_LVL                                       ((uint32_t)(MXC_V_GPIO_INT_MODE_PIN5_HIGH_LVL      << MXC_F_GPIO_INT_MODE_PIN5_POS))

#define MXC_V_GPIO_INT_MODE_PIN6_DISABLE                                        ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_INT_MODE_PIN6_FALLING_EDGE                                   ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_INT_MODE_PIN6_RISING_EDGE                                    ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_INT_MODE_PIN6_ANY_EDGE                                       ((uint32_t)(0x00000003UL))
#define MXC_V_GPIO_INT_MODE_PIN6_LOW_LVL                                        ((uint32_t)(0x00000004UL))
#define MXC_V_GPIO_INT_MODE_PIN6_HIGH_LVL                                       ((uint32_t)(0x00000005UL))

#define MXC_S_GPIO_INT_MODE_PIN6_HIGH_LVL                                       ((uint32_t)(MXC_V_GPIO_INT_MODE_PIN6_HIGH_LVL      << MXC_F_GPIO_INT_MODE_PIN6_POS))

#define MXC_V_GPIO_INT_MODE_PIN7_DISABLE                                        ((uint32_t)(0x00000000UL))
#define MXC_V_GPIO_INT_MODE_PIN7_FALLING_EDGE                                   ((uint32_t)(0x00000001UL))
#define MXC_V_GPIO_INT_MODE_PIN7_RISING_EDGE                                    ((uint32_t)(0x00000002UL))
#define MXC_V_GPIO_INT_MODE_PIN7_ANY_EDGE                                       ((uint32_t)(0x00000003UL))
#define MXC_V_GPIO_INT_MODE_PIN7_LOW_LVL                                        ((uint32_t)(0x00000004UL))
#define MXC_V_GPIO_INT_MODE_PIN7_HIGH_LVL                                       ((uint32_t)(0x00000005UL))

#define MXC_S_GPIO_INT_MODE_PIN7_HIGH_LVL                                       ((uint32_t)(MXC_V_GPIO_INT_MODE_PIN7_HIGH_LVL      << MXC_F_GPIO_INT_MODE_PIN7_POS))



#ifdef __cplusplus
}
#endif

#endif   /* _MXC_GPIO_REGS_H_ */

