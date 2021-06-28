/*
 * Copyright (c) 2017, Pelion and affiliates.
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

#ifndef _NS_FNET_EVENTS_H_
#define _NS_FNET_EVENTS_H_

/*
 * Start events module
 */
void ns_fnet_events_start(void);

/*
 * Stop events module
 */
void ns_fnet_events_stop(void);

/*
 * Make immediate poll.
 * Service is driven by timer events, this method allows client to make immediate poll
 * to reduce service response time.
 */
void ns_fnet_events_fast_poll(void);

/*
 * Get current time in milliseconds, time starts counting from first call to ns_fnet_events_start.
 */
uint32_t ns_fnet_time_in_ms_get(void);

#endif /* _NS_FNET_EVENTS_H_ */
