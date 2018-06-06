/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 ARM Limited
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

/*
 * Code implementation file for the LAN Ethernet interface.
 *
 * This file is the based on mps2_ethernet_api and Selftest's ETH_MPS2.
 * MPS2 Selftest:https://silver.arm.com/browse/VEI10 ->
 *     \ISCM-1-0\AN491\software\Selftest\v2m_mps2\
 */

#include <stdio.h>

#include "mbed_retarget.h"
#include "mbed_wait_api.h"
#include "CM3DS.h"
#include "smsc9220_eth.h"

#define REG_WRITE_TIME_OUT  50
#define RESET_TIME_OUT      10
#define PHY_RESET_TIME_OUT_MS   100

/* Forward declarations */

static unsigned int smsc9220_mac_regread(unsigned char regoffset, unsigned int *data);
static unsigned int smsc9220_mac_regwrite(unsigned char regoffset, unsigned int data);
static unsigned int smsc9220_phy_regread(unsigned char regoffset, unsigned int *data);
static unsigned int smsc9220_phy_regwrite(unsigned char regoffset, unsigned int data);

static unsigned int smsc9220_read_id(void);
static unsigned int smsc9220_soft_reset(void);
static void smsc9220_set_txfifo(unsigned int val);
static unsigned int smsc9220_wait_eeprom(void);
static void smsc9220_init_irqs(void);
static unsigned int smsc9220_check_phy(void);
static unsigned int smsc9220_reset_phy(void);

static void smsc9220_advertise_cap(void);
static void smsc9220_enable_xmit(void);
static void smsc9220_enable_mac_xmit(void);
static void smsc9220_enable_mac_recv(void);

/* SMSC9220 low-level operations */

/**
 * \brief Read MAC register.
 *
 * \param[in] regoffset Register offset
 * \param[out] data Register value is read
 *
 * \return 0 in case of success, 1 otherwise
 */
static unsigned int smsc9220_mac_regread(unsigned char regoffset, unsigned int *data)
{
    unsigned int val = 0;
    unsigned int maccmd = 0;
    int time_out = REG_WRITE_TIME_OUT;

    val = SMSC9220->MAC_CSR_CMD;
    if(!(val & ((unsigned int)1 << 31))) {    /* Make sure there's no pending operation */
        maccmd |= regoffset;
        maccmd |= ((unsigned int)1 << 30);     /* Indicates read */
        maccmd |= ((unsigned int)1 << 31);     /* Start bit */
        SMSC9220->MAC_CSR_CMD = maccmd;        /* Start operation */

        do {
            val = SMSC9220->BYTE_TEST;  /* A no-op read. */
            wait_ms(1);
            time_out--;
        } while(time_out && (SMSC9220->MAC_CSR_CMD & ((unsigned int)1 << 31)));

        if(!time_out) {
            return 1;
        }
        else {
            *data = SMSC9220->MAC_CSR_DATA;
        }
    } else {
        *data = 0;
    }
    return 0;
}

/**
 * \brief Write MAC register.
 *
 * \param[in] regoffset Register offset
 * \param[in] data Register value to write
 *
 * \return 0 in case of success, 1 otherwise
 */
static unsigned int smsc9220_mac_regwrite(unsigned char regoffset, unsigned int data)
{
    unsigned int read = 0;
    unsigned int maccmd = 0;
    int time_out = REG_WRITE_TIME_OUT;

    read = SMSC9220->MAC_CSR_CMD;
    if(!(read & ((unsigned int)1 << 31))) { /* Make sure there's no pending operation */
        SMSC9220->MAC_CSR_DATA = data;      /* Store data. */
        maccmd |= regoffset;
        maccmd &= ~((unsigned int)1 << 30); /* Clear indicates write */
        maccmd |= ((unsigned int)1 << 31);  /* Indicate start of operation */
        SMSC9220->MAC_CSR_CMD = maccmd;

        do {
            read = SMSC9220->BYTE_TEST;     /* A no-op read. */
            wait_ms(1);
            time_out--;
        } while(time_out && (SMSC9220->MAC_CSR_CMD & ((unsigned int)1 << 31)));

        if(!time_out) {
            return 1;
        }
    } else {
       printf("Error: SMSC9220 MAC CSR is busy. No data written.\n");
    }
    return 0;
}

/**
 * \brief Read PHY register.
 *
 * \param[in] regoffset Register offset
 * \param[out] data Register value is read
 *
 * \return 0 in case of success, 1 otherwise
 */
static unsigned int smsc9220_phy_regread(unsigned char regoffset, unsigned int *data)
{
    unsigned int val = 0;
    unsigned int phycmd = 0;
    int time_out = REG_WRITE_TIME_OUT;

    if (smsc9220_mac_regread(SMSC9220_MAC_MII_ACC, &val)) {
        return 1;
    }

    if(!(val & 1)) {    /* Not busy */
        phycmd = 0;
        phycmd |= (1 << 11);                 /* 1 to [15:11] */
        phycmd |= ((regoffset & 0x1F) << 6); /* Put regoffset to [10:6] */
        phycmd &= ~(1 << 1);                 /* Clear [1] indicates read. */
        phycmd |= (1 << 0);                  /* Set [0] indicates operation start */

        if (smsc9220_mac_regwrite(SMSC9220_MAC_MII_ACC, phycmd)) {
            return 1;
        }

        val = 0;
        do {
            wait_ms(1);
            time_out--;
            if (smsc9220_mac_regread(SMSC9220_MAC_MII_ACC,&val)) {
                return 1;
            }
        } while(time_out && (val & ((unsigned int)1 << 0)));

        if (!time_out) {
            return 1;
        } else if (smsc9220_mac_regread(SMSC9220_MAC_MII_DATA, data)) {
            return 1;
        }
    } else {
        *data = 0;
        return 1;
    }
    return 0;
}

/**
 * \brief Write PHY register.
 *
 * \param[in] regoffset Register offset
 * \param[in] data Register value to write
 *
 * \return 0 in case of success, 1 otherwise
 */
static unsigned int smsc9220_phy_regwrite(unsigned char regoffset, unsigned int data)
{
    unsigned int val = 0;
    unsigned int phycmd = 0;
    int time_out = REG_WRITE_TIME_OUT;

    if (smsc9220_mac_regread(SMSC9220_MAC_MII_ACC, &val)) {
        return 1;
    }

    if(!(val & 1)) {    /* Not busy */
        /* Load the data */
        if (smsc9220_mac_regwrite(SMSC9220_MAC_MII_DATA, (data & 0xFFFF))) {
            return 1;
        }
        phycmd = 0;
        phycmd |= (1 << 11);                    /* 1 to [15:11] */
        phycmd |= ((regoffset & 0x1F) << 6);    /* Put regoffset to [10:6] */
        phycmd |= (1 << 1);                     /* Set [1] indicates write. */
        phycmd |= (1 << 0);                     /* Set [0] indicates operation start */
        /* Start operation */
        if (smsc9220_mac_regwrite(SMSC9220_MAC_MII_ACC, phycmd)) {
            return 1;
        }

        phycmd = 0;

        do {
            wait_ms(1);
            time_out--;
            if (smsc9220_mac_regread(SMSC9220_MAC_MII_ACC, &phycmd)){
                return 1;
            }
        } while(time_out && (phycmd & (1 << 0)));

        if (!time_out) {
            return 1;
        }

    } else {
        printf("Error: SMSC9220 MAC MII is busy. No data written.\n");
    }
    return 0;
}

/**
 * \brief Read SMSC9220 ID.
 *
 * \return ID number
 */
inline static unsigned int smsc9220_read_id(void)
{
    return SMSC9220->ID_REV;
}

/**
 * \brief Initiates a soft reset, returns failure or success.
 *
 * \return 0 in case of success, 1 otherwise
 */
static unsigned int smsc9220_soft_reset(void)
{
    int time_out = RESET_TIME_OUT;

    /* Soft reset */
    SMSC9220->HW_CFG |= 1;

    do {
        wait_ms(1);
        time_out--;
    } while(time_out && (SMSC9220->HW_CFG & 1));

    if (!time_out) {
        return 1;
    }

    return 0;
}

/**
 * \brief Set maximum transition unit by Tx fifo size.
 *        Note: The MTU will be smaller by 512 bytes,
 *        because the status uses this fixed space.
 *
 * \param[in] val Size of the fifo in kbytes, 2-14
 */
static void smsc9220_set_txfifo(unsigned int val)
{
    /* 2kb minimum, 14kb maximum */
    if(val >= 2 && val <= 14) {
      SMSC9220->HW_CFG = val << 16;
    }
}

/**
 * \brief Wait for EEPROM to be ready to use.
 *
 * \return 0 if ready, 1 in case of timeout
 */
static unsigned int smsc9220_wait_eeprom(void)
{
    int time_out = REG_WRITE_TIME_OUT;

    do {
        wait_ms(1);
        time_out--;
    } while(time_out && (SMSC9220->E2P_CMD & ((unsigned int) 1 << 31)));

    if (!time_out) {
        return 1;
    }

    return 0;
}

/**
 * \brief Initialise irqs
 */
static void smsc9220_init_irqs(void)
{
    SMSC9220->INT_EN = 0x0;
    SMSC9220->INT_STS = 0xFFFFFFFF;   /* clear all interrupts */
    SMSC9220->IRQ_CFG = 0x22000100;   /* irq deassertion at 220 usecs and master IRQ enable. */
}

/**
 * \brief Check PHY ID registers.
 *
 * \return 0 in case of success, 1 otherwise
 */
static unsigned int smsc9220_check_phy(void)
{
    unsigned int phyid1, phyid2;

    if (smsc9220_phy_regread(SMSC9220_PHY_ID1,&phyid1)) {
        return 1;
    }
    if (smsc9220_phy_regread(SMSC9220_PHY_ID2,&phyid2)) {
        return 1;
    }
    return ((phyid1 == 0xFFFF && phyid2 == 0xFFFF) ||
            (phyid1 == 0x0 && phyid2 == 0x0));
}

/**
 * \brief Reset PHY
 *
 * \return 0 in case of success, 1 otherwise
 */
static unsigned int smsc9220_reset_phy(void)
{
    unsigned int read;

    if(smsc9220_phy_regread(SMSC9220_PHY_BCONTROL, &read)) {
        return 1;
    }

    read |= (1 << 15);
    if(smsc9220_phy_regwrite(SMSC9220_PHY_BCONTROL, read)) {
        return 1;
    }
    return 0;
}


/**
 * \brief Advertise all speeds and pause capabilities
 *
 * \return 0 in case of success, 1 otherwise
 */
static void smsc9220_advertise_cap(void)
{
    unsigned int aneg_adv = 0;

    smsc9220_phy_regread(SMSC9220_PHY_ANEG_ADV, &aneg_adv);
    aneg_adv |= 0xDE0;

    smsc9220_phy_regwrite(SMSC9220_PHY_ANEG_ADV, aneg_adv);
    smsc9220_phy_regread(SMSC9220_PHY_ANEG_ADV, &aneg_adv);
}

/**
 * \brief Enable trasmission
 */
inline static void smsc9220_enable_xmit(void)
{
    SMSC9220->TX_CFG = 0x2;
}

static void smsc9220_enable_mac_xmit(void)
{
    unsigned int mac_cr = 0;

    smsc9220_mac_regread(SMSC9220_MAC_CR, &mac_cr);

    mac_cr |= (1 << 3);     /* xmit enable */
    mac_cr |= (1 << 28);    /* Heartbeat disable */

    smsc9220_mac_regwrite(SMSC9220_MAC_CR, mac_cr);
}

/**
 * \brief Enable receive
 */
static void smsc9220_enable_mac_recv(void)
{
    unsigned int mac_cr = 0;

    smsc9220_mac_regread(SMSC9220_MAC_CR, &mac_cr);
    mac_cr |= (1 << 2);     /* Recv enable */
    smsc9220_mac_regwrite(SMSC9220_MAC_CR, mac_cr);
}

/**
 * \brief Check device ID.
 *
 * \return 0 in case of success, 1 otherwise
 */
static int smsc9220_check_id(void)
{
    unsigned int id = smsc9220_read_id();

    /* If bottom and top halves of the word are the same */
    if(((id >> 16) & 0xFFFF) == (id & 0xFFFF)) {
        return 1;
    }
    switch(((id >> 16) & 0xFFFF)) {
        case 0x9220:
            break;

        default:
            return 1;
    }

    return 0;
}

/**
 * \brief Fill the SMSC9220 TX FIFO with a number of words at an aligned
 *        address.
 *
 * \param[in] data Pointer to the aligned data that should be sent.
 * \param[in] dwords_to_write Number of data words to write.
 */
static void fill_tx_fifo_aligned(unsigned int *data,
                                 unsigned int dwords_to_write)
{
    while (dwords_to_write > 0) {
        SMSC9220->TX_DATA_PORT = *data;
        data++;
        dwords_to_write--;
    }
}

/**
 * \brief Fill the SMSC9220 TX FIFO with a number of words at an unaligned
 *        address. This function ensures that loading words at that address will
 *        not generate unaligned access which can trigger an exception to the
 *        processor.
 *
 * \param[in] data Pointer to the unaligned data that should be sent.
 * \param[in] dwords_to_write Number of data words to write.
 */
static void fill_tx_fifo_unaligned(uint8_t *data, unsigned int dwords_to_write)
{
    /*
     * Prevent unaligned word access from data pointer, 4 bytes are copied to
     * this variable for each word that need to be sent.
     */
    unsigned int tx_data_port_tmp = 0;
    uint8_t *tx_data_port_tmp_ptr = (uint8_t *)&tx_data_port_tmp;

    while (dwords_to_write > 0) {
        /* Keep the same endianness in data than in the temp variable */
        tx_data_port_tmp_ptr[0] = data[0];
        tx_data_port_tmp_ptr[1] = data[1];
        tx_data_port_tmp_ptr[2] = data[2];
        tx_data_port_tmp_ptr[3] = data[3];
        SMSC9220->TX_DATA_PORT = tx_data_port_tmp;
        data += 4;
        dwords_to_write--;
    }
}

/*----------------------------------------------------------------------------
  Public API
 *----------------------------------------------------------------------------*/
int smsc9220_init(void)
{
    unsigned int phyreset = 0;

    if(smsc9220_check_id()) {
        return 1;
    }

    if(smsc9220_soft_reset()) {
        return 1;
    }

    smsc9220_set_txfifo(5);

    /* Sets automatic flow control thresholds, and backpressure */
    /* threshold to defaults specified. */
    SMSC9220->AFC_CFG = 0x006E3740;

    if(smsc9220_wait_eeprom()) {
        return 1;
    }

    /* Configure GPIOs as LED outputs. */
    SMSC9220->GPIO_CFG = 0x70070000;

    smsc9220_init_irqs();

    /* Configure MAC addresses here if needed. */

    if(smsc9220_check_phy()) {
        return 1;
    }

    if(smsc9220_reset_phy()) {
        return 1;
    }

    wait_ms(PHY_RESET_TIME_OUT_MS);
    /* Checking whether phy reset completed successfully.*/
    if (smsc9220_phy_regread(SMSC9220_PHY_BCONTROL, &phyreset)) {
        return 1;
    }
    if(phyreset & (1 << 15)) {
        return 1;
    }

    smsc9220_advertise_cap();
    smsc9220_establish_link();      /* bit [12] of BCONTROL seems self-clearing. */
                                    /* Although it's not so in the manual. */

    /* Interrupt threshold */
    SMSC9220->FIFO_INT = 0xFF000000;

    smsc9220_enable_mac_xmit();
    smsc9220_enable_xmit();
    SMSC9220->RX_CFG = 0;
    smsc9220_enable_mac_recv();

    /* Rx status FIFO level irq threshold */
    SMSC9220->FIFO_INT &= ~(0xFF);  /* Clear 2 bottom nibbles */

    /* This sleep is compulsory otherwise txmit/receive will fail. */
    wait_ms(2000);
    return 0;
}

void smsc9220_enable_interrupt(enum smsc9220_interrupt_source source)
{
    SMSC9220->INT_EN |= (1 << source);
}

void smsc9220_disable_interrupt(enum smsc9220_interrupt_source source)
{
    SMSC9220->INT_EN &= ~(1 << source);
}

void smsc9220_clear_interrupt(enum smsc9220_interrupt_source source)
{
    SMSC9220->INT_STS |= (1 << source);
}

int smsc9220_get_interrupt(enum smsc9220_interrupt_source source)
{
    return (SMSC9220->INT_STS & (1 << source));
}

void smsc9220_establish_link(void)
{
    unsigned int bcr = 0;
    unsigned int hw_cfg = 0;

    smsc9220_phy_regread(SMSC9220_PHY_BCONTROL, &bcr);
    bcr |= (1 << 12) | (1 << 9);
    smsc9220_phy_regwrite(SMSC9220_PHY_BCONTROL, bcr);
    smsc9220_phy_regread(SMSC9220_PHY_BCONTROL, &bcr);

    hw_cfg = SMSC9220->HW_CFG;
    hw_cfg &= 0xF0000;
    hw_cfg |= (1 << 20);
    SMSC9220->HW_CFG = hw_cfg;
}

int smsc9220_read_mac_address(char *mac)
{
    unsigned int mac_low = 0;
    unsigned int mac_high = 0;

    if( !mac ) {
        return 1;
    }

    /* Read current mac address. */
    if (smsc9220_mac_regread(SMSC9220_MAC_ADDRH, &mac_high)) {
        return 1;
    }
    if (smsc9220_mac_regread(SMSC9220_MAC_ADDRL, &mac_low)) {
        return 1;
    }
    mac[0] = mac_low & 0xFF;
    mac[1] = (mac_low >> 8) & 0xFF;
    mac[2] = (mac_low >> 16) & 0xFF;
    mac[3] = (mac_low >> 24) & 0xFF;
    mac[4] = mac_high & 0xFF;
    mac[5] = (mac_high >> 8) & 0xFF;

    return 0;
}

unsigned int smsc9220_get_tx_data_fifo_size(void)
{
  const unsigned int tx_status_fifo_size = 512; /* fixed allocation in bytes */
  unsigned int tx_fifo_size = SMSC9220->HW_CFG;
  tx_fifo_size = (( tx_fifo_size >> 16 ) & 0x0F) * 1024; /* size is set in kbytes */
  return (tx_fifo_size - tx_status_fifo_size);
}

int smsc9220_send_by_chunks(unsigned int total_packet_length, int is_new_packet,
                            const char *data, unsigned int current_size)
{
    static unsigned int ongoing_packet_length = 0; /* size in bytes of the packet is sending */
    static unsigned int ongoing_packet_length_sent = 0; /* size in bytes of the packet has been sent */
    int is_first_segment = 0; /* signing this is the first segment of the packet to be sent */
    int is_last_segment = 0; /* signing this is the last segment of the packet to be sent */
    unsigned int txcmd_a, txcmd_b = 0;
    unsigned int dwords_to_write = 0;
    unsigned int xmit_inf = 0;
    unsigned int tx_buffer_free_space = 0;
    unsigned int xmit_stat = 0;

    if (!data) {
        return -1; /* Invalid input parameter */
    }

    if (is_new_packet) {
        is_first_segment = 1;
        ongoing_packet_length = total_packet_length;
        ongoing_packet_length_sent = 0;
    } else if (ongoing_packet_length != total_packet_length ||
             ongoing_packet_length_sent >= total_packet_length) {
        return -1; /* Invalid input parameter */
    }

    /* Would next chunk fit into buffer? */
    xmit_inf = SMSC9220->TX_FIFO_INF;
    tx_buffer_free_space = xmit_inf & 0xFFFF;
    if (current_size > tx_buffer_free_space) {
        return -1; /* Not enough space in FIFO */
    }
    if ((ongoing_packet_length_sent + current_size) == total_packet_length) {
        is_last_segment = 1;
    }

    txcmd_a = 0;
    txcmd_b = 0;

    txcmd_a |= (is_last_segment << 12) | (is_first_segment << 13); /* Last and first segments */
    txcmd_a |= current_size & 0x7FF; /* [10:0] contains length */

    txcmd_b |= ((current_size & 0xFFFF) << 16); /* [31:16] contains length */
    txcmd_b |= current_size & 0x7FF; /* [10:0] also contains length */

    SMSC9220->TX_DATA_PORT = txcmd_a;
    SMSC9220->TX_DATA_PORT = txcmd_b;
    dwords_to_write = (current_size + 3) >> 2;

    /*
     * Copy to TX FIFO
     * The function to use depends on the alignment of the data pointer on a 32
     * bits boundary.
     */
    if (((unsigned int)data % sizeof(uint32_t)) == 0) {
        /* Cast is safe because we know data is aligned */
        fill_tx_fifo_aligned((unsigned int *)data, dwords_to_write);
    } else {
        fill_tx_fifo_unaligned((uint8_t *)data, dwords_to_write);
    }

    if (is_last_segment) {
        /* pop status port */
        /* for error check it should be checked "at a later time" according to data sheet */
        xmit_stat = SMSC9220->TX_STAT_PORT;
        (void)xmit_stat;
    }

    ongoing_packet_length_sent += current_size;
    return 0;
}

unsigned int smsc9220_get_rxfifo_data_used_space(void)
{
    unsigned int rxfifo_inf = SMSC9220->RX_FIFO_INF;
    return rxfifo_inf & 0xFFFF;
}

unsigned int smsc9220_receive_by_chunks(char *data, unsigned int dlen)
{
    static unsigned int current_packet_size_words = 0;
    unsigned int rxfifo_inf = 0;
    unsigned int rxfifo_stat = 0;
    unsigned int dlen_word = 0;
    unsigned int read_length_word = 0;
    unsigned int i = 0;

    if (!data) {
        return 0; /* Invalid input parameter */
    }

    if (current_packet_size_words == 0) {
        /* First the packet status word should be read, */
        /* which tells the size of the data, */
        /* after the data can be read in synchron. */
        rxfifo_inf = SMSC9220->RX_FIFO_INF;

        if(rxfifo_inf & 0xFFFF) { /* If there's data */
            rxfifo_stat = SMSC9220->RX_STAT_PORT;
            if(rxfifo_stat != 0) {   /* Fetch status of this packet */
                if(rxfifo_stat & (1 << 15)) {
                    current_packet_size_words = 0; /* error */
                }
                else {
                    /* Ethernet controller is padding to 32bit aligned data */
                    current_packet_size_words = (((rxfifo_stat >> 16) & 0x3FFF) + 3) >> 2;
                }
            }
        }
    }
    dlen_word = dlen / 4;
    read_length_word = (dlen_word < current_packet_size_words)  ? dlen_word : current_packet_size_words;

    for (i = 0; i < read_length_word; i++) {
        ((unsigned int*)data)[i] =  SMSC9220->RX_DATA_PORT;
        current_packet_size_words--;
    }
    return (current_packet_size_words * 4);
}

unsigned int smsc9220_peek_next_packet_size(void)
{
    unsigned int packet_size = 0;
    unsigned int rx_stat_peek = 0;

    if(smsc9220_get_rxfifo_data_used_space()) {
        rx_stat_peek = SMSC9220->RX_STAT_PEEK;
        packet_size = ((rx_stat_peek >> 16) & 0x3FFF);
    }
    return (((packet_size + 3) >> 2) << 2);
}

