/**
 * Copyright (c) 2019, Arm Limited and affiliates.
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

/** @addtogroup ClockSyncControlPackage
 * LoRaWAN Application Layer Clock Sync Protocol
 *  @{
 */

#ifndef MBED_OS_FEATURES_LORAWAN_PLUGINS_CLOCKSYNCCONTROLPACKAGE_H_
#define MBED_OS_FEATURES_LORAWAN_PLUGINS_CLOCKSYNCCONTROLPACKAGE_H_

#include "LoRaWANBase.h"
#include "lorawan_data_structures.h"

/**
 * Maximum size  for the in-bound buffer. If all the ClockSync commands
 * are sent in the down-link, the size will be 11 bytes
 */
#define MAX_CLK_SYNC_CMD_INBOUND_BUFFER_SIZE     11

/**
 * Maximum size for the out-bound buffer needs. If all the ClockSync responses
 * are sent in the up-link, the size will be 15 bytes
 */
#define MAX_CLK_SYNC_CMD_OUTBOUND_BUFFER_SIZE    15

/**
 * Response length for AppTimeAns command
 */
#define APP_TIME_ANS_LENGTH             6

/**
 * ClockSync command identifiers
 * For the exact meaning of these commands, please visit:
 * https://lora-alliance.org/resource-hub/lorawan-application-layer-clock-synchronization-specification-v100
 */
typedef enum {
    CLK_SYNC_PACKAGE_VERSION                = 0x00,
    CLK_SYNC_APP_TIME                       = 0x01,
    CLK_SYNC_APP_TIME_PERIODICITY           = 0x02,
    CLK_SYNC_FORCE_RESYNC                   = 0x03,
} clk_sync_ctrl_cmds;

/**
 * A structure containing response to a ClockSync operation.
 *
 * data - A pointer to a buffer containing ClockSync command  response
 * size - Size of the 'data' buffer
 * forced_resync_required
 *      - Set to true if the network is forcing a re-synchronization, that
 *        means the application layer has to request another clock sync request
 * forced_resync_nb_trans
 *      - Number of times a re-sync is required (for QoS purpose)
 */
typedef struct {
    uint8_t *data;
    bool forced_resync_required;
    uint8_t forced_resync_nb_trans;
    uint16_t size;
} clk_sync_response_t;

/** Clock synchronization plug-in
 * ClockSyncControlPackage is an implementation of LoRaWAN application layer
 * clock synch specification v1.0.0. It is presented as a plug-in to be used
 * with Mbed LoRaWAN stack. This plug-in should be used only with LoRaWAN
 * protocol specification v1.0.2 and earlier. For v1.0.3 and later, please use
 * 'DevTimeReq' MAC command.
 */
class ClockSyncControlPackage {
public:
    /**
     * CTOR
     */
    ClockSyncControlPackage();

    /**
     * DTOR
     */
    ~ClockSyncControlPackage();

    /** Send a clock sync request
     *
     * Requests the network for clock synchronization by issuing an 'AppTimeReq'
     * clock sync command. The network may or may not respond to this request.
     *
     * NOTE: 'AppTimeReq' command requires that the stack temporarily disables
     *        ADR and turns off QoS (i.e., NbTrans = 1). For simplicity, Mbed OS
     *        LoRaWAN stack disables ADR and turns off QoS for all the traffic on
     *        the ClockSync port 202. As soon as the transaction is done, the stack
     *        returns to normal state. If the user disabled ADR prior to this, it
     *        will remain disabled.
     *
     * @param ans_required    false: network will respond only if the clock has drifted
     *                        true: network has to respond even if there is negligible drift
     *
     * @return A pointer to 'clk_sync_response_t' which contains the response that the device
     *         should transmit at ClockSync port 202 along-with some meta data.
     */
    clk_sync_response_t *request_clock_sync(bool ans_required);

    /** Parses a ClockSync command sent by the network
     *
     * This API off-loads the parsing of ClockSync commands from the application.
     * Typical way of using this API would be to call it in application receive
     * handler when the incoming port is ClockSync port 202.
     *
     * The commands will be processed and an appropriate response will be
     * generated. The ownership of the response buffer is not transferred.
     * Application should take a copy of the response buffer if another request
     * is expected any time. For example in Class C mode, you may receive any time,
     * so there is a danger that the buffer may get overwritten.
     *
     * @param payload    A pointer to the buffer containing commands
     * @param size       Size of the buffer (cannot be larger than 11 bytes)
     *
     * @return A pointer to 'clk_sync_response_t' which contains the response that the device
     *         should transmit at ClockSync port 202 along-with some meta data.
     */
    clk_sync_response_t *parse(const uint8_t *payload, uint16_t size);

    /** Get the number of times a re-sync request should be sent
     *
     * The number is set by 'ForceDeviceResyncReq' command (in our code
     * CLK_SYNC_FORCE_RESYNC).
     *
     * Application is responsible to invoke 'request_clock_sync()' API to generate
     * a request multiple times to satisfy the need. The delay between consecutive
     * transmissions is application specific.
     *
     * @return Number of times a ClockSync request is supposed to be sent
     */
    uint8_t get_resync_nb_trans(void);

    /** Get the time period of a re-sync request
     *
     * The number is set by 'DeviceAppTimePeriodicityReq' command (in our code
     * CLK_SYNC_APP_TIME_PERIODICITY).
     *
     * Application is responsible to invoke 'request_clock_sync()' API to generate
     * a request periodically based upon the period provided here.
     *
     * @return Time period after which ClockSync request is supposed to be sent
     */
    uint8_t get_sync_req_periodicity(void);

private:
    uint16_t prepare_clock_sync_request(bool ans_required, uint16_t start_idx);

private:
    uint8_t _inbound_buf[MAX_CLK_SYNC_CMD_INBOUND_BUFFER_SIZE];
    uint8_t _outbound_buf[MAX_CLK_SYNC_CMD_OUTBOUND_BUFFER_SIZE];
    uint8_t _clock_sync_token_req;
    uint8_t _forced_nb_trans;
    uint8_t _app_time_periodicty;
    clk_sync_response_t _resp;
};

#endif /* MBED_OS_FEATURES_LORAWAN_PLUGINS_CLOCKSYNCCONTROLPACKAGE_H_ */
/** @}*/
