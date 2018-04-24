/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

/***************************************************************** Includes **/
#include "pinmap.h"
#include "PeripheralPins.h"
#include "xmc4_helper.h"
#include "xmc_gpio.h"
#include "xmc_eru.h"
#include "gpio_irq_api.h"
#include "cmsis.h"

/****************************************************************** Defines **/
#define NUM_CHANNELS                8
#define NUM_ERU_UNITS               2
#define USED                        1
#define NOT_USED                    0
#define ERU_UNIT0                   0
#define ERU_UNIT1                   1

#define NO_EVENT_TYPE               0
#define IRQ_RISE_POSITION           1
#define IRQ_FALL_POSITION           2

/******************************************************************* Macros **/
#define GET_ERU_UNIT(x)             ((x & 0x000000F0) >> 4)
#define GET_ETL_CH(x)               (x & 0x0000000F)
#define GET_INPUT_SRC(x)            (((x & 0x000000F0) >> 4) - 10)
#define GET_INPUT_NBR(x)            (x & 0x0000000F)

/****************************************************************** Typedef **/
typedef struct eru_params_s {
    int pin;
    int eru_unit;
    int etl_channel;
    int input_nbr;
    int input_source;
    int trigger_channel;
    int event_type;     ///> 0x00 => NONE; 0x01 => RISE; 0x02 => FALL; 0x03 => BOTH
    int channel_nbr;
    int id;
    IRQn_Type irq_n;
    uint32_t vector;
}eru_params_t;

/****************************************************************** Globals **/
///> Holds status of channel 0 => unused; 1 => used
static int channel_used[NUM_CHANNELS] = { NOT_USED };
///> Holds ERU parameters of the specific channel
static eru_params_t channel_params[NUM_CHANNELS];
///> IRQ Handler
static gpio_irq_handler irq_handler;

/************************************************************** ISR Handler **/

/* @internal eru_irq
 *
 * @param channel_id
 *
 */
static void eru_irq(int channel_nbr) {

    eru_params_t *eru_params = &channel_params[channel_nbr];
    gpio_irq_event event = IRQ_NONE;

    /* Get Pin and check if rising or falling */
    int pin = eru_params->pin;
    /* Get PORT Base from pin */
    port_type_t *port = (port_type_t *)(PORT0_BASE + (uint32_t)((pin & 0xF0) << 4));
    if(((port->IN) >> (pin & 0x0F)) & 0x1U){
        /* Rising edge detected */
        event = IRQ_RISE;
    }else{
        /* Falling edge detected */
        event = IRQ_FALL;
    }

    /* Call Interrupt handler */
    if(event != IRQ_NONE) {
        irq_handler(eru_params->id, event);
    }
}

/**
 * ISR Handler ERU1_3 Interrupt
 */
void eru_7_irq(){

    eru_irq(7);
}

/**
 * ISR Handler ERU1_2 Interrupt
 */
void eru_6_irq(){

    eru_irq(6);
}

/**
 * ISR Handler ERU1_1 Interrupt
 */
void eru_5_irq(){

    eru_irq(5);
}

/**
 * ISR Handler ERU1_0 Interrupt
 */
void eru_4_irq(){

    eru_irq(4);
}

/**
 * ISR Handler ERU0_3 Interrupt
 */
void eru_3_irq(){

    eru_irq(3);
}

/**
 * ISR Handler ERU0_2 Interrupt
 */
void eru_2_irq(){

    eru_irq(2);
}

/**
 * ISR Handler ERU0_1 Interrupt
 */
void eru_1_irq(){

    eru_irq(1);
}

/**
 * ISR Handler ERU0_0 Interrupt
 */
void eru_0_irq(){

    eru_irq(0);
}

/************************************************************ API Functions **/

/* @internal Get ERU parameters
 *
 * @param eru_params
 *
 */
static void get_eru_params(eru_params_t *eru_params){

    /* Get PIN functions */
    pin_io_function_t *pin_function = get_pin_function(eru_params->pin);

    /* Fill structure */
    switch(eru_params->eru_unit){
    case ERU_UNIT0:
        eru_params->input_nbr = GET_INPUT_NBR(pin_function->eru0_input);
        eru_params->input_source = GET_INPUT_SRC(pin_function->eru0_input);
        break;
    case ERU_UNIT1:
        eru_params->input_nbr = GET_INPUT_NBR(pin_function->eru1_input);
        eru_params->input_source = GET_INPUT_SRC(pin_function->eru1_input);
        break;
    }

    /* Set interrupt vector and number */
    switch(eru_params->channel_nbr){
    case 0:
        eru_params->irq_n = ERU0_0_IRQn;
        eru_params->vector = (uint32_t)&eru_0_irq;
        break;
    case 1:
        eru_params->irq_n = ERU0_1_IRQn;
        eru_params->vector = (uint32_t)&eru_1_irq;
        break;
    case 2:
        eru_params->irq_n = ERU0_2_IRQn;
        eru_params->vector = (uint32_t)&eru_2_irq;
        break;
    case 3:
        eru_params->irq_n = ERU0_3_IRQn;
        eru_params->vector = (uint32_t)&eru_3_irq;
        break;
    case 4:
        eru_params->irq_n = ERU1_0_IRQn;
        eru_params->vector = (uint32_t)&eru_4_irq;
        break;
    case 5:
        eru_params->irq_n = ERU1_1_IRQn;
        eru_params->vector = (uint32_t)&eru_5_irq;
        break;
    case 6:
        eru_params->irq_n = ERU1_2_IRQn;
        eru_params->vector = (uint32_t)&eru_6_irq;
        break;
    case 7:
        eru_params->irq_n = ERU1_3_IRQn;
        eru_params->vector = (uint32_t)&eru_7_irq;
        break;
    }
}

/** Initialize the GPIO IRQ pin
 *
 * @param obj     The GPIO object to initialize
 * @param pin     The GPIO pin name
 * @param handler The handler to be attached to GPIO IRQ
 * @param id      The object ID (id != 0, 0 is reserved)
 * @return -1 if pin is NC, 0 otherwise
 */
int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id){

    int eru_units[NUM_ERU_UNITS];
    XMC_ERU_ETL_CONFIG_t event_generator_cfg;
    XMC_ERU_OGU_CONFIG_t event_detection_cfg;
    eru_params_t eru_params;
    uint32_t i;
    uint32_t trigger_channel = 0;   ///> trigger channel [0 -4]
    uint32_t eru_unit;              ///> ERU Unit [0 -NUM:ERU_UNITS]
    uint32_t etl_channel;           ///> ETL Channel [0 -4]
    uint32_t ch_start, ch_end;      ///> start & stop point to search free channel

    /* Determine corresponding ERU unit & channel */
    eru_units[0] = (uint32_t)pinmap_find_peripheral(pin, PinMap_ERU_0);
    eru_units[1] = (uint32_t)pinmap_find_peripheral(pin, PinMap_ERU_1);

    /* Check if PIN can be used */
    if((eru_units[0] == NC) && (eru_units[1] == NC)){
        MBED_ASSERT(obj);
        return -1;
    }

    /* Get ERU Unit and ETL channel */
    if(eru_units[ERU_UNIT0] != NC){
        eru_unit = GET_ERU_UNIT(eru_units[ERU_UNIT0]);
        etl_channel = GET_ETL_CH(eru_units[ERU_UNIT0]);
    }else{
        eru_unit = GET_ERU_UNIT(eru_units[ERU_UNIT1]);
        etl_channel = GET_ETL_CH(eru_units[ERU_UNIT1]);
    }
    /* get start & end point */
    ch_start = eru_unit << 2;
    ch_end = ch_start + 4;
    for(i = ch_start; i <= ch_end; i++){
        /* NO Channel for this PIN is free */
        if(i == ch_end){
            MBED_ASSERT(obj);
            return -1;
        }
        if(channel_used[i] == NOT_USED){
            channel_used[i] = USED;
            trigger_channel = i - (4 * eru_unit);
            break;
        }
    }

    /* Set IRQ Handler */
    irq_handler = handler;

    /* Fill structure */
    eru_params.pin = pin;
    eru_params.eru_unit = eru_unit;
    eru_params.etl_channel = etl_channel;
    eru_params.channel_nbr = i;
    eru_params.trigger_channel = trigger_channel;
    eru_params.event_type = NO_EVENT_TYPE;
    eru_params.id = id;

    /* Get ERU parameters for this channel and copy to global structure */
    get_eru_params(&eru_params);
    memcpy(&channel_params[i], &eru_params, sizeof(eru_params_t));
    obj->eru_params = (void *)&channel_params[i];
    obj->id = id;

    /* Fill event generator structure */
    event_generator_cfg.input_a = eru_params.input_nbr;
    event_generator_cfg.input_b = eru_params.input_nbr;
    event_generator_cfg.source = eru_params.input_source;
    event_generator_cfg.edge_detection = XMC_ERU_ETL_EDGE_DETECTION_DISABLED;
    event_generator_cfg.status_flag_mode = XMC_ERU_ETL_STATUS_FLAG_MODE_HWCTRL;
    event_generator_cfg.enable_output_trigger = true;
    event_generator_cfg.output_trigger_channel = eru_params.trigger_channel;

    /* Fill event detection structure */
    event_detection_cfg.service_request = XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER;

    /* Initialize ERU module */
    switch(eru_params.eru_unit){
    case ERU_UNIT0:
        XMC_ERU_ETL_Init(XMC_ERU0, eru_params.etl_channel, &event_generator_cfg);
        XMC_ERU_OGU_Init(XMC_ERU0, eru_params.trigger_channel, &event_detection_cfg);
        break;
    case ERU_UNIT1:
        XMC_ERU_ETL_Init(XMC_ERU1, eru_params.etl_channel, &event_generator_cfg);
        XMC_ERU_OGU_Init(XMC_ERU1, eru_params.trigger_channel, &event_detection_cfg);
        break;
    }

    /* Set PIN mode */
    port_type_t *port = (port_type_t *)(PORT0_BASE + (uint32_t)((pin & 0xF0) << 4));
    XMC_GPIO_SetMode((XMC_GPIO_PORT_t *)port, pin & 0x0F, XMC_GPIO_MODE_INPUT_TRISTATE);

    /* Set NVIC vector and enable interrupt */
    NVIC_SetVector(eru_params.irq_n, eru_params.vector);
    NVIC_EnableIRQ(eru_params.irq_n);

    return 0;
}

/** Release the GPIO IRQ PIN
 *
 * @param obj The gpio object
 */
void gpio_irq_free(gpio_irq_t *obj){

    eru_params_t *eru_params = (eru_params_t *)obj->eru_params;

    channel_used[eru_params->channel_nbr] = NOT_USED;
}

/** Enable/disable pin IRQ event
 *
 * @param obj    The GPIO object
 * @param event  The GPIO IRQ event
 * @param enable The enable flag
 */
void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable){

    eru_params_t *eru_params = (eru_params_t *)obj->eru_params;
    XMC_ERU_ETL_EDGE_DETECTION_t edge_detection;

    if(enable){
        if (event == IRQ_RISE) {
            eru_params->event_type |= IRQ_RISE_POSITION;
        } else if (event == IRQ_FALL) {
            eru_params->event_type |= IRQ_FALL_POSITION;
        }
    }else{
        if (event == IRQ_RISE) {
            eru_params->event_type &= ~IRQ_RISE_POSITION;
        } else if (event == IRQ_FALL) {
            eru_params->event_type &= ~IRQ_FALL_POSITION;
        }
    }

    /* check which events must be enabled */
    if (eru_params->event_type == (IRQ_RISE_POSITION | IRQ_FALL_POSITION)) {
        edge_detection = XMC_ERU_ETL_EDGE_DETECTION_BOTH;
    } else if (eru_params->event_type == IRQ_RISE_POSITION) {
        edge_detection = XMC_ERU_ETL_EDGE_DETECTION_RISING;
    } else if (eru_params->event_type == IRQ_FALL_POSITION) {
        edge_detection = XMC_ERU_ETL_EDGE_DETECTION_FALLING;
    } else {
        edge_detection = XMC_ERU_ETL_EDGE_DETECTION_DISABLED;
    }

    /* Set Edge Detection */
    switch(eru_params->eru_unit){
    case 0:
        XMC_ERU_ETL_SetEdgeDetection(XMC_ERU0, eru_params->etl_channel, edge_detection);
        break;
    case 1:
        XMC_ERU_ETL_SetEdgeDetection(XMC_ERU1, eru_params->etl_channel, edge_detection);
        break;
    }
}

/** Enable GPIO IRQ
 *
 * This is target dependent, as it might enable the entire port or just a pin
 * @param obj The GPIO object
 */
void gpio_irq_enable(gpio_irq_t *obj){

    eru_params_t *eru_params = (eru_params_t *)obj->eru_params;

    /* Set NVIC vector and enable interrupt */
    NVIC_SetVector(eru_params->irq_n, eru_params->vector);
    NVIC_EnableIRQ(eru_params->irq_n);
}

/** Disable GPIO IRQ
 *
 * This is target dependent, as it might disable the entire port or just a pin
 * @param obj The GPIO object
 */
void gpio_irq_disable(gpio_irq_t *obj){

    eru_params_t *eru_params = (eru_params_t *)obj->eru_params;

    /* Disable Interrupt */
    NVIC_DisableIRQ(eru_params->irq_n);
}


/*EOF*/
