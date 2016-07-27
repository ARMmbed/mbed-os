/**
 * \file
 *
 * \brief SAM G55 serial driver configuration.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
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

#ifndef CONF_SERIALDRV_H_INCLUDED
#define CONF_SERIALDRV_H_INCLUDED

/* BTLC1000 Wakeup Pin */
#define BTLC1000_WAKEUP_PIN			(EXT1_PIN_6)

/* BTLC1000 Chip Enable Pin */
#define BTLC1000_CHIP_ENABLE_PIN	(EXT1_PIN_10)

/* BTLC1000 50ms Reset Duration */
#define BTLC1000_RESET_MS			(50)

/* set port pin high */
#define IOPORT_PIN_LEVEL_HIGH		(true)
/* Set port pin low */
#define IOPORT_PIN_LEVEL_LOW		(false)

/** UART Interface */
#define BLE_UART            EXT1_UART_MODULE
#define BLE_UART_ID		    ID_FLEXCOM0
#define BLE_USART_FLEXCOM   FLEXCOM0
#define BLE_UART_IRQn		FLEXCOM0_IRQn
/* Configuration for console uart IRQ handler */
#define BLE_UART_Handler    FLEXCOM0_Handler

/** Baudrate setting */
#define CONF_UART_BAUDRATE   (115200UL)
/** Character length setting */
#define CONF_UART_CHAR_LENGTH  US_MR_CHRL_8_BIT
/** Parity setting */
#define CONF_UART_PARITY     US_MR_PAR_NO
/** Stop bits setting */
#define CONF_UART_STOP_BITS    US_MR_NBSTOP_1_BIT

void serial_rx_callback(void);
void serial_tx_callback(void);

#define SERIAL_DRV_RX_CB serial_rx_callback
#define SERIAL_DRV_TX_CB serial_tx_callback
#define SERIAL_DRV_TX_CB_ENABLE  true
#define SERIAL_DRV_RX_CB_ENABLE  true

#define BLE_MAX_TX_PAYLOAD_SIZE 512
#define BLE_MAX_RX_PAYLOAD_SIZE 512

/* Set BLE Wakeup pin to be low */
static inline bool ble_wakeup_pin_level(void)
{
    return (ioport_get_pin_level(BTLC1000_WAKEUP_PIN));
}

/* Set BLE Wakeup pin to be low */
static inline void ble_wakeup_pin_set_low(void)
{
    ioport_set_pin_level(BTLC1000_WAKEUP_PIN,
                         IOPORT_PIN_LEVEL_LOW);
}

/* Set wakeup pin to high */
static inline void ble_wakeup_pin_set_high(void)
{
    ioport_set_pin_level(BTLC1000_WAKEUP_PIN,
                         IOPORT_PIN_LEVEL_HIGH);
}

/* Set enable pin to Low */
static inline void ble_enable_pin_set_low(void)
{
    ioport_set_pin_level(BTLC1000_CHIP_ENABLE_PIN,
                         IOPORT_PIN_LEVEL_LOW);
}

/* Set enable pin to high */
static inline void ble_enable_pin_set_high(void)
{
    ioport_set_pin_level(BTLC1000_CHIP_ENABLE_PIN,
                         IOPORT_PIN_LEVEL_HIGH);
}

/* Configure the BTLC1000 control(chip_enable, wakeup) pins */
static inline void ble_configure_control_pin(void)
{
    /* initialize the delay before use */
    delay_init();

    /* Configure control pins as output */
    ioport_init();

    ioport_set_pin_dir(BTLC1000_WAKEUP_PIN, IOPORT_DIR_OUTPUT);

    /* set wakeup pin to low */
    ble_wakeup_pin_set_high();

    ioport_set_pin_dir(BTLC1000_CHIP_ENABLE_PIN, IOPORT_DIR_OUTPUT);

    /* set chip enable to low */
    ble_enable_pin_set_low();

    /* Delay for 50ms */
    delay_ms(BTLC1000_RESET_MS);

    /* set chip enable to high */
    ble_enable_pin_set_high();
}

static inline void ble_reset(void)
{
    /* BTLC1000 Reset Sequence @Todo */
    ble_enable_pin_set_high();
    ble_wakeup_pin_set_high();
    delay_ms(BTLC1000_RESET_MS);

    ble_enable_pin_set_low();
    ble_wakeup_pin_set_low();
    delay_ms(BTLC1000_RESET_MS);

    ble_enable_pin_set_high();
    ble_wakeup_pin_set_high();
    delay_ms(BTLC1000_RESET_MS);
}


#endif /* CONF_SERIALDRV_H_INCLUDED */
