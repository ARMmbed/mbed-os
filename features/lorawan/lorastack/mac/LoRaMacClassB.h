#ifndef MBED_LORAWAN_LORAMACCLASSB_H_
#define MBED_LORAWAN_LORAMACCLASSB_H_
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

#include "system/lorawan_data_structures.h"
#include "lorastack/phy/LoRaPHY.h"
#include "lorastack/mac/LoRaMacCrypto.h"
#include "platform/ScopedLock.h"
#if MBED_CONF_RTOS_PRESENT
#include "rtos/Mutex.h"
#endif

class LoRaMacClassB {
protected:

    // LoRaWANClassBInterface provides the public interface
    friend class LoRaMacClassBInterface;

    /**
     * @brief Construct a new LoRaMacClass B object
     *
     */
    LoRaMacClassB();

    /**
     * @brief Destroy the LoRaMacClass B object
     *
     */
    ~LoRaMacClassB() {};

    /**
     * Initialize Class B system
     *
     * @param [in] lora_time
     * @param [in] lora_phy
     * @param [in] lora_crypto
     * @param [in] params
     * @param [in] open_rx_window   LoRaMac open rx window callback
     * @param [in] close_rx_window  LoRaMac close rx window callback
     * @return LORAWAN_STATUS_OK or a negative error return code
     */
    lorawan_status_t initialize(LoRaWANTimeHandler *lora_time,
                                LoRaPHY *lora_phy,
                                LoRaMacCrypto *lora_crypto,
                                loramac_protocol_params *params,
                                mbed::Callback<bool(rx_config_params_t *)> open_rx_window,
                                mbed::Callback<void(rx_slot_t)> close_rx_window);

    /**
     * @brief Enable beacon acquisition and tracking
     *
     * @param beacon_event_cb  stack beacon event callback
     * @return lorawan_status_t LORAWAN_STATUS_OK or a negative error return code
     */
    lorawan_status_t enable_beacon_acquisition(mbed::Callback<void(loramac_beacon_status_t,
                                                                   const loramac_beacon_t *)> beacon_event_cb);

    /**
     * @brief Enable class B. To be called after successful beacon acquisition to enable B ping slots
     *
     * @return lorawan_status_t LORAWAN_STATUS_OK or a negative error return code
     */
    lorawan_status_t enable(void);

    /**
     * @brief Disable class B. Disables beacon and ping slots
     *
     * @return lorawan_status_t
     */
    lorawan_status_t disable(void);

    /**
     * @brief Temporarily pause class B rx slots during class A uplink & rx windows
     *
     */
    void pause(void);

    /**
     * @brief Resumes class b rx slots after pause. To be called after class A receive windows
     *
     */
    void resume(void);

    /**
     * @brief Handle radio receive timeout
     *
     * @param rx_slot  The rx slot type
     */
    void handle_rx_timeout(rx_slot_t rx_slot);

    /**
     * @brief Handle radio receive
     *
     * @param rx_slot
     * @param data
     * @param size
     * @param rx_timestamp
     */
    void handle_rx(rx_slot_t rx_slot, const uint8_t *const data, uint16_t size, uint32_t rx_timestamp);

    /**
     * @brief Get the last rx beacon object
     *
     * @param beacon
     * @return lorawan_status_t
     */
    lorawan_status_t get_last_rx_beacon(loramac_beacon_t &beacon);

    /**
     * @brief Add ping slot multicast address
     *
     * @param address  Multicast address
     * @return Add result
     */
    bool add_multicast_address(uint32_t address);

    /**
     * @brief  Remove ping slot multicast address
     *
     * @param [in] address  Multicast address
     */
    void remove_multicast_address(uint32_t address);

    /**
     * @brief Return gps time
     */
    inline lorawan_gps_time_t get_gps_time(void)
    {
        return _lora_time->get_gps_time();
    }

    /**
     * @brief Get beacon rx is enabled status
     *
     * @return true if beacons rx slot is enabled
     */
    inline bool is_operational(void)
    {
        return _opstatus.beacon_on;
    }

public:
    /**
     * These locks trample through to the upper layers and make
     * the stack thread safe.
     */
#if MBED_CONF_RTOS_PRESENT
    void lock(void)
    {
        _mutex.lock();
    }
    void unlock(void)
    {
        _mutex.unlock();
    }
#else
    void lock(void) { }
    void unlock(void) { }
#endif

private:

    /**
     * @brief Process frame received in beacon window
     * @param [in] data
     * @param [in] size  data size in bytes
     * @param [in] rx_timestamp  radio receive time
     */
    void handle_beacon_rx(const uint8_t *const data, uint16_t size, uint32_t rx_timestamp);

    /**
     * @brief Process a beacon frame and perform integrity checks
     * @param [in] frame
     * @param [in] size
     */
    lorawan_time_t process_beacon_frame(const uint8_t *const frame, uint16_t size);

    /**
     * @brief Handle beacon window timeout
     */
    void handle_beacon_rx_timeout(void);

    /**
     * @brief Set the beacon rx config
     *
     * @param beacon_time
     * @param rx_config
     * @return False if beacon PHY configuration could not be computed
     */
    bool set_beacon_rx_config(uint32_t beacon_time, rx_config_params_t &rx_config);

    /**
     * @brief Schedules next beacon window opening
     */
    bool schedule_beacon_window(void);

    /**
     * @brief Open beacon window
     */
    void open_beacon_window(void);

    /**
     * @brief Schedule next ping slot window
     */
    void schedule_ping_slot(void);

    /**
     * @brief Resets window expansion parameters to defaults
     */
    void reset_window_expansion(void);

    /**
     * @brief Expands beacon window size
     */
    void expand_window(void);

    /**
     * @brief Beacon acquisition timeout callback
     *
     */
    void beacon_acquisition_timeout(void);

    /**
     * @brief Computes beacon window ping slot configuration and
     *        schedules the next ping rx window
     * param [in] beacon_time
     */
    void start_ping_slots(uint32_t beacon_time);

    /**
     * @brief Open ping slot window
     */
    void open_ping_slot(void);

    /**
     * @brief Computes beacon window ping slot offset
     *
     * param [in] beacon_time
     * param [in] address device address
     * param [in] ping_period  ping slot period expressed in number of ping slots
     * param [out] ping_offset the offset for the beacon window
     */
    bool compute_ping_offset(uint32_t beacon_time, uint32_t address,
                             uint16_t ping_period, uint16_t &ping_offset);

    /**
     * @brief Computes next ping slot for given address
     *
     * param [in] beacon_time  beacon time
     * param [in] current_time  current device time
     * param [in] ping_period Period of receiver wakeup expressed in number of slots
     * param [in] ping_nb Number of ping slots per beacon period
     * param [in] address  unicast or multicast device address
     * param [in] ping_slot_offset  Randomized offset computed at each beacon period start
     * param [out] next_slot_nb   slot number to open
     * param [out] rx_config ping slot rx configuration
     * return GPS time for the ping slot, 0 if no ping slots remain for the current beacon period
     */
    lorawan_gps_time_t compute_ping_slot(uint32_t beacon_time, lorawan_gps_time_t current_time,
                                         uint16_t ping_period, uint8_t ping_nb,
                                         uint32_t address, uint16_t ping_slot_offset,
                                         uint16_t &next_slot_nb, rx_config_params_t &rx_config);

    /**
     * @brief  send beacon miss event to the stack
     *
     */
    void send_beacon_miss_indication(void);

    /**
     * @brief get protocol parameters pointer
     */
    inline loramac_protocol_params *protocol_params(void)
    {
        MBED_ASSERT(_protocol_params);
        return _protocol_params;
    }

    /**
     * @brief  RX Window Expansion parameters
     *
     */
    struct loramac_window_expansion_t {
        int16_t movement; // window movement in ms
        uint16_t timeout; // window timeout in symbols
    };

    /**
     * @brief Beacon state variables
     *
     */
    typedef struct loramac_beacon_context_s {
        static const uint8_t BEACON_FRAME_COMMON_SIZE = 15;

        uint8_t frame_size(void)
        {
            return rfu1_size + rfu2_size + BEACON_FRAME_COMMON_SIZE;
        }

        lorawan_time_t beacon_time; // beacon period time
        rx_config_params_t rx_config; // beacon receiver configuration
        uint8_t rfu1_size; // beacon frame rfu1 size
        uint8_t rfu2_size; // beacon frame rfu2 size
        loramac_beacon_t received_beacon; // last received beacon
        loramac_window_expansion_t expansion; // beacon window expansion
        uint32_t time_on_air; // beacon frame time on air
    } loramac_beacon_context_t;

    /**
     * @brief Ping state variables
     *
     */
    typedef struct loramac_ping_slot_context_s {
        const static int8_t UNICAST_PING_SLOT_IDX = 0; // Invalid slot index value

        loramac_window_expansion_t expansion; // ping slot expansion
        struct {
            uint32_t address; // device address
            uint16_t offset; // ping slot offset
            uint16_t slot_nb; // next ping slot number
            rx_config_params_t rx_config; // receiver configuration
        } slot[1 + MBED_CONF_LORA_CLASS_B_MULTICAST_ADDRESS_MAX_COUNT];

        int8_t slot_idx; // holds next ping_slot idx
    } loramac_ping_slot_context_t;

    /**
     * Class B Operational Status
     */
    typedef struct {
        uint32_t initialized  : 1; // class B initialized
        uint32_t beacon_on    : 1; // beacon windows enabled
        uint32_t beacon_found : 1; // beacon found
        uint32_t ping_on      : 1; // ping slots are enabled
        uint32_t paused       : 1; // class B enabled, all rx slots paused
        uint32_t beacon_rx    : 1; // beacon slot is open
        uint32_t ping_rx      : 1; // ping slot is open
    } class_b_operstatus_bits_t;

    class_b_operstatus_bits_t _opstatus; // operational status mask
    loramac_beacon_context_t _beacon; // beacon state
    loramac_ping_slot_context_t _ping; // ping slot state

    timer_event_t _beacon_timer;     // beacon window timer
    timer_event_t _beacon_acq_timer; // beacon acquisition timeout
    timer_event_t _ping_slot_timer;  // ping window timer

    LoRaPHY *_lora_phy; // LoRa PHY
    LoRaWANTimeHandler *_lora_time;  // time support
    LoRaMacCrypto *_lora_crypto; // Crypto library
    loramac_protocol_params *_protocol_params; // loramac parammeters

    mbed::Callback<bool(rx_config_params_t *)> _open_rx_window; // loramac open rx window  function
    mbed::Callback<void(rx_slot_t)> _close_rx_window; // loramac close rx window function
    mbed::Callback<void(loramac_beacon_status_t, const loramac_beacon_t *)> _beacon_event_cb; // beacon event callback

#if MBED_CONF_RTOS_PRESENT
    rtos::Mutex _mutex;
#endif
    typedef mbed::ScopedLock<LoRaMacClassB> Lock;

};
#endif // #ifndef MBED_LORAWAN_LORAMACCLASSB_H_