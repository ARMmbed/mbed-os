/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _INTCTRL_H_
#define _INTCTRL_H_

#define TIMER_IRQ_0 0
#define TIMER_IRQ_1 1
#define TIMER_IRQ_2 2
#define TIMER_IRQ_3 3
#define PWM_IRQ_WRAP 4
#define USBCTRL_IRQ 5
#define XIP_IRQ 6
#define PIO0_IRQ_0 7
#define PIO0_IRQ_1 8
#define PIO1_IRQ_0 9
#define PIO1_IRQ_1 10
#define DMA_IRQ_0 11
#define DMA_IRQ_1 12
#define IO_IRQ_BANK0 13
#define IO_IRQ_QSPI 14
#define SIO_IRQ_PROC0 15
#define SIO_IRQ_PROC1 16
#define CLOCKS_IRQ 17
#define SPI0_IRQ 18
#define SPI1_IRQ 19
#define UART0_IRQ 20
#define UART1_IRQ 21
#define ADC_IRQ_FIFO 22
#define I2C0_IRQ 23
#define I2C1_IRQ 24
#define RTC_IRQ 25

#define isr_timer_0 TIMER_IRQ_0_Handler
#define isr_timer_1 TIMER_IRQ_1_Handler
#define isr_timer_2 TIMER_IRQ_2_Handler
#define isr_timer_3 TIMER_IRQ_3_Handler
#define isr_pwm_wrap PWM_IRQ_WRAP_Handler
#define isr_usbctrl USBCTRL_IRQ_Handler
#define isr_xip XIP_IRQ_Handler
#define isr_pio0_0 PIO0_IRQ_0_Handler
#define isr_pio0_1 PIO0_IRQ_1_Handler
#define isr_pio1_0 PIO1_IRQ_0_Handler
#define isr_pio1_1 TIMER_IRQ_1_Handler0
#define isr_dma_0 TIMER_IRQ_1_Handler1
#define isr_dma_1 TIMER_IRQ_1_Handler2
#define isr_io_bank0 TIMER_IRQ_1_Handler3
#define isr_io_qspi TIMER_IRQ_1_Handler4
#define isr_sio_proc0 TIMER_IRQ_1_Handler5
#define isr_sio_proc1 TIMER_IRQ_1_Handler6
#define isr_clocks TIMER_IRQ_1_Handler7
#define isr_spi0 TIMER_IRQ_1_Handler8
#define isr_spi1 TIMER_IRQ_1_Handler9
#define isr_uart0 TIMER_IRQ_2_Handler0
#define isr_uart1 TIMER_IRQ_2_Handler1
#define isr_adc_fifo TIMER_IRQ_2_Handler2
#define isr_i2c0 TIMER_IRQ_2_Handler3
#define isr_i2c1 TIMER_IRQ_2_Handler4
#define isr_rtc TIMER_IRQ_2_Handler5

#endif // _INTCTRL_H_
