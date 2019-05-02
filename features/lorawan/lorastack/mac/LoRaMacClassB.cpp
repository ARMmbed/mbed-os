/**
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech
 ___ _____ _   ___ _  _____ ___  ___  ___ ___
/ __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
\__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
|___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
embedded.connectivity.solutions===============

Description: LoRaWAN stack layer that controls both MAC and PHY underneath

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )


Copyright (c) 2019, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/
#include "LoRaMacClassB.h"
#include "mbed_assert.h"
#include "stdio.h"
#include "LoRaMacCrypto.h"
#include  "mbed.h"
#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP "LMAC"

/*!
 * Limit ping slot mbed tracing to once per this period in millis
 */
#define CLASSB_PING_OPEN_DBG_TRACE_PERIOD    5000

/*!
 * Beacon reserved time in ms
 */
#define CLASSB_BEACON_RESERVED               2120

/*!
 * Ping slot length time in ms
 */
#define CLASSB_PING_SLOT_WINDOW              30

/*!
 * Invalid ping slot
 */
#define LORAMAC_INVALID_PING_SLOT -1

/*!
 * LoRaWAN Beacon Interval in milliseconds
 */
#define LORA_BEACON_INTERVAL_MILLIS          (LORA_BEACON_INTERVAL*1000UL)


LoRaMacClassB::LoRaMacClassB()
    : _lora_phy(NULL),
      _lora_time(NULL),
      _lora_crypto(NULL),
      _protocol_params(NULL),
      _open_rx_window(NULL),
      _close_rx_window(NULL),
      _beacon_event_cb(NULL)
{
    memset(&_beacon, 0, sizeof(_beacon));
    memset(&_ping, 0, sizeof(_ping));
    memset(&_opstatus, 0, sizeof(_opstatus));
}

lorawan_status_t LoRaMacClassB::initialize(LoRaWANTimeHandler *lora_time, LoRaPHY *lora_phy,
                                           LoRaMacCrypto *lora_crypto, loramac_protocol_params *params,
                                           mbed::Callback<bool(rx_config_params_t *)> open_window,
                                           mbed::Callback<void(rx_slot_t)> close_window)
{
    // Initialization should not be called more than once
    MBED_ASSERT(!_opstatus.initialized);

    if (!_opstatus.initialized) {
        MBED_ASSERT(lora_time);
        MBED_ASSERT(lora_crypto);
        MBED_ASSERT(params);
        MBED_ASSERT(lora_phy);
        MBED_ASSERT(open_window);
        MBED_ASSERT(close_window);

        _lora_time = lora_time;
        _lora_phy  = lora_phy;
        _lora_crypto = lora_crypto;
        _protocol_params = params;

        _lora_time->init(_beacon_timer,
                         mbed::callback(this, &LoRaMacClassB::open_beacon_window));

        _lora_time->init(_ping_slot_timer,
                         mbed::callback(this, &LoRaMacClassB::open_ping_slot));

        _lora_time->init(_beacon_acq_timer,
                         mbed::callback(this, &LoRaMacClassB::beacon_acquisition_timeout));

        _lora_phy->get_beacon_rfu_size(_beacon.rfu1_size, _beacon.rfu2_size);

        _open_rx_window = open_window;
        _close_rx_window = close_window;

        _beacon.time_on_air = _lora_phy->compute_beacon_time_on_air();

        _ping.slot[0].rx_config.rx_slot =  RX_SLOT_WIN_UNICAST_PING_SLOT;
        for (uint8_t i = 1; i <= MBED_CONF_LORA_CLASS_B_MULTICAST_ADDRESS_MAX_COUNT; i++) {
            _ping.slot[i].rx_config.rx_slot =  RX_SLOT_WIN_MULTICAST_PING_SLOT;
        }

        _opstatus.initialized = 1;
    }

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacClassB::enable(void)
{
    if (!_opstatus.initialized) {
        return LORAWAN_STATUS_NOT_INITIALIZED;
    } else if (!_opstatus.beacon_found) {
        return LORAWAN_STATUS_NO_BEACON_FOUND;
    } else if (!_opstatus.ping_on) {
        // Initialize unicast ping slot address
        _ping.slot[0].address = protocol_params()->dev_addr;
        _opstatus.ping_on = 1;
        start_ping_slots(_beacon.beacon_time);
    }
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacClassB::disable(void)
{
    _opstatus.beacon_on = 0;
    _opstatus.ping_on = 0;
    _lora_time->stop(_beacon_timer);
    _lora_time->stop(_ping_slot_timer);
    _lora_time->stop(_beacon_acq_timer);

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacClassB::enable_beacon_acquisition(mbed::Callback<void(loramac_beacon_status_t,
                                                                              const loramac_beacon_t *)> beacon_event_cb)
{
    lorawan_status_t status = LORAWAN_STATUS_OK;

    if (!_opstatus.initialized) {
        status = LORAWAN_STATUS_NOT_INITIALIZED;
    } else if (!_opstatus.beacon_on) {
        _beacon_event_cb = beacon_event_cb;
        _beacon.beacon_time = 0;
        _opstatus.beacon_found = 0;
        _opstatus.beacon_on = 1;

        reset_window_expansion();

        if (schedule_beacon_window()) {

            uint32_t acquisition_timeout = MBED_CONF_LORA_BEACON_ACQUISITION_NB_TRIALS *
                                           LORA_BEACON_INTERVAL_MILLIS;
            _lora_time->start(_beacon_acq_timer, acquisition_timeout);

            tr_debug("Enabled Beacon Acquisition");
        } else {
            _opstatus.beacon_on = 0;
            status = LORAWAN_STATUS_NO_OP; // Cannot perform request operation
        }
    }
    return status;
}

void LoRaMacClassB::beacon_acquisition_timeout(void)
{
    // Disable beacon acquisition
    _opstatus.beacon_on = 0;
    _lora_time->stop(_beacon_timer);
    _close_rx_window(RX_SLOT_WIN_BEACON);
    send_beacon_miss_indication();
}

bool LoRaMacClassB::set_beacon_rx_config(uint32_t beacon_time, rx_config_params_t &rx_config)
{
    bool status;

    rx_config.rx_slot = RX_SLOT_WIN_BEACON;
    rx_config.is_rx_continuous =  beacon_time == 0;

    status = _lora_phy->compute_beacon_win_params(beacon_time,
                                                  MBED_CONF_LORA_BEACON_PREAMBLE_LENGTH,
                                                  MBED_CONF_LORA_MAX_SYS_RX_ERROR, &rx_config);

    if (status) {
        rx_config.window_timeout = MAX(_beacon.expansion.timeout, rx_config.window_timeout);
    } else {
        tr_error("Compute Beacon PHY parameters failed");
    }

    return status;
}

bool LoRaMacClassB::schedule_beacon_window(void)
{
    bool status = false;

    // Abort if beacon slot is disabled
    if (!_opstatus.beacon_on) {
        return false;
    }

    lorawan_gps_time_t current_time = get_gps_time();

    // Receiver put in continuous mode when device time is not set
    if (current_time == 0) {
        status = set_beacon_rx_config(0, _beacon.rx_config);
        if (status) {
            open_beacon_window();
        }
    } else {
        lorawan_gps_time_t next_beacon_time = ((current_time / LORA_BEACON_INTERVAL_MILLIS) + 1)
                                              * LORA_BEACON_INTERVAL_MILLIS;

        // Delay until next beacon
        lorawan_gps_time_t beacon_delay = next_beacon_time - current_time;

        // Compute beacon window receiver configuration
        status = set_beacon_rx_config(next_beacon_time / 1000, _beacon.rx_config);
        if (status) {
            // PHY layer computes window offset adjusting for minimum preamble,
            // timing errors, receiver wakeup time
            int32_t window_offset = _beacon.rx_config.window_offset;
            // Expand PHY offset by class b window expansion
            window_offset -= _beacon.expansion.movement;
            int32_t delay =  beacon_delay + window_offset;

            // Open window now if delay elapsed
            if (delay < 0) {
                open_beacon_window();
            } else {
                _lora_time->start(_beacon_timer, delay);
            }

            tr_debug("Next beacon time = %llu in %ld ms", next_beacon_time / 1000, delay);
        }
    }

    return status;
}

void LoRaMacClassB::open_beacon_window(void)
{
    Lock(*this);

    if (!_opstatus.beacon_on) {
        return;
    } else if (_opstatus.paused) {
        if (_opstatus.beacon_found) {
            _beacon.beacon_time += LORA_BEACON_INTERVAL;
            send_beacon_miss_indication();
        }
        // Naively schedule next beacon window. Accurate scheduling will occur
        // on resume
        _lora_time->start(_beacon_timer, LORA_BEACON_INTERVAL_MILLIS);
    } else {
        // Set beacon rx regardless of open window status. Only expected reason for failure is
        // conflict with class A reception slots. In this case, handle rx(timeout) callback checks for failure
        _opstatus.beacon_rx = 1;
        if (_open_rx_window(&_beacon.rx_config)) {
            tr_debug("Beacon slot open, Freq = %lu", _beacon.rx_config.frequency);
        }
    }
}

void LoRaMacClassB::handle_rx(rx_slot_t rx_slot, const uint8_t *const payload, uint16_t size, uint32_t rx_timestamp)
{
    switch (rx_slot) {
        case RX_SLOT_WIN_BEACON:
            handle_beacon_rx(payload, size, rx_timestamp);
            break;
        case RX_SLOT_WIN_UNICAST_PING_SLOT:
        case RX_SLOT_WIN_MULTICAST_PING_SLOT:
            _opstatus.ping_rx = 0;
            schedule_ping_slot();
            break;
        default:
            // beacon rx state is set if beacon slot was not opened or aborted by class A operation
            if (_opstatus.beacon_rx) {
                _opstatus.beacon_rx = 0;
                handle_beacon_rx_timeout();
            }
            break;
    }
}

void LoRaMacClassB::handle_rx_timeout(rx_slot_t rx_slot)
{
    switch (rx_slot) {
        case RX_SLOT_WIN_BEACON:
            expand_window();
            handle_beacon_rx_timeout();
            break;
        case RX_SLOT_WIN_UNICAST_PING_SLOT:
        case RX_SLOT_WIN_MULTICAST_PING_SLOT:
            _opstatus.ping_rx = 0;
            schedule_ping_slot();
            break;
        default:
            // beacon rx state is set if beacon slot was not opened or aborted by class A operation
            if (_opstatus.beacon_rx) {
                _opstatus.beacon_rx = 0;
                handle_beacon_rx_timeout();
            }
            break;
    }
}

void LoRaMacClassB::handle_beacon_rx(const uint8_t *const frame, uint16_t size, uint32_t rx_timestamp)
{
    lorawan_time_t beacon_time;
    loramac_beacon_status_t beacon_status;
    lorawan_gps_time_t gps_time;

    _opstatus.beacon_rx = 0;

    beacon_time = process_beacon_frame(frame, size);

    // Beacon is ok if beacon time read
    if (beacon_time) {
        _beacon.beacon_time = beacon_time;

        // Update gps time
        gps_time = (lorawan_gps_time_t)beacon_time * 1000 + _beacon.time_on_air +
                   (_lora_time->get_current_time() - rx_timestamp);
        _lora_time->set_gps_time(gps_time);
        tr_debug("Synchronized GPS Time = %llu from Received Beacon", gps_time);

        // Reset window expansion to defaults
        reset_window_expansion();

        // Stop acquistion timeout if this the first found beacon
        if (!_opstatus.beacon_found) {
            _opstatus.beacon_found = 1;
            _lora_time->stop(_beacon_acq_timer);
            beacon_status = BEACON_STATUS_ACQUISITION_SUCCESS;
        } else {
            beacon_status = BEACON_STATUS_LOCK;
        }

        // Send event to the stack
        _beacon_event_cb(beacon_status, &_beacon.received_beacon);
        // Schedule next beacon window reception
        schedule_beacon_window();
        // Schedule beacon window ping slots
        start_ping_slots(beacon_time);
    } else {
        handle_beacon_rx_timeout();
    }
}

void LoRaMacClassB::send_beacon_miss_indication(void)
{
    loramac_beacon_status_t status = _opstatus.beacon_found ? BEACON_STATUS_MISS : BEACON_STATUS_ACQUISITION_FAILED;
    _beacon_event_cb(status, NULL);
}

void LoRaMacClassB::handle_beacon_rx_timeout(void)
{
    _opstatus.beacon_rx = 0;

    // Missed beacon, adjust time if beacon previously found
    if (_opstatus.beacon_found) {
        _beacon.beacon_time += LORA_BEACON_INTERVAL;
        send_beacon_miss_indication();
    }
    schedule_beacon_window();
    start_ping_slots(_beacon.beacon_time);
}

uint32_t LoRaMacClassB::process_beacon_frame(const uint8_t *const frame, uint16_t size)
{
    MbedCRC<POLY_16BIT_CCITT, 16> ct(0, 0, false, false);
    uint16_t beacon_crc1;
    uint16_t beacon_crc2;
    uint32_t crc1;
    uint32_t crc2;
    uint32_t beacon_time = 0;

    // A beacon frame is defined as:
    // Bytes: |  x   |  4   |  2   |     7      |  y   |  2   |
    //        |------|------|------|------------|------|------|
    // Field: | RFU1 | Time | CRC1 | GwSpecific | RFU2 | CRC2 |
    //
    // Field RFU1 and RFU2 have variable sizes. It depends on the region specific implementation
    if (size == _beacon.frame_size()) {
        beacon_crc1  = frame[_beacon.rfu1_size + 4] & 0xFF;
        beacon_crc1 |= ((uint16_t)frame[_beacon.rfu1_size + 5] << 8) & 0xFF00;

        // Validate the first crc of the beacon frame
        ct.compute((void *)frame, _beacon.rfu1_size + 4, &crc1);
        if (crc1 == beacon_crc1) {
            // Read Time from the frame
            uint8_t pos = _beacon.rfu1_size;
            beacon_time  = ((uint32_t)frame[pos]) & 0x000000FF;
            beacon_time |= ((uint32_t)(frame[pos + 1] << 8))  & 0x0000FF00;
            beacon_time |= ((uint32_t)(frame[pos + 2] << 16)) & 0x00FF0000;
            beacon_time |= ((uint32_t)(frame[pos + 3] << 24)) & 0xFF000000;
            _beacon.received_beacon.time = beacon_time;

            // Validate the second crc of the beacon frame
            beacon_crc2  = frame[_beacon.rfu1_size + 4 + 2 + 7 + _beacon.rfu2_size] & 0xFF;
            beacon_crc2 |= ((uint16_t)frame[_beacon.rfu1_size + 4 + 2 + 7 + _beacon.rfu2_size + 1] << 8) & 0xFF00;
            ct.compute((void *)(frame + _beacon.rfu1_size + 4 + 2), 7 + _beacon.rfu2_size, &crc2);
            if (crc2 == beacon_crc2) {
                //  Read frame GwSpecific  field
                memcpy(_beacon.received_beacon.gw_specific, frame + _beacon.rfu1_size + 4 + 2, LORAMAC_BEACON_GW_SPECIFIC_LEN);
            } else {
                tr_debug("Beacon frame Gateway Specific part integrity check failed");
                memset(_beacon.received_beacon.gw_specific, 0, LORAMAC_BEACON_GW_SPECIFIC_LEN);
            }
        } else {
            tr_debug("Beacon frame Network Common part integrity check failed");
        }
    }

    return beacon_time;
}

void LoRaMacClassB::reset_window_expansion(void)
{
    // Beacon slot expansion defaults
    _beacon.expansion.timeout  = MBED_CONF_LORA_CLASS_B_EXPANSION_TIMEOUT_DEFAULT;
    _beacon.expansion.movement = MBED_CONF_LORA_CLASS_B_EXPANSION_OFFSET_DEFAULT;

    // Ping slot expansion defaults
    _ping.expansion.timeout  = MBED_CONF_LORA_CLASS_B_EXPANSION_TIMEOUT_DEFAULT;
    _ping.expansion.movement = MBED_CONF_LORA_CLASS_B_EXPANSION_OFFSET_DEFAULT;
}

void LoRaMacClassB::expand_window(void)
{
    // beacon symbol timeout
    _beacon.expansion.timeout *= MBED_CONF_LORA_CLASS_B_EXPANSION_TIMEOUT_FACTOR;
    if (_beacon.expansion.timeout > MBED_CONF_LORA_CLASS_B_EXPANSION_BEACON_TIMEOUT_MAX) {
        _beacon.expansion.timeout = MBED_CONF_LORA_CLASS_B_EXPANSION_BEACON_TIMEOUT_MAX;
    }

    // beacon slot movement
    _beacon.expansion.movement *= MBED_CONF_LORA_CLASS_B_EXPANSION_OFFSET_FACTOR;
    if (_beacon.expansion.movement > MBED_CONF_LORA_CLASS_B_EXPANSION_OFFSET_MAX) {
        _beacon.expansion.movement = MBED_CONF_LORA_CLASS_B_EXPANSION_OFFSET_MAX;
    }
    tr_debug("Expand beacon window timeout=%u, movement=%u",
             _beacon.expansion.timeout, _beacon.expansion.movement);

    // ping slot symbol timeout
    _ping.expansion.timeout *= MBED_CONF_LORA_CLASS_B_EXPANSION_TIMEOUT_FACTOR;
    if (_ping.expansion.timeout > MBED_CONF_LORA_CLASS_B_EXPANSION_PING_TIMEOUT_MAX) {
        _ping.expansion.timeout = MBED_CONF_LORA_CLASS_B_EXPANSION_PING_TIMEOUT_MAX;
    }

    // ping slot movement
    _ping.expansion.movement *= MBED_CONF_LORA_CLASS_B_EXPANSION_OFFSET_FACTOR;
    if (_ping.expansion.movement > MBED_CONF_LORA_CLASS_B_EXPANSION_OFFSET_MAX) {
        _ping.expansion.movement = MBED_CONF_LORA_CLASS_B_EXPANSION_OFFSET_MAX;
    }
    tr_debug("Expand ping slot timeout=%u, movement=%u",
             _ping.expansion.timeout, _ping.expansion.movement);
}

bool LoRaMacClassB::compute_ping_offset(uint32_t beacon_time, uint32_t address, uint16_t ping_period, uint16_t &ping_offset)
{
    int crypto_status;
    uint16_t rand_offset;

    crypto_status = _lora_crypto->compute_ping_slot_random_offset(beacon_time, address, &rand_offset);

    if (crypto_status == 0) {
        ping_offset = rand_offset % ping_period;
        return true;
    } else {
        tr_error("Compute Ping slot random offset crypto error (%d)", crypto_status);
    }

    return false;
}

lorawan_gps_time_t LoRaMacClassB::compute_ping_slot(uint32_t beacon_time, lorawan_gps_time_t current_time,
                                                    uint16_t ping_period, uint8_t  ping_nb, uint32_t address,
                                                    uint16_t ping_slot_offset, uint16_t &next_slot_nb,
                                                    rx_config_params_t &rx_config)
{
    uint64_t beacon_time_millis;
    uint8_t  slot = 0;
    uint16_t slot_nb = ping_slot_offset;
    lorawan_gps_time_t slot_time = 0;
    bool status;

    // Convert beacon time to millis
    beacon_time_millis = (uint64_t)beacon_time * 1000;

    // Compute the first ping slot = beacon reserved + (ping_offset * slot window length)
    slot_time = beacon_time_millis + CLASSB_BEACON_RESERVED + (slot_nb * CLASSB_PING_SLOT_WINDOW);

    // first slot is in the past, compute next slot time
    if (slot_time <= current_time) {
        // Get next slot number
        slot = ((current_time - slot_time) / (ping_period * CLASSB_PING_SLOT_WINDOW)) + 1;

        // Compute next slot time
        slot_nb = ping_slot_offset + (slot * ping_period);
        slot_time = (beacon_time_millis + CLASSB_BEACON_RESERVED) + (slot_nb * CLASSB_PING_SLOT_WINDOW);
    }

    // Check computed slot is less than configured number of slots per beacon period
    if (slot < ping_nb) {
        status = _lora_phy->compute_ping_win_params(beacon_time, address,
                                                    MBED_CONF_LORA_DOWNLINK_PREAMBLE_LENGTH,
                                                    MBED_CONF_LORA_MAX_SYS_RX_ERROR, &rx_config);

        if (status) {
            // For window timeout select the larger of the phy computed and class-b window expanded
            rx_config.window_timeout = MAX(_ping.expansion.timeout, rx_config.window_timeout);

            // PHY layer computes window offset adjusting for minimum preamble,
            // timing errors, receiver wakeup time
            int32_t window_offset = rx_config.window_offset - _ping.expansion.movement;

            slot_time += window_offset;
            next_slot_nb = slot_nb;
            return slot_time;
        } else {
            tr_error("Compute Ping slot PHY parameters failed");
        }
    }

    return 0;

}

void LoRaMacClassB::start_ping_slots(uint32_t beacon_time)
{
    // Do nothing if ping slots are not enabled
    if (!_opstatus.ping_on) {
        return;
    }

    loramac_protocol_params *params = protocol_params();

    // Compute unicast & multicast ping slot offsets
    for (uint8_t i = 0; i <= MBED_CONF_LORA_CLASS_B_MULTICAST_ADDRESS_MAX_COUNT; i++) {
        if (_ping.slot[i].address) {
            compute_ping_offset(beacon_time, _ping.slot[i].address,
                                params->sys_params.ping_slot.ping_period,
                                _ping.slot[i].offset);
        }
    }

    // Schedule next ping slot
    schedule_ping_slot();
}

void LoRaMacClassB::schedule_ping_slot(void)
{
    lorawan_gps_time_t current_time = get_gps_time();
    lorawan_gps_time_t best_slot_time = 0;
    lorawan_gps_time_t slot_time;
    int32_t delay;
    loramac_protocol_params *params = protocol_params();

    const uint16_t ping_period = params->sys_params.ping_slot.ping_period;
    const uint16_t ping_nb = params->sys_params.ping_slot.ping_nb;

    _lora_time->stop(_ping_slot_timer);
    _ping.slot_idx = LORAMAC_INVALID_PING_SLOT;

    // Compute next ping slot times
    for (uint8_t i = 0; i <= MBED_CONF_LORA_CLASS_B_MULTICAST_ADDRESS_MAX_COUNT; i++) {
        if (!_ping.slot[i].address) {
            continue;
        }

        slot_time = compute_ping_slot(_beacon.beacon_time, current_time, ping_period, ping_nb,
                                      _ping.slot[i].address, _ping.slot[i].offset, _ping.slot[i].slot_nb,
                                      _ping.slot[i].rx_config);

        if (slot_time) {
            if (!best_slot_time || (slot_time < best_slot_time)) {
                _ping.slot_idx = i;
                best_slot_time = slot_time;
            }
        }
    }

    // Valid slot idx if ping slot to open in the current beacon period
    if (_ping.slot_idx != LORAMAC_INVALID_PING_SLOT) {
        delay = best_slot_time - get_gps_time();
        if (delay > 0) {
            _lora_time->start(_ping_slot_timer, delay);
        } else {
            open_ping_slot();
        }
    }
}

void LoRaMacClassB::open_ping_slot(void)
{
    static lorawan_time_t last_ping_slot_time = 0;
    lorawan_time_t current_time;

    Lock(*this);

    // Check ping slot reception is enabled and receiver configuration is set
    if (!_opstatus.ping_on || _opstatus.paused || (_ping.slot_idx == LORAMAC_INVALID_PING_SLOT)) {
        return;
    }

    if (_open_rx_window(&_ping.slot[_ping.slot_idx].rx_config)) {
        _opstatus.ping_rx = 1;

        // Limiting ping slot trace period to prevent excessive debug output for small ping period
        current_time = _lora_time->get_current_time();
        if ((current_time - last_ping_slot_time) > CLASSB_PING_OPEN_DBG_TRACE_PERIOD) {
            tr_debug("Ping address = %lx slot = %u open, Freq = %lu",
                     _ping.slot[_ping.slot_idx].address,
                     _ping.slot[_ping.slot_idx].slot_nb,
                     _ping.slot[_ping.slot_idx].rx_config.frequency);

            last_ping_slot_time = current_time;
        }
    }
}

void LoRaMacClassB::pause(void)
{
    Lock(*this);

    if (_opstatus.paused) {
        return;
    }

    _opstatus.paused = 1;
    if (_opstatus.beacon_on) {
        tr_debug("Pause Class B");

        // Disable radio if beacon or ping slot is active
        if (_opstatus.beacon_rx) {
            _close_rx_window(RX_SLOT_WIN_BEACON);
        } else if (_opstatus.ping_rx) {
            if (_ping.slot_idx == 0) {
                _close_rx_window(RX_SLOT_WIN_UNICAST_PING_SLOT);
            } else {
                _close_rx_window(RX_SLOT_WIN_MULTICAST_PING_SLOT);
            }
        }
    }
}

void LoRaMacClassB::resume(void)
{
    if (!_opstatus.paused) {
        return;
    }
    _opstatus.paused = 0;
    if (!_opstatus.beacon_on) {
        return;
    }

    tr_debug("Resume Class B");

    // If pause occured during beacon slot in the beacon found state, send miss notification
    if (_opstatus.beacon_rx) {
        _opstatus.beacon_rx = 0;
        if (_opstatus.beacon_found) {
            send_beacon_miss_indication();
        }
    }

    // Schedule next beacon window
    _lora_time->stop(_beacon_timer);
    schedule_beacon_window();

    // Resume ping slots
    if (_opstatus.ping_on) {
        schedule_ping_slot();
    }
}

lorawan_status_t LoRaMacClassB::get_last_rx_beacon(loramac_beacon_t &beacon)
{
    if (_opstatus.beacon_found) {
        beacon = _beacon.received_beacon;
        return LORAWAN_STATUS_OK;
    }
    return LORAWAN_STATUS_NO_BEACON_FOUND;
}

bool LoRaMacClassB::add_multicast_address(uint32_t address)
{
    // Look for available slot
    for (uint8_t i = 1; i <= MBED_CONF_LORA_CLASS_B_MULTICAST_ADDRESS_MAX_COUNT; i++) {
        if (_ping.slot[i].address != 0) {
            continue;
        }

        _ping.slot[i].address = address;
        if (_opstatus.ping_on) {
            compute_ping_offset(_beacon.beacon_time, address,
                                protocol_params()->sys_params.ping_slot.ping_period,
                                _ping.slot[i].offset);
        }

        return true;
    }
    return false;
}

void LoRaMacClassB::remove_multicast_address(uint32_t address)
{
    for (uint8_t i = 1; i < MBED_CONF_LORA_CLASS_B_MULTICAST_ADDRESS_MAX_COUNT; i++) {
        if (_ping.slot[i].address == address) {
            _ping.slot[i].address = 0;
        }
    }
}