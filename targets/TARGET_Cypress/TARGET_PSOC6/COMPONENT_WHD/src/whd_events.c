/*
 * Copyright 2019 Cypress Semiconductor Corporation
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
 */

#include "whd_int.h"
#include "whd_cdc_bdc.h"
#include "whd_events_int.h"
#include "cyabs_rtos.h"
#include "whd_network_types.h"
#include "whd_types_int.h"
#include "whd_wlioctl.h"
#include "whd_thread_internal.h"
#include "whd_buffer_api.h"

/******************************************************
*        Constants
******************************************************/
/******************************************************
*             Macros
******************************************************/
/* bit map related macros */
#define  NBBY  8  /* 8 bits per byte */
#define  setbit(a, i)   ( ( (uint8_t *)a )[(int)(i) / (int)(NBBY)] |= (uint8_t)(1 << ( (i) % NBBY ) ) )
#define  clrbit(a, i)   ( ( (uint8_t *)a )[(int)(i) / (int)(NBBY)] &= (uint8_t) ~(1 << ( (i) % NBBY ) ) )
#define  isset(a, i)    ( ( (const uint8_t *)a )[(int)(i) / (int)(NBBY)]& (1 << ( (i) % NBBY ) ) )
#define  isclr(a, i)    ( ( ( (const uint8_t *)a )[(int)(i) / (int)(NBBY)]& (1 << ( (i) % NBBY ) ) ) == 0 )

/******************************************************
*             Local Structures
******************************************************/

/******************************************************
*             Static Variables
******************************************************/

/******************************************************
*             Static Function Prototypes
******************************************************/
/* helper function for event messages ext API */
static uint8_t *whd_management_alloc_event_msgs_buffer(whd_interface_t ifp, whd_buffer_t *buffer);
static uint8_t whd_find_number_of_events(const whd_event_num_t *event_nums);

/******************************************************
*             Static Functions
******************************************************/

static uint8_t whd_find_number_of_events(const whd_event_num_t *event_nums)
{
    uint8_t count = 0;

    while (*event_nums != WLC_E_NONE)
    {
        count++;
        event_nums++;

        if (count >= WHD_MAX_EVENT_SUBSCRIPTION)
            return 0;
    }
    return count + 1;
}

/**
 * Registers locally a handler to receive event callbacks.
 * Does not notify Wi-Fi about event subscription change.
 * Can be used to refresh local callbacks (e.g. after deep-sleep)
 * if Wi-Fi is already notified about them.
 *
 * This function registers a callback handler to be notified when
 * a particular event is received.
 *
 * @note : Currently there is a limit to the number of simultaneously
 *         registered events
 *
 * @param ifp                 Pointer to handle instance of whd interface
 * @param event_nums          An array of event types that is to trigger the handler.
 *                            The array must be terminated with a WLC_E_NONE event
 *                            See @ref whd_event_num_t for available events
 * @param handler_func        A function pointer to the new handler callback,
 *                            or NULL if callbacks are to be disabled for the given event type
 * @param handler_user_data   A pointer value which will be passed to the event handler function
 *                            at the time an event is triggered (NULL is allowed)
 * @param[out] *event_index   entry where the event handler is registered in the list
 *
 * @return WHD result code
 */
whd_result_t whd_management_set_event_handler_locally(whd_interface_t ifp, const whd_event_num_t *event_nums,
                                                      whd_event_handler_t handler_func,
                                                      void *handler_user_data, uint16_t *event_index)
{
    uint16_t entry = (uint16_t)0xFF;
    uint16_t i;
    whd_driver_t whd_driver = ifp->whd_driver;
    whd_cdc_bdc_info_t *cdc_bdc_info = &whd_driver->cdc_bdc_info;
    uint8_t num_of_events;
    num_of_events = whd_find_number_of_events(event_nums);

    if (num_of_events <= 1)
    {
        WPRINT_WHD_ERROR( ("Exceeded the maximum event subscription/no event subscribed\n") );
        return WHD_UNFINISHED;
    }

    /* Find an existing matching entry OR the next empty entry */
    for (i = 0; i < (uint16_t)WHD_EVENT_HANDLER_LIST_SIZE; i++)
    {
        /* Find a matching event list OR the first empty event entry */
        if (!(memcmp(cdc_bdc_info->whd_event_list[i].events, event_nums,
                     num_of_events * (sizeof(whd_event_num_t) ) ) ) )
        {
            /* Check if all the data already matches */
            if ( (cdc_bdc_info->whd_event_list[i].handler           == handler_func) &&
                 (cdc_bdc_info->whd_event_list[i].handler_user_data == handler_user_data) &&
                 (cdc_bdc_info->whd_event_list[i].ifidx == ifp->ifidx) )
            {
                /* send back the entry where the handler is added */
                *event_index = i;
                return WHD_SUCCESS;
            }
        }
        else if ( (entry == (uint16_t)0xFF) && (cdc_bdc_info->whd_event_list[i].event_set == WHD_FALSE) )
        {
            entry = i;
        }
    }

    /* Check if handler function was provided */
    if (handler_func != NULL)
    {
        /* Check if an empty entry was not found */
        if (entry == (uint16_t)0xFF)
        {
            WPRINT_WHD_DEBUG( ("Out of space in event handlers table - try increasing WHD_EVENT_HANDLER_LIST_SIZE\n") );
            return WHD_OUT_OF_EVENT_HANDLER_SPACE;
        }

        /* Add the new handler in at the free space */
        memcpy (cdc_bdc_info->whd_event_list[entry].events, event_nums, num_of_events * (sizeof(whd_event_num_t) ) );
        cdc_bdc_info->whd_event_list[entry].handler           = handler_func;
        cdc_bdc_info->whd_event_list[entry].handler_user_data = handler_user_data;
        cdc_bdc_info->whd_event_list[entry].ifidx             = ifp->ifidx;
        cdc_bdc_info->whd_event_list[entry].event_set         = WHD_TRUE;

        /* send back the entry where the handler is added */
        *event_index = entry;
    }
    else
    {
        WPRINT_WHD_ERROR( ("Event handler callback function is NULL/not provided to register\n") );
        return WHD_BADARG;
    }

    return WHD_SUCCESS;
}

/* allocates memory for the needed iovar and returns a pointer to the event mask */
static uint8_t *whd_management_alloc_event_msgs_buffer(whd_interface_t ifp, whd_buffer_t *buffer)
{
    uint16_t i;
    uint16_t j;
    whd_bool_t use_extended_evt       = WHD_FALSE;
    uint32_t max_event                  = 0;
    eventmsgs_ext_t *eventmsgs_ext_data = NULL;
    uint32_t *data                      = NULL;
    whd_driver_t whd_driver = ifp->whd_driver;
    whd_cdc_bdc_info_t *cdc_bdc_info = &whd_driver->cdc_bdc_info;

    /* Check to see if event that's set requires more than 128 bit */
    for (i = 0; i < (uint16_t)WHD_EVENT_HANDLER_LIST_SIZE; i++)
    {
        if (cdc_bdc_info->whd_event_list[i].event_set)
        {
            for (j = 0; cdc_bdc_info->whd_event_list[i].events[j] != WLC_E_NONE; j++)
            {
                uint32_t event_value = cdc_bdc_info->whd_event_list[i].events[j];
                if (event_value > 127)
                {
                    use_extended_evt = WHD_TRUE;
                    if (event_value > max_event)
                    {
                        max_event = event_value;
                    }
                    /* keep going to get highest value */
                }
            }
        }
    }

    if (WHD_FALSE == use_extended_evt)
    {
        /* use old iovar for backwards compat */
        data = (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, buffer, (uint16_t)WL_EVENTING_MASK_LEN + 4,
                                                    "bsscfg:" IOVAR_STR_EVENT_MSGS);

        if (NULL == data)
        {
            return NULL;
        }

        data[0] = ifp->bsscfgidx;

        return (uint8_t *)&data[1];
    }
    else
    {
        uint8_t mask_len   = (uint8_t)( (max_event + 8) / 8 );
        data =
            (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, buffer,
                                                 (uint16_t)(sizeof(eventmsgs_ext_t) + mask_len + 4),
                                                 "bsscfg:" IOVAR_STR_EVENT_MSGS_EXT);

        if (NULL == data)
        {
            return NULL;
        }

        data[0] = ifp->bsscfgidx;

        eventmsgs_ext_data = (eventmsgs_ext_t *)&data[1];

        memset(eventmsgs_ext_data, 0, sizeof(*eventmsgs_ext_data) );
        eventmsgs_ext_data->ver     = EVENTMSGS_VER;
        eventmsgs_ext_data->command = EVENTMSGS_SET_MASK;
        eventmsgs_ext_data->len     = mask_len;
        return eventmsgs_ext_data->mask;
    }
}

/**
 * Registers a handler to receive event callbacks.
 * Subscribe locally and notify Wi-Fi about subscription.
 *
 * This function registers a callback handler to be notified when
 * a particular event is received.
 *
 * @note : Currently there is a limit to the number of simultaneously
 *         registered events
 *
 * @param ifp                 Pointer to handle instance of whd interface
 * @param event_nums          An array of event types that is to trigger the handler.
 *                            The array must be terminated with a WLC_E_NONE event
 *                            See @ref whd_event_num_t for available events
 * @param handler_func        A function pointer to the new handler callback,
 *                            or NULL if callbacks are to be disabled for the given event type
 * @param handler_user_data   A pointer value which will be passed to the event handler function
 *                            at the time an event is triggered (NULL is allowed)
 * @param[out] *event_index   entry where the event handler is registered in the list
 *
 * @return WHD result code
 */
whd_result_t whd_management_set_event_handler(whd_interface_t ifp, const whd_event_num_t *event_nums,
                                              whd_event_handler_t handler_func,
                                              void *handler_user_data, uint16_t *event_index)
{
    whd_buffer_t buffer;
    uint8_t *event_mask;
    uint16_t i;
    uint16_t j;
    whd_result_t res;
    whd_driver_t whd_driver;
    whd_cdc_bdc_info_t *cdc_bdc_info;
    whd_interface_t prim_ifp;

    if (ifp == NULL)
    {
        return WHD_UNKNOWN_INTERFACE;
    }

    if (!event_nums || !event_index)
    {
        WPRINT_WHD_ERROR( ("Event list to be registered is NULL/Event index is NULL") );
        return WHD_BADARG;
    }

    whd_driver = ifp->whd_driver;
    cdc_bdc_info = &whd_driver->cdc_bdc_info;
    prim_ifp = whd_get_primary_interface(whd_driver);

    if (prim_ifp == NULL)
    {
        return WHD_UNKNOWN_INTERFACE;
    }

    /* Acquire mutex preventing multiple threads accessing the handler at the same time */
    res = cy_rtos_get_semaphore(&cdc_bdc_info->event_list_mutex, CY_RTOS_NEVER_TIMEOUT, WHD_FALSE);
    if (res != WHD_SUCCESS)
    {
        return res;
    }

    /* Set event handler locally  */
    res = whd_management_set_event_handler_locally(ifp, event_nums, handler_func, handler_user_data, event_index);
    if (res != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error in setting event handler locally, %s failed at %d \n", __func__, __LINE__) );
        goto set_event_handler_exit;
    }

    /* Send the new event mask value to the wifi chip */
    event_mask = whd_management_alloc_event_msgs_buffer(ifp, &buffer);

    if (NULL == event_mask)
    {
        res = WHD_BUFFER_UNAVAILABLE_PERMANENT;
        WPRINT_WHD_ERROR( ("Buffer unavailable permanently, %s failed at %d \n", __func__, __LINE__) );
        goto set_event_handler_exit;
    }

    /* Keep the wlan awake while we set the event_msgs */
    WHD_WLAN_KEEP_AWAKE(whd_driver);

    /* Set the event bits for each event from every handler */
    memset(event_mask, 0, (size_t)WL_EVENTING_MASK_LEN);
    for (i = 0; i < (uint16_t)WHD_EVENT_HANDLER_LIST_SIZE; i++)
    {
        if (cdc_bdc_info->whd_event_list[i].event_set)
        {
            for (j = 0; cdc_bdc_info->whd_event_list[i].events[j] != WLC_E_NONE; j++)
            {
                setbit(event_mask, cdc_bdc_info->whd_event_list[i].events[j]);
            }
        }
    }

    /* set the event_list_mutex from calling thread before sending iovar
     * as the RX thread also waits on this Mutex when an ASYNC Event received
     * causing deadlock
     */
    CHECK_RETURN(cy_rtos_set_semaphore(&cdc_bdc_info->event_list_mutex, WHD_FALSE) );

    CHECK_RETURN(whd_cdc_send_iovar(prim_ifp, CDC_SET, buffer, 0) );

    /* The wlan chip can sleep from now on */
    WHD_WLAN_LET_SLEEP(whd_driver);
    return WHD_SUCCESS;

set_event_handler_exit:
    CHECK_RETURN(cy_rtos_set_semaphore(&cdc_bdc_info->event_list_mutex, WHD_FALSE) );
    return res;
}

whd_result_t whd_wifi_set_event_handler(whd_interface_t ifp, const uint32_t *event_type,
                                        whd_event_handler_t handler_func,
                                        void *handler_user_data, uint16_t *event_index)
{
    whd_buffer_t buffer;
    uint8_t *event_mask;
    uint16_t i;
    uint16_t j;
    whd_result_t res;
    whd_driver_t whd_driver;
    whd_cdc_bdc_info_t *cdc_bdc_info;
    whd_interface_t prim_ifp;

    if (ifp == NULL)
    {
        return WHD_UNKNOWN_INTERFACE;
    }

    if (!event_type || !event_index)
    {
        WPRINT_WHD_ERROR( ("Event list to be registered is NULL/Event index is NULL") );
        return WHD_BADARG;
    }

    whd_driver = ifp->whd_driver;
    cdc_bdc_info = &whd_driver->cdc_bdc_info;
    prim_ifp = whd_get_primary_interface(whd_driver);

    if (prim_ifp == NULL)
    {
        return WHD_UNKNOWN_INTERFACE;
    }

    /* Acquire mutex preventing multiple threads accessing the handler at the same time */
    res = cy_rtos_get_semaphore(&cdc_bdc_info->event_list_mutex, CY_RTOS_NEVER_TIMEOUT, WHD_FALSE);
    if (res != WHD_SUCCESS)
    {
        return res;
    }

    /* Set event handler locally  */
    res = whd_management_set_event_handler_locally(ifp, (whd_event_num_t *)event_type, handler_func, handler_user_data,
                                                   event_index);
    if (res != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error in setting event handler locally, %s failed at %d \n", __func__, __LINE__) );
        goto set_event_handler_exit;
    }

    /* Send the new event mask value to the wifi chip */
    event_mask = whd_management_alloc_event_msgs_buffer(ifp, &buffer);

    if (NULL == event_mask)
    {
        res = WHD_BUFFER_UNAVAILABLE_PERMANENT;
        WPRINT_WHD_ERROR( ("Buffer unavailable permanently, %s failed at %d \n", __func__, __LINE__) );
        goto set_event_handler_exit;
    }

    /* Keep the wlan awake while we set the event_msgs */
    WHD_WLAN_KEEP_AWAKE(whd_driver);

    /* Set the event bits for each event from every handler */
    memset(event_mask, 0, (size_t)WL_EVENTING_MASK_LEN);
    for (i = 0; i < (uint16_t)WHD_EVENT_HANDLER_LIST_SIZE; i++)
    {
        if (cdc_bdc_info->whd_event_list[i].event_set)
        {
            for (j = 0; cdc_bdc_info->whd_event_list[i].events[j] != WLC_E_NONE; j++)
            {
                setbit(event_mask, cdc_bdc_info->whd_event_list[i].events[j]);
            }
        }
    }

    /* set the event_list_mutex from calling thread before sending iovar
     * as the RX thread also waits on this Mutex when an ASYNC Event received
     * causing deadlock
     */
    CHECK_RETURN(cy_rtos_set_semaphore(&cdc_bdc_info->event_list_mutex, WHD_FALSE) );

    CHECK_RETURN(whd_cdc_send_iovar(prim_ifp, CDC_SET, buffer, 0) );

    /* The wlan chip can sleep from now on */
    WHD_WLAN_LET_SLEEP(whd_driver);
    return WHD_SUCCESS;

set_event_handler_exit:
    CHECK_RETURN(cy_rtos_set_semaphore(&cdc_bdc_info->event_list_mutex, WHD_FALSE) );
    return res;
}

uint32_t whd_wifi_deregister_event_handler(whd_interface_t ifp, uint16_t event_index)
{
    whd_driver_t whd_driver;
    whd_cdc_bdc_info_t *cdc_bdc_info;

    if (ifp == NULL)
    {
        return WHD_UNKNOWN_INTERFACE;
    }

    whd_driver = ifp->whd_driver;
    cdc_bdc_info = &whd_driver->cdc_bdc_info;

    if (event_index < WHD_EVENT_HANDLER_LIST_SIZE)
    {
        memset(cdc_bdc_info->whd_event_list[event_index].events, 0xFF,
               (sizeof(cdc_bdc_info->whd_event_list[event_index].events) ) );
        cdc_bdc_info->whd_event_list[event_index].handler = NULL;
        cdc_bdc_info->whd_event_list[event_index].handler_user_data = NULL;
        cdc_bdc_info->whd_event_list[event_index].event_set = WHD_FALSE;
        return WHD_SUCCESS;
    }
    if (event_index == 0xFF)
    {
        WPRINT_WHD_INFO( ("Event handler not registered \n") );
        return WHD_SUCCESS;
    }
    WPRINT_WHD_DEBUG( ("Invalid event index received to deregister the event handler \n") );
    return WHD_BADARG;
}

