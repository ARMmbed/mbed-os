/*
 * Copyright (c) 2022 Nuvoton Technology Corp.
 * Copyright (c) 2022 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
 *
 * Description:   M460 MAC driver source file
 */
//#include <stdbool.h>
#include "m460_eth.h"
#include "mbed_toolchain.h"
//#define NU_TRACE
#include "numaker_eth_hal.h"

#include "synopGMAC_network_interface.h"

#ifdef  NU_TRACE_ISR
#define NU_RAW_Debug(x) { mbed_error_printf x; }
#else
#define NU_RAW_Debug(x)
#endif

#define NU_M460_INTF            0       // Device EMAC Interface port
#define NU_M460_RMII_PIN_GRP    0       // Device EMAC RMII pin group 0 or 1

extern synopGMACdevice GMACdev[GMAC_CNT];
extern struct sk_buff tx_buf[GMAC_CNT][TRANSMIT_DESC_SIZE];
extern struct sk_buff rx_buf[GMAC_CNT][RECEIVE_DESC_SIZE];

eth_callback_t nu_eth_txrx_cb = NULL;
void *nu_userData = NULL;

extern void ack_emac_rx_isr(void);

void plat_delay(uint32_t delay)
{
    volatile uint32_t loop = delay*200;
    while (loop--);
}

static void mdio_write(int addr, int reg, int data)
{
    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];
    synopGMAC_write_phy_reg((u32 *) gmacdev->MacBase, addr, reg, data);
}

static int mdio_read(int addr, int reg)
{
    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];
    uint16_t data;
    synopGMAC_read_phy_reg((u32 *) gmacdev->MacBase, addr, reg, &data);
    return data;
}


static int reset_phy(void)
{

    uint16_t reg;
    uint32_t delayCnt;
    int retVal = 0;
    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];
    
    mdio_write(CONFIG_PHY_ADDR, MII_BMCR, BMCR_RESET);

    delayCnt = 2000;
    while (delayCnt > 0) {
        delayCnt--;
        if ((mdio_read(CONFIG_PHY_ADDR, MII_BMCR) & BMCR_RESET) == 0) {
            break;
        }
    }

    if (delayCnt == 0) {
        NU_DEBUGF(("Reset phy failed\n"));
        return (-1);
    }

    delayCnt = 200000;
    while (delayCnt > 0) {
        delayCnt--;
        if (numaker_eth_link_ok()) {
            gmacdev->LinkState = LINKUP;
            NU_DEBUGF(("Link Up\n"));
            break;
        }
    }    
    if (delayCnt == 0) {
        gmacdev->LinkState = LINKDOWN;
        NU_DEBUGF(("Link Down\n"));
        return (-1);
    }
    
    mdio_write(CONFIG_PHY_ADDR, MII_ADVERTISE, ADVERTISE_CSMA |
               ADVERTISE_10HALF |
               ADVERTISE_10FULL |
               ADVERTISE_100HALF |
               ADVERTISE_100FULL);

    reg = mdio_read(CONFIG_PHY_ADDR, MII_BMCR);
    mdio_write(CONFIG_PHY_ADDR, MII_BMCR, reg | BMCR_ANRESTART);

    delayCnt = 200000;
    while (delayCnt > 0) {
        delayCnt--;
        if ((mdio_read(CONFIG_PHY_ADDR, MII_BMSR) & (BMSR_ANEGCOMPLETE | BMSR_LSTATUS))
                == (BMSR_ANEGCOMPLETE | BMSR_LSTATUS)) {
            break;
        }
    }
    
    if( delayCnt == 0 ) {
        NU_DEBUGF(("AN failed. Set to 100 FULL\n"));
        synopGMAC_set_full_duplex(gmacdev);
        synopGMAC_set_mode(NU_M460_INTF, 1);    // Set mode 1: 100Mbps; 2: 10Mbps
        return (-1);
    } else {
        reg = mdio_read(CONFIG_PHY_ADDR, MII_LPA);
        if (reg & ADVERTISE_100FULL) {
            NU_DEBUGF(("100 full\n"));
            gmacdev->DuplexMode = FULLDUPLEX;
            gmacdev->Speed = SPEED100;
            synopGMAC_set_full_duplex(gmacdev);
            synopGMAC_set_mode(NU_M460_INTF, 1);    // Set mode 1: 100Mbps; 2: 10Mbps
        } else if (reg & ADVERTISE_100HALF) {
            NU_DEBUGF(("100 half\n"));
            gmacdev->DuplexMode = HALFDUPLEX;
            gmacdev->Speed = SPEED100;
            synopGMAC_set_half_duplex(gmacdev);
            synopGMAC_set_mode(NU_M460_INTF, 1);    // Set mode 1: 100Mbps; 2: 10Mbps
        } else if (reg & ADVERTISE_10FULL) {
            NU_DEBUGF(("10 full\n"));
            gmacdev->DuplexMode = FULLDUPLEX;
            gmacdev->Speed = SPEED10;
            synopGMAC_set_full_duplex(gmacdev);
            synopGMAC_set_mode(NU_M460_INTF, 2);    // Set mode 1: 100Mbps; 2: 10Mbps
        } else {
            NU_DEBUGF(("10 half\n"));
            gmacdev->DuplexMode = HALFDUPLEX;
            gmacdev->Speed = SPEED10;
            synopGMAC_set_half_duplex(gmacdev);
            synopGMAC_set_mode(NU_M460_INTF, 2);    // Set mode 1: 100Mbps; 2: 10Mbps
        }
    }    
    
    printf("PHY ID 1:0x%x\r\n", mdio_read(CONFIG_PHY_ADDR, MII_PHYSID1));
    printf("PHY ID 2:0x%x\r\n", mdio_read(CONFIG_PHY_ADDR, MII_PHYSID2));

    return (retVal);
    
}


void numaker_set_mac_addr(uint8_t *addr)
{

    synopGMAC_set_mac_address(NU_M460_INTF, addr);

}

static void __set_mac_pins(uint32_t group)
{
    if(group == 0)
    {
        SET_EMAC0_RMII_MDC_PE8();
        SET_EMAC0_RMII_MDIO_PE9();
        SET_EMAC0_RMII_TXD0_PE10();
        SET_EMAC0_RMII_TXD1_PE11();
        SET_EMAC0_RMII_TXEN_PE12();    
        SET_EMAC0_RMII_REFCLK_PC8();
        SET_EMAC0_RMII_RXD0_PC7();
        SET_EMAC0_RMII_RXD1_PC6();
        SET_EMAC0_RMII_CRSDV_PA7();
        SET_EMAC0_RMII_RXERR_PA6();
        
        SET_EMAC0_PPS_PB6();
    }
    else
    {
        SET_EMAC0_RMII_MDC_PB11();
        SET_EMAC0_RMII_MDIO_PB10();
        SET_EMAC0_RMII_TXD0_PB9();
        SET_EMAC0_RMII_TXD1_PB8();
        SET_EMAC0_RMII_TXEN_PB7();    
        SET_EMAC0_RMII_REFCLK_PB5();
        SET_EMAC0_RMII_RXD0_PB4();
        SET_EMAC0_RMII_RXD1_PB3();
        SET_EMAC0_RMII_CRSDV_PB2();
        SET_EMAC0_RMII_RXERR_PB1();
        
        SET_EMAC0_PPS_PE13();
    }
}

static void __eth_clk_pin_init()
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable IP clock */
    CLK_EnableModuleClock(EMAC0_MODULE);

    // Configure MDC clock rate

    /* Update System Core Clock */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    // Configure RMII pins
    __set_mac_pins(NU_M460_RMII_PIN_GRP);
    
    /* Lock protected registers */
    SYS_LockReg();

}

void numaker_eth_init(uint8_t *mac_addr)
{
    int status = 0;
    int retval = 0;
    int i;
    uint32_t offload_needed = 0;    
    uint32_t dma_addr;
    struct sk_buff *skb;
    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];

    NVIC_DisableIRQ(EMAC0_TXRX_IRQn);    
    /* init CLK & pins */
    __eth_clk_pin_init();

    /*Attach the device to MAC struct This will configure all the required base addresses
      such as Mac base, configuration base, phy base address(out of 32 possible phys )*/
    synopGMAC_attach(gmacdev, GMAC0MappedAddr + MACBASE, GMAC0MappedAddr + DMABASE, DEFAULT_PHY_BASE);
    
    // Reset MAC
    synopGMAC_reset(gmacdev);
    gmacdev->Intf = NU_M460_INTF;
    synopGMAC_read_version(gmacdev);
    
    /*Check for Phy initialization*/
    synopGMAC_set_mdc_clk_div(gmacdev,GmiiCsrClk5);
    gmacdev->ClockDivMdc = synopGMAC_get_mdc_clk_div(gmacdev);
    
    /*Reset PHY*/
    //status = synopGMAC_check_phy_init(gmacdev);
    status = reset_phy();

    /*Set up the tx and rx descriptor queue/ring*/
    synopGMAC_setup_tx_desc_queue(gmacdev,TRANSMIT_DESC_SIZE, RINGMODE);
    synopGMAC_init_tx_desc_base(gmacdev);	//Program the transmit descriptor base address in to DmaTxBase addr

    synopGMAC_setup_rx_desc_queue(gmacdev,RECEIVE_DESC_SIZE, RINGMODE);
    synopGMAC_init_rx_desc_base(gmacdev);	//Program the transmit descriptor base address in to DmaTxBase addr

    /*Initialize the dma interface*/
    synopGMAC_dma_bus_mode_init(gmacdev, DmaBurstLength32 | DmaDescriptorSkip0/*DmaDescriptorSkip2*/ | DmaDescriptor8Words ); synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward |DmaTxSecondFrame|DmaRxThreshCtrl128);

    /*Initialize the mac interface*/
    synopGMAC_mac_init(gmacdev);
    synopGMAC_promisc_enable(gmacdev);

    synopGMAC_pause_control(gmacdev); // This enables the pause control in Full duplex mode of operation

#if defined(NU_USING_HW_CHECKSUM)
    /*IPC Checksum offloading is enabled for this driver. Should only be used if Full Ip checksumm offload engine is configured in the hardware*/
    offload_needed = 1;
    synopGMAC_enable_rx_chksum_offload(gmacdev);  	//Enable the offload engine in the receive path
    synopGMAC_rx_tcpip_chksum_drop_enable(gmacdev); // This is default configuration, DMA drops the packets if error in encapsulated ethernet payload
#endif

    for(i = 0; i < RECEIVE_DESC_SIZE; i ++) {
        skb = &rx_buf[NU_M460_INTF][i];
        synopGMAC_set_rx_qptr(gmacdev, (u32)((u64)(skb->data) & 0xFFFFFFFF), sizeof(skb->data), (u32)((u64)skb & 0xFFFFFFFF));
    }

    for(i = 0; i < TRANSMIT_DESC_SIZE; i ++) {
        skb = &tx_buf[NU_M460_INTF][i];
        synopGMAC_set_tx_qptr(gmacdev, (u32)((u64)(skb->data) & 0xFFFFFFFF), sizeof(skb->data), (u32)((u64)skb & 0xFFFFFFFF), offload_needed ,0);
    }    

    numaker_set_mac_addr(mac_addr);  // need to reconfigure hardware address 'cos we just RESET emc...
    
    synopGMAC_clear_interrupt(gmacdev);

    synopGMAC_enable_interrupt(gmacdev,DmaIntEnable);
    synopGMAC_enable_dma_rx(gmacdev);
    synopGMAC_enable_dma_tx(gmacdev);
    
    synopGMAC_tx_enable(gmacdev);
    synopGMAC_rx_enable(gmacdev);
    NVIC_EnableIRQ(EMAC0_TXRX_IRQn);
}



void  ETH_halt(void)
{
    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];
    
    synopGMAC_tx_disable(gmacdev);
    synopGMAC_rx_disable(gmacdev);
}

unsigned int m_status;

/*----------------------------------------------------------------------------
  EMAC IRQ Handler
 *----------------------------------------------------------------------------*/
void EMAC0_IRQHandler(void)
{
    synopGMACdevice * gmacdev = &GMACdev[0];
    uint32_t interrupt,dma_status_reg, mac_status_reg;
    int status;
    uint32_t dma_addr;

    // Check GMAC interrupt
    mac_status_reg = synopGMACReadReg((u32 *)gmacdev->MacBase, GmacInterruptStatus);
    if(mac_status_reg & GmacTSIntSts) {
    	gmacdev->synopGMACNetStats.ts_int = 1;
    	status = synopGMACReadReg((u32 *)gmacdev->MacBase, GmacTSStatus);
    	if(!(status & (1 << 1))) {
    		NU_RAW_Debug(("TS alarm flag not set??\n"));
    	} else {
    		NU_RAW_Debug(("TS alarm!!!!!!!!!!!!!!!!\n"));
        }
    }
    if(mac_status_reg & GmacLPIIntSts) {
    	//NU_RAW_Debug("LPI\n");
    	//LPIStsChange = 1;
    	//LPIReg = synopGMACReadReg((u32 *)gmacdev->MacBase, GmacLPICtrlSts);
        ;
    }
    if(mac_status_reg & GmacRgmiiIntSts) {
    	uint32_t volatile reg;
    	reg = synopGMACReadReg((u32 *)gmacdev->MacBase, GmacRgmiiCtrlSts);

    }
    synopGMACWriteReg((u32 *)gmacdev->MacBase, GmacInterruptStatus ,mac_status_reg);

    /*Read the Dma interrupt status to know whether the interrupt got generated by our device or not*/
    dma_status_reg = synopGMACReadReg((u32 *)gmacdev->DmaBase, DmaStatus);
    NU_RAW_Debug(("i %08x %08x\n", mac_status_reg, dma_status_reg));

    if(dma_status_reg == 0)
        return;

    synopGMAC_disable_interrupt_all(gmacdev);

    NU_RAW_Debug(("%s:Dma Status Reg: 0x%08x\n",__FUNCTION__,dma_status_reg));

    if(dma_status_reg & GmacPmtIntr) {
        NU_RAW_Debug(("%s:: Interrupt due to PMT module\n",__FUNCTION__));
        synopGMAC_powerup_mac(gmacdev);
    }

    if(dma_status_reg & GmacLineIntfIntr) {
        NU_RAW_Debug(("%s:: Interrupt due to GMAC LINE module\n",__FUNCTION__));
    }

    /*Now lets handle the DMA interrupts*/
    interrupt = synopGMAC_get_interrupt_type(gmacdev);
    NU_RAW_Debug(("%s:Interrupts to be handled: 0x%08x\n",__FUNCTION__,interrupt));

    if(interrupt & synopGMACDmaError) {
        uint8_t hwaddr[6];
        mbed_mac_address((char *)hwaddr);
        NU_RAW_Debug(("%s::Fatal Bus Error Interrupt Seen\n",__FUNCTION__));
        synopGMAC_disable_dma_tx(gmacdev);
        synopGMAC_disable_dma_rx(gmacdev);

        synopGMAC_take_desc_ownership_tx(gmacdev);
        synopGMAC_take_desc_ownership_rx(gmacdev);

        synopGMAC_init_tx_rx_desc_queue(gmacdev);

        synopGMAC_reset(gmacdev);//reset the DMA engine and the GMAC ip
        synopGMAC_set_mac_address(NU_M460_INTF, (uint8_t*)hwaddr);
        synopGMAC_dma_bus_mode_init(gmacdev,DmaFixedBurstEnable| DmaBurstLength8 | DmaDescriptorSkip0/*DmaDescriptorSkip2*/ );
        synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward);
        synopGMAC_init_rx_desc_base(gmacdev);
        synopGMAC_init_tx_desc_base(gmacdev);
        synopGMAC_mac_init(gmacdev);
        synopGMAC_enable_dma_rx(gmacdev);
        synopGMAC_enable_dma_tx(gmacdev);

    }

    if(interrupt & synopGMACDmaRxNormal) {
    	//NU_RAW_Debug(("rx\n"));
        NU_RAW_Debug(("%s:: Rx Normal \r\n", __FUNCTION__));
        // to handle received data
        if (nu_eth_txrx_cb != NULL) {
            nu_eth_txrx_cb('R', nu_userData);
        }
    }

    if(interrupt & synopGMACDmaRxAbnormal) {
        mbed_error_printf("%s::Abnormal Rx Interrupt Seen \r\n",__FUNCTION__);
    	gmacdev->synopGMACNetStats.rx_over_errors++;
        if(gmacdev->GMAC_Power_down == 0) {	// If Mac is not in powerdown
            synopGMAC_resume_dma_rx(gmacdev);//To handle GBPS with 12 descriptors
        }
    }

    if(interrupt & synopGMACDmaRxStopped) {
        mbed_error_printf("%s::Receiver stopped seeing Rx interrupts \r\n",__FUNCTION__); //Receiver gone in to stopped state
        if(gmacdev->GMAC_Power_down == 0) {	// If Mac is not in powerdown
        	gmacdev->synopGMACNetStats.rx_over_errors++;
            synopGMAC_enable_dma_rx(gmacdev);
        }
    }

    if(interrupt & synopGMACDmaTxNormal) {
        //NU_RAW_Debug(("rx\n"));
        //xmit function has done its job
        NU_RAW_Debug(("%s::Finished Normal Transmission \n",__FUNCTION__));
        synop_handle_transmit_over(0);//Do whatever you want after the transmission is over
        if (nu_eth_txrx_cb != NULL) {
            nu_eth_txrx_cb('T', nu_userData);
        }
    }

    if(interrupt & synopGMACDmaTxAbnormal) {
        mbed_error_printf("%s::Abnormal Tx Interrupt Seen\n",__FUNCTION__);
        if(gmacdev->GMAC_Power_down == 0) {	// If Mac is not in powerdown
            synop_handle_transmit_over(0);
            if (nu_eth_txrx_cb != NULL) {
                nu_eth_txrx_cb('T', nu_userData);
            }
        }
    }

    if(interrupt & synopGMACDmaTxStopped) {
        mbed_error_printf("%s::Transmitter stopped sending the packets\n",__FUNCTION__);
        if(gmacdev->GMAC_Power_down == 0) {	// If Mac is not in powerdown
            synopGMAC_disable_dma_tx(gmacdev);
            synopGMAC_take_desc_ownership_tx(gmacdev);

            synopGMAC_enable_dma_tx(gmacdev);
            mbed_error_printf("%s::Transmission Resumed\n",__FUNCTION__);
        }
    }

    /* Enable the interrupt before returning from ISR*/
//    if( !(interrupt & synopGMACDmaRxNormal)) {  /* RxNormal will enable INT in numaker_eth_trigger_rx */
        synopGMAC_enable_interrupt(gmacdev,DmaIntEnable);
//    }
    return;
}

static void dump_desc(DmaDesc *desc)
{
#if 1
    NU_DEBUGF(("###--- %s... Desc[0x%08x] status[0x%08x] ---###\r\n", __FUNCTION__, desc, desc->status));
    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];
    DmaDesc *tmp_desc;
#endif
#if 0
    int i=0;
    for ( i=0; i< RECEIVE_DESC_SIZE; i++ ) {
        tmp_desc = gmacdev->RxDesc + i;
        NU_DEBUGF(("### RX %s... desc[0x%08x]  status[0x%08x]   length[0x%08x]  buffer1[0x%08x]  buffer2[0x%08x]\r\n", 
        __FUNCTION__, tmp_desc, tmp_desc->status, tmp_desc->length, tmp_desc->buffer1, tmp_desc->buffer2)); 
        //NU_DEBUGF(("### %s... length[0x%x]\r\n", __FUNCTION__, desc->length));   
        //NU_DEBUGF(("### %s... buffer1[0x%x]\r\n", __FUNCTION__, desc->buffer1));   
        //NU_DEBUGF(("### %s... buffer2[0x%x]\r\n", __FUNCTION__, desc->buffer2));   
    }
    for ( i=0; i< TRANSMIT_DESC_SIZE; i++ ) {
        tmp_desc = gmacdev->TxDesc + i;
        NU_DEBUGF(("### TX %s... desc[0x%08x]  status[0x%08x]   length[0x%08x]  buffer1[0x%08x]  buffer2[0x%08x]\r\n", 
        __FUNCTION__, tmp_desc, tmp_desc->status, tmp_desc->length, tmp_desc->buffer1, tmp_desc->buffer2));
    }        
#endif
}

void numaker_eth_trigger_rx(void)
{
    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];
    NU_DEBUGF(("=== %s...  ---Start---\r\n", __FUNCTION__));
    dump_desc(gmacdev->RxBusyDesc);
    /* Enable the interrupt */
    synopGMAC_enable_interrupt(gmacdev,DmaIntEnable);
    /* Trigger RX DMA */
    synopGMAC_enable_dma_rx(gmacdev);
    synopGMAC_resume_dma_rx(gmacdev);
    NU_DEBUGF(("%s... resume RX DMA\r\n", __FUNCTION__));
    NU_DEBUGF(("=== %s...  ---End---\r\n", __FUNCTION__));
}

int numaker_eth_get_rx_buf(uint16_t *len, uint8_t **buf)
{

    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];    
    NU_DEBUGF(("=== %s... ---Start---\r\n", __FUNCTION__));
    DmaDesc * rxdesc = gmacdev->RxBusyDesc;
    dump_desc(rxdesc);
    if(synopGMAC_is_desc_owned_by_dma(rxdesc))
        return -1;
    if(synopGMAC_is_desc_empty(rxdesc))
        return -1;
    
//    synopGMAC_disable_dma_rx(gmacdev);    // it will encounter DMA interrupt status as "Receiver stopped seeing Rx interrupts"
    *len = synop_handle_received_data(NU_M460_INTF, buf);
    dump_desc(gmacdev->RxBusyDesc);
    if( *len <= 0 ) return -1; /* No available RX frame */

    // length of payload should be <= 1514
    if (*len > (NU_ETH_MAX_FLEN - 4)) {
        NU_DEBUGF(("%s... unexpected long packet length=%d, buf=0x%x\r\n", __FUNCTION__, *len, *buf));
        *len = 0; // Skip this unexpected long packet
    }
    NU_DEBUGF(("=== %s...  ---End---\r\n", __FUNCTION__));  
    return 0;
}

void numaker_eth_rx_next(void)
{
    NU_DEBUGF(("=== %s... ---Start---\r\n", __FUNCTION__));
    /* Already did in synop_handle_received_data */
    /* No-op at this stage */    
#if 0    
    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];
    DmaDesc * rxdesc = (gmacdev->RxBusyDesc - 1);

	rxdesc->status = DescOwnByDma;
#endif

}

uint8_t *numaker_eth_get_tx_buf(void)
{
    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];
    DmaDesc * txdesc = gmacdev->TxNextDesc;

    if(!synopGMAC_is_desc_empty(txdesc)) {
        return  (NULL);
    }
    
    if (synopGMAC_is_desc_owned_by_dma(txdesc))
    {
        return (NULL);
    } else {
        dump_desc(txdesc);
        return (uint8_t *) (txdesc->buffer1);
    }
        
}

void numaker_eth_trigger_tx(uint16_t length, void *p)
{
    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];
    DmaDesc * txdesc = gmacdev->TxNextDesc;
    uint32_t  txnext = gmacdev->TxNext;
#if defined(NU_USING_HW_CHECKSUM)
    uint32_t offload_needed = 1;
#else
    uint32_t offload_needed = 0;
#endif
    
    (gmacdev->BusyTxDesc)++; //busy tx descriptor is incremented by one as it will be handed over to DMA    
    txdesc->length |= ((length <<DescSize1Shift) & DescSize1Mask);
    txdesc->status |=  (DescTxFirst | DescTxLast | DescTxIntEnable ); //ENH_DESC

    if(offload_needed) {
        /*
         Make sure that the OS you are running supports the IP and TCP checksum offloading,
         before calling any of the functions given below.
        */
        synopGMAC_tx_checksum_offload_tcp_pseudo(gmacdev, txdesc);
    } else {
      	synopGMAC_tx_checksum_offload_bypass(gmacdev, txdesc);
    }
    __DSB();
    txdesc->status |= DescOwnByDma;//ENH_DESC

    gmacdev->TxNext = synopGMAC_is_last_tx_desc(gmacdev,txdesc) ? 0 : txnext + 1;
    gmacdev->TxNextDesc = synopGMAC_is_last_tx_desc(gmacdev,txdesc) ? gmacdev->TxDesc : (txdesc + 1);
    
    /* Enable the interrupt */
    synopGMAC_enable_interrupt(gmacdev,DmaIntEnable);    
    /* Trigger TX DMA */    
    synopGMAC_resume_dma_tx(gmacdev);
//    synopGMAC_enable_dma_tx(gmacdev);

}

int numaker_eth_link_ok(void)
{
    /* first, a dummy read to latch */
    mdio_read(CONFIG_PHY_ADDR, MII_BMSR);
    if (mdio_read(CONFIG_PHY_ADDR, MII_BMSR) & BMSR_LSTATUS) {
        return 1;
    }
    return 0;
}

void numaker_eth_set_cb(eth_callback_t eth_cb, void *userData)
{
    nu_eth_txrx_cb =  eth_cb;
    nu_userData = userData;
}

// Override mbed_mac_address of mbed_interface.c to provide ethernet devices with a semi-unique MAC address
void mbed_mac_address(char *mac)
{
    uint32_t uID1;
    // Fetch word 0
    uint32_t word0 = *(uint32_t *)0xFF804; // 2KB Data Flash at 0xFF800
    // Fetch word 1
    // we only want bottom 16 bits of word1 (MAC bits 32-47)
    // and bit 9 forced to 1, bit 8 forced to 0
    // Locally administered MAC, reduced conflicts
    // http://en.wikipedia.org/wiki/MAC_address
    uint32_t word1 = *(uint32_t *)0xFF800; // 2KB Data Flash at 0xFF800

    if (word0 == 0xFFFFFFFF) {       // Not burn any mac address at 1st 2 words of Data Flash
        // with a semi-unique MAC address from the UUID
        /* Enable FMC ISP function */
        SYS_UnlockReg();
        FMC_Open();
        // = FMC_ReadUID(0);
        uID1 = FMC_ReadUID(1);
        word1 = (uID1 & 0x003FFFFF) | ((uID1 & 0x030000) << 6) >> 8;
        word0 = ((FMC_ReadUID(0) >> 4) << 20) | ((uID1 & 0xFF) << 12) | (FMC_ReadUID(2) & 0xFFF);
        /* Disable FMC ISP function */
        FMC_Close();
        /* Lock protected registers */
        SYS_LockReg();
    }

    word1 |= 0x00000200;
    word1 &= 0x0000FEFF;

    mac[0] = (word1 & 0x0000ff00) >> 8;
    mac[1] = (word1 & 0x000000ff);
    mac[2] = (word0 & 0xff000000) >> 24;
    mac[3] = (word0 & 0x00ff0000) >> 16;
    mac[4] = (word0 & 0x0000ff00) >> 8;
    mac[5] = (word0 & 0x000000ff);

    NU_DEBUGF(("mac address %02x-%02x-%02x-%02x-%02x-%02x \r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]));
}

void numaker_eth_enable_interrupts(void)
{
    synopGMACdevice *gmacdev = &GMACdev[NU_M460_INTF];

    /* Enable the interrupt */
    synopGMAC_enable_interrupt(gmacdev,DmaIntEnable);

    NVIC_EnableIRQ(EMAC0_TXRX_IRQn);
}

void numaker_eth_disable_interrupts(void)
{
    NVIC_DisableIRQ(EMAC0_TXRX_IRQn);
}
