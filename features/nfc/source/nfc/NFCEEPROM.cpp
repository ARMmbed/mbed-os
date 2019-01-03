/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "NFCEEPROM.h"
#include "ndef/ndef.h"

using namespace mbed;
using namespace mbed::nfc;

NFCEEPROM::NFCEEPROM(NFCEEPROMDriver *driver, events::EventQueue *queue, const Span<uint8_t> &ndef_buffer) : NFCTarget(ndef_buffer),
    _delegate(NULL), _driver(driver), _event_queue(queue), _initialized(false), _current_op(nfc_eeprom_idle), _ndef_buffer_read_sz(0), _eeprom_address(0), _operation_result(NFC_ERR_UNKNOWN)
{
    _driver->set_delegate(this);
    _driver->set_event_queue(queue);
}

nfc_err_t NFCEEPROM::initialize()
{
    MBED_ASSERT(_initialized == false); // Initialize should only be called once

    // Initialize driver
    _driver->reset();
    _initialized = true;
    return NFC_OK;
}

void NFCEEPROM::set_delegate(NFCEEPROM::Delegate *delegate)
{
    _delegate = delegate;
}

void NFCEEPROM::write_ndef_message()
{
    MBED_ASSERT(_initialized == true);
    if (_current_op != nfc_eeprom_idle) {
        if (_delegate != NULL) {
            _delegate->on_ndef_message_written(NFC_ERR_BUSY);
        }
        return;
    }

    // First update NDEF message if required
    ndef_msg_encode(ndef_message());

    _current_op = nfc_eeprom_write_start_session;

    // Retrieve reader
    ac_buffer_dup(&_ndef_buffer_reader, ac_buffer_builder_buffer(ndef_msg_buffer_builder(ndef_message())));

    // Check that NDEF message is not too big
    if (ac_buffer_reader_readable(&_ndef_buffer_reader) > _driver->read_max_size()) {
        handle_error(NFC_ERR_BUFFER_TOO_SMALL);
        return;
    }

    // Reset EEPROM address
    _eeprom_address = 0;
    // Go through the steps!
    _driver->start_session();

    // 1 - Start session
    // 2 - Write bytes (can be repeated)
    // 3 - Set NDEF message size
    // 4 - End session
}

void NFCEEPROM::read_ndef_message()
{
    MBED_ASSERT(_initialized == true);
    if (_current_op != nfc_eeprom_idle) {
        if (_delegate != NULL) {
            _delegate->on_ndef_message_written(NFC_ERR_BUSY);
        }
        return;
    }
    _current_op = nfc_eeprom_read_start_session;

    // Reset EEPROM address
    _eeprom_address = 0;

    // Go through the steps!
    _driver->start_session();

    // 1 - Start session
    // 2 - Get NDEF message size
    // 3 - Read bytes (can be repeated)
    // 4 - End session
}

void NFCEEPROM::erase_ndef_message()
{
    // We don't want to take any risks, so erase the whole address space
    // And set the message size to 0

    MBED_ASSERT(_initialized == true);
    if (_current_op != nfc_eeprom_idle) {
        if (_delegate != NULL) {
            _delegate->on_ndef_message_erased(NFC_ERR_BUSY);
        }
        return;
    }
    _current_op = nfc_eeprom_erase_start_session;

    // Reset EEPROM address
    _eeprom_address = 0;

    // Go through the steps!
    _driver->start_session();

    // 1 - Start session
    // 2 - Set addressable size to the max
    // 3 - Erase bytes (can be repeated)
    // 4 - Set addressable size to 0
    // 5 - End session
}

void NFCEEPROM::on_session_started(bool success)
{
    switch (_current_op) {
        case nfc_eeprom_write_start_session:
            if (!success) {
                handle_error(NFC_ERR_CONTROLLER); // An EEPROM is not really a controller but close enough
                return;
            }
            _current_op = nfc_eeprom_write_write_size;
            _driver->write_size(ac_buffer_reader_readable(&_ndef_buffer_reader));
            break;

        case nfc_eeprom_read_start_session:
            if (!success) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }
            _current_op = nfc_eeprom_read_read_size;
            _driver->read_size();
            break;

        case nfc_eeprom_erase_start_session:
            if (!success) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }

            _current_op = nfc_eeprom_erase_write_max_size;
            _driver->write_size(_driver->read_max_size());
            break;

        default:
            // Should not happen, state machine is broken or driver is doing something wrong
            handle_error(NFC_ERR_UNKNOWN);
            return;
    }
}

void NFCEEPROM::on_session_ended(bool success)
{
    switch (_current_op) {
        case nfc_eeprom_write_end_session:
            if (!success) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }
            _current_op = nfc_eeprom_idle;
            if (_delegate != NULL) {
                _delegate->on_ndef_message_written(_operation_result);
            }
            break;

        case nfc_eeprom_read_end_session:
            if (!success) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }
            _current_op = nfc_eeprom_idle;

            // Try to parse the NDEF message
            ndef_msg_decode(ndef_message());

            if (_delegate != NULL) {
                _delegate->on_ndef_message_read(_operation_result);
            }
            break;

        case nfc_eeprom_erase_end_session:
            if (!success) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }
            _current_op = nfc_eeprom_idle;
            if (_delegate != NULL) {
                _delegate->on_ndef_message_erased(_operation_result);
            }
            break;

        default:
            // Should not happen, state machine is broken or driver is doing something wrong
            handle_error(NFC_ERR_UNKNOWN);
            return;
    }
}

void NFCEEPROM::on_bytes_read(size_t count)
{
    switch (_current_op) {
        case nfc_eeprom_read_read_bytes: {
            if (count == 0) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }

            // Discard bytes that were actually read and update address
            _eeprom_address += count;
            ac_buffer_builder_t *buffer_builder = ndef_msg_buffer_builder(ndef_message());
            ac_buffer_builder_write_n_skip(buffer_builder, count);

            // Continue reading
            _event_queue->call(this, &NFCEEPROM::continue_read);
            break;
        }
        default:
            // Should not happen, state machine is broken or driver is doing something wrong
            handle_error(NFC_ERR_UNKNOWN);
            return;
    }
}

void NFCEEPROM::on_bytes_written(size_t count)
{
    switch (_current_op) {
        case nfc_eeprom_write_write_bytes:
            if (count == 0) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }

            // Skip bytes that were actually written and update address
            _eeprom_address += count;
            ac_buffer_read_n_skip(&_ndef_buffer_reader, count);

            // Continue writing
            _event_queue->call(this, &NFCEEPROM::continue_write);
            break;
        default:
            // Should not happen, state machine is broken or driver is doing something wrong
            handle_error(NFC_ERR_UNKNOWN);
            return;
    }
}

void NFCEEPROM::on_size_written(bool success)
{
    switch (_current_op) {
        case nfc_eeprom_write_write_size:
            if (!success) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }

            _current_op = nfc_eeprom_write_write_bytes;
            continue_write();
            break;
        case nfc_eeprom_erase_write_max_size:
            if (!success) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }

            // Start erasing bytes
            _current_op = nfc_eeprom_erase_erase_bytes;
            continue_erase();
            break;
        case nfc_eeprom_erase_write_0_size:
            if (!success) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }

            // End session
            _current_op = nfc_eeprom_erase_end_session;
            _operation_result = NFC_OK;
            _driver->end_session();
            break;
        default:
            // Should not happen, state machine is broken or driver is doing something wrong
            handle_error(NFC_ERR_UNKNOWN);
            return;
    }
}

void NFCEEPROM::on_size_read(bool success, size_t size)
{
    switch (_current_op) {
        case nfc_eeprom_read_read_size: {
            if (!success) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }

            // Reset NDEF message buffer builder
            ac_buffer_builder_t *buffer_builder = ndef_msg_buffer_builder(ndef_message());
            ac_buffer_builder_reset(buffer_builder);

            // Check that we have a big enough buffer to read the message
            if (size > ac_buffer_builder_writable(buffer_builder)) {
                // Not enough space, close session
                _current_op = nfc_eeprom_read_end_session;
                _operation_result = NFC_ERR_BUFFER_TOO_SMALL;
                _driver->end_session();
                return;
            }

            // Save size and reset address
            _eeprom_address = 0;
            _ndef_buffer_read_sz = size;

            // Start reading bytes
            _current_op = nfc_eeprom_read_read_bytes;
            _event_queue->call(this, &NFCEEPROM::continue_read);
            break;
        }
        default:
            // Should not happen, state machine is broken or driver is doing something wrong
            handle_error(NFC_ERR_UNKNOWN);
            return;
    }
}

void NFCEEPROM::on_bytes_erased(size_t count)
{

    switch (_current_op) {
        case nfc_eeprom_erase_erase_bytes:
            if (count == 0) {
                handle_error(NFC_ERR_CONTROLLER);
                return;
            }

            // Update address
            _eeprom_address += count;

            // Continue erasing
            _event_queue->call(this, &NFCEEPROM::continue_erase);
            break;
        default:
            // Should not happen, state machine is broken or driver is doing something wrong
            handle_error(NFC_ERR_UNKNOWN);
            return;
    }
}

void NFCEEPROM::continue_write()
{
    if (ac_buffer_reader_readable(&_ndef_buffer_reader) > 0) {
        // Continue writing
        _driver->write_bytes(_eeprom_address, ac_buffer_reader_current_buffer_pointer(&_ndef_buffer_reader), ac_buffer_reader_current_buffer_length(&_ndef_buffer_reader));
    } else {
        // we are done
        _current_op = nfc_eeprom_write_end_session;
        _operation_result = NFC_OK;
        _driver->end_session();
    }
}

void NFCEEPROM::continue_erase()
{
    if (_eeprom_address < _driver->read_max_size()) {
        // Continue erasing
        _driver->erase_bytes(_eeprom_address, _driver->read_max_size() - _eeprom_address);
    } else {
        // Now update size
        _current_op = nfc_eeprom_erase_write_0_size;
        _driver->write_size(0);
    }
}

void NFCEEPROM::continue_read()
{
    if (_eeprom_address < _ndef_buffer_read_sz) {
        // Continue reading
        ac_buffer_builder_t *buffer_builder = ndef_msg_buffer_builder(ndef_message());
        _driver->read_bytes(_eeprom_address, ac_buffer_builder_write_position(buffer_builder), _ndef_buffer_read_sz - _eeprom_address);
    } else {
        // Done, close session
        _current_op = nfc_eeprom_read_end_session;
        _operation_result = NFC_OK;
        _driver->end_session();
    }
}

void NFCEEPROM::handle_error(nfc_err_t ret)
{
    // Save & reset current op
    nfc_eeprom_operation_t last_op = _current_op;
    _current_op = nfc_eeprom_idle;

    if (_delegate != NULL) {
        if (last_op <= nfc_eeprom_write_end_session) {
            _delegate->on_ndef_message_written(ret);
        } else if (last_op <= nfc_eeprom_read_end_session) {
            _delegate->on_ndef_message_read(ret);
        } else if (last_op <= nfc_eeprom_erase_end_session) {
            _delegate->on_ndef_message_erased(ret);
        }
    }
}

NFCNDEFCapable::Delegate *NFCEEPROM::ndef_capable_delegate()
{
    return _delegate;
}
