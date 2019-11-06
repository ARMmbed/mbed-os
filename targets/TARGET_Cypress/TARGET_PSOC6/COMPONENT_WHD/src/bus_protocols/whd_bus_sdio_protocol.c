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
 *  Broadcom WLAN SDIO Protocol interface
 *
 *  Implements the WHD Bus Protocol Interface for SDIO
 *  Provides functions for initialising, de-intitialising 802.11 device,
 *  sending/receiving raw packets etc
 */

#include <stdlib.h>
#include "cyabs_rtos.h"
#include "cyhal_sdio.h"
#include "cyhal_gpio.h"

#include "whd_bus_sdio_protocol.h"
#include "whd_bus.h"
#include "whd_bus_common.h"
#include "whd_chip_reg.h"
#include "whd_chip_constants.h"
#include "whd_int.h"
#include "whd_chip.h"
#include "whd_sdpcm.h"
#include "whd_debug.h"
#include "whd_sdio.h"
#include "whd_buffer_api.h"
#include "whd_resource_if.h"
#include "whd_types_int.h"
#include "whd_types.h"


/******************************************************
*             Constants
******************************************************/
/* function 1 OCP space */
#define SBSDIO_SB_OFT_ADDR_MASK     0x07FFF     /* sb offset addr is <= 15 bits, 32k */
#define SBSDIO_SB_OFT_ADDR_LIMIT    0x08000
#define SBSDIO_SB_ACCESS_2_4B_FLAG  0x08000     /* with b15, maps to 32-bit SB access */

#define F0_WORKING_TIMEOUT_MS (500)
#define F1_AVAIL_TIMEOUT_MS   (500)
#define F2_AVAIL_TIMEOUT_MS   (500)
#define F2_READY_TIMEOUT_MS   (1000)
#define ALP_AVAIL_TIMEOUT_MS  (100)
#define HT_AVAIL_TIMEOUT_MS   (500)
#define ABORT_TIMEOUT_MS      (100)
/* Taken from FALCON_5_90_195_26 dhd/sys/dhd_sdio.c. */
#define SDIO_F2_WATERMARK     (8)

#define INITIAL_READ   4

#define WHD_THREAD_POLL_TIMEOUT      (CY_RTOS_NEVER_TIMEOUT)

#define WHD_THREAD_POKE_TIMEOUT      (100)

#define HOSTINTMASK                 (I_HMB_SW_MASK)


/******************************************************
*             Structures
******************************************************/
struct whd_bus_priv
{
    whd_sdio_config_t sdio_config;
    whd_bus_stats_t whd_bus_stats;
    cyhal_sdio_t *sdio_obj;

};

/******************************************************
*             Variables
******************************************************/

/******************************************************
*             Static Function Declarations
******************************************************/

static whd_result_t whd_bus_sdio_transfer(whd_driver_t whd_driver, whd_bus_transfer_direction_t direction,
                                          whd_bus_function_t function, uint32_t address, uint16_t data_size,
                                          uint8_t *data, sdio_response_needed_t response_expected);
static whd_result_t whd_bus_sdio_cmd52(whd_driver_t whd_driver, whd_bus_transfer_direction_t direction,
                                       whd_bus_function_t function, uint32_t address, uint8_t value,
                                       sdio_response_needed_t response_expected, uint8_t *response);
static whd_result_t whd_bus_sdio_cmd53(whd_driver_t whd_driver, whd_bus_transfer_direction_t direction,
                                       whd_bus_function_t function, sdio_transfer_mode_t mode, uint32_t address,
                                       uint16_t data_size, uint8_t *data,
                                       sdio_response_needed_t response_expected,
                                       uint32_t *response);
static whd_result_t whd_bus_sdio_abort_read(whd_driver_t whd_driver, whd_bool_t retry);
static whd_result_t whd_bus_sdio_download_firmware(whd_driver_t whd_driver);

static whd_result_t whd_bus_sdio_set_oob_interrupt(whd_driver_t whd_driver, uint8_t gpio_pin_number);

static void whd_bus_sdio_irq_handler(void *handler_arg, cyhal_sdio_irq_event_t event);
static void whd_bus_sdio_oob_irq_handler(void *arg, cyhal_gpio_irq_event_t event);

static whd_result_t whd_bus_sdio_irq_register(whd_driver_t whd_driver);
static whd_result_t whd_bus_sdio_irq_enable(whd_driver_t whd_driver, whd_bool_t enable);
static whd_result_t whd_bus_sdio_init_oob_intr(whd_driver_t whd_driver);
static whd_result_t whd_bus_sdio_deinit_oob_intr(whd_driver_t whd_driver);
static whd_result_t whd_bus_sdio_register_oob_intr(whd_driver_t whd_driver);
static whd_result_t whd_bus_sdio_unregister_oob_intr(whd_driver_t whd_driver);
static whd_result_t whd_bus_sdio_enable_oob_intr(whd_driver_t whd_driver, whd_bool_t enable);

/******************************************************
*             Global Function definitions
******************************************************/

uint32_t whd_bus_sdio_attach(whd_driver_t whd_driver, whd_sdio_config_t *whd_sdio_config, cyhal_sdio_t *sdio_obj)
{
    struct whd_bus_info *whd_bus_info;

    if (!whd_driver || !whd_sdio_config)
    {
        WPRINT_WHD_ERROR( ("Invalid param in func %s at line %d \n",
                           __func__, __LINE__) );
        return WHD_WLAN_BADARG;
    }

    whd_bus_info = (whd_bus_info_t *)malloc(sizeof(whd_bus_info_t) );

    if (whd_bus_info == NULL)
    {
        WPRINT_WHD_ERROR( ("Memory allocation failed for whd_bus_info in %s\n", __FUNCTION__) );
        return WHD_BUFFER_UNAVAILABLE_PERMANENT;
    }
    memset(whd_bus_info, 0, sizeof(whd_bus_info_t) );

    whd_driver->bus_if = whd_bus_info;

    whd_driver->bus_priv = (struct whd_bus_priv *)malloc(sizeof(struct whd_bus_priv) );

    if (whd_driver->bus_priv == NULL)
    {
        WPRINT_WHD_ERROR( ("Memory allocation failed for whd_bus_priv in %s\n", __FUNCTION__) );
        return WHD_BUFFER_UNAVAILABLE_PERMANENT;
    }
    memset(whd_driver->bus_priv, 0, sizeof(struct whd_bus_priv) );

    whd_driver->bus_priv->sdio_obj = sdio_obj;
    whd_driver->bus_priv->sdio_config = *whd_sdio_config;

    whd_bus_info->whd_bus_init_fptr = whd_bus_sdio_init;
    whd_bus_info->whd_bus_deinit_fptr = whd_bus_sdio_deinit;

    whd_bus_info->whd_bus_write_backplane_value_fptr = whd_bus_sdio_write_backplane_value;
    whd_bus_info->whd_bus_read_backplane_value_fptr = whd_bus_sdio_read_backplane_value;
    whd_bus_info->whd_bus_write_register_value_fptr = whd_bus_sdio_write_register_value;
    whd_bus_info->whd_bus_read_register_value_fptr = whd_bus_sdio_read_register_value;

    whd_bus_info->whd_bus_send_buffer_fptr = whd_bus_sdio_send_buffer;
    whd_bus_info->whd_bus_transfer_bytes_fptr = whd_bus_sdio_transfer_bytes;

    whd_bus_info->whd_bus_read_frame_fptr = whd_bus_sdio_read_frame;

    whd_bus_info->whd_bus_packet_available_to_read_fptr = whd_bus_sdio_packet_available_to_read;
    whd_bus_info->whd_bus_poke_wlan_fptr = whd_bus_sdio_poke_wlan;
    whd_bus_info->whd_bus_wait_for_wlan_event_fptr = whd_bus_sdio_wait_for_wlan_event;

    whd_bus_info->whd_bus_ack_interrupt_fptr = whd_bus_sdio_ack_interrupt;
    whd_bus_info->whd_bus_wake_interrupt_present_fptr = whd_bus_sdio_wake_interrupt_present;

    whd_bus_info->whd_bus_wakeup_fptr = whd_bus_sdio_wakeup;
    whd_bus_info->whd_bus_sleep_fptr = whd_bus_sdio_sleep;

    whd_bus_info->whd_bus_backplane_read_padd_size_fptr = whd_bus_sdio_backplane_read_padd_size;
    whd_bus_info->whd_bus_use_status_report_scheme_fptr = whd_bus_sdio_use_status_report_scheme;

    whd_bus_info->whd_bus_get_max_transfer_size_fptr = whd_bus_sdio_get_max_transfer_size;

    whd_bus_info->whd_bus_init_stats_fptr = whd_bus_sdio_init_stats;
    whd_bus_info->whd_bus_print_stats_fptr = whd_bus_sdio_print_stats;
    whd_bus_info->whd_bus_reinit_stats_fptr = whd_bus_sdio_reinit_stats;
    whd_bus_info->whd_bus_irq_register_fptr = whd_bus_sdio_irq_register;
    whd_bus_info->whd_bus_irq_enable_fptr = whd_bus_sdio_irq_enable;

    return WHD_SUCCESS;
}

void whd_bus_sdio_detach(whd_driver_t whd_driver)
{
    if (whd_driver->bus_if != NULL)
    {
        free(whd_driver->bus_if);
        whd_driver->bus_if = NULL;
    }
    if (whd_driver->bus_priv != NULL)
    {
        free(whd_driver->bus_priv);
        whd_driver->bus_priv = NULL;
    }
}

whd_result_t whd_bus_sdio_ack_interrupt(whd_driver_t whd_driver, uint32_t intstatus)
{
    return whd_bus_write_backplane_value(whd_driver, (uint32_t)SDIO_INT_STATUS, (uint8_t)4, intstatus);
}

whd_result_t whd_bus_sdio_wait_for_wlan_event(whd_driver_t whd_driver, cy_semaphore_t *transceive_semaphore)
{
    whd_result_t result = WHD_SUCCESS;
    uint32_t timeout_ms = 1;
    uint32_t delayed_release_timeout_ms;

    REFERENCE_DEBUG_ONLY_VARIABLE(result);

    delayed_release_timeout_ms = whd_bus_handle_delayed_release(whd_driver);
    if (delayed_release_timeout_ms != 0)
    {
        timeout_ms = delayed_release_timeout_ms;
    }
    else
    {
        result = whd_allow_wlan_bus_to_sleep(whd_driver);
        whd_assert("Error setting wlan sleep", (result == WHD_SUCCESS) || (result == WHD_PENDING) );

        if (result == WHD_SUCCESS)
        {
            timeout_ms = CY_RTOS_NEVER_TIMEOUT;
        }
    }

    /* Check if we have run out of bus credits */
    if ( (whd_sdpcm_has_tx_packet(whd_driver) == WHD_TRUE) && (whd_sdpcm_get_available_credits(whd_driver) == 0) )
    {
        /* Keep poking the WLAN until it gives us more credits */
        result = whd_bus_poke_wlan(whd_driver);
        whd_assert("Poking failed!", result == WHD_SUCCESS);

        result = cy_rtos_get_semaphore(transceive_semaphore, (uint32_t)MIN_OF(timeout_ms,
                                                                              WHD_THREAD_POKE_TIMEOUT), WHD_FALSE);
    }
    else
    {
        result = cy_rtos_get_semaphore(transceive_semaphore, (uint32_t)MIN_OF(timeout_ms,
                                                                              WHD_THREAD_POLL_TIMEOUT), WHD_FALSE);
    }
    whd_assert("Could not get whd sleep semaphore\n", (result == CY_RSLT_SUCCESS) || (result == CY_RTOS_TIMEOUT) );

    return result;
}

/* Device data transfer functions */
whd_result_t whd_bus_sdio_send_buffer(whd_driver_t whd_driver, whd_buffer_t buffer)
{
    whd_result_t retval;
    retval =
        whd_bus_transfer_bytes(whd_driver, BUS_WRITE, WLAN_FUNCTION, 0,
                               (uint16_t)(whd_buffer_get_current_piece_size(whd_driver,
                                                                            buffer) - sizeof(whd_buffer_t) ),
                               (whd_transfer_bytes_packet_t *)(whd_buffer_get_current_piece_data_pointer(whd_driver,
                                                                                                         buffer) +
                                                               sizeof(whd_buffer_t) ) );
    CHECK_RETURN(whd_buffer_release(whd_driver, buffer, WHD_NETWORK_TX) );
    if (retval == WHD_SUCCESS)
    {
        DELAYED_BUS_RELEASE_SCHEDULE(whd_driver, WHD_TRUE);
    }
    CHECK_RETURN (retval);

    return WHD_SUCCESS;
}

whd_result_t whd_bus_sdio_init(whd_driver_t whd_driver)
{
    uint8_t byte_data;
    whd_result_t result;
    uint32_t loop_count;
    whd_time_t elapsed_time, current_time;
    uint32_t wifi_firmware_image_size = 0;
    uint16_t chip_id;

    whd_bus_set_flow_control(whd_driver, WHD_FALSE);

    whd_bus_init_backplane_window(whd_driver);

    /* Setup the backplane*/
    loop_count = 0;
    do
    {
        /* Enable function 1 (backplane) */
        CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_IOEN, (uint8_t)1,
                                                  SDIO_FUNC_ENABLE_1) );
        if (loop_count != 0)
        {
            (void)cy_rtos_delay_milliseconds( (uint32_t)1 );    /* Ignore return - nothing can be done if it fails */
        }
        CHECK_RETURN(whd_bus_read_register_value (whd_driver, BUS_FUNCTION, SDIOD_CCCR_IOEN, (uint8_t)1, &byte_data) );
        loop_count++;
        if (loop_count >= (uint32_t)F0_WORKING_TIMEOUT_MS)
        {
            WPRINT_WHD_ERROR( ("Timeout while setting up the backplane, %s failed at %d \n", __func__, __LINE__) );
            return WHD_TIMEOUT;
        }
    } while (byte_data != (uint8_t)SDIO_FUNC_ENABLE_1);

    if (whd_driver->bus_priv->sdio_config.sdio_1bit_mode == WHD_FALSE)
    {
        /* Read the bus width and set to 4 bits */
        CHECK_RETURN(whd_bus_read_register_value (whd_driver, BUS_FUNCTION, SDIOD_CCCR_BICTRL, (uint8_t)1,
                                                  &byte_data) );
        CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_BICTRL, (uint8_t)1,
                                                  (byte_data & (~BUS_SD_DATA_WIDTH_MASK) ) |
                                                  BUS_SD_DATA_WIDTH_4BIT) );
        /* NOTE: We don't need to change our local bus settings since we're not sending any data (only using CMD52)
         * until after we change the bus speed further down */
    }

    /* Set the block size */

    /* Wait till the backplane is ready */
    loop_count = 0;
    while ( ( (result = whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_BLKSIZE_0, (uint8_t)1,
                                                     (uint32_t)SDIO_64B_BLOCK) ) == WHD_SUCCESS ) &&
            ( (result = whd_bus_read_register_value (whd_driver, BUS_FUNCTION, SDIOD_CCCR_BLKSIZE_0, (uint8_t)1,
                                                     &byte_data) ) == WHD_SUCCESS ) &&
            (byte_data != (uint8_t)SDIO_64B_BLOCK) &&
            (loop_count < (uint32_t)F0_WORKING_TIMEOUT_MS) )
    {
        (void)cy_rtos_delay_milliseconds( (uint32_t)1 );    /* Ignore return - nothing can be done if it fails */
        loop_count++;
        if (loop_count >= (uint32_t)F0_WORKING_TIMEOUT_MS)
        {
            /* If the system fails here, check the high frequency crystal is working */
            WPRINT_WHD_ERROR( ("Timeout while setting block size, %s failed at %d \n", __func__, __LINE__) );
            return WHD_TIMEOUT;
        }
    }

    CHECK_RETURN(result);

    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_BLKSIZE_0,   (uint8_t)1,
                                              (uint32_t)SDIO_64B_BLOCK) );
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_F1BLKSIZE_0, (uint8_t)1,
                                              (uint32_t)SDIO_64B_BLOCK) );
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_F2BLKSIZE_0, (uint8_t)1,
                                              (uint32_t)SDIO_64B_BLOCK) );
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_F2BLKSIZE_1, (uint8_t)1,
                                              (uint32_t)0) );                                                                                  /* Function 2 = 64 */

    /* Register interrupt handler*/
    whd_bus_sdio_irq_register(whd_driver);
    /* Enable SDIO IRQ */
    whd_bus_sdio_irq_enable(whd_driver, WHD_TRUE);

    /* Enable/Disable Client interrupts */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_INTEN, (uint8_t)1,
                                              INTR_CTL_MASTER_EN | INTR_CTL_FUNC1_EN | INTR_CTL_FUNC2_EN) );

    if (whd_driver->bus_priv->sdio_config.high_speed_sdio_clock)
    {
        /* This code is required if we want more than 25 MHz clock */
        CHECK_RETURN(whd_bus_read_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_SPEED_CONTROL, 1, &byte_data) );
        if ( (byte_data & 0x1) != 0 )
        {
            CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_SPEED_CONTROL, 1,
                                                      byte_data | SDIO_SPEED_EHS) );
        }
        else
        {
            WPRINT_WHD_ERROR( ("Error reading bus register, %s failed at %d \n", __func__, __LINE__) );
            return WHD_BUS_READ_REGISTER_ERROR;
        }
    }/* HIGH_SPEED_SDIO_CLOCK */



    /* Wait till the backplane is ready */
    loop_count = 0;
    while ( ( (result = whd_bus_read_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_IORDY, (uint8_t)1,
                                                    &byte_data) ) == WHD_SUCCESS ) &&
            ( (byte_data & SDIO_FUNC_READY_1) == 0 ) &&
            (loop_count < (uint32_t)F1_AVAIL_TIMEOUT_MS) )
    {
        (void)cy_rtos_delay_milliseconds( (uint32_t)1 );   /* Ignore return - nothing can be done if it fails */
        loop_count++;
    }
    if (loop_count >= (uint32_t)F1_AVAIL_TIMEOUT_MS)
    {
        WPRINT_WHD_ERROR( ("Timeout while waiting for backplane to be ready\n") );
        return WHD_TIMEOUT;
    }
    CHECK_RETURN(result);

    /* Set the ALP */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_CHIP_CLOCK_CSR, (uint8_t)1,
                                              (uint32_t)(SBSDIO_FORCE_HW_CLKREQ_OFF | SBSDIO_ALP_AVAIL_REQ |
                                                         SBSDIO_FORCE_ALP) ) );

    loop_count = 0;
    while ( ( (result = whd_bus_read_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_CHIP_CLOCK_CSR, (uint8_t)1,
                                                    &byte_data) ) == WHD_SUCCESS ) &&
            ( (byte_data & SBSDIO_ALP_AVAIL) == 0 ) &&
            (loop_count < (uint32_t)ALP_AVAIL_TIMEOUT_MS) )
    {
        (void)cy_rtos_delay_milliseconds( (uint32_t)1 );   /* Ignore return - nothing can be done if it fails */
        loop_count++;
    }
    if (loop_count >= (uint32_t)ALP_AVAIL_TIMEOUT_MS)
    {
        WPRINT_WHD_ERROR( ("Timeout while waiting for alp clock\n") );
        return WHD_TIMEOUT;
    }
    CHECK_RETURN(result);

    /* Clear request for ALP */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_CHIP_CLOCK_CSR, (uint8_t)1, 0) );

    /* Disable the extra SDIO pull-ups */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_PULL_UP, (uint8_t)1, 0) );
    /* Enable F1 and F2 */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_IOEN, (uint8_t)1,
                                              SDIO_FUNC_ENABLE_1 | SDIO_FUNC_ENABLE_2) );

    /* Setup host-wake signals */
    CHECK_RETURN(whd_bus_sdio_init_oob_intr(whd_driver) );

    /* Enable F2 interrupt only */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_INTEN, (uint8_t)1,
                                              INTR_CTL_MASTER_EN | INTR_CTL_FUNC2_EN) );

    CHECK_RETURN(whd_bus_read_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_IORDY, (uint8_t)1, &byte_data) );

    /* Read the chip id */
    CHECK_RETURN(whd_bus_read_backplane_value(whd_driver, CHIPCOMMON_BASE_ADDRESS, 2, (uint8_t *)&chip_id) );
    whd_chip_set_chip_id(whd_driver, chip_id);

    cy_rtos_get_time(&elapsed_time);
    result = whd_bus_sdio_download_firmware(whd_driver);
    cy_rtos_get_time(&current_time);
    elapsed_time = current_time - elapsed_time;
    CHECK_RETURN(whd_resource_size(whd_driver, WHD_RESOURCE_WLAN_FIRMWARE, &wifi_firmware_image_size) );
    WPRINT_WHD_INFO( ("WLAN FW download size: %" PRIu32 " bytes\n", wifi_firmware_image_size) );
    WPRINT_WHD_INFO( ("WLAN FW download time: %" PRIu32 " ms\n", elapsed_time) );

    if (result != WHD_SUCCESS)
    {
        /*  either an error or user abort */
        WPRINT_WHD_ERROR( ("SDIO firmware download error, %s failed at %d \n", __func__, __LINE__) );
        return result;
    }

    /* Wait for F2 to be ready */
    loop_count = 0;
    while ( ( (result = whd_bus_read_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_IORDY, (uint8_t)1,
                                                    &byte_data) ) == WHD_SUCCESS ) &&
            ( (byte_data & SDIO_FUNC_READY_2) == 0 ) &&
            (loop_count < (uint32_t)F2_READY_TIMEOUT_MS) )
    {
        (void)cy_rtos_delay_milliseconds( (uint32_t)1 );   /* Ignore return - nothing can be done if it fails */
        loop_count++;
    }
    if (loop_count >= (uint32_t)F2_READY_TIMEOUT_MS)
    {
        /* If your system fails here, it could be due to incorrect NVRAM variables.
         * Check which 'wifi_nvram_image.h' file your platform is using, and
         * check that it matches the WLAN device on your platform, including the
         * crystal frequency.
         */
        WPRINT_WHD_ERROR( ("Timeout while waiting for function 2 to be ready\n") );
        /* Reachable after hitting assert */
        return WHD_TIMEOUT;
    }

    CHECK_RETURN(whd_chip_specific_init(whd_driver) );
    CHECK_RETURN(whd_ensure_wlan_bus_is_up(whd_driver) );
    cyhal_sdio_irq_enable(whd_driver->bus_priv->sdio_obj, CYHAL_SDIO_CARD_INTERRUPT, WHD_TRUE);
    UNUSED_PARAMETER(elapsed_time);
    return result;
}

whd_result_t whd_bus_sdio_deinit(whd_driver_t whd_driver)
{
    CHECK_RETURN(whd_bus_sdio_deinit_oob_intr(whd_driver) );

    cyhal_sdio_irq_enable(whd_driver->bus_priv->sdio_obj, CYHAL_SDIO_CARD_INTERRUPT, WHD_FALSE);

    CHECK_RETURN(whd_allow_wlan_bus_to_sleep(whd_driver) );

    whd_bus_set_resource_download_halt(whd_driver, WHD_FALSE);

    DELAYED_BUS_RELEASE_SCHEDULE(whd_driver, WHD_FALSE);

    return WHD_SUCCESS;
}

whd_bool_t whd_bus_sdio_wake_interrupt_present(whd_driver_t whd_driver)
{
    uint32_t int_status = 0;

    /* Ensure the wlan backplane bus is up */
    if (WHD_SUCCESS != whd_ensure_wlan_bus_is_up(whd_driver) )
        return WHD_FALSE;

    if (whd_bus_read_backplane_value(whd_driver, (uint32_t)SDIO_INT_STATUS, (uint8_t)4,
                                     (uint8_t *)&int_status) != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("%s: Error reading interrupt status\n", __FUNCTION__) );
        goto exit;
    }
    if ( (I_HMB_HOST_INT & int_status) != 0 )
    {
        /* Clear any interrupts */
        if (whd_bus_write_backplane_value(whd_driver, (uint32_t)SDIO_INT_STATUS, (uint8_t)4,
                                          I_HMB_HOST_INT) != WHD_SUCCESS)
        {
            WPRINT_WHD_ERROR( ("%s: Error clearing interrupts\n", __FUNCTION__) );
            goto exit;
        }
        if (whd_bus_read_backplane_value(whd_driver, (uint32_t)SDIO_INT_STATUS, (uint8_t)4,
                                         (uint8_t *)&int_status) != WHD_SUCCESS)
        {
            WPRINT_WHD_ERROR( ("%s: Error reading interrupt status\n", __FUNCTION__) );
            goto exit;
        }
        WPRINT_WHD_DEBUG( ("whd_bus_sdio_wake_interrupt_present after clearing int_status  = [%x]\n",
                           (uint8_t)int_status) );
        return WHD_TRUE;
    }
exit:
    return WHD_FALSE;
}

uint32_t whd_bus_sdio_packet_available_to_read(whd_driver_t whd_driver)
{
    uint32_t int_status = 0;

    /* Ensure the wlan backplane bus is up */
    CHECK_RETURN(whd_ensure_wlan_bus_is_up(whd_driver) );

    /* Read the IntStatus */
    if (whd_bus_read_backplane_value(whd_driver, (uint32_t)SDIO_INT_STATUS, (uint8_t)4,
                                     (uint8_t *)&int_status) != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("%s: Error reading interrupt status\n", __FUNCTION__) );
        int_status = 0;
        goto exit;
    }

    if ( (HOSTINTMASK & int_status) != 0 )
    {
        /* Clear any interrupts */
        if (whd_bus_write_backplane_value(whd_driver, (uint32_t)SDIO_INT_STATUS, (uint8_t)4,
                                          int_status & HOSTINTMASK) != WHD_SUCCESS)
        {
            WPRINT_WHD_ERROR( ("%s: Error clearing interrupts\n", __FUNCTION__) );
            int_status = 0;
            goto exit;
        }
    }
exit:
    return ( (int_status) & (FRAME_AVAILABLE_MASK) );
}

/*
 * From internal documentation: hwnbu-twiki/SdioMessageEncapsulation
 * When data is available on the device, the device will issue an interrupt:
 * - the device should signal the interrupt as a hint that one or more data frames may be available on the device for reading
 * - the host may issue reads of the 4 byte length tag at any time -- that is, whether an interupt has been issued or not
 * - if a frame is available, the tag read should return a nonzero length (>= 4) and the host can then read the remainder of the frame by issuing one or more CMD53 reads
 * - if a frame is not available, the 4byte tag read should return zero
 */
whd_result_t whd_bus_sdio_read_frame(whd_driver_t whd_driver, whd_buffer_t *buffer)
{
    uint16_t hwtag[8];
    uint16_t extra_space_required;
    whd_result_t result;

    *buffer = NULL;

    /* Ensure the wlan backplane bus is up */
    CHECK_RETURN(whd_ensure_wlan_bus_is_up(whd_driver) );

    /* Read the frame header and verify validity */
    memset(hwtag, 0, sizeof(hwtag) );

    result = whd_bus_sdio_transfer(whd_driver, BUS_READ, WLAN_FUNCTION, 0, (uint16_t)INITIAL_READ, (uint8_t *)hwtag,
                                   RESPONSE_NEEDED);
    if (result != WHD_SUCCESS)
    {
        (void)whd_bus_sdio_abort_read(whd_driver, WHD_FALSE);    /* ignore return - not much can be done if this fails */
        WPRINT_WHD_ERROR( ("Error during SDIO receive, %s failed at %d \n", __func__, __LINE__) );
        return WHD_SDIO_RX_FAIL;
    }

    if ( ( (hwtag[0] | hwtag[1]) == 0 ) ||
         ( (hwtag[0] ^ hwtag[1]) != (uint16_t)0xFFFF ) )
    {
        return WHD_HWTAG_MISMATCH;
    }

    if ( (hwtag[0] == (uint16_t)12) &&
         (whd_driver->internal_info.whd_wlan_status.state == WLAN_UP) )
    {
        result = whd_bus_sdio_transfer(whd_driver, BUS_READ, WLAN_FUNCTION, 0, (uint16_t)8, (uint8_t *)&hwtag[2],
                                       RESPONSE_NEEDED);
        if (result != WHD_SUCCESS)
        {
            /* ignore return - not much can be done if this fails */
            (void)whd_bus_sdio_abort_read(whd_driver, WHD_FALSE);
            WPRINT_WHD_ERROR( ("Error during SDIO receive, %s failed at %d \n", __func__, __LINE__) );
            return WHD_SDIO_RX_FAIL;
        }
        whd_sdpcm_update_credit(whd_driver, (uint8_t *)hwtag);
        return WHD_SUCCESS;
    }

    /* Calculate the space we need to store entire packet */
    if ( (hwtag[0] > (uint16_t)INITIAL_READ) )
    {
        extra_space_required = (uint16_t)(hwtag[0] - (uint16_t)INITIAL_READ);
    }
    else
    {
        extra_space_required = 0;
    }

    /* Allocate a suitable buffer */
    result = whd_host_buffer_get(whd_driver, buffer, WHD_NETWORK_RX, (unsigned short)( (uint16_t)INITIAL_READ +
                                                                                       extra_space_required +
                                                                                       (uint16_t)sizeof(
                                                                                           whd_buffer_header_t) ),
                                 WHD_FALSE);
    if (result != WHD_SUCCESS)
    {
        /* Read out the first 12 bytes to get the bus credit information, 4 bytes are already read in hwtag */
        whd_assert("Get buffer error",
                   ( (result == WHD_BUFFER_UNAVAILABLE_TEMPORARY) || (result == WHD_BUFFER_UNAVAILABLE_PERMANENT) ) );
        result = whd_bus_sdio_transfer(whd_driver, BUS_READ, WLAN_FUNCTION, 0, (uint16_t)8, (uint8_t *)&hwtag[2],
                                       RESPONSE_NEEDED);
        if (result != WHD_SUCCESS)
        {
            /* ignore return - not much can be done if this fails */
            (void)whd_bus_sdio_abort_read(whd_driver, WHD_FALSE);
            WPRINT_WHD_ERROR( ("Error during SDIO receive, %s failed at %d \n", __func__, __LINE__) );
            return WHD_SDIO_RX_FAIL;
        }
        result = whd_bus_sdio_abort_read(whd_driver, WHD_FALSE);
        whd_assert("Read-abort failed", result == WHD_SUCCESS);
        REFERENCE_DEBUG_ONLY_VARIABLE(result);

        whd_sdpcm_update_credit(whd_driver, (uint8_t *)hwtag);
        WPRINT_WHD_ERROR( ("Failed to allocate a buffer to receive into, %s failed at %d \n", __func__, __LINE__) );
        return WHD_RX_BUFFER_ALLOC_FAIL;
    }

    /* Copy the data already read */
    memcpy(whd_buffer_get_current_piece_data_pointer(whd_driver, *buffer) + sizeof(whd_buffer_header_t), hwtag,
           (size_t)INITIAL_READ);

    /* Read the rest of the data */
    if (extra_space_required > 0)
    {
        result = whd_bus_sdio_transfer(whd_driver, BUS_READ, WLAN_FUNCTION, 0, extra_space_required,
                                       whd_buffer_get_current_piece_data_pointer(whd_driver,
                                                                                 *buffer) + sizeof(whd_buffer_header_t) +
                                       INITIAL_READ, RESPONSE_NEEDED);
        if (result != WHD_SUCCESS)
        {
            (void)whd_bus_sdio_abort_read(whd_driver, WHD_FALSE);    /* ignore return - not much can be done if this fails */
            CHECK_RETURN(whd_buffer_release(whd_driver, *buffer, WHD_NETWORK_RX) );
            WPRINT_WHD_ERROR( ("Error during SDIO receive, %s failed at %d \n", __func__, __LINE__) );
            return WHD_SDIO_RX_FAIL;
        }
    }
    DELAYED_BUS_RELEASE_SCHEDULE(whd_driver, WHD_TRUE);
    return WHD_SUCCESS;
}

/******************************************************
*     Function definitions for Protocol Common
******************************************************/

/* Device register access functions */
whd_result_t whd_bus_sdio_write_backplane_value(whd_driver_t whd_driver, uint32_t address, uint8_t register_length,
                                                uint32_t value)
{
    CHECK_RETURN(whd_bus_set_backplane_window(whd_driver, address) );

    address &= SBSDIO_SB_OFT_ADDR_MASK;

    if (register_length == 4)
        address |= SBSDIO_SB_ACCESS_2_4B_FLAG;

    CHECK_RETURN(whd_bus_sdio_transfer(whd_driver, BUS_WRITE, BACKPLANE_FUNCTION, address, register_length,
                                       (uint8_t *)&value, RESPONSE_NEEDED) );

    return whd_bus_set_backplane_window(whd_driver, CHIPCOMMON_BASE_ADDRESS);
}

whd_result_t whd_bus_sdio_read_backplane_value(whd_driver_t whd_driver, uint32_t address, uint8_t register_length,
                                               uint8_t *value)
{
    *value = 0;
    CHECK_RETURN(whd_bus_set_backplane_window(whd_driver, address) );

    address &= SBSDIO_SB_OFT_ADDR_MASK;

    if (register_length == 4)
        address |= SBSDIO_SB_ACCESS_2_4B_FLAG;

    CHECK_RETURN(whd_bus_sdio_transfer(whd_driver, BUS_READ, BACKPLANE_FUNCTION, address, register_length, value,
                                       RESPONSE_NEEDED) );

    return whd_bus_set_backplane_window(whd_driver, CHIPCOMMON_BASE_ADDRESS);
}

whd_result_t whd_bus_sdio_write_register_value(whd_driver_t whd_driver, whd_bus_function_t function, uint32_t address,
                                               uint8_t value_length, uint32_t value)
{
    return whd_bus_sdio_transfer(whd_driver, BUS_WRITE, function, address, value_length, (uint8_t *)&value,
                                 RESPONSE_NEEDED);
}

whd_result_t whd_bus_sdio_transfer_bytes(whd_driver_t whd_driver, whd_bus_transfer_direction_t direction,
                                         whd_bus_function_t function, uint32_t address, uint16_t size,
                                         whd_transfer_bytes_packet_t *data)
{
    DISABLE_COMPILER_WARNING(diag_suppress = Pa039)
    return whd_bus_sdio_transfer(whd_driver, direction, function, address, size, (uint8_t *)data->data,
                                 RESPONSE_NEEDED);
    ENABLE_COMPILER_WARNING(diag_suppress = Pa039)
}

/******************************************************
*             Static  Function definitions
******************************************************/

static whd_result_t whd_bus_sdio_transfer(whd_driver_t whd_driver, whd_bus_transfer_direction_t direction,
                                          whd_bus_function_t function, uint32_t address, uint16_t data_size,
                                          uint8_t *data, sdio_response_needed_t response_expected)
{
    /* Note: this function had broken retry logic (never retried), which has been removed.
     * Failing fast helps problems on the bus get brought to light more quickly
     * and preserves the original behavior.
     */
    whd_result_t result = WHD_SUCCESS;
    uint16_t data_byte_size;
    uint16_t data_blk_size;

    if (data_size == 0)
    {
        return WHD_BADARG;
    }
    else if (data_size == (uint16_t)1)
    {
        return whd_bus_sdio_cmd52(whd_driver, direction, function, address, *data, response_expected, data);
    }
    else if (whd_driver->internal_info.whd_wlan_status.state == WLAN_UP)
    {
        return whd_bus_sdio_cmd53(whd_driver, direction, function,
                                  (data_size >= (uint16_t)64) ? SDIO_BLOCK_MODE : SDIO_BYTE_MODE, address, data_size,
                                  data, response_expected, NULL);
    }
    else
    {
        /* We need to handle remaining size for source image download */
        data_byte_size = data_size % SDIO_64B_BLOCK;
        data_blk_size = data_size - data_byte_size;
        if (data_blk_size != 0)
        {
            result = whd_bus_sdio_cmd53(whd_driver, direction, function, SDIO_BLOCK_MODE, address,
                                        data_blk_size, data, response_expected, NULL);
            if (result != WHD_SUCCESS)
            {
                return result;
            }
            data += data_blk_size;
            address += data_blk_size;
        }
        if (data_byte_size)
        {
            result = whd_bus_sdio_cmd53(whd_driver, direction, function, SDIO_BYTE_MODE, address,
                                        data_byte_size, data, response_expected, NULL);
        }
        return result;
    }
}

static whd_result_t whd_bus_sdio_cmd52(whd_driver_t whd_driver, whd_bus_transfer_direction_t direction,
                                       whd_bus_function_t function, uint32_t address, uint8_t value,
                                       sdio_response_needed_t response_expected, uint8_t *response)
{
    uint32_t sdio_response;
    whd_result_t result;
    sdio_cmd_argument_t arg;
    arg.value = 0;
    arg.cmd52.function_number = (unsigned int)(function & BUS_FUNCTION_MASK);
    arg.cmd52.register_address = (unsigned int)(address & 0x00001ffff);
    arg.cmd52.rw_flag = (unsigned int)( (direction == BUS_WRITE) ? 1 : 0 );
    arg.cmd52.write_data = value;

    WHD_BUS_STATS_INCREMENT_VARIABLE(whd_driver->bus_priv, cmd52);
    result = cyhal_sdio_send_cmd(whd_driver->bus_priv->sdio_obj, (cyhal_transfer_t)direction,
                                 CYHAL_SDIO_CMD_IO_RW_DIRECT, arg.value,
                                 &sdio_response);
    WHD_BUS_STATS_CONDITIONAL_INCREMENT_VARIABLE(whd_driver->bus_priv, (result != WHD_SUCCESS), cmd52_fail);

    if (response != NULL)
    {
        *response = (uint8_t)(sdio_response & 0x00000000ff);
    }

    if (result == CY_RSLT_SUCCESS)
        return WHD_SUCCESS;
    else
        return WHD_HAL_ERROR;

}

static whd_result_t whd_bus_sdio_cmd53(whd_driver_t whd_driver, whd_bus_transfer_direction_t direction,
                                       whd_bus_function_t function, sdio_transfer_mode_t mode, uint32_t address,
                                       uint16_t data_size, uint8_t *data,
                                       sdio_response_needed_t response_expected, uint32_t *response)
{
    sdio_cmd_argument_t arg;
    whd_result_t result;

    if (direction == BUS_WRITE)
    {
        WHD_BUS_STATS_INCREMENT_VARIABLE(whd_driver->bus_priv, cmd53_write);
    }

    arg.value = 0;
    arg.cmd53.function_number = (unsigned int)(function & BUS_FUNCTION_MASK);
    arg.cmd53.register_address = (unsigned int)(address & BIT_MASK(17) );
    arg.cmd53.op_code = (unsigned int)1;
    arg.cmd53.rw_flag = (unsigned int)( (direction == BUS_WRITE) ? 1 : 0 );

    if (mode == SDIO_BYTE_MODE)
    {
        whd_assert("whd_bus_sdio_cmd53: data_size > 512 for byte mode", (data_size <= (uint16_t )512) );
        arg.cmd53.count = (unsigned int)(data_size & 0x1FF);

        result =
            cyhal_sdio_bulk_transfer(whd_driver->bus_priv->sdio_obj, (cyhal_transfer_t)direction, arg.value,
                                     (uint32_t *)data, data_size, response);

        if (result != CY_RSLT_SUCCESS)
        {
            WPRINT_WHD_ERROR( ("%s:%d cyhal_sdio_bulk_transfer SDIO_BYTE_MODE failed\n", __func__, __LINE__) );
            goto done;
        }
    }
    else
    {
        arg.cmd53.count = (unsigned int)( (data_size / (uint16_t)SDIO_64B_BLOCK) & BIT_MASK(9) );
        if ( (uint32_t)(arg.cmd53.count * (uint16_t)SDIO_64B_BLOCK) < data_size )
        {
            ++arg.cmd53.count;
        }
        arg.cmd53.block_mode = (unsigned int)1;

        result =
            cyhal_sdio_bulk_transfer(whd_driver->bus_priv->sdio_obj, (cyhal_transfer_t)direction, arg.value,
                                     (uint32_t *)data, data_size, response);

        if (result != CY_RSLT_SUCCESS)
        {
            WPRINT_WHD_ERROR( ("%s:%d cyhal_sdio_bulk_transfer failed\n", __func__, __LINE__) );
            goto done;
        }
    }

    if (direction == BUS_READ)
    {
        WHD_BUS_STATS_INCREMENT_VARIABLE(whd_driver->bus_priv, cmd53_read);
    }

done:
    WHD_BUS_STATS_CONDITIONAL_INCREMENT_VARIABLE(whd_driver->bus_priv,
                                                 ( (result != WHD_SUCCESS) && (direction == BUS_READ) ),
                                                 cmd53_read_fail);
    WHD_BUS_STATS_CONDITIONAL_INCREMENT_VARIABLE(whd_driver->bus_priv,
                                                 ( (result != WHD_SUCCESS) && (direction == BUS_WRITE) ),
                                                 cmd53_write_fail);
    if (result == CY_RSLT_SUCCESS)
        return WHD_SUCCESS;
    else
        return WHD_HAL_ERROR;
}

static whd_result_t whd_bus_sdio_download_firmware(whd_driver_t whd_driver)
{
    uint8_t csr_val = 0;
    whd_result_t result;
    uint32_t loop_count;
    uint32_t ram_start_address;

    ram_start_address = GET_C_VAR(whd_driver, ATCM_RAM_BASE_ADDRESS);

    if (ram_start_address != 0)
    {
        CHECK_RETURN(whd_reset_core(whd_driver, WLAN_ARM_CORE, SICF_CPUHALT, SICF_CPUHALT) );
    }
    else
    {
        CHECK_RETURN(whd_disable_device_core(whd_driver, WLAN_ARM_CORE, WLAN_CORE_FLAG_NONE) );
        CHECK_RETURN(whd_disable_device_core(whd_driver, SOCRAM_CORE, WLAN_CORE_FLAG_NONE) );
        CHECK_RETURN(whd_reset_device_core(whd_driver, SOCRAM_CORE, WLAN_CORE_FLAG_NONE) );

        CHECK_RETURN(whd_chip_specific_socsram_init(whd_driver) );
    }

#if 0
    /* 43362 specific: Remap JTAG pins to UART output */
    uint32_t data = 0;
    CHECK_RETURN(whd_bus_write_backplane_value(0x18000650, 1, 1) );
    CHECK_RETURN(whd_bus_read_backplane_value(0x18000654, 4, (uint8_t *)&data) );
    data |= (1 << 24);
    CHECK_RETURN(whd_bus_write_backplane_value(0x18000654, 4, data) );
#endif

    result = whd_bus_write_wifi_firmware_image(whd_driver);

    if (result == WHD_UNFINISHED)
    {
        WPRINT_WHD_INFO( ("User aborted fw download\n") );
        /* user aborted */
        return result;
    }
    else if (result != WHD_SUCCESS)
    {
        whd_assert("Failed to load wifi firmware\n", result == WHD_SUCCESS);
        return result;
    }

    CHECK_RETURN(whd_bus_write_wifi_nvram_image(whd_driver) );

    /* Take the ARM core out of reset */
    if (ram_start_address != 0)
    {
        CHECK_RETURN(whd_reset_core(whd_driver, WLAN_ARM_CORE, 0, 0) );
    }
    else
    {
        CHECK_RETURN(whd_reset_device_core(whd_driver, WLAN_ARM_CORE, WLAN_CORE_FLAG_NONE) );

        result = whd_device_core_is_up(whd_driver, WLAN_ARM_CORE);
        if (result != WHD_SUCCESS)
        {
            WPRINT_WHD_ERROR( ("Could not bring ARM core up\n") );
            /* Reachable after hitting assert */
            return result;
        }
    }

    /* Wait until the High Throughput clock is available */
    loop_count = 0;
    while ( ( (result = whd_bus_read_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_CHIP_CLOCK_CSR, (uint8_t)1,
                                                    &csr_val) ) == WHD_SUCCESS ) &&
            ( (csr_val & SBSDIO_HT_AVAIL) == 0 ) &&
            (loop_count < (uint32_t)HT_AVAIL_TIMEOUT_MS) )
    {
        (void)cy_rtos_delay_milliseconds( (uint32_t)1 );   /* Ignore return - nothing can be done if it fails */
        loop_count++;
    }
    if (loop_count >= (uint32_t)HT_AVAIL_TIMEOUT_MS)
    {
        /* If your system times out here, it means that the WLAN firmware is not booting.
         * Check that your WLAN chip matches the 'wifi_image.c' being built - in GNU toolchain, $(CHIP)
         * makefile variable must be correct.
         */
        WPRINT_WHD_ERROR( ("Timeout while waiting for high throughput clock\n") );
        /* Reachable after hitting assert */
        return WHD_TIMEOUT;
    }
    if (result != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error while waiting for high throughput clock\n") );
        /* Reachable after hitting assert */
        return result;
    }

    /* Set up the interrupt mask and enable interrupts */
    CHECK_RETURN(whd_bus_write_backplane_value(whd_driver, SDIO_INT_HOST_MASK, (uint8_t)4, HOSTINTMASK) );

    /* Enable F2 interrupts. This wasn't required for 4319 but is for the 43362 */
    CHECK_RETURN(whd_bus_write_backplane_value(whd_driver, SDIO_FUNCTION_INT_MASK, (uint8_t)1,
                                               SDIO_FUNC_MASK_F1 | SDIO_FUNC_MASK_F2) );

    /* Lower F2 Watermark to avoid DMA Hang in F2 when SD Clock is stopped. */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_FUNCTION2_WATERMARK, (uint8_t)1,
                                              (uint32_t)SDIO_F2_WATERMARK) );

    return WHD_SUCCESS;
}

/** Aborts a SDIO read of a packet from the 802.11 device
 *
 * This function is necessary because the only way to obtain the size of the next
 * available received packet is to read the first four bytes of the packet.
 * If the system reads these four bytes, and then fails to allocate the required
 * memory, then this function allows the system to abort the packet read cleanly,
 * and to optionally tell the 802.11 device to keep it allowing reception once
 * memory is available.
 *
 * In order to do this abort, the following actions are performed:
 * - Sets abort bit for Function 2 (WLAN Data) to request stopping transfer
 * - Sets Read Frame Termination bit to flush and reset fifos
 * - If packet is to be kept and resent by 802.11 device, a NAK  is sent
 * - Wait whilst the Fifo is emptied of the packet ( reading during this period would cause all zeros to be read )
 *
 * @param retry : WHD_TRUE if 802.11 device is to keep and resend packet
 *                WHD_FALSE if 802.11 device is to drop packet
 *
 * @return WHD_SUCCESS if successful, otherwise error code
 */
static whd_result_t whd_bus_sdio_abort_read(whd_driver_t whd_driver, whd_bool_t retry)
{
    WHD_BUS_STATS_INCREMENT_VARIABLE(whd_driver->bus_priv, read_aborts);

    /* Abort transfer on WLAN_FUNCTION */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_IOABORT, (uint8_t)1,
                                              (uint32_t)WLAN_FUNCTION) );

    /* Send frame terminate */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_FRAME_CONTROL, (uint8_t)1,
                                              SFC_RF_TERM) );

    /* If we want to retry message, send NAK */
    if (retry == WHD_TRUE)
    {
        CHECK_RETURN(whd_bus_write_backplane_value(whd_driver, (uint32_t)SDIO_TO_SB_MAIL_BOX, (uint8_t)1, SMB_NAK) );
    }

    return WHD_SUCCESS;
}

whd_result_t whd_bus_sdio_read_register_value(whd_driver_t whd_driver, whd_bus_function_t function, uint32_t address,
                                              uint8_t value_length, uint8_t *value)
{
    memset(value, 0, (size_t)value_length);
    return whd_bus_sdio_transfer(whd_driver, BUS_READ, function, address, value_length, value, RESPONSE_NEEDED);
}

whd_result_t whd_bus_sdio_poke_wlan(whd_driver_t whd_driver)
{
    /*TODO: change 1<<3 to a register hash define */
    return whd_bus_write_backplane_value(whd_driver, SDIO_TO_SB_MAILBOX, (uint8_t)4, (uint32_t)(1 << 3) );
}

whd_result_t whd_bus_sdio_wakeup(whd_driver_t whd_driver)
{
    return WHD_SUCCESS;
}

whd_result_t whd_bus_sdio_sleep(whd_driver_t whd_driver)
{
    return WHD_SUCCESS;
}

static whd_result_t whd_bus_sdio_set_oob_interrupt(whd_driver_t whd_driver, uint8_t gpio_pin_number)
{
    if (gpio_pin_number != 0)
    {
        /* Redirect to OOB interrupt to GPIO1 */
        CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_GPIO_SELECT, (uint8_t)1,
                                                  (uint32_t)0xF) );
        CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_GPIO_OUTPUT, (uint8_t)1,
                                                  (uint32_t)0x0) );

        /* Enable GPIOx (bit x) */
        CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_GPIO_ENABLE, (uint8_t)1,
                                                  (uint32_t)0x2) );

        /* Set GPIOx (bit x) on Chipcommon GPIO Control register */
        CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, CHIPCOMMON_GPIO_CONTROL, (uint8_t)4,
                                                  (uint32_t)0x2) );
    }

    return WHD_SUCCESS;
}

void whd_bus_sdio_init_stats(whd_driver_t whd_driver)
{
    memset(&whd_driver->bus_priv->whd_bus_stats, 0, sizeof(whd_bus_stats_t) );
}

whd_result_t whd_bus_sdio_print_stats(whd_driver_t whd_driver, whd_bool_t reset_after_print)
{
    WPRINT_MACRO( ("Bus Stats.. \n"
                   "cmd52:%" PRIu32 ", cmd53_read:%" PRIu32 ", cmd53_write:%" PRIu32 "\n"
                   "cmd52_fail:%" PRIu32 ", cmd53_read_fail:%" PRIu32 ", cmd53_write_fail:%" PRIu32 "\n"
                   "oob_intrs:%" PRIu32 ", sdio_intrs:%" PRIu32 ", error_intrs:%" PRIu32 ", read_aborts:%" PRIu32
                   "\n",
                   whd_driver->bus_priv->whd_bus_stats.cmd52, whd_driver->bus_priv->whd_bus_stats.cmd53_read,
                   whd_driver->bus_priv->whd_bus_stats.cmd53_write,
                   whd_driver->bus_priv->whd_bus_stats.cmd52_fail,
                   whd_driver->bus_priv->whd_bus_stats.cmd53_read_fail,
                   whd_driver->bus_priv->whd_bus_stats.cmd53_write_fail,
                   whd_driver->bus_priv->whd_bus_stats.oob_intrs,
                   whd_driver->bus_priv->whd_bus_stats.sdio_intrs,
                   whd_driver->bus_priv->whd_bus_stats.error_intrs,
                   whd_driver->bus_priv->whd_bus_stats.read_aborts) );

    if (reset_after_print == WHD_TRUE)
    {
        memset(&whd_driver->bus_priv->whd_bus_stats, 0, sizeof(whd_bus_stats_t) );
    }

    return WHD_SUCCESS;
}

/* Waking the firmware up from Deep Sleep */
whd_result_t whd_bus_sdio_reinit_stats(whd_driver_t whd_driver, whd_bool_t wake_from_firmware)
{
    whd_result_t result = WHD_SUCCESS;
    uint8_t byte_data;
    uint32_t loop_count;
    loop_count = 0;

    /* Setup the backplane*/
    loop_count = 0;

    do
    {
        /* Enable function 1 (backplane) */
        CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_IOEN, (uint8_t)1,
                                                  SDIO_FUNC_ENABLE_1) );
        if (loop_count != 0)
        {
            (void)cy_rtos_delay_milliseconds( (uint32_t)1 );  /* Ignore return - nothing can be done if it fails */
        }

        CHECK_RETURN(whd_bus_read_register_value (whd_driver, BUS_FUNCTION, SDIOD_CCCR_IOEN, (uint8_t)1, &byte_data) );
        loop_count++;
        if (loop_count >= (uint32_t)F0_WORKING_TIMEOUT_MS)
        {
            WPRINT_WHD_ERROR( ("Timeout on CCCR update\n") );
            return WHD_TIMEOUT;
        }
    } while (byte_data != (uint8_t)SDIO_FUNC_ENABLE_1);

    if (whd_driver->bus_priv->sdio_config.sdio_1bit_mode == WHD_FALSE)
    {
        /* Read the bus width and set to 4 bits */
        CHECK_RETURN(whd_bus_read_register_value (whd_driver, BUS_FUNCTION, SDIOD_CCCR_BICTRL, (uint8_t)1,
                                                  &byte_data) );
        CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_BICTRL, (uint8_t)1,
                                                  (byte_data & (~BUS_SD_DATA_WIDTH_MASK) ) | BUS_SD_DATA_WIDTH_4BIT) );
        /* NOTE: We don't need to change our local bus settings since we're not sending any data (only using CMD52)
         * until after we change the bus speed further down */
    }

    /* Set the block size */
    /* Wait till the backplane is ready */
    loop_count = 0;
    while ( ( (result = whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_BLKSIZE_0, (uint8_t)1,
                                                     (uint32_t)SDIO_64B_BLOCK) ) == WHD_SUCCESS ) &&
            ( (result = whd_bus_read_register_value (whd_driver, BUS_FUNCTION, SDIOD_CCCR_BLKSIZE_0, (uint8_t)1,
                                                     &byte_data) ) == WHD_SUCCESS ) &&
            (byte_data != (uint8_t)SDIO_64B_BLOCK) &&
            (loop_count < (uint32_t)F0_WORKING_TIMEOUT_MS) )
    {
        (void)cy_rtos_delay_milliseconds( (uint32_t)1 );    /* Ignore return - nothing can be done if it fails */
        loop_count++;
        if (loop_count >= (uint32_t)F0_WORKING_TIMEOUT_MS)
        {
            /* If the system fails here, check the high frequency crystal is working */
            WPRINT_WHD_ERROR( ("Timeout while setting block size\n") );
            return WHD_TIMEOUT;
        }
    }

    CHECK_RETURN(result);

    WPRINT_WHD_DEBUG( ("Modding registers for blocks\n") );

    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_BLKSIZE_0,   (uint8_t)1,
                                              (uint32_t)SDIO_64B_BLOCK) );
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_F1BLKSIZE_0, (uint8_t)1,
                                              (uint32_t)SDIO_64B_BLOCK) );
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_F2BLKSIZE_0, (uint8_t)1,
                                              (uint32_t)SDIO_64B_BLOCK) );
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_F2BLKSIZE_1, (uint8_t)1,
                                              (uint32_t)0) );                                                                                  /* Function 2 = 64 */

    /* Enable/Disable Client interrupts */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_INTEN,       (uint8_t)1,
                                              INTR_CTL_MASTER_EN | INTR_CTL_FUNC1_EN | INTR_CTL_FUNC2_EN) );


    if (whd_driver->bus_priv->sdio_config.high_speed_sdio_clock)
    {
        WPRINT_WHD_DEBUG( ("SDIO HS clock enable\n") );

        /* This code is required if we want more than 25 MHz clock */
        CHECK_RETURN(whd_bus_read_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_SPEED_CONTROL, 1, &byte_data) );
        if ( (byte_data & 0x1) != 0 )
        {
            CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_SPEED_CONTROL, 1,
                                                      byte_data | SDIO_SPEED_EHS) );
        }
        else
        {
            WPRINT_WHD_ERROR( ("Error writing to WLAN register, %s failed at %d \n", __func__, __LINE__) );
            return WHD_BUS_READ_REGISTER_ERROR;
        }
    } /* HIGH_SPEED_SDIO_CLOCK */

    /* Wait till the backplane is ready */
    loop_count = 0;
    while ( ( (result = whd_bus_read_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_IORDY, (uint8_t)1,
                                                    &byte_data) ) == WHD_SUCCESS ) &&
            ( (byte_data & SDIO_FUNC_READY_1) == 0 ) &&
            (loop_count < (uint32_t)F1_AVAIL_TIMEOUT_MS) )
    {
        (void)cy_rtos_delay_milliseconds( (uint32_t)1 );   /* Ignore return - nothing can be done if it fails */
        loop_count++;
    }

    if (loop_count >= (uint32_t)F1_AVAIL_TIMEOUT_MS)
    {
        WPRINT_WHD_ERROR( ("Timeout while waiting for backplane to be ready\n") );
        return WHD_TIMEOUT;
    }
    CHECK_RETURN(result);

    /* Set the ALP */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_CHIP_CLOCK_CSR, (uint8_t)1,
                                              (uint32_t)(SBSDIO_FORCE_HW_CLKREQ_OFF | SBSDIO_ALP_AVAIL_REQ |
                                                         SBSDIO_FORCE_ALP) ) );
    loop_count = 0;
    while ( ( (result = whd_bus_read_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_CHIP_CLOCK_CSR, (uint8_t)1,
                                                    &byte_data) ) != WHD_SUCCESS ) ||
            ( ( (byte_data & SBSDIO_ALP_AVAIL) == 0 ) &&
              (loop_count < (uint32_t)ALP_AVAIL_TIMEOUT_MS) ) )
    {
        (void)cy_rtos_delay_milliseconds( (uint32_t)1 );   /* Ignore return - nothing can be done if it fails */
        loop_count++;
    }
    if (loop_count >= (uint32_t)ALP_AVAIL_TIMEOUT_MS)
    {
        WPRINT_WHD_ERROR( ("Timeout while waiting for alp clock\n") );
        return WHD_TIMEOUT;
    }
    CHECK_RETURN(result);

    /* Clear request for ALP */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_CHIP_CLOCK_CSR, (uint8_t)1, 0) );

    /* Disable the extra SDIO pull-ups */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BACKPLANE_FUNCTION, SDIO_PULL_UP, (uint8_t)1, 0) );

    /* Enable F1 and F2 */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_IOEN, (uint8_t)1,
                                              SDIO_FUNC_ENABLE_1 | SDIO_FUNC_ENABLE_2) );

    /* Setup host-wake signals */
    CHECK_RETURN(whd_bus_sdio_init_oob_intr(whd_driver) );

    /* Enable F2 interrupt only */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_INTEN, (uint8_t)1,
                                              INTR_CTL_MASTER_EN | INTR_CTL_FUNC2_EN) );

    CHECK_RETURN(whd_bus_read_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_IORDY, (uint8_t)1, &byte_data) );

    result = whd_bus_sdio_download_firmware(whd_driver);

    if (result != WHD_SUCCESS)
    {
        /*  either an error or user abort */
        WPRINT_WHD_DEBUG( ("FW download failed\n") );
        return result;
    }

    /* Wait for F2 to be ready */
    loop_count = 0;
    while ( ( (result = whd_bus_read_register_value(whd_driver, BUS_FUNCTION, SDIOD_CCCR_IORDY, (uint8_t)1,
                                                    &byte_data) ) != WHD_SUCCESS ) ||
            ( ( (byte_data & SDIO_FUNC_READY_2) == 0 ) &&
              (loop_count < (uint32_t)F2_READY_TIMEOUT_MS) ) )
    {
        (void)cy_rtos_delay_milliseconds( (uint32_t)1 );   /* Ignore return - nothing can be done if it fails */
        loop_count++;
    }

    if (loop_count >= (uint32_t)F2_READY_TIMEOUT_MS)
    {
        WPRINT_WHD_DEBUG( ("Timeout while waiting for function 2 to be ready\n") );

        if (WHD_TRUE == wake_from_firmware)
        {
            /* If your system fails here, it could be due to incorrect NVRAM variables.
             * Check which 'wifi_nvram_image.h' file your platform is using, and
             * check that it matches the WLAN device on your platform, including the
             * crystal frequency.
             */
            WPRINT_WHD_ERROR( ("F2 failed on wake fr FW\n") );
            /* Reachable after hitting assert */
            return WHD_TIMEOUT;
        }
        /* Else: Ignore this failure if we're doing a reinit due to host wake: Linux DHD also ignores */

    }

    /* Do chip specific init */
    CHECK_RETURN(whd_chip_specific_init(whd_driver) );

    /* Ensure Bus is up */
    CHECK_RETURN(whd_ensure_wlan_bus_is_up(whd_driver) );

    /* Allow bus to go to  sleep */
    CHECK_RETURN(whd_allow_wlan_bus_to_sleep(whd_driver) );

    WPRINT_WHD_INFO( ("whd_bus_reinit Completed \n") );
    return WHD_SUCCESS;
}

uint8_t whd_bus_sdio_backplane_read_padd_size(whd_driver_t whd_driver)
{
    return WHD_BUS_SDIO_BACKPLANE_READ_PADD_SIZE;
}

whd_bool_t whd_bus_sdio_use_status_report_scheme(whd_driver_t whd_driver)
{
    return WHD_FALSE;
}

uint32_t whd_bus_sdio_get_max_transfer_size(whd_driver_t whd_driver)
{
    return WHD_BUS_SDIO_MAX_BACKPLANE_TRANSFER_SIZE;
}

static void whd_bus_sdio_irq_handler(void *handler_arg, cyhal_sdio_irq_event_t event)
{
    whd_driver_t whd_driver = (whd_driver_t)handler_arg;

    /* WHD registered only for CY_CYHAL_SDIO_CARD_INTERRUPT */
    if (event != CYHAL_SDIO_CARD_INTERRUPT)
    {
        WPRINT_WHD_ERROR( ("Unexpected interrupt event %d\n", event) );
        WHD_BUS_STATS_INCREMENT_VARIABLE(whd_driver->bus_priv, error_intrs);
        return;
    }

    WHD_BUS_STATS_INCREMENT_VARIABLE(whd_driver->bus_priv, sdio_intrs);

    /* call thread notify to wake up WHD thread */
    whd_thread_notify_irq(whd_driver);
}

whd_result_t whd_bus_sdio_irq_register(whd_driver_t whd_driver)
{
    cyhal_sdio_register_irq(whd_driver->bus_priv->sdio_obj, whd_bus_sdio_irq_handler, whd_driver);
    return WHD_SUCCESS;
}

whd_result_t whd_bus_sdio_irq_enable(whd_driver_t whd_driver, whd_bool_t enable)
{
    cyhal_sdio_irq_enable(whd_driver->bus_priv->sdio_obj, CYHAL_SDIO_CARD_INTERRUPT, enable);
    return WHD_SUCCESS;
}

static void whd_bus_sdio_oob_irq_handler(void *arg, cyhal_gpio_irq_event_t event)
{
    whd_driver_t whd_driver = (whd_driver_t)arg;
    const whd_oob_config_t *config = &whd_driver->bus_priv->sdio_config.oob_config;
    const cyhal_gpio_irq_event_t expected_event = (config->is_falling_edge == WHD_TRUE)
                                                  ? CYHAL_GPIO_IRQ_FALL : CYHAL_GPIO_IRQ_RISE;

    if (event != expected_event)
    {
        WPRINT_WHD_ERROR( ("Unexpected interrupt event %d\n", event) );
        WHD_BUS_STATS_INCREMENT_VARIABLE(whd_driver->bus_priv, error_intrs);
        return;
    }

    WHD_BUS_STATS_INCREMENT_VARIABLE(whd_driver->bus_priv, oob_intrs);

    /* Call thread notify to wake up WHD thread */
    whd_thread_notify_irq(whd_driver);
}

static whd_result_t whd_bus_sdio_register_oob_intr(whd_driver_t whd_driver)
{
    const whd_oob_config_t *config = &whd_driver->bus_priv->sdio_config.oob_config;

    cyhal_gpio_init(config->host_oob_pin, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, 0);
    cyhal_gpio_register_irq(config->host_oob_pin, config->intr_priority, whd_bus_sdio_oob_irq_handler,
                            whd_driver);

    return WHD_SUCCESS;
}

static whd_result_t whd_bus_sdio_unregister_oob_intr(whd_driver_t whd_driver)
{
    const whd_oob_config_t *config = &whd_driver->bus_priv->sdio_config.oob_config;

    cyhal_gpio_register_irq(config->host_oob_pin, config->intr_priority, NULL, NULL);

    return WHD_SUCCESS;
}

static whd_result_t whd_bus_sdio_enable_oob_intr(whd_driver_t whd_driver, whd_bool_t enable)
{
    const whd_oob_config_t *config = &whd_driver->bus_priv->sdio_config.oob_config;
    const cyhal_gpio_irq_event_t event =
        (config->is_falling_edge == WHD_TRUE) ? CYHAL_GPIO_IRQ_FALL : CYHAL_GPIO_IRQ_RISE;

    cyhal_gpio_irq_enable(config->host_oob_pin, event, (enable == WHD_TRUE) ? true : false);

    return WHD_SUCCESS;
}

static whd_result_t whd_bus_sdio_init_oob_intr(whd_driver_t whd_driver)
{
    const whd_oob_config_t *config = &whd_driver->bus_priv->sdio_config.oob_config;
    uint8_t sepintpol;

    /* OOB isn't configured so bail */
    if (config->host_oob_pin == CYHAL_NC_PIN_VALUE)
        return WHD_SUCCESS;

    /* Choose out-of-band interrupt polarity */
    if (config->is_falling_edge == WHD_FALSE)
    {
        sepintpol = SEP_INTR_CTL_POL;
    }
    else
    {
        sepintpol = 0;
    }

    /* Set OOB interrupt to the correct WLAN GPIO pin (default to GPIO0) */
    if (config->dev_gpio_sel)
        CHECK_RETURN(whd_bus_sdio_set_oob_interrupt(whd_driver, config->dev_gpio_sel) );

    /* Enable out-of-band interrupt on the device */
    CHECK_RETURN(whd_bus_write_register_value(whd_driver, BUS_FUNCTION, SDIOD_SEP_INT_CTL, (uint8_t)1,
                                              SEP_INTR_CTL_MASK | SEP_INTR_CTL_EN | sepintpol) );

    /* Register and enable OOB */
    /* XXX Remove this when BSP377 is implemented */
    CHECK_RETURN(whd_bus_sdio_register_oob_intr(whd_driver) );
    CHECK_RETURN(whd_bus_sdio_enable_oob_intr(whd_driver, WHD_TRUE) );

    return WHD_SUCCESS;
}

static whd_result_t whd_bus_sdio_deinit_oob_intr(whd_driver_t whd_driver)
{
    const whd_oob_config_t *config = &whd_driver->bus_priv->sdio_config.oob_config;

    if (config->host_oob_pin != CYHAL_NC_PIN_VALUE)
    {
        CHECK_RETURN(whd_bus_sdio_enable_oob_intr(whd_driver, WHD_FALSE) );
        CHECK_RETURN(whd_bus_sdio_unregister_oob_intr(whd_driver) );
    }

    return WHD_SUCCESS;
}

