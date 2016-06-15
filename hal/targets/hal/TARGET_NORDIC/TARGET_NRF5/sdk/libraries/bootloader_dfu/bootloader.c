/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "bootloader.h"
#include "bootloader_types.h"
#include "bootloader_util.h"
#include "bootloader_settings.h"
#include "dfu.h"
#include "dfu_transport.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf_sdm.h"
#include "nrf_mbr.h"
#include "nordic_common.h"
#include "crc16.h"
#include "pstorage.h"
#include "app_scheduler.h"
#include "nrf_delay.h"
#include "sdk_common.h"

#define IRQ_ENABLED             0x01                    /**< Field identifying if an interrupt is enabled. */
#define MAX_NUMBER_INTERRUPTS   32                      /**< Maximum number of interrupts available. */

/**@brief Enumeration for specifying current bootloader status.
 */
typedef enum
{
    BOOTLOADER_UPDATING,                                /**< Bootloader status for indicating that an update is in progress. */
    BOOTLOADER_SETTINGS_SAVING,                         /**< Bootloader status for indicating that saving of bootloader settings is in progress. */
    BOOTLOADER_COMPLETE,                                /**< Bootloader status for indicating that all operations for the update procedure has completed and it is safe to reset the system. */
    BOOTLOADER_TIMEOUT,                                 /**< Bootloader status field for indicating that a timeout has occured and current update process should be aborted. */
    BOOTLOADER_RESET,                                   /**< Bootloader status field for indicating that a reset has been requested and current update process should be aborted. */
} bootloader_status_t;

static pstorage_handle_t        m_bootsettings_handle;  /**< Pstorage handle to use for registration and identifying the bootloader module on subsequent calls to the pstorage module for load and store of bootloader setting in flash. */
static bootloader_status_t      m_update_status;        /**< Current update status for the bootloader module to ensure correct behaviour when updating settings and when update completes. */

/**@brief   Function for handling callbacks from pstorage module.
 *
 * @details Handles pstorage results for clear and storage operation. For detailed description of
 *          the parameters provided with the callback, please refer to \ref pstorage_ntf_cb_t.
 */
static void pstorage_callback_handler(pstorage_handle_t * p_handle,
                                      uint8_t             op_code,
                                      uint32_t            result,
                                      uint8_t           * p_data,
                                      uint32_t            data_len)
{
    // If we are in BOOTLOADER_SETTINGS_SAVING state and we receive an PSTORAGE_STORE_OP_CODE
    // response then settings has been saved and update has completed.
    if ((m_update_status == BOOTLOADER_SETTINGS_SAVING) && (op_code == PSTORAGE_STORE_OP_CODE))
    {
        m_update_status = BOOTLOADER_COMPLETE;
    }

    APP_ERROR_CHECK(result);
}


/**@brief   Function for waiting for events.
 *
 * @details This function will place the chip in low power mode while waiting for events from
 *          the SoftDevice or other peripherals. When interrupted by an event, it will call the
 *          @ref app_sched_execute function to process the received event. This function will return
 *          when the final state of the firmware update is reached OR when a tear down is in
 *          progress.
 */
static void wait_for_events(void)
{
    for (;;)
    {
        // Wait in low power state for any events.
        uint32_t err_code = sd_app_evt_wait();
        APP_ERROR_CHECK(err_code);

        // Event received. Process it from the scheduler.
        app_sched_execute();

        if ((m_update_status == BOOTLOADER_COMPLETE) ||
            (m_update_status == BOOTLOADER_TIMEOUT)  ||
            (m_update_status == BOOTLOADER_RESET))
        {
            // When update has completed or a timeout/reset occured we will return.
            return;
        }
    }
}


bool bootloader_app_is_valid(uint32_t app_addr)
{
    const bootloader_settings_t * p_bootloader_settings;

    // There exists an application in CODE region 1.
    if (*((uint32_t *)app_addr) == EMPTY_FLASH_MASK)
    {
        return false;
    }

    bool success = false;

    bootloader_util_settings_get(&p_bootloader_settings);

    // The application in CODE region 1 is flagged as valid during update.
    if (p_bootloader_settings->bank_0 == BANK_VALID_APP)
    {
        uint16_t image_crc = 0;

        // A stored crc value of 0 indicates that CRC checking is not used.
        if (p_bootloader_settings->bank_0_crc != 0)
        {
            image_crc = crc16_compute((uint8_t *)DFU_BANK_0_REGION_START,
                                      p_bootloader_settings->bank_0_size,
                                      NULL);
        }

        success = (image_crc == p_bootloader_settings->bank_0_crc);
    }

    return success;
}


static void bootloader_settings_save(bootloader_settings_t * p_settings)
{
    uint32_t err_code = pstorage_clear(&m_bootsettings_handle, sizeof(bootloader_settings_t));
    APP_ERROR_CHECK(err_code);

    err_code = pstorage_store(&m_bootsettings_handle,
                              (uint8_t *)p_settings,
                              sizeof(bootloader_settings_t),
                              0);
    APP_ERROR_CHECK(err_code);
}


void bootloader_dfu_update_process(dfu_update_status_t update_status)
{
    static bootloader_settings_t  settings;
    const bootloader_settings_t * p_bootloader_settings;

    bootloader_util_settings_get(&p_bootloader_settings);

    if (update_status.status_code == DFU_UPDATE_APP_COMPLETE)
    {
        settings.bank_0_crc  = update_status.app_crc;
        settings.bank_0_size = update_status.app_size;
        settings.bank_0      = BANK_VALID_APP;
        settings.bank_1      = BANK_INVALID_APP;

        m_update_status      = BOOTLOADER_SETTINGS_SAVING;
        bootloader_settings_save(&settings);
    }
    else if (update_status.status_code == DFU_UPDATE_SD_COMPLETE)
    {
        settings.bank_0_crc     = update_status.app_crc;
        settings.bank_0_size    = update_status.sd_size + 
                                  update_status.bl_size + 
                                  update_status.app_size;
        settings.bank_0         = BANK_VALID_SD;
        settings.bank_1         = BANK_INVALID_APP;
        settings.sd_image_size  = update_status.sd_size;
        settings.bl_image_size  = update_status.bl_size;
        settings.app_image_size = update_status.app_size;
        settings.sd_image_start = update_status.sd_image_start;

        m_update_status         = BOOTLOADER_SETTINGS_SAVING;
        bootloader_settings_save(&settings);
    }
    else if (update_status.status_code == DFU_UPDATE_BOOT_COMPLETE)
    {
        settings.bank_0         = p_bootloader_settings->bank_0;
        settings.bank_0_crc     = p_bootloader_settings->bank_0_crc;
        settings.bank_0_size    = p_bootloader_settings->bank_0_size;
        settings.bank_1         = BANK_VALID_BOOT;
        settings.sd_image_size  = update_status.sd_size;
        settings.bl_image_size  = update_status.bl_size;
        settings.app_image_size = update_status.app_size;

        m_update_status         = BOOTLOADER_SETTINGS_SAVING;
        bootloader_settings_save(&settings);
    }
    else if (update_status.status_code == DFU_UPDATE_SD_SWAPPED)
    {
        if (p_bootloader_settings->bank_0 == BANK_VALID_SD)
        {
            settings.bank_0_crc     = 0;
            settings.bank_0_size    = 0;
            settings.bank_0         = BANK_INVALID_APP;
        }
        // This handles cases where SoftDevice was not updated, hence bank0 keeps its settings.
        else
        {
            settings.bank_0         = p_bootloader_settings->bank_0;
            settings.bank_0_crc     = p_bootloader_settings->bank_0_crc;
            settings.bank_0_size    = p_bootloader_settings->bank_0_size;
        }

        settings.bank_1         = BANK_INVALID_APP;
        settings.sd_image_size  = 0;
        settings.bl_image_size  = 0;
        settings.app_image_size = 0;

        m_update_status         = BOOTLOADER_SETTINGS_SAVING;
        bootloader_settings_save(&settings);
    }
    else if (update_status.status_code == DFU_TIMEOUT)
    {
        // Timeout has occurred. Close the connection with the DFU Controller.
        uint32_t err_code = dfu_transport_close();
        APP_ERROR_CHECK(err_code);

        m_update_status = BOOTLOADER_TIMEOUT;
    }
    else if (update_status.status_code == DFU_BANK_0_ERASED)
    {
        settings.bank_0_crc  = 0;
        settings.bank_0_size = 0;
        settings.bank_0      = BANK_INVALID_APP;
        settings.bank_1      = p_bootloader_settings->bank_1;

        bootloader_settings_save(&settings);
    }
    else if (update_status.status_code == DFU_RESET)
    {
        m_update_status = BOOTLOADER_RESET;
    }
    else
    {
        // No implementation needed.
    }
}


uint32_t bootloader_init(void)
{
    uint32_t                err_code;
    pstorage_module_param_t storage_params = {.cb = pstorage_callback_handler};

    err_code = pstorage_init();
    VERIFY_SUCCESS(err_code);

    m_bootsettings_handle.block_id = BOOTLOADER_SETTINGS_ADDRESS;
    err_code = pstorage_register(&storage_params, &m_bootsettings_handle);

    return err_code;
}


uint32_t bootloader_dfu_start(void)
{
    uint32_t err_code;

    // Clear swap if banked update is used.
    err_code = dfu_init(); 
    VERIFY_SUCCESS(err_code);

    err_code = dfu_transport_update_start();

    wait_for_events();

    return err_code;
}


/**@brief Function for disabling all interrupts before jumping from bootloader to application.
 */
static void interrupts_disable(void)
{
    uint32_t interrupt_setting_mask;
    uint32_t irq = 0; // We start from first interrupt, i.e. interrupt 0.

    // Fetch the current interrupt settings.
    interrupt_setting_mask = NVIC->ISER[0];

    for (; irq < MAX_NUMBER_INTERRUPTS; irq++)
    {
        if (interrupt_setting_mask & (IRQ_ENABLED << irq))
        {
            // The interrupt was enabled, and hence disable it.
            NVIC_DisableIRQ((IRQn_Type)irq);
        }
    }
}


void bootloader_app_start(uint32_t app_addr)
{
    // If the applications CRC has been checked and passed, the magic number will be written and we
    // can start the application safely.
    uint32_t err_code = sd_softdevice_disable();
    APP_ERROR_CHECK(err_code);

    interrupts_disable();

    err_code = sd_softdevice_vector_table_base_set(CODE_REGION_1_START);
    APP_ERROR_CHECK(err_code);

    bootloader_util_app_start(CODE_REGION_1_START);
}


bool bootloader_dfu_sd_in_progress(void)
{
    const bootloader_settings_t * p_bootloader_settings;

    bootloader_util_settings_get(&p_bootloader_settings);

    if (p_bootloader_settings->bank_0 == BANK_VALID_SD ||
        p_bootloader_settings->bank_1 == BANK_VALID_BOOT)
    {
        return true;
    }

    return false;
}


uint32_t bootloader_dfu_sd_update_continue(void)
{
    uint32_t err_code;

    if ((dfu_sd_image_validate() == NRF_SUCCESS) &&
        (dfu_bl_image_validate() == NRF_SUCCESS))
    {
        return NRF_SUCCESS;
    }

    // Ensure that flash operations are not executed within the first 100 ms seconds to allow
    // a debugger to be attached.
    nrf_delay_ms(100);

    err_code = dfu_sd_image_swap();
    APP_ERROR_CHECK(err_code);

    err_code = dfu_sd_image_validate();
    APP_ERROR_CHECK(err_code);

    err_code = dfu_bl_image_swap();
    APP_ERROR_CHECK(err_code);

    return err_code;
}


uint32_t bootloader_dfu_sd_update_finalize(void)
{
    dfu_update_status_t update_status = {DFU_UPDATE_SD_SWAPPED, };

    bootloader_dfu_update_process(update_status);

    wait_for_events();

    return NRF_SUCCESS;
}


void bootloader_settings_get(bootloader_settings_t * const p_settings)
{
    const bootloader_settings_t *  p_bootloader_settings;

    bootloader_util_settings_get(&p_bootloader_settings);

    p_settings->bank_0         = p_bootloader_settings->bank_0;
    p_settings->bank_0_crc     = p_bootloader_settings->bank_0_crc;
    p_settings->bank_0_size    = p_bootloader_settings->bank_0_size;
    p_settings->bank_1         = p_bootloader_settings->bank_1;
    p_settings->sd_image_size  = p_bootloader_settings->sd_image_size;
    p_settings->bl_image_size  = p_bootloader_settings->bl_image_size;
    p_settings->app_image_size = p_bootloader_settings->app_image_size;
    p_settings->sd_image_start = p_bootloader_settings->sd_image_start;
}

