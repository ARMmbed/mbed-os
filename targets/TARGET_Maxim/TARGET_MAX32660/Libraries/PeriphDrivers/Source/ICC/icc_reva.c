/* *****************************************************************************
 * Copyright(C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files(the "Software"),
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
 **************************************************************************** */

/* **** Includes **** */
#include <stddef.h>
#include "mxc_device.h"
#include "mxc_errors.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "icc.h"
#include "icc_reva_regs.h"

/* **** Definitions **** */

/* **** Globals **** */

/* **** Functions **** */
static int MXC_ICC_Ready(mxc_icc_reva_regs_t* icc)
{
    return (icc->ctrl & MXC_F_ICC_REVA_CTRL_RDY);
}

int MXC_ICC_RevA_ID(mxc_icc_reva_regs_t* icc, mxc_icc_info_t cid)
{
    if(icc == NULL) {
        return E_NULL_PTR;
    }

    switch(cid) {
    case ICC_INFO_RELNUM:
        return ((icc->info & MXC_F_ICC_REVA_INFO_RELNUM) >> MXC_F_ICC_REVA_INFO_RELNUM_POS);
        
    case ICC_INFO_PARTNUM:
        return ((icc->info & MXC_F_ICC_REVA_INFO_PARTNUM) >> MXC_F_ICC_REVA_INFO_PARTNUM_POS);
        
    case ICC_INFO_ID:
        return ((icc->info & MXC_F_ICC_REVA_INFO_ID) >> MXC_F_ICC_REVA_INFO_ID_POS);
        
    default:
        return E_BAD_PARAM;
    }
}

void MXC_ICC_RevA_Enable(mxc_icc_reva_regs_t *icc)
{
    // Invalidate cache and wait until ready
    icc->ctrl &= ~MXC_F_ICC_REVA_CTRL_EN;
    icc->invalidate = 1;
    
    while(!(MXC_ICC_Ready(icc)));
    
    // Enable Cache
    icc->ctrl |= MXC_F_ICC_REVA_CTRL_EN;
    while(!(MXC_ICC_Ready(icc)));
}

void MXC_ICC_RevA_Disable(mxc_icc_reva_regs_t* icc)
{
    // Disable Cache
    icc->ctrl &= ~MXC_F_ICC_REVA_CTRL_EN;
}
