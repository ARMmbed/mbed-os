/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "objects.h"
#include "spi_api.h"
#include "spi_ex_api.h"
#include "PinNames.h"
#include "pinmap.h"
#include "hal_ssi.h"

#ifdef CONFIG_MBED_ENABLED
#include "platform_stdlib.h"
#endif

extern u32 SystemGetCpuClk(VOID);
extern VOID HAL_GPIO_PullCtrl(u32 pin, u32 mode);

void spi_tx_done_callback(VOID *obj);
void spi_rx_done_callback(VOID *obj);
void spi_bus_tx_done_callback(VOID *obj);

#ifdef CONFIG_GDMA_EN
HAL_GDMA_OP SpiGdmaOp;
#endif

uint8_t SPI0_IS_AS_SLAVE = 0;

//TODO: Load default Setting: It should be loaded from external setting file.
extern const DW_SSI_DEFAULT_SETTING SpiDefaultSetting;

#ifdef CONFIG_MBED_ENABLED
#include "PeripheralPins.h"
#else
static const PinMap PinMap_SSI_MOSI[] = {
    {PE_2,  RTL_PIN_PERI(SPI0, 0, S0), RTL_PIN_FUNC(SPI0, S0)},
    {PC_2,  RTL_PIN_PERI(SPI0, 0, S1), RTL_PIN_FUNC(SPI0, S1)},
    {PA_1,  RTL_PIN_PERI(SPI1, 1, S0), RTL_PIN_FUNC(SPI1, S0)},
    {PB_6,  RTL_PIN_PERI(SPI1, 1, S1), RTL_PIN_FUNC(SPI1, S1)},
    {PD_6,  RTL_PIN_PERI(SPI1, 1, S2), RTL_PIN_FUNC(SPI1, S2)},
    {PG_2,  RTL_PIN_PERI(SPI2, 2, S0), RTL_PIN_FUNC(SPI2, S0)},
    {PE_6,  RTL_PIN_PERI(SPI2, 2, S1), RTL_PIN_FUNC(SPI2, S1)},
    {PD_2,  RTL_PIN_PERI(SPI2, 2, S2), RTL_PIN_FUNC(SPI2, S2)},
    {NC,    NC,     0}
};

static const PinMap PinMap_SSI_MISO[] = {
    {PE_3,  RTL_PIN_PERI(SPI0, 0, S0), RTL_PIN_FUNC(SPI0, S0)},
    {PC_3,  RTL_PIN_PERI(SPI0, 0, S1), RTL_PIN_FUNC(SPI0, S1)},
    {PA_0,  RTL_PIN_PERI(SPI1, 1, S0), RTL_PIN_FUNC(SPI1, S0)},
    {PB_7,  RTL_PIN_PERI(SPI1, 1, S1), RTL_PIN_FUNC(SPI1, S1)},
    {PD_7,  RTL_PIN_PERI(SPI1, 1, S2), RTL_PIN_FUNC(SPI1, S2)},
    {PG_3,  RTL_PIN_PERI(SPI2, 2, S0), RTL_PIN_FUNC(SPI2, S0)},
    {PE_7,  RTL_PIN_PERI(SPI2, 2, S1), RTL_PIN_FUNC(SPI2, S1)},
    {PD_3,  RTL_PIN_PERI(SPI2, 2, S2), RTL_PIN_FUNC(SPI2, S2)},
    {NC,    NC,     0}
};

// For testing only
static const PinMap PinMap_SSI_SCLK[] = {
    {PC_1,  RTL_PIN_PERI(SPI0, 0, S1), /* Unused */ 0},
    {PA_2,  RTL_PIN_PERI(SPI1, 1, S0), /* Unused */ 0},
    {PA_2,  RTL_PIN_PERI(SPI1, 1, S1), /* Unused */ 0},
    {PA_2,  RTL_PIN_PERI(SPI1, 1, S2), /* Unused */ 0},
    {PD_5,  RTL_PIN_PERI(SPI1, 1, S0), /* Unused */ 0},
    {PD_5,  RTL_PIN_PERI(SPI1, 1, S1), /* Unused */ 0},
    {PD_5,  RTL_PIN_PERI(SPI1, 1, S2), /* Unused */ 0},

    {NC,    NC,     0}
};

// For testing only
static const PinMap PinMap_SSI_SSEL[] = {
    {PE_5,  RTL_PIN_PERI(SPI0, 0, S2), /* Unused */ 0},
    {PC_5,  RTL_PIN_PERI(SPI0, 0, S2), /* Unused */ 0},
    {PC_4,  RTL_PIN_PERI(SPI0, 0, S1), /* Unused */ 0},
    {PC_0,  RTL_PIN_PERI(SPI0, 0, S0), /* Unused */ 0},
    {PA_4,  RTL_PIN_PERI(SPI1, 1, S0), /* Unused */ 0},
    {PD_4,  RTL_PIN_PERI(SPI1, 1, S0), /* Unused */ 0},

    {NC,    NC,     0}
};
#endif

void spi_init (spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    SSI_DBG_ENTRANCE("spi_init()\n");

    int ssi_mosi, ssi_miso, ssi_peri;
    uint8_t  ssi_idx, ssi_pinmux;
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;

    _memset((void*)obj, 0, sizeof(spi_t));
    obj->state = 0;

    uint32_t SystemClock = SystemGetCpuClk();
    uint32_t MaxSsiFreq  = (SystemClock >> 2) >> 1;

    /* SsiClockDivider doesn't support odd number */
    DBG_SSI_INFO("SystemClock: %d\n", SystemClock);
    DBG_SSI_INFO("MaxSsiFreq : %d\n", MaxSsiFreq);

    ssi_mosi = pinmap_peripheral(mosi, PinMap_SSI_MOSI);
    ssi_miso = pinmap_peripheral(miso, PinMap_SSI_MISO);

    ssi_peri = pinmap_merge(ssi_mosi, ssi_miso);
    if (unlikely(ssi_peri == NC)) {
        DBG_SSI_ERR("spi_init(): Cannot find matched SSI index.\n");
        return;
    }
    obj->sclk = (u8)sclk;
    ssi_idx = RTL_GET_PERI_IDX(ssi_peri);
    ssi_pinmux = RTL_GET_PERI_SEL(ssi_peri);
    DBG_SSI_INFO("ssi_peri: %d, ssi_idx: %d, ssi_pinmux: %d\n", ssi_peri, ssi_idx, ssi_pinmux);

    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    pHalSsiAdaptor->Index = ssi_idx;
    pHalSsiAdaptor->PinmuxSelect = ssi_pinmux;
    pHalSsiAdaptor->Role = SSI_MASTER;

    HalSsiOpInit((VOID*)pHalSsiOp);

    pHalSsiOp->HalSsiSetDeviceRole(pHalSsiAdaptor, pHalSsiAdaptor->Role);

    /* Pinmux workaround */
    if ((ssi_idx == 0) && (ssi_pinmux == SSI0_MUX_TO_GPIOC)) {
        EEPROM_PIN_CTRL(OFF);
    }

    if ((ssi_idx == 0) && (ssi_pinmux == SSI0_MUX_TO_GPIOE)) {
            DBG_SSI_WARN(ANSI_COLOR_MAGENTA"SPI0 Pin may conflict with JTAG\r\n"ANSI_COLOR_RESET);        
    }

    //TODO: Implement default setting structure.
    pHalSsiOp->HalSsiLoadSetting(pHalSsiAdaptor, (void*)&SpiDefaultSetting);
    pHalSsiAdaptor->DefaultRxThresholdLevel = SpiDefaultSetting.RxThresholdLevel;

    if(HalSsiInit(pHalSsiAdaptor) != HAL_OK){
        DBG_SSI_ERR(ANSI_COLOR_RED"spi_init(): SPI %x init fails.\n"ANSI_COLOR_RESET,pHalSsiAdaptor->Index);
        return;        
    }

    pHalSsiAdaptor->TxCompCallback = spi_tx_done_callback;
    pHalSsiAdaptor->TxCompCbPara = (void*)obj;
    pHalSsiAdaptor->RxCompCallback = spi_rx_done_callback;
    pHalSsiAdaptor->RxCompCbPara = (void*)obj;
    pHalSsiAdaptor->TxIdleCallback = spi_bus_tx_done_callback;
    pHalSsiAdaptor->TxIdleCbPara = (void*)obj;

#ifdef CONFIG_GDMA_EN
    HalGdmaOpInit((VOID*)&SpiGdmaOp);
    pHalSsiAdaptor->DmaConfig.pHalGdmaOp = &SpiGdmaOp;
    pHalSsiAdaptor->DmaConfig.pRxHalGdmaAdapter = &obj->spi_gdma_adp_rx;
    pHalSsiAdaptor->DmaConfig.pTxHalGdmaAdapter = &obj->spi_gdma_adp_tx;
    obj->dma_en = 0;
    pHalSsiAdaptor->HaveTxChannel = 0;
    pHalSsiAdaptor->HaveRxChannel = 0;
#endif
}

void spi_free (spi_t *obj)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;    
    pHalSsiAdaptor = &obj->spi_adp;
    HalSsiDeInit(pHalSsiAdaptor);

    SPI0_MULTI_CS_CTRL(OFF);

#ifdef CONFIG_GDMA_EN
    if (obj->dma_en & SPI_DMA_RX_EN) {
        HalSsiRxGdmaDeInit(pHalSsiAdaptor);
    }

    if (obj->dma_en & SPI_DMA_TX_EN) {
        HalSsiTxGdmaDeInit(pHalSsiAdaptor);
    }    
    obj->dma_en = 0;
#endif
}

void spi_format (spi_t *obj, int bits, int mode, int slave)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;

    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    pHalSsiAdaptor->DataFrameSize = (bits - 1);

    /*
     * mode | POL PHA
     * -----+--------
     *   0  |  0   0
     *   1  |  0   1
     *   2  |  1   0
     *   3  |  1   1
     *
     * SCPOL_INACTIVE_IS_LOW  = 0,
     * SCPOL_INACTIVE_IS_HIGH = 1
     *
     * SCPH_TOGGLES_IN_MIDDLE = 0,
     * SCPH_TOGGLES_AT_START  = 1
     */
    switch (mode)
    {
        case 0:
            pHalSsiAdaptor->SclkPolarity = SCPOL_INACTIVE_IS_LOW;
            pHalSsiAdaptor->SclkPhase    = SCPH_TOGGLES_IN_MIDDLE;
            break;
        case 1:
            pHalSsiAdaptor->SclkPolarity = SCPOL_INACTIVE_IS_LOW;
            pHalSsiAdaptor->SclkPhase    = SCPH_TOGGLES_AT_START;
            break;
        case 2:
            pHalSsiAdaptor->SclkPolarity = SCPOL_INACTIVE_IS_HIGH;
            pHalSsiAdaptor->SclkPhase    = SCPH_TOGGLES_IN_MIDDLE;
            break;
        case 3:
            pHalSsiAdaptor->SclkPolarity = SCPOL_INACTIVE_IS_HIGH;
            pHalSsiAdaptor->SclkPhase    = SCPH_TOGGLES_AT_START;
            break;
        default:  // same as 3
            pHalSsiAdaptor->SclkPolarity = SCPOL_INACTIVE_IS_HIGH;
            pHalSsiAdaptor->SclkPhase    = SCPH_TOGGLES_AT_START;
            break;
    }

    if (slave == 1) {
        if (pHalSsiAdaptor->Index == 0) {
            pHalSsiAdaptor->Role = SSI_SLAVE;
            pHalSsiAdaptor->SlaveOutputEnable = SLV_TXD_ENABLE;  // <-- Slave only
            SPI0_IS_AS_SLAVE = 1;
            DBG_SSI_INFO("SPI0 is as slave\n");
        } else {
            DBG_SSI_ERR("The SPI%d cannot work as Slave mode, only SPI0 does.\r\n", pHalSsiAdaptor->Index);
            pHalSsiAdaptor->Role = SSI_MASTER;
        }
    } else {
        pHalSsiAdaptor->Role = SSI_MASTER;
    }
    pHalSsiOp->HalSsiSetDeviceRole(pHalSsiAdaptor, pHalSsiAdaptor->Role);

#ifdef CONFIG_GPIO_EN
    if (pHalSsiAdaptor->Role == SSI_SLAVE) {
        if (pHalSsiAdaptor->SclkPolarity == SCPOL_INACTIVE_IS_LOW) {
            HAL_GPIO_PullCtrl((u32)obj->sclk, hal_PullDown);
        } else {
            HAL_GPIO_PullCtrl((u32)obj->sclk, hal_PullUp);
        }
    }
#endif
    HalSsiSetFormat(pHalSsiAdaptor);
}

void spi_frequency (spi_t *obj, int hz)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;

    pHalSsiAdaptor = &obj->spi_adp;
    HalSsiSetSclk(pHalSsiAdaptor, (u32)hz);
}

static inline void ssi_write (spi_t *obj, int value)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;

    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    while (!pHalSsiOp->HalSsiWriteable(pHalSsiAdaptor));
    pHalSsiOp->HalSsiWrite((VOID*)pHalSsiAdaptor, value);
}

static inline int ssi_read(spi_t *obj)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;

    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    while (!pHalSsiOp->HalSsiReadable(pHalSsiAdaptor));
    return (int)pHalSsiOp->HalSsiRead(pHalSsiAdaptor);
}

int spi_master_write (spi_t *obj, int value)
{
    ssi_write(obj, value);
    return ssi_read(obj);
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    int total = (tx_length > rx_length) ? tx_length : rx_length;
    int i;
    char out, in;

    for (i = 0; i < total; i++) {
        out = (i < tx_length) ? tx_buffer[i] : write_fill;
        in = spi_master_write(obj, out);
        if (i < rx_length) {
            rx_buffer[i] = in;
        }
    }

    return total;
}

int spi_slave_receive (spi_t *obj)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int Readable;
    int Busy;

    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    Readable = pHalSsiOp->HalSsiReadable(pHalSsiAdaptor);
    Busy     = (int)pHalSsiOp->HalSsiBusy(pHalSsiAdaptor);
    return ((Readable && !Busy) ? 1 : 0);
}

int spi_slave_read (spi_t *obj)
{
    return ssi_read(obj);
}

void spi_slave_write (spi_t *obj, int value)
{
    ssi_write(obj, value);
}

int spi_busy (spi_t *obj)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;

    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    return (int)pHalSsiOp->HalSsiBusy(pHalSsiAdaptor);
}

// Bus Idle: Real TX done, TX FIFO empty and bus shift all data out already
void spi_bus_tx_done_callback(VOID *obj)
{
    spi_t *spi_obj = (spi_t *)obj;
    spi_irq_handler handler;

    if (spi_obj->bus_tx_done_handler) {
        handler = (spi_irq_handler)spi_obj->bus_tx_done_handler;
        handler(spi_obj->bus_tx_done_irq_id, (SpiIrq)0);
    }    
}

void spi_tx_done_callback(VOID *obj)
{
    spi_t *spi_obj = (spi_t *)obj;
    spi_irq_handler handler;

    if (spi_obj->state & SPI_STATE_TX_BUSY) {
        spi_obj->state &= ~SPI_STATE_TX_BUSY;
        if (spi_obj->irq_handler) {
            handler = (spi_irq_handler)spi_obj->irq_handler;
            handler(spi_obj->irq_id, SpiTxIrq);
        }
    }
}

void spi_rx_done_callback(VOID *obj)
{
    spi_t *spi_obj = (spi_t *)obj;
    spi_irq_handler handler;

    spi_obj->state &= ~SPI_STATE_RX_BUSY;
    if (spi_obj->irq_handler) {
        handler = (spi_irq_handler)spi_obj->irq_handler;
        handler(spi_obj->irq_id, SpiRxIrq);
    }
}

const PinMap *spi_master_mosi_pinmap()
{
    return PinMap_SSI_MOSI;
}

const PinMap *spi_master_miso_pinmap()
{
    return PinMap_SSI_MISO;
}

const PinMap *spi_master_clk_pinmap()
{
    return PinMap_SSI_SCLK;
}

const PinMap *spi_master_cs_pinmap()
{
    return PinMap_SSI_SSEL;
}

const PinMap *spi_slave_mosi_pinmap()
{
    return PinMap_SSI_MOSI;
}

const PinMap *spi_slave_miso_pinmap()
{
    return PinMap_SSI_MISO;
}

const PinMap *spi_slave_clk_pinmap()
{
    return PinMap_SSI_SCLK;
}

const PinMap *spi_slave_cs_pinmap()
{
    return PinMap_SSI_SSEL;
}

