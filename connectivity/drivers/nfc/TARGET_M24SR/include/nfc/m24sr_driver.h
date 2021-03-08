/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 * @file    m24sr_driver.h
 * @author  ST Central Labs
 * @brief   This file provides a set of functions to interface with the M24SR
 *          device.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/*
 Based on:         X-CUBE-MEMS1/trunk/Drivers/BSP/Components/m24sr/m24sr.h
 Revision:         M24SR Driver V1.0.0
 */

#ifndef M24SR_H
#define M24SR_H

#include <stdint.h>
#include <mbed.h>
#include "I2C.h"
#include "NFCEEPROMDriver.h"
#include "EventQueue.h"

#if defined TARGET_DISCO_L475VG_IOT01A

#define M24SR_I2C_SDA_PIN     PB_11
#define M24SR_I2C_SCL_PIN     PB_10
#define M24SR_GPO_PIN         PE_4
#define M24SR_RF_DISABLE_PIN  PE_2

#elif MBED_CONF_X_NUCLEO_NFC01A1

#define M24SR_I2C_SDA_PIN     D14
#define M24SR_I2C_SCL_PIN     D15
#define M24SR_GPO_PIN         D12
#define M24SR_RF_DISABLE_PIN  D11

#else

#define M24SR_I2C_SDA_PIN     NC
#define M24SR_I2C_SCL_PIN     NC
#define M24SR_GPO_PIN         NC
#define M24SR_RF_DISABLE_PIN  NC

#endif

namespace mbed {
namespace nfc {
namespace vendor {
namespace ST {

#define OPEN_SESSION_RETRIES  5
#define CC_FILE_LENGTH        15
#define NDEF_FILE_HEADER_SIZE 2
#define MAX_NDEF_SIZE         0x1FFF

/**
 * User parameter used to invoke a command,
 * it is used to provide the data back with the response
 */
struct CommandData_t {
    uint8_t *data; /**< data */
    uint16_t length; /**< number of bytes in the data array */
    uint16_t offset; /**< offset parameter used in the read/write command */
};

/**
 * @brief APDU Command structure
 */
class C_APDU {
public:
    struct C_APDUHeader_t {
        uint8_t CLA; /**< Command class */
        uint8_t INS; /**< Operation code */
        uint8_t P1; /**< Selection Mode */
        uint8_t P2; /**< Selection Option */
    };

    struct C_APDUBody_t {
        uint8_t LC; /**< Data field length */
        const uint8_t *data; /**< Command parameters */
        uint8_t LE; /**< Expected length of data to be returned */
    };

    C_APDU(uint8_t cla, uint8_t ins, uint16_t p1p2, uint8_t length, const uint8_t *data, uint8_t expected)
    {
        header.CLA = cla;
        header.INS = ins;
        header.P1 = (uint8_t)((p1p2 & 0xFF00) >> 8);
        header.P2 = (uint8_t)(p1p2 & 0x00FF);
        body.LC = length;
        body.data = data;
        body.LE = expected;
    }

    C_APDUHeader_t header;
    C_APDUBody_t body;
};

/**
 * @brief SC response structure
 */
struct R_APDU {
    uint8_t *data; /**< Data returned from the card */ // pointer on the transceiver buffer = ReaderRecBuf[CR95HF_DATA_OFFSET ];
    uint8_t SW1; /**< Command Processing status */
    uint8_t SW2; /**< Command Processing qualification */
};

enum M24srError_t : uint16_t {
    M24SR_SUCCESS = 0,
    M24SR_ERROR = 0x6F00,
    M24SR_FILE_OVERFLOW_LE = 0x6280,
    M24SR_EOF = 0x6282,
    M24SR_PASSWORD_REQUIRED = 0x6300,
    M24SR_PASSWORD_INCORRECT = 0x63C0,
    M24SR_PASSWORD_INCORRECT1RETRY = 0x63C1,
    M24SR_PASSWORD_INCORRECT2RETRY = 0x63C2,
    M24SR_WRONG_LENGHT = 0x6700,
    M24SR_UNSUCESSFUL_UPDATING = 0x6581,
    M24SR_INCOPATIBLE_COMMAND = 0x6981,
    M24SR_SECURITY_UNSATISFIED = 0x6982,
    M24SR_REFERENCE_DATA_NOT_USABLE = 0x6984,

    M24SR_INCORRECT_PARAMETER = 0x6a80,
    M24SR_FILE_NOT_FOUND = 0x6a82,
    M24SR_FILE_OVERFLOW_LC = 0x6A84,

    M24SR_INCORRECT_P1_OR_P2 = 0x6A86,
    M24SR_RF_SESSION_KILLED = 0x6500,
    M24SR_INS_NOT_SUPPORTED = 0x6D00,
    M24SR_CLASS_NOT_SUPPORTED = 0x6E00,

    M24SR_IO_ERROR_I2CTIMEOUT = 0x0011,
    M24SR_IO_ERROR_CRC = 0x0012,
    M24SR_IO_ERROR_NACK = 0x0013,
    M24SR_IO_ERROR_PARAMETER = 0x0014,
    M24SR_IO_ERROR_NBATEMPT = 0x0015,
    M24SR_IO_NOACKNOWLEDGE = 0x0016,
    M24SR_IO_PIN_NOT_CONNECTED = 0x0017
};

/**
 * @brief GPO state
 */
enum NfcGpoState_t {
    HIGH_IMPEDANCE = 0,
    SESSION_OPENED = 1,
    WIP = 2,
    I2C_ANSWER_READY = 3,
    INTERRUPT = 4,
    STATE_CONTROL = 5
};

/**
 * Possible password to set.
 */
enum PasswordType_t {
    READ_PASSWORD = 0x01, /**< Password to use before reading the tag */
    WRITE_PASSWORD = 0x02, /**< Password to use before writing the tag */
    I2C_PASSWORD = 0x03, /**< Root password, used only through nfc */
};

/**
 * Command that the component can accept
 */
enum Command_t {
    NONE,
    DESELECT,
    SELECT_APPLICATION,
    SELECT_CC_FILE,
    SELECT_NDEF_FILE,
    SELECT_SYSTEM_FILE,
    READ,
    UPDATE,
    VERIFY,
    MANAGE_I2C_GPO,
    MANAGE_RF_GPO,
    CHANGE_REFERENCE_DATA,
    ENABLE_VERIFICATION_REQUIREMENT,
    DISABLE_VERIFICATION_REQUIREMENT,
    ENABLE_PERMANET_STATE,
    DISABLE_PERMANET_STATE,
};

/**
 * Communication mode used by this device
 */
enum Communication_t {
    SYNC, /**< SYNC wait the command response before returning */
    ASYNC /**< ASYNC use a callback to notify the end of a command */
};

/**
 * Class representing a M24SR component.
 * This component has two operation modes, sync or async.
 * In sync mode each function call returns only after the command has completed.
 * In async mode each function call returns immediately and the answer will be notified
 * through a callback.
 * The default behaviour is sync mode.
 * To enable the async mode ManageI2CGPO(I2C_ANSWER_READY) function must be called.
 * When the component notifies an interrupt user must call  {@link ManageEvent} function.
 * Note that passing a parameter other than I2C_ANSWER_READY to ManageI2CGPO initialize the component in sync mode.
 */
class M24srDriver : public NFCEEPROMDriver {
public:
    /**
     * Object that contains all the callbacks fired by this class, each command has its own callback.
     * The callback default implementation is an empty function.
     */
    class Callbacks {
    public:
        /** called when get_session completes */
        virtual void on_session_open(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when deselect completes */
        virtual void on_deselect(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when select_application completes */
        virtual void on_selected_application(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when select_cc_file completes */
        virtual void on_selected_cc_file(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when select_ndef_file completes */
        virtual void on_selected_ndef_file(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when select_system_file completes */
        virtual void on_selected_system_file(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when read_binary completes */
        virtual void on_read_byte(M24srDriver *nfc, M24srError_t status, uint16_t offset, uint8_t *bytes_read,
                                  uint16_t read_count)
        {
            (void) nfc;
            (void) status;
            (void) offset;
            (void) bytes_read;
            (void) read_count;
        }

        /** called when update_binary completes */
        virtual void on_updated_binary(M24srDriver *nfc, M24srError_t status, uint16_t offset, uint8_t *bytes_written,
                                       uint16_t write_count)
        {
            (void) nfc;
            (void) status;
            (void) bytes_written;
            (void) write_count;
            (void) offset;
        }

        /** called when verify completes */
        virtual void on_verified(M24srDriver *nfc, M24srError_t status, PasswordType_t password_type, const uint8_t *pwd)
        {
            (void) nfc;
            (void) status;
            (void) password_type;
            (void) pwd;
        }

        /** called when manage_i2c_gpo completes */
        virtual void on_manage_i2c_gpo(M24srDriver *nfc, M24srError_t status, NfcGpoState_t new_status)
        {
            (void) nfc;
            (void) status;
            (void) new_status;
        }

        /** called when manage_rf_gpo completes */
        virtual void on_manage_rf_gpo(M24srDriver *nfc, M24srError_t status, NfcGpoState_t new_status)
        {
            (void) nfc;
            (void) status;
            (void) new_status;
        }

        /** called when change_reference_data completes */
        virtual void on_change_reference_data(M24srDriver *nfc, M24srError_t status, PasswordType_t type,
                                              const uint8_t *data)
        {
            (void) nfc;
            (void) status;
            (void) type;
            (void) data;
        }

        /** called when enable_verification_requirement completes */
        virtual void on_enable_verification_requirement(M24srDriver *nfc, M24srError_t status, PasswordType_t type)
        {
            (void) nfc;
            (void) status;
            (void) type;
        }

        /** called when disable_verification_requirement completes */
        virtual void on_disable_verification_requirement(M24srDriver *nfc, M24srError_t status, PasswordType_t type)
        {
            (void) nfc;
            (void) status;
            (void) type;
        }

        /** called when enable_permanent_state completes */
        virtual void on_enable_permanent_state(M24srDriver *nfc, M24srError_t status, PasswordType_t type)
        {
            (void) nfc;
            (void) status;
            (void) type;
        }

        /** called when disable_permanent_state completes */
        virtual void on_disable_permanent_state(M24srDriver *nfc, M24srError_t status, PasswordType_t type)
        {
            (void) nfc;
            (void) status;
            (void) type;
        }

        /** called when read_id completes */
        virtual void on_read_id(M24srDriver *nfc, M24srError_t status, uint8_t *id)
        {
            (void) nfc;
            (void) status;
            (void) id;
        }

        /** called when enable_read_password completes */
        virtual void on_enable_read_password(M24srDriver *nfc, M24srError_t status, const uint8_t *new_password)
        {
            (void) nfc;
            (void) status;
            (void) new_password;
        }

        /** called when oenable_write_password completes */
        virtual void on_enable_write_password(M24srDriver *nfc, M24srError_t status, const uint8_t *new_password)
        {
            (void) nfc;
            (void) status;
            (void) new_password;
        }

        /** called when disable_read_password completes */
        virtual void on_disable_read_password(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when disable_write_password completes */
        virtual void on_disable_write_password(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when disable_all_password completes */
        virtual void on_disable_all_password(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when enable_read_only completes */
        virtual void on_enable_read_only(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when enable_write_only completes */
        virtual void on_enable_write_only(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when disable_read_only completes */
        virtual void on_disable_read_only(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        /** called when disable_write_only completes */
        virtual void on_disable_write_only(M24srDriver *nfc, M24srError_t status)
        {
            (void) nfc;
            (void) status;
        }

        virtual ~Callbacks() { }
    };

public:
    /** Create the driver, default pin names will be used appropriate for the board.
     *  @param i2c_data_pin I2C data pin name.
     *  @param i2c_clock_pin I2C clock pin name.
     *  @param gpo_pin I2C GPO pin name.
     *  @param rf_disable_pin pin name for breaking the RF connection.
     */
    M24srDriver(PinName i2c_data_pin = M24SR_I2C_SDA_PIN, PinName i2c_clock_pin = M24SR_I2C_SCL_PIN,
                PinName gpo_pin = M24SR_GPO_PIN, PinName rf_disable_pin = M24SR_RF_DISABLE_PIN);

    virtual ~M24srDriver() { }

    /** @see NFCEEPROMDriver::reset
     */
    virtual void reset()
    {
        set_callback(&_default_cb);
        init();
        manage_i2c_gpo(I2C_ANSWER_READY);
    }

    /** @see NFCEEPROMDriver::get_max_size
     */
    virtual size_t read_max_size()
    {
        return MAX_NDEF_SIZE;
    }

    /** @see NFCEEPROMDriver::start_session
     */
    virtual void start_session(bool force = true)
    {
        if (_is_session_open) {
            delegate()->on_session_started(true);
            return;
        }

        set_callback(&_open_session_cb);

        get_session(force);
    }

    /** @see NFCEEPROMDriver::end_session
     */
    virtual void end_session()
    {
        set_callback(&_close_session_cb);
        deselect();
    }

    /** @see NFCEEPROMDriver::read_bytes
     */
    virtual void read_bytes(uint32_t address, uint8_t *bytes, size_t count)
    {
        if (!_is_session_open) {
            delegate()->on_bytes_read(0);
            return;
        }

        if (address > _ndef_size) {
            delegate()->on_bytes_read(0);
            return;
        }

        set_callback(&_read_byte_cb);

        if (count > _max_read_bytes) {
            count = _max_read_bytes;
        }

        if (address + count > _ndef_size) {
            count = _ndef_size - address;
        }

        if (count == 0) {
            delegate()->on_bytes_read(0);
            return;
        }

        /* offset by ndef file size*/
        address += NDEF_FILE_HEADER_SIZE;

        read_binary((uint16_t) address, (uint8_t) count, bytes);
    }

    /** @see NFCEEPROMDriver::write_bytes
     */
    virtual void write_bytes(uint32_t address, const uint8_t *bytes, size_t count)
    {
        if (!_is_session_open) {
            delegate()->on_bytes_written(0);
            return;
        }

        if (address > _ndef_size) {
            delegate()->on_bytes_written(0);
            return;
        }

        if (bytes) {
            set_callback(&_write_byte_cb);
        } else {
            set_callback(&_erase_bytes_cb);
        }

        if (count > _max_write_bytes) {
            count = _max_write_bytes;
        }

        if (address + count > _ndef_size) {
            count = _ndef_size - address;
        }

        if (count == 0) {
            delegate()->on_bytes_written(0);
            return;
        }

        /* offset by ndef file size*/
        address += NDEF_FILE_HEADER_SIZE;

        update_binary((uint16_t) address, (uint8_t) count, bytes);
    }

    /** @see NFCEEPROMDriver::set_size
     */
    virtual void write_size(size_t count)
    {
        if (!_is_session_open) {
            delegate()->on_size_read(false, 0);
            return;
        }

        if (count > MAX_NDEF_SIZE - NDEF_FILE_HEADER_SIZE) {
            delegate()->on_size_read(false, 0);
            return;
        }

        set_callback(&_set_size_cb);

        _ndef_size = (uint16_t)count;

        /* NDEF file size is BE */
        uint8_t *bytes = (uint8_t *)&_ndef_size;
        _ndef_size_buffer[0] = bytes[1];
        _ndef_size_buffer[1] = bytes[0];

        update_binary(0, NDEF_FILE_HEADER_SIZE, (const uint8_t *)&_ndef_size_buffer);
    }

    /** @see NFCEEPROMDriver::get_size
     */
    virtual void read_size()
    {
        if (!_is_session_open) {
            delegate()->on_size_read(false, 0);
            return;
        }

        set_callback(&_get_size_cb);

        read_binary(0, NDEF_FILE_HEADER_SIZE, (uint8_t *)&_ndef_size_buffer);
    }

    /** @see NFCEEPROMDriver::erase_bytes
     */
    virtual void erase_bytes(uint32_t address, size_t size)
    {
        write_bytes(address, NULL, size);
    }

private:
    /**
     * Change the function to call when a command ends.
     * @param commandCallback Object containing the callback, if NULL it will use empty callback
     */
    void set_callback(Callbacks *callback)
    {
        if (callback) {
            _command_cb = callback;
        } else {
            _command_cb = &_default_cb;
        }
    }

    /**
     * get the callback object to use
     * @return callback object to use
     */
    Callbacks *get_callback()
    {
        /* this allows for two levels of operation, the previous command will continue
         * when this set of callbacks has finished */
        if (_subcommand_cb) {
            return _subcommand_cb;
        }
        return _command_cb;
    }

    void nfc_interrupt_callback()
    {
        if (_communication_type == ASYNC) {
            event_queue()->call(this, &M24srDriver::manage_event);
        }
    }

    /**
     * Enable the request of a password before reading the tag.
     * @param current_write_password Current password
     * @param new_password Password to request before reading the tag.
     * @return return M24SR_SUCCESS if no errors
     * @note The password must have a length of 16 chars.
     */
    M24srError_t enable_read_password(const uint8_t *current_write_password, const uint8_t *new_password)
    {
        _subcommand_cb = &_change_password_request_status_cb;
        _change_password_request_status_cb.set_task(READ_PASSWORD, new_password);

        return verify(WRITE_PASSWORD, current_write_password);
    }

    /**
     * Disable the request of a password before reading the tag.
     * @param current_write_password Current password
     * @return return M24SR_SUCCESS if no errors
     * @note The password must have a length of 16 chars.
     */
    M24srError_t disable_read_password(const uint8_t *current_write_password)
    {
        _subcommand_cb = &_change_password_request_status_cb;
        _change_password_request_status_cb.set_task(READ_PASSWORD, NULL);

        return verify(WRITE_PASSWORD, current_write_password);
    }

    /**
     * Enable the request of a password before writing to the tag.
     * @param current_write_password Current password
     * @param new_password Password to request before reading the tag.
     * @return return M24SR_SUCCESS if no errors
     * @note The password must have a length of 16 chars.
     */
    M24srError_t enable_write_password(const uint8_t *current_write_password, uint8_t *new_password)
    {
        _subcommand_cb = &_change_password_request_status_cb;
        _change_password_request_status_cb.set_task(WRITE_PASSWORD, new_password);

        return verify(WRITE_PASSWORD, current_write_password);
    }

    /**
     * Disable the request of a password before writing the tag.
     * @param current_write_password Current password.
     * @return return M24SR_SUCCESS if no errors
     * @note The password must have a length of 16 chars.
     */
    M24srError_t disable_write_password(const uint8_t *current_write_password)
    {
        _subcommand_cb = &_change_password_request_status_cb;
        _change_password_request_status_cb.set_task(WRITE_PASSWORD, NULL);

        return verify(WRITE_PASSWORD, current_write_password);
    }

    /**
     * @brief This function disables both read and write passwords.
     * @param super_user_password I2C super user password.
     * @return return M24SR_SUCCESS if no errors
     * @note The password must have a length of 16 chars.
     */
    M24srError_t disable_all_password(const uint8_t *super_user_password)
    {
        _subcommand_cb = &_remove_password_cb;
        return verify(I2C_PASSWORD, super_user_password);
    }

    /**
     * @brief This function enables read only mode.
     * @param current_write_password Write password is needed to enable read only mode.
     * @return return M24SR_SUCCESS if no errors
     * @note The password must have a length of 16 chars.
     */
    M24srError_t enable_read_only(const uint8_t *current_write_password)
    {
        _subcommand_cb = &_change_access_state_cb;
        _change_access_state_cb.change_access_state(ChangeAccessStateCallback::WRITE, false);

        return verify(WRITE_PASSWORD, current_write_password);
    }

    /**
     * @brief This function disables read only mode.
     * @param current_write_password Write password is needed to disable read only mode.
     * @return return M24SR_SUCCESS if no errors
     * @note The password must have a length of 16 chars.
     */
    M24srError_t disable_read_only(const uint8_t *current_write_password)
    {
        _subcommand_cb = &_change_access_state_cb;
        _change_access_state_cb.change_access_state(ChangeAccessStateCallback::WRITE, true);

        return verify(I2C_PASSWORD, current_write_password);
    }

    /**
     * @brief This function enables write only mode.
     * @param current_write_password Write password is needed to enable write only mode.
     * @return return M24SR_SUCCESS if no errors
     * @note The password must have a length of 16 chars.
     */
    M24srError_t enable_write_only(const uint8_t *current_write_password)
    {
        _subcommand_cb = &_change_access_state_cb;
        _change_access_state_cb.change_access_state(ChangeAccessStateCallback::READ, false);

        return verify(WRITE_PASSWORD, current_write_password);
    }

    /**
     * @brief This function disables write only mode.
     * @param current_write_password Write password is needed to disable write only mode.
     * @return return M24SR_SUCCESS if no errors
     * @note The password must have a length of 16 chars.
     */
    M24srError_t disable_write_only(const uint8_t *current_write_password)
    {
        _subcommand_cb = &_change_access_state_cb;
        _change_access_state_cb.change_access_state(ChangeAccessStateCallback::READ, true);

        return verify(I2C_PASSWORD, current_write_password);
    }

private:
    M24srError_t init();
    M24srError_t read_id(uint8_t *nfc_id);
    M24srError_t get_session(bool force = false);

    M24srError_t deselect();
    M24srError_t receive_deselect();

    M24srError_t select_application();
    M24srError_t receive_select_application();

    M24srError_t select_cc_file();
    M24srError_t receive_select_cc_file();

    M24srError_t select_ndef_file(uint16_t ndef_file_id);
    M24srError_t receive_select_ndef_file();

    M24srError_t select_system_file();
    M24srError_t receive_select_system_file();

    M24srError_t read_binary(uint16_t offset, uint8_t length, uint8_t *buffer);
    M24srError_t st_read_binary(uint16_t offset, uint8_t length, uint8_t *buffer);
    M24srError_t receive_read_binary();

    M24srError_t update_binary(uint16_t offset, uint8_t length, const uint8_t *data);
    M24srError_t receive_update_binary();

    M24srError_t verify(PasswordType_t password_type, const uint8_t *password);
    M24srError_t receive_verify();

    M24srError_t change_reference_data(PasswordType_t password_type, const uint8_t *password);
    M24srError_t receive_change_reference_data();

    M24srError_t enable_verification_requirement(PasswordType_t password_type);
    M24srError_t receive_enable_verification_requirement();

    M24srError_t disable_verification_requirement(PasswordType_t password_type);
    M24srError_t receive_disable_verification_requirement();

    M24srError_t enable_permanent_state(PasswordType_t password_type);
    M24srError_t receive_enable_permanent_state();

    M24srError_t disable_permanent_state(PasswordType_t password_type);
    M24srError_t receive_disable_permanent_state();

    M24srError_t send_interrupt();
    M24srError_t state_control(bool gpo_reset);

    M24srError_t manage_i2c_gpo(NfcGpoState_t gpo_i2c_config);
    M24srError_t manage_rf_gpo(NfcGpoState_t gpo_rf_config);

    M24srError_t rf_config(bool enable);
    M24srError_t send_fwt_extension(uint8_t fwt_byte);

    M24srError_t send_receive_i2c(uint16_t length, uint8_t *command);

    /**
     * Function to call when the component fire an interrupt.
     * @return last operation status
     */
    M24srError_t manage_event();

    /**
     * Send a command to the component.
     * @param length Length of the command.
     * @param command Buffer containing the command.
     * @return M24SR_SUCCESS if no errors
     */
    M24srError_t io_send_i2c_command(uint8_t length, const uint8_t *command);

    /**
     * Read a command response.
     * @param length Number of bytes to read.
     * @param command Buffer to store the response into.
     * @return M24SR_SUCCESS if no errors
     */
    M24srError_t io_receive_i2c_response(uint8_t length, uint8_t *command);

    /**
     * Do an active polling on the I2C bus until the answer is ready.
     * @return M24SR_SUCCESS if no errors
     */
    M24srError_t io_poll_i2c();

    bool manage_sync_communication(M24srError_t *status);

private:
    /**
     * @brief This class permits to enable/disable the password request to read/write into the tag
     */
    class ChangePasswordRequestStatusCallback : public Callbacks {
    public:
        /**
         * Build the chain of callbacks.
         */
        ChangePasswordRequestStatusCallback()
            : _new_password(NULL),
              _type(I2C_PASSWORD),
              _enable(false) { }

        /* This class is equivalent to calling the methods:
         *
         * To enable the request:
         * - Verify
         * - change_reference_data
         * - enable_permanent_state
         *
         * To disable the request:
         * - verify
         * - disable_verification_requirement
         */

        /**
         * Set the password to enable/disable.
         * @param type Type of password to enable/disable.
         * @param new_password Array of 16bytes with the new password, if null the request will be disabled.
         */
        void set_task(PasswordType_t type, const uint8_t *new_password)
        {
            _new_password = new_password;
            _type = type;
            _enable = (new_password != NULL);
        }

        virtual void on_verified(M24srDriver *nfc, M24srError_t status, PasswordType_t, const uint8_t *)
        {
            if (status != M24SR_SUCCESS) {
                return on_finish_command(nfc, status);
            }
            if (_enable) {
                nfc->change_reference_data(_type, _new_password);
            } else {
                nfc->disable_verification_requirement(_type);
            }
        }

        virtual void on_disable_verification_requirement(M24srDriver *nfc, M24srError_t status, PasswordType_t)
        {
            on_finish_command(nfc, status);
        }

        virtual void on_change_reference_data(M24srDriver *nfc, M24srError_t status, PasswordType_t type, const uint8_t *)
        {
            if (status == M24SR_SUCCESS) {
                nfc->enable_permanent_state(type);
            } else {
                on_finish_command(nfc, status);
            }
        }

        virtual void on_enable_permanent_state(M24srDriver *nfc, M24srError_t status, PasswordType_t)
        {
            on_finish_command(nfc, status);
        }

    private:
        /**
         * Remove the private callbacks and call the user callback.
         * @param nfc Object triggering the command.
         * @param status Command status.
         */
        void on_finish_command(M24srDriver *nfc, M24srError_t status)
        {
            nfc->_subcommand_cb = NULL;

            if (_enable) {
                if (_type == READ_PASSWORD) {
                    nfc->get_callback()->on_enable_read_password(nfc, status, _new_password);
                } else {
                    nfc->get_callback()->on_enable_write_password(nfc, status, _new_password);
                }
            } else {
                if (_type == READ_PASSWORD) {
                    nfc->get_callback()->on_disable_read_password(nfc, status);
                } else {
                    nfc->get_callback()->on_disable_write_password(nfc, status);
                }
            }
        }

    private:
        const uint8_t *_new_password;
        PasswordType_t _type;
        bool _enable;
    };

    /**
     * @brief This class permits to disable all the password requests to read/write into the tag.
     */
    class RemoveAllPasswordCallback : public Callbacks {
    public:
        /**
         * Build the chain of callbacks.
         */
        RemoveAllPasswordCallback()
            : _password(NULL) { }

        /* This class is equivalent to calling the methods:
         * - verify(i2c)
         * - disable_permanent_state(Read)
         * - disable_permanent_state(write)
         * - disable_verification_requirement(Read)
         * - disable_verification_requirement(write)
         * - change_reference_data(Read)
         * - change_reference_data(write)
         */

        virtual void on_verified(M24srDriver *nfc, M24srError_t status, PasswordType_t, const uint8_t *data)
        {
            if (status != M24SR_SUCCESS) {
                return on_finish_command(nfc, status);
            }
            _password = data;
            nfc->disable_permanent_state(READ_PASSWORD);
        }

        virtual void on_disable_permanent_state(M24srDriver *nfc, M24srError_t status, PasswordType_t type)
        {
            if (status != M24SR_SUCCESS) {
                return on_finish_command(nfc, status);
            }
            if (type == READ_PASSWORD) {
                nfc->disable_permanent_state(WRITE_PASSWORD);
            } else {
                nfc->disable_verification_requirement(READ_PASSWORD);
            }
        }

        virtual void on_disable_verification_requirement(M24srDriver *nfc, M24srError_t status, PasswordType_t type)
        {
            if (status != M24SR_SUCCESS) {
                return on_finish_command(nfc, status);
            }
            if (type == READ_PASSWORD) {
                nfc->disable_verification_requirement(WRITE_PASSWORD);
            } else {
                nfc->change_reference_data(READ_PASSWORD, _password);
            }
        }

        virtual void on_change_reference_data(M24srDriver *nfc, M24srError_t status, PasswordType_t type,
                                              const uint8_t *data)
        {
            if (status != M24SR_SUCCESS) {
                return on_finish_command(nfc, status);
            }
            if (type == READ_PASSWORD) {
                nfc->change_reference_data(WRITE_PASSWORD, data);
            } else {
                on_finish_command(nfc, status);
            }
        }

    private:
        /**
         * Remove the private callback and call the onDisableAllPassword callback.
         * @param nfc Object triggering the command.
         * @param status Command status.
         */
        void on_finish_command(M24srDriver *nfc, M24srError_t status)
        {
            nfc->_subcommand_cb = NULL;
            _password = NULL;
            nfc->get_callback()->on_disable_all_password(nfc, status);
        }

    private:
        /**
         * Store the default password used for open a super user session
         * it will be set as default read/write password
         */
        const uint8_t *_password;
    };

    /**
     * @brief This class permits to set the tag as read/write only.
     */
    class ChangeAccessStateCallback : public Callbacks {
    public:
        enum AccessType_t {
            WRITE,
            READ
        };

        /**
         * Build the chain of callbacks.
         */
        ChangeAccessStateCallback()
            : _type(WRITE),
              _enable(false) { }

        /* This class is equivalent to calling the methods:
         * - verify(i2c)
         * - enable_permanent_state(Read/write)
         * or:
         * - verify(i2c)
         * - disable_permanent_state</li>
         * - disable_verification_requirement(Read/write)
         */

        /**
         * Set the access to enable/disable an access type.
         * @param type Access type.
         * @param enable True to enable the state, False to disable it.
         */
        void change_access_state(AccessType_t type, bool enable)
        {
            _type = type;
            _enable = enable;
        }

        virtual void on_verified(M24srDriver *nfc, M24srError_t status, PasswordType_t, const uint8_t *)
        {
            if (status != M24SR_SUCCESS) {
                return on_finish_command(nfc, status);
            }

            if (_enable) {
                nfc->disable_permanent_state(_type == WRITE ? WRITE_PASSWORD : READ_PASSWORD);
            } else {
                nfc->enable_permanent_state(_type == WRITE ? WRITE_PASSWORD : READ_PASSWORD);
            }

        }

        virtual void on_disable_permanent_state(M24srDriver *nfc, M24srError_t status, PasswordType_t type)
        {
            if (status != M24SR_SUCCESS) {
                return on_finish_command(nfc, status);
            }

            nfc->disable_verification_requirement(type);
        }

        virtual void on_disable_verification_requirement(M24srDriver *nfc, M24srError_t status, PasswordType_t)
        {
            on_finish_command(nfc, status);
        }

        virtual void on_enable_permanent_state(M24srDriver *nfc, M24srError_t status, PasswordType_t)
        {
            on_finish_command(nfc, status);
        }

    private:
        /**
         * Remove the private callback and call the user callback.
         * @param nfc Object triggering the command.
         * @param status Command status.
         */
        void on_finish_command(M24srDriver *nfc, M24srError_t status)
        {
            nfc->_subcommand_cb = NULL;
            if (_enable) {
                if (_type == READ) {
                    //enable read = disable write only
                    nfc->get_callback()->on_disable_write_only(nfc, status);
                } else {
                    //enable write = disable read only
                    nfc->get_callback()->on_disable_read_only(nfc, status);
                }
            } else {
                if (_type == WRITE) {
                    //disable write = enable read only
                    nfc->get_callback()->on_enable_read_only(nfc, status);
                } else {
                    nfc->get_callback()->on_enable_write_only(nfc, status);
                }
            }
        }

    private:
        AccessType_t _type;
        bool _enable;
    };

    /**
     * @brief Object with the callback used to send a ManageGPO command.
     */
    class ManageGPOCallback : public Callbacks {

    public:
        /**
         * Build the chain of callbacks.
         * @param parent Parent component to run the command on.
         */
        ManageGPOCallback()
            : _new_gpo_config(HIGH_IMPEDANCE),
              _read_gpo_config(0),
              _change_i2c_gpo(true) { }

        /* This class is equivalent to calling the methods:
         * - selected_application
         * - select_system_file
         * - read_binary
         * - verify
         * - update_binary
         */

        /**
         * Command parameters.
         * @param i2cGpo true to change the i2c gpo, false for the rf gpo.
         * @param new_config new gpo function.
         */
        void set_new_gpo_config(bool i2cGpo, NfcGpoState_t new_config)
        {
            _new_gpo_config = new_config;
            _change_i2c_gpo = i2cGpo;
        }

        virtual void on_selected_application(M24srDriver *nfc, M24srError_t status)
        {
            if (status == M24SR_SUCCESS) {
                nfc->select_system_file();
            } else {
                on_finish_command(nfc, status);
            }
        }

        virtual void on_selected_system_file(M24srDriver *nfc, M24srError_t status)
        {
            if (status == M24SR_SUCCESS) {
                nfc->read_binary(0x0004, 0x01, &_read_gpo_config);
            } else {
                on_finish_command(nfc, status);
            }
        }

        virtual void on_read_byte(M24srDriver *nfc, M24srError_t status, uint16_t, uint8_t *, uint16_t)
        {
            if (status == M24SR_SUCCESS) {
                nfc->verify(I2C_PASSWORD, default_password);
            } else {
                on_finish_command(nfc, status);
            }
        }

        virtual void on_verified(M24srDriver *nfc, M24srError_t status, PasswordType_t, const uint8_t *)
        {
            if (status != M24SR_SUCCESS) {
                return on_finish_command(nfc, status);
            }

            if (_change_i2c_gpo) {
                _read_gpo_config = (_read_gpo_config & 0xF0) | (uint8_t) _new_gpo_config;
            } else {
                _read_gpo_config = (_read_gpo_config & 0x0F) | (((uint8_t) _new_gpo_config) << 4);
            }

            nfc->update_binary(0x0004, 0x01, &_read_gpo_config);
        }

        virtual void on_updated_binary(M24srDriver *nfc, M24srError_t status, uint16_t, uint8_t *, uint16_t)
        {

            if (status == M24SR_SUCCESS) {
                if (_new_gpo_config == I2C_ANSWER_READY) {
                    nfc->_communication_type = ASYNC;
                } else {
                    nfc->_communication_type = SYNC;
                }
            }
            on_finish_command(nfc, status);
        }

    private:
        /**
         * Remove the private callback and call the user callback.
         * @param nfc Object where the command was send to.
         * @param status Command status.
         */
        void on_finish_command(M24srDriver *nfc, M24srError_t status)
        {
            nfc->_subcommand_cb = NULL;
            if (_change_i2c_gpo) {
                nfc->_command_cb->on_manage_i2c_gpo(nfc, status, _new_gpo_config);
            } else {
                nfc->_command_cb->on_manage_rf_gpo(nfc, status, _new_gpo_config);
            }
        }

    private:
        /** new gpo function that this class has to write */
        NfcGpoState_t _new_gpo_config;

        /** variable where storeing the read gpo configuration */
        uint8_t _read_gpo_config;

        /** true to change the i2c gpo, false to change the rf gpo */
        bool _change_i2c_gpo;
    };

    /**
     * @brief Object with the callback used to read the component ID
     */
    class ReadIDCallback : public Callbacks {
    public:
        /**
         * Build the chain of callbacks.
         * @param parent object where to send the command to.
         */
        ReadIDCallback() : _id(NULL) { }

        /* This class is equivalent to calling the methods:
         * - select_application
         * - select_system_file
         * - read_binary
         */

        /**
         * Set the variable containing the result
         * @param idPtr
         */
        void set_task(uint8_t *id)
        {
            _id = id;
        }

        virtual void on_selected_application(M24srDriver *nfc, M24srError_t status)
        {
            if (status == M24SR_SUCCESS) {
                nfc->select_system_file();
            } else {
                on_finish_command(nfc, status);
            }

        }

        virtual void on_selected_system_file(M24srDriver *nfc, M24srError_t status)
        {
            if (status == M24SR_SUCCESS) {
                nfc->read_binary(0x0011, 0x01, _id);
            } else {
                on_finish_command(nfc, status);
            }
        }

        virtual void on_read_byte(M24srDriver *nfc, M24srError_t status, uint16_t, uint8_t *, uint16_t)
        {
            on_finish_command(nfc, status);
        }

    private:
        /**
         * Remove the private callback and call the user onReadId function.
         * @param nfc Object where the command was send.
         * @param status Command status.
         */
        void on_finish_command(M24srDriver *nfc, M24srError_t status)
        {
            nfc->_subcommand_cb = NULL;
            nfc->get_callback()->on_read_id(nfc, status, _id);
        }

    private:
        /** pointer to read id */
        uint8_t *_id;
    };

    /**
     * Class containing the callback needed to open a session and read the max
     * read/write size
     */
    class OpenSessionCallBack : public Callbacks {
    public:
        OpenSessionCallBack()
            : _retries(OPEN_SESSION_RETRIES) { }

        void on_session_open(M24srDriver *nfc, M24srError_t status)
        {
            if (status == M24SR_SUCCESS) {
                nfc->select_application();
            } else {
                nfc->delegate()->on_session_started(false);
            }
        }

        void on_selected_application(M24srDriver *nfc, M24srError_t status)
        {
            if (status == M24SR_SUCCESS) {
                nfc->select_cc_file();
            } else {
                if (_retries == 0) {
                    nfc->delegate()->on_session_started(false);
                } else {
                    _retries--;
                    nfc->select_application();
                }
            }
        }

        void on_selected_cc_file(M24srDriver *nfc, M24srError_t status)
        {
            if (status == M24SR_SUCCESS) {
                nfc->read_binary(0x0000, CC_FILE_LENGTH, CCFile);
            } else {
                nfc->delegate()->on_session_started(false);
            }
        }

        void on_read_byte(M24srDriver *nfc, M24srError_t status, uint16_t, uint8_t *bytes_read,
                          uint16_t read_count)
        {
            if (status != M24SR_SUCCESS || read_count != CC_FILE_LENGTH) {
                nfc->delegate()->on_session_started(false);
            }
            uint16_t ndef_file_id = (uint16_t)((bytes_read[0x09] << 8) | bytes_read[0x0A]);
            nfc->_max_read_bytes = (uint16_t)((bytes_read[0x03] << 8) | bytes_read[0x04]);
            nfc->_max_write_bytes = (uint16_t)((bytes_read[0x05] << 8) | bytes_read[0x06]);
            nfc->select_ndef_file(ndef_file_id);
        }

        void on_selected_ndef_file(M24srDriver *nfc, M24srError_t status)
        {
            nfc->_is_session_open = (status == M24SR_SUCCESS);
            nfc->delegate()->on_session_started(nfc->_is_session_open);
        }

    private:
        /** number of trials done for open the session */
        uint32_t _retries;

        /** buffer where read the CC file */
        uint8_t CCFile[15];
    };

    /**
     * Class containing the callback needed to close a session
     */
    class CloseSessionCallBack : public Callbacks {
    public:
        CloseSessionCallBack() { }

        virtual void on_deselect(M24srDriver *nfc, M24srError_t status)
        {
            if (status == M24SR_SUCCESS) {
                nfc->_is_session_open = false;
                nfc->delegate()->on_session_ended(true);
            } else {
                nfc->delegate()->on_session_ended(false);
            }
        }
    };

    /**
     * Class containing the callback needed to write a buffer
     */
    class WriteByteCallback : public Callbacks {
    public:
        WriteByteCallback() { }

        virtual void on_updated_binary(M24srDriver *nfc, M24srError_t status, uint16_t offset, uint8_t *bytes_written,
                                       uint16_t write_count)
        {
            if (status != M24SR_SUCCESS) {
                nfc->delegate()->on_bytes_written(0);
                return;
            }

            nfc->delegate()->on_bytes_written(write_count);
        }
    };

    /**
     * Class containing the callback needed to read a buffer
     */
    class ReadByteCallback : public Callbacks {
    public:
        ReadByteCallback() { }

        virtual void on_read_byte(M24srDriver *nfc, M24srError_t status, uint16_t offset, uint8_t *bytes_read,
                                  uint16_t read_count)
        {
            if (status != M24SR_SUCCESS) {
                nfc->delegate()->on_bytes_read(0);
                return;
            }

            nfc->delegate()->on_bytes_read(read_count);
        }
    };

    class SetSizeCallback : public Callbacks {
    public:
        SetSizeCallback() { }

        virtual void on_updated_binary(M24srDriver *nfc, M24srError_t status, uint16_t offset, uint8_t *bytes_written,
                                       uint16_t write_count)
        {
            if (status != M24SR_SUCCESS) {
                nfc->delegate()->on_size_written(false);
                return;
            }

            nfc->delegate()->on_size_written(true);
        }
    };

    class GetSizeCallback : public Callbacks {
    public:
        GetSizeCallback() { }

        virtual void on_read_byte(M24srDriver *nfc, M24srError_t status, uint16_t offset, uint8_t *bytes_read,
                                  uint16_t read_count)
        {
            if (status != M24SR_SUCCESS) {
                nfc->delegate()->on_size_read(false, 0);
                return;
            }

            /* NDEF file size is BE */
            nfc->_ndef_size = (((uint16_t) nfc->_ndef_size_buffer[0]) << 8 | nfc->_ndef_size_buffer[1]);

            nfc->delegate()->on_size_read(true, nfc->_ndef_size);
        }
    };

    class EraseBytesCallback : public Callbacks {
    public:
        EraseBytesCallback() { }

        virtual void on_updated_binary(M24srDriver *nfc, M24srError_t status, uint16_t offset, uint8_t *bytes_written,
                                       uint16_t write_count)
        {
            if (status != M24SR_SUCCESS) {
                nfc->delegate()->on_bytes_erased(0);
                return;
            }

            nfc->delegate()->on_bytes_erased(write_count);
        }
    };

private:
    /** Default password used to change the write/read permission */
    static const uint8_t default_password[16];

    I2C _i2c_channel;

    /** Interrupt object fired when the gpo status changes */
    InterruptIn _gpo_event_interrupt;
    DigitalIn _gpo_pin;
    DigitalOut _rf_disable_pin;

    /** object containing the callbacks to use*/
    Callbacks *_command_cb;

    /**
     * Object with private callbacks used to hide high level commands each
     * calling multiple low level commands. This callbacks object has
     * higher priority comparing to the user callbacks.
     */
    Callbacks *_subcommand_cb;

    Callbacks _default_cb;
    ManageGPOCallback _manage_gpo_cb;
    ReadIDCallback _read_id_cb;
    ChangePasswordRequestStatusCallback _change_password_request_status_cb;
    RemoveAllPasswordCallback _remove_password_cb;
    ChangeAccessStateCallback _change_access_state_cb;
    OpenSessionCallBack _open_session_cb;
    CloseSessionCallBack _close_session_cb;
    WriteByteCallback _write_byte_cb;
    ReadByteCallback _read_byte_cb;
    SetSizeCallback _set_size_cb;
    GetSizeCallback _get_size_cb;
    EraseBytesCallback _erase_bytes_cb;


    uint8_t _buffer[0xFF];

    /** Type of communication being used (SYNC, ASYNC) */
    Communication_t _communication_type;

    Command_t _last_command;
    CommandData_t _last_command_data;

    /** Buffer used to build the command to send to the chip. */
    uint16_t _ndef_size;
    uint8_t _ndef_size_buffer[NDEF_FILE_HEADER_SIZE];
    uint8_t _max_read_bytes;
    uint8_t _max_write_bytes;
    uint8_t _did_byte;

    bool _is_session_open;
};

} //ST
} //vendor
} //nfc
} //mbed

#endif // M24SR_H

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
