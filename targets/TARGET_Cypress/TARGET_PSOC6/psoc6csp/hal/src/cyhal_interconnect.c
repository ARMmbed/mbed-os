/***************************************************************************//**
* \file cyhal_interconnect.c
*
* \brief
* Provides a high level interface for interacting with the internal digital 
* routing on the chip. This is a wrapper around the lower level PDL API.
* 
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
*******************************************************************************/

#include "cyhal_interconnect.h"
#include "cyhal_hwmgr.h"
#include "cyhal_gpio_impl.h"

#ifdef CY_IP_MXPERI

/** Trigger type */
typedef enum 
{
    CY_TRIGGER_LEVEL, //!< Level triggered
    CY_TRIGGER_EDGE,  //!< Edge triggered
    CY_TRIGGER_EITHER,//!< Level or edge triggered
} cyhal_trigger_type_t;

/** Number of muxes */
#define MUX_COUNT
/** Indicates that a mux output does not continue to another mux */
#define NOT_CONTINUATION 0xFF
/** Mux identiifer the one-to-one triggers */
#define ONE_TO_ONE_IDENT 0x80
/** Determines whether a mux is one-to-one */
#define IS_1TO1(muxId) (ONE_TO_ONE_IDENT == (muxId & ONE_TO_ONE_IDENT))
#define WRITE_REGISTER(muxIdx, sourceId, destId) /* TODO */

/* Maps each cyhal_destination_t to a mux index                    ~100b */
extern uint8_t* cyhal_dest_to_mux;
/* Maps each cyhal_destination_t to a specific output in its mux    ~100b */
extern uint8_t* cyhal_mux_dest_index;
/* Number of sources for each mux                                ~30b */
extern uint8_t* cyhal_source_count_per_mux;
/* Mux index to a table of cyhal_source_t                             ~2400b (2b * 15muxes * 80sources_per_mux) (could be 1/2 the size if there are less than 255 sources) */
extern cyhal_source_t** cyhal_mux_to_sources;
/* Mapping from cyhal_source_t to cyhal_destination_t for intra mux connections     ~80b*/
extern cyhal_dest_t* cyhal_intra_trigger_source;
/* Trigger type for each input                                    ~400b */
cyhal_trigger_type_t cyhal_trigger_type_source;
/* Trigger type for each output                                    ~100b */
cyhal_trigger_type_t cyhal_trigger_type_dest;

bool cyhal_has_connection(uint8_t mux, uint8_t outputIdx)
{
    // TODO
    return false;
}

cy_rslt_t cyhal_connect_trigger(cyhal_source_t source, cyhal_dest_t dest)
{
    uint8_t muxIdx = cyhal_dest_to_mux[dest];
    uint8_t destId = dest - cyhal_mux_dest_index[dest];
    uint8_t sourceCount = cyhal_source_count_per_mux[muxIdx];
    
    if (cyhal_has_connection(muxIdx, destId))
    {
        return CYHAL_CONNECT_RSLT_ALREADY_CONNECTED;
    }
    
    for (uint8_t sourceId = 0; sourceId < sourceCount; sourceId++)
    {
        cyhal_source_t foundSource = cyhal_mux_to_sources[muxIdx][sourceId];
        if (foundSource == source)
        {
            if (IS_1TO1(muxIdx) && sourceId != destId)
            {
                return CYHAL_CONNECT_RSLT_INVALID_1TO1_CONNECTION;
            }
            
            WRITE_REGISTER (muxIdx, sourceId, destId);
            return CY_RSLT_SUCCESS;
        }
        else
        {
            cyhal_dest_t intraDest = cyhal_intra_trigger_source[foundSource];
            if (NOT_CONTINUATION != intraDest)
            {
                cy_rslt_t result = cyhal_connect_trigger(source, intraDest);
                if (result == CY_RSLT_SUCCESS)
                {
                    WRITE_REGISTER (muxIdx, sourceId, destId);
                    return result;
                }
            }
        }
    }
    return CYHAL_CONNECT_RSLT_NO_CONNECTION;
}

cy_rslt_t cyhal_connect_pin(const cyhal_resource_pin_mapping_t *pin_connection)
{
    cyhal_gpio_t pin = pin_connection->pin;
    GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(pin));
    en_hsiom_sel_t hsiom = CY_GPIO_CFG_GET_HSIOM(pin_connection->cfg);
    uint8_t mode = CY_GPIO_CFG_GET_MODE(pin_connection->cfg);

    Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(pin), mode, 1, hsiom);
    // Force output to enable pulls.
    switch (mode) {
        case CY_GPIO_DM_PULLUP:
            Cy_GPIO_Write(port, CYHAL_GET_PIN(pin), 1);
            break;
        case CY_GPIO_DM_PULLDOWN:
            Cy_GPIO_Write(port, CYHAL_GET_PIN(pin), 0);
            break;
        default:
            /* do nothing */
            break;
    }    
    
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_disconnect_pin(cyhal_gpio_t pin)
{    
    GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(CYHAL_GET_PORT(pin));
    Cy_GPIO_Pin_FastInit(port, CYHAL_GET_PIN(pin), CY_GPIO_DM_HIGHZ, 1, HSIOM_SEL_GPIO);
    return CY_RSLT_SUCCESS;
}

#endif /* CY_IP_MXPERI */
