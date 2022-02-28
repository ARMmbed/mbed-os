/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
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
 *
 *************************************************************************** */

/* **** Includes **** */
#include "gpio_reva_regs.h"

#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief   Enumeration type for the pullup strength on a given pin.
 */
typedef enum {
    MXC_GPIO_PS_NONE,           /**< No pull-up or pull-down strength required*/
    MXC_GPIO_PS_PULL_SELECT,    /**< Selct pull-up or pull-down strength*/         
} mxc_gpio_ps_t;

/* **** Function Prototypes **** */

uint32_t MXC_GPIO_RevA_InGet (mxc_gpio_reva_regs_t* port, uint32_t mask);
void MXC_GPIO_RevA_OutSet (mxc_gpio_reva_regs_t* port, uint32_t mask);
void MXC_GPIO_RevA_OutClr (mxc_gpio_reva_regs_t* port, uint32_t mask);
uint32_t MXC_GPIO_RevA_OutGet (mxc_gpio_reva_regs_t* port, uint32_t mask);
void MXC_GPIO_RevA_OutPut (mxc_gpio_reva_regs_t* port, uint32_t mask, uint32_t val);
void MXC_GPIO_RevA_OutToggle (mxc_gpio_reva_regs_t* port, uint32_t mask);
int MXC_GPIO_RevA_IntConfig (const mxc_gpio_cfg_t *cfg, mxc_gpio_int_pol_t pol);
void MXC_GPIO_RevA_EnableInt (mxc_gpio_reva_regs_t* port, uint32_t mask);
void MXC_GPIO_RevA_DisableInt (mxc_gpio_reva_regs_t* port, uint32_t mask);
void MXC_GPIO_RevA_ClearFlags (mxc_gpio_reva_regs_t* port, uint32_t flags);
uint32_t MXC_GPIO_RevA_GetFlags (mxc_gpio_reva_regs_t* port);
int MXC_GPIO_RevA_SetVSSEL (mxc_gpio_reva_regs_t* port, mxc_gpio_vssel_t vssel, uint32_t mask);
int MXC_GPIO_RevA_SetAF (mxc_gpio_reva_regs_t* port, mxc_gpio_func_t func, uint32_t mask);

/**@} end of group gpio */

#ifdef __cplusplus
}
#endif
