/**
 *
 * @defgroup app_timer_config Application timer functionality configuration
 * @{
 * @ingroup app_timer
 */
/** @brief Enabling app_timer module
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_TIMER_ENABLED

/** @brief Enable app_timer profiling
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_TIMER_WITH_PROFILER


/** @brief Enable RTC always on
 *
 * If option is enabled RTC is kept running even if there is no active timers.
 * This option can be used when app_timer is used for timestamping.
 *
 *  Set to 1 to activate.
 *
 * @note This is an NRF_CONFIG macro.
 */
#define APP_TIMER_KEEPS_RTC_ACTIVE



/** @} */
