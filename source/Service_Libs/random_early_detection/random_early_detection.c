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

#include "nsconfig.h"
#include "ns_types.h"
#include "string.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "Service_Libs/random_early_detection/random_early_detection.h"
#include "Service_Libs/random_early_detection/random_early_detection_api.h"


red_info_t *random_early_detection_create(uint16_t threshold_min, uint16_t threshold_max, uint8_t drop_max_p, uint16_t weight)
{
    //Weight must be between 1-256
    if (weight == 0 || weight > 256) {
        return NULL;
    }

    //Probability  must be between 1-100
    if (drop_max_p == 0 || drop_max_p > 100) {
        return NULL;
    }

    //Max Threshold can't smaller or similar than min
    if (threshold_max <= threshold_min) {
        return NULL;
    }

    red_info_t *red_info = ns_dyn_mem_alloc(sizeof(red_info_t));
    if (red_info) {
        red_info->count = 0;
        red_info->averageQ = 0;
        red_info->parameters.drop_maX_P = drop_max_p;
        red_info->parameters.threshold_max = threshold_max;
        red_info->parameters.threshold_min = threshold_min;
        red_info->parameters.weight = weight;
    }

    return red_info;
}


void random_early_detection_free(struct red_info_s *red_info)
{
    ns_dyn_mem_free(red_info);
}

//calculate average and return averaged value back
uint16_t random_early_detetction_aq_calc(red_info_t *red_info, uint16_t sampleLen)
{
    if (!red_info) {
        return 0;
    }

    if (red_info->parameters.weight == RED_AVERAGE_WEIGHT_DISABLED || red_info->averageQ == 0) {
        red_info->averageQ = sampleLen * 256;
        return sampleLen;
    }

    // AQ = (1-weight) * average_queue + weight*sampleLen
    // Now Sample is scaled by 256 which is not loosing so much tail at average

    //Weight Last Average part (1-weight) * average_queue with scaled 256
    uint32_t averageSum = ((256 - red_info->parameters.weight) * red_info->averageQ) / 256;
    //Add new weighted sample lenght (weight*sampleLen)
    averageSum += (red_info->parameters.weight * sampleLen);

    if (averageSum & 1) {
        //If sum is ODD add 1 this will help to not stuck like 1,99 average to -> 2
        averageSum++;
    }
    //Store new average
    red_info->averageQ = averageSum;
    //Return always same format scaled than inn
    return red_info->averageQ / 256;

}

uint16_t random_early_detetction_aq_read(red_info_t *red_info)
{
    if (!red_info) {
        return 0;
    }
    return red_info->averageQ / 256;
}



bool random_early_detection_congestion_check(red_info_t *red_info)
{
    if (!red_info) {
        return false;
    }

    //Calulate Average queue size
    uint16_t sampleLen = red_info->averageQ / 256;;

    if (sampleLen <= red_info->parameters.threshold_min) {
        //Can be added to queue without any RED operation
        red_info->count = 0;
        return false;
    }

    if (sampleLen > red_info->parameters.threshold_max) {
        //Always drop over threshold_max
        red_info->count = 0;
        return true;
    }

    // Calculate probability for packet drop
    // tempP = drop_maX_P *(AQ - threshold_min) / (threshold_max - threshold_min);
    uint32_t tempP = (uint32_t) red_info->parameters.drop_maX_P * PROB_SCALE
                     * (sampleLen  - red_info->parameters.threshold_min)
                     / (red_info->parameters.threshold_max - red_info->parameters.threshold_min);

    // Next Prob = tempP / (1 - count*tempP)
    // This will increase probability and

    //Calculate first divider part
    uint32_t Prob = red_info->count * tempP;

    //Check that divider it is not  >= 0
    if (Prob >=  PROB_SCALE_MAX) {

        red_info->count = 0;
        return true;
    }

    //Calculate only when count * tempP is smaller than scaler
    Prob = (tempP * PROB_SCALE_MAX) / (PROB_SCALE_MAX - Prob);
    if (Prob > randLIB_get_random_in_range(0, PROX_MAX_RANDOM)) {
        //Drop packet
        red_info->count = 0;
        return true;
    }

    //Increment count next round check
    red_info->count++;
    return false;

}
