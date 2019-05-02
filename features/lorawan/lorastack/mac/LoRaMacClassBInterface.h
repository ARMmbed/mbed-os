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


#ifndef MBED_LORAWAN_LORAMACCLASSB_INTERFACE_H_
#define MBED_LORAWAN_LORAMACCLASSB_INTERFACE_H_

#include "LoRaMacClassB.h"

#if (MBED_CONF_LORA_VERSION >= 01) && (MBED_CONF_LORA_CLASS_B == true)
#define LORA_CLASS_B_ENABLED
#endif

#ifdef LORA_CLASS_B_ENABLED

/** LoRaWANClassBInterface provides the public interface to the
 *  LoRaMac Class B implementation
 */
class LoRaMacClassBInterface {

public:

    inline static lorawan_status_t initialize(LoRaWANTimeHandler *lora_time,
                                              LoRaPHY *lora_phy, LoRaMacCrypto *lora_crypto,
                                              loramac_protocol_params *params,
                                              mbed::Callback<bool(rx_config_params_t *)> open_rx_window,
                                              mbed::Callback<void(rx_slot_t)> close_rx_window)
    {
        return _class_b.initialize(lora_time, lora_phy, lora_crypto, params, open_rx_window, close_rx_window);
    }

    inline static lorawan_status_t enable_beacon_acquisition(mbed::Callback<void(loramac_beacon_status_t,
                                                                                 const loramac_beacon_t *)> beacon_event_cb)
    {
        return _class_b.enable_beacon_acquisition(beacon_event_cb);
    }

    inline static lorawan_status_t enable(void)
    {
        return _class_b.enable();
    }

    inline static lorawan_status_t disable(void)
    {
        return _class_b.disable();
    }

    inline static void pause(void)
    {
        _class_b.pause();
    }

    inline static void resume(void)
    {
        _class_b.resume();
    }

    inline static void handle_rx_timeout(rx_slot_t rx_slot)
    {
        // As an optimization, it is safe to skip calling the handler when beacon Rx is disabled
        if (_class_b.is_operational()) {
            _class_b.handle_rx_timeout(rx_slot);
        }
    }

    inline static void handle_rx(rx_slot_t rx_slot, const uint8_t *const data, uint16_t size, uint32_t rx_timestamp)
    {
        // As an optimization, it is safe to skip calling the handler when beacon Rx is disabled
        if (_class_b.is_operational()) {
            _class_b.handle_rx(rx_slot, data, size, rx_timestamp);
        }
    }

    inline static lorawan_status_t get_last_rx_beacon(loramac_beacon_t &beacon)
    {
        return _class_b.get_last_rx_beacon(beacon);
    }

    inline static bool add_multicast_address(uint32_t address)
    {
        return _class_b.add_multicast_address(address);
    }

    inline static void remove_multicast_address(uint32_t address)
    {
        _class_b.remove_multicast_address(address);
    }

    static LoRaMacClassB _class_b;
};

#else

class LoRaMacClassBInterface {

public:

    inline static lorawan_status_t initialize(LoRaWANTimeHandler *lora_time,
                                              LoRaPHY *lora_phy, LoRaMacCrypto *lora_crypto,
                                              loramac_protocol_params *params,
                                              mbed::Callback<bool(rx_config_params_t *)> open_rx_window,
                                              mbed::Callback<void(rx_slot_t)> close_rx_window)
    {
        return LORAWAN_STATUS_UNSUPPORTED;
    }

    inline static lorawan_status_t enable_beacon_acquisition(mbed::Callback<void(loramac_beacon_status_t,
                                                                                 const loramac_beacon_t *)> beacon_event_cb)
    {
        return LORAWAN_STATUS_UNSUPPORTED;
    }

    inline static lorawan_status_t enable(void)
    {
        return LORAWAN_STATUS_UNSUPPORTED;
    }

    inline static lorawan_status_t disable(void)
    {
        return LORAWAN_STATUS_UNSUPPORTED;
    }

    inline static void pause(void)
    {
    }

    inline static void resume(void)
    {
    }

    inline static void handle_rx_timeout(rx_slot_t rx_slot)
    {
    }

    inline static void handle_rx(rx_slot_t rx_slot, const uint8_t *const data, uint16_t size, uint32_t rx_timestamp)
    {
    }

    inline static lorawan_status_t get_last_rx_beacon(loramac_beacon_t &beacon)
    {
        return LORAWAN_STATUS_NO_BEACON_FOUND;
    }

    inline static bool add_multicast_address(uint32_t address)
    {
        return false;
    }

    inline static void remove_multicast_address(uint32_t address)
    {
    }
};

#endif // #if LORA_CLASS_B_ENABLED
#endif  // #ifndef MBED_LORAWAN_LORAMACCLASSB_INTERFACE_H_