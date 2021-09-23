/*
 * Copyright (c) 2015-2018, Pelion and affiliates.
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
 * Get channel number using channel index.
 *
 * @param list to scan
 * @param channel index
 *
 * @return channel number
 */
uint8_t channel_list_get_channel(const uint32_t *list, int current_index);
/**
 * set matching bit on in in channel mask.
 *
 * @param list channel mask
 * @param channel channel number
 * @param active set the channel on if true, disable channel if false.
 *
 * @return channel number
 */
void channel_list_set_channel(uint32_t *list, int channel, bool active);

/**
 * Count the amount of channels enabled in a list.
 *
 * @param list to scan
 * @return amount of bits set in the channel masks
 */
int channel_list_count_channels(const uint32_t *list);


#ifdef __cplusplus
}
#endif
#endif // !__CHANNEL_LIST_H__
