/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*************************************************************************************************
 * Includes Section
 ************************************************************************************************/

#include "sw_timer.h"
#include "fsl_interrupt_manager.h"
#include "fsl_clock_manager.h"
#include "fsl_pit_driver.h"

/*************************************************************************************************
 * Defines & Macros Section
 ************************************************************************************************/

#ifndef SW_TIMER_NUMBER_CHANNELS

/*! Number of channels to be used as timers. The maximum value of channels supported is 32.*/
#define SW_TIMER_NUMBER_CHANNELS            (32)

/*! Defines the count unit in ms of the software timer.*/
#define SW_TIMER_COUNT_UNIT_MS              (1)

#endif

/*! Return if the timer channel is reserved.*/
#define IS_TIMER_CHANNEL_RESERVED(chann)  (gs_areChannelsEnabled & (channel_enabler_t)(1 << chann))

/*************************************************************************************************
 * Typedef Section
 ************************************************************************************************/

#if (SW_TIMER_NUMBER_CHANNELS <= 8)

/*! Defines the size of variable that stores "enable" flags of each channel.*/
typedef uint8_t channel_enabler_t;

#else 
    #if (SW_TIMER_NUMBER_CHANNELS <= 16)
        
/*! Defines the size of variable that stores "enable" flags of each channel.*/
typedef uint16_t channel_enabler_t;
        
    #else 
        
        #if (SW_TIMER_NUMBER_CHANNELS <= 32)
            
/*! Defines the size of variable that stores "enable" flags of each channel.*/
typedef uint32_t channel_enabler_t;
            
        #else
            #error "Cannot handle more than 32 auxiliary software timer channels."
        #endif

    #endif
#endif

/*************************************************************************************************
 * Global Constants Section
 ************************************************************************************************/


/*************************************************************************************************
 * Static Constants Section
 ************************************************************************************************/
/** Constant to select the PIT timer channel to use for timebase. Channels 0 and 1 have special
 * hardware trigger capabilities, so they should be avoided.
 */
static const uint8_t kSWTimerPITChannel = FSL_FEATURE_PIT_TIMER_COUNT - 1;

/*************************************************************************************************
 * Global Variables Section
 ************************************************************************************************/


/*************************************************************************************************
 * Static Variables Section
 ************************************************************************************************/

/*! Free running counter that everyone can read.*/
static volatile time_free_counter_t gs_freeCounter;

/*! Independent counters of software timer module. A counter is defined for every channel.*/
static volatile time_counter_t gs_channelCounters[SW_TIMER_NUMBER_CHANNELS];

/*! Variable used to indicate which channels are enabled. Bit 0 corresponds to channel 0, bit 1 to*/
/*! channel 1, etc.*/
static volatile channel_enabler_t gs_areChannelsEnabled;


/*************************************************************************************************
 * Functions Section
 ************************************************************************************************/

/* See sw_timer.h for documentation of this function.*/
uint32_t sw_timer_init_service(void)
{
    pit_user_config_t pitConfig;
  
    /* All channels are disabled*/
    gs_areChannelsEnabled = 0;
    
    /* Init free running counter*/
    gs_freeCounter = 0;
    
    /* Define PIT channel init structure. */
    pitConfig.isInterruptEnabled = true;
    pitConfig.isTimerChained = false;
    pitConfig.periodUs = 1000;/* Set 1ms period */

    /* Init PIT module and enable all timers run in debug mode.*/
    pit_init_module(true);

    /* Init PIT channel. */
    pit_init_channel(kSWTimerPITChannel, &pitConfig);

    /* Register PIT callback function.*/
    pit_register_isr_callback_function(kSWTimerPITChannel, sw_timer_update_counters);

    /* Start timer counting. */
    pit_timer_start(kSWTimerPITChannel);
    
    return kSwTimerStatusSuccess;
}

/* See sw_timer.h for documentation of this function.*/
void sw_timer_shutdown_service(void)
{
    pit_shutdown();
}

/* See sw_timer.h for documentation of this function.*/
uint8_t sw_timer_reserve_channel(void)
{
    static uint8_t lastFreeChannel = 0;
    uint8_t searchIndex;
    uint8_t newReservedChannel;
    
    /* Initialize search index with the last free channel to search faster for a free channel*/
    searchIndex = lastFreeChannel;
    
    /* Not channel available by default*/
    newReservedChannel = kSwTimerChannelNotAvailable;
    
    /* Searching for a free channel*/
    do {
        /* The channel is free*/
        if(!IS_TIMER_CHANNEL_RESERVED(searchIndex))
        {
            /* Set channel as reserved*/
            newReservedChannel = searchIndex;
            gs_areChannelsEnabled |= (1u << newReservedChannel);
            
            /* Update last free channel with the next channel*/
            lastFreeChannel = newReservedChannel + 1;
            
            /* Flag of the last channel has been checked and need to start over with channel 0*/
            if (lastFreeChannel == SW_TIMER_NUMBER_CHANNELS)
            {
                lastFreeChannel = 0;
            }
        }
        /* The channel is already reserved*/
        else
        {
            searchIndex = (searchIndex + 1) % SW_TIMER_NUMBER_CHANNELS;
        }
        
    }while((searchIndex != lastFreeChannel) && 
           (kSwTimerChannelNotAvailable == newReservedChannel));
    
    return newReservedChannel;
}

/* See sw_timer.h for documentation of this function.*/
sw_timer_channel_status_t sw_timer_get_channel_status(uint8_t timerChannel)
{
    sw_timer_channel_status_t channelStatus;
    
    /* Is it a valid channel?*/
    if(SW_TIMER_NUMBER_CHANNELS > timerChannel)
    {
        /* The channel is reserved*/
        if(IS_TIMER_CHANNEL_RESERVED(timerChannel))
        {
            /* The timeout has expired*/
            if(0 >= gs_channelCounters[timerChannel])
            {
                channelStatus = kSwTimerChannelExpired;
            }
            /* The counter is still counting*/
            else
            {
                channelStatus = kSwTimerChannelStillCounting;
            }
        }
        /* The channel is not reserved*/
        else
        {
            channelStatus = kSwTimerChannelIsDisable;
        }
    }
    /* The channel is not valid*/
    else
    {
        channelStatus = kSwTimerChannelNotAvailable;
    }
    
    return channelStatus;
}

/* See sw_timer.h for documentation of this function.*/
uint32_t sw_timer_start_channel(uint8_t timerChannel, time_counter_t timeout)
{
    uint32_t startStatus;
    
    /* Is it a valid channel?*/
    if(SW_TIMER_NUMBER_CHANNELS > timerChannel)
    {
        /* Set the given timeout in the corresponding channel counter. The timeout should be a */
        /* multiple of SW_TIMER_COUNT_UNIT_MS, otherwise it will be taken the integer part of the */
        /* division.*/
        gs_channelCounters[timerChannel] = timeout / SW_TIMER_COUNT_UNIT_MS;
        
        startStatus = kSwTimerStatusSuccess;
    }
    else
    {
        startStatus = kSwTimerStatusInvalidChannel;
    }
    
    return startStatus;
}

/* See sw_timer.h for documentation of this function.*/
uint32_t sw_timer_release_channel(uint8_t timerChannel)
{
    uint32_t releaseStatus;
    
    /* Is it a valid channel?*/
    if(SW_TIMER_NUMBER_CHANNELS > timerChannel)
    {
        /* Set channel as disable*/
        gs_areChannelsEnabled &= (~(1u << timerChannel));
        releaseStatus = kSwTimerStatusSuccess;
    }
    else
    {
        releaseStatus = kSwTimerStatusInvalidChannel;
    }

    return releaseStatus;
}

/* See sw_timer.h for documentation of this function.*/
time_free_counter_t sw_timer_get_free_counter(void)
{
    return gs_freeCounter;
}

/* See sw_timer.h for documentation of this function.*/
void sw_timer_update_counters(void)
{
    uint8_t index;
    
    index = SW_TIMER_NUMBER_CHANNELS; 
    
    do {
        --index;
        
        /* Decrement all counters. To determine if the timeout has expired call */
        --gs_channelCounters[index];
        
    } while(0 != index);
    
    /* Increment free running counter*/
    ++gs_freeCounter;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

