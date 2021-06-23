/*
 * Copyright (c) 2020, Pelion and affiliates.
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

#ifndef SERVICE_LIBS_RANDOM_EARLY_DETECTION_RANDOM_EARLY_DETECTION_H_
#define SERVICE_LIBS_RANDOM_EARLY_DETECTION_RANDOM_EARLY_DETECTION_H_

#ifdef __cplusplus
extern "C" {
#endif

//This value cant be bigger than 655
#define PROB_SCALE 512
#define PROB_SCALE_MAX PROB_SCALE * 100
#define PROX_MAX_RANDOM PROB_SCALE_MAX-1

typedef struct red_config_s {
    uint16_t weight;            /*< Weight for new sample len, 256 disabled average */
    uint16_t threshold_min;     /*< Threshold Min value which start possibility start drop a packet */
    uint16_t threshold_max;     /*< Threshold Max this value give max Probability for configured value over that every new packet will be dropped*/
    uint8_t drop_maX_P;         /*< Max probability for drop packet between threshold_min and threshold_max threshold */
} red_config_t;

typedef struct red_info_s {
    red_config_t parameters;    /*< Random Early detetction parameters for queue avarge and packet drop */
    uint32_t averageQ;          /*< Average queue size Scaled by 256 1.0 is 256 */
    uint16_t count;             /*< Missed Packet drop's. This value is incremented when average queue is over min threshoild and packet is noot dropped */
} red_info_t;

#ifdef __cplusplus
}
#endif

#endif /* SERVICE_LIBS_RANDOM_EARLY_DETECTION_RANDOM_EARLY_DETECTION_H_ */
