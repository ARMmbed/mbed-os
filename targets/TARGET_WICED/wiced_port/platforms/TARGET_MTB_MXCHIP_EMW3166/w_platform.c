/* mbed Microcontroller Library
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
 */

#include "w_platform.h"
#include "w_platform_config.h"
#include "w_platform_init.h"
#include "w_platform_isr.h"
#include "w_platform_peripheral.h"
#include "wwd_platform_common.h"
#include "wwd_rtos_isr.h"
#include "wiced_defaults.h"
#include "wiced_platform.h"


/* GPIO pin table. Used by WICED/platform/MCU/wiced_platform_common.c */
const platform_gpio_t platform_gpio_pins[] =
{
	[WICED_GPIO_2] = { GPIOB,  2 },
	[WICED_GPIO_4] = { GPIOB, 15 },
	[WICED_GPIO_5] = { GPIOB, 12 },
	[WICED_GPIO_6] = { GPIOB, 13 },
	[WICED_GPIO_7] = { GPIOB, 14 },
	[WICED_GPIO_8] = { GPIOC,  6 },
	[WICED_GPIO_9] = { GPIOA, 15 },
	[WICED_GPIO_12] = { GPIOC, 7 },
	[WICED_GPIO_14] = { GPIOC, 0 },
	[WICED_GPIO_16] = { GPIOC, 13 },
	[WICED_GPIO_17] = { GPIOB,  8 },
	[WICED_GPIO_18] = { GPIOB,  9 },
	[WICED_GPIO_19] = { GPIOB, 10 },
	[WICED_GPIO_25] = { GPIOA, 14 },
	[WICED_GPIO_26] = { GPIOA, 13 },
	[WICED_GPIO_27] = { GPIOB,  3 },
	[WICED_GPIO_29] = { GPIOB,  7 },
	[WICED_GPIO_30] = { GPIOB,  6 },
	[WICED_GPIO_31] = { GPIOB,  4 },
	[WICED_GPIO_33] = { GPIOA, 10 },
	[WICED_GPIO_34] = { GPIOA, 12 },
	[WICED_GPIO_35] = { GPIOA, 11 },
	[WICED_GPIO_36] = { GPIOA,  5 },
	[WICED_GPIO_37] = { GPIOB,  0 },
	[WICED_GPIO_38] = { GPIOA,  4 },
};

const platform_adc_t platform_adc_peripherals[] =
{
    [WICED_ADC_1] = {ADC1, ADC_Channel_4, RCC_APB2Periph_ADC1, 1, (platform_gpio_t*)&platform_gpio_pins[WICED_GPIO_38]},
    [WICED_ADC_2] = {ADC1, ADC_Channel_5, RCC_APB2Periph_ADC1, 1, (platform_gpio_t*)&platform_gpio_pins[WICED_GPIO_34]},
};

const platform_i2c_t platform_i2c_peripherals[] =
{
    [WICED_I2C_1] =
    {
        .port                    = I2C1,
        .pin_scl                 = &platform_gpio_pins[WICED_GPIO_17],
        .pin_sda                 = &platform_gpio_pins[WICED_GPIO_18],
        .peripheral_clock_reg    = RCC_APB1Periph_I2C1,
        .tx_dma                  = DMA1,
        .tx_dma_peripheral_clock = RCC_AHB1Periph_DMA1,
        .tx_dma_stream           = DMA1_Stream7,
        .rx_dma_stream           = DMA1_Stream5,
        .tx_dma_stream_id        = 7,
        .rx_dma_stream_id        = 5,
        .tx_dma_channel          = DMA_Channel_1,
        .rx_dma_channel          = DMA_Channel_1,
        .gpio_af                 = GPIO_AF_I2C1
    },
};

const wiced_i2c_device_t auth_chip_i2c_device =
{
    .port          = WICED_I2C_1,
    .address       = 0x11,
    .address_width = I2C_ADDRESS_WIDTH_7BIT,
    .speed_mode    = I2C_STANDARD_SPEED_MODE,
};

const platform_uart_t platform_uart_peripherals[] =
{
  [WICED_UART_1] =
  {
    .port                         = USART6,
    .tx_pin                       = &platform_gpio_pins[WICED_GPIO_8],
    .rx_pin                       = &platform_gpio_pins[WICED_GPIO_12],
    .cts_pin                      = NULL,
    .rts_pin                      = NULL,
    .tx_dma_config =
    {
      .controller                 = DMA2,
      .stream                     = DMA2_Stream6,
      .channel                    = DMA_Channel_5,
      .irq_vector                 = DMA2_Stream6_IRQn,
      .complete_flags             = DMA_HISR_TCIF6,
      .error_flags                = ( DMA_HISR_TEIF6 | DMA_HISR_FEIF6 ),
    },
    .rx_dma_config =
    {
      .controller                 = DMA2,
      .stream                     = DMA2_Stream1,
      .channel                    = DMA_Channel_5,
      .irq_vector                 = DMA2_Stream1_IRQn,
      .complete_flags             = DMA_LISR_TCIF1,
      .error_flags                = ( DMA_LISR_TEIF1 | DMA_LISR_FEIF1 | DMA_LISR_DMEIF1 ),
    },
  },
  [WICED_UART_2] =
  {
    .port                         = USART1,
    .tx_pin                       = &platform_gpio_pins[WICED_GPIO_30],
    .rx_pin                       = &platform_gpio_pins[WICED_GPIO_29],
    .cts_pin                      = NULL,
    .rts_pin                      = NULL,
    .tx_dma_config =
    {
      .controller                 = DMA2,
      .stream                     = DMA2_Stream7,
      .channel                    = DMA_Channel_4,
      .irq_vector                 = DMA2_Stream7_IRQn,
      .complete_flags             = DMA_HISR_TCIF7,
      .error_flags                = ( DMA_HISR_TEIF7 | DMA_HISR_FEIF7 ),
    },
    .rx_dma_config =
    {
      .controller                 = DMA2,
      .stream                     = DMA2_Stream2,
      .channel                    = DMA_Channel_4,
      .irq_vector                 = DMA2_Stream2_IRQn,
      .complete_flags             = DMA_LISR_TCIF2,
      .error_flags                = ( DMA_LISR_TEIF2 | DMA_LISR_FEIF2 | DMA_LISR_DMEIF2 ),
    },
  },
};
platform_uart_driver_t platform_uart_drivers[WICED_UART_MAX];

#ifndef WICED_DISABLE_STDIO
static const platform_uart_config_t stdio_config =
{
    .baud_rate    = 115200,
    .data_width   = DATA_WIDTH_8BIT,
    .parity       = NO_PARITY,
    .stop_bits    = STOP_BITS_1,
    .flow_control = FLOW_CONTROL_DISABLED,
};
#endif

const platform_spi_t platform_spi_peripherals[] =
{
  [WICED_SPI_1]  =
  {
      .port                         = SPI2,
      .gpio_af                      = GPIO_AF_SPI2,
      .peripheral_clock_reg         = RCC_APB1Periph_SPI2,
      .peripheral_clock_func        = RCC_APB1PeriphClockCmd,
      .pin_mosi                     = &platform_gpio_pins[WICED_GPIO_4],
      .pin_miso                     = &platform_gpio_pins[WICED_GPIO_7],
      .pin_clock                    = &platform_gpio_pins[WICED_GPIO_6],
      .tx_dma =
      {
        .controller                 = DMA1,
        .stream                     = DMA1_Stream4,
        .channel                    = DMA_Channel_0,
        .irq_vector                 = DMA1_Stream4_IRQn,
        .complete_flags             = DMA_HISR_TCIF4,
        .error_flags                = ( DMA_HISR_TEIF4 | DMA_HISR_FEIF4 ),
      },
      .rx_dma =
      {
        .controller                 = DMA1,
        .stream                     = DMA1_Stream3,
        .channel                    = DMA_Channel_0,
        .irq_vector                 = DMA1_Stream3_IRQn,
        .complete_flags             = DMA_LISR_TCIF3,
        .error_flags                = ( DMA_LISR_TEIF3 | DMA_LISR_FEIF3 | DMA_LISR_DMEIF3 ),
      },
   }
};


#if defined ( WICED_PLATFORM_INCLUDES_SPI_FLASH )
const wiced_spi_device_t wiced_spi_flash =
{
    .port        = WICED_SPI_1,
    .chip_select = WICED_SPI_FLASH_CS,
    .speed       = 20000000, // 50 MHz
    .mode        = (SPI_CLOCK_RISING_EDGE | SPI_CLOCK_IDLE_HIGH | SPI_NO_DMA | SPI_MSB_FIRST),
    .bits        = 8
};
#endif

const platform_gpio_t wifi_control_pins[] =
{
  [WWD_PIN_RESET]           = { GPIOA, 9 },
};

const platform_gpio_t wifi_sdio_pins[] =
{
  [WWD_PIN_SDIO_OOB_IRQ] = { GPIOC,  0 },
  [WWD_PIN_SDIO_CLK    ] = { GPIOC, 12 },
  [WWD_PIN_SDIO_CMD    ] = { GPIOD,  2 },
  [WWD_PIN_SDIO_D0     ] = { GPIOC,  8 },
  [WWD_PIN_SDIO_D1     ] = { GPIOC,  9 },
  [WWD_PIN_SDIO_D2     ] = { GPIOC, 10 },
  [WWD_PIN_SDIO_D3     ] = { GPIOB,  5 },
};

/******************************************************
 *               Function Definitions
 ******************************************************/

void platform_init_peripheral_IRQHandler_priorities( void )
{
    /* Interrupt priority setup. Called by WICED/platform/MCU/STM32F2xx/platform_init.c */
    NVIC_SetPriority( RTC_WKUP_IRQn    ,  1 ); /* RTC Wake-up event   */
    NVIC_SetPriority( SDIO_IRQn        ,  2 ); /* WLAN SDIO           */
    NVIC_SetPriority( DMA2_Stream3_IRQn,  3 ); /* WLAN SDIO DMA       */
    NVIC_SetPriority( DMA1_Stream3_IRQn,  3 ); /* WLAN SPI DMA        */
    NVIC_SetPriority( USART6_IRQn      ,  6 ); /* WICED_UART_1        */
    NVIC_SetPriority( USART1_IRQn      ,  6 ); /* WICED_UART_2        */
    NVIC_SetPriority( DMA2_Stream6_IRQn,  7 ); /* WICED_UART_1 TX DMA */
    NVIC_SetPriority( DMA2_Stream1_IRQn,  7 ); /* WICED_UART_1 RX DMA */
    NVIC_SetPriority( DMA2_Stream7_IRQn,  7 ); /* WICED_UART_2 TX DMA */
    NVIC_SetPriority( DMA2_Stream2_IRQn,  7 ); /* WICED_UART_2 RX DMA */
    NVIC_SetPriority( EXTI0_IRQn       , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI1_IRQn       , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI2_IRQn       , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI3_IRQn       , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI4_IRQn       , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI9_5_IRQn     , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI15_10_IRQn   , 14 ); /* GPIO                */
}

void platform_init_external_devices( void )
{
    /*  This function initializes the external interfaces for the platform.
        You could use this to initialize the peripherals you use.
        Note however that if you initialize alternative functionality of 
        JTAG and UART pins, you could lock yourself out of being able to flash
        new firmware.
    */

    //patch added to resolve the microseconds delay hang issue.
    do
    {
        // enable DWT hardware and cycle counting
        CoreDebug->DEMCR = CoreDebug->DEMCR | CoreDebug_DEMCR_TRCENA_Msk;
        // reset a counter
        DWT->CYCCNT = 0;
        // enable the counter
        DWT->CTRL = (DWT->CTRL | DWT_CTRL_CYCCNTENA_Msk) ;
    }
    while(0);

    /* Initialise button to input by default */
    platform_gpio_init( &platform_gpio_pins[WICED_BUTTON1], INPUT_PULL_UP );

#ifndef GPIO_LED_NOT_SUPPORTED
    /* Initialise LEDs and turn off by default */
    platform_gpio_init( &platform_gpio_pins[WICED_LED1], OUTPUT_PUSH_PULL );
    platform_gpio_output_low( &platform_gpio_pins[WICED_LED1] );
#endif

#ifndef WICED_DISABLE_STDIO
    /* Initialise UART standard I/O */
    platform_stdio_init( &platform_uart_drivers[STDIO_UART], &platform_uart_peripherals[STDIO_UART], &stdio_config );
#endif
}


uint32_t  platform_get_button_press_time ( int button_index, int led_index, uint32_t max_time )
{
    int             button_gpio;
    uint32_t        button_press_timer = 0;
    int             led_state = 0;

    /* Initialize input */
     platform_gpio_init( &platform_gpio_pins[ PLATFORM_FACTORY_RESET_BUTTON_INDEX ], INPUT_PULL_UP );

    while ( (PLATFORM_BUTTON_PRESSED_STATE == platform_gpio_input_get(&platform_gpio_pins[ button_gpio ])) )
    {
        /* wait a bit */
        host_rtos_delay_milliseconds( PLATFORM_BUTTON_PRESS_CHECK_PERIOD );

        /* Toggle LED */
        platform_led_set_state(led_index, (led_state == 0) ? WICED_LED_OFF : WICED_LED_ON);
        led_state ^= 0x01;

        /* keep track of time */
        button_press_timer += PLATFORM_BUTTON_PRESS_CHECK_PERIOD;
        if ((max_time > 0) && (button_press_timer >= max_time))
        {
            break;
        }
    }

     /* turn off the LED */
    platform_led_set_state(led_index, WICED_LED_OFF );

    return button_press_timer;
}

uint32_t  platform_get_factory_reset_button_time ( uint32_t max_time )
{
    return platform_get_button_press_time ( PLATFORM_FACTORY_RESET_BUTTON_INDEX, PLATFORM_RED_LED_INDEX, max_time );
}
