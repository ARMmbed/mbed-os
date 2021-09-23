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

#ifndef SERVICE_LIBS_RANDOM_EARLY_DETECTION_RANDOM_EARLY_DETECTION_API_H_
#define SERVICE_LIBS_RANDOM_EARLY_DETECTION_RANDOM_EARLY_DETECTION_API_H_

#ifdef __cplusplus
extern "C" {
#endif

struct red_info_s;

#define RED_AVERAGE_WEIGHT_DISABLED 256     /*< Average is disabled */
#define RED_AVERAGE_WEIGHT_HALF     128     /*< Average weight for new sample is 0.5*new + 0.5 to last one */
#define RED_AVERAGE_WEIGHT_QUARTER  64      /*< Average weight for new sample is 1/4 + 3/4 to last one */
#define RED_AVERAGE_WEIGHT_EIGHTH   32      /*< Average weight for new sample is 1/8 + 7/8 to last one */

/**
 * \brief Create Random early detection data
 *
 * Function will config parameters how wide are Random Early detection drop will work.
 *
 * How to use parameters:
 *
 * Weight is definition how message queue Average (AQ) is calculated. Smaller weight will give smoother AQ update.
 *
 *  AQ = (1-weight) * average_queue + weight*sampleLen;
 *
 *  * RED_AVERAGE_WEIGHT_DISABLED disable Average by max weight to new sample length
 *  * RED_AVERAGE_WEIGHT_HALF last average*0.5 + 0.5*new sample: Smooth Average light packet filter
 *  * RED_AVERAGE_WEIGHT_QUARTER last average*0.75 + 0.25*new sample: Medium packet burst filtering
 *  * RED_AVERAGE_WEIGHT_EIGHTH last average*7/8 + 1/8*new sample: Good for filtering packet burst and big networks
 *
 * How to configure packet drop possibility:
 *
 * Define base Probability based current AQ, average length
 *
 * tempP = drop_maX_P *(AQ - threshold_min) / (threshold_max - threshold_min);
 *
 * Prob = tempP / (1 - count*tempP)
 *
 * threshold_min and threshold_max threshold define area for random early detection drop. When Average queue size go over Min threshold packet may drop by given maxProbability.
 * System will work smoother if min -max threshold range is wide. Then random drop is may cover small data burst until Max threshold Avarage is reached.
 * After Max every new packet will be dropped.
 *
 * Config Examples.
 *
 * Threshold values must be set how much device can buffer data.
 *
 * Small size data buffering:
 * random_early_detection_create(32, 96, 10, RED_AVERAGE_WEIGHT_QUARTER)
 *
 * Medium size data buffering:
 * random_early_detection_create(96, 256, 10, RED_AVERAGE_WEIGHT_EIGHTH)
 *
 * High size buffering:
 * random_early_detection_create(256, 600, 10, RED_AVERAGE_WEIGHT_EIGHTH)
 *
 * \param threshold_min min average queue size which enable packet drop
 * \param threshold_max average queue size when all new packets start drop
 * \param drop_maX_P is percent probability to drop packet 100-1 are possible values
 * \param weight accepted values 256-1, 256 is 1.0 weight which mean that new queue size overwrite old. 128 is 0.5 which gives 0.5 from old + 0.5 from new.
 * \return Pointer for allocated structure, NULL if memory allocation fail
 */
struct red_info_s *random_early_detection_create(uint16_t threshold_min, uint16_t threshold_max, uint8_t drop_maX_P, uint16_t weight);


/**
 * \brief Free Random early detection data
 *
 *
 * \param red_info pointer to data
 */
void random_early_detection_free(struct red_info_s *red_info);


/**
 * \brief Random early detection drop function
 *
 * \param red_info pointer, which is created user include all configurations
 * \param sampleLen Current queue length
 * \return true Drop packet
 * \return false Packet can be added to queue
 */
bool random_early_detection_congestion_check(struct red_info_s *red_info);

/**
 * \brief Random early detection Average queue calculate
 *
 *  Call this when add or remove from queue
 *
 * \param red_info pointer, which is created user include all configurations
 * \param sampleLen Current queue length
 *
 * \return New average
 */
uint16_t random_early_detetction_aq_calc(struct red_info_s *red_info, uint16_t sampleLen);

/**
 * \brief Read Random early detection Average queue size
 *
 *  Call this when add or remove from queue
 *
 * \param red_info pointer, which is created user include all configurations
 *
 * \return Current average
 */
uint16_t random_early_detetction_aq_read(struct red_info_s *red_info);

#ifdef __cplusplus
}
#endif

#endif /* SERVICE_LIBS_RANDOM_EARLY_DETECTION_RANDOM_EARLY_DETECTION_API_H_ */
