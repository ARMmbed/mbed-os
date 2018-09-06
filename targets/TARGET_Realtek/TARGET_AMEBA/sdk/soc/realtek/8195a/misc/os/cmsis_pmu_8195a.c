#if DEVICE_SLEEP

//#include "FreeRTOS.h"
#include "cmsis_pmu_8195a.h"

#include <platform_opts.h>

#include "platform_autoconf.h"
#include "platform_stdlib.h"
//#include "sys_api.h"

#include "sleep_ex_api.h"

#ifndef portNVIC_SYSTICK_CURRENT_VALUE_REG
#define portNVIC_SYSTICK_CURRENT_VALUE_REG	( * ( ( volatile uint32_t * ) 0xe000e018 ) )
#endif

uint32_t missing_tick = 0;

static uint32_t wakelock     = 0;
//static uint32_t wakelock     = DEFAULT_WAKELOCK;

static uint32_t wakeup_event = DEFAULT_WAKEUP_EVENT;

typedef struct {
    uint32_t     nDeviceId;
    PSM_HOOK_FUN sleep_hook_fun;
    void*        sleep_param_ptr;
    PSM_HOOK_FUN wakeup_hook_fun;
    void*        wakeup_param_ptr;
} PSM_DD_HOOK_INFO;

#define MAX_PSM_DD_HOOK_INFO_SIZE 8
uint32_t psm_dd_hook_info_size = 0;
PSM_DD_HOOK_INFO psm_dd_hook_infos[MAX_PSM_DD_HOOK_INFO_SIZE];

static uint8_t last_wakelock_state[32] = {
    DEFAULT_WAKELOCK & 0x01, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};
static uint32_t last_acquire_wakelock_time[32] = {0};
static uint32_t hold_wakelock_time[32] = {0};
static uint32_t base_sys_time = 0;

static uint32_t sys_sleep_time = 0;

unsigned char reserve_pll = 0;
unsigned char generate_wakelock_stats = 0;


/* -------- FreeRTOS macro implementation -------- */

int cmsis_ready_to_sleep() {
    return wakelock == 0;
}

void pmu_acquire_wakelock(uint32_t lock_id) {

    wakelock |= BIT(lock_id);

    if (generate_wakelock_stats) {
        uint32_t i;

        //uint32_t current_timestamp = osKernelSysTick();
        uint32_t current_timestamp = osKernelGetSysTimerCount();

        for (i=0; i<32; i++) {
            if ( (1<<i & BIT(lock_id)) && (last_wakelock_state[i] == 0) ) {
                last_acquire_wakelock_time[i] = current_timestamp;
                last_wakelock_state[i] = 1;            
            }
        }
    }
}

void pmu_release_wakelock(uint32_t lock_id) {
    wakelock &= ~BIT(lock_id);

    if (generate_wakelock_stats) {
        uint32_t i;

        //uint32_t current_timestamp = osKernelSysTick();
        uint32_t current_timestamp = osKernelGetSysTimerCount();

        for (i=0; i<32; i++) {
            if ( (1<<i & BIT(lock_id)) && (last_wakelock_state[i] == 1) ) {
                hold_wakelock_time[i] += current_timestamp - last_acquire_wakelock_time[i];
                last_wakelock_state[i] = 0;
            }
        }
    }
}

uint32_t pmu_get_wakelock_status() {
    return wakelock;
}

void pmu_enable_wakelock_stats(unsigned char enable) {
    generate_wakelock_stats = enable;
}

void pmu_get_wakelock_hold_stats( char *pcWriteBuffer ) {
    uint32_t i;

    //uint32_t current_timestamp = osKernelSysTick();
    uint32_t current_timestamp = osKernelGetSysTimerCount();

    *pcWriteBuffer = 0x00;

    if (generate_wakelock_stats) {
        // print header
        sprintf(pcWriteBuffer, "wakelock_id\tholdtime\r\n");
        pcWriteBuffer += strlen( pcWriteBuffer );

        for (i=0; i<32; i++) {
            if (last_wakelock_state[i] == 1) {
                sprintf(pcWriteBuffer, "%x\t\t%d\r\n", i, hold_wakelock_time[i] + (current_timestamp - last_acquire_wakelock_time[i]));
            } else {
                if (hold_wakelock_time[i] > 0) {
                    sprintf(pcWriteBuffer, "%x\t\t%d\r\n", i, hold_wakelock_time[i]);
                }
            }
            pcWriteBuffer += strlen( pcWriteBuffer );
        }
        sprintf(pcWriteBuffer, "time passed: %d ms, system sleep %d ms\r\n", current_timestamp - base_sys_time, sys_sleep_time);
    }
}

void pmu_clean_wakelock_stat() {
    uint32_t i;

    //base_sys_time = osKernelSysTick();
    base_sys_time = osKernelGetSysTimerCount();

    for (i=0; i<32; i++) {
        hold_wakelock_time[i] = 0;
        if (last_wakelock_state[i] == 1) {
            last_acquire_wakelock_time[i] = base_sys_time;
        }
    }
	sys_sleep_time = 0;
}

void pmu_add_wakeup_event(uint32_t event) {
    wakeup_event |= event;
}

void pmu_del_wakeup_event(uint32_t event) {
    wakeup_event &= ~event;
    // To fulfill tickless design, system timer is required to be wakeup event
    wakeup_event |= SLEEP_WAKEUP_BY_STIMER;
}

void pmu_register_sleep_callback(uint32_t nDeviceId, PSM_HOOK_FUN sleep_hook_fun, void* sleep_param_ptr, PSM_HOOK_FUN wakeup_hook_fun, void* wakeup_param_ptr) {
    uint32_t i;
    for (i=0; i<psm_dd_hook_info_size; i++) {
        if (psm_dd_hook_infos[i].nDeviceId == nDeviceId) {
            psm_dd_hook_infos[i].sleep_hook_fun   = sleep_hook_fun;
            psm_dd_hook_infos[i].sleep_param_ptr  = sleep_param_ptr;
            psm_dd_hook_infos[i].wakeup_hook_fun  = wakeup_hook_fun;
            psm_dd_hook_infos[i].wakeup_param_ptr = wakeup_param_ptr;
            break;
        }
    }
    if (i == psm_dd_hook_info_size) {
        psm_dd_hook_infos[psm_dd_hook_info_size].nDeviceId        = nDeviceId;
        psm_dd_hook_infos[psm_dd_hook_info_size].sleep_hook_fun   = sleep_hook_fun;
        psm_dd_hook_infos[psm_dd_hook_info_size].sleep_param_ptr  = sleep_param_ptr;
        psm_dd_hook_infos[psm_dd_hook_info_size].wakeup_hook_fun  = wakeup_hook_fun;
        psm_dd_hook_infos[psm_dd_hook_info_size].wakeup_param_ptr = wakeup_param_ptr;
        psm_dd_hook_info_size++;
    }
}

void pmu_unregister_sleep_callback(uint32_t nDeviceId) {
    uint32_t i;
    for (i=0; i<psm_dd_hook_info_size; i++) {
        if (psm_dd_hook_infos[i].nDeviceId == nDeviceId) {
            if (psm_dd_hook_info_size > 1) {
                // if we have more than 2 items, just swap the last item into current slot
                psm_dd_hook_infos[i].nDeviceId        = psm_dd_hook_infos[psm_dd_hook_info_size-1].nDeviceId;
                psm_dd_hook_infos[i].sleep_hook_fun   = psm_dd_hook_infos[psm_dd_hook_info_size-1].sleep_hook_fun;
                psm_dd_hook_infos[i].sleep_param_ptr  = psm_dd_hook_infos[psm_dd_hook_info_size-1].sleep_param_ptr;
                psm_dd_hook_infos[i].wakeup_hook_fun  = psm_dd_hook_infos[psm_dd_hook_info_size-1].wakeup_hook_fun;
                psm_dd_hook_infos[i].wakeup_param_ptr = psm_dd_hook_infos[psm_dd_hook_info_size-1].wakeup_param_ptr;

                // Then erase the last item
                psm_dd_hook_infos[psm_dd_hook_info_size-1].nDeviceId        = 0;
                psm_dd_hook_infos[psm_dd_hook_info_size-1].sleep_hook_fun   = NULL;
                psm_dd_hook_infos[psm_dd_hook_info_size-1].sleep_param_ptr  = NULL;
                psm_dd_hook_infos[psm_dd_hook_info_size-1].wakeup_hook_fun  = NULL;
                psm_dd_hook_infos[psm_dd_hook_info_size-1].wakeup_param_ptr = NULL;
            } else {
                // we only have one item, just erase it
                psm_dd_hook_infos[i].nDeviceId = 0;
                psm_dd_hook_infos[i].sleep_hook_fun   = NULL;
                psm_dd_hook_infos[i].sleep_param_ptr  = NULL;
                psm_dd_hook_infos[i].wakeup_hook_fun  = NULL;
                psm_dd_hook_infos[i].wakeup_param_ptr = NULL;
            }
            psm_dd_hook_info_size--;
            break;
        }
    }
}

void pmu_set_pll_reserved(unsigned char reserve) {
    reserve_pll = reserve;
}

#endif
