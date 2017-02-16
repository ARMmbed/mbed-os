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

#ifndef CONFIG_MBED_ENABLED
#include "spi_ex_api.h"
#endif

#include "PinNames.h"
#include "pinmap.h"
#include "hal_ssi.h"

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


void spi_init (spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    SSI_DBG_ENTRANCE("spi_init()\n");

    uint32_t ssi_mosi, ssi_miso, ssi_peri;
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

#ifndef CONFIG_MBED_ENABLED  
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
#endif
}

void spi_free (spi_t *obj)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;    
    pHalSsiAdaptor = &obj->spi_adp;
    HalSsiDeInit(pHalSsiAdaptor);

    SPI0_MULTI_CS_CTRL(OFF);
#ifndef CONFIG_MBED_ENABLED    
#ifdef CONFIG_GDMA_EN
    if (obj->dma_en & SPI_DMA_RX_EN) {
        HalSsiRxGdmaDeInit(pHalSsiAdaptor);
    }

    if (obj->dma_en & SPI_DMA_TX_EN) {
        HalSsiTxGdmaDeInit(pHalSsiAdaptor);
    }    
    obj->dma_en = 0;
#endif
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
        }
        else {
            DBG_SSI_ERR("The SPI%d cannot work as Slave mode, only SPI0 does.\r\n", pHalSsiAdaptor->Index);
            pHalSsiAdaptor->Role = SSI_MASTER;
        }
    }
    else {
        pHalSsiAdaptor->Role = SSI_MASTER;
    }
    pHalSsiOp->HalSsiSetDeviceRole(pHalSsiAdaptor, pHalSsiAdaptor->Role);

#ifdef CONFIG_GPIO_EN
    if (pHalSsiAdaptor->Role == SSI_SLAVE) {
        if (pHalSsiAdaptor->SclkPolarity == SCPOL_INACTIVE_IS_LOW) {
            HAL_GPIO_PullCtrl((u32)obj->sclk, hal_PullDown);
        }
        else {
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

#ifndef CONFIG_MBED_ENABLED
void spi_slave_select(spi_t *obj, ChipSelect slaveindex)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    u8 Index;

    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;
    Index = pHalSsiAdaptor->Index;
    
    if((pHalSsiAdaptor->Role == SSI_MASTER) && (Index == 0)){
        pHalSsiOp->HalSsiSetSlaveEnableRegister((VOID*)pHalSsiAdaptor,slaveindex);
        if(slaveindex != CS_0){
            SPI0_MULTI_CS_CTRL(ON);
        }
    }
    else{
        DBG_SSI_ERR("Only SPI 0  master mode supports slave selection.\n");
    }
}

void spi_slave_select_bypin(spi_t *obj, PinName pinname)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    u8 Index;
    u8 slaveindex = 8;
    u8 pinmux;

    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;
    Index = pHalSsiAdaptor->Index;
    pinmux = pHalSsiAdaptor->PinmuxSelect;
    
    if((pHalSsiAdaptor->Role == SSI_MASTER) && (Index == 0)){
        if(pinmux == S0){
            switch (pinname){
                case PE_0:
                    slaveindex = CS_0;
                    break;
                case PE_4:
                    slaveindex = CS_1;
                    break;
                case PE_5:
                    slaveindex = CS_2;
                    break;
                case PE_6:
                    slaveindex = CS_3;
                    break;
                case PE_7:
                    slaveindex = CS_4;
                    break;
                case PE_8:
                    slaveindex = CS_5;
                    break;
                case PE_9:
                    slaveindex = CS_6;
                    break;
                case PE_A:
                    slaveindex = CS_7;
                    break;
                default:
                    slaveindex = 8;
            }
        }
        
        if(pinmux == S1){
            switch (pinname){
                case PC_0:
                    slaveindex = CS_0;
                    break;
                case PC_4:
                    slaveindex = CS_1;
                    break;
                case PC_5:
                    slaveindex = CS_2;
                    break;
                case PC_6:
                    slaveindex = CS_3;
                    break;
                case PC_7:
                    slaveindex = CS_4;
                    break;
                case PC_8:
                    slaveindex = CS_5;
                    break;
                case PC_9:
                    slaveindex = CS_6;
                    break;
                default:
                    slaveindex = 8;
            }
        }

        if(slaveindex != 8){
            pHalSsiOp->HalSsiSetSlaveEnableRegister((VOID*)pHalSsiAdaptor,slaveindex);
            if(slaveindex != CS_0){
                SPI0_MULTI_CS_CTRL(ON);
            }
        }
        else
            DBG_SSI_ERR("Wrong Chip Seleect Pin.\n");
        
    }
    else{
        DBG_SSI_ERR("Only SPI 0  master mode supports slave selection.\n");
    }
}
#endif

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

#ifndef CONFIG_MBED_ENABLED
//Discard data in the rx fifo, SPI bus can observe these data
void spi_flush_rx_fifo (spi_t *obj)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    u32 rx_fifo_level;
    u32 i;

    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    while(pHalSsiOp->HalSsiReadable(pHalSsiAdaptor)){
        rx_fifo_level = pHalSsiOp->HalSsiGetRxFifoLevel(pHalSsiAdaptor);
        for(i=0;i<rx_fifo_level;i++) {
            pHalSsiOp->HalSsiRead(pHalSsiAdaptor);
        }
    }
}

//This function is only for the slave device to flush tx fifo
//It will discard all data in both tx fifo and rx fifo, then reset the state of slave tx. 
//Data in the tx & rx fifo will be dropped without being able to be observed from SPI bus
void spi_slave_flush_fifo(spi_t *obj)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    u8 index;

    pHalSsiAdaptor = &obj->spi_adp;
    index = pHalSsiAdaptor->Index;
    if(index != 0){
        DBG_SSI_ERR("spi %x is not a slave\n", index);
    }
    HalSsiClearFIFO(pHalSsiAdaptor);
    obj->state &= ~SPI_STATE_TX_BUSY;
}

// Slave mode read a sequence of data by interrupt mode
int32_t spi_slave_read_stream(spi_t *obj, char *rx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int32_t ret;

    if (obj->state & SPI_STATE_RX_BUSY) {
        DBG_SSI_WARN("spi_slave_read_stream: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    obj->state |= SPI_STATE_RX_BUSY;
    if ((ret=pHalSsiOp->HalSsiReadInterrupt(pHalSsiAdaptor, rx_buffer, length)) != HAL_OK) {
        obj->state &= ~SPI_STATE_RX_BUSY;
    }

    return ret;
}

// Slave mode write a sequence of data by interrupt mode
int32_t spi_slave_write_stream(spi_t *obj, char *tx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int32_t ret;

    if (obj->state & SPI_STATE_TX_BUSY) {
        DBG_SSI_WARN("spi_slave_write_stream: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    obj->state |= SPI_STATE_TX_BUSY;
    if ((ret=pHalSsiOp->HalSsiWriteInterrupt(pHalSsiAdaptor, (u8 *) tx_buffer, length)) != HAL_OK) {
        obj->state &= ~SPI_STATE_TX_BUSY;
    }
    return ret;
}

// Master mode read a sequence of data by interrupt mode
// The length unit is byte, for both 16-bits and 8-bits mode
int32_t spi_master_read_stream(spi_t *obj, char *rx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int32_t ret;

    if (obj->state & SPI_STATE_RX_BUSY) {
        DBG_SSI_WARN("spi_master_read_stream: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;
    // wait bus idle
    while(pHalSsiOp->HalSsiBusy(pHalSsiAdaptor));

    obj->state |= SPI_STATE_RX_BUSY;
    if ((ret=pHalSsiOp->HalSsiReadInterrupt(pHalSsiAdaptor, rx_buffer, length)) == HAL_OK) {
        /* as Master mode, it need to push data to TX FIFO to generate clock out 
           then the slave can transmit data out */
        // send some dummy data out
        if ((ret=pHalSsiOp->HalSsiWriteInterrupt(pHalSsiAdaptor, NULL, length)) != HAL_OK) {
            obj->state &= ~SPI_STATE_RX_BUSY;
        }
    }
    else {
        obj->state &= ~SPI_STATE_RX_BUSY;
    }

    return ret;
}

// Master mode write a sequence of data by interrupt mode
// The length unit is byte, for both 16-bits and 8-bits mode
int32_t spi_master_write_stream(spi_t *obj, char *tx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int32_t ret;

    if (obj->state & SPI_STATE_TX_BUSY) {
        DBG_SSI_WARN("spi_master_write_stream: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    obj->state |= SPI_STATE_TX_BUSY;
    /* as Master mode, sending data will receive data at sametime, so we need to
       drop those received dummy data */
    if ((ret=pHalSsiOp->HalSsiWriteInterrupt(pHalSsiAdaptor, (u8 *) tx_buffer, length)) != HAL_OK) {
        obj->state &= ~SPI_STATE_TX_BUSY;
    }
    return ret;
}

// Master mode write a sequence of data by interrupt mode
// The length unit is byte, for both 16-bits and 8-bits mode
int32_t spi_master_write_read_stream(spi_t *obj, char *tx_buffer, 
        char *rx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int32_t ret;

    if (obj->state & (SPI_STATE_RX_BUSY|SPI_STATE_TX_BUSY)) {
        DBG_SSI_WARN("spi_master_write_and_read_stream: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;
    // wait bus idle
    while(pHalSsiOp->HalSsiBusy(pHalSsiAdaptor));

    obj->state |= SPI_STATE_RX_BUSY;
    /* as Master mode, sending data will receive data at sametime */
    if ((ret=pHalSsiOp->HalSsiReadInterrupt(pHalSsiAdaptor, rx_buffer, length)) == HAL_OK) {
        obj->state |= SPI_STATE_TX_BUSY;
        if ((ret=pHalSsiOp->HalSsiWriteInterrupt(pHalSsiAdaptor, (u8 *) tx_buffer, length)) != HAL_OK) {
            obj->state &= ~(SPI_STATE_RX_BUSY|SPI_STATE_TX_BUSY);
            // Disable RX IRQ
            pHalSsiAdaptor->InterruptMask &= ~(BIT_IMR_RXFIM | BIT_IMR_RXOIM | BIT_IMR_RXUIM);
            pHalSsiOp->HalSsiSetInterruptMask((VOID*)pHalSsiAdaptor);
        }
    }
    else {
        obj->state &= ~(SPI_STATE_RX_BUSY);
    }
    
    return ret;
}

int32_t spi_slave_read_stream_timeout(spi_t *obj, char *rx_buffer, uint32_t length, uint32_t timeout_ms)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int ret,timeout = 0;
    uint32_t StartCount, TimeoutCount = 0;

    if (obj->state & SPI_STATE_RX_BUSY) {
        DBG_SSI_WARN("spi_slave_read_stream: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    obj->state |= SPI_STATE_RX_BUSY;
    HalSsiEnterCritical(pHalSsiAdaptor);
    if ((ret=pHalSsiOp->HalSsiReadInterrupt(pHalSsiAdaptor, rx_buffer, length)) != HAL_OK) {
        obj->state &= ~SPI_STATE_RX_BUSY;
    }
    HalSsiExitCritical(pHalSsiAdaptor);
    
    if ((ret == HAL_OK) && (timeout_ms > 0)) {
        TimeoutCount = (timeout_ms*1000/TIMER_TICK_US);
        StartCount = HalTimerOp.HalTimerReadCount(1);
        while (obj->state & SPI_STATE_RX_BUSY) {
            if (HAL_TIMEOUT == HalSsiTimeout(StartCount, TimeoutCount)) {
                ret = HalSsiStopRecv(pHalSsiAdaptor);
                obj->state &= ~ SPI_STATE_RX_BUSY;
                timeout = 1;
                DBG_SSI_INFO("Slave is timeout\n");
                break;
            }
        }
        if ((pHalSsiAdaptor->DataFrameSize+1) > 8){
            pHalSsiAdaptor->RxLength <<= 1;
        }

        if(timeout)
            return (length - pHalSsiAdaptor->RxLength);
        else
            return length;
    } 
    else {
        return (-ret);
    }
}

int32_t spi_slave_read_stream_terminate(spi_t *obj, char *rx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    volatile u8 csterminate;
    int ret;
    volatile u32 spistate;

    csterminate = 0;
    if (obj->state & SPI_STATE_RX_BUSY) {
        DBG_SSI_WARN("spi_slave_read_stream_dma: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;
    

    obj->state |= SPI_STATE_RX_BUSY;
    HalSsiEnterCritical(pHalSsiAdaptor);
    if ((ret=pHalSsiOp->HalSsiReadInterrupt(pHalSsiAdaptor, rx_buffer, length)) != HAL_OK) {
        obj->state &= ~SPI_STATE_RX_BUSY;
    }
    HalSsiExitCritical(pHalSsiAdaptor);


    while(obj->state & SPI_STATE_RX_BUSY){
        spistate = pHalSsiOp->HalSsiGetStatus(pHalSsiAdaptor);
        while((spistate & 0x1) == 1){
            if((obj->state & SPI_STATE_RX_BUSY) == 0){   
                csterminate = 0;
                break;
            }   
            spistate = pHalSsiOp->HalSsiGetStatus(pHalSsiAdaptor);
            if((spistate & 0x1) == 0){
                ret = HalSsiStopRecv(pHalSsiAdaptor); 
                goto EndOfCS;
            }
        }
    }
EndOfCS:
    if((obj->state & SPI_STATE_RX_BUSY) != 0){ 
        csterminate = 1;
        obj->state &= ~ SPI_STATE_RX_BUSY;
    }

    if ((pHalSsiAdaptor->DataFrameSize+1) > 8){
        pHalSsiAdaptor->RxLength <<= 1;
    }    
    
    if(csterminate == 1)
        return (length - pHalSsiAdaptor->RxLength);
    else
        return length;

}

// Bus Idle: Real TX done, TX FIFO empty and bus shift all data out already
void spi_bus_tx_done_callback(VOID *obj)
{
    spi_t *spi_obj = (spi_t *)obj;
    spi_irq_handler handler;

    if (spi_obj->bus_tx_done_handler) {
        handler = (spi_irq_handler)spi_obj->bus_tx_done_handler;
        handler(spi_obj->bus_tx_done_irq_id, 0);
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

void spi_irq_hook(spi_t *obj, spi_irq_handler handler, uint32_t id) 
{
    obj->irq_handler = (u32)handler;
    obj->irq_id = (u32)id;
}

void spi_bus_tx_done_irq_hook(spi_t *obj, spi_irq_handler handler, uint32_t id) 
{
    obj->bus_tx_done_handler = (u32)handler;
    obj->bus_tx_done_irq_id = (u32)id;
}

void spi_enable(spi_t *obj)
{
    PHAL_SSI_ADAPTOR pHalSsiAdapter;
    pHalSsiAdapter = &obj->spi_adp;

    HalSsiEnable((VOID*)pHalSsiAdapter);
}

void spi_disable(spi_t *obj)
{
    PHAL_SSI_ADAPTOR pHalSsiAdapter;
    pHalSsiAdapter = &obj->spi_adp;

    HalSsiDisable((VOID*)pHalSsiAdapter);

}
#ifdef CONFIG_GDMA_EN    
int32_t spi_slave_read_stream_dma(spi_t *obj, char *rx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int32_t ret;

    if (obj->state & SPI_STATE_RX_BUSY) {
        DBG_SSI_WARN("spi_slave_read_stream_dma: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    if ((obj->dma_en & SPI_DMA_RX_EN)==0) {
        if (HAL_OK == HalSsiRxGdmaInit(pHalSsiOp, pHalSsiAdaptor)) {
            obj->dma_en |= SPI_DMA_RX_EN;
        }
        else {
            return HAL_BUSY;
        }
    }
    
    obj->state |= SPI_STATE_RX_BUSY;
    ret = HalSsiDmaRecv(pHalSsiAdaptor, (u8 *) rx_buffer, length);
    if (ret != HAL_OK) {
        obj->state &= ~SPI_STATE_RX_BUSY;
    }
    return (ret);
}

int32_t spi_slave_write_stream_dma(spi_t *obj, char *tx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int32_t ret;

    if (obj->state & SPI_STATE_TX_BUSY) {
        DBG_SSI_WARN("spi_slave_write_stream_dma: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    if ((obj->dma_en & SPI_DMA_TX_EN)==0) {
        if (HAL_OK == HalSsiTxGdmaInit(pHalSsiOp, pHalSsiAdaptor)) {
            obj->dma_en |= SPI_DMA_TX_EN;
        }
        else {
            return HAL_BUSY;
        }
    }
    obj->state |= SPI_STATE_TX_BUSY;
    ret = HalSsiDmaSend(pHalSsiAdaptor, (u8 *) tx_buffer, length);
    if (ret != HAL_OK) {
        obj->state &= ~SPI_STATE_TX_BUSY;
    }
    return (ret);
}

int32_t spi_master_write_read_stream_dma(spi_t *obj, char *tx_buffer, 
        char *rx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int32_t ret;

    if (obj->state & (SPI_STATE_RX_BUSY|SPI_STATE_TX_BUSY)) {
        DBG_SSI_WARN("spi_master_write_and_read_stream: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;
    if ((obj->dma_en & SPI_DMA_TX_EN)==0) {
        if (HAL_OK == HalSsiTxGdmaInit(pHalSsiOp, pHalSsiAdaptor)) {
            obj->dma_en |= SPI_DMA_TX_EN;
        }
        else {
            return HAL_BUSY;
        }
    }
    
    if ((obj->dma_en & SPI_DMA_RX_EN)==0) {
        if (HAL_OK == HalSsiRxGdmaInit(pHalSsiOp, pHalSsiAdaptor)) {
            obj->dma_en |= SPI_DMA_RX_EN;
        }
        else {
            return HAL_BUSY;
        }
    }
    
    obj->state |= SPI_STATE_RX_BUSY;
    /* as Master mode, sending data will receive data at sametime */
    if ((ret=HalSsiDmaRecv(pHalSsiAdaptor, (u8 *) rx_buffer, length)) == HAL_OK) {
        obj->state |= SPI_STATE_TX_BUSY;
        if ((ret=HalSsiDmaSend(pHalSsiAdaptor, (u8 *) tx_buffer, length)) != HAL_OK) {
            obj->state &= ~(SPI_STATE_RX_BUSY|SPI_STATE_TX_BUSY);
        }
    }
    else {
        obj->state &= ~(SPI_STATE_RX_BUSY);
    }    

    return ret;
}

int32_t spi_master_read_stream_dma(spi_t *obj, char *rx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int32_t ret;

    if (obj->state & SPI_STATE_RX_BUSY) {
        DBG_SSI_WARN("spi_master_read_stream_dma: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    if ((obj->dma_en & SPI_DMA_RX_EN)==0) {
        if (HAL_OK == HalSsiRxGdmaInit(pHalSsiOp, pHalSsiAdaptor)) {
            obj->dma_en |= SPI_DMA_RX_EN;
        }
        else {
            return HAL_BUSY;
        }
    }
    
    obj->state |= SPI_STATE_RX_BUSY;
    ret = HalSsiDmaRecv(pHalSsiAdaptor, (u8 *) rx_buffer, length);
    if (ret != HAL_OK) {
        obj->state &= ~SPI_STATE_RX_BUSY;
    }

    // for master mode, we need to send data to generate clock out
    if (obj->dma_en & SPI_DMA_TX_EN) {
        // TX DMA is on already, so use DMA to TX data
        // Make the GDMA to use the rx_buffer too
        ret = HalSsiDmaSend(pHalSsiAdaptor, (u8 *) rx_buffer, length);
        if (ret != HAL_OK) {
            obj->state &= ~SPI_STATE_RX_BUSY;
        }
    }
    else {
        // TX DMA isn't enabled, so we just use Interrupt mode to TX dummy data
        if ((ret=pHalSsiOp->HalSsiWriteInterrupt(pHalSsiAdaptor, NULL, length)) != HAL_OK) {
            obj->state &= ~SPI_STATE_RX_BUSY;
        }        
    }

    return ret;
}

int32_t spi_master_write_stream_dma(spi_t *obj, char *tx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int32_t ret;

    if (obj->state & SPI_STATE_TX_BUSY) {
        DBG_SSI_WARN("spi_master_write_stream_dma: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    if ((obj->dma_en & SPI_DMA_TX_EN)==0) {
        if (HAL_OK == HalSsiTxGdmaInit(pHalSsiOp, pHalSsiAdaptor)) {
            obj->dma_en |= SPI_DMA_TX_EN;
        }
        else {
            return HAL_BUSY;
        }
    }
    
    obj->state |= SPI_STATE_TX_BUSY;
    ret = HalSsiDmaSend(pHalSsiAdaptor, (u8 *) tx_buffer, length);
    if (ret != HAL_OK) {
        obj->state &= ~SPI_STATE_TX_BUSY;
    }

    return ret;
}

int32_t spi_slave_read_stream_dma_timeout(spi_t *obj, char *rx_buffer, uint32_t length, uint32_t timeout_ms)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    int ret,timeout = 0;
    uint32_t StartCount, TimeoutCount = 0;


    if (obj->state & SPI_STATE_RX_BUSY) {
        DBG_SSI_WARN("spi_slave_read_stream_dma: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;

    if ((obj->dma_en & SPI_DMA_RX_EN)==0) {
        if (HAL_OK == HalSsiRxGdmaInit(pHalSsiOp, pHalSsiAdaptor)) {
            obj->dma_en |= SPI_DMA_RX_EN;
        }
        else {
            return HAL_BUSY;
        }
    }
    
    obj->state |= SPI_STATE_RX_BUSY;
    HalSsiEnterCritical(pHalSsiAdaptor);
    ret = HalSsiDmaRecv(pHalSsiAdaptor, (u8 *) rx_buffer, length);    
    HalSsiExitCritical(pHalSsiAdaptor);

    if ((ret == HAL_OK) && (timeout_ms > 0)) {
        TimeoutCount = (timeout_ms*1000/TIMER_TICK_US);
        StartCount = HalTimerOp.HalTimerReadCount(1);
        while (obj->state & SPI_STATE_RX_BUSY) {
             if (HAL_TIMEOUT == HalSsiTimeout(StartCount, TimeoutCount)) {
                ret = HalSsiStopRecv(pHalSsiAdaptor);               
                obj->state &= ~ SPI_STATE_RX_BUSY;
                timeout = 1;
                DBG_SSI_INFO("Slave is timeout\n");
                break;
            }
        }

        if(timeout)
            return (length - pHalSsiAdaptor->RxLength);
        else
            return length;
               
    } 
    else {
        obj->state &= ~ SPI_STATE_RX_BUSY;
        return (-ret);
    }
}

int32_t spi_slave_read_stream_dma_terminate(spi_t *obj, char *rx_buffer, uint32_t length)
{
    PHAL_SSI_ADAPTOR pHalSsiAdaptor;
    PHAL_SSI_OP pHalSsiOp;
    volatile u8 csterminate;
    int ret;
    volatile u32 spistate;

    csterminate = 0;
    if (obj->state & SPI_STATE_RX_BUSY) {
        DBG_SSI_WARN("spi_slave_read_stream_dma: state(0x%x) is not ready\r\n", 
            obj->state);
        return HAL_BUSY;
    }
    
    pHalSsiAdaptor = &obj->spi_adp;
    pHalSsiOp = &obj->spi_op;
    
    if ((obj->dma_en & SPI_DMA_RX_EN)==0) {
        if (HAL_OK == HalSsiRxGdmaInit(pHalSsiOp, pHalSsiAdaptor)) {
            obj->dma_en |= SPI_DMA_RX_EN;
        }
        else {
            return HAL_BUSY;
        }
    }


    obj->state |= SPI_STATE_RX_BUSY;
    HalSsiEnterCritical(pHalSsiAdaptor);
    ret = HalSsiDmaRecv(pHalSsiAdaptor, (u8 *) rx_buffer, length);    
    HalSsiExitCritical(pHalSsiAdaptor);

    while(obj->state & SPI_STATE_RX_BUSY){
        spistate = pHalSsiOp->HalSsiGetStatus(pHalSsiAdaptor);
        while((spistate & 0x1) == 1){
            if((obj->state & SPI_STATE_RX_BUSY) == 0){   
                csterminate = 0;
                break;
            }   
            spistate = pHalSsiOp->HalSsiGetStatus(pHalSsiAdaptor);
            if((spistate & 0x1) == 0){
                ret = HalSsiStopRecv(pHalSsiAdaptor); 
                goto EndOfDMACS;
            }
        }
    }
EndOfDMACS:
    if((obj->state & SPI_STATE_RX_BUSY) != 0){ 
        csterminate = 1;
        obj->state &= ~ SPI_STATE_RX_BUSY;
    }
    
    if(csterminate == 1)
        return (length - pHalSsiAdaptor->RxLength);
    else
        return length;

}
#endif

#endif  // end of "#ifdef CONFIG_GDMA_EN"
