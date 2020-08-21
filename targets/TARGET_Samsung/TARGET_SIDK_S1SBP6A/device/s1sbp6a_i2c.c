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

#ifdef DEVICE_I2C
#include <stdbool.h>
#include "s1sbp6a.h"
#include "s1sbp6a_cmu.h"
#include "s1sbp6a_type.h"
#include "s1sbp6a_i2c.h"

#define BP6A_I2C_DEFAULT_MODE   (I2C_AUTO |I2C_INTERRUPT)
#define I2C_MAX_FIFO_SIZE       16

bp6a_i2c_priv_t bp6a_i2c_priv[5] = {
    {
        .index = 0,
        .xfer_speed = DEFAULT_I2CXFER_CLOCK,
        .master = true,
        .mode = BP6A_I2C_DEFAULT_MODE,
        .slave_addr = DEFAULT_I2CSLAVE_ADDR,
        .addrlen = 7,
        .timeout = DEFAULT_I2C_TIMEOUT,
    },
    {
        .index = 1,
        .xfer_speed = DEFAULT_I2CXFER_CLOCK,
        .master = true,
        .mode = BP6A_I2C_DEFAULT_MODE,
        .slave_addr = DEFAULT_I2CSLAVE_ADDR,
        .addrlen = 7,
        .timeout = DEFAULT_I2C_TIMEOUT,
    },
    {
        .index = 2,
        .xfer_speed = DEFAULT_I2CXFER_CLOCK,
        .master = true,
        .mode = BP6A_I2C_DEFAULT_MODE,
        .slave_addr = DEFAULT_I2CSLAVE_ADDR,
        .addrlen = 7,
        .timeout = DEFAULT_I2C_TIMEOUT,
    },
    {
        .index = 3,
        .xfer_speed = DEFAULT_I2CXFER_CLOCK,
        .master = true,
        .mode = BP6A_I2C_DEFAULT_MODE,
        .slave_addr = DEFAULT_I2CSLAVE_ADDR,
        .addrlen = 7,
        .timeout = DEFAULT_I2C_TIMEOUT,
    },
    {
        .index = 4,
        .xfer_speed = DEFAULT_I2CXFER_CLOCK,
        .master = true,
        .mode = BP6A_I2C_DEFAULT_MODE,
        .slave_addr = DEFAULT_I2CSLAVE_ADDR,
        .addrlen = 7,
        .timeout = DEFAULT_I2C_TIMEOUT,
    }
};

static uint32_t bp6a_get_i2c_base_addr(uint32_t ch)
{
    return (BP_I2C0_BASE + ch * 0x1000);
}

static void bp6a_i2c_run_auto_mode(bp6a_i2c_priv_t *priv, bool enable)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    modifyreg32(&(i2c->AUTO_CONF), I2C_AUTO_CONF_MASTER_RUN_MASK,
                I2C_AUTO_CONF_MASTER_RUN(enable));
    modifyreg32(&(i2c->INT_EN), I2C_INT_EN_XFER_DONE_MANUAL_EN_MASK,
                I2C_INT_EN_XFER_DONE_MANUAL_EN(!enable));
}

static int bp6a_i2c_xfer_wait_done_auto(bp6a_i2c_priv_t *priv)
{
    int timeout = priv->timeout;

    while (timeout-- > 0) {
        if (priv->int_stat  & I2C_INT_STAT_XFER_DONE_AUTO_MASK) {
            return 0;
        }

    }

    return -1;
}

static int bp6a_i2c_xfer_wait_done_manual(bp6a_i2c_priv_t *priv)
{
    uint32_t timeout = priv->timeout;
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    while (timeout-- > 0) {
        if (getreg32(&(i2c->INT_STAT)) & I2C_INT_STAT_XFER_DONE_MANUAL_MASK) {
            return 0;
        }
    }
    return -1;
}

static void bp6a_i2c_set_channel(bp6a_i2c_priv_t *priv, bool tx, bool rx)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    modifyreg32(&(i2c->CTL), I2C_CTL_RXCHON_MASK | I2C_CTL_TXCHON_MASK,
                I2C_CTL_RXCHON(rx) | I2C_CTL_TXCHON(tx));
}

static void bp6a_i2c_set_ctl_mode(bp6a_i2c_priv_t *priv)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    modifyreg32(&(i2c->CTL), I2C_CTL_MASTER_MASK, I2C_CTL_MASTER(priv->master));
}

static void bp6a_i2c_set_master_addr(bp6a_i2c_priv_t *priv, uint32_t addr)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    /* For auto mode and HS mode only */
    modifyreg32(&(i2c->ADDR), I2C_ADDR_MASTERID_MASK, I2C_ADDR_MASTERID(addr));
}

static void bp6a_i2c_set_slave_addr(bp6a_i2c_priv_t *priv)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    if (priv->addrlen == 10) {
        modifyreg32(&(i2c->CONF), I2C_CONF_ADDR_MODE_MASK, I2C_CONF_ADDR_MODE(1));
    } else {
        modifyreg32(&(i2c->CONF), I2C_CONF_ADDR_MODE_MASK, I2C_CONF_ADDR_MODE(0));
    }

    if (priv->master) {
        modifyreg32(&(i2c->ADDR), I2C_ADDR_SLAVE_ADDR_MAS_MASK,
                    I2C_ADDR_SLAVE_ADDR_MAS(priv->slave_addr));
        modifyreg32(&(i2c->ADDR), I2C_ADDR_SLAVE_ADDR_SLA_MASK,
                    I2C_ADDR_SLAVE_ADDR_SLA(0));
    } else
        modifyreg32(&(i2c->ADDR), I2C_ADDR_SLAVE_ADDR_SLA_MASK,
                    I2C_ADDR_SLAVE_ADDR_SLA(priv->slave_addr));
}

static void bp6a_i2c_set_hs_mode_timing(BP_I2C_TypeDef *i2c, uint8_t clkDiv,
                                        uint8_t tSTART_SU, uint16_t tSTART_HD, uint8_t tSTOP_SU,
                                        uint8_t tDATA_SU, uint8_t tDATA_HD, uint8_t tSCL_L,
                                        uint8_t tSCL_H, uint8_t tSR_RELEASE)
{
    putreg32(&(i2c->TIMING_HS1),
             I2C_TIMING_HS1_TSDA_SU_HS(0) |
             I2C_TIMING_HS1_TSTOP_SU_HS(tSTOP_SU) |
             I2C_TIMING_HS1_TSTART_HD_HS(tSTART_HD) |
             I2C_TIMING_HS1_TSTART_SU_HS(tSTART_SU));
    putreg32(&(i2c->TIMING_HS2),
             I2C_TIMING_HS2_TSCL_H_HS(tSCL_H) |
             I2C_TIMING_HS2_TSCL_L_HS(tSCL_L) |
             I2C_TIMING_HS2_TDATA_SU_HS(tDATA_SU));

    putreg32(&(i2c->TIMING_HS3),
             I2C_TIMING_HS3_TSR_RELEASE(tSR_RELEASE) |
             I2C_TIMING_HS3_CLK_DIV(clkDiv));

    putreg32(&(i2c->TIMING_SLA), tDATA_HD & 0xFFFF);
}

static void bp6a_set_fs_mode_timing(BP_I2C_TypeDef *i2c, uint8_t clkDiv,
                                    uint8_t tSTART_SU, uint8_t tSTART_HD, uint8_t tSTOP_SU,
                                    uint8_t tDATA_SU, uint16_t tDATA_HD, uint8_t tSCL_L,
                                    uint8_t tSCL_H, uint8_t tSR_RELEASE)
{
    putreg32(&(i2c->TIMING_FS1),
             I2C_TIMING_FS1_TSDA_SU_FS(0) |
             I2C_TIMING_FS1_TSTOP_SU_FS(tSTOP_SU) |
             I2C_TIMING_FS1_TSTART_HD_FS(tSTART_HD) |
             I2C_TIMING_FS1_TSTART_SU_FS(tSTART_SU));

    putreg32(&(i2c->TIMING_FS2),
             I2C_TIMING_FS2_TSCL_H_FS(tSCL_H) |
             I2C_TIMING_FS2_TSCL_L_FS(tSCL_L) |
             I2C_TIMING_FS2_TDATA_SU_FS(tDATA_SU));

    putreg32(&(i2c->TIMING_FS3),
             I2C_TIMING_FS3_TSR_RELEASE(tSR_RELEASE) |
             I2C_TIMING_FS3_CLK_DIV(clkDiv));

    putreg32(&(i2c->TIMING_SLA), 0); //tDATA_HD);
}

static void bp6a_i2c_calculate_timing(bp6a_i2c_priv_t *priv)
{
    uint32_t clkDiv;
    uint32_t tFTL_CYCLE_SCL;

    int32_t i = 0;
    int32_t uTemp0 = 0;
    int32_t uTemp1 = 0;
    int32_t uTemp2 = 0;
    uint32_t ipClk = bp6a_cmu_get_clock_freq(CMU_I2C0_CLK + priv->index);
    uint32_t opClk = priv->xfer_speed;
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    modifyreg32(&(i2c->CONF),
                I2C_CONF_FLT_CYCLE_SDA_MASK |
                I2C_CONF_FLT_CYCLE_SCL_MASK,
                I2C_CONF_FLT_CYCLE_SDA(0) |
                I2C_CONF_FLT_CYCLE_SCL(0));

    tFTL_CYCLE_SCL = (getreg32(&(i2c->CONF)) & I2C_CONF_FLT_CYCLE_SCL_MASK)
                     >> I2C_CONF_FLT_CYCLE_SCL_SHIFT;

    if (tFTL_CYCLE_SCL > 0x2) {
        uTemp0 = (uint32_t)((float)(ipClk / opClk) - (tFTL_CYCLE_SCL + 3) * 2);
    } else {
        uTemp0 = (uint32_t)((float)(ipClk / opClk) - (tFTL_CYCLE_SCL + 1 + 3) * 2);
    }

    for (i = 0; i < 256; i++) {
        uTemp1 = uTemp0 / (i + 1);

        if (uTemp1 < 256) {
            uTemp2 = uTemp1 - 2;
            break;
        }
    }

    clkDiv = i;
    uint32_t tSCL_H;
    if (opClk > I2C_FREQ_400KHZ) {
        tSCL_H = ((uTemp2 + 10) / 3) - 5;
    } else {
        tSCL_H = uTemp2 / 2;
    }

    uint32_t tSCL_L = uTemp2 - tSCL_H;

    uint32_t tSTART_SU = tSCL_L;
    uint32_t tSTART_HD = tSCL_L;
    uint32_t tSTOP_SU = tSCL_L;
    uint32_t tDATA_SU = tSCL_L / 2;
    uint32_t tDATA_HD = tSCL_L / 2;
    uint32_t tSR_RELEASE = uTemp2;

    if (opClk > I2C_FREQ_400KHZ) {
        bp6a_set_fs_mode_timing(i2c, 1, 37, 37, 37, 18, 18, 37, 37, 74);
        bp6a_i2c_set_hs_mode_timing(i2c, clkDiv, tSTART_SU, tSTART_HD, tSTOP_SU,
                                    tDATA_SU, tDATA_HD, tSCL_L, tSCL_H, tSR_RELEASE);
    } else
        bp6a_set_fs_mode_timing(i2c, clkDiv, tSTART_SU, tSTART_HD, tSTOP_SU,
                                tDATA_SU, tDATA_HD,  tSCL_L, tSCL_H, tSR_RELEASE);
}

static void bp6a_i2c_set_fifo_level(bp6a_i2c_priv_t *priv, uint32_t tx, uint32_t rx)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    modifyreg32(&(i2c->FIFO_CTL), I2C_FIFO_CTL_RXFIFO_TRIG_MASK |
                I2C_FIFO_CTL_TXFIFO_TRIG_MASK |
                I2C_FIFO_CTL_RXFIFO_EN_MASK |
                I2C_FIFO_CTL_TXFIFO_EN_MASK,
                I2C_FIFO_CTL_RXFIFO_EN(!!rx) |
                I2C_FIFO_CTL_TXFIFO_EN(!!tx) |
                I2C_FIFO_CTL_RXFIFO_TRIG(rx) |
                I2C_FIFO_CTL_TXFIFO_TRIG(tx));
}

static void bp6a_i2c_set_timeout(bp6a_i2c_priv_t *priv)
{
    uint32_t en = 1;
    uint32_t timeoutCount = priv->timeout;

    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    if (priv->timeout == 0) {
        timeoutCount = 0xFF;
        en = 0;
    }

    modifyreg32(&(i2c->TIMEOUT), I2C_TIMEOUT_TIMEOUT_EN_MASK |
                I2C_TIMEOUT_TOUT_COUNT_MASK,
                I2C_TIMEOUT_TIMEOUT_EN(en) |
                I2C_TIMEOUT_TOUT_COUNT(timeoutCount | 0xFF00));
}

static void bp6a_i2c_reset_txFIFO(bp6a_i2c_priv_t *priv)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    modifyreg32(&(i2c->FIFO_CTL), I2C_FIFO_CTL_TXFIFO_RST_MASK, I2C_FIFO_CTL_TXFIFO_RST(1));
    modifyreg32(&(i2c->FIFO_CTL), I2C_FIFO_CTL_TXFIFO_RST_MASK, I2C_FIFO_CTL_TXFIFO_RST(0));
}

static void bp6a_i2c_reset_rxFIFO(bp6a_i2c_priv_t *priv)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    modifyreg32(&(i2c->FIFO_CTL), I2C_FIFO_CTL_RXFIFO_RST_MASK, I2C_FIFO_CTL_RXFIFO_RST(1));
    modifyreg32(&(i2c->FIFO_CTL), I2C_FIFO_CTL_RXFIFO_RST_MASK, I2C_FIFO_CTL_RXFIFO_RST(0));
}

static int bp6a_i2c_out_byte(bp6a_i2c_priv_t *priv, uint8_t data)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    /* Set Data to TX buffer and Send 1 byte */
    modifyreg32(&(i2c->MANUAL_CMD), I2C_MANUAL_CMD_TX_DATA_MASK, I2C_MANUAL_CMD_TX_DATA(data));
    modifyreg32(&(i2c->MANUAL_CMD), I2C_MANUAL_CMD_SEND_DATA_MASK, I2C_MANUAL_CMD_SEND_DATA(1));

    return bp6a_i2c_xfer_wait_done_manual(priv);
}

static int bp6a_i2c_in_byte(bp6a_i2c_priv_t *priv, bool is_ack)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    /* Looks awkward, but if I2C_RX_ACK is set, ACK is NOT generated */
    if (!is_ack)
        modifyreg32((&i2c->MANUAL_CMD), I2C_MANUAL_CMD_RX_ACK_MASK |
                    I2C_MANUAL_CMD_READ_DATA_MASK,
                    I2C_MANUAL_CMD_RX_ACK(1) |
                    I2C_MANUAL_CMD_READ_DATA(1));
    else
        modifyreg32((&i2c->MANUAL_CMD), I2C_MANUAL_CMD_READ_DATA_MASK,
                    I2C_MANUAL_CMD_READ_DATA(1));

    if (bp6a_i2c_xfer_wait_done_manual(priv) < 0) {
        return -1;
    }

    return ((getreg32(&i2c->MANUAL_CMD) & I2C_MANUAL_CMD_RX_DATA_MASK)
            >> I2C_MANUAL_CMD_RX_DATA_SHIFT);
}

static void bp6a_i2c_set_buffer(bp6a_i2c_priv_t *priv, struct i2c_msg_s *msgv)
{
    priv->mptr = msgv->buffer;
    priv->mcnt = msgv->length;
    priv->cur_msg = 0;
}

static void bp6a_i2c_set_auto_config(bp6a_i2c_priv_t *priv, bool stop,
                                     bool is_read, uint32_t len)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    /* Set Auto Stop */
    modifyreg32(&(i2c->AUTO_CONF), I2C_AUTO_CONF_STOP_AFTER_TRANS_MASK,
                I2C_AUTO_CONF_STOP_AFTER_TRANS(stop));

    /* Set Type of transaction : 0(Tx), 1(Rx) */
    modifyreg32(&(i2c->AUTO_CONF), I2C_AUTO_CONF_READ_WRITE_MASK,
                I2C_AUTO_CONF_READ_WRITE(is_read));

    /* Set Length of transaction */
    modifyreg32(&(i2c->AUTO_CONF), I2C_AUTO_CONF_TRANS_LEN_MASK, I2C_AUTO_CONF_TRANS_LEN(len));
}

static uint8_t bp6a_i2c_get_tx_fifo_level(bp6a_i2c_priv_t *priv)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    return (uint8_t)((getreg32(&i2c->FIFO_STAT) & I2C_FIFO_STAT_TX_FIFO_LEVEL_MASK) >> I2C_FIFO_STAT_TX_FIFO_LEVEL_SHIFT);
}

static uint8_t bp6a_i2c_get_rx_fifo_level(bp6a_i2c_priv_t *priv)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    return (uint8_t)((getreg32(&i2c->FIFO_STAT) & I2C_FIFO_STAT_RX_FIFO_LEVEL_MASK) >> I2C_FIFO_STAT_RX_FIFO_LEVEL_SHIFT);
}

static void bp6a_i2c_tx_handle(bp6a_i2c_priv_t *priv)
{
    int xferCount;
    int i;

    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    xferCount = I2C_MAX_FIFO_SIZE - bp6a_i2c_get_tx_fifo_level(priv);

    if (priv->mcnt < xferCount) {
        xferCount = priv->mcnt;
    }

    for (i = 0; i < xferCount; i++) {
        putreg32(&(i2c->TXDATA), priv->mptr[priv->cur_msg++]);
        priv->mcnt--;
    }

    if (priv->mcnt == 0) {
        modifyreg32(&(i2c->INT_EN), I2C_INT_EN_TX_ALMOST_EMPTY_EN_MASK,
                    I2C_INT_EN_TX_ALMOST_EMPTY_EN(0));
    }
}

static void bp6a_i2c_rx_handle(bp6a_i2c_priv_t *priv)
{
    uint32_t fifoCount;
    uint32_t i;
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    fifoCount = bp6a_i2c_get_rx_fifo_level(priv);

    for (i = 0; i < fifoCount; i++) {
        priv->mptr[priv->cur_msg++] = getreg32(&(i2c->RXDATA));
    }
    if (priv->mcnt <= priv->cur_msg)
        modifyreg32(&(i2c->INT_EN), I2C_INT_EN_RX_ALMOST_FULL_EN_MASK,
                    I2C_INT_EN_RX_ALMOST_FULL_EN(0));

}

static void i2c_handler(uint32_t ch)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[ch];
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);
    uint32_t status = getreg32(&(i2c->INT_STAT));

    priv->int_stat |= status;

    if (status & I2C_INT_STAT_TX_ALMOST_EMPTY_MASK) {
        priv->st_slave_rx_master_tx = 1;
        bp6a_i2c_tx_handle(priv);
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_TX_ALMOST_EMPTY_MASK);
    }

    if (status & I2C_INT_STAT_RX_ALMOST_FULL_MASK) {
        priv->st_slave_tx_master_rx = 1;
        bp6a_i2c_rx_handle(priv);
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_RX_ALMOST_FULL_MASK);
    }

    if (status & I2C_INT_STAT_TRAILING_MASK) {
        bp6a_i2c_rx_handle(priv);
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_TRAILING_MASK);
    }

    if (status & I2C_INT_STAT_XFER_DONE_AUTO_MASK) {
        bp6a_i2c_rx_handle(priv);
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_XFER_DONE_AUTO_MASK);
    }

    if (status & I2C_INT_STAT_TX_OVERRUN_MASK) {
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_TX_OVERRUN_MASK);
    }

    if (status & I2C_INT_STAT_RX_OVERRUN_MASK) {
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_RX_OVERRUN_MASK);
    }

    if (status & I2C_INT_STAT_RX_UNDERRUN_MASK) {
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_RX_UNDERRUN_MASK);
    }

    if (status & I2C_INT_STAT_XFER_ABORT_AUTO_MASK) {
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_XFER_ABORT_AUTO_MASK);
    }

    if (status & I2C_INT_STAT_NO_DEV_ACK_AUTO_MASK) {
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_NO_DEV_ACK_AUTO_MASK);
    }

    if (status & I2C_INT_STAT_NO_DEV_AUTO_MASK) {
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_NO_DEV_AUTO_MASK);
    }

    if (status & I2C_INT_STAT_TIMEOUT_AUTO_MASK) {
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_TIMEOUT_AUTO_MASK);
    }

    if (status & I2C_INT_STAT_SLAVE_ADDR_MATCH_SHIFT) {
        putreg32(&(i2c->INT_STAT), I2C_INT_STAT_SLAVE_ADDR_MATCH_MASK);
    }

}

void I2C0_Handler(void)
{
    i2c_handler(0);
    NVIC_ClearPendingIRQ(I2C0_IRQn);
}

void I2C1_Handler(void)
{
    i2c_handler(1);
    NVIC_ClearPendingIRQ(I2C1_IRQn);
}

void I2C2_Handler(void)
{
    i2c_handler(2);
    NVIC_ClearPendingIRQ(I2C2_IRQn);
}

void I2C3_Handler(void)
{
    i2c_handler(3);
    NVIC_ClearPendingIRQ(I2C3_IRQn);
}

void I2C4_Handler(void)
{
    i2c_handler(4);
    NVIC_ClearPendingIRQ(I2C4_IRQn);
}

static void bp6a_i2c_set_interruptmode(bp6a_i2c_priv_t *priv)
{
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    /* disable interrupt */
    uint32_t reg_val = 0;

    putreg32(&(i2c->INT_EN), 0);


    if (!(priv->mode & I2C_INTERRUPT)) {
        return;
    }

    if (priv->master) {
        reg_val = I2C_INT_EN_XFER_DONE_AUTO_EN(1) |
                  I2C_INT_EN_XFER_ABORT_AUTO_EN(1) |
                  I2C_INT_EN_NO_DEV_ACK_AUTO_EN(1) |
                  I2C_INT_EN_NO_DEV_AUTO_EN(1) |
                  I2C_INT_EN_TIMEOUT_AUTO_EN(1);

        if (priv->mode & I2C_M_READ) {
            reg_val |= I2C_INT_EN_RX_ALMOST_FULL_EN(1) |
                       I2C_INT_EN_RX_UNDERRUN_EN(1) |
                       I2C_INT_EN_RX_OVERRUN_EN(1) |
                       I2C_INT_EN_TRAILING_EN(1);

        } else {
            reg_val |= I2C_INT_EN_TX_ALMOST_EMPTY_EN(1) |
                       I2C_INT_EN_TX_UNDERRUN_EN(1) |
                       I2C_INT_EN_TX_OVERRUN_EN(1);
        }
    } else {
        reg_val = I2C_INT_EN_SLAVE_ADDR_MATCH_EN(1) |
                  I2C_INT_EN_TIMEOUT_AUTO_EN(1);

        if (priv->mode & I2C_M_READ) {
            reg_val |=  I2C_INT_EN_TRAILING_EN(1) |
                        I2C_INT_EN_RX_UNDERRUN_EN(1) |
                        I2C_INT_EN_RX_ALMOST_FULL_EN(1) |
                        I2C_INT_EN_RX_OVERRUN_EN(1);
        } else {
            reg_val |=  I2C_INT_EN_TX_ALMOST_EMPTY_EN(1) |
                        I2C_INT_EN_TX_UNDERRUN_EN(1) |
                        I2C_INT_EN_TX_OVERRUN_EN(1);
        }

    }
    putreg32(&(i2c->INT_EN), reg_val);
}

static void bp6a_i2c_enable_isr(bp6a_i2c_priv_t *priv)
{
    NVIC_DisableIRQ((IRQn_Type)(I2C0_IRQn + priv->index));

    if (priv->mode & I2C_INTERRUPT) {
        NVIC_ClearPendingIRQ((IRQn_Type)(I2C0_IRQn + priv->index));

        if (priv->index == 0) {
            NVIC_SetVector((IRQn_Type)(I2C0_IRQn + priv->index), (uint32_t)I2C0_Handler);
        } else if (priv->index == 1) {
            NVIC_SetVector((IRQn_Type)(I2C0_IRQn + priv->index), (uint32_t)I2C1_Handler);
        } else if (priv->index == 2) {
            NVIC_SetVector((IRQn_Type)(I2C0_IRQn + priv->index), (uint32_t)I2C2_Handler);
        } else if (priv->index == 3) {
            NVIC_SetVector((IRQn_Type)(I2C0_IRQn + priv->index), (uint32_t)I2C3_Handler);
        } else if (priv->index == 4) {
            NVIC_SetVector((IRQn_Type)(I2C0_IRQn + priv->index), (uint32_t)I2C4_Handler);
        }

        NVIC_EnableIRQ((IRQn_Type)(I2C0_IRQn + priv->index));
    }
}

static int bp6a_i2c_xfer_slave(bp6a_i2c_priv_t *priv, struct i2c_msg_s *msgv)
{
    priv->slave_addr = msgv->addr;
    bp6a_i2c_set_slave_addr(priv);

    bp6a_i2c_set_auto_config(priv, !!(msgv->flags & I2C_M_NOSTOP),
                             !!(msgv->flags & I2C_M_READ), msgv->length);

    bp6a_i2c_set_fifo_level(priv, DEFAULT_I2C_TX_TRIGLVL, DEFAULT_I2C_RX_TRIGLVL);
    bp6a_i2c_set_channel(priv, !(msgv->flags & I2C_M_NOSTOP), !!(msgv->flags & I2C_M_NOSTOP));

    bp6a_i2c_set_interruptmode(priv);
    bp6a_i2c_run_auto_mode(priv, true);

    return bp6a_i2c_xfer_wait_done_auto(priv);
}

static int bp6a_i2c_xfer_master_auto(bp6a_i2c_priv_t *priv, struct i2c_msg_s *msgv)
{
    priv->slave_addr = msgv->addr;
    bp6a_i2c_set_slave_addr(priv);

    bp6a_i2c_set_auto_config(priv, !!(msgv->flags & I2C_M_NOSTOP),
                             !!(msgv->flags & I2C_M_READ), msgv->length);

    bp6a_i2c_set_fifo_level(priv, DEFAULT_I2C_TX_TRIGLVL, DEFAULT_I2C_RX_TRIGLVL);
    bp6a_i2c_set_channel(priv, !(msgv->flags & I2C_M_NOSTOP), !!(msgv->flags & I2C_M_NOSTOP));

    bp6a_i2c_set_interruptmode(priv);
    bp6a_i2c_run_auto_mode(priv, true);

    return bp6a_i2c_xfer_wait_done_auto(priv);
}

static int bp6a_i2c_transfer(uint32_t index, struct i2c_msg_s *msgv)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];
    priv->mode = BP6A_I2C_DEFAULT_MODE;

    priv->mode |= msgv->flags;

    bp6a_i2c_set_buffer(priv, msgv);

    if (priv->master) {
        if (priv->mode & I2C_AUTO) {
            return bp6a_i2c_xfer_master_auto(priv, msgv);
        } else {
            return -1;
        }
        // return bp6a_i2c_xfer_master_manual(priv, msgv);
    } else {
        return bp6a_i2c_xfer_slave(priv, msgv);
    }
}
/******************************************************************************
* Public function
******************************************************************************/
void bp6a_i2c_master_init(uint32_t index, uint32_t freq, int addr_len)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];

    priv->master = true;
    priv->xfer_speed = freq;
    priv->addrlen = addr_len;
    bp6a_cmu_enable_clock((cmu_clock_t)(CMU_I2C0_CLK + index), true);
    bp6a_i2c_reset(index);

    bp6a_i2c_set_master_addr(priv, I2C_DEFAULT_MASTER_ADDRESS);
    /* Set master mode */
    bp6a_i2c_set_ctl_mode(priv);
    priv->timeout = 0xFFFF;
    bp6a_i2c_set_timeout(priv);
    /* Set speed */
    bp6a_i2c_calculate_timing(priv);
    bp6a_i2c_reset_rxFIFO(priv);
    bp6a_i2c_reset_txFIFO(priv);
    bp6a_i2c_enable_isr(priv);
}

void bp6a_i2c_slave_init(uint32_t index)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];
    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    bp6a_cmu_enable_clock((cmu_clock_t)(CMU_I2C0_CLK + index), true);
    priv->master = false;
    priv->mode |= I2C_AUTO | I2C_INTERRUPT;
    priv->st_slave_tx_master_rx = 0;
    priv->st_slave_rx_master_tx = 0;

    /* Set slave mode */
    bp6a_i2c_set_ctl_mode(priv);

    /* Set slave address */
    bp6a_i2c_set_slave_addr(priv);

    /* Enable stretch-mode */
    modifyreg32(&(i2c->CONF), I2C_CONF_STRCH_EN_MASK, I2C_CONF_STRCH_EN(1));
    /* set tx/rx channel */
    bp6a_i2c_set_channel(priv, true, true);

    /* Enable interrupt */
    bp6a_i2c_set_interruptmode(priv);
}

int bp6a_i2c_get_slave_status(uint32_t index)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];

    if (priv->st_slave_tx_master_rx) {
        priv->st_slave_tx_master_rx = 0;
        return 1;   // Master read
    }

    if (priv->st_slave_rx_master_tx) {
        priv->st_slave_rx_master_tx = 0;
        return 3;   // Master is writing
    }

    return 0;

}

int bp6a_i2c_start(uint32_t index)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];

    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    modifyreg32(&(i2c->MANUAL_CMD), I2C_MANUAL_CMD_SEND_START_MASK,
                I2C_MANUAL_CMD_SEND_START(1));
    return bp6a_i2c_xfer_wait_done_manual(priv);
}

int bp6a_i2c_stop(uint32_t index)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];

    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    modifyreg32(&(i2c->MANUAL_CMD), I2C_MANUAL_CMD_SEND_STOP_MASK,
                I2C_MANUAL_CMD_SEND_STOP(1));
    return bp6a_i2c_xfer_wait_done_manual(priv);
}

void bp6a_i2c_reset(uint32_t index)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];

    BP_I2C_TypeDef *i2c = (BP_I2C_TypeDef *)bp6a_get_i2c_base_addr(priv->index);

    modifyreg32(&(i2c->CTL), I2C_CTL_SW_RST_MASK, I2C_CTL_SW_RST(1));
    _Wait(100);
    modifyreg32(&(i2c->CTL), I2C_CTL_SW_RST_MASK, I2C_CTL_SW_RST(0));
}

int bp6a_i2c_setaddress(uint32_t index, int addr, int nbits)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];

    priv->slave_addr = addr;
    if (nbits == 1) {
        priv->msgv->flags |= I2C_M_TEN;
    }

    return 0;
}

int bp6a_i2c_write_byte(uint32_t index, int data)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];

    return bp6a_i2c_out_byte(priv, data);
}

int bp6a_i2c_read_byte(uint32_t index, bool last)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];

    return bp6a_i2c_in_byte(priv, last);
}

int bp6a_i2c_read(uint32_t index, uint8_t *buffer, int buflen, int start, int stop)
{
    struct i2c_msg_s msg;
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];

    priv->int_stat = 0;
    /* 7- or 10-bit? */
    msg.flags = (priv->addrlen == 10) ? I2C_M_TEN : 0;
    /* Setup for the transfer */
    msg.addr = priv->slave_addr;

    msg.flags |= I2C_M_READ;
    if (start) {
        msg.flags |= I2C_M_NOSTART;
    }
    if (stop) {
        msg.flags |= I2C_M_NOSTOP;
    }
    msg.buffer = (uint8_t *) buffer;
    msg.length = buflen;

    return bp6a_i2c_transfer(index, &msg);
}

int bp6a_i2c_write(uint32_t index, const uint8_t *buffer, int buflen, int start, int stop)
{
    struct i2c_msg_s msg;
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];

    priv->int_stat = 0;
    /* Setup for the transfer */
    msg.addr = priv->slave_addr;
    msg.flags = (priv->addrlen == 10) ? I2C_M_TEN : 0;
    if (start) {
        msg.flags |= I2C_M_NOSTART;
    }
    if (stop) {
        msg.flags |= I2C_M_NOSTOP;
    }
    msg.buffer = (uint8_t *) buffer;    /* Override const */
    msg.length = buflen;

    return bp6a_i2c_transfer(index, &msg);
}

void bp6a_i2c_set_slave_address(uint32_t index, int addr, bool is_slave)
{
    bp6a_i2c_priv_t *priv = &bp6a_i2c_priv[index];

    priv->slave_addr = addr;
    if (is_slave) {
        priv->master = false;
        bp6a_i2c_set_slave_addr(priv);
    }
}

#endif // DEVICE_I2C
