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

#include "sdk_mapped_flags.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "compiler_abstraction.h"


/**@brief Function for setting the state of a flag to true.
 *
 * @note This function does not check whether the index is valid.
 *
 * @param[in]  p_flags  The collection of flags to modify.
 * @param[in]  index    The index of the flag to modify.
 */
static __INLINE void sdk_mapped_flags_set_by_index(sdk_mapped_flags_t * p_flags, uint16_t index)
{
    *p_flags |= (1U << index);
}


/**@brief Function for setting the state of a flag to false.
 *
 * @note This function does not check whether the index is valid.
 *
 * @param[in]  p_flags  The collection of flags to modify.
 * @param[in]  index    The index of the flag to modify.
 */
static __INLINE void sdk_mapped_flags_clear_by_index(sdk_mapped_flags_t * p_flags, uint16_t index)
{
    *p_flags &= ~(1U << index);
}


/**@brief Function for getting the state of a flag.
 *
 * @note This function does not check whether the index is valid.
 *
 * @param[in]  p_flags  The collection of flags to read.
 * @param[in]  index    The index of the flag to get.
 */
static __INLINE bool sdk_mapped_flags_get_by_index(sdk_mapped_flags_t flags, uint16_t index)
{
    return ((flags & (1 << index)) != 0);
}



uint16_t sdk_mapped_flags_first_key_index_get(sdk_mapped_flags_t flags)
{
    for (uint16_t i = 0; i < SDK_MAPPED_FLAGS_N_KEYS; i++)
    {
        if (sdk_mapped_flags_get_by_index(flags, i))
        {
            return i;
        }
    }
    return SDK_MAPPED_FLAGS_INVALID_INDEX;
}


void sdk_mapped_flags_update_by_key(uint16_t           * p_keys,
                                    sdk_mapped_flags_t * p_flags,
                                    uint16_t             key,
                                    bool                 value)
{
    sdk_mapped_flags_bulk_update_by_key(p_keys, p_flags, 1, key, value);
}


void sdk_mapped_flags_bulk_update_by_key(uint16_t           * p_keys,
                                         sdk_mapped_flags_t * p_flags,
                                         uint32_t             n_flag_collections,
                                         uint16_t             key,
                                         bool                 value)
{
    if ((p_keys != NULL) && (p_flags != NULL) && (n_flag_collections > 0))
    {
        for (uint32_t i = 0; i < SDK_MAPPED_FLAGS_N_KEYS; i++)
        {
            if (p_keys[i] == key)
            {
                for (uint32_t j = 0; j < n_flag_collections; j++)
                {
                    if (value)
                    {
                        sdk_mapped_flags_set_by_index(&p_flags[j], i);
                    }
                    else
                    {
                        sdk_mapped_flags_clear_by_index(&p_flags[j], i);
                    }
                }
                return;
            }
        }
    }
}


bool sdk_mapped_flags_get_by_key(uint16_t * p_keys, sdk_mapped_flags_t flags, uint16_t key)
{
    if (p_keys != NULL)
    {
        for (uint32_t i = 0; i < SDK_MAPPED_FLAGS_N_KEYS; i++)
        {
            if (p_keys[i] == key)
            {
                return sdk_mapped_flags_get_by_index(flags, i);
            }
        }
    }
    return false;
}


sdk_mapped_flags_key_list_t sdk_mapped_flags_key_list_get(uint16_t           * p_keys,
                                                          sdk_mapped_flags_t   flags)
{
    sdk_mapped_flags_key_list_t key_list;
    key_list.len = 0;

    if (p_keys != NULL)
    {
        for (uint32_t i = 0; i < SDK_MAPPED_FLAGS_N_KEYS; i++)
        {
            if (sdk_mapped_flags_get_by_index(flags, i))
            {
                key_list.flag_keys[key_list.len++] = p_keys[i];
            }
        }
    }

    return key_list;
}


uint32_t sdk_mapped_flags_n_flags_set(sdk_mapped_flags_t flags)
{
    uint32_t n_flags_set = 0;

    for (uint32_t i = 0; i < SDK_MAPPED_FLAGS_N_KEYS; i++)
    {
        if (sdk_mapped_flags_get_by_index(flags, i))
        {
            n_flags_set += 1;
        }
    }
    return n_flags_set;
}
