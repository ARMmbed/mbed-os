/*
 * Copyright (c) 2018 Nuvoton Technology Corp. 
 * Copyright (c) 2018 ARM Limited
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
 * Description:   NUC472 MAC driver source file
 */
#include "nuc472_eth.h"
#include "mbed_toolchain.h"
#include "numaker_eth_hal.h"

#define ETH_TRIGGER_RX()    do{EMAC->RXST = 0;}while(0)
#define ETH_TRIGGER_TX()    do{EMAC->TXST = 0;}while(0)
#define ETH_ENABLE_TX()     do{EMAC->CTL |= EMAC_CTL_TXON;}while(0)
#define ETH_ENABLE_RX()     do{EMAC->CTL |= EMAC_CTL_RXON;}while(0)
#define ETH_DISABLE_TX()    do{EMAC->CTL &= ~EMAC_CTL_TXON;}while(0)
#define ETH_DISABLE_RX()    do{EMAC->CTL &= ~EMAC_CTL_RXON;}while(0)
    

MBED_ALIGN(4) struct eth_descriptor rx_desc[RX_DESCRIPTOR_NUM];
MBED_ALIGN(4) struct eth_descriptor tx_desc[TX_DESCRIPTOR_NUM];

struct eth_descriptor volatile *cur_tx_desc_ptr, *cur_rx_desc_ptr, *fin_tx_desc_ptr;

MBED_ALIGN(4) uint8_t rx_buf[RX_DESCRIPTOR_NUM][PACKET_BUFFER_SIZE];
MBED_ALIGN(4) uint8_t tx_buf[TX_DESCRIPTOR_NUM][PACKET_BUFFER_SIZE];

eth_callback_t nu_eth_txrx_cb = NULL;
void *nu_userData = NULL;

extern void ack_emac_rx_isr(void);

// PTP source clock is 84MHz (Real chip using PLL). Each tick is 11.90ns
// Assume we want to set each tick to 100ns.
// Increase register = (100 * 2^31) / (10^9) = 214.71 =~ 215 = 0xD7
// Addend register = 2^32 * tick_freq / (84MHz), where tick_freq = (2^31 / 215) MHz
// From above equation, addend register = 2^63 / (84M * 215) ~= 510707200 = 0x1E70C600



static void mdio_write(uint8_t addr, uint8_t reg, uint16_t val)
{

    EMAC->MIIMDAT = val;
    EMAC->MIIMCTL = (addr << EMAC_MIIMCTL_PHYADDR_Pos) | reg | EMAC_MIIMCTL_BUSY_Msk | EMAC_MIIMCTL_WRITE_Msk | EMAC_MIIMCTL_MDCON_Msk;

    while (EMAC->MIIMCTL & EMAC_MIIMCTL_BUSY_Msk);

}


static uint16_t mdio_read(uint8_t addr, uint8_t reg)
{
    EMAC->MIIMCTL = (addr << EMAC_MIIMCTL_PHYADDR_Pos) | reg | EMAC_MIIMCTL_BUSY_Msk | EMAC_MIIMCTL_MDCON_Msk;
    while (EMAC->MIIMCTL & EMAC_MIIMCTL_BUSY_Msk);

    return(EMAC->MIIMDAT);
}

static int reset_phy(void)
{

    uint16_t reg;
    uint32_t delayCnt;


    mdio_write(CONFIG_PHY_ADDR, MII_BMCR, BMCR_RESET);

    delayCnt = 2000;
    while(delayCnt-- > 0) {
        if((mdio_read(CONFIG_PHY_ADDR, MII_BMCR) & BMCR_RESET) == 0)
            break;

    }

    if(delayCnt == 0) {
        NU_DEBUGF(("Reset phy failed\n"));
        return(-1);
    }

    mdio_write(CONFIG_PHY_ADDR, MII_ADVERTISE, ADVERTISE_CSMA |
               ADVERTISE_10HALF |
               ADVERTISE_10FULL |
               ADVERTISE_100HALF |
               ADVERTISE_100FULL);

    reg = mdio_read(CONFIG_PHY_ADDR, MII_BMCR);
    mdio_write(CONFIG_PHY_ADDR, MII_BMCR, reg | BMCR_ANRESTART);

    delayCnt = 200000;
    while(delayCnt-- > 0) {
        if((mdio_read(CONFIG_PHY_ADDR, MII_BMSR) & (BMSR_ANEGCOMPLETE | BMSR_LSTATUS))
                == (BMSR_ANEGCOMPLETE | BMSR_LSTATUS))
            break;
    }

    if(delayCnt == 0) {
        NU_DEBUGF(("AN failed. Set to 100 FULL\n"));
        EMAC->CTL |= (EMAC_CTL_OPMODE_Msk | EMAC_CTL_FUDUP_Msk);
        return(-1);
    } else {
        reg = mdio_read(CONFIG_PHY_ADDR, MII_LPA);

        if(reg & ADVERTISE_100FULL) {
            NU_DEBUGF(("100 full\n"));
            EMAC->CTL |= (EMAC_CTL_OPMODE_Msk | EMAC_CTL_FUDUP_Msk);
        } else if(reg & ADVERTISE_100HALF) {
            NU_DEBUGF(("100 half\n"));
            EMAC->CTL = (EMAC->CTL & ~EMAC_CTL_FUDUP_Msk) | EMAC_CTL_OPMODE_Msk;
        } else if(reg & ADVERTISE_10FULL) {
            NU_DEBUGF(("10 full\n"));
            EMAC->CTL = (EMAC->CTL & ~EMAC_CTL_OPMODE_Msk) | EMAC_CTL_FUDUP_Msk;
        } else {
            NU_DEBUGF(("10 half\n"));
            EMAC->CTL &= ~(EMAC_CTL_OPMODE_Msk | EMAC_CTL_FUDUP_Msk);
        }
    }
	printf("PHY ID 1:0x%x\r\n", mdio_read(CONFIG_PHY_ADDR, MII_PHYSID1));
	printf("PHY ID 2:0x%x\r\n", mdio_read(CONFIG_PHY_ADDR, MII_PHYSID2));

    return(0);
}


static void init_tx_desc(void)
{
    uint32_t i;


    cur_tx_desc_ptr = fin_tx_desc_ptr = &tx_desc[0];

    for(i = 0; i < TX_DESCRIPTOR_NUM; i++) {
        tx_desc[i].status1 = TXFD_PADEN | TXFD_CRCAPP | TXFD_INTEN;
        tx_desc[i].buf = &tx_buf[i][0];
        tx_desc[i].status2 = 0;
        tx_desc[i].next = &tx_desc[(i + 1) % TX_DESCRIPTOR_NUM];

    }
    EMAC->TXDSA = (unsigned int)&tx_desc[0];
    return;
}

static void init_rx_desc(void)
{
    uint32_t i;


    cur_rx_desc_ptr = &rx_desc[0];

    for(i = 0; i < RX_DESCRIPTOR_NUM; i++) {
        rx_desc[i].status1 = OWNERSHIP_EMAC;
        rx_desc[i].buf = &rx_buf[i][0];
        rx_desc[i].status2 = 0;
        rx_desc[i].next = &rx_desc[(i + 1) % TX_DESCRIPTOR_NUM];
    }
    EMAC->RXDSA = (unsigned int)&rx_desc[0];
    return;
}

void numaker_set_mac_addr(uint8_t *addr)
{

    EMAC->CAM0M = (addr[0] << 24) |
                  (addr[1] << 16) |
                  (addr[2] << 8) |
                  addr[3];

    EMAC->CAM0L = (addr[4] << 24) |
                  (addr[5] << 16);

    EMAC->CAMCTL = EMAC_CAMCTL_CMPEN_Msk | EMAC_CAMCTL_AMP_Msk | EMAC_CAMCTL_ABP_Msk;
    EMAC->CAMEN = 1;    // Enable CAM entry 0

}

static void __eth_clk_pin_init()
{
	 /* Enable IP clock */
    CLK_EnableModuleClock(EMAC_MODULE);
    // Configure MDC clock rate to HCLK / (127 + 1) = 656 kHz if system is running at 84 MHz
    CLK_SetModuleClock(EMAC_MODULE, 0, CLK_CLKDIV3_EMAC(127));
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    // Configure RMII pins
    SYS->GPC_MFPL &= ~( SYS_GPC_MFPL_PC0MFP_Msk | SYS_GPC_MFPL_PC1MFP_Msk |
                    SYS_GPC_MFPL_PC2MFP_Msk | SYS_GPC_MFPL_PC3MFP_Msk |
                    SYS_GPC_MFPL_PC4MFP_Msk | SYS_GPC_MFPL_PC6MFP_Msk | SYS_GPC_MFPL_PC7MFP_Msk ); 
    SYS->GPC_MFPL |= SYS_GPC_MFPL_PC0MFP_EMAC_REFCLK |
                    SYS_GPC_MFPL_PC1MFP_EMAC_MII_RXERR |
                    SYS_GPC_MFPL_PC2MFP_EMAC_MII_RXDV |
                    SYS_GPC_MFPL_PC3MFP_EMAC_MII_RXD1 |
                    SYS_GPC_MFPL_PC4MFP_EMAC_MII_RXD0 |
                    SYS_GPC_MFPL_PC6MFP_EMAC_MII_TXD0 |
                    SYS_GPC_MFPL_PC7MFP_EMAC_MII_TXD1;

    SYS->GPC_MFPH &= ~SYS_GPC_MFPH_PC8MFP_Msk;
    SYS->GPC_MFPH |= SYS_GPC_MFPH_PC8MFP_EMAC_MII_TXEN;
    // Enable high slew rate on all RMII pins
    PC->SLEWCTL |= 0x1DF;

    // Configure MDC, MDIO at PB14 & PB15
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB14MFP_Msk | SYS_GPB_MFPH_PB15MFP_Msk);
    SYS->GPB_MFPH |= SYS_GPB_MFPH_PB14MFP_EMAC_MII_MDC | SYS_GPB_MFPH_PB15MFP_EMAC_MII_MDIO;

}

void numaker_eth_init(uint8_t *mac_addr)
{
	// init CLK & pins
	__eth_clk_pin_init();
	
    // Reset MAC
    EMAC->CTL = EMAC_CTL_RST_Msk;

    init_tx_desc();
    init_rx_desc();

    numaker_set_mac_addr(mac_addr);  // need to reconfigure hardware address 'cos we just RESET emc...

    /* Limit the max receive frame length to 1514 + 4 */
    EMAC->MRFL = NU_ETH_MAX_FLEN;
    reset_phy();

    EMAC->CTL |= EMAC_CTL_STRIPCRC_Msk | EMAC_CTL_RXON_Msk | EMAC_CTL_TXON_Msk | EMAC_CTL_RMIIEN_Msk | EMAC_CTL_RMIIRXCTL_Msk;
    EMAC->INTEN |= EMAC_INTEN_RXIEN_Msk |
                   EMAC_INTEN_RXGDIEN_Msk |
                   EMAC_INTEN_RDUIEN_Msk |
                   EMAC_INTEN_RXBEIEN_Msk |
                   EMAC_INTEN_TXIEN_Msk |
                   EMAC_INTEN_TXABTIEN_Msk |
                   EMAC_INTEN_TXCPIEN_Msk |
                   EMAC_INTEN_TXBEIEN_Msk;
    EMAC->RXST = 0;  // trigger Rx
}



void  ETH_halt(void)
{

    EMAC->CTL &= ~(EMAC_CTL_RXON_Msk | EMAC_CTL_TXON_Msk);
}

unsigned int m_status;

void EMAC_RX_IRQHandler(void)
{
//    NU_DEBUGF(("%s ... nu_eth_txrx_cb=0x%x\r\n", __FUNCTION__, nu_eth_txrx_cb));
    m_status = EMAC->INTSTS & 0xFFFF;
    EMAC->INTSTS = m_status;
    if (m_status & EMAC_INTSTS_RXBEIF_Msk) {
        // Shouldn't goes here, unless descriptor corrupted
		NU_DEBUGF(("RX descriptor corrupted \r\n"));
		//return;
    }
	if (nu_eth_txrx_cb != NULL) nu_eth_txrx_cb('R', nu_userData); 
}


void numaker_eth_trigger_rx(void)
{
    ETH_TRIGGER_RX();
}

int numaker_eth_get_rx_buf(uint16_t *len, uint8_t **buf)
{
    unsigned int cur_entry, status;

    cur_entry = EMAC->CRXDSA;
    if ((cur_entry == (uint32_t)cur_rx_desc_ptr) && (!(m_status & EMAC_INTSTS_RDUIF_Msk)))  // cur_entry may equal to cur_rx_desc_ptr if RDU occures
            return -1;
    status = cur_rx_desc_ptr->status1;

    if(status & OWNERSHIP_EMAC)
            return -1;

    if (status & RXFD_RXGD) {
        *buf = cur_rx_desc_ptr->buf;
        *len = status & 0xFFFF;
    }
    return 0;
}    

void numaker_eth_rx_next(void)
{
    cur_rx_desc_ptr->status1 = OWNERSHIP_EMAC;
    cur_rx_desc_ptr = cur_rx_desc_ptr->next;    
}    

void EMAC_TX_IRQHandler(void)
{
    unsigned int cur_entry, status;

    status = EMAC->INTSTS & 0xFFFF0000;
    EMAC->INTSTS = status;
    if(status & EMAC_INTSTS_TXBEIF_Msk) {
        // Shouldn't goes here, unless descriptor corrupted
        return;
    }

    cur_entry = EMAC->CTXDSA;

    while (cur_entry != (uint32_t)fin_tx_desc_ptr) {

        fin_tx_desc_ptr = fin_tx_desc_ptr->next;
    }
    
    if (nu_eth_txrx_cb != NULL) nu_eth_txrx_cb('T', nu_userData);
}

uint8_t *numaker_eth_get_tx_buf(void)
{
    if(cur_tx_desc_ptr->status1 & OWNERSHIP_EMAC)
        return(NULL);
    else
        return(cur_tx_desc_ptr->buf);
}

void numaker_eth_trigger_tx(uint16_t length, void *p)
{
    struct eth_descriptor volatile *desc;
    cur_tx_desc_ptr->status2 = (unsigned int)length;
    desc = cur_tx_desc_ptr->next;    // in case TX is transmitting and overwrite next pointer before we can update cur_tx_desc_ptr
    cur_tx_desc_ptr->status1 |= OWNERSHIP_EMAC;
    cur_tx_desc_ptr = desc;

    ETH_TRIGGER_TX();

}

int numaker_eth_link_ok(void)
{
	/* first, a dummy read to latch */
	mdio_read(CONFIG_PHY_ADDR, MII_BMSR);
	if(mdio_read(CONFIG_PHY_ADDR, MII_BMSR) & BMSR_LSTATUS)
		return 1;
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
    uint32_t word0 = *(uint32_t *)0x7F804; // 2KB Data Flash at 0x7F800
    // Fetch word 1
    // we only want bottom 16 bits of word1 (MAC bits 32-47)
    // and bit 9 forced to 1, bit 8 forced to 0
    // Locally administered MAC, reduced conflicts
    // http://en.wikipedia.org/wiki/MAC_address
    uint32_t word1 = *(uint32_t *)0x7F800; // 2KB Data Flash at 0x7F800

	if( word0 == 0xFFFFFFFF )		// Not burn any mac address at 1st 2 words of Data Flash
	{
        // with a semi-unique MAC address from the UUID
        /* Enable FMC ISP function */
        SYS_UnlockReg();
        FMC_Open();
        // = FMC_ReadUID(0);
        uID1 = FMC_ReadUID(1);
        word1 = (uID1 & 0x003FFFFF) | ((uID1 & 0x030000) << 6) >> 8;
        word0 = ((FMC_ReadUID(0) >> 4) << 20) | ((uID1 & 0xFF)<<12) | (FMC_ReadUID(2) & 0xFFF);
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
    
    NU_DEBUGF(("mac address %02x-%02x-%02x-%02x-%02x-%02x \r\n", mac[0], mac[1],mac[2],mac[3],mac[4],mac[5]));
}

void numaker_eth_enable_interrupts(void) {
    EMAC->INTEN |= EMAC_INTEN_RXIEN_Msk |
                   EMAC_INTEN_TXIEN_Msk ;
	NVIC_EnableIRQ(EMAC_RX_IRQn);
	NVIC_EnableIRQ(EMAC_TX_IRQn);
}

void numaker_eth_disable_interrupts(void) {
	NVIC_DisableIRQ(EMAC_RX_IRQn);
	NVIC_DisableIRQ(EMAC_TX_IRQn);
}
