/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CHANNEL_LIST_H__
#define __CHANNEL_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Dump the channel list object data to ns_trace using given trace level and group.
 */
void channel_list_print(uint8_t dlevel, const char* grp, const uint32_t* list);

/**
 * Get channel number using channel index.
 *
 * @param list to scan
 * @param channel index
 *
 * @return channel number
 */
uint8_t channel_list_get_channel(const uint32_t* list, int current_index);

/**
 * Clear the channel mask bitmap, does not change channel page.
 *
 * @param list list which mask is to be cleared
 */
void channel_list_clear_mask(uint32_t* list);

/**
 * Get next enabled channel number from given list. Channels are now taken sequentially,
 * starting from the index given.
 *
 * @param list to scan
 * @param the currently used channel index, ie. the place where search for next channel
 * is started
 * @return channel number of next channel
 */
int channel_list_get_next(const uint32_t* list, int current_index);

int channel_list_get_next_broadcast(const uint32_t* list, int broadcast_channel_count, int current_index);

/**
 * Get the first channel enabled in a list.
 *
 * @param list to scan
 * @return index of the first channel enabled
 */
int channel_list_get_first(const uint32_t* list);

/**
 * Count the amount of channels enabled in a list.
 *
 * @param list to scan
 * @return amount of bits set in the channel masks
 */
int channel_list_count_channels(const uint32_t* list);

/**
 * Enable channel by given channel number. This is likely to be used
 * from the test/application configuration side.
 *
 * Note: the channel number validity is not (yet?) verified, so one
 * can enable invalid channels which should not be according to channel page.
 *
 * @param list to modify
 * @param channel number
 * @return 0 on success, negative on failure (out of bounds channel)
 */
int channel_list_enable_channel(uint32_t* list, int channel_number);

/**
 * Check, if given channel is enabled. This is likely to be used
 * from the test/application configuration side.
 *
 * Note: the channel number validity is not (yet?) verified, so one
 * can enable invalid channels which should not be according to channel page.
 *
 * @param list to test
 * @param channel number
 * @return true, if channel is enabled on mask, false if not
 */
bool channel_list_is_channel_enabled(const uint32_t* list, int channel_number);

#ifdef __cplusplus
}
#endif
#endif // !__CHANNEL_LIST_H__
