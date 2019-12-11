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

/** @file
 *  Allows thread safe access to the WHD WiFi Driver (WHD) hardware bus
 *
 *  This file provides functions which allow multiple threads to use the WHD hardware bus (SDIO or SPI)
 *  This is achieved by having a single thread (the "WHD Thread") which queues messages to be sent, sending
 *  them sequentially, as well as receiving messages as they arrive.
 *
 *  Messages to be sent come from the @ref whd_sdpcm_send_common function in whd_sdpcm.c .  The messages already
 *  contain SDPCM headers, but not any bus headers (GSPI), and are passed to the whd_thread_send_data function.
 *  This function can be called from any thread.
 *
 *  Messages are received by way of a callback supplied by in SDPCM.c - whd_sdpcm_process_rx_packet
 *  Received messages are delivered in the context of the WHD Thread, so the callback function needs to avoid blocking.
 *
 *  It is also possible to use these functions without any operating system, by periodically calling the whd_thread_send_one_packet,
 *  @ref whd_thread_receive_one_packet or @ref whd_thread_poll_all functions
 *
 */
#include "stdlib.h"
#include "whd_debug.h"
#include "whd_thread.h"
#include "bus_protocols/whd_bus_protocol_interface.h"
#include "cyabs_rtos.h"
#include "whd_int.h"
#include "whd_chip.h"
#include "whd_poll.h"
#include "whd_sdpcm.h"
#include "whd_buffer_api.h"

/******************************************************
*             Static Function Prototypes
******************************************************/
static void whd_thread_func(whd_thread_arg_t thread_input);

/******************************************************
*             Global Functions
******************************************************/
void whd_thread_info_init(whd_driver_t whd_driver, whd_init_config_t *whd_init_config)
{
    memset(&whd_driver->thread_info, 0, sizeof(whd_driver->thread_info) );
    whd_driver->thread_info.thread_stack_start = whd_init_config->thread_stack_start;
    whd_driver->thread_info.thread_stack_size = whd_init_config->thread_stack_size;
    whd_driver->thread_info.thread_priority = (cy_thread_priority_t)whd_init_config->thread_priority;
}

/** Initialises the WHD Thread
 *
 * Initialises the WHD thread, and its flags/semaphores,
 * then starts it running
 *
 * @return    WHD_SUCCESS : if initialisation succeeds
 *            otherwise, a result code
 */
whd_result_t whd_thread_init(whd_driver_t whd_driver)
{
    whd_result_t retval;

    retval = whd_sdpcm_init(whd_driver);

    if (retval != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Could not initialize SDPCM codec\n") );
        /* Lint: Reachable after hitting assert & globals not defined due to error */
        return retval;
    }

    /* Create the event flag which signals the WHD thread needs to wake up */
    retval = cy_rtos_init_semaphore(&whd_driver->thread_info.transceive_semaphore, 1, 0);
    if (retval != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Could not initialize WHD thread semaphore\n") );
        /* Lint: Reachable after hitting assert & globals not defined due to error */
        return retval;
    }

    retval = cy_rtos_create_thread(&whd_driver->thread_info.whd_thread, (cy_thread_entry_fn_t)whd_thread_func,
                                   "WHD", whd_driver->thread_info.thread_stack_start,
                                   whd_driver->thread_info.thread_stack_size,
                                   whd_driver->thread_info.thread_priority, (cy_thread_arg_t)whd_driver);
    if (retval != WHD_SUCCESS)
    {
        /* Could not start WHD main thread */
        WPRINT_WHD_ERROR( ("Could not start WHD thread\n") );
        return retval;
    }

    /* Ready now. Indicate it here and in thread, whatever be executed first. */
    whd_driver->thread_info.whd_inited = WHD_TRUE;

    return WHD_SUCCESS;
}

/** Sends the first queued packet
 *
 * Checks the queue to determine if there is any packets waiting
 * to be sent. If there are, then it sends the first one.
 *
 * This function is normally used by the WHD Thread, but can be
 * called periodically by systems which have no RTOS to ensure
 * packets get sent.
 *
 * @return    1 : packet was sent
 *            0 : no packet sent
 */
int8_t whd_thread_send_one_packet(whd_driver_t whd_driver)
{
    whd_buffer_t tmp_buf_hnd = NULL;
    whd_result_t result;

    if (whd_sdpcm_get_packet_to_send(whd_driver, &tmp_buf_hnd) != WHD_SUCCESS)
    {
        /* Failed to get a packet */
        return 0;
    }

    /* Ensure the wlan backplane bus is up */
    result = whd_ensure_wlan_bus_is_up(whd_driver);
    if (result != WHD_SUCCESS)
    {
        whd_assert("Could not bring bus back up", 0 != 0);
        CHECK_RETURN(whd_buffer_release(whd_driver, tmp_buf_hnd, WHD_NETWORK_TX) );
        return 0;
    }

    WPRINT_WHD_DATA_LOG( ("Wcd:> Sending pkt 0x%08lX\n", (unsigned long)tmp_buf_hnd) );
    if (whd_bus_send_buffer(whd_driver, tmp_buf_hnd) != WHD_SUCCESS)
    {
        WHD_STATS_INCREMENT_VARIABLE(whd_driver, tx_fail);
        return 0;
    }

    WHD_STATS_INCREMENT_VARIABLE(whd_driver, tx_total);
    return (int8_t)1;
}

/** Receives a packet if one is waiting
 *
 * Checks the wifi chip fifo to determine if there is any packets waiting
 * to be received. If there are, then it receives the first one, and calls
 * the callback @ref whd_sdpcm_process_rx_packet (in whd_sdpcm.c).
 *
 * This function is normally used by the WHD Thread, but can be
 * called periodically by systems which have no RTOS to ensure
 * packets get received properly.
 *
 * @return    1 : packet was received
 *            0 : no packet waiting
 */
int8_t whd_thread_receive_one_packet(whd_driver_t whd_driver)
{
    /* Check if there is a packet ready to be received */
    whd_buffer_t recv_buffer;
    if (whd_bus_read_frame(whd_driver, &recv_buffer) != WHD_SUCCESS)
    {
        /* Failed to read a packet */
        return 0;
    }

    if (recv_buffer != NULL)    /* Could be null if it was only a credit update */
    {

        WPRINT_WHD_DATA_LOG( ("Wcd:< Rcvd pkt 0x%08lX\n", (unsigned long)recv_buffer) );
        WHD_STATS_INCREMENT_VARIABLE(whd_driver, rx_total);

        /* Send received buffer up to SDPCM layer */
        whd_sdpcm_process_rx_packet(whd_driver, recv_buffer);
    }
    return (int8_t)1;
}

/** Sends and Receives all waiting packets
 *
 * Calls whd_thread_send_one_packet and whd_thread_receive_one_packet
 * once to send and receive packets, until there are no more packets waiting to
 * be transferred.
 *
 * This function is normally used by the WHD Thread, but can be
 * called periodically by systems which have no RTOS to ensure
 * packets get send and received properly.
 *
 * Note: do not loop in here, to avoid overwriting previously rx-ed packets
 */
int8_t whd_thread_poll_all(whd_driver_t whd_driver)
{
    int8_t result = 0;
    result |= whd_thread_send_one_packet(whd_driver);
    result |= whd_thread_receive_one_packet(whd_driver);
    return result;
}

/** Terminates the WHD Thread
 *
 * Sets a flag then wakes the WHD Thread to force it to terminate.
 *
 */
void whd_thread_quit(whd_driver_t whd_driver)
{
    whd_thread_info_t *thread_info = &whd_driver->thread_info;
    whd_result_t result;

    /* signal main thread and wake it */
    thread_info->thread_quit_flag = WHD_TRUE;
    result = cy_rtos_set_semaphore(&thread_info->transceive_semaphore, WHD_FALSE);
    if (result == WHD_SUCCESS)
    {
        /* Wait for the WHD thread to end */
        cy_rtos_join_thread(&thread_info->whd_thread);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );
    }

    /* Delete the semaphore */
    /* Ignore return - not much can be done about failure */
    (void)cy_rtos_deinit_semaphore(&thread_info->transceive_semaphore);
}

/**
 * Informs WHD of an interrupt
 *
 * This function should be called from the SDIO/SPI interrupt function
 * and usually indicates newly received data is available.
 * It wakes the WHD Thread, forcing it to check the send/receive
 *
 */
/* ignore failure since there is nothing that can be done about it in a ISR */
void whd_thread_notify_irq(whd_driver_t whd_driver)
{
    whd_driver->thread_info.bus_interrupt = WHD_TRUE;

    /* just wake up the main thread and let it deal with the data */
    if (whd_driver->thread_info.whd_inited == WHD_TRUE)
    {
        (void)cy_rtos_set_semaphore(&whd_driver->thread_info.transceive_semaphore, WHD_TRUE);
    }
}

void whd_thread_notify(whd_driver_t whd_driver)
{
    /* just wake up the main thread and let it deal with the data */
    if (whd_driver->thread_info.whd_inited == WHD_TRUE)
    {
        /* Ignore return - not much can be done about failure */
        (void)cy_rtos_set_semaphore(&whd_driver->thread_info.transceive_semaphore, WHD_FALSE);
    }
}

/******************************************************
*             Static Functions
******************************************************/

/** The WHD Thread function
 *
 *  This is the main loop of the WHD Thread.
 *  It simply calls @ref whd_thread_poll_all to send/receive all waiting packets, then goes
 *  to sleep.  The sleep has a 100ms timeout, causing the send/receive queues to be
 *  checked 10 times per second in case an interrupt is missed.
 *  Once the quit flag has been set, flags/mutexes are cleaned up, and the function exits.
 *
 * @param thread_input  : unused parameter needed to match thread prototype.
 *
 */
static void whd_thread_func(whd_thread_arg_t thread_input)
{
    int8_t rx_status;
    int8_t tx_status;
    uint8_t rx_cnt;

    whd_driver_t whd_driver = ( whd_driver_t )thread_input;
    whd_thread_info_t *thread_info = &whd_driver->thread_info;

    WPRINT_WHD_DATA_LOG( ("Started whd Thread\n") );

    /* Interrupts may be enabled inside thread. To make sure none lost set flag now. */
    thread_info->whd_inited = WHD_TRUE;

    while (thread_info->thread_quit_flag != WHD_TRUE)
    {
        rx_cnt = 0;
        /* Check if we were woken by interrupt */
        if ( (thread_info->bus_interrupt == WHD_TRUE) ||
             (whd_bus_use_status_report_scheme(whd_driver) ) )
        {
            thread_info->bus_interrupt = WHD_FALSE;

            /* Check if the interrupt indicated there is a packet to read */
            if (whd_bus_packet_available_to_read(whd_driver) != 0)
            {
                /* Receive all available packets */
                do
                {
                    rx_status = whd_thread_receive_one_packet(whd_driver);
                    rx_cnt++;
                } while (rx_status != 0 && rx_cnt < WHD_THREAD_RX_BOUND);
            }
        }

        /* Send all queued packets */
        do
        {
            tx_status = whd_thread_send_one_packet(whd_driver);
        } while (tx_status != 0);

        if (rx_cnt >= WHD_THREAD_RX_BOUND)
        {
            thread_info->bus_interrupt = WHD_TRUE;
            continue;
        }

        /* Sleep till WLAN do something */
        whd_bus_wait_for_wlan_event(whd_driver, &thread_info->transceive_semaphore);
        WPRINT_WHD_DATA_LOG( ("whd Thread: Woke\n") );
    }

    /* Set flag before releasing objects */
    thread_info->whd_inited = WHD_FALSE;

    /* Reset the quit flag */
    thread_info->thread_quit_flag = WHD_FALSE;

    whd_sdpcm_quit(whd_driver);

    WPRINT_WHD_DATA_LOG( ("Stopped whd Thread\n") );

    /* Ignore return - not much can be done about failure */
    (void)cy_rtos_exit_thread();
}

