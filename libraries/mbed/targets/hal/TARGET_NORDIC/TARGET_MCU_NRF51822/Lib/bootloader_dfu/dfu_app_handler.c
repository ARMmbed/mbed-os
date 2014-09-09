/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
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

#include "dfu_app_handler.h"
#include "bootloader_util.h"
#include "nrf_sdm.h"
#include "app_error.h"

#define IRQ_ENABLED             0x01                                            /**< Field identifying if an interrupt is enabled. */
#define MAX_NUMBER_INTERRUPTS   32                                              /**< Maximum number of interrupts available. */

static void                     dfu_app_reset_prepare(void);                    /**< Forward declare of default reset handler. */
static dfu_app_reset_prepare_t  m_reset_prepare = dfu_app_reset_prepare;        /**< Callback function to application to prepare for system reset. Allows application to cleanup of service and memory prior to reset. */


/**@brief Default reset prepare handler if application hasn't registered a handler.
 */
static void dfu_app_reset_prepare(void)
{
    // Reset prepare should be handled by application.
    // This function can be extended to include default handling if application does not implement
    // own handler.
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


/**@brief Function for preparing the reset, disabling SoftDevice and jump to the bootloader.
 */
void bootloader_start(void)
{
    m_reset_prepare();

    uint32_t err_code = sd_power_gpregret_set(BOOTLOADER_DFU_START);
    APP_ERROR_CHECK(err_code);

    err_code = sd_softdevice_disable();
    APP_ERROR_CHECK(err_code);

    interrupts_disable();

    err_code = sd_softdevice_vector_table_base_set(NRF_UICR->BOOTLOADERADDR);
    APP_ERROR_CHECK(err_code);

    bootloader_util_app_start(NRF_UICR->BOOTLOADERADDR);
}



void dfu_app_reset_prepare_set(dfu_app_reset_prepare_t reset_prepare_func)
{
    m_reset_prepare = reset_prepare_func;
}
