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

/** @file
 *
 * @defgroup app_gpiote GPIOTE Handler
 * @{
 * @ingroup app_common
 *
 * @brief GPIOTE handler module.
 *
 * @details The GPIOTE handler allows several modules ("users") to share the GPIOTE interrupt,
 *          each user defining a set of pins able to generate events to the user.
 *          When a GPIOTE interrupt occurs, the GPIOTE interrupt handler will call the event handler
 *          of each user for which at least one of the pins generated an event.
 *
 *          The GPIOTE users are responsible for configuring all their corresponding pins, except
 *          the SENSE field, which should be initialized to GPIO_PIN_CNF_SENSE_Disabled.
 *          The SENSE field will be updated by the GPIOTE module when it is enabled or disabled,
 *          and also while it is enabled.
 *
 *          The module specifies on which pins events should be generated if the pin(s) goes
 *          from low->high or high->low or both directions.
 *
 * @note    Even if the application is using the @ref app_scheduler, the GPIOTE event handlers will
 *          be called directly from the GPIOTE interrupt handler.
 *
 * @warning If multiple users registers for the same pins the behavior for those pins are undefined.
 */

#ifndef APP_GPIOTE_H__
#define APP_GPIOTE_H__

#include <stdint.h>
#include <stdbool.h>
// #include "nrf.h"
#include "app_error.h"
#include "app_util.h"

#ifdef __cpluplus
extern "C" {
#endif

#define GPIOTE_USER_NODE_SIZE   20          /**< Size of app_gpiote.gpiote_user_t (only for use inside APP_GPIOTE_BUF_SIZE()). */
#define NO_OF_PINS              32          /**< Number of GPIO pins on the nRF51 chip. */

/**@brief Compute number of bytes required to hold the GPIOTE data structures.
 *
 * @param[in]   MAX_USERS   Maximum number of GPIOTE users.
 *
 * @return      Required buffer size (in bytes).
 */
#define APP_GPIOTE_BUF_SIZE(MAX_USERS)  ((MAX_USERS) * GPIOTE_USER_NODE_SIZE)

typedef uint8_t app_gpiote_user_id_t;

/**@brief GPIOTE event handler type. */
typedef void (*app_gpiote_event_handler_t)(uint32_t event_pins_low_to_high,
                                           uint32_t event_pins_high_to_low);

/**@brief GPIOTE input event handler type. */
typedef void (*app_gpiote_input_event_handler_t)(void);

/**@brief Macro for initializing the GPIOTE module.
 *
 * @details It will handle dimensioning and allocation of the memory buffer required by the module,
 *          making sure that the buffer is correctly aligned.
 *
 * @param[in]   MAX_USERS   Maximum number of GPIOTE users.
 *
 * @note Since this macro allocates a buffer, it must only be called once (it is OK to call it
 *       several times as long as it is from the same location, e.g. to do a reinitialization).
 */
/*lint -emacro(506, APP_GPIOTE_INIT) */ /* Suppress "Constant value Boolean */
#define APP_GPIOTE_INIT(MAX_USERS)                                                                 \
    do                                                                                             \
    {                                                                                              \
        static uint32_t app_gpiote_buf[CEIL_DIV(APP_GPIOTE_BUF_SIZE(MAX_USERS), sizeof(uint32_t))];\
        uint32_t ERR_CODE = app_gpiote_init((MAX_USERS), app_gpiote_buf);                          \
        APP_ERROR_CHECK(ERR_CODE);                                                                 \
    } while (0)

/**@brief Function for initializing the GPIOTE module.
 *
 * @note Normally initialization should be done using the APP_GPIOTE_INIT() macro, as that will
 *       allocate the buffer needed by the GPIOTE module (including aligning the buffer correctly).
 *
 * @param[in]   max_users               Maximum number of GPIOTE users.
 * @param[in]   p_buffer                Pointer to memory buffer for internal use in the app_gpiote
 *                                      module. The size of the buffer can be computed using the
 *                                      APP_GPIOTE_BUF_SIZE() macro. The buffer must be aligned to
 *                                      a 4 byte boundary.
 *
 * @retval      NRF_SUCCESS             Successful initialization.
 * @retval      NRF_ERROR_INVALID_PARAM Invalid parameter (buffer not aligned to a 4 byte
 *                                      boundary).
 */
uint32_t app_gpiote_init(uint8_t max_users, void * p_buffer);

/**@brief Function for registering a GPIOTE user.
 *
 * @param[out]  p_user_id               Id for the new GPIOTE user.
 * @param[in]   pins_low_to_high_mask   Mask defining which pins will generate events to this user
 *                                      when state is changed from low->high.
 * @param[in]   pins_high_to_low_mask   Mask defining which pins will generate events to this user
 *                                      when state is changed from high->low.
 * @param[in]   event_handler           Pointer to function to be executed when an event occurs.
 *
 * @retval      NRF_SUCCESS             Successful initialization.
 * @retval      NRF_ERROR_INVALID_PARAM Invalid parameter (buffer not aligned to a 4 byte boundary).
 * @retval      NRF_ERROR_INALID_STATE  If @ref app_gpiote_init has not been called on the GPIOTE
 *                                      module.
 * @retval      NRF_ERROR_NO_MEM        Returned if the application tries to register more users
 *                                      than defined when the GPIOTE module was initialized in
 *                                      @ref app_gpiote_init.
 */
uint32_t app_gpiote_user_register(app_gpiote_user_id_t *     p_user_id,
                                  uint32_t                   pins_low_to_high_mask,
                                  uint32_t                   pins_high_to_low_mask,
                                  app_gpiote_event_handler_t event_handler);

/**@brief Function for informing the GPIOTE module that the specified user wants to use the GPIOTE module.
 *
 * @param[in]   user_id                 Id of user to enable.
 *
 * @retval      NRF_SUCCESS             On success.
 * @retval      NRF_ERROR_INVALID_PARAM Invalid user_id provided, No a valid user.
 * @retval      NRF_ERROR_INALID_STATE  If @ref app_gpiote_init has not been called on the GPIOTE
 *                                      module.
 */
uint32_t app_gpiote_user_enable(app_gpiote_user_id_t user_id);

/**@brief Function for informing the GPIOTE module that the specified user is done using the GPIOTE module.
 *
 * @param[in]   user_id                   Id of user to enable.
 *
 * @return      NRF_SUCCESS               On success.
 * @retval      NRF_ERROR_INVALID_PARAM   Invalid user_id provided, No a valid user.
 * @retval      NRF_ERROR_INALID_STATE    If @ref app_gpiote_init has not been called on the GPIOTE
 *                                        module.
 */
uint32_t app_gpiote_user_disable(app_gpiote_user_id_t user_id);

/**@brief Function for getting the state of the pins which are registered for the specified user.
 *
 * @param[in]   user_id         Id of user to check.
 * @param[out]  p_pins          Bit mask corresponding to the pins configured to generate events to
 *                              the specified user. All bits corresponding to pins in the state
 *                              'high' will have value '1', all others will have value '0'.
 *
 * @return      NRF_SUCCESS               On success.
 * @retval      NRF_ERROR_INVALID_PARAM   Invalid user_id provided, No a valid user.
 * @retval      NRF_ERROR_INALID_STATE    If @ref app_gpiote_init has not been called on the GPIOTE
 *                                        module.
 */
uint32_t app_gpiote_pins_state_get(app_gpiote_user_id_t user_id, uint32_t * p_pins);

/**@brief Function for registering event handlers for GPIOTE IN events.
 *
 * @param[in] channel         GPIOTE channel [0..3].
 * @param[in] pin             Pins associated with GPIOTE channel. Changes on following pins will generate events.
 * @param[in] polarity        Specify operation on input that shall trigger IN event.
 * @param[in] event_handler   Event handler invoked on the IN event in the GPIOTE interrupt.
 *
 * @return   NRF_SUCCESS                 On success.
 * @retval   NRF_ERROR_INVALID_PARAM     Invalid channel or pin number.
 * @retval   NRF_ERROR_NOT_SUPPORTED     Driver doesn't support IN events.
 */
uint32_t app_gpiote_input_event_handler_register(const uint8_t channel,
                                                 const uint32_t pin,
                                                 const uint32_t polarity,
                                                 app_gpiote_input_event_handler_t event_handler);

/**@brief Function for unregistering event handlers for GPIOTE IN events.
 *
 * @return   NRF_SUCCESS                 On success.
 * @retval   NRF_ERROR_NOT_SUPPORTED     Driver doesn't support IN events.
 */
uint32_t app_gpiote_input_event_handler_unregister(const uint8_t channel);

/**@brief Function for registering event handler invoked at the end of a GPIOTE interrupt.
 *
 * @param[in] event_handler    Event handler invoked at the end of the GPIOTE interrupt.
 *
 * @return   NRF_SUCCESS                 On success.
 * @retval   NRF_ERROR_NOT_SUPPORTED     Driver doesn't support IN events.
 */
uint32_t app_gpiote_end_irq_event_handler_register(app_gpiote_input_event_handler_t event_handler);

/**@brief Function for unregistering event handler invoked at the end of a GPIOTE interrupt.
 *
 * @return   NRF_SUCCESS                 On success.
 * @retval   NRF_ERROR_NOT_SUPPORTED     Driver doesn't support IN events.
 */
uint32_t app_gpiote_end_irq_event_handler_unregister(void);

/**@brief Function for enabling interrupts in the GPIOTE driver.
 *
 * @return   NRF_SUCCESS                 On success.
 * @retval   NRF_ERROR_NOT_SUPPORTED     Driver doesn't support.
 */
uint32_t app_gpiote_enable_interrupts(void);

/**@brief Function for disabling interrupts in the GPIOTE driver.
 *
 * @return   NRF_SUCCESS                 On success.
 * @retval   NRF_ERROR_NOT_SUPPORTED     Driver doesn't support.
 */
uint32_t app_gpiote_disable_interrupts(void);

#ifdef __cpluplus
}
#endif

#endif // APP_GPIOTE_H__

/** @} */
