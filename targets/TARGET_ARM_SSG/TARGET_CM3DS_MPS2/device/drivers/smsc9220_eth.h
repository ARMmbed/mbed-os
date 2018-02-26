/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

/* This file is the re-implementation of mps2_ethernet_api and Selftest's ETH_MPS2.
 * MPS2 Selftest:https://silver.arm.com/browse/VEI10 ->
 *     \ISCM-1-0\AN491\software\Selftest\v2m_mps2\
 */
#ifndef _SMSC9220_ETH_H_
#define _SMSC9220_ETH_H_

enum smsc9220_interrupt_source {
    enum_smsc9220_interrupt_gpio0 = 0,
    enum_smsc9220_interrupt_gpio1 = 1,
    enum_smsc9220_interrupt_gpio2 = 2,
    enum_smsc9220_interrupt_rxstatus_fifo_level = 3,
    enum_smsc9220_interrupt_rxstatus_fifo_full = 4,
    /* 5 Reserved according to Datasheet */
    enum_smsc9220_interrupt_rx_dropped_frame = 6,
    enum_smsc9220_interrupt_txstatus_fifo_level = 7,
    enum_smsc9220_interrupt_txstatus_fifo_full = 8,
    enum_smsc9220_interrupt_txdata_fifo_available = 9,
    enum_smsc9220_interrupt_txdata_fifo_overrun = 10,
    /* 11, 12 Reserved according to Datasheet */
    enum_smsc9220_interrupt_transmit_error = 13,
    enum_smsc9220_interrupt_receive_error = 14,
    enum_smsc9220_interrupt_receive_watchdog_timeout = 15,
    enum_smsc9220_interrupt_txstatus_overflow = 16,
    enum_smsc9220_interrupt_power_management = 17,
    enum_smsc9220_interrupt_phy = 18,
    enum_smsc9220_interrupt_gp_timer = 19,
    enum_smsc9220_interrupt_rx_dma = 20,
    enum_smsc9220_interrupt_tx_ioc = 21,
    /* 22 Reserved according to Datasheet*/
    enum_smsc9220_interrupt_rx_dropped_frame_half = 23,
    enum_smsc9220_interrupt_rx_stopped = 24,
    enum_smsc9220_interrupt_tx_stopped = 25,
    /* 26 - 30 Reserved according to Datasheet*/
    enum_smsc9220_interrupt_sw = 31
};

/* Function declarations */

/**
 * \brief Initialize SMS9220 Ethernet controller
 *
 * \return 0 if init is successful, 1 otherwise
 */
int smsc9220_init(void);

/**
 * \brief Enable the given interrupt source.
 *
 * \param[in] source Enum of the interrupt source.
 */
void smsc9220_enable_interrupt(enum smsc9220_interrupt_source source);

/**
 * \brief Disable the given interrupt source.
 *
 * \param[in] source Enum of the interrupt source.
 */
void smsc9220_disable_interrupt(enum smsc9220_interrupt_source source);

/**
 * \brief Clear the given interrupt source.
 *
 * \param[in] source Enum of the interrupt source.
 */
void smsc9220_clear_interrupt(enum smsc9220_interrupt_source source);

/**
 * \brief Get the status of the given interrupt source.
 *
 * \param[in] source Enum of the interrupt source.
 *
 * \return non-zero if the given interrupt source is triggered, zero otherwise
 */
int smsc9220_get_interrupt(enum smsc9220_interrupt_source source);

/**
 * \brief Establish link
 */
void smsc9220_establish_link(void);

/**
 * \brief Read MAC address from EEPROM.
 *
 * \param[in,out] mac array will include the read MAC address in
 *                6 bytes hexadecimal format.
 *                It should be allocated by the caller to 6 bytes.
 *
 * \return 0 if read is successful, 1 otherwise
 */
int smsc9220_read_mac_address(char *mac);

/**
 * \brief Get the data size of the Tx buffer, aka Maximum Transition Unit
 *
 * \return Fifo data size in bytes
 */
unsigned int smsc9220_get_tx_data_fifo_size(void);

/**
 * \brief Send Ethernet packet from buffer chain.
 *        The full packet length should be known in the beginning
 *        of a new packet.
 *
 * \param[in] total_packet_length Length of the packet. Should be equal to
 *            the sum of passed buffers within a packet.
 * \param[in] is_new_packet Should be set to non-zero if the passed buffer
 *            should be sent as the start of a new packet.
 *            If the current buffer should be sent as a full packet,
 *            it should be set to non-zero respectively.
 * \param[in] data Pointer to the data should be sent.
 * \param[in] current_size Size of the data in bytes.
 *
 * \return 0 if the send process is successful, standard C error code otherwise
 */
int smsc9220_send_by_chunks(unsigned int total_packet_length, int is_new_packet,
                            const char *data, unsigned int current_size);

/**
 * \brief Receive Ethernet packet from Rx FIFO to the passed buffer.
 *        Stops reading at packet border.
 *        If the passed buffer is larger than the current packet,
 *        the whole packet will be read into the buffer.
 *        If the current packet is larger than the passed buffer,
 *        the buffer will be filled with data and the next call
 *        will continue the read from that point.
 *
 * \param[in,out] data Pointer where the data will be read to.
 *                     The caller is responsible to allocate it.
 * \param[in] dlen Length of the allocated data in bytes.
 *
 * \return Remaining bytes left in the fifo of the current packet.
 */
unsigned int smsc9220_receive_by_chunks(char *data, unsigned int dlen);

/**
 * \brief Get the used space of Rx fifo in bytes.
 *
 * \return Data received and waiting for read in bytes
 */
unsigned int smsc9220_get_rxfifo_data_used_space(void);

/**
 * \brief Get the size of next unread packet in Rx buffer, using the peak
 *        register, which is not destructive so can be read asynchronously.
 *        Warning: In case of heavy receiving load, it's possible this register
 *        is not perfectly in sync.
 *
 * \return Size in bytes of the next packet can be read from Rx fifo, according
 *         to the peek register.
 */
unsigned int smsc9220_peek_next_packet_size(void);

#endif
