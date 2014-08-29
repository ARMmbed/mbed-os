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
 * @defgroup app_timer Application Timer
 * @{
 * @ingroup app_common
 *
 * @brief Application timer functionality.
 *
 * @details It enables the application to create multiple timer instances based on the RTC1
 *          peripheral. Checking for timeouts and invokation of user timeout handlers is performed
 *          in the RTC1 interrupt handler. List handling is done using a software interrupt (SWI0).
 *          Both interrupt handlers are running in APP_LOW priority level.
 *
 * @note    When calling app_timer_start() or app_timer_stop(), the timer operation is just queued,
 *          and the software interrupt is triggered. The actual timer start/stop operation is
 *          executed by the SWI0 interrupt handler. Since the SWI0 interrupt is running in APP_LOW,
 *          if the application code calling the timer function is running in APP_LOW or APP_HIGH,
 *          the timer operation will not be performed until the application handler has returned.
 *          This will be the case e.g. when stopping a timer from a timeout handler when not using
 *          the scheduler.
 *
 * @details Use the USE_SCHEDULER parameter of the APP_TIMER_INIT() macro to select if the
 *          @ref app_scheduler is to be used or not.
 *
 * @note    Even if the scheduler is not used, app_timer.h will include app_scheduler.h, so when
 *          compiling, app_scheduler.h must be available in one of the compiler include paths.
 */

#ifndef APP_TIMER_H__
#define APP_TIMER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "app_error.h"
#include "app_util.h"
#include "app_scheduler.h"
#include "compiler_abstraction.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#define APP_TIMER_SCHED_EVT_SIZE     sizeof(app_timer_event_t)  /**< Size of button events being passed through the scheduler (is to be used for computing the maximum size of scheduler events). */
#define APP_TIMER_CLOCK_FREQ         32768                      /**< Clock frequency of the RTC timer used to implement the app timer module. */
#define APP_TIMER_MIN_TIMEOUT_TICKS  5                          /**< Minimum value of the timeout_ticks parameter of app_timer_start(). */

#define APP_TIMER_NODE_SIZE          40                         /**< Size of app_timer.timer_node_t (only for use inside APP_TIMER_BUF_SIZE()). */
#define APP_TIMER_USER_OP_SIZE       24                         /**< Size of app_timer.timer_user_op_t (only for use inside APP_TIMER_BUF_SIZE()). */
#define APP_TIMER_USER_SIZE          8                          /**< Size of app_timer.timer_user_t (only for use inside APP_TIMER_BUF_SIZE()). */
#define APP_TIMER_INT_LEVELS         3                          /**< Number of interrupt levels from where timer operations may be initiated (only for use inside APP_TIMER_BUF_SIZE()). */

#define MAX_RTC_COUNTER_VAL     0x00FFFFFF                                  /**< Maximum value of the RTC counter. */

/**@brief Compute number of bytes required to hold the application timer data structures.
 *
 * @param[in]  MAX_TIMERS      Maximum number of timers that can be created at any given time.
 * @param[in]  OP_QUEUE_SIZE   Size of queues holding timer operations that are pending execution.
 *                             NOTE: Due to the queue implementation, this size must be one more
 *                             than the size that is actually needed.
 *
 * @return     Required application timer buffer size (in bytes).
 */
#define APP_TIMER_BUF_SIZE(MAX_TIMERS, OP_QUEUE_SIZE)                                              \
    (                                                                                              \
        ((MAX_TIMERS) * APP_TIMER_NODE_SIZE)                                                       \
        +                                                                                          \
        (                                                                                          \
            APP_TIMER_INT_LEVELS                                                                   \
            *                                                                                      \
            (APP_TIMER_USER_SIZE + ((OP_QUEUE_SIZE) + 1) * APP_TIMER_USER_OP_SIZE)                 \
        )                                                                                          \
    )

/**@brief Convert milliseconds to timer ticks.
 *
 * @note This macro uses 64 bit integer arithmetic, but as long as the macro parameters are
 *       constants (i.e. defines), the computation will be done by the preprocessor.
 *
 * @param[in]  MS          Milliseconds.
 * @param[in]  PRESCALER   Value of the RTC1 PRESCALER register (must be the same value that was
 *                         passed to APP_TIMER_INIT()).
 *
 * @note   When using this macro, it is the responsibility of the developer to ensure that the
 *         values provided as input result in an output value that is supported by the
 *         @ref app_timer_start function. For example, when the ticks for 1 ms is needed, the
 *         maximum possible value of PRESCALER must be 6, when @ref APP_TIMER_CLOCK_FREQ is 32768.
 *         This will result in a ticks value as 5. Any higher value for PRESCALER will result in a
 *         ticks value that is not supported by this module.
 *
 * @return     Number of timer ticks.
 */
#define APP_TIMER_TICKS(MS, PRESCALER)\
            ((uint32_t)ROUNDED_DIV((MS) * (uint64_t)APP_TIMER_CLOCK_FREQ, ((PRESCALER) + 1) * 1000))

/**@brief Timer id type. */
typedef uint32_t app_timer_id_t;

#define TIMER_NULL                  ((app_timer_id_t)(0 - 1))                   /**< Invalid timer id. */

/**@brief Application timeout handler type. */
typedef void (*app_timer_timeout_handler_t)(void * p_context);

/**@brief Type of function for passing events from the timer module to the scheduler. */
typedef uint32_t (*app_timer_evt_schedule_func_t) (app_timer_timeout_handler_t timeout_handler,
                                                   void *                      p_context);

/**@brief Timer modes. */
typedef enum
{
    APP_TIMER_MODE_SINGLE_SHOT,                 /**< The timer will expire only once. */
    APP_TIMER_MODE_REPEATED                     /**< The timer will restart each time it expires. */
} app_timer_mode_t;

/**@brief Macro for initializing the application timer module.
 *
 * @details It will handle dimensioning and allocation of the memory buffer required by the timer,
 *          making sure that the buffer is correctly aligned. It will also connect the timer module
 *          to the scheduler (if specified).
 *
 * @note    This module assumes that the LFCLK is already running. If it isn't, the module will
 *          be non-functional, since the RTC will not run. If you don't use a softdevice, you'll
 *          have to start the LFCLK manually. See the rtc_example's \ref lfclk_config() function
 *          for an example of how to do this. If you use a softdevice, the LFCLK is started on
 *          softdevice init.
 *
 *
 * @param[in]  PRESCALER        Value of the RTC1 PRESCALER register. This will decide the
 *                              timer tick rate. Set to 0 for no prescaling.
 * @param[in]  MAX_TIMERS       Maximum number of timers that can be created at any given time.
 * @param[in]  OP_QUEUES_SIZE   Size of queues holding timer operations that are pending execution.
 * @param[in]  USE_SCHEDULER    TRUE if the application is using the event scheduler,
 *                              FALSE otherwise.
 *
 * @note Since this macro allocates a buffer, it must only be called once (it is OK to call it
 *       several times as long as it is from the same location, e.g. to do a reinitialization).
 */
/*lint -emacro(506, APP_TIMER_INIT) */ /* Suppress "Constant value Boolean */
#define APP_TIMER_INIT(PRESCALER, MAX_TIMERS, OP_QUEUES_SIZE, USE_SCHEDULER)                       \
    do                                                                                             \
    {                                                                                              \
        static uint32_t APP_TIMER_BUF[CEIL_DIV(APP_TIMER_BUF_SIZE((MAX_TIMERS),                    \
                                                                  (OP_QUEUES_SIZE) + 1),           \
                                               sizeof(uint32_t))];                                 \
        uint32_t ERR_CODE = app_timer_init((PRESCALER),                                            \
                                           (MAX_TIMERS),                                           \
                                           (OP_QUEUES_SIZE) + 1,                                   \
                                           APP_TIMER_BUF,                                          \
                                           (USE_SCHEDULER) ? app_timer_evt_schedule : NULL);       \
        APP_ERROR_CHECK(ERR_CODE);                                                                 \
    } while (0)

/**@brief Function for initializing the timer module.
 *
 * @note Normally initialization should be done using the APP_TIMER_INIT() macro, as that will both
 *       allocate the buffers needed by the timer module (including aligning the buffers correctly,
 *       and also take care of connecting the timer module to the scheduler (if specified).
 *
 * @param[in]  prescaler           Value of the RTC1 PRESCALER register. Set to 0 for no prescaling.
 * @param[in]  max_timers          Maximum number of timers that can be created at any given time.
 * @param[in]  op_queues_size      Size of queues holding timer operations that are pending
 *                                 execution. NOTE: Due to the queue implementation, this size must
 *                                 be one more than the size that is actually needed.
 * @param[in]  p_buffer            Pointer to memory buffer for internal use in the app_timer
 *                                 module. The size of the buffer can be computed using the
 *                                 APP_TIMER_BUF_SIZE() macro. The buffer must be aligned to a
 *                                 4 byte boundary.
 * @param[in]  evt_schedule_func   Function for passing timeout events to the scheduler. Point to
 *                                 app_timer_evt_schedule() to connect to the scheduler. Set to NULL
 *                                 to make the timer module call the timeout handler directly from
 *                                 the timer interrupt handler.
 *
 * @retval     NRF_SUCCESS               Successful initialization.
 * @retval     NRF_ERROR_INVALID_PARAM   Invalid parameter (buffer not aligned to a 4 byte
 *                                       boundary or NULL).
 */
uint32_t app_timer_init(uint32_t                      prescaler,
                        uint8_t                       max_timers,
                        uint8_t                       op_queues_size,
                        void *                        p_buffer,
                        app_timer_evt_schedule_func_t evt_schedule_func);

/**@brief Function for creating a timer instance.
 *
 * @param[out] p_timer_id        Id of the newly created timer.
 * @param[in]  mode              Timer mode.
 * @param[in]  timeout_handler   Function to be executed when the timer expires.
 *
 * @retval     NRF_SUCCESS               Timer was successfully created.
 * @retval     NRF_ERROR_INVALID_PARAM   Invalid parameter.
 * @retval     NRF_ERROR_INVALID_STATE   Application timer module has not been initialized.
 * @retval     NRF_ERROR_NO_MEM          Maximum number of timers has already been reached.
 *
 * @note This function does the timer allocation in the caller's context. It is also not protected
 *       by a critical region. Therefore care must be taken not to call it from several interrupt
 *       levels simultaneously.
 */
uint32_t app_timer_create(app_timer_id_t *            p_timer_id,
                          app_timer_mode_t            mode,
                          app_timer_timeout_handler_t timeout_handler);

/**@brief Function for starting a timer.
 *
 * @param[in]  timer_id        Id of timer to start.
 * @param[in]  timeout_ticks   Number of ticks (of RTC1, including prescaling) to timeout event
 *                             (minimum 5 ticks).
 * @param[in]  p_context       General purpose pointer. Will be passed to the timeout handler when
 *                             the timer expires.
 *
 * @retval     NRF_SUCCESS               Timer was successfully started.
 * @retval     NRF_ERROR_INVALID_PARAM   Invalid parameter.
 * @retval     NRF_ERROR_INVALID_STATE   Application timer module has not been initialized, or timer
 *                                       has not been created.
 * @retval     NRF_ERROR_NO_MEM          Timer operations queue was full.
 *
 * @note The minimum timeout_ticks value is 5.
 * @note For multiple active timers, timeouts occurring in close proximity to each other (in the
 *       range of 1 to 3 ticks) will have a positive jitter of maximum 3 ticks.
 * @note When calling this method on a timer which is already running, the second start operation
 *       will be ignored.
 */
uint32_t app_timer_start(app_timer_id_t timer_id, uint32_t timeout_ticks, void * p_context);

/**@brief Function for stopping the specified timer.
 *
 * @param[in]  timer_id   Id of timer to stop.
 *
 * @retval     NRF_SUCCESS               Timer was successfully stopped.
 * @retval     NRF_ERROR_INVALID_PARAM   Invalid parameter.
 * @retval     NRF_ERROR_INVALID_STATE   Application timer module has not been initialized, or timer
 *                                       has not been created.
 * @retval     NRF_ERROR_NO_MEM          Timer operations queue was full.
 */
uint32_t app_timer_stop(app_timer_id_t timer_id);

/**@brief Function for stopping all running timers.
 *
 * @retval     NRF_SUCCESS               All timers were successfully stopped.
 * @retval     NRF_ERROR_INVALID_STATE   Application timer module has not been initialized.
 * @retval     NRF_ERROR_NO_MEM          Timer operations queue was full.
 */
uint32_t app_timer_stop_all(void);

/**@brief Function for returning the current value of the RTC1 counter. The
 * value includes overflow bits to extend the range to 64-bits.
 *
 * @param[out] p_ticks   Current value of the RTC1 counter.
 *
 * @retval     NRF_SUCCESS   Counter was successfully read.
 */
uint32_t app_timer_cnt_get(uint64_t * p_ticks);

/**@brief Function for computing the difference between two RTC1 counter values.
 *
 * @param[in]  ticks_to       Value returned by app_timer_cnt_get().
 * @param[in]  ticks_from     Value returned by app_timer_cnt_get().
 * @param[out] p_ticks_diff   Number of ticks from ticks_from to ticks_to.
 *
 * @retval     NRF_SUCCESS   Counter difference was successfully computed.
 */
uint32_t app_timer_cnt_diff_compute(uint32_t   ticks_to,
                                    uint32_t   ticks_from,
                                    uint32_t * p_ticks_diff);


// Type and functions for connecting the timer to the scheduler:

/**@cond NO_DOXYGEN */
typedef struct
{
    app_timer_timeout_handler_t timeout_handler;
    void *                      p_context;
} app_timer_event_t;

static __INLINE void app_timer_evt_get(void * p_event_data, uint16_t event_size)
{
    app_timer_event_t * p_timer_event = (app_timer_event_t *)p_event_data;

    APP_ERROR_CHECK_BOOL(event_size == sizeof(app_timer_event_t));
    p_timer_event->timeout_handler(p_timer_event->p_context);
}

static __INLINE uint32_t app_timer_evt_schedule(app_timer_timeout_handler_t timeout_handler,
                                                void *                      p_context)
{
    app_timer_event_t timer_event;

    timer_event.timeout_handler = timeout_handler;
    timer_event.p_context       = p_context;

    return app_sched_event_put(&timer_event, sizeof(timer_event), app_timer_evt_get);
}
/**@endcond */

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // APP_TIMER_H__

/** @} */
