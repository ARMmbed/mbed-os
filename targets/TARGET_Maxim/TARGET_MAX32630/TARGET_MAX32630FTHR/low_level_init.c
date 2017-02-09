/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
 
#include "cmsis.h"
#include "i2cm_regs.h"
#include "ioman_regs.h"
#include "clkman_regs.h"
 
//******************************************************************************
// This function will get called early in system initialization
void low_level_init(void)
{
    /* The MAX32630FTHR board utilizes the MAX14690N PMIC which has the 3.3V
     * LDO supplying the VDDB and VDDIOH disabled by default.  USB cannot
     * be initialized until this regulator is enabled.  This code enables
     * the regulator early in the process so that the system does not hang
     * if USB is configured before the PMIC is configured in user code.
     */
    MXC_CLKMAN->sys_clk_ctrl_9_i2cm = 1; // source clock for I2C
    MXC_CLKMAN->i2c_timer_ctrl = 1;      // enable timer for timeouts
    MXC_IOMAN->i2cm2_req = 0x10;         // Request mode A
    // Configure clock divider for 96MHz (worst case)
    MXC_I2CM2->fs_clk_div =((48 << MXC_F_I2CM_FS_CLK_DIV_FS_FILTER_CLK_DIV_POS) |
                            (164 << MXC_F_I2CM_FS_CLK_DIV_FS_SCL_HI_CNT_POS) |
                            (576 << MXC_F_I2CM_FS_CLK_DIV_FS_SCL_LO_CNT_POS));
 
    // Reset module
    MXC_I2CM2->ctrl = MXC_F_I2CM_CTRL_MSTR_RESET_EN;
    MXC_I2CM2->ctrl = 0;
 
    // Enable tx_fifo and rx_fifo
    MXC_I2CM2->ctrl |= (MXC_F_I2CM_CTRL_TX_FIFO_EN | MXC_F_I2CM_CTRL_RX_FIFO_EN);
 
    // Disable and clear the interrupts
    MXC_I2CM2->inten = 0;
    MXC_I2CM2->intfl = MXC_I2CM2->intfl;
 
    // Write the address to the TXFIFO
    MXC_I2CM2_FIFO->tx = (MXC_S_I2CM_TRANS_TAG_START | 0x50); // MAX14690 PMIC I2C Address
 
    // Start the transaction
    MXC_I2CM2->trans |= MXC_F_I2CM_TRANS_TX_START;
 
    // Load write data into the FIFO
    MXC_I2CM2_FIFO->tx = (MXC_S_I2CM_TRANS_TAG_TXDATA_ACK | 0x15); // ldo2vset register addresss
    MXC_I2CM2_FIFO->tx = (MXC_S_I2CM_TRANS_TAG_TXDATA_ACK | 0x19); // 3.3V
    // Send the stop condition
    MXC_I2CM2_FIFO->tx = MXC_S_I2CM_TRANS_TAG_STOP;
 
    // Wait for first write to complete
    while (MXC_I2CM2->trans & MXC_F_I2CM_TRANS_TX_IN_PROGRESS);
 
    // Write the address to the TXFIFO
    MXC_I2CM2_FIFO->tx = (MXC_S_I2CM_TRANS_TAG_START | 0x50); // MAX14690 PMIC I2C Address
 
    // Start the transaction
    MXC_I2CM2->trans |= MXC_F_I2CM_TRANS_TX_START;
 
    // Load write data into the FIFO
    MXC_I2CM2_FIFO->tx = (MXC_S_I2CM_TRANS_TAG_TXDATA_ACK | 0x14); // ldo2cfg register address
    MXC_I2CM2_FIFO->tx = (MXC_S_I2CM_TRANS_TAG_TXDATA_ACK | 0x02); // ldo enabled
    // Send the stop condition
    MXC_I2CM2_FIFO->tx = MXC_S_I2CM_TRANS_TAG_STOP;
}
