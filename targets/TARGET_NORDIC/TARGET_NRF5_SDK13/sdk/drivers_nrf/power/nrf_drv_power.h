/* Copyright (c) Nordic Semiconductor. All Rights Reserved.
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
#ifndef NRF_DRV_POWER_H__
#define NRF_DRV_POWER_H__

#include <stdbool.h>
#include <stdint.h>
#include "nrf_power.h"
#include "sdk_config.h"
#include "nrf_drv_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_power Power HAL and driver
 * @ingroup nrf_drivers
 * @brief POWER peripheral APIs.
 *
 * The power peripheral HAL provides basic APIs for accessing
 * the registers of the POWER peripheral.
 * The POWER driver provides APIs on a higher level.
 */

/**
 * @defgroup nrf_drv_power POWER driver
 * @{
 * @ingroup nrf_power
 * @brief Driver for managing events and the state of POWER peripheral.
 *
 */

/**
 * @brief Power mode possible configurations
 */
typedef enum
{
    NRF_DRV_POWER_MODE_CONSTLAT, /**< Constant latency mode *///!< NRF_DRV_POWER_MODE_CONSTLAT
    NRF_DRV_POWER_MODE_LOWPWR    /**< Low power mode        *///!< NRF_DRV_POWER_MODE_LOWPWR
}nrf_drv_power_mode_t;

#if NRF_POWER_HAS_SLEEPEVT
/**
 * @brief Events from power system
 */
typedef enum
{
    NRF_DRV_POWER_SLEEP_EVT_ENTER, /**< CPU entered WFI/WFE sleep
                                    *
                                    * Keep in mind that if this interrupt is enabled,
                                    * it means that CPU was waken up just after WFI by this interrupt.
                                    */
    NRF_DRV_POWER_SLEEP_EVT_EXIT   /**< CPU exited WFI/WFE sleep */
}nrf_drv_power_sleep_evt_t;
#endif /* NRF_POWER_HAS_SLEEPEVT */

#if NRF_POWER_HAS_USBREG
/**
 * @brief Events from USB power system
 */
typedef enum
{
    NRF_DRV_POWER_USB_EVT_DETECTED, /**< USB power detected on the connector (plugged in). */
    NRF_DRV_POWER_USB_EVT_REMOVED,  /**< USB power removed from the connector. */
    NRF_DRV_POWER_USB_EVT_READY     /**< USB power regulator ready. */
}nrf_drv_power_usb_evt_t;

/**
 * @brief USB power state
 *
 * The single enumerator that holds all data about current state of USB
 * related POWER.
 *
 * Organized this way that higher power state has higher numeric value
 */
typedef enum
{
    NRF_DRV_POWER_USB_STATE_DISCONNECTED, /**< No power on USB lines detected */
    NRF_DRV_POWER_USB_STATE_CONNECTED,    /**< The USB power is detected, but USB power regulator is not ready */
    NRF_DRV_POWER_USB_STATE_READY         /**< From the power point of view USB is ready for working */
}nrf_drv_power_usb_state_t;
#endif /* NRF_POWER_HAS_USBREG */

/**
 * @name Callback types
 *
 * Defined types of callback functions
 * @{
 */
/**
 * @brief Event handler for power failure warning
 */
typedef void (*nrf_drv_power_pofwarn_event_handler_t)(void);

#if NRF_POWER_HAS_SLEEPEVT
/**
 * @brief Event handler for entering/exiting sleep
 *
 * @param event Event type
 */
typedef void (*nrf_drv_power_sleep_event_handler_t)(nrf_drv_power_sleep_evt_t event);
#endif

#if NRF_POWER_HAS_USBREG
/**
 * @brief Event handler for USB related power events
 *
 * @param event Event type
 */
typedef void (*nrf_drv_power_usb_event_handler_t)(nrf_drv_power_usb_evt_t event);
#endif
/** @} */

/**
 * @brief General power configuration
 *
 * Parameters required to initialize power driver.
 */
typedef struct
{
    /**
     * @brief Enable main DCDC regulator
     *
     * This bit only informs the driver that elements for DCDC regulator
     * are installed and regulator can be used.
     * The regulator would be enabled or disabled automatically
     * automatically by the hardware, basing on current power requirement.
     */
    bool dcdcen:1;

#if NRF_POWER_HAS_VDDH
    /**
     * @brief Enable HV DCDC regulator
     *
     * This bit only informs the driver that elements for DCDC regulator
     * are installed and regulator can be used.
     * The regulator would be enabled or disabled automatically
     * automatically by the hardware, basing on current power requirement.
     */
    bool dcdcenhv: 1;
#endif
}nrf_drv_power_config_t;

/**
 * @brief The configuration for power failure comparator
 *
 * Configuration used to enable and configure power failure comparator
 */
typedef struct
{
    nrf_drv_power_pofwarn_event_handler_t handler; //!< Event handler
    nrf_power_pof_thr_t                   thr;     //!< Threshold for power failure detection
#if NRF_POWER_HAS_VDDH
    nrf_power_pof_thrvddh_t               thrvddh; //!< Threshold for power failure detection on VDDH pin
#endif
}nrf_drv_power_pofwarn_config_t;

#if NRF_POWER_HAS_SLEEPEVT
/**
 * @brief The configuration of sleep event processing
 *
 * Configuration used to enable and configure sleep event handling
 */
typedef struct
{
    nrf_drv_power_sleep_event_handler_t handler;    //!< Event handler
    bool                                en_enter:1; //!< Enable event on sleep entering
    bool                                en_exit :1; //!< Enable event on sleep exiting
}nrf_drv_power_sleepevt_config_t;
#endif

#if NRF_POWER_HAS_USBREG
/**
 * @brief The configuration of USB related power events
 *
 * Configuration used to enable and configure USB power event handling
 */
typedef struct
{
    nrf_drv_power_usb_event_handler_t handler; //!< Event processing
}nrf_drv_power_usbevt_config_t;
#endif /* NRF_POWER_HAS_USBREG */

/**
 * @brief Function for checking if driver is already initialized
 *
 * This function is used to check whatever common POWER_CLOCK common interrupt
 * should be disabled or not if @ref nrf_drv_clock tries to disable the interrupt.
 *
 * @retval true  Driver is initialized
 * @retval false Driver is uninitialized
 *
 * @sa nrf_drv_power_uninit
 */
bool nrf_drv_power_init_check(void);

/**
 * @brief Initialize power module driver
 *
 * Enabled power module driver would process all the interrupts from power system.
 *
 * @param[in] p_config Driver configuration. Can be NULL - the default configuration
 *                     from @em sdk_config.h file would be used then.
 *
 * @retval NRF_ERROR_MODULE_ALREADY_INITIALIZED Module is initialized already.
 * @retval NRF_SUCCESS                          Successfully initialized.
 */
ret_code_t nrf_drv_power_init(nrf_drv_power_config_t const * p_config);

/**
 * @brief Unintialize power module driver
 *
 * Disables all the interrupt handling in the module.
 *
 * @sa nrf_drv_power_init
 */
void nrf_drv_power_uninit(void);

/**
 * @brief Initialize power failure comparator
 *
 * Configures and setups the power failure comparator and enables it.
 *
 * @param[in] p_config Configuration with values and event handler.
 *                     If event handler is set to NULL, interrupt would be disabled.
 */
void nrf_drv_power_pof_init(nrf_drv_power_pofwarn_config_t const * p_config);

/**
 * @brief Turn off the power failure comparator
 *
 * Disables and clears the settings of the power failure comparator.
 */
void nrf_drv_power_pof_uninit(void);

#if NRF_POWER_HAS_SLEEPEVT
/**
 * @brief Initialize sleep entering and exiting events processing
 *
 * Configures and setups the sleep event processing.
 *
 * @param[in] p_config Configuration with values and event handler.
 *
 * @sa nrf_drv_power_sleepevt_uninit
 */
void nrf_drv_power_sleepevt_init(nrf_drv_power_sleepevt_config_t const * p_config);

/**
 * @brief Uninitialize sleep entering and exiting events processing
 *
 * @sa nrf_drv_power_sleepevt_init
 */
void nrf_drv_power_sleepevt_uninit(void);
#endif /* NRF_POWER_HAS_SLEEPEVT */

#if NRF_POWER_HAS_USBREG
/**
 * @brief Initialize USB power event processing
 *
 * Configures and setups the USB power event processing.
 *
 * @param[in] p_config Configuration with values and event handler.
 *
 * @sa nrf_drv_power_usbevt_uninit
 */
void nrf_drv_power_usbevt_init(nrf_drv_power_usbevt_config_t const * p_config);

/**
 * @brief Uninitalize USB power event processing
 *
 * @sa nrf_drv_power_usbevt_init
 */
void nrf_drv_power_usbevt_uninit(void);

/**
 *  @brief Get the status of USB power
 *
 *  @return Current USB power status
 */
__STATIC_INLINE nrf_drv_power_usb_state_t nrf_drv_power_usbstatus_get(void);

#endif /* NRF_POWER_HAS_USBREG */

/** @} */

#ifndef SUPPRESS_INLINE_IMPLEMENTATION

#if NRF_POWER_HAS_USBREG
__STATIC_INLINE nrf_drv_power_usb_state_t nrf_drv_power_usbstatus_get(void)
{
    uint32_t status = nrf_power_usbregstatus_get();
    if(0 == (status & NRF_POWER_USBREGSTATUS_VBUSDETECT_MASK))
    {
        return NRF_DRV_POWER_USB_STATE_DISCONNECTED;
    }
    if(0 == (status & NRF_POWER_USBREGSTATUS_OUTPUTRDY_MASK))
    {
        return NRF_DRV_POWER_USB_STATE_CONNECTED;
    }
    return NRF_DRV_POWER_USB_STATE_READY;
}
#endif /* NRF_POWER_HAS_USBREG */

#endif /* SUPPRESS_INLINE_IMPLEMENTATION */


#ifdef __cplusplus
}
#endif

#endif /* NRF_DRV_POWER_H__ */
