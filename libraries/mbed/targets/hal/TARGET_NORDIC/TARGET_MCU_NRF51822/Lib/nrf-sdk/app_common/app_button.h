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
 * @defgroup app_button Button Handler
 * @{
 * @ingroup app_common
 *
 * @brief Buttons handling module.
 *
 * @details The button handler uses the @ref app_gpiote to detect that a button has been
 *          pushed. To handle debouncing, it will start a timer in the GPIOTE event handler.
 *          The button will only be reported as pushed if the corresponding pin is still active when
 *          the timer expires. If there is a new GPIOTE event while the timer is running, the timer
 *          is restarted.
 *          Use the USE_SCHEDULER parameter of the APP_BUTTON_INIT() macro to select if the
 *          @ref app_scheduler is to be used or not.
 *
 * @note    The app_button module uses the app_timer module. The user must ensure that the queue in
 *          app_timer is large enough to hold the app_timer_stop() / app_timer_start() operations
 *          which will be executed on each event from GPIOTE module (2 operations), as well as other
 *          app_timer operations queued simultaneously in the application.
 *
 * @note    Even if the scheduler is not used, app_button.h will include app_scheduler.h, so when
 *          compiling, app_scheduler.h must be available in one of the compiler include paths.
 */

#ifndef APP_BUTTON_H__
#define APP_BUTTON_H__

#include <stdint.h>
#include <stdbool.h>
#include "app_error.h"
#include "app_scheduler.h"
#include "nrf_gpio.h"

#define APP_BUTTON_SCHED_EVT_SIZE sizeof(app_button_event_t)   /**< Size of button events being passed through the scheduler (is to be used for computing the maximum size of scheduler events). */
#define APP_BUTTON_PUSH        1                               /**< Indicates that a button is pushed. */
#define APP_BUTTON_RELEASE     0                               /**< Indicates that a button is released. */
#define APP_BUTTON_ACTIVE_HIGH 1                               /**< Indicates that a button is active high. */
#define APP_BUTTON_ACTIVE_LOW  0                               /**< Indicates that a button is active low. */

/**@brief Button event handler type. */
typedef void (*app_button_handler_t)(uint8_t pin_no, uint8_t button_action);

/**@brief Type of function for passing events from the Button Handler module to the scheduler. */
typedef uint32_t (*app_button_evt_schedule_func_t) (app_button_handler_t button_handler,
                                                    uint8_t              pin_no,
                                                    uint8_t              button_action);

/**@brief Button configuration structure. */
typedef struct
{
    uint8_t              pin_no;           /**< Pin to be used as a button. */
    uint8_t              active_state;     /**< APP_BUTTON_ACTIVE_HIGH or APP_BUTTON_ACTIVE_LOW. */
    nrf_gpio_pin_pull_t  pull_cfg;         /**< Pull-up or -down configuration. */
    app_button_handler_t button_handler;   /**< Handler to be called when button is pushed. */
} app_button_cfg_t;

/**@brief  Pin transition direction struct. */
typedef struct
{
    uint32_t high_to_low;   /**Pin went from high to low */
    uint32_t low_to_high;   /**Pin went from low to high */
} pin_transition_t;

/**@brief Macro for initializing the Button Handler module.
 *
 * @details It will initialize the specified pins as buttons, and configure the Button Handler
 *          module as a GPIOTE user (but it will not enable button detection). It will also connect
 *          the Button Handler module to the scheduler (if specified).
 *
 * @param[in]  BUTTONS           Array of buttons to be used (type app_button_cfg_t, must be
 *                               static!).
 * @param[in]  BUTTON_COUNT      Number of buttons.
 * @param[in]  DETECTION_DELAY   Delay from a GPIOTE event until a button is reported as pushed.
 * @param[in]  USE_SCHEDULER     TRUE if the application is using the event scheduler,
 *                               FALSE otherwise.
 */
/*lint -emacro(506, APP_BUTTON_INIT) */ /* Suppress "Constant value Boolean */
#define APP_BUTTON_INIT(BUTTONS, BUTTON_COUNT, DETECTION_DELAY, USE_SCHEDULER)                     \
    do                                                                                             \
    {                                                                                              \
        uint32_t ERR_CODE = app_button_init((BUTTONS),                                             \
                                            (BUTTON_COUNT),                                        \
                                            (DETECTION_DELAY),                                     \
                                            (USE_SCHEDULER) ? app_button_evt_schedule : NULL);     \
        APP_ERROR_CHECK(ERR_CODE);                                                                 \
    } while (0)

/**@brief Function for initializing the Buttons.
 *
 * @details This function will initialize the specified pins as buttons, and configure the Button
 *          Handler module as a GPIOTE user (but it will not enable button detection).
 *
 * @note Normally initialization should be done using the APP_BUTTON_INIT() macro, as that will take
 *       care of connecting the Buttons module to the scheduler (if specified).
 *
 * @note app_button_enable() function must be called in order to enable the button detection.
 *
 * @param[in]  p_buttons           Array of buttons to be used (NOTE: Must be static!).
 * @param[in]  button_count        Number of buttons.
 * @param[in]  detection_delay     Delay from a GPIOTE event until a button is reported as pushed.
 * @param[in]  evt_schedule_func   Function for passing button events to the scheduler. Point to
 *                                 app_button_evt_schedule() to connect to the scheduler. Set to
 *                                 NULL to make the Buttons module call the event handler directly
 *                                 from the delayed button push detection timeout handler.
 *
 * @return   NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t app_button_init(app_button_cfg_t *             p_buttons,
                         uint8_t                        button_count,
                         uint32_t                       detection_delay,
                         app_button_evt_schedule_func_t evt_schedule_func);

/**@brief Function for enabling button detection.
 *
 * @retval  NRF_ERROR_INVALID_PARAM   GPIOTE has to many users.
 * @retval  NRF_ERROR_INVALID_STATE   Button or GPIOTE not initialized.
 * @retval  NRF_SUCCESS               Button detection successfully enabled.
 */
uint32_t app_button_enable(void);

/**@brief Function for disabling button detection.
 *
 * @retval  NRF_ERROR_INVALID_PARAM   GPIOTE has to many users.
 * @retval  NRF_ERROR_INVALID_STATE   Button or GPIOTE not initialized.
 * @retval  NRF_SUCCESS               Button detection successfully enabled.
 */
uint32_t app_button_disable(void);

/**@brief Function for checking if a button is currently being pushed.
 *
 * @param[in]  pin_no        Button pin to be checked.
 * @param[out] p_is_pushed   Button state.
 *
 * @retval     NRF_SUCCESS               State successfully read.
 * @retval     NRF_ERROR_INVALID_PARAM   Invalid pin_no.
 */
uint32_t app_button_is_pushed(uint8_t pin_no, bool * p_is_pushed);


// Type and functions for connecting the Buttons module to the scheduler:

/**@cond NO_DOXYGEN */
typedef struct
{
    app_button_handler_t button_handler;
    uint8_t              pin_no;
    uint8_t              button_action;
} app_button_event_t;

static __INLINE void app_button_evt_get(void * p_event_data, uint16_t event_size)
{
    app_button_event_t * p_buttons_event = (app_button_event_t *)p_event_data;

    APP_ERROR_CHECK_BOOL(event_size == sizeof(app_button_event_t));
    p_buttons_event->button_handler(p_buttons_event->pin_no, p_buttons_event->button_action);
}

static __INLINE uint32_t app_button_evt_schedule(app_button_handler_t button_handler,
                                                 uint8_t              pin_no,
                                                 uint8_t              button_action)
{
    app_button_event_t buttons_event;

    buttons_event.button_handler = button_handler;
    buttons_event.pin_no         = pin_no;
    buttons_event.button_action  = button_action;

    return app_sched_event_put(&buttons_event, sizeof(buttons_event), app_button_evt_get);
}
/**@endcond */

#endif // APP_BUTTON_H__

/** @} */
