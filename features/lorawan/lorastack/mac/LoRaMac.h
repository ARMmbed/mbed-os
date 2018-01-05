/**
 * \file      LoRaMac.h
 *
 * \brief     LoRa MAC layer implementation
 *
 * \copyright Revised BSD License, see LICENSE.TXT file include in the project
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013 Semtech
 *
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jaeckle ( STACKFORCE )
 *
 * \defgroup  LORAMAC LoRa MAC layer implementation
 *            This module specifies the API implementation of the LoRaMAC layer.
 *            This is a placeholder for a detailed description of the LoRaMac
 *            layer and the supported features.
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __LORAMAC_H__
#define __LORAMAC_H__

#include "lorawan/system/LoRaWANTimer.h"
#include "netsocket/LoRaRadio.h"
#include "lorastack/phy/LoRaPHY.h"
#include "lorawan/system/lorawan_data_structures.h"

/*!
 * Check the MAC layer state every MAC_STATE_CHECK_TIMEOUT in ms.
 */
#define MAC_STATE_CHECK_TIMEOUT                     1000

/*!
 * The maximum number of times the MAC layer tries to get an acknowledge.
 */
#define MAX_ACK_RETRIES                             8

/*!
 * The frame direction definition for uplink communications.
 */
#define UP_LINK                                     0

/*!
 * The frame direction definition for downlink communications.
 */
#define DOWN_LINK                                   1


/*!
 * LoRaMAC max EIRP (dBm) table.
 */
static const uint8_t LoRaMacMaxEirpTable[] = { 8, 10, 12, 13, 14, 16, 18, 20, 21, 24, 26, 27, 29, 30, 33, 36 };


/*!
 * \brief   LoRaMAC layer initialization
 *
 * \details In addition to the initialization of the LoRaMAC layer, this
 *          function initializes the callback primitives of the MCPS and
 *          MLME services. Every data field of \ref LoRaMacPrimitives_t must be
 *          set to a valid callback function.
 *
 * \param   primitives [in] - A pointer to the structure defining the LoRaMAC
 *                            event functions. Refer to \ref LoRaMacPrimitives_t.
 *
 * \param   callbacks [in] - A pointer to the structure defining the LoRaMAC
 *                        callback functions. Refer to \ref LoRaMacCallback_t.
 *
 * \param   phy [in]- A pointer to the selected PHY layer.
 *
 * \param   queue [in]- A pointer to the application provided EventQueue.
 *
 * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
 *          \ref LORAMAC_STATUS_OK
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID
 *          \ref LORAMAC_STATUS_REGION_NOT_SUPPORTED.
 */
LoRaMacStatus_t LoRaMacInitialization(LoRaMacPrimitives_t *primitives,
                                      LoRaMacCallback_t *callbacks,
                                      LoRaPHY *phy,
                                      events::EventQueue *queue);

/*!
 * \brief   Queries the LoRaMAC whether it is possible to send the next frame with
 *          a given payload size. The LoRaMAC takes the scheduled MAC commands into
 *          account and reports when the frame can be sent.
 *
 * \param   size [in]- The size of the applicable payload to be sent next.
 * \param   txInfo  [out] - The structure \ref LoRaMacTxInfo_t contains
 *                         information on the actual maximum payload possible
 *                         (according to the configured datarate or the next
 *                         datarate according to ADR), and the maximum frame
 *                         size, taking the scheduled MAC commands into account.
 *
 * \retval  `LoRaMacStatus_t` The status of the operation. When the parameters are
 *          not valid, the function returns \ref LORAMAC_STATUS_PARAMETER_INVALID.
 *          In case of a length error caused by the applicable payload in combination
 *          with the MAC commands, the function returns \ref LORAMAC_STATUS_LENGTH_ERROR.
 *          Please note that if the size of the MAC commands in the queue do
 *          not fit into the payload size on the related datarate, the LoRaMAC will
 *          omit the MAC commands.
 *          If the query is valid, and the LoRaMAC is able to send the frame,
 *          the function returns \ref LORAMAC_STATUS_OK.
 */
LoRaMacStatus_t LoRaMacQueryTxPossible( uint8_t size, LoRaMacTxInfo_t* txInfo );

/*!
 * \brief   LoRaMAC channel add service.
 *
 * \details Adds a new channel to the channel list and activates the ID in
 *          the channel mask. Please note that this functionality is not available
 *          in all regions. Information on the allowed ranges is available at the LoRaWAN Regional Parameters V1.0.2rB.
 *
 * \param   id [in] - The ID of the channel.
 *
 * \param   params [in] - The channel parameters to set.
 *
 * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
 *          \ref LORAMAC_STATUS_OK
 *          \ref LORAMAC_STATUS_BUSY
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID
 */
LoRaMacStatus_t LoRaMacChannelAdd( uint8_t id, ChannelParams_t params );

/*!
 * \brief   LoRaMAC channel remove service.
 *
 * \details Deactivates the ID in the channel mask.
 *
 * \param   id - Id of the channel.
 *
 * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
 *          \ref LORAMAC_STATUS_OK
 *          \ref LORAMAC_STATUS_BUSY
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID
 */
LoRaMacStatus_t LoRaMacChannelRemove( uint8_t id );

/*!
 * \brief   LoRaMAC multicast channel link service.
 *
 * \details Links a multicast channel into the linked list.
 *
 * \param   [in] channelParam - The multicast channel parameters to link.
 *
 * \retval  `LoRaMacStatus_t` The  status of the operation. The possible values are:
 *          \ref LORAMAC_STATUS_OK
 *          \ref LORAMAC_STATUS_BUSY
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID
 */
LoRaMacStatus_t LoRaMacMulticastChannelLink( MulticastParams_t *channelParam );

/*!
 * \brief   LoRaMAC multicast channel unlink service.
 *
 * \details Unlinks a multicast channel from the linked list.
 *
 * \param   [in] channelParam - The multicast channel parameters to unlink.
 *
 * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
 *          \ref LORAMAC_STATUS_OK
 *          \ref LORAMAC_STATUS_BUSY
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID
 */
LoRaMacStatus_t LoRaMacMulticastChannelUnlink( MulticastParams_t *channelParam );

/*!
 * \brief   LoRaMAC MIB-GET.
 *
 * \details The MAC information base service to get the attributes of the LoRaMac layer.
 *
 *          The following code-snippet shows how to use the API to get the
 *          parameter `AdrEnable`, defined by the enumeration type
 *          \ref MIB_ADR.
 * \code
 * MibRequestConfirm_t mibReq;
 * mibReq.Type = MIB_ADR;
 *
 * if( LoRaMacMibGetRequestConfirm( &mibReq ) == LORAMAC_STATUS_OK )
 * {
 *   // LoRaMAC updated the parameter mibParam.AdrEnable
 * }
 * \endcode
 *
 * \param   [in] mibGet - The MIB-GET request to perform. Refer to \ref MibRequestConfirm_t.
 *
 * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
 *          \ref LORAMAC_STATUS_OK
 *          \ref LORAMAC_STATUS_SERVICE_UNKNOWN
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID
 */
LoRaMacStatus_t LoRaMacMibGetRequestConfirm( MibRequestConfirm_t *mibGet );

/*!
 * \brief   LoRaMAC MIB-SET.
 *
 * \details The MAC information base service to set the attributes of the LoRaMac layer.
 *
 *          The following code-snippet shows how to use the API to set the
 *          parameter `AdrEnable`, defined by the enumeration type
 *          \ref MIB_ADR.
 *
 * \code
 * MibRequestConfirm_t mibReq;
 * mibReq.Type = MIB_ADR;
 * mibReq.Param.AdrEnable = true;
 *
 * if( LoRaMacMibGetRequestConfirm( &mibReq ) == LORAMAC_STATUS_OK )
 * {
 *   // LoRaMAC updated the parameter
 * }
 * \endcode
 *
 * \param   [in] mibSet - The MIB-SET request to perform. Refer to \ref MibRequestConfirm_t.
 *
 * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
 *          \ref LORAMAC_STATUS_OK
 *          \ref LORAMAC_STATUS_BUSY
 *          \ref LORAMAC_STATUS_SERVICE_UNKNOWN
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID
 */
LoRaMacStatus_t LoRaMacMibSetRequestConfirm( MibRequestConfirm_t *mibSet );

/*!
 * \brief   LoRaMAC MLME request
 *
 * \details The MAC layer management entity handles the management services. The
 *          following code-snippet shows how to use the API to perform a
 *          network join request.
 *
 * \code
 * static uint8_t DevEui[] =
 * {
 *   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
 * };
 * static uint8_t AppEui[] =
 * {
 *   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
 * };
 * static uint8_t AppKey[] =
 * {
 *   0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
 *   0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
 * };
 *
 * MlmeReq_t mlmeReq;
 * mlmeReq.Type = MLME_JOIN;
 * mlmeReq.Req.Join.DevEui = DevEui;
 * mlmeReq.Req.Join.AppEui = AppEui;
 * mlmeReq.Req.Join.AppKey = AppKey;
 *
 * if( LoRaMacMlmeRequest( &mlmeReq ) == LORAMAC_STATUS_OK )
 * {
 *   // Service started successfully. Waiting for the Mlme-Confirm event
 * }
 * \endcode
 *
 * \param   [in] mlmeRequest - The MLME request to perform. Refer to \ref MlmeReq_t.
 *
 * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
 *          \ref LORAMAC_STATUS_OK
 *          \ref LORAMAC_STATUS_BUSY
 *          \ref LORAMAC_STATUS_SERVICE_UNKNOWN
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID
 *          \ref LORAMAC_STATUS_NO_NETWORK_JOINED
 *          \ref LORAMAC_STATUS_LENGTH_ERROR
 *          \ref LORAMAC_STATUS_DEVICE_OFF
 */
LoRaMacStatus_t LoRaMacMlmeRequest( MlmeReq_t *mlmeRequest );

/*!
 * \brief   LoRaMAC MCPS request
 *
 * \details The MAC Common Part Sublayer handles the data services. The following
 *          code-snippet shows how to use the API to send an unconfirmed
 *          LoRaMAC frame.
 *
 * \code
 * uint8_t myBuffer[] = { 1, 2, 3 };
 *
 * McpsReq_t mcpsReq;
 * mcpsReq.Type = MCPS_UNCONFIRMED;
 * mcpsReq.Req.Unconfirmed.fPort = 1;
 * mcpsReq.Req.Unconfirmed.fBuffer = myBuffer;
 * mcpsReq.Req.Unconfirmed.fBufferSize = sizeof( myBuffer );
 *
 * if( LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK )
 * {
 *   // Service started successfully. Waiting for the MCPS-Confirm event
 * }
 * \endcode
 *
 * \param   [in] mcpsRequest - The MCPS request to perform. Refer to \ref McpsReq_t.
 *
 * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
 *          \ref LORAMAC_STATUS_OK
 *          \ref LORAMAC_STATUS_BUSY
 *          \ref LORAMAC_STATUS_SERVICE_UNKNOWN
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID
 *          \ref LORAMAC_STATUS_NO_NETWORK_JOINED
 *          \ref LORAMAC_STATUS_LENGTH_ERROR
 *          \ref LORAMAC_STATUS_DEVICE_OFF
 */
LoRaMacStatus_t LoRaMacMcpsRequest( McpsReq_t *mcpsRequest );

/**
 * \brief LoRaMAC layer provides its callback functions for
 *        PHY layer
 *
 * \return Pointer to callback functions for radio events
 */
radio_events_t *GetPhyEventHandlers();

#if defined(LORAWAN_COMPLIANCE_TEST)
/**
 * \brief   LoRaMAC set tx timer.
 *
 * \details Sets up a timer for next transmission (application specific timers).
 *
 * \param   [in] NextTxTime - Periodic time for next uplink.

 * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
 *          \ref LORAMAC_STATUS_OK
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID
 */
LoRaMacStatus_t LoRaMacSetTxTimer( uint32_t NextTxTime );

/**
 * \brief   LoRaMAC stop tx timer.
 *
 * \details Stops the next tx timer.
 *
 * \retval  `LoRaMacStatus_t` The status of the operation. The possible values are:
 *          \ref LORAMAC_STATUS_OK
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID
 */
LoRaMacStatus_t LoRaMacStopTxTimer( );
#endif

#endif // __LORAMAC_H__
