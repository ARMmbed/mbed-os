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
#include "mbed_assert.h"
#include "cmsis.h"
#include "serial_api.h"
#include <string.h>
#include "pinmap.h"
#include "PeripheralPins.h"
#include "usart.h"
#include "usart_interrupt.h"
#include "samr21_xplained_pro.h"

#define UART_NUM 1  // for SAMR21 // to be updated for samd21

struct usart_module usart_instance;

static uint32_t serial_irq_ids[UART_NUM] = {0};
static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

/**
 * \internal
 * Set Configuration of the USART module
 */
static enum status_code _usart_set_config(
		struct usart_module *const module,
		const struct usart_config *const config)
{
	/* Sanity check arguments */
	Assert(module);
	Assert(module->hw);

	/* Get a pointer to the hardware module instance */
	SercomUsart *const usart_hw = &(module->hw->USART);

	/* Index for generic clock */
	uint32_t sercom_index = _sercom_get_sercom_inst_index(module->hw);
	uint32_t gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;

	/* Cache new register values to minimize the number of register writes */
	uint32_t ctrla = 0;
	uint32_t ctrlb = 0;
	uint16_t baud  = 0;

	enum sercom_asynchronous_operation_mode mode = SERCOM_ASYNC_OPERATION_MODE_ARITHMETIC;
	enum sercom_asynchronous_sample_num sample_num = SERCOM_ASYNC_SAMPLE_NUM_16;

#ifdef FEATURE_USART_OVER_SAMPLE
	switch (config->sample_rate) {
		case USART_SAMPLE_RATE_16X_ARITHMETIC:
			mode = SERCOM_ASYNC_OPERATION_MODE_ARITHMETIC;
			sample_num = SERCOM_ASYNC_SAMPLE_NUM_16;
			break;
		case USART_SAMPLE_RATE_8X_ARITHMETIC:
			mode = SERCOM_ASYNC_OPERATION_MODE_ARITHMETIC;
			sample_num = SERCOM_ASYNC_SAMPLE_NUM_8;
			break;
		case USART_SAMPLE_RATE_3X_ARITHMETIC:
			mode = SERCOM_ASYNC_OPERATION_MODE_ARITHMETIC;
			sample_num = SERCOM_ASYNC_SAMPLE_NUM_3;
			break;
		case USART_SAMPLE_RATE_16X_FRACTIONAL:
			mode = SERCOM_ASYNC_OPERATION_MODE_FRACTIONAL;
			sample_num = SERCOM_ASYNC_SAMPLE_NUM_16;
			break;
		case USART_SAMPLE_RATE_8X_FRACTIONAL:
			mode = SERCOM_ASYNC_OPERATION_MODE_FRACTIONAL;
			sample_num = SERCOM_ASYNC_SAMPLE_NUM_8;
			break;
	}
#endif

	/* Set data order, internal muxing, and clock polarity */
	ctrla = (uint32_t)config->data_order |
		(uint32_t)config->mux_setting |
	#ifdef FEATURE_USART_OVER_SAMPLE
		config->sample_adjustment |
		config->sample_rate |
	#endif
	#ifdef FEATURE_USART_IMMEDIATE_BUFFER_OVERFLOW_NOTIFICATION
		(config->immediate_buffer_overflow_notification << SERCOM_USART_CTRLA_IBON_Pos) |
	#endif
		(config->clock_polarity_inverted << SERCOM_USART_CTRLA_CPOL_Pos);

	enum status_code status_code = STATUS_OK;

	/* Get baud value from mode and clock */
	switch (config->transfer_mode)
	{
		case USART_TRANSFER_SYNCHRONOUSLY:
			if (!config->use_external_clock) {
				status_code = _sercom_get_sync_baud_val(config->baudrate,
						system_gclk_chan_get_hz(gclk_index), &baud);
			}

			break;

		case USART_TRANSFER_ASYNCHRONOUSLY:
			if (config->use_external_clock) {
				status_code =
						_sercom_get_async_baud_val(config->baudrate,
							config->ext_clock_freq, &baud, mode, sample_num);
			} else {
				status_code =
						_sercom_get_async_baud_val(config->baudrate,
							system_gclk_chan_get_hz(gclk_index), &baud, mode, sample_num);
			}

			break;
	}

	/* Check if calculating the baudrate failed */
	if (status_code != STATUS_OK) {
		/* Abort */
		return status_code;
	}

#ifdef FEATURE_USART_IRDA
	if(config->encoding_format_enable) {
		usart_hw->RXPL.reg = config->receive_pulse_length;
	}
#endif

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(module);

	/*Set baud val */
	usart_hw->BAUD.reg = baud;

	/* Set sample mode */
	ctrla |= config->transfer_mode;

	if (config->use_external_clock == false) {
		ctrla |= SERCOM_USART_CTRLA_MODE(0x1);
	}
	else {
		ctrla |= SERCOM_USART_CTRLA_MODE(0x0);
	}

	/* Set stopbits, character size and enable transceivers */
	ctrlb = (uint32_t)config->stopbits | (uint32_t)config->character_size |
		#ifdef FEATURE_USART_IRDA
			(config->encoding_format_enable << SERCOM_USART_CTRLB_ENC_Pos) |
		#endif
		#ifdef FEATURE_USART_START_FRAME_DECTION
			(config->start_frame_detection_enable << SERCOM_USART_CTRLB_SFDE_Pos) |
		#endif
		#ifdef FEATURE_USART_COLLISION_DECTION
			(config->collision_detection_enable << SERCOM_USART_CTRLB_COLDEN_Pos) |
		#endif
			(config->receiver_enable << SERCOM_USART_CTRLB_RXEN_Pos) |
			(config->transmitter_enable << SERCOM_USART_CTRLB_TXEN_Pos);

	/* Check parity mode bits */
	if (config->parity != USART_PARITY_NONE) {
#ifdef FEATURE_USART_LIN_SLAVE
		if(config->lin_slave_enable) {
			ctrla |= SERCOM_USART_CTRLA_FORM(0x5);
		} else {
			ctrla |= SERCOM_USART_CTRLA_FORM(1);
		}
#else
		ctrla |= SERCOM_USART_CTRLA_FORM(1);
#endif
		ctrlb |= config->parity;
	} else {
#ifdef FEATURE_USART_LIN_SLAVE
		if(config->lin_slave_enable) {
			ctrla |= SERCOM_USART_CTRLA_FORM(0x4);
		} else {
			ctrla |= SERCOM_USART_CTRLA_FORM(0);
		}
#else
		ctrla |= SERCOM_USART_CTRLA_FORM(0);
#endif
	}

	/* Set whether module should run in standby. */
	if (config->run_in_standby || system_is_debugger_present()) {
		ctrla |= SERCOM_USART_CTRLA_RUNSTDBY;
	}

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(module);

	/* Write configuration to CTRLB */
	usart_hw->CTRLB.reg = ctrlb;

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(module);

	/* Write configuration to CTRLA */
	usart_hw->CTRLA.reg = ctrla;

	return STATUS_OK;
}

void serial_init(serial_t *obj, PinName tx, PinName rx) {

    struct usart_config config_usart;
	struct system_gclk_chan_config gclk_chan_conf;
	uint32_t gclk_index;
	uint32_t pm_index;
	uint32_t sercom_index = 0;  // index 0 for SERCOM0
	enum status_code status_code = STATUS_OK;
	
	usart_get_config_defaults(&config_usart); // get default configuration setting used below
	
    config_usart.baudrate    = 9600;  // setting default configurations for SERCOM0 serial
    config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
    config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
    config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
    config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
    config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;

	//TODO: now noly UART0. code to get the SERCOM instance from Pins (pinmapping) to be added later
	UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
	UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);

    obj->uart = (SercomUsart *)uart;
	usart_instance.hw = (Sercom *)uart;
	
	pm_index = sercom_index + PM_APBCMASK_SERCOM0_Pos;
	gclk_index = sercom_index + SERCOM0_GCLK_ID_CORE;
		
	if (obj->uart->CTRLA.reg & SERCOM_USART_CTRLA_SWRST) {
		/* The module is busy resetting itself */
		return;
	}
	
	if (obj->uart->CTRLA.reg & SERCOM_USART_CTRLA_ENABLE) {
		/* Check the module is enabled */
		return;
	}
	
	/* Turn on module in PM */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 << pm_index);
	
	/* Set up the GCLK for the module */
	system_gclk_chan_get_config_defaults(&gclk_chan_conf);
	gclk_chan_conf.source_generator = config_usart.generator_source;
	system_gclk_chan_set_config(gclk_index, &gclk_chan_conf);
	system_gclk_chan_enable(gclk_index);
	sercom_set_gclk_generator(config_usart.generator_source, false);
	
	/* Set character size */
	usart_instance.character_size = config_usart.character_size;
	
	/* Set transmitter and receiver status */
	usart_instance.receiver_enabled = config_usart.receiver_enable;
	usart_instance.transmitter_enabled = config_usart.transmitter_enable;	
	
#ifdef FEATURE_USART_LIN_SLAVE
	usart_instance.lin_slave_enabled = config_usart.lin_slave_enable;
#endif
#ifdef FEATURE_USART_START_FRAME_DECTION
	usart_instance.start_frame_detection_enabled = config_usart.start_frame_detection_enable;
#endif

	/* Set configuration according to the config struct */
	status_code = _usart_set_config(&usart_instance, &config_usart);
	if(status_code != STATUS_OK) {
		return;
	}
	
	struct system_pinmux_config pin_conf;
	system_pinmux_get_config_defaults(&pin_conf);
	pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
	pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;

	uint32_t pad_pinmuxes[] = {
			config_usart.pinmux_pad0, config_usart.pinmux_pad1,
			config_usart.pinmux_pad2, config_usart.pinmux_pad3
	};
	
	/* Configure the SERCOM pins according to the user configuration */
	for (uint8_t pad = 0; pad < 4; pad++) {
		uint32_t current_pinmux = pad_pinmuxes[pad];

		if (current_pinmux == PINMUX_DEFAULT) {
			current_pinmux = _sercom_get_default_pad(usart_instance.hw, pad);
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
	
	system_interrupt_enable(_sercom_get_interrupt_vector(usart_instance.hw));
	
	/* Wait until synchronization is complete */
	_usart_wait_for_sync(&usart_instance);
	
	/* Enable USART module */
	obj->uart->CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;	
	
/*    usart_get_config_defaults(&config_usart);

    config_usart.baudrate    = 9600;
    config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
    config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
    config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
    config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
    config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
    while (usart_init(&usart_instance,
            EDBG_CDC_MODULE, &config_usart) != STATUS_OK) {
	}*/
}
/*
void serial_free(serial_t *obj) {
    serial_irq_ids[obj->index] = 0;
}*/

void serial_baud(serial_t *obj, int baudrate) {
/*    struct usart_config config_usart;

    usart_get_config_defaults(&config_usart);	
    config_usart.baudrate    = baudrate;
    while (usart_init(&usart_instance,
            EDBG_CDC_MODULE, &config_usart) != STATUS_OK) {
	}*/
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
   /* struct usart_config config_usart;
    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2));
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven));
    MBED_ASSERT((data_bits == 5) || (data_bits == 6) || (data_bits == 7) || (data_bits == 8) || (data_bits == 9));

    usart_get_config_defaults(&config_usart);
    switch (stop_bits){
	    case 1:
		    config_usart.stopbits = USART_STOPBITS_1;
			break;
		case 2:
		    config_usart.stopbits = USART_STOPBITS_2;
			break;
		default:
		    config_usart.stopbits = USART_STOPBITS_1;
	}
	
	switch (parity){
	    case ParityNone:
		    config_usart.parity = USART_PARITY_NONE;
			break;
		case ParityOdd:
		    config_usart.parity = USART_PARITY_ODD;
			break;
		case ParityEven:
		    config_usart.parity = USART_PARITY_EVEN;
			break;
		default:
		    config_usart.parity = USART_PARITY_NONE;
	}
	
	switch (data_bits){
	    case 5:
		    config_usart.character_size = USART_CHARACTER_SIZE_5BIT;
			break;
		case 6:
		    config_usart.character_size = USART_CHARACTER_SIZE_6BIT;
			break;
		case 7:
		    config_usart.character_size = USART_CHARACTER_SIZE_7BIT;
			break;
	    case 8:
		    config_usart.character_size = USART_CHARACTER_SIZE_8BIT;
			break;
	    case 9:
		    config_usart.character_size = USART_CHARACTER_SIZE_9BIT;
			break;
		default:
		    config_usart.character_size = USART_CHARACTER_SIZE_8BIT;
	}
	
	while (usart_init(&usart_instance,
            EDBG_CDC_MODULE, &config_usart) != STATUS_OK) {
	}*/
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
static inline void uart_irq(SercomUsart *const uart, uint32_t index) {
    uint16_t interrupt_status;
    /* Read and mask interrupt flag register */
    interrupt_status = uart->INTFLAG.reg;
    interrupt_status &= uart->INTENSET.reg;
    if (serial_irq_ids[index] != 0) {
        if (interrupt_status & /*SERCOM_USART_INTFLAG_TXC*/SERCOM_USART_INTFLAG_DRE)  // for transmit complete
            irq_handler(serial_irq_ids[index], TxIrq);
        if (interrupt_status & SERCOM_USART_INTFLAG_RXC)  // for recieve complete
            irq_handler(serial_irq_ids[index], RxIrq);
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
    switch ((int)obj->uart) {
        case UART_0: 
		    irq_n = (uint32_t)&SERCOM0_Handler;
			vector = (uint32_t)&uart0_irq;
			break;
    }

    if (enable) {
//        usart_enable(&usart_instance);
		switch (irq){
		    case RxIrq: obj->uart->INTENSET.reg |= SERCOM_USART_INTENSET_RXC; break;
		    case TxIrq: obj->uart->INTENSET.reg |= /*SERCOM_USART_INTENSET_TXC*/SERCOM_USART_INTFLAG_DRE; break;
 		}
       /* switch (irq) {
            case RxIrq: obj->uart->C2 |= (UARTLP_C2_RIE_MASK); break;
            case TxIrq: obj->uart->C2 |= (UARTLP_C2_TIE_MASK); break;
        }*/
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);

    } else { // disable
//	    usart_disable(&usart_instance);
//        int all_disabled = 0;
//        SerialIrq other_irq = (irq == RxIrq) ? (TxIrq) : (RxIrq);
        switch (irq) {
            case RxIrq: obj->uart->INTENCLR.reg &= ~(SERCOM_USART_INTENCLR_RXC); break;
            case TxIrq: obj->uart->INTENCLR.reg &= ~(/*SERCOM_USART_INTENCLR_TXC*/SERCOM_USART_INTFLAG_DRE); break;
        }
		NVIC_DisableIRQ(irq_n);
        /*int all_disabled = 0;
        SerialIrq other_irq = (irq == RxIrq) ? (TxIrq) : (RxIrq);
        switch (irq) {
            case RxIrq: obj->uart->C2 &= ~(UARTLP_C2_RIE_MASK); break;
            case TxIrq: obj->uart->C2 &= ~(UARTLP_C2_TIE_MASK); break;
        }
        switch (other_irq) {
            case RxIrq: all_disabled = (obj->uart->C2 & (UARTLP_C2_RIE_MASK)) == 0; break;
            case TxIrq: all_disabled = (obj->uart->C2 & (UARTLP_C2_TIE_MASK)) == 0; break;
        }
        if (all_disabled)
            NVIC_DisableIRQ(irq_n);*/
    }
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj) {
    while (!serial_readable(obj));
        return obj->uart->DATA.reg ;
//    return obj->uart->D;
}

void serial_putc(serial_t *obj, int c) {
    while (!serial_writable(obj));
	obj->uart->DATA.reg = (c & SERCOM_USART_DATA_MASK);
//    obj->uart->D = c;
}

int serial_readable(serial_t *obj) {
    // check overrun
    /*if (obj->uart->S1 &  UARTLP_S1_OR_MASK) {
        obj->uart->S1 |= UARTLP_S1_OR_MASK;
    }*/
    return 0/*(obj->uart->S1 & UARTLP_S1_RDRF_MASK)*/;
}

int serial_writable(serial_t *obj) {
    // check overrun
    /*if (obj->uart->S1 &  UARTLP_S1_OR_MASK) {
        obj->uart->S1 |= UARTLP_S1_OR_MASK;
    }*/
    return 0/*(obj->uart->S1 & UARTLP_S1_TDRE_MASK)*/;
}

void serial_clear(serial_t *obj) {
}

void serial_pinout_tx(PinName tx) {
//    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj) {
//    obj->uart->C2 |= UARTLP_C2_SBK_MASK;
}

void serial_break_clear(serial_t *obj) {
//    obj->uart->C2 &= ~UARTLP_C2_SBK_MASK;
}
