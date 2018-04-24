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
#include "mbed_assert.h"
#include "analogin_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "xmc_vadc.h"
#include "xmc_scu.h"

/****************************************************************** Defines **/
#define GROUP0                      0
#define GROUP1                      1
#define GROUP2                      2
#define GROUP3                      3

#define RES_REG_NUMBER              0
#define RANGE_12BIT                 0xFFF

/******************************************************************* Macros **/
#define GET_GROUP(x)                ((x & 0x000000F0) >> 4)
#define GET_CHANNEL(x)              (x & 0x0000000F)

/**************************************************************** Functions **/

/** Initialize the analogin peripheral
 *
 * Configures the pin used by analogin.
 * @param obj The analogin object to initialize
 * @param pin The analogin pin name
 */
void analogin_init(analogin_t *obj, PinName pin){

    XMC_VADC_GLOBAL_CONFIG_t g_global_config = { 0 };
    XMC_VADC_GROUP_CONFIG_t g_group_config = { 0 };
    XMC_VADC_QUEUE_CONFIG_t g_queue_config = { 0 };
    XMC_VADC_CHANNEL_CONFIG_t  g_channel_config = { 0 };
    XMC_VADC_RESULT_CONFIG_t g_result_config = { 0 };
    XMC_VADC_QUEUE_ENTRY_t g_queue_entry = { 0 };

    VADC_G_TypeDef *group_ptr = NULL;
    uint32_t ch_nbr = 0;
    uint32_t group_nbr, adc_pin;

    /* Check Pin */
    adc_pin = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(adc_pin != (ADCName)NC);

    /* Get ADC Group Pointer */
    group_nbr = GET_GROUP(adc_pin);
    switch(group_nbr){
    case GROUP0:
        group_ptr = VADC_G0;
        break;
    case GROUP1:
        group_ptr = VADC_G1;
        break;
    case GROUP2:
        group_ptr = VADC_G2;
        break;
    case GROUP3:
        group_ptr = VADC_G3;
        break;
    }

    /* Save group ptr to object */
    obj->group_ptr = group_ptr;

    /* Initialization data of VADC Global resources */
    g_global_config.clock_config.analog_clock_divider = 3;
    g_global_config.clock_config.msb_conversion_clock = 0;
    g_global_config.clock_config.arbiter_clock_divider = 1;

    /* Initialization data of a VADC group */
    g_group_config.class1.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT;
    g_group_config.class1.sample_time_std_conv = 3U;

    /* Channel configuration data */
    g_channel_config.input_class = XMC_VADC_CHANNEL_CONV_GROUP_CLASS1;
    g_channel_config.result_reg_number = RES_REG_NUMBER;
    g_channel_config.alias_channel = XMC_VADC_CHANNEL_ALIAS_DISABLED;

    /* Result configuration data */
    g_result_config.wait_for_read_mode = true;

    /* Get ADC channel number */
    ch_nbr = GET_CHANNEL(adc_pin);

    /* Queue Entry */
    g_queue_entry.channel_num = ch_nbr;
    g_queue_entry.refill_needed = true;
    g_queue_entry.generate_interrupt = false;
    g_queue_entry.external_trigger = false;

    /* Initialize the VADC global registers */
    XMC_VADC_GLOBAL_Init(VADC, &g_global_config);

    /* Configure a conversion kernel */
    XMC_VADC_GROUP_Init(group_ptr, &g_group_config);

    XMC_VADC_GROUP_QueueInit(group_ptr, &g_queue_config);

    /* Configure a channel belonging to the aforesaid conversion kernel */
    XMC_VADC_GROUP_ChannelInit(group_ptr, ch_nbr, &g_channel_config);

    /* Configure a result resource belonging to the aforesaid conversion kernel */
    XMC_VADC_GROUP_ResultInit(group_ptr, RES_REG_NUMBER, &g_result_config);

    /* Add the channel to the queue */
    XMC_VADC_GROUP_QueueInsertChannel(group_ptr, g_queue_entry);

    /* Enable the analog converters */
    XMC_VADC_GROUP_SetPowerMode(group_ptr, XMC_VADC_GROUP_POWERMODE_NORMAL);

    /* Perform calibration of the converter */
    XMC_VADC_GLOBAL_StartupCalibration(VADC);
}

/** Read the value from analogin pin, represented as an unsigned 16bit value
 *
 * @param obj The analogin object
 * @return An unsigned 16bit value representing the current input voltage
 */
uint16_t analogin_read_u16(analogin_t *obj){

    VADC_G_TypeDef *group_ptr = (VADC_G_TypeDef *)obj->group_ptr;

    /* Trigger conversion */
    XMC_VADC_GROUP_QueueTriggerConversion(group_ptr);

    /* wait until conversation complete */
    while(XMC_VADC_GROUP_IsConverterBusy(group_ptr) != XMC_VADC_GROUP_STATE_IDLE);

    /* return result */
    return XMC_VADC_GROUP_GetResult(group_ptr, RES_REG_NUMBER);
}

/** Read the input voltage, represented as a float in the range [0.0, 1.0]
 *
 * @param obj The analogin object
 * @return A floating value representing the current input voltage
 */
float analogin_read(analogin_t *obj){

    uint16_t value = analogin_read_u16(obj);
    return (float)value * (1.0f / (float)RANGE_12BIT);
}

/*EOF*/
