#ifndef __FREERTOS_PMU_H_
#define __FREERTOS_PMU_H_

#if DEVICE_SLEEP
#include "sleep_ex_api.h"

#ifndef BIT
#define BIT(n)                   (1<<n)
#endif

#ifdef CONFIG_PLATFORM_8195A
#define DEFAULT_WAKEUP_EVENT (SLEEP_WAKEUP_BY_STIMER | SLEEP_WAKEUP_BY_GTIMER | SLEEP_WAKEUP_BY_GPIO_INT | SLEEP_WAKEUP_BY_WLAN)

typedef enum PMU_DEVICE {

    PMU_OS = 0,
    PMU_WLAN_DEVICE = 1,
    PMU_LOGUART_DEVICE = 2,
    PMU_SDIO_DEVICE = 3,

    PMU_DEV_USER_BASE= 16,

    PMU_MAX = 31

} PMU_DEVICE;

// default locked by OS and not to sleep until OS release wakelock in somewhere
#define DEFAULT_WAKELOCK         (BIT(PMU_OS))
typedef uint32_t (*PSM_HOOK_FUN)( unsigned int, void* param_ptr );
#endif

/** Acquire wakelock
 *
 *  A wakelock is a 32-bit map. Each module own 1 bit in this bit map.
 *  FreeRTOS tickless reference the wakelock and decide that if it can or cannot enter sleep state.
 *  If any module acquire and hold a bit in wakelock, then the whole system won't enter sleep state.
 *
 *  If wakelock is not equals to 0, then the system won't enter sleep.
 *
 *  @param nDeviceId        : The bit which is attempt to add into wakelock
 */
void pmu_acquire_wakelock(uint32_t nDeviceId);

/** Release wakelock
 *
 *  If wakelock equals to 0, then the system may enter sleep state if it is in idle state.
 *
 *  @param nDeviceId        : The bit which is attempt to remove from wakelock
 */
void pmu_release_wakelock(uint32_t nDeviceId);

/** Get current wakelock bit map value
 *
 *  @return               : the current wakelock bit map value
 */
uint32_t pmu_get_wakelock_status(void);

#if (configGENERATE_RUN_TIME_STATS == 1)

/** enable to keep wakelock stats
 *
 */
void pmu_enable_wakelock_stats( unsigned char enable );

/** Get text report that contain the statics of wakelock holding time
 *
 *  Each time a module acquries or releases wakelock, a holding time is calculated and sum up to a table.
 *  It is for debug that which module is power saving killer.
 *
 *  @param pcWriteBuffer  : The char buffer that contain the report
 */
void pmu_get_wakelock_hold_stats( char *pcWriteBuffer );

/** Recalculate the wakelock statics
 *
 *  By default the wakelock statics is calculated from system boot up.
 *  If we want to debug power saving killer from a specified timestamp, we can reset the statics.
 */
void pmu_clean_wakelock_stat(void);

#endif

/**
  * @brief  set system active time, system can not sleep beore timeout.
  * @param  timeout: system can not sleep beore timeout, unit is ms.
  * @retval status value:
  *          - 0: _FAIL
  *          - 1: _SUCCESS   
  */
uint32_t pmu_set_sysactive_time(uint32_t timeout_ms);

void pmu_add_wakeup_event(uint32_t event);
void pmu_del_wakeup_event(uint32_t event);

#if (defined CONFIG_PLATFORM_8195A) || (defined CONFIG_PLATFORM_8195BHP) || (defined CONFIG_PLATFORM_8710C) || (defined CONFIG_PLATFORM_8711B)
void pmu_register_sleep_callback(uint32_t nDeviceId, PSM_HOOK_FUN sleep_hook_fun, void* sleep_param_ptr, PSM_HOOK_FUN wakeup_hook_fun, void* wakeup_param_ptr);
void pmu_unregister_sleep_callback(uint32_t nDeviceId);
#endif

#ifdef CONFIG_PLATFORM_8195A
/** Set PLL reserved or not when sleep is called
 *
 *  @param reserve: true for sleep with PLL reserve
 */
void pmu_set_pll_reserved(unsigned char reserve);
#endif

#endif

#endif
