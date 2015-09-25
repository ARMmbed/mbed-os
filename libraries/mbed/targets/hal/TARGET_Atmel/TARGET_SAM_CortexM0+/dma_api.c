/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "mbed_assert.h"
#include "system.h"
#include "dma_api.h"
#include "dma_api_HAL.h"

#include <math.h>

#include "cmsis.h"
#include "pinmap.h"

/**
 * \internal
 * Structure redefinition, already defined in dma.c.
 * Redefining as that definition is not available here
 */
struct _dma_module {
    volatile bool _dma_init;
    volatile uint32_t allocated_channels;
    uint8_t free_channels;
};

extern struct _dma_module _dma_inst;
extern uint8_t g_sys_init;

static struct dma_instance_s dma_channels[CONF_MAX_USED_CHANNEL_NUM];

/**
 * \internal
 * Get resource index from channel id
 *
 * @param[in] channelid Valid DMA channel id
 * @return index to DMA instance
 */
static uint8_t get_index_from_id(int channelid)
{
    /* Sanity check arguments */
    MBED_ASSERT(channelid < CONF_MAX_USED_CHANNEL_NUM);

    uint8_t i;

    for (i=0; i<CONF_MAX_USED_CHANNEL_NUM; i++) {
        if ((dma_channels[i].status & DMA_ALLOCATED)
                && (dma_channels[i].resource.channel_id == channelid)) {
            break;
        }
    }

    return i;
}

/**
 * \internal
 * Handler function for DMA callback
 *
 * @param[in] resource pointer to the resource
 * @return void
 */
static void dma_handler(const struct dma_resource* const resource)
{
    MBED_ASSERT(resource);
    void (*callback_func)(void);

    uint8_t channelid = resource->channel_id;
    uint8_t channel_index;

    channel_index = get_index_from_id(channelid);
    if (channel_index >= CONF_MAX_USED_CHANNEL_NUM) {
        return;
    }

    callback_func = dma_channels[channel_index].handler;
    if (callback_func) {
        callback_func();
    }
}

/**
 * \internal
 * Configure a DMA channel for specified resource
 *
 * @param[in] channel_index index to the resource
 * @return void
 */
static void configure_dma_resource(uint8_t channel_index)
{
    /* Sanity check arguments */
    MBED_ASSERT(channel_index < CONF_MAX_USED_CHANNEL_NUM);

    enum status_code ret;
    struct dma_resource_config config;

    if (dma_channels[channel_index].status & DMA_ALLOCATED) {
        return;
    }

    /* Get default configuration for DMA */
    dma_get_config_defaults(&config);

    /* Allocate a free channel */
    ret = dma_allocate(&dma_channels[channel_index].resource, &config);

    if (ret == STATUS_OK) {
        dma_channels[channel_index].status = DMA_ALLOCATED;
    }
}

/** Setup a DMA descriptor for specified resource
 *
 * @param[in] channel_index		DMA channel id
 * @param[in] src				source address
 * @param[in] src_inc_enable	source address auto increment enable flag
 * @param[in] desc				destination address
 * @param[in] desc_inc_enable	destination address auto increment enable flag
 * @param[in] length			length of data to be transferred
 * @param[in] beat_size			beat size to be set
 * @return void
 */
void dma_setup_transfer(uint8_t channelid, uint32_t src, bool src_inc_enable, uint32_t desc, bool desc_inc_enable, uint32_t length, uint8_t beat_size)
{
    enum status_code result;
    uint8_t channel_index;
    struct dma_descriptor_config descriptor_config;

    /* Sanity check arguments */
    MBED_ASSERT(channelid < CONF_MAX_USED_CHANNEL_NUM);
    MBED_ASSERT(src);
    MBED_ASSERT(desc);

    channel_index = get_index_from_id(channelid);

    dma_descriptor_get_config_defaults(&descriptor_config);

    if (beat_size <= 8) {
        descriptor_config.beat_size = DMA_BEAT_SIZE_BYTE;
    } else if ((beat_size > 8) && (beat_size <= 16)) {
        descriptor_config.beat_size = DMA_BEAT_SIZE_HWORD;
    } else {
        descriptor_config.beat_size = DMA_BEAT_SIZE_WORD;
    }
    descriptor_config.block_transfer_count = length;
    descriptor_config.source_address = src;
    descriptor_config.destination_address = desc;

    /* Source address auto-increment is enabled by default */
    if (!src_inc_enable) {
        descriptor_config.src_increment_enable = false;
    }

    /* Destination address auto-increment is enabled by default */
    if (!desc_inc_enable) {
        descriptor_config.dst_increment_enable = false;
    }

    dma_descriptor_create(&dma_channels[channel_index].descriptor, &descriptor_config);

    /* Add descriptor to resource */
    if (dma_channels[channel_index].resource.descriptor == NULL) {
        /* Multiple calls to this function without releasing already allocated channel is not handled now */
        result = dma_add_descriptor(&dma_channels[channel_index].resource, &dma_channels[channel_index].descriptor);
        if (result != STATUS_OK) {
            dma_channels[channel_index].status |= DMA_ERROR;
        }
    }
}


/** Initialize the DMA
 *
 * Configures clock for DMAC
 */
void dma_init()
{
    int i;

    if (g_sys_init == 0) {
        system_init();
        g_sys_init = 1;
    }

    if (!_dma_inst._dma_init) {
        for (i=0; i<CONF_MAX_USED_CHANNEL_NUM; i++) {
            dma_channels[i].status = DMA_NOT_USED;
        }
    }
    /* Do nothing for now. ASF does the clock init when allocating channel */
}

/** Allocates channel for DMA
 *
 * Allocates channel for DMA with specified capability
 * @param[in] capabilities     Capability of DMA channel
 */
int dma_channel_allocate(uint32_t capabilities)
{
    uint8_t channel_index = 0;

    for (channel_index=0; channel_index<CONF_MAX_USED_CHANNEL_NUM; channel_index++) {
        if (dma_channels[channel_index].status == DMA_NOT_USED) {
            break;
        }
    }

    if (channel_index != CONF_MAX_USED_CHANNEL_NUM) {
        configure_dma_resource(channel_index);
        if (dma_channels[channel_index].status & DMA_ALLOCATED) {
            return dma_channels[channel_index].resource.channel_id;
        }
    }

    /* Couldn't find a channel. */
    return DMA_ERROR_OUT_OF_CHANNELS;
}

/** Start DMA transfer
 *
 * Kick starts transfer in DMA channel with specified channel id
 * @param[in] channelid    Channel id of DMA channel
 * @return zero if success otherwise non zero
 */
bool dma_start_transfer(int channelid)
{
    /* Sanity check arguments */
    MBED_ASSERT(channelid < CONF_MAX_USED_CHANNEL_NUM);

    uint8_t channel_index;

    channel_index = get_index_from_id(channelid);

    if (channel_index >= CONF_MAX_USED_CHANNEL_NUM) {
        /* Return invalid value for now */
        return false;
    }

    if (!(dma_channels[channel_index].status & DMA_ALLOCATED)) {
        /* DMA not allocated, return invalid value for now */
        return false;
    }

    /* Start DMA transfer */
    if (STATUS_OK != dma_start_transfer_job(&dma_channels[channel_index].resource)) {
        /* Error in starting DMA transfer */
        return false;
    }

    return true;
}

/** DMA channel busy check
 *
 * To check whether DMA channel is busy with a job or not
 * @param[in] channelid    Channel id of DMA channel
 * @return non zero if busy otherwise zero
 */
bool dma_busy(int channelid)
{
    int res = 0;
    /* Sanity check arguments */
    MBED_ASSERT(channelid < CONF_MAX_USED_CHANNEL_NUM);

    uint8_t channel_index;

    channel_index = get_index_from_id(channelid);

    if (channel_index >= CONF_MAX_USED_CHANNEL_NUM) {
        /* This channel is not active! return zero for now */
        res = 0;
    }

    return dma_is_busy(&dma_channels[channel_index].resource);
}

/** DMA channel transfer completion check
 *
 * To check whether DMA channel job is completed or not
 * @param[in] channelid    Channel id of DMA channel
 * @return non zero if busy otherwise zero
 */
bool dma_is_transfer_complete(int channelid)
{
    int res = 0;
    /* Sanity check arguments */
    MBED_ASSERT(channelid < CONF_MAX_USED_CHANNEL_NUM);

    uint8_t channel_index;

    channel_index = get_index_from_id(channelid);

    if (channel_index >= CONF_MAX_USED_CHANNEL_NUM) {
        /* This channel is not active! return zero for now */
        res = 0;
    }

    return (STATUS_OK == dma_get_job_status(&dma_channels[channel_index].resource));
}

/** Registers callback function for DMA
 *
 * Registers callback function for DMA for specified events
 * @param[in] channelid    Channel id of DMA channel
 * @param[in] handler      Callback function pointer
 * @param[in] event        Events mask
 * @return void
 */
void dma_set_handler(int channelid, uint32_t handler, uint32_t event)
{
    /* Sanity check arguments */
    MBED_ASSERT(channelid < CONF_MAX_USED_CHANNEL_NUM);

    uint8_t channel_index;

    channel_index = get_index_from_id(channelid);

    if (channel_index >= CONF_MAX_USED_CHANNEL_NUM) {
        /* Return for now */
        return;
    }

    dma_channels[channel_index].handler = handler;
    if (event & DMA_TRANSFER_ERROR) {
        dma_register_callback(&dma_channels[channel_index].resource, dma_handler, DMA_CALLBACK_TRANSFER_ERROR);
    }
    if (event & DMA_TRANSFER_COMPLETE) {
        dma_register_callback(&dma_channels[channel_index].resource, dma_handler, DMA_CALLBACK_TRANSFER_DONE);
    }

    /* Set interrupt vector if someone have removed it */
    NVIC_SetVector(DMAC_IRQn, (uint32_t)DMAC_Handler);
    /* Enable interrupt */
    NVIC_EnableIRQ(DMAC_IRQn);
}

/** Frees an allocated DMA channel
 *
 * Frees an already allocated DMA channel with specified channel id
 * @param[in] channelid    Channel id of DMA channel to be disabled
 * @return zero if success
 */
int dma_channel_free(int channelid)
{
    /* Sanity check arguments */
    MBED_ASSERT(channelid < CONF_MAX_USED_CHANNEL_NUM);

    uint8_t channel_index;

    channel_index = get_index_from_id(channelid);

    if (STATUS_OK == dma_free(&dma_channels[channel_index].resource)) {
        dma_channels[channel_index].status = DMA_NOT_USED;
        dma_channels[channel_index].resource.descriptor = NULL;
        return 0;
    } else {
        /* Return invalid value for now */
        return -1;
    }
}


