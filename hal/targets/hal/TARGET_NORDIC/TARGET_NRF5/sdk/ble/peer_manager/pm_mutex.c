/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
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


#include "pm_mutex.h"

#include <stdbool.h>
#include <string.h>
#include "nrf_error.h"
#include "app_util_platform.h"



/**@brief Locks the mutex defined by the mask.
 *
 * @param p_mutex pointer to the mutex storage.
 * @param mutex_mask the mask identifying the mutex position.
 *
 * @retval true if the mutex could be locked.
 * @retval false if the mutex was already locked.
 */
static bool lock_by_mask(uint8_t * p_mutex, uint8_t mutex_mask)
{
    bool success = false;

    if ( (*p_mutex & mutex_mask) == 0 )
    {
        CRITICAL_REGION_ENTER();
        if ( (*p_mutex & mutex_mask) == 0 )
        {
            *p_mutex |= mutex_mask;

            success = true;
        }
        CRITICAL_REGION_EXIT();
    }

    return ( success );
}


void pm_mutex_init(uint8_t * p_mutex, uint16_t mutex_size)
{
    if (p_mutex != NULL)
    {
        memset(&p_mutex[0], 0, MUTEX_STORAGE_SIZE(mutex_size));
    }
}


bool pm_mutex_lock(uint8_t * p_mutex, uint16_t mutex_id)
{
    if (p_mutex != NULL)
    {
        return ( lock_by_mask(&(p_mutex[mutex_id >> 3]), (1 << (mutex_id & 0x07))) );
    }
    else
    {
        return false;
    }
}


void pm_mutex_unlock(uint8_t * p_mutex, uint16_t mutex_id)
{
    uint8_t mutex_base = mutex_id >> 3;
    uint8_t mutex_mask = (1 << (mutex_id & 0x07));

    if   ((p_mutex != NULL)
       && (p_mutex[mutex_base] & mutex_mask))
    {
        CRITICAL_REGION_ENTER();
        p_mutex[mutex_base] &= ~mutex_mask;
        CRITICAL_REGION_EXIT();
    }
}


uint16_t pm_mutex_lock_first_available(uint8_t * p_mutex, uint16_t mutex_size)
{
    if (p_mutex != NULL)
    {
        for ( uint16_t i = 0; i < mutex_size; i++ )
        {
            if ( lock_by_mask(&(p_mutex[i >> 3]), 1 << (i & 0x07)) )
            {
                return ( i );
            }
        }
    }

    return ( mutex_size );
}


bool pm_mutex_lock_status_get(uint8_t * p_mutex, uint16_t mutex_id)
{
    if (p_mutex != NULL)
    {
        return ( (p_mutex[mutex_id >> 3] & (1 << (mutex_id & 0x07))) );
    }
    else
    {
        return true;
    }
}
