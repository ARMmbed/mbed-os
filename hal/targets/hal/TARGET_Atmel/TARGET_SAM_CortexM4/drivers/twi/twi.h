/**
 * \file
 *
 * \brief Two-Wire Interface (TWI) driver for SAM.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef TWI_H_INCLUDED
#define TWI_H_INCLUDED

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

    /** Time-out value (number of attempts). */
#define TWI_TIMEOUT              15000

    /**
     * \brief Return codes for TWI APIs.
     * @{
     */
#define TWI_SUCCESS              0
#define TWI_INVALID_ARGUMENT     1
#define TWI_ARBITRATION_LOST     2
#define TWI_NO_CHIP_FOUND        3
#define TWI_RECEIVE_OVERRUN      4
#define TWI_RECEIVE_NACK         5
#define TWI_SEND_OVERRUN         6
#define TWI_SEND_NACK            7
#define TWI_BUSY                 8
#define TWI_ERROR_TIMEOUT        9
    /**
     * @}
     */

    /**
     * \brief Input parameters when initializing the TWI module mode.
     */
    typedef struct twi_options {
        //! MCK for TWI.
        uint32_t master_clk;
        //! The baud rate of the TWI bus.
        uint32_t speed;
        //! The desired address.
        uint8_t chip;
        //! SMBUS mode (set 1 to use SMBUS quick command, otherwise don't).
        uint8_t smbus;
    } twi_options_t;

    /**
     * \brief Information concerning the data transmission.
     */
    typedef struct twi_packet {
        //! TWI address/commands to issue to the other chip (node).
        uint8_t addr[3];
        //! Length of the TWI data address segment (1-3 bytes).
        uint32_t addr_length;
        //! Where to find the data to be transferred.
        void *buffer;
        //! How many bytes do we want to transfer.
        uint32_t length;
        //! TWI chip address to communicate with.
        uint8_t chip;
    } twi_packet_t;

#if SAMG55
enum twi_source_clock {
    TWI_SOURCE_PERIPH_CLK = TWI_CWGR_BRSRCCLK_PERIPH_CLK,
    TWI_SOURCE_PCK_CLK = TWI_CWGR_BRSRCCLK_PMC_PCK,
};
#endif

void twi_enable_master_mode(Twi *p_twi);
void twi_disable_master_mode(Twi *p_twi);
uint32_t twi_master_init(Twi *p_twi, const twi_options_t *p_opt);
uint32_t twi_set_speed(Twi *p_twi, uint32_t ul_speed, uint32_t ul_mck);
uint32_t twi_probe(Twi *p_twi, uint8_t uc_slave_addr);
uint32_t twi_master_read(Twi *p_twi, twi_packet_t *p_packet);
uint32_t twi_master_write(Twi *p_twi, twi_packet_t *p_packet);
void twi_enable_interrupt(Twi *p_twi, uint32_t ul_sources);
void twi_disable_interrupt(Twi *p_twi, uint32_t ul_sources);
uint32_t twi_get_interrupt_status(Twi *p_twi);
uint32_t twi_get_interrupt_mask(Twi *p_twi);
uint8_t twi_read_byte(Twi *p_twi);
void twi_write_byte(Twi *p_twi, uint8_t uc_byte);
void twi_enable_slave_mode(Twi *p_twi);
void twi_disable_slave_mode(Twi *p_twi);
void twi_slave_init(Twi *p_twi, uint32_t ul_device_addr);
void twi_set_slave_addr(Twi *p_twi, uint32_t ul_device_addr);
uint32_t twi_slave_read(Twi *p_twi, uint8_t *p_data);
uint32_t twi_slave_write(Twi *p_twi, uint8_t *p_data);
void twi_reset(Twi *p_twi);
Pdc *twi_get_pdc_base(Twi *p_twi);
#if (SAM4E || SAM4C || SAMG || SAM4CP || SAM4CM)
void twi_set_write_protection(Twi *p_twi, bool flag);
void twi_read_write_protection_status(Twi *p_twi, uint32_t *p_status);
#endif

#if SAMG55
void twi_smbus_set_timing(Twi *p_twi, uint32_t ul_timing);
void twi_set_alternative_command(Twi *p_twi, uint32_t ul_alt_cmd);
void twi_set_filter(Twi *p_twi, uint32_t ul_filter);
void twi_mask_slave_addr(Twi *p_twi, uint32_t ul_mask);
void twi_set_sleepwalking(Twi *p_twi,
                          uint32_t ul_matching_addr1, bool flag1,
                          uint32_t ul_matching_addr2, bool flag2,
                          uint32_t ul_matching_addr3, bool flag3,
                          uint32_t ul_matching_data, bool flag);

/**
 * \brief Enable high speed mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_highspeed(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_HSEN;
}

/**
 * \brief Disable high speed mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_highspeed(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_HSDIS;
}

/**
 * \brief Enable SMBus mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_smbus(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_SMBEN;
}

/**
 * \brief Disable SMBus mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_smbus(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_SMBDIS;
}

/**
 * \brief Enable packet error checking.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_pec(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_PECEN;
}

/**
 * \brief Disable packet error checking.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_pec(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_PECDIS;
}

/**
 * \brief Request a packet error checking.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_request_pec(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_PECRQ;
}

/**
 * \brief If master mode is enabled, send a bus clear command.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_send_clear(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_CLEAR;
}

/**
 * \brief Enable alternative command mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_alternative_command(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_ACMEN;
}

/**
 * \brief Enable alternative command mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_alternative_command(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_ACMDIS;
}

/**
 * \brief Clear the Transmit Holding Register and set TXRDY, TXCOMP flags.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_thr_clear(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_THRCLR;
}

/**
 * \brief Clear the TWI FSM lock.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_lock_clear(Twi *p_twi)
{
    p_twi->TWI_CR = TWI_CR_LOCKCLR;
}

/**
 * \brief Normal value to be returned in the ACK cycle of the data phase in slave receiver mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_nack(Twi *p_twi)
{
    p_twi->TWI_SMR &= ~TWI_SMR_NACKEN;
}

/**
 * \brief NACK value to be returned in the ACK cycle of the data phase in slave receiver mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_nack(Twi *p_twi)
{
    p_twi->TWI_SMR |= TWI_SMR_NACKEN;
}

/**
 * \brief Acknowledge of the SMBus Default Address disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_default_addr(Twi *p_twi)
{
    p_twi->TWI_SMR &= ~TWI_SMR_SMDA;
}

/**
 * \brief Acknowledge of the SMBus Default Address enabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_default_addr(Twi *p_twi)
{
    p_twi->TWI_SMR |= TWI_SMR_SMDA;
}

/**
 * \brief Acknowledge of the SMBus Host Header disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_smbus_host_header(Twi *p_twi)
{
    p_twi->TWI_SMR &= ~TWI_SMR_SMHH;
}

/**
 * \brief Acknowledge of the SMBus Host Header enabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_smbus_host_header(Twi *p_twi)
{
    p_twi->TWI_SMR |= TWI_SMR_SMHH;
}

/**
 * \brief Clock stretching disabled in slave mode, OVRE and UNRE will indicate overrun and underrun.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_clock_wait_state(Twi *p_twi)
{
    p_twi->TWI_SMR |= TWI_SMR_SCLWSDIS;
}

/**
 * \brief Clear clock wait state disable mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_clear_disable_clock_wait_state(Twi *p_twi)
{
    p_twi->TWI_SMR &= ~TWI_SMR_SCLWSDIS;
}

/**
 * \brief Slave Address 1 matching disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_addr1_matching(Twi *p_twi)
{
    p_twi->TWI_SMR &= ~TWI_SMR_SADR1EN;
}

/**
 * \brief Slave Address 1 matching enabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_addr1_matching(Twi *p_twi)
{
    p_twi->TWI_SMR |= TWI_SMR_SADR1EN;
}

/**
 * \brief Slave Address 2 matching disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_addr2_matching(Twi *p_twi)
{
    p_twi->TWI_SMR &= ~TWI_SMR_SADR2EN;
}

/**
 * \brief Slave Address 2 matching enabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_addr2_matching(Twi *p_twi)
{
    p_twi->TWI_SMR |= TWI_SMR_SADR2EN;
}

/**
 * \brief Slave Address 3 matching disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_addr3_matching(Twi *p_twi)
{
    p_twi->TWI_SMR &= ~TWI_SMR_SADR3EN;
}

/**
 * \brief Slave Address 3 matching enabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_addr3_matching(Twi *p_twi)
{
    p_twi->TWI_SMR |= TWI_SMR_SADR3EN;
}

/**
 * \brief First received data matching disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_data_matching(Twi *p_twi)
{
    p_twi->TWI_SMR &= ~TWI_SMR_DATAMEN;
}

/**
 * \brief Select the souce clock for the bit rate generation.
 *
 * \param p_twi   Base address of the TWI instance.
 * \param src_clk  Source clock.
 */
static inline void twi_select_source_clock(Twi *p_twi, enum twi_source_clock src_clk)
{
    p_twi->TWI_CWGR &= ~TWI_CWGR_BRSRCCLK;
    p_twi->TWI_CWGR |= src_clk;
}
#endif

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* TWI_H_INCLUDED */
