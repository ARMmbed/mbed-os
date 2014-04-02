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
#ifndef __EDMA_HAL_H__
#define __EDMA_HAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_edma_features.h"
#include "fsl_device_registers.h"

/*!
 * @addtogroup edma_hal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief eDMA status */
typedef enum _edma_status
{
    kStatus_EDMA_Success = 0U,
    kStatus_EDMA_InvalidArgument = 1U,  /*!< Parameter is not available for the current configuration.*/
    kStatus_EDMA_Fail = 2U              /*!< Function operation failed.*/
} edma_status_t;

/*! @brief eDMA TCD control configuration */
typedef union EdmaTCDControl {
    struct {
        uint16_t reserve1 : 1;
        uint16_t majorInterrupt : 1;        /*!< Interrupt after the major loop is complete. */
        uint16_t halfInterrupt : 1;         /*!< Interrupt after half of the major loop is complete. */
        uint16_t disabledDmaRequest : 1;    /*!< Disabled DMA request after the major loop is complete. */
        uint16_t enabledScatterGather : 1;  /*!< Enable scatter/gather processing. */
        uint16_t enableMajorLink : 1;       /*!< Enabled major link after the major loop is complete. */
        uint16_t reserve2 : 1;
        uint16_t reserve3 : 1;
        uint16_t majorLinkChannel : 4;      /*!< Major link channel number*/
        uint16_t reserve4 : 2;
        uint16_t bandwidthControl : 2;      /*!< Bandwidth control configuration */
    } U;
    uint16_t B;
} edma_tcd_control_t;

/*! @brief eDMA TCD Minor loop mapping configuration */
typedef struct EdmaMinorLoopOffset {
    bool isEnableSourceMinorloop;
    bool isEnableDestMinorloop;
    uint32_t offset;
} edma_minorloop_offset_config_t;

/*! @brief Priority limitation of the eDMA channel */
typedef enum _edma_channel_priority {
    kEdmaChannelPriority = 16
} edma_channel_priority_t;

/*! @brief eDMA modulo configuration */
typedef enum _edma_modulo {
    kEdmaModuloDisable = 0x0U,
    kEdmaModulo2bytes = 0x1U,
    kEdmaModulo4bytes = 0x2U,
    kEdmaModulo8bytes = 0x3U,
    kEdmaModulo16bytes = 0x4U,
    kEdmaModulo32bytes = 0x5U,
    kEdmaModulo64bytes = 0x6U,
    kEdmaModulo128bytes = 0x7U,
    kEdmaModulo256bytes = 0x8U,
    kEdmaModulo512bytes = 0x9U,
    kEdmaModulo1Kbytes = 0xaU,
    kEdmaModulo2Kbytes = 0xbU,
    kEdmaModulo4Kbytes = 0xcU,
    kEdmaModulo8Kbytes = 0xdU,
    kEdmaModulo16Kbytes = 0xeU,
    kEdmaModulo32Kbytes = 0xfU,
    kEdmaModulo64Kbytes = 0x10U,
    kEdmaModulo128Kbytes = 0x11U,
    kEdmaModulo256Kbytes = 0x12U,
    kEdmaModulo512Kbytes = 0x13U,
    kEdmaModulo1Mbytes = 0x14U,
    kEdmaModulo2Mbytes = 0x15U,
    kEdmaModulo4Mbytes = 0x16U,
    kEdmaModulo8Mbytes = 0x17U,
    kEdmaModulo16Mbytes = 0x18U,
    kEdmaModulo32Mbytes = 0x19U,
    kEdmaModulo64Mbytes = 0x1aU,
    kEdmaModulo128Mbytes = 0x1bU,
    kEdmaModulo256Mbytes = 0x1cU,
    kEdmaModulo512Mbytes = 0x1dU,
    kEdmaModulo1Gbytes = 0x1eU,
    kEdmaModulo2Gbytes = 0x1fU
} edma_modulo_t;

/*! @brief eDMA transfer size configuration */
typedef enum _edma_transfer_size {
    kEdmaTransferSize1bytes = 0x0U,
    kEdmaTransferSize2bytes = 0x1U,
    kEdmaTransferSize4bytes = 0x2U,
    kEdmaTransferSize16bytes = 0x4U,
    kEdmaTransferSize32bytes = 0x5U
} edma_transfer_size_t;

/*! @brief Error status of the eDMA module */
typedef union EdmaErrorStatusAll {
    struct {
        uint32_t destinationBusError : 1;               /*!< Bus error on destination address */
        uint32_t sourceBusError : 1;                    /*!< Bus error on the SRC address */
        uint32_t scatterOrGatherConfigurationError : 1; /*!< Error on the Scatter/Gather address */
        uint32_t nbyteOrCiterConfigurationError : 1;    /*!< NBYTES/CITER configuration error */
        uint32_t destinationOffsetError : 1;            /*!< Destination offset error */
        uint32_t destinationAddressError : 1;           /*!< Destination address error */
        uint32_t sourceOffsetError : 1;                 /*!< Source offset error */
        uint32_t sourceAddressError : 1;                /*!< Source address error */
        uint32_t errorChannel : 5;                      /*!< Error channel number of the cancelled
                                                             channel number */
        uint32_t _reserved1 : 1;
        uint32_t channelPriorityError : 1;              /*!< Channel priority error */
        uint32_t groupPriorityError : 1;                /*!< Group priority error */
        uint32_t transferCancelledError : 1;            /*!< Transfer cancelled */
        uint32_t _reserved0 : 14;
        uint32_t orOfAllError : 1;                      /*!< Logical OR  all ERR status bits */
    } U;
    uint32_t B;
} edma_error_status_all_t;

/*! @brief Bandwidth control configuration */
typedef enum _edma_bandwidth_configuration {
    kEdmaBandwidthStallNone = 0,    /*!< No eDMA engine stalls. */
    kEdmaBandwidthStall4Cycle = 2,  /*!< eDMA engine stalls for 4 cycles after each read/write. */
    kEdmaBandwidthStall8Cycle = 3   /*!< eDMA engine stalls for 4 cycles after each read/write. */
} edma_bandwidth_configuration_t;

/*! @brief eDMA TCD */
typedef struct EdmaSoftwareTcd {
    uint32_t SADDR;
    uint16_t SOFF;
    uint16_t ATTR;
    union {
        uint32_t NBYTES_MLNO;
        uint32_t NBYTES_MLOFFNO;
        uint32_t NBYTES_MLOFFYES;
    };
    uint32_t SLAST;
    uint32_t DADDR;
    uint16_t DOFF;
    union {
        uint16_t CITER_ELINKNO;
        uint16_t CITER_ELINKYES;
    };
    uint32_t DLAST_SGA;
    uint16_t CSR;
    union {
        uint16_t BITER_ELINKNO;
        uint16_t BITER_ELINKYES;
    };
} edma_software_tcd_t;

/*! @brief eDMA group priority */
typedef enum _edma_group_priority {
    kEdmaGroup0Priority0Group1Priority1,
    kEdmaGroup0Priority1Group1Priority0
} edma_group_priority_t;

/*! @brief DMA configuration structure */
typedef struct EdmaConfiguration {
    bool isEnableMinorLoopping;         /*!< Enabled the minor loop mapping. */
    bool isEnableContinuousMode;        /*!< Enabled the continuous mode. */
	bool isHaltOnError;                 /*!< Halt if error happens. */
    bool isEnableRoundrobinArbitration; /*!< Enabled round robin or fixed priority arbitration. */
    bool isEnableDebug;                /*!< Enabled Debug mode. */
#if (FSL_FEATURE_DMA_CHANNEL_GROUP_COUNT > 0x1U)
    edma_group_priority_t groupPriority;
    bool isEnableGroupRoundrobinArbitration;
#endif
} edma_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! 
 * @name EDMA HAL common configuration
 * @{
 */

/*!
 * @brief Initializes the eDMA module.
 *
 * The function  configures the eDMA module with the corresponding global configuration. The
 * configuration is for all channels in this module. 
 *
 * @param module eDMA module
 * @param init Init data structure
 */
void edma_hal_init(uint32_t instance, const edma_config_t *init);

/*!
 * @brief Cancels the remaining data transfer. Stops the executing channel and forces the minor loop
 * to finish. The cancellation takes effect after the last write of the current read/write sequence.
 * The CX clears itself after the cancel has been honored. This cancel retires the channel 
 * normally as if the minor loop had completed.
 *
 * @param instance eDMA module
 */
static inline void edma_hal_cancel_transfer(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_CX(instance, 1U);
    while (BR_DMA_CR_CX(instance))
    {}
}

/*!
 * @brief Cancels the remaining data transfer. Stops the executing channel and forces the minor loop to
 * finish. The cancellation takes effect after the last write of the current read/write sequence. The
 * ECX bit clears itself after the cancel is honored. In addition to cancelling the transfer, ECX
 * treats the cancel as an error condition. 
 *
 * @param instance eDMA module
 */
static inline void edma_hal_error_cancel_transfer(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_ECX(instance, 1U);
    while(BR_DMA_CR_ECX(instance))
    {}
}

/*!
 * @brief Enables/Disables the minor loop mapping.
 *
 * If enabled, the NBYTES is redefined to include individual enable fields. And the NBYTES field. The
 * individual enable fields allow the minor loop offset to be applied to the source address, the 
 * destination address, or both. The NBYTES field is reduced when either offset is enabled.
 *
 * @param instance eDMA module
 * @param isEnabled Enable or disable.
 */
static inline void edma_hal_set_minor_loop_mapping(uint32_t instance, bool isEnabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_EMLM(instance, isEnabled);
}


#if (FSL_FEATURE_DMA_CHANNEL_GROUP_COUNT > 0x1U)
/*!
 * @brief Configures the group priority.
 *
 * @param module eDMA module
 * @param isContinuous Whether the minor loop finish  triggers itself.
 */
static inline void edma_hal_set_group_priority(uint32_t instance, edma_group_priority_t groupPriority)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);

    if (groupPriority == kEdmaGroup0Priority0Group1Priority1)
    {
        BW_DMA_CR_GRP0PRI(instance, 0U);
        BW_DMA_CR_GRP1PRI(instance, 1U);
    }
    else
    {
        BW_DMA_CR_GRP0PRI(instance, 1U);
        BW_DMA_CR_GRP1PRI(instance, 0U);

    }
}

/*!
 * @brief The fixed priority arbitration is used for the group selection.
 *
 * @param instance eDMA module
 */
static inline void edma_hal_set_fixed_priority_group_arbitration(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_ERGA(instance, 0U);
}

/*!
 * @brief The round robin arbitration is used for the group selection.
 *
 * @param instance eDMA module
 */
static inline void edma_hal_set_roundrobin_group_arbitration(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_ERGA(instance, 1U);
}

#endif

/*!
 * @brief Configures the continuous mode. If set, a minor loop channel link does not
 * go through the channel arbitration before being activated again. Upon minor loop completion, the
 * channel activates again if that channel has a minor loop channel link enabled and the link
 * channel is itself. 
 *
 * @param module eDMA module
 * @param isContinuous Whether the minor loop finish  triggers itself.
 */
static inline void edma_hal_set_continuous_mode(uint32_t instance, bool isContinuous)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_CLM(instance, isContinuous);
}

/*!
 * @brief Halts the DMA Operations.
 *
 * Stalls the start of any new channels. Executing channels are allowed to complete. 
 *
 * @param instance eDMA module.
 */
static inline void edma_hal_halt(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_HALT(instance, 1U);
}

/*!
 * @brief Clears the halt bit.
 *
 * If a previous eDMA channel is halted, clear operation would resume it back to executing.
 *
 * @param instance eDMA module.
 */
static inline void edma_hal_clear_halt(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_HALT(instance, 0U);
}

/*!
 * @brief Halts the eDMA module when an error occurs.
 *
 * An error causes the HALT bit to be set. Subsequently, all service requests are ignored until the
 * HALT bit is cleared.
 *
 * @param instance eDMA module.
 * @param isHaltOnError halts or does not halt when an error occurs. 
 */
static inline void edma_hal_set_halt_on_error(uint32_t instance, bool isHaltOnError)
{	
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_HOE(instance, isHaltOnError);
}

/*!
 * @brief The fixed priority arbitration is used for the channel selection.
 *
 * @param instance eDMA module.
 */
static inline void edma_hal_set_fixed_priority_channel_arbitration(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_ERCA(instance, 0U);
}

/*!
 * @brief The round robin arbitration is used for the channel selection.
 *
 * @param instance eDMA module.
 */
static inline void edma_hal_set_roundrobin_channel_arbitration(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_ERCA(instance, 1U);
}

/*!
 * @brief Enables/Disables the eDMA DEBUG mode.
 *
 * When in debug mode, the DMA stalls the start of a new 
 * channel. Executing channels are allowed to complete. Channel execution resumes 
 * either when the system exits debug mode or when the EDBG bit is cleared. 
 *
 * @param instance eDMA module.
 */
static inline void edma_hal_set_debug_mode(uint32_t instance, bool isEnabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CR_EDBG(instance, isEnabled);
}

/*!
 * @brief Gets the error status of the eDMA module. The detailed reason is listed along with the error
 * channel.
 *
 * @param instance eDMA module
 * @return Detailed information of the error type in the eDMA module.
 */
static inline uint32_t edma_hal_get_error_status(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    return HW_DMA_ES_RD(instance);
}

/*!
 * @brief Disables the interrupt when an error happens on any of channel in the eDMA module.
 *
 * @param instance eDMA module
 */
static inline void edma_hal_disable_all_enabled_error_interrupt(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    BW_DMA_CEEI_CAEE(instance, 1U);
}

/*!
 * @brief Enables an interrupt when an error happens on any channel in the eDMA module.
 *
 * @param instance eDMA module
 */
static inline void edma_hal_enable_all_channel_error_interrupt(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);

    BW_DMA_SEEI_SAEE(instance, 1U);
}

/*!
 * @brief Disables the DMA request for all eDMA channels.
 *
 * @param instance eDMA module
 */
static inline void edma_hal_disable_all_channel_dma_request(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    HW_DMA_CERQ_WR(instance, DMA_CERQ_CAER_MASK);
}

/*!
 * @brief Enables the DMA request for all eDMA channels.
 *
 * @param instance eDMA module
 */
static inline void edma_hal_enable_all_channel_dma_request(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    HW_DMA_SERQ_WR(instance, DMA_SERQ_SAER_MASK);
}

/*!
 * @brief Clears the done status for all eDMA channels.
 *
 * @param instance eDMA module
 */
static inline void edma_hal_clear_all_channel_done_status(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    HW_DMA_CDNE_WR(instance, DMA_CDNE_CADN_MASK);
}

/*!
 * @brief Triggers all channel start bits.
 *
 * @param instance eDMA module
 */
static inline void edma_hal_trigger_all_channel_start_bit(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    HW_DMA_SSRT_WR(instance, DMA_SSRT_SAST_MASK);
}

/*!
 * @brief Clears the error status for all eDMA channels.
 *
 * @param instance eDMA module
 */
static inline void edma_hal_clear_all_channel_error_status(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    HW_DMA_CERR_WR(instance, DMA_CERR_CAEI_MASK);
}

/*!
 * @brief Clears an interrupt request for all eDMA channels.
 *
 * @param instance eDMA module
 */
static inline void edma_hal_clear_all_channel_interrupt_request(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    HW_DMA_CINT_WR(instance, DMA_CINT_CAIR_MASK);
}

/*!
 * @brief Gets the interrupt status for all eDMA channels.
 *
 * @param instance eDMA module
 * @return 32 bit data. Every bit stands for an eDMA channel. For example, bit 0 stands for channel 0 and so on.
 */
static inline uint32_t edma_hal_get_all_channel_interrupt_request_status(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    return HW_DMA_INT_RD(instance);
}

/*!
 * @brief Gets the channel error status for all eDMA channels.
 *
 * @param instance eDMA module
 * @return 32 bit data. Every bit stands for an eDMA channel. For example, bit 0 stands for channel 0 and so on.
 */
static inline uint32_t edma_hal_get_all_channel_error_status(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    return HW_DMA_ERR_RD(instance);
}

/*!
 * @brief Gets the status of the DMA request for all DMA channels.
 *
 * @param instance eDMA module
 * @return 32 bit data. Every bit stands for an eDMA channel. For example, bit 0 stands for channel 0 and so on.
 */
static inline uint32_t edma_hal_get_all_channel_dma_request_status(uint32_t instance)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    return HW_DMA_HRS_RD(instance);
}

/* @} */

/*! 
 * @name EDMA HAL channel configuration. 
 * @{
 */

/*!
 * @brief Check whether the channel DMA request is enabled.
 *
 * Check whether the DMA request of a specified channel  is enabled.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 *
 * @return True stands for enabled. False stands for disabled.
 */
static inline bool edma_hal_check_dma_request_enable_status(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);

    return ((HW_DMA_ERQ_RD(instance)>>channel) & 1U); 
}

/*!
 * @brief Disables an interrupt when an error happens in the eDMA channel.
 *
 * Disables an error interrupt for the eDMA module.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 */
static inline void edma_hal_disable_error_interrupt(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    HW_DMA_CEEI_WR(instance, DMA_CEEI_CEEI(channel));
}

/*!
 * @brief Enables an interrupt when an error happens in the eDMA channel.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 */
static inline void edma_hal_enable_error_interrupt(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    HW_DMA_SEEI_WR(instance, DMA_SEEI_SEEI(channel));
}

/*!
 * @brief Disables the DMA request for an eDMA channel.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 */
static inline void edma_hal_disable_dma_request(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    HW_DMA_CERQ_WR(instance, DMA_CERQ_CERQ(channel)); 
}

/*!
 * @brief Enables the DMA request for a specified eDMA channel.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 */
static inline void edma_hal_enable_dma_request(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    HW_DMA_SERQ_WR(instance, DMA_SERQ_SERQ(channel));
}

/*!
 * @brief Clears the done status for an eDMA channel.
 *
 * The DONE status of the DMA channel  is cleared. If the scatter/gather state is
 * enabled, the DONE status in CSR can be cleared but the global DONE statue is
 * still set. This function is to clear the global done state.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 */
static inline void edma_hal_clear_done_status(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    HW_DMA_CDNE_WR(instance, DMA_CDNE_CDNE(channel));
}

/*!
 * @brief Starts an eDMA channel manually.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 */
static inline void edma_hal_trigger_start_bit(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    HW_DMA_SSRT_WR(instance, DMA_SSRT_SSRT(channel));
}

/*!  * @brief Clears an error status for the eDMA channel.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 */
static inline void edma_hal_clear_error_status(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    HW_DMA_CERR_WR(instance, DMA_CERR_CERR(channel));
}

/*!
 * @brief Clears an interrupt request for the eDMA channel.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 */
static inline void edma_hal_clear_interrupt_request(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    HW_DMA_CINT_WR(instance, DMA_CINT_CINT(channel));
}

#if (FSL_FEATURE_DMA_ASYNCHRO_REQUEST_CHANNEL_COUNT > 0x0U)
/*!
 * @brief Enables/Disables an asynchronous request in stop mode.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 */
static inline void edma_hal_set_asynchronous_request_in_stop_mode(
                    uint32_t instance, uint32_t channel, bool isEnabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    if(isEnabled) 
    {
        HW_DMA_EARS_SET(instance, 1U << channel);
    }
    else
    {
        HW_DMA_EARS_CLR(instance, 1U << channel);
    }
}
#endif

/*!
 * @brief Configures the preempt feature for an eDMA channel.
 *
 * If it is disabled, the DMA channel can't suspend a lower priority channel.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param preempt configuration mode for preempt
 */
static inline void edma_hal_set_channel_preemp_ability(
                uint32_t instance, uint32_t channel, bool isDisabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_DCHPRIn_DPA(instance, HW_DMA_DCHPRIn_CHANNEL(channel), isDisabled);
}

/*!
 * @brief Configures the preempt feature for the eDMA channel.
 *
 * If enabled, channel can be temporarily suspended by a higher priority channel.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param preempt configuration mode for preempt
 */
static inline void edma_hal_set_channel_preemption_ability(uint32_t instance, uint32_t channel, bool isEnabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_DCHPRIn_ECP(instance, HW_DMA_DCHPRIn_CHANNEL(channel), isEnabled);
}

/*!
 * @brief Configures the eDMA channel priority.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param priority Priority of the DMA channel. Different channels should have different priority inside a
 * group.
 */
static inline void edma_hal_set_channel_priority(
                uint32_t instance, uint32_t channel, uint32_t priority)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);

    BW_DMA_DCHPRIn_CHPRI(instance, HW_DMA_DCHPRIn_CHANNEL(channel), priority);
}

/* @} */

/*! 
 * @name eDMA HAL hardware TCD configuration
 * @{
 */

/*!
 * @brief Configures the source address for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param address memory address pointing to the source data
 */
static inline void edma_hal_htcd_configure_source_address(
                uint32_t instance, uint32_t channel, uint32_t address)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_SADDR_SADDR(instance, channel, address);
}

/*!
 * @brief Configures the source address signed offset for the hardware TCD.
 *
 * Sign-extended offset applied to the current source address to form the next-state value as each
 * source read is complete.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param offset signed-offset
 */
static inline void edma_hal_htcd_configure_source_offset(
                uint32_t instance, uint32_t channel, int16_t offset)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_SOFF_SOFF(instance, channel, offset);
}

/*!
 * @brief Configures the source modulo for the hardware TCD.
 *
 * The value defines a specific address range specified as the value after the SADDR + SOFF
 * calculation is performed on the original register value. Setting this field provides the ability
 * to implement a circular data. For data queues requiring power-of-2 size bytes, the
 * queue should start at a 0-modulo-size address and the SMOD field should be set to the appropriate
 * value for the queue, freezing the desired number of upper address bits. The value programmed into
 * this field specifies the number of the lower address bits allowed to change. For a circular queue
 * application, the SOFF is typically set to the transfer size to implement post-increment addressing
 * with SMOD function restricting the addresses to a 0-modulo-size range.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param modulo enum type for an allowed modulo
 */
static inline void edma_hal_htcd_configure_source_modulo(
                uint32_t instance, uint32_t channel, edma_modulo_t modulo)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_ATTR_SMOD(instance, channel, modulo);
}

/*!
 * @brief Configures the source data transfersize for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param size enum type for transfer size
 */
static inline void edma_hal_htcd_configure_source_transfersize(
                uint32_t instance, uint32_t channel, edma_transfer_size_t size)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_ATTR_SSIZE(instance, channel, size);
}

/*!
 * @brief Configures the destination modulo for the hardware TCD.
 *
 * The value defines a specific address range as the value after the DADDR + DOFF
 * calculation is performed on the original register value. Setting this field provides the ability
 * to implement a circular data. For data queues requiring power-of-2 size bytes, the
 * queue should start at a 0-modulo-size address and the SMOD field should be set to the appropriate
 * value for the queue, freezing the desired number of upper address bits. The value programmed into
 * this field specifies the number of lower address bits allowed to change. For a circular queue
 * application, the SOFF is typically set to the transfer size to implement post-increment addressing
 * with DMOD function restricting the addresses to a 0-modulo-size range.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param modulo enum type for an allowed modulo
 */
static inline void edma_hal_htcd_configure_dest_modulo(
                uint32_t instance, uint32_t channel, edma_modulo_t modulo) 
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_ATTR_DMOD(instance, channel, modulo);
}

/*!
 * @brief Configures the destination data transfersize for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param size enum type for the transfer size
 */
static inline void edma_hal_htcd_configure_dest_transfersize(
                uint32_t instance, uint32_t channel, edma_transfer_size_t size)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_ATTR_DSIZE(instance, channel, size);
}

/*!
 * @brief Configures the nbytes if minor loop mapping is disabled  for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param nbytes Number of bytes to be transferred in each service request of the channel
 */
static inline void edma_hal_htcd_configure_nbytes_minorloop_disabled(
                uint32_t instance, uint32_t channel, uint32_t nbytes)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_NBYTES_MLNO_NBYTES(instance, channel, nbytes); 
}

/*!
 * @brief Configures the nbytes if the minor loop mapping is enabled and offset is disabled for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param nbytes Number of bytes to be transferred in each service request of the channel
 */
static inline void edma_hal_htcd_configure_nbytes_minorloop_enabled_offset_disabled(
                uint32_t instance, uint32_t channel, uint32_t nbytes)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_NBYTES_MLOFFNO_NBYTES(instance, channel, nbytes);
}

/*!
 * @brief Configures the nbytes if the minor loop mapping is enabled and offset is enabled for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param nbytes Number of bytes to be transferred in each service request of the channel
 */
static inline void edma_hal_htcd_configure_nbytes_minorloop_enabled_offset_enabled(
                uint32_t instance, uint32_t channel, uint32_t nbytes)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_NBYTES_MLOFFYES_NBYTES(instance, channel, nbytes);
}

/*!
 * @brief Gets the nbytes configuration data.
 *
 * This function  decides whether the minor loop mapping is enabled or whether the source/dest
 * minor loop mapping is enabled. Then, the nbytes are returned accordingly.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @return nbytes configuration
 */
uint32_t edma_hal_htcd_get_nbytes_configuration(uint32_t instance, uint32_t channel);


/*!
 * @brief Configures the minorloop offset for the hardware TCD.
 *
 * Configures both the enable bits and the offset value. If neither source nor dest offset is enabled,
 * offset  is not configured.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param config Configuration data structure for the minorloop offset
 */
static inline void edma_hal_htcd_configure_minorloop_offset(
        uint32_t instance, uint32_t channel, edma_minorloop_offset_config_t config)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_NBYTES_MLOFFYES_SMLOE(instance, channel, config.isEnableSourceMinorloop);
    BW_DMA_TCDn_NBYTES_MLOFFYES_DMLOE(instance, channel, config.isEnableDestMinorloop);
    if ((config.isEnableSourceMinorloop == true) || (config.isEnableDestMinorloop == true))
    {
        BW_DMA_TCDn_NBYTES_MLOFFYES_MLOFF(instance, channel, config.offset);
    }
}

/*!
 * @brief Configures the last source address adjustment for the hardware TCD.
 *
 * Adjustment value added to the source address at the completion of the major iteration count. This
 * value can be applied to restore the source address to the initial value, or adjust the address to
 * reference the next data structure.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param size adjustment value
 */
static inline void edma_hal_htcd_configure_source_last_adjustment(
                uint32_t instance, uint32_t channel, int32_t size)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_SLAST_SLAST(instance, channel, size);
}

/*!
 * @brief Configures the destination address for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param address memory address pointing to destination data
 */
static inline void edma_hal_htcd_configure_dest_address(
                uint32_t instance, uint32_t channel, uint32_t address)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_DADDR_DADDR(instance, channel, address);
}

/*!
 * @brief Configures the destination address signed offset for the hardware TCD.
 *
 * Sign-extended offset applied to the current source address to form the next-state value as each
 * destination write is complete.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param offset signed-offset
 */
static inline void edma_hal_htcd_configure_dest_offset(
                uint32_t instance, uint32_t channel, int16_t offset)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_DOFF_DOFF(instance, channel, offset);
}

/*!
 * @brief Configures the last source address adjustment or the memory address for the next transfer
 * control for the hardware TCD.
 *
 * If a scatter/gather feature is enabled (edma_hal_htcd_set_scatter_gather_process()):
 *
 * This address points to the beginning of a 0-modulo-32 byte region containing the next transfer
 * control descriptor to be loaded into this channel. The channel reload is performed as the major
 * iteration count completes. The scatter/gather address must be 0-modulo-32-byte. Otherwise, a
 * configuration error is reported.
 *
 * else:
 *
 * An adjustment value added to the source address at the completion of the major iteration count. This
 * value can be applied to restore the source address to the initial value, or adjust the address to
 * reference the next data structure.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param size adjustment value
 */
static inline void edma_hal_htcd_configure_dest_last_adjustment_or_scatter_address(
        uint32_t instance, uint32_t channel, uint32_t address)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_DLASTSGA_DLASTSGA(instance, channel, address);
}

/*!
 * @brief Configures the bandwidth for the hardware TCD.
 *
 * Throttles the amount of bus bandwidth consumed by the eDMA. In general, as the eDMA processes the
 * minor loop, it continuously generates read/write sequences until the minor count is exhausted.
 * This field forces the eDMA to stall after the completion of each read/write access to control the
 * bus request bandwidth seen by the crossbar switch.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param bandwidth enum type for bandwidth control
 */
static inline void edma_hal_htcd_configure_bandwidth(
        uint32_t instance, uint32_t channel, edma_bandwidth_configuration_t bandwidth)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CSR_BWC(instance, channel, bandwidth);
}

/*!
 * @brief Configures the major link channel number for the hardware TCD.
 *
 * If the majorlink is enabled, after the major loop counter is exhausted, the eDMA engine initiates a
 * channel service request at the channel defined by these six bits by setting that channel start
 * bits.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param majorchannel channel number for major link
 */
static inline void edma_hal_htcd_configure_majorlink_channel(
        uint32_t instance, uint32_t channel, uint32_t majorchannel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CSR_MAJORLINKCH(instance, channel, majorchannel);
}

/*!
 * @brief Gets the major link channel for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @return major link channel number
 */
static inline uint32_t edma_hal_htcd_get_majorlink_channel( 
        uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    return BR_DMA_TCDn_CSR_MAJORLINKCH(instance, channel);
}

/*!
 * @brief Enables/Disables the major link channel feature for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_htcd_set_majorlink(uint32_t instance, uint32_t channel, bool isEnabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CSR_MAJORELINK(instance, channel, isEnabled);
}

/*!
 * @brief Enables/Disables the scatter/gather feature for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_htcd_set_scatter_gather_process(
        uint32_t instance, uint32_t channel, bool isEnabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CSR_ESG(instance, channel, isEnabled);
}

/*!
 * @brief Checks whether the scatter/gather feature is enabled for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @return True stand for enabled. False stands for disabled. 
 */
static inline bool edma_hal_htcd_is_gather_scatter_enabled(
        uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    return BR_DMA_TCDn_CSR_ESG(instance, channel);

}

/*!
 * @brief Disables/Enables the DMA request after the major loop completes for the hardware TCD.
 *
 * If disabled, the eDMA hardware automatically clears the corresponding DMA request when the
 * current major iteration count reaches zero.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param isDisabled Disable/Enable.
 */
static inline void edma_hal_htcd_set_disable_dma_request_after_tcd_done(
        uint32_t instance, uint32_t channel, bool isDisabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CSR_DREQ(instance, channel, isDisabled);
}  

/*!
 * @brief Enables/Disables the half complete interrupt for the hardware TCD.
 *
 * If set, the channel generates an interrupt request by setting the appropriate bit in the
 * interrupt register when the current major iteration count reaches the halfway point. Specifically,
 * the comparison performed by the eDMA engine is (CITER == (BITER >> 1)). This half-way point
 * interrupt request is provided to support the double-buffered schemes or other types of data movement
 * where the processor needs an early indication of the transfer's process.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_htcd_set_half_complete_interrupt(
        uint32_t instance, uint32_t channel, bool isEnabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CSR_INTHALF(instance, channel, isEnabled);
}

/*!
 * @brief Enables/Disables the interrupt after the major loop completes for the hardware TCD.
 *
 * If enabled, the channel generates an interrupt request by setting the appropriate bit in the 
 * interrupt register when the current major iteration count reaches zero.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_htcd_set_complete_interrupt(
        uint32_t instance, uint32_t channel, bool isEnabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CSR_INTMAJOR(instance, channel, isEnabled);
}

/*!
 * @brief Triggers the start bits for the hardware TCD.
 *
 * The eDMA hardware automatically clears this flag after the channel begins execution.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 */
static inline void edma_hal_htcd_trigger_channel_start(
        uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CSR_START(instance, channel, 1); 
}

/*!
 * @brief Checks whether the channel is running for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @return True stands for running. False stands for not. 
 */
static inline bool edma_hal_htcd_is_channel_active(
        uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    return BR_DMA_TCDn_CSR_ACTIVE(instance, channel); 
}

/*!
 * @brief Checks whether the major loop is exhausted for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @return True stands for running. False stands for not. 
 */
static inline bool edma_hal_htcd_is_channel_done(
        uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    return BR_DMA_TCDn_CSR_DONE(instance, channel);
}

/*!
 * @brief Enables/Disables the channel link after the minor loop for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_htcd_set_minor_link(
        uint32_t instance, uint32_t channel, bool isEnabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_BITER_ELINKYES_ELINK(instance, channel, isEnabled);
}

/*!
 * @brief Enables/Disables the channel link after the minor loop in the current register for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_htcd_set_current_minor_link(
        uint32_t instance, uint32_t channel, bool isEnabled)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CITER_ELINKYES_ELINK(instance, channel, isEnabled);
}

/*!
 * @brief Configures the minor loop link channel for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param minorchannel minor loop link channel
 */
static inline void edma_hal_htcd_configure_minor_link_channel(
        uint32_t instance, uint32_t channel, uint32_t minorchannel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_BITER_ELINKYES_LINKCH(instance, channel, minorchannel);
}

/*!
 * @brief Configures the minor loop link channel in the current register for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param minorchannel minor loop link channel
 */
static inline void edma_hal_htcd_configure_current_minor_link_channel(
        uint32_t instance, uint32_t channel, uint32_t minorchannel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CITER_ELINKYES_LINKCH(instance, channel, minorchannel);
}

/*!
 * @brief Configures the major count if the minor loop channel link is disabled for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param count major loop count
 */
static inline void edma_hal_htcd_configure_majorcount_minorlink_disabled(
        uint32_t instance, uint32_t channel, uint32_t count)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_BITER_ELINKNO_BITER(instance, channel, count);
}

/*!
 * @brief Configures the current major count if the minor loop channel link is disabled for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param count major loop count
 */
static inline void edma_hal_htcd_configure_current_majorcount_minorlink_disabled(
        uint32_t instance, uint32_t channel, uint32_t count)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CITER_ELINKNO_CITER(instance, channel, count);
}

/*!
 * @brief Configures the major count if the minor loop channel link is enabled for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param count major loop count
 */
static inline void edma_hal_htcd_configure_majorcount_minorlink_enabled(
        uint32_t instance, uint32_t channel, uint32_t count)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_BITER_ELINKYES_BITER(instance, channel, count);
}

/*!
 * @brief Configures the current major count if the minor loop channel link is enabled for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param count major loop count
 */
static inline void edma_hal_htcd_configure_current_majorcount_minorlink_enabled(
        uint32_t instance, uint32_t channel, uint32_t count)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    BW_DMA_TCDn_CITER_ELINKYES_CITER(instance, channel, count);
}

/*!
 * @brief Gets the current major loop count.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @return current major loop count
 */
uint32_t edma_hal_htcd_get_current_major_count(uint32_t instance, uint32_t channel);

/*!
 * @brief Gets the beginning major loop count.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @return begin major loop count
 */
uint32_t edma_hal_htcd_get_begin_major_count(uint32_t instance, uint32_t channel);

/*!
 * @brief Gets the bytes number not to be transferred for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @return data bytes to be transferred
 */
uint32_t edma_hal_htcd_get_unfinished_bytes(uint32_t instance, uint32_t channel);

/*!
 * @brief Gets the number of already transferred bytes for the hardware TCD.
 *
 * @param instance eDMA module
 * @param channel eDMA channel
 * @return data bytes to be transferred
 */
uint32_t edma_hal_htcd_get_finished_bytes(uint32_t instance, uint32_t channel);
/* @} */

/*! 
 * @name eDMA HAL software TCD configuration
 * @{
 */

/*!
 * @brief Configures the source address for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param address memory address pointing to the source data
 */
static inline void edma_hal_stcd_configure_source_address(
        edma_software_tcd_t *stcd, uint32_t address)
{
    assert(stcd);
    stcd->SADDR = DMA_SADDR_SADDR(address); 
}

/*!
 * @brief Configures the source address for the software TCD.
 *
 * @param STCD memory pointing to  the eDMA software TCD
 * @param address memory address pointing to the source data
 */
static inline void edma_hal_stcd_configure_source_offset(
                edma_software_tcd_t *stcd, uint32_t offset)
{
    assert(stcd);
    stcd->SOFF = DMA_SOFF_SOFF(offset);
}

/*!
 * @brief Configures the source modulo for the software TCD.
 *
 * The value defines a specific address range as the value after the SADDR + SOFF
 * calculation is performed on the original register value. Setting this field provides the ability
 * to implement a circular data queue. For data queues requiring power-of-2 size bytes, the
 * queue should start at a 0-modulo-size address and the SMOD field should be set to the appropriate
 * value for the queue, freezing the desired number of upper address bits. The value programmed into
 * this field specifies the number of lower address bits allowed to change. For a circular queue
 * application, the SOFF is typically set to the transfer size to implement post-increment addressing
 * with SMOD function restricting the addresses to a 0-modulo-size range.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param modulo enum type for the allowed modulo
 */
static inline void edma_hal_stcd_configure_source_modulo(
                edma_software_tcd_t *stcd, edma_modulo_t modulo)
{
    assert(stcd);
    stcd->ATTR = (stcd->ATTR & ~DMA_ATTR_SMOD_MASK) | DMA_ATTR_SMOD(modulo);
}

/*!
 * @brief Configures the source data transfersize for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param size enum type for transfer size
 */
static inline void edma_hal_stcd_configure_source_transfersize(
                edma_software_tcd_t *stcd, edma_transfer_size_t size)
{
    assert(stcd);
    stcd->ATTR = (stcd->ATTR & ~DMA_ATTR_SSIZE_MASK) | DMA_ATTR_SSIZE(size);
}

/*!
 * @brief Configures the destination modulo for the software TCD.
 *
 * The value defines a specific address range as the value after the DADDR + DOFF
 * calculation is performed on the original register value. Setting this field provides the ability
 * to implement a circular data queue easily. For data queues requiring power-of-2 size bytes, the
 * queue should start at a 0-modulo-size address and the SMOD field should be set to the appropriate
 * value for the queue, freezing the desired number of upper address bits. The value programmed into
 * this field specifies the number of lower address bits allowed to change. For a circular queue
 * application, the SOFF is typically set to the transfer size to implement post-increment addressing
 * with DMOD function restricting the addresses to a 0-modulo-size range.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param modulo enum type for allowed modulo
 */
static inline void edma_hal_stcd_configure_dest_modulo(
                edma_software_tcd_t *stcd, edma_modulo_t modulo) 
{
    assert(stcd);
    stcd->ATTR = (stcd->ATTR & ~DMA_ATTR_DMOD_MASK) | DMA_ATTR_DMOD(modulo);
}

/*!
 * @brief Configures the destination data transfersize for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param size enum type for transfer size
 */
static inline void edma_hal_stcd_configure_dest_transfersize(
                edma_software_tcd_t *stcd, edma_transfer_size_t size)
{
    assert(stcd);
    stcd->ATTR = (stcd->ATTR & ~DMA_ATTR_DSIZE_MASK) | DMA_ATTR_DSIZE(size);
}

/*!
 * @brief Configures the nbytes if minor loop mapping is disabled the for software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param nbytes Number of bytes to be transferred in each service request of the channel
 */
static inline void edma_hal_stcd_configure_nbytes_minorloop_disabled(
                edma_software_tcd_t *stcd, uint32_t nbytes)
{
    assert(stcd);
    stcd->NBYTES_MLNO =
        (stcd->NBYTES_MLNO & ~DMA_NBYTES_MLNO_NBYTES_MASK) | DMA_NBYTES_MLNO_NBYTES(nbytes);
}

/*!
 * @brief Configures the nbytes if the minor loop mapping is enabled and offset is disabled for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param nbytes Number of bytes to be transferred in each service request of the channel
 */
static inline void edma_hal_stcd_configure_nbytes_minorloop_enabled_offset_disabled(
                edma_software_tcd_t *stcd, uint32_t nbytes)
{
    assert(stcd);
    stcd->NBYTES_MLOFFNO =
        (stcd->NBYTES_MLOFFNO & ~DMA_NBYTES_MLOFFNO_NBYTES_MASK) | DMA_NBYTES_MLOFFNO_NBYTES(nbytes);
}

/*!
 * @brief Configures the nbytes if minor loop mapping is enabled and offset is enabled for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD.
 * @param nbytes Number of bytes to be transferred in each service request of the channel.
 */
static inline void edma_hal_stcd_configure_nbytes_minorloop_enabled_offset_enabled(
                edma_software_tcd_t *stcd, uint32_t nbytes)
{
    assert(stcd);
    stcd->NBYTES_MLOFFYES =
        (stcd->NBYTES_MLOFFYES & ~DMA_NBYTES_MLOFFYES_NBYTES_MASK) | DMA_NBYTES_MLOFFYES_NBYTES(nbytes);
}

/*!
 * @brief Configures the minorloop offset for the software TCD.
 *
 * Configures both the enable bits and the offset value. If neither source nor destination offset is enabled,
 * offset  can't be configured.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param config Configuration data structure for the minorloop offset
 */
static inline void edma_hal_stcd_configure_minorloop_offset(
        edma_software_tcd_t *stcd, edma_minorloop_offset_config_t *config)
{
    assert(stcd);
    stcd->NBYTES_MLOFFYES =
        (stcd->NBYTES_MLOFFYES & ~(DMA_NBYTES_MLOFFYES_SMLOE_MASK | DMA_NBYTES_MLOFFYES_DMLOE_MASK)) |
        (((uint32_t)config->isEnableSourceMinorloop << DMA_NBYTES_MLOFFYES_SMLOE_SHIFT) |
         ((uint32_t)config->isEnableDestMinorloop << DMA_NBYTES_MLOFFYES_DMLOE_SHIFT));

    if ((config->isEnableSourceMinorloop == true) || (config->isEnableDestMinorloop == true))
    {
        stcd->NBYTES_MLOFFYES =
            (stcd->NBYTES_MLOFFYES & ~DMA_NBYTES_MLOFFYES_MLOFF_MASK) |
            DMA_NBYTES_MLOFFYES_MLOFF(config->offset);
    }
}

/*!
 * @brief Configures the last source address adjustment for the software TCD.
 *
 * Adjustment value added to the source address at the completion of the major iteration count. This
 * value can be applied to restore the source address to the initial value, or adjust the address to
 * reference the next data structure.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param size adjustment value
 */
static inline void edma_hal_stcd_configure_source_last_adjustment(
                edma_software_tcd_t *stcd, int32_t size)
{
    assert(stcd);
    stcd->SLAST = (stcd->SLAST & ~DMA_SLAST_SLAST_MASK) | DMA_SLAST_SLAST(size);
}

/*!
 * @brief Configures the destination address for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param address memory address pointing to destination data
 */
static inline void edma_hal_stcd_configure_dest_address(
                edma_software_tcd_t *stcd, uint32_t address)
{
    assert(stcd);
    stcd->DADDR = DMA_DADDR_DADDR(address); 
}

/*!
 * @brief Configures the destination address signed offset for the software TCD.
 *
 * Sign-extended offset applied to the current source address to form the next-state value as each
 * destination write is complete.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param offset signed-offset
 */
static inline void edma_hal_stcd_configure_dest_offset(
                edma_software_tcd_t *stcd, uint32_t offset)
{
    assert(stcd);
    stcd->DOFF = DMA_DOFF_DOFF(offset);
}

/*!
 * @brief Configures the last source address adjustment or the memory address for the next transfer
 * control for the software TCD.
 *
 * If the scatter/gather feature is enabled(edma_hal_htcd_set_scatter_gather_process()):
 *
 * This address points to the beginning of a 0-modulo-32 byte region containing the next transfer
 * control descriptor to be loaded into this channel. The channel reload is performed as the major
 * iteration count completes. The scatter/gather address must be 0-modulo-32-byte. Otherwise, a
 * configuration error is reported.
 *
 * else:
 *
 * Adjustment value added to the source address at the completion of the major iteration count. This
 * value can be applied to restore the source address to the initial value, or adjust the address to
 * reference the next data structure.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param size adjustment value
 */
static inline void edma_hal_stcd_configure_dest_last_adjustment_or_scatter_address(
        edma_software_tcd_t *stcd, uint32_t address)
{
    assert(stcd);
    stcd->DLAST_SGA = DMA_DLAST_SGA_DLASTSGA(address);
}

/*!
 * @brief Configures the bandwidth for the software TCD.
 *
 * Throttles the amount of bus bandwidth consumed by the eDMA. As the eDMA processes the
 * minor loop, it continuously generates read/write sequences until the minor count is exhausted.
 * This field forces the eDMA to stall after the completion of each read/write access to control the
 * bus request bandwidth seen by the crossbar switch.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param bandwidth enum type for bandwidth control
 */
static inline void edma_hal_stcd_configure_bandwidth(
        edma_software_tcd_t *stcd, edma_bandwidth_configuration_t bandwidth)
{
    assert(stcd);
    stcd->CSR = (stcd->CSR & ~DMA_CSR_BWC_MASK) | DMA_CSR_BWC(bandwidth);
}

/*!
 * @brief Configures the major link channel number for the software TCD.
 *
 * If majorlink is enabled, after the major loop counter is exhausted, the eDMA engine initiates a
 * channel service request at the channel defined by these six bits by setting that channel start
 * bits.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param majorchannel channel number for major link
 */
static inline void edma_hal_stcd_configure_majorlink_channel(
        edma_software_tcd_t *stcd, uint32_t majorchannel)
{
    assert(stcd);
    stcd->CSR = (stcd->CSR & ~DMA_CSR_MAJORLINKCH_MASK) | DMA_CSR_MAJORLINKCH(majorchannel);
}

/*!
 * @brief Enables/Disables the major link channel feature for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_stcd_set_majorlink(edma_software_tcd_t *stcd, bool isEnabled)
{
    assert(stcd);
    stcd->CSR = (stcd->CSR & ~DMA_CSR_MAJORELINK_MASK) |
        ((uint32_t)isEnabled << DMA_CSR_MAJORELINK_SHIFT);
}

/*!
 * @brief Enables/Disables the scatter/gather feature for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_stcd_set_scatter_gather_process(
        edma_software_tcd_t *stcd, bool isEnabled)
{
    assert(stcd);
    stcd->CSR = (stcd->CSR & ~DMA_CSR_ESG_MASK) | ((uint32_t)isEnabled << DMA_CSR_ESG_SHIFT);
}

/*!
 * @brief Disables/Enables the DMA request after the major loop completes for the software TCD.
 *
 * If disabled, the eDMA hardware automatically clears the corresponding DMA request when the
 * current major iteration count reaches zero.
 *
 * @param STCD memory pointing to the eDMA software TCD.
 * @param isDisabled Disable/Enable
 */
static inline void edma_hal_stcd_set_disable_dma_request_after_tcd_done(
        edma_software_tcd_t *stcd, bool isDisabled)
{
    assert(stcd);
    stcd->CSR = (stcd->CSR & ~DMA_CSR_DREQ_MASK) | ((uint32_t)isDisabled << DMA_CSR_DREQ_SHIFT);
}  

/*!
 * @brief Enables/Disables the half complete interrupt for the software TCD.
 *
 * If set, the channel generates an interrupt request by setting the appropriate bit in the
 * interrupt register when the current major iteration count reaches the halfway point. Specifically,
 * the comparison performed by the eDMA engine is (CITER == (BITER >> 1)). This half way point
 * interrupt request is provided to support the double-buffered schemes or other types of data movement
 * where the processor needs an early indication of the transfer process.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_stcd_set_half_complete_interrupt(
        edma_software_tcd_t *stcd, bool isEnabled)
{
    assert(stcd);
    stcd->CSR = (stcd->CSR & ~DMA_CSR_INTHALF_MASK) | ((uint32_t)isEnabled << DMA_CSR_INTHALF_SHIFT);
}

/*!
 * @brief Enables/Disables the interrupt after major loop complete for the software TCD.
 *
 * If enabled, the channel generates an interrupt request by setting the appropriate bit in the 
 * interrupt register when the current major iteration count reaches zero.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_stcd_set_complete_interrupt(
        edma_software_tcd_t *stcd, bool isEnabled)
{
    assert(stcd);
    stcd->CSR = (stcd->CSR & ~DMA_CSR_INTMAJOR_MASK) | ((uint32_t)isEnabled << DMA_CSR_INTMAJOR_SHIFT);
}

/*!
 * @brief Sets the trigger start bits for the software TCD.
 *
 */
static inline void edma_hal_stcd_trigger_channel_start(
        edma_software_tcd_t *stcd)
{
    assert(stcd);
    stcd->CSR |= DMA_CSR_START_MASK;
}

/*!
 * @brief Enables/Disables the channel link after the minor loop for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_stcd_set_minor_link(
        edma_software_tcd_t *stcd, bool isEnabled)
{
    assert(stcd);
    stcd->BITER_ELINKYES = (stcd->BITER_ELINKYES & ~DMA_BITER_ELINKYES_ELINK_MASK) |
                            ((uint32_t)isEnabled << DMA_BITER_ELINKYES_ELINK_SHIFT);
}

/*!
 * @brief Enables/Disables the current channel link after the minor loop for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param isEnabled Enable/Disable
 */
static inline void edma_hal_stcd_set_current_minor_link(
        edma_software_tcd_t *stcd, bool isEnabled)
{
    assert(stcd);
    stcd->CITER_ELINKYES = (stcd->CITER_ELINKYES & ~DMA_CITER_ELINKYES_ELINK_MASK) |
                            ((uint32_t)isEnabled << DMA_CITER_ELINKYES_ELINK_SHIFT);
}

/*!
 * @brief Configures the minor loop link channel for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param minorchannel minor loop link channel
 */
static inline void edma_hal_stcd_configure_minor_link_channel(
        edma_software_tcd_t *stcd, uint32_t minorchannel)
{
    assert(stcd);
    stcd->BITER_ELINKYES = (stcd->BITER_ELINKYES & ~DMA_BITER_ELINKYES_LINKCH_MASK) |
                            DMA_BITER_ELINKYES_LINKCH(minorchannel);
}

/*!
 * @brief Configures the minor loop link channel for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param minorchannel minor loop link channel.
 */
static inline void edma_hal_stcd_configure_current_minor_link_channel(
        edma_software_tcd_t *stcd, uint32_t minorchannel)
{
    assert(stcd);
    stcd->CITER_ELINKYES = (stcd->CITER_ELINKYES & ~DMA_CITER_ELINKYES_LINKCH_MASK) |
                            DMA_CITER_ELINKYES_LINKCH(minorchannel);
}

/*!
 * @brief Configures the major count if the minor loop channel link is disabled for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param count major loop count
 */
static inline void edma_hal_stcd_configure_majorcount_minorlink_disabled(
        edma_software_tcd_t *stcd, uint32_t count)
{
    assert(stcd);
    stcd->BITER_ELINKNO = (stcd->BITER_ELINKNO & ~DMA_BITER_ELINKNO_BITER_MASK) |
                            DMA_BITER_ELINKNO_BITER(count);
}

/*!
 * @brief Configure current major count if minor loop channel link is disabled for software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param count major loop count
 */
static inline void edma_hal_stcd_configure_current_majorcount_minorlink_disabled(
        edma_software_tcd_t *stcd, uint32_t count)
{
    assert(stcd);
    stcd->CITER_ELINKNO = (stcd->CITER_ELINKNO & ~DMA_CITER_ELINKNO_CITER_MASK) |
                            DMA_CITER_ELINKNO_CITER(count);
}

/*!
 * @brief Configures the major count if the minor loop channel link is enabled for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param count major loop count
 */
static inline void edma_hal_stcd_configure_majorcount_minorlink_enabled(
        edma_software_tcd_t *stcd, uint32_t count)
{
    assert(stcd);
    stcd->BITER_ELINKYES = (stcd->BITER_ELINKYES & ~DMA_BITER_ELINKYES_BITER_MASK) |
                            DMA_BITER_ELINKYES_BITER(count);
}

/*!
 * @brief Configures the current major count if the minor loop channel link is enabled for the software TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param count major loop count
 */
static inline void edma_hal_stcd_configure_current_majorcount_minorlink_enabled(
        edma_software_tcd_t *stcd, uint32_t count)
{
    assert(stcd);
    stcd->CITER_ELINKYES = (stcd->CITER_ELINKYES & ~DMA_CITER_ELINKYES_CITER_MASK) |
                            DMA_CITER_ELINKYES_CITER(count);
}

/*!
 * @brief Copy the software TCD configuration to the hardware TCD.
 *
 * @param STCD memory pointing to the eDMA software TCD
 * @param instance eDMA module
 * @param channel eDMA channel
 * @param STCD memory pointing to the software TCD
 */
void edma_hal_stcd_push_to_htcd(uint32_t instance, uint32_t channel, edma_software_tcd_t *stcd);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* __EDMA_HAL_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/

