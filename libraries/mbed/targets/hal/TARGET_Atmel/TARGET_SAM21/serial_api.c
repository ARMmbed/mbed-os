/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include <string.h>
#include "mbed_assert.h"
#include "cmsis.h"
#include "serial_api.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "usart.h"
#include "samr21_xplained_pro.h"

#define _USART(obj)			obj->usart->USART
#define USART_NUM 1  // for SAMR21 // to be updated for samd21
//#define USART_BUF 8 

static uint32_t serial_irq_ids[USART_NUM] = {0};
static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;


/*struct serial_global_data_s{
    uint8_t string[USART_BUF];
	uint8_t count;
	};
static struct serial_global_data_s uart_data[USART_NUM];*/

extern uint8_t g_sys_init;

static inline bool usart_syncing(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    return (_USART(obj).SYNCBUSY.reg);
}

static inline void enable_usart(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
	
#if USART_CALLBACK_MODE == true  //TODO: to be implemented
    /* Enable Global interrupt for module */
    system_interrupt_enable(_sercom_get_interrupt_vector(obj->usart));
#endif

    /* Wait until synchronization is complete */
    usart_syncing(obj);

	/* Enable USART module */
    _USART(obj).CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
}

static inline void disable_usart(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
	
#if USART_CALLBACK_MODE == true  //TODO: to be implemented
    /* Disable Global interrupt for module */
    system_interrupt_disable(_sercom_get_interrupt_vector(obj->usart));
#endif
    /* Wait until synchronization is complete */
    usart_syncing(obj);

    /* Disable USART module */
    _USART(obj).CTRLA.reg &= ~SERCOM_USART_CTRLA_ENABLE;
}

static inline void reset_usart(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    disable_usart(obj);

    /* Wait until synchronization is complete */
    usart_syncing(obj);
	
    /* Reset module */
    _USART(obj).CTRLA.reg = SERCOM_USART_CTRLA_SWRST;
}

static enum status_code usart_set_config_asf( serial_t *obj)
{

    /* Index for generic clock */
	uint32_t sercom_index = _sercom_get_sercom_inst_index(obj->usart);
	uint32_t gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;

	/* Cache new register values to minimize the number of register writes */
	uint32_t ctrla = 0;
	uint32_t ctrlb = 0;
	uint16_t baud  = 0;

	enum sercom_asynchronous_operation_mode mode = SERCOM_ASYNC_OPERATION_MODE_ARITHMETIC;
	enum sercom_asynchronous_sample_num sample_num = SERCOM_ASYNC_SAMPLE_NUM_16;

	/* Set data order, internal muxing, and clock polarity */
	ctrla = (uint32_t)obj->data_order |
		(uint32_t)obj->mux_setting |
	#ifdef FEATURE_USART_OVER_SAMPLE
		obj->sample_adjustment |
		obj->sample_rate |
	#endif
		(obj->clock_polarity_inverted << SERCOM_USART_CTRLA_CPOL_Pos);

	/* Get baud value from mode and clock */
	switch (obj->transfer_mode)
	{
		case USART_TRANSFER_SYNCHRONOUSLY:
			if (!obj->use_external_clock) {
				_sercom_get_sync_baud_val(obj->baudrate,
						system_gclk_chan_get_hz(gclk_index), &baud);
			}

			break;

		case USART_TRANSFER_ASYNCHRONOUSLY:
			if (obj->use_external_clock) {
				_sercom_get_async_baud_val(obj->baudrate,
							obj->ext_clock_freq, &baud, mode, sample_num);
			} else {
						_sercom_get_async_baud_val(obj->baudrate,
							system_gclk_chan_get_hz(gclk_index), &baud, mode, sample_num);
			}

			break;
	}

	/* Wait until synchronization is complete */
	usart_syncing(obj);

	/*Set baud val */
	_USART(obj).BAUD.reg = baud;

	/* Set sample mode */
	ctrla |= obj->transfer_mode;

	if (obj->use_external_clock == false) {
		ctrla |= SERCOM_USART_CTRLA_MODE(0x1);
	}
	else {
		ctrla |= SERCOM_USART_CTRLA_MODE(0x0);
	}

	/* Set stopbits, character size and enable transceivers */
	ctrlb = (uint32_t)obj->stopbits | (uint32_t)obj->character_size |
		#ifdef FEATURE_USART_START_FRAME_DECTION
			(obj->start_frame_detection_enable << SERCOM_USART_CTRLB_SFDE_Pos) |
		#endif
			(obj->receiver_enable << SERCOM_USART_CTRLB_RXEN_Pos) |
			(obj->transmitter_enable << SERCOM_USART_CTRLB_TXEN_Pos);

	/* Check parity mode bits */
	if (obj->parity != USART_PARITY_NONE) {
		ctrla |= SERCOM_USART_CTRLA_FORM(1);
		ctrlb |= obj->parity;
	} else {
		ctrla |= SERCOM_USART_CTRLA_FORM(0);
	}

	/* Set whether module should run in standby. */
	if (obj->run_in_standby || system_is_debugger_present()) {
		ctrla |= SERCOM_USART_CTRLA_RUNSTDBY;
	}

	/* Wait until synchronization is complete */
	usart_syncing(obj);

	/* Write configuration to CTRLB */
	_USART(obj).CTRLB.reg = ctrlb;

	/* Wait until synchronization is complete */
	usart_syncing(obj);

	/* Write configuration to CTRLA */
	_USART(obj).CTRLA.reg = ctrla;

	return STATUS_OK;
}

void serial_init(serial_t *obj, PinName tx, PinName rx) {
    if (g_sys_init == 0) {
	    system_init();
        g_sys_init = 1;
    }
	
	struct system_gclk_chan_config gclk_chan_conf;
	uint32_t gclk_index;
	uint32_t pm_index;
	uint32_t sercom_index = 0; 
	
	obj->usart = EXT1_UART_MODULE;
	
	/* Disable USART module */
	disable_usart(obj);
	
    /* Set default config to object */
	obj->data_order = USART_DATAORDER_LSB;
	obj->transfer_mode = USART_TRANSFER_ASYNCHRONOUSLY;
	obj->parity = USART_PARITY_NONE;
	obj->stopbits = USART_STOPBITS_1;
	obj->character_size = USART_CHARACTER_SIZE_8BIT;
	obj->baudrate = 9600;
	obj->receiver_enable = true;
	obj->transmitter_enable = true;
	obj->clock_polarity_inverted = false;
	obj->use_external_clock = false;
	obj->ext_clock_freq = 0;
	obj->mux_setting = USART_RX_1_TX_2_XCK_3;
	obj->run_in_standby = false;
	obj->generator_source = GCLK_GENERATOR_0;
	obj->pinmux_pad0 = PINMUX_DEFAULT;
	obj->pinmux_pad1 = PINMUX_DEFAULT;
	obj->pinmux_pad2 = PINMUX_DEFAULT;
	obj->pinmux_pad3 = PINMUX_DEFAULT;
	obj->start_frame_detection_enable = false;
	
    obj->mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;  // to be done according to the pin received from user
    obj->pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
    obj->pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
    obj->pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
    obj->pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;

	//TODO: now noly UART0. code to get the SERCOM instance from Pins (pinmapping) to be added later
	UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
	UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);

	sercom_index = _sercom_get_sercom_inst_index(obj->usart);

	pm_index     = sercom_index + PM_APBCMASK_SERCOM0_Pos;
	gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
		
	if (_USART(obj).CTRLA.reg & SERCOM_USART_CTRLA_SWRST) {
		/* The module is busy resetting itself */
	}
	
	if (_USART(obj).CTRLA.reg & SERCOM_USART_CTRLA_ENABLE) {
		/* Check the module is enabled */
	}
	
	/* Turn on module in PM */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 << pm_index);
	
	/* Set up the GCLK for the module */
	obj->generator_source = GCLK_GENERATOR_0;  
    gclk_chan_conf.source_generator = obj->generator_source;
	system_gclk_chan_set_config(gclk_index, &gclk_chan_conf);
	system_gclk_chan_enable(gclk_index);
	sercom_set_gclk_generator(obj->generator_source, false);
	
    /* Set configuration according to the config struct */
	usart_set_config_asf(obj);
	struct system_pinmux_config pin_conf;
	system_pinmux_get_config_defaults(&pin_conf);
	pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
	pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;

	uint32_t pad_pinmuxes[] = {
			obj->pinmux_pad0, obj->pinmux_pad1,
			obj->pinmux_pad2, obj->pinmux_pad3
	};
	
	/* Configure the SERCOM pins according to the user configuration */
	for (uint8_t pad = 0; pad < 4; pad++) {
		uint32_t current_pinmux = pad_pinmuxes[pad];

		if (current_pinmux == PINMUX_DEFAULT) {
			current_pinmux = _sercom_get_default_pad(obj->usart, pad);
		}

		if (current_pinmux != PINMUX_UNUSED) {
			pin_conf.mux_position = current_pinmux & 0xFFFF;
			system_pinmux_pin_set_config(current_pinmux >> 16, &pin_conf);
		}
	}
	
	if (uart == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
	
	/* Wait until synchronization is complete */
    usart_syncing(obj);
	
	/* Enable USART module */
    enable_usart(obj);
	
}

void serial_free(serial_t *obj) {
    serial_irq_ids[obj->index] = 0;
}

void serial_baud(serial_t *obj, int baudrate) {
    MBED_ASSERT((baudrate == 110) || (baudrate == 150) || (baudrate == 300) || (baudrate == 1200) ||
	    (baudrate == 2400) || (baudrate == 4800) || (baudrate == 9600) || (baudrate == 19200) || (baudrate == 38400) ||
		    (baudrate == 57600) || (baudrate == 115200) || (baudrate == 230400) || (baudrate == 460800) || (baudrate == 921600) );   
    
    struct system_gclk_chan_config gclk_chan_conf;
    uint32_t gclk_index;
    uint16_t baud  = 0;
    uint32_t sercom_index = 0; 
    enum sercom_asynchronous_operation_mode mode = SERCOM_ASYNC_OPERATION_MODE_ARITHMETIC;
    enum sercom_asynchronous_sample_num sample_num = SERCOM_ASYNC_SAMPLE_NUM_16;

    obj->baudrate = baudrate; 
    disable_usart(obj);

    sercom_index = _sercom_get_sercom_inst_index(obj->usart);
	gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;
	
    obj->generator_source = GCLK_GENERATOR_0;  
    gclk_chan_conf.source_generator = obj->generator_source;
	system_gclk_chan_set_config(gclk_index, &gclk_chan_conf);
	system_gclk_chan_enable(gclk_index);
	sercom_set_gclk_generator(obj->generator_source, false);

    /* Get baud value from mode and clock */
    switch (obj->transfer_mode)
    {
        case USART_TRANSFER_SYNCHRONOUSLY:
            if (!obj->use_external_clock) {
                    _sercom_get_sync_baud_val(obj->baudrate,
                        system_gclk_chan_get_hz(gclk_index), &baud);
            }
            break;

        case USART_TRANSFER_ASYNCHRONOUSLY:
            if (obj->use_external_clock) {
                _sercom_get_async_baud_val(obj->baudrate,
                            obj->ext_clock_freq, &baud, mode, sample_num); 
            } else { 
                        _sercom_get_async_baud_val(obj->baudrate,
                                system_gclk_chan_get_hz(gclk_index), &baud, mode, sample_num);
            }
            break;
    }
	/* Wait until synchronization is complete */
    usart_syncing(obj);

	/*Set baud val */
    _USART(obj).BAUD.reg = baud;
    /* Wait until synchronization is complete */
    usart_syncing(obj);	
    
	enable_usart(obj);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2));
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven));
    MBED_ASSERT((data_bits == 5) || (data_bits == 6) || (data_bits == 7) || (data_bits == 8) /*|| (data_bits == 9)*/);
	
    /* Cache new register values to minimize the number of register writes */
    uint32_t ctrla = 0;
    uint32_t ctrlb = 0;

    disable_usart(obj);
	
	ctrla = _USART(obj).CTRLA.reg;
    ctrlb = _USART(obj).CTRLB.reg;
	
    ctrla &= ~(SERCOM_USART_CTRLA_FORM_Msk);
	ctrlb &= ~(SERCOM_USART_CTRLB_CHSIZE_Msk);
    ctrlb &= ~(SERCOM_USART_CTRLB_SBMODE);
    ctrlb &= ~(SERCOM_USART_CTRLB_PMODE);
	
    switch (stop_bits){
	    case 1:
		    obj->stopbits = USART_STOPBITS_1;
			break;
		case 2:
		    obj->stopbits = USART_STOPBITS_2;
			break;
		default:
		    obj->stopbits = USART_STOPBITS_1;
	}
	
	switch (parity){
	    case ParityNone:
		    obj->parity = USART_PARITY_NONE;
			break;
		case ParityOdd:
		    obj->parity = USART_PARITY_ODD;
			break;
		case ParityEven:
		    obj->parity = USART_PARITY_EVEN;
			break;
		default:
		    obj->parity = USART_PARITY_NONE;
	}
	
	switch (data_bits){
	    case 5:
		    obj->character_size = USART_CHARACTER_SIZE_5BIT;
			break;
		case 6:
		    obj->character_size = USART_CHARACTER_SIZE_6BIT;
			break;
		case 7:
		    obj->character_size = USART_CHARACTER_SIZE_7BIT;
			break;
	    case 8:
		    obj->character_size = USART_CHARACTER_SIZE_8BIT;
			break;
	    /*case 9:
		    obj->character_size = USART_CHARACTER_SIZE_9BIT;
			break;*/
		default:
		    obj->character_size = USART_CHARACTER_SIZE_8BIT;
	}
	
	
    /* Set stopbits, character size and enable transceivers */
    ctrlb = (uint32_t)obj->stopbits | (uint32_t)obj->character_size;

	/* Check parity mode bits */
	if (obj->parity != USART_PARITY_NONE) {
		ctrla |= SERCOM_USART_CTRLA_FORM(1);
		ctrlb |= obj->parity;
	} else {
		ctrla |= SERCOM_USART_CTRLA_FORM(0);
	}
	
    	/* Write configuration to CTRLB */
    _USART(obj).CTRLB.reg = ctrlb;

    /* Wait until synchronization is complete */
    usart_syncing(obj);

    /* Write configuration to CTRLA */
    _USART(obj).CTRLA.reg = ctrla;

    /* Wait until synchronization is complete */
    usart_syncing(obj);
	
    enable_usart(obj);
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
static inline void uart_irq(SercomUsart *const usart, uint32_t index) {
    uint16_t interrupt_status;
    interrupt_status = usart->INTFLAG.reg;
    interrupt_status &= usart->INTENSET.reg;
    if (serial_irq_ids[index] != 0) {
        if (interrupt_status & SERCOM_USART_INTFLAG_TXC)  // for transmit complete
		{
            usart->INTENCLR.reg = SERCOM_USART_INTFLAG_TXC;
            irq_handler(serial_irq_ids[index], TxIrq);
     	}
		/*if (interrupt_status & SERCOM_USART_INTFLAG_DRE)  // for data ready for transmit
		{
            if (uart_data[index].count > 0){
				usart->DATA.reg = uart_data[index].string[uart_data[index].count];
				uart_data[index].count--;
			}
			if(uart_data[index].count == 0){
			    usart->INTENCLR.reg = SERCOM_USART_INTFLAG_DRE;
			    usart->INTENSET.reg = SERCOM_USART_INTFLAG_TXC;
            } else {
			    usart->INTENCLR.reg = SERCOM_USART_INTFLAG_DRE;
			}
		}*/
        if (interrupt_status & SERCOM_USART_INTFLAG_RXC)  // for receive complete
		{
			usart->INTENCLR.reg = SERCOM_USART_INTFLAG_RXC;
            irq_handler(serial_irq_ids[index], RxIrq);
		}
    }
}

void uart0_irq() {
    uart_irq((SercomUsart *)UART_0, 0);
}

/*#if UART_NUM > 1
void uart1_irq() {uart_irq(UART1->S1, 1);}
void uart2_irq() {uart_irq(UART2->S1, 2);}
#endif
*/

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;
    uint32_t ctrlb = 0;
	
    disable_usart(obj);
    ctrlb = _USART(obj).CTRLB.reg;
	
    switch ((int)obj->usart) {
        case UART_0: 
			irq_n = SERCOM0_IRQn;
			vector = (uint32_t)&uart0_irq;
			break;
    }
	
    if (enable) {
		switch (irq){
			case RxIrq:
                 ctrlb |= (SERCOM_USART_CTRLB_RXEN);
				 break;
			case TxIrq:
                 ctrlb |= (SERCOM_USART_CTRLB_TXEN);
				 break;
 		}
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);

    } else { 
        switch (irq) {
			case RxIrq:
                 ctrlb &= ~(SERCOM_USART_CTRLB_RXEN);
				 break;
				break;
			case TxIrq:
                 ctrlb &= ~(SERCOM_USART_CTRLB_TXEN);
				 break;
        }
		NVIC_DisableIRQ(irq_n);
    }
    _USART(obj).CTRLB.reg = ctrlb; 
    enable_usart(obj);
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj) {
    _USART(obj).INTENSET.reg = SERCOM_USART_INTFLAG_RXC;  // test
    while (!serial_readable(obj));
        return _USART(obj).DATA.reg ;
}

void serial_putc(serial_t *obj, int c) {
	uint16_t q = (c & SERCOM_USART_DATA_MASK);
    while (!serial_writable(obj));
//	uart_data[obj->index].count++;
//    uart_data[obj->index].string[uart_data[obj->index].count] = q;
//    _USART(obj).INTENSET.reg = SERCOM_USART_INTFLAG_DRE;
	_USART(obj).DATA.reg = q;
	while (!(_USART(obj).INTFLAG.reg & SERCOM_USART_INTFLAG_TXC));  // wait till data is sent
}

int serial_readable(serial_t *obj) {
	uint32_t status = 1;
	if (!(_USART(obj).INTFLAG.reg & SERCOM_USART_INTFLAG_RXC)) {
        status = 0;
	}
	else {
        status = 1;
	}
    return status;
}

int serial_writable(serial_t *obj) {
    uint32_t status = 1;
    if (!(_USART(obj).INTFLAG.reg & SERCOM_USART_INTFLAG_DRE)) {
        status = 0;
    }
    else {
        status = 1; 
    }
    return status;
}

/************************************************************************************
 * 			ASYNCHRONOUS HAL														*
 ************************************************************************************/

#if DEVICE_SERIAL_ASYNCH

/************************************
 * HELPER FUNCTIONS					*
 ***********************************/

/** Configure TX events
 *
 * @param obj    The serial object
 * @param event  The logical OR of the TX events to configure
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
void serial_tx_enable_event(serial_t *obj, int event, uint8_t enable)
{

}

/**
 * @param obj    The serial object.
 * @param event  The logical OR of the RX events to configure
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
void serial_rx_enable_event(serial_t *obj, int event, uint8_t enable)
{
  
}

/** Configure the TX buffer for an asynchronous write serial transaction
 *
 * @param obj       The serial object.
 * @param tx        The buffer for sending.
 * @param tx_length The number of words to transmit.
 */
void serial_tx_buffer_set(serial_t *obj, void *tx, int tx_length, uint8_t width)
{

}

/** Configure the TX buffer for an asynchronous read serial transaction
 *
 * @param obj       The serial object.
 * @param rx        The buffer for receiving.
 * @param rx_length The number of words to read.
 */
void serial_rx_buffer_set(serial_t *obj, void *rx, int rx_length, uint8_t width)
{

}

/** Set character to be matched. If an event is enabled, and received character
 *  matches the defined char_match, the receiving process is stopped and MATCH event
 *  is invoked
 *
 * @param obj        The serial object
 * @param char_match A character in range 0-254
 */
void serial_set_char_match(serial_t *obj, uint8_t char_match)
{

}

/************************************
 * TRANSFER FUNCTIONS				*
 ***********************************/

/** Begin asynchronous TX transfer. The used buffer is specified in the serial object,
 *  tx_buff
 *
 * @param obj  The serial object
 * @param cb   The function to call when an event occurs
 * @param hint A suggestion for how to use DMA with this transfer
 * @return Returns number of data transfered, or 0 otherwise
 */
int serial_tx_asynch(serial_t *obj, void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{

}

/** Begin asynchronous RX transfer (enable interrupt for data collecting)
 *  The used buffer is specified in the serial object - rx_buff
 *
 * @param obj  The serial object
 * @param cb   The function to call when an event occurs
 * @param hint A suggestion for how to use DMA with this transfer
 */
void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{

}

/** Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param obj The serial object
 * @return Non-zero if the TX transaction is ongoing, 0 otherwise
 */
uint8_t serial_tx_active(serial_t *obj)
{

}

/** Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_rx_active(serial_t *obj)
{

}

/** The asynchronous TX handler. Writes to the TX FIFO and checks for events.
 *  If any TX event has occured, the TX abort function is called.
 *
 * @param obj The serial object
 * @return Returns event flags if a TX transfer termination condition was met or 0 otherwise
 */
int serial_tx_irq_handler_asynch(serial_t *obj)
{

}

/** The asynchronous RX handler. Reads from the RX FIFOF and checks for events.
 *  If any RX event has occured, the RX abort function is called.
 *
 * @param obj The serial object
 * @return Returns event flags if a RX transfer termination condition was met or 0 otherwise
 */
int serial_rx_irq_handler_asynch(serial_t *obj)
{

}

/** Unified IRQ handler. Determines the appropriate handler to execute and returns the flags.
 *
 * WARNING: this code should be stateless, as re-entrancy is very possible in interrupt-based mode.
 */
int serial_irq_handler_asynch(serial_t *obj)
{

}

/** Abort the ongoing TX transaction. It disables the enabled interupt for TX and
 *  flush TX hardware buffer if TX FIFO is used
 *
 * @param obj The serial object
 */
void serial_tx_abort_asynch(serial_t *obj)
{
}

/** Abort the ongoing RX transaction It disables the enabled interrupt for RX and
 *  flush RX hardware buffer if RX FIFO is used
 *
 * @param obj The serial object
 */
void serial_rx_abort_asynch(serial_t *obj)
{
   
}

#endif