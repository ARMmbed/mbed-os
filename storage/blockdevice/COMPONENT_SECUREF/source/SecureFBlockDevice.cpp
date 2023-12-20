/* mbed Microcontroller Library
 * Copyright (c) 2023 ARM Limited
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

#include "SecureFBlockDevice.h"
#include "rtos/ThisThread.h"
#include "mbed_critical.h"

#include <string.h>
#include <inttypes.h>

#include "mbed_trace.h"

#include "../spi_nor_flash/spi_nor.h"
#include "../TG424_3/JEDEC_security_HAL/include/error.h"
#include "../TG424_3/JEDEC_security_HAL/include/jedec_defs.h"
#include "../TG424_3/JEDEC_security_HAL/jedec_security_hal.h"
#include "../TG424_3/vendor_impl/vendor_secureflash.h"

#define TRACE_GROUP "SECUREF"
using namespace std::chrono;
using namespace mbed;

// Status Register Bits
#define SPIF_STATUS_BIT_WIP 0x1 //Write In Progress
#define SPIF_STATUS_BIT_WEL 0x2 // Write Enable Latch

mbed::SPI *SecureFBlockDevice::_spi;
// Mutex is used for some SPI Driver commands that must be done sequentially with no other commands in between
// e.g. (1)Set Write Enable, (2)Program, (3)Wait Memory Ready
SingletonPtr<PlatformMutex> SecureFBlockDevice::_mutex;

//**************************
// SECUREF Block Device APIs
//**************************
SecureFBlockDevice::SecureFBlockDevice(PinName mosi, PinName miso, PinName sclk, PinName csel, int freq)
    :    
    _mosi(mosi), _miso(miso), _sclk(sclk), _csel(csel), _freq(freq),
    _is_initialized(false)
{}

int SecureFBlockDevice::init()
{
    int status = SECUREF_BD_ERROR_OK;    

    _mutex->lock();

    if (!_is_initialized) {
        _init_ref_count = 0;
    }

    _init_ref_count++;

    if (_init_ref_count != 1) {
        goto exit_point;
    }
    memset(&_secureflash, 0x00, sizeof (secureflash_t));

    _spi = new (std::nothrow) mbed::SPI(_mosi, _miso, _sclk, _csel, use_gpio_ssel);
    if (0 == _spi) {
        tr_error("Allocation of mbed::SPI failed");
        goto exit_point;
    }
    if (SECUREF_BD_ERROR_OK != _spi_set_frequency(_freq)) {
        tr_error("SPI Set Frequency Failed");
        goto exit_point;
    }
    if (0 != spi_nor_init(_spi_read, _spi_write)) {
        status = SECUREF_BD_ERROR_DEVICE_ERROR;
        goto exit_point;
    }
    // Soft Reset
    if (0 != _reset_flash_mem()) {
        tr_error("init - Unable to initialize flash memory, tests failed");
        status = SECUREF_BD_ERROR_DEVICE_ERROR;
        goto exit_point;
    } else {
        tr_debug("Initialize flash memory OK");
    } 
    // Synchronize Device
    if (0 != spi_nor_polling_for_wr_ready()) {
        tr_error("init - spi_nor_polling_for_wr_ready Failed");
        status = SECUREF_BD_ERROR_READY_FAILED;
        goto exit_point;
    }
    tr_debug("Probe secure flash");
    status = _probe();
    if (SECUREF_BD_ERROR_OK != status) {
        goto exit_point;
    }
#ifdef SECUREFLASH_PROVISION
    tr_debug("Provision the secure flash");
    status = _secureflash.flash_info.vendor_provisioning_op->perform_and_verify(_secureflash.flash_info.vendor_ctx, 
                                                                                NULL, 0);
    if (JEDEC_ERROR_NONE != status) {
        status = SECUREF_BD_ERROR_PROVISION_FLASH;
        goto exit_point;
    }
#endif /* SECUREFLASH_PROVISION */    
    tr_debug("Get the applicaition information");
    status = _secureflash_get_app_info();
    if (SECUREF_BD_ERROR_OK != status) {
        goto exit_point;
    }
    tr_debug("Jedect secure init");
    status = jedec_secure_init(SECUREFLASH_AUTHEN_KEY_ID);
    if (JEDEC_ERROR_NONE != status) {
        status = SECUREF_BD_ERROR_DEVICE_ERROR;
        goto exit_point;
    }        
    _is_initialized = true;
    tr_debug("Init successful");
exit_point:
    if (false == _is_initialized && 0 != _spi) {
        delete _spi;
    }
    _mutex->unlock();
    return status;
}

int SecureFBlockDevice::_probe()
{
    uint8_t vendor_device_ids[4];
    uint32_t data_length = 3;

    // Read Manufacturer ID (1byte), and Device ID (2bytes)
    if (0 != spi_nor_read_id(vendor_device_ids, data_length)) {
        tr_error("Read Vendor ID Failed");
        return SECUREF_BD_ERROR_DEVICE_ERROR;
    }

    tr_debug("Vendor device ID = 0x%x 0x%x 0x%x", vendor_device_ids[0], vendor_device_ids[1], vendor_device_ids[2]);

    if (0 != _find_flash_info(vendor_device_ids)) {
        return SECUREF_BD_ERROR_DEVICE_UNSUPPORT;
    }
    return SECUREF_BD_ERROR_OK;
}

int SecureFBlockDevice::_find_flash_info(uint8_t *id)
{
    for (uint8_t i = 0; i < sizeof(flash_info); i++) {
        if (!memcmp(id, flash_info[i]->id, flash_info[i]->id_len)) {
            jedec_set_vendor(flash_info[i]->vendor_security_op,
                             flash_info[i]->crypto_wrapper,
                             flash_info[i]->vendor_ctx);
            flash_info[i]->crypto_wrapper->init();
            memcpy(&_secureflash.flash_info, flash_info[i], sizeof(secure_flash_info_t));
            return 0;
        }
    }
    return -1;
}

int SecureFBlockDevice::_secureflash_get_app_info()
{
    int status = JEDEC_ERROR_NONE;
    uint32_t actual_size;
    vendor_provisioning_op_t *provision_op = _secureflash.flash_info.vendor_provisioning_op;
    
    status = provision_op->provision_item_get_data(_secureflash.flash_info.vendor_ctx,
                                                   ITEM_APP_INFO,
                                                   (uint8_t *)_secureflash.app_info.app_data,
                                                   sizeof(_secureflash.app_info.app_data),
                                                   &_secureflash.app_info.num,
                                                   &actual_size);
    if (JEDEC_ERROR_NONE != status) {
        return SECUREF_BD_ERROR_GET_PROVISION;
    }        
    return SECUREF_BD_ERROR_OK;
}

int SecureFBlockDevice::deinit()
{
    securef_bd_error status = SECUREF_BD_ERROR_OK;

    _mutex->lock();
    if (!_is_initialized) {
        _init_ref_count = 0;
        goto exit_point;
    }    
    _init_ref_count--;

    if (_init_ref_count) {
        goto exit_point;
    }    
    _is_initialized = false;

exit_point:
    if (false == _is_initialized && 0 != _spi) {
        delete _spi;
    }    
    _mutex->unlock();
    return status;
}

int SecureFBlockDevice::_spi_read(uint8_t *tx_buf, uint32_t tx_len, uint8_t *rx_buf, uint32_t rx_len)
{    
    _spi->select();    
    
    // Write/Read Data
    for (uint32_t i = 0; i < tx_len; i++) {        
        _spi->write(tx_buf[i]);
    }
    for (uint32_t i = 0; i < rx_len; i++) {        
        rx_buf[i] = _spi->write(0);        
    }    

    _spi->deselect();    
    return 0;
}

int SecureFBlockDevice::_spi_write(uint8_t *tx_buf, uint32_t tx_len)
{    
    _spi->select();
    
    // Write Data
    for (uint32_t i = 0; i < tx_len; i++) {        
        _spi->write(tx_buf[i]);
    }

    _spi->deselect();
    return 0;
}

int SecureFBlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int SecureFBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int SecureFBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{    
    return 0;
}

app_data_t *SecureFBlockDevice::_query_app_info(mbed::bd_addr_t addr, int app_id)
{    
    uint8_t zone_id = (uint8_t)(addr / SecureFBlockDevice::secure_zone_size());
    
    for (int i = 0; i < _secureflash.app_info.num; i++) {        
        if ((_secureflash.app_info.app_data[i].app_id == app_id) &&
            (_secureflash.app_info.app_data[i].zone_id == zone_id)) {
            return &(_secureflash.app_info.app_data[i]);
        }
    }    
    return NULL;
}

int SecureFBlockDevice::secure_read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size, int app_id)
{
    int status = SECUREF_BD_ERROR_OK;
    app_data_t *app_data;
    uint32_t session_key_id, actual_read_size, offset, chunk, read_size = get_read_size();    

    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }    
    if (((uint32_t)(addr + size)) > SecureFBlockDevice::size()) {
        return SECUREF_BD_ERROR_INVALID_PGM_PARAMS;
    }
    _mutex->lock();
    app_data = _query_app_info(addr, app_id);
    if (NULL == app_data) {
        status = SECUREF_BD_ERROR_ILLEGAL_ACCESS;
        goto secure_read_exit_point;
    }    
    status = jedec_create_session(app_data->key_id, 0, &session_key_id);
    if (JEDEC_ERROR_NONE != status) {
        status = SECUREF_BD_ERROR_CREATE_SESSION;
        goto secure_read_exit_point;
    }
    while (size > 0) {
        offset = addr % read_size;
        chunk = (offset + size < read_size) ? size : (read_size - offset);
        status = jedec_secure_read(addr, (uint8_t *)buffer, chunk, session_key_id, &actual_read_size);
        if (JEDEC_ERROR_NONE != status) {
            status = SECUREF_BD_ERROR_SECURE_READ;
            goto secure_read_exit_point;
        }
        size -= chunk;
        addr += chunk;
        buffer += chunk;
    }
    status = jedec_terminate_session(session_key_id);
    if (JEDEC_ERROR_NONE != status) {
        status = SECUREF_BD_ERROR_TERMINATE_SESSION;
        goto secure_read_exit_point;
    }
secure_read_exit_point:
    _mutex->unlock();
    return status;
}
int SecureFBlockDevice::secure_program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size, int app_id)
{
    int status = SECUREF_BD_ERROR_OK;
    uint32_t session_key_id;
    app_data_t *app_data;
    uint32_t actual_program_size, chunk, offset, pgm_size = get_program_size();
    
    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }
    
    if (((uint32_t)(addr + size)) > SecureFBlockDevice::size()) {
        return SECUREF_BD_ERROR_INVALID_PGM_PARAMS;
    }
    _mutex->lock();
    app_data = _query_app_info(addr, app_id);
    if (NULL == app_data) {
        status = SECUREF_BD_ERROR_ILLEGAL_ACCESS;
        goto secure_program_exit_point;
    }    
    status = jedec_create_session(app_data->key_id, 0, &session_key_id);
    if (JEDEC_ERROR_NONE != status) {
        status = SECUREF_BD_ERROR_CREATE_SESSION;
        goto secure_program_exit_point;
    }
    while (size > 0) {
        offset = addr % pgm_size;
        chunk = (offset + size < pgm_size) ? size : (pgm_size - offset);
        status = jedec_secure_program(addr, (uint8_t *)buffer, chunk, session_key_id, &actual_program_size);
        if (JEDEC_ERROR_NONE != status) {
            status = SECUREF_BD_ERROR_SECURE_PROGRAM;
            goto secure_program_exit_point;
        }
        size -= chunk;
        addr += chunk;
        buffer += chunk;
    }
    status = jedec_terminate_session(session_key_id);
    if (JEDEC_ERROR_NONE != status) {
        status = SECUREF_BD_ERROR_TERMINATE_SESSION;
        goto secure_program_exit_point;        
    }
secure_program_exit_point:
    _mutex->unlock();
    return status;
}

int SecureFBlockDevice::secure_erase(mbed::bd_addr_t addr, mbed::bd_size_t size, int app_id)
{
    int status = SECUREF_BD_ERROR_OK;    
    app_data_t *app_data;
    uint32_t session_key_id, ers_size = get_erase_size();

    if (!_is_initialized) {
        return BD_ERROR_DEVICE_ERROR;
    }    
    if (((uint32_t)(addr + size)) > SecureFBlockDevice::size()) {
        return SECUREF_BD_ERROR_INVALID_ERASE_PARAMS;
    }
    if ((addr % ers_size) || (size % ers_size)) {
        return SECUREF_BD_ERROR_INVALID_ERASE_PARAMS;
    }
    _mutex->lock();
    app_data = _query_app_info(addr, app_id);
    if (NULL == app_data) {
        status = SECUREF_BD_ERROR_ILLEGAL_ACCESS;
        goto secure_erase_exit_point;
    }    
    status = jedec_create_session(app_data->key_id, 0, &session_key_id);
    if (JEDEC_ERROR_NONE != status) {
        status = SECUREF_BD_ERROR_CREATE_SESSION;
        goto secure_erase_exit_point;
    }
    while (size > 0) {
        status = jedec_secure_erase((uint32_t)addr, ers_size, session_key_id);
        if (JEDEC_ERROR_NONE != status) {
            status = SECUREF_BD_ERROR_SECURE_ERASE;
            goto secure_erase_exit_point;
        }
        size -= ers_size;
        addr += ers_size;
    }
    status = jedec_terminate_session(session_key_id);
    if (JEDEC_ERROR_NONE != status) {
        status = SECUREF_BD_ERROR_TERMINATE_SESSION;
        goto secure_erase_exit_point;
    }
secure_erase_exit_point:
    _mutex->unlock();
    return status;
}

bd_size_t SecureFBlockDevice::get_read_size() const
{
    if (!_is_initialized) {
        return 0;
    }
    return _secureflash.flash_info.flash_profile->architecture.secure_read_size;
}

bd_size_t SecureFBlockDevice::get_program_size() const
{
    if (!_is_initialized) {
        return 0;
    }
    return _secureflash.flash_info.flash_profile->architecture.secure_program_size;
}

bd_size_t SecureFBlockDevice::get_erase_size() const
{
    if (!_is_initialized) {
        return 0;
    }
    return _secureflash.flash_info.flash_profile->architecture.regions_min_secure_erase_size;
}

// Find minimal erase size supported by the region to which the address belongs to
bd_size_t SecureFBlockDevice::get_erase_size(bd_addr_t addr) const
{
    if (!_is_initialized) {
        return 0;
    }
    return _secureflash.flash_info.flash_profile->architecture.regions_min_secure_erase_size;
}
bd_size_t SecureFBlockDevice::secure_zone_number() const
{
    if (!_is_initialized) {
        return 0;
    }
    return _secureflash.flash_info.flash_profile->architecture.secure_zone_number;    
}

bd_size_t SecureFBlockDevice::secure_zone_size() const
{
    if (!_is_initialized) {
        return 0;
    }
    return _secureflash.flash_info.flash_profile->architecture.secure_zone_size;
}

bd_size_t SecureFBlockDevice::size() const
{
    if (!_is_initialized) {
        return 0;
    }

    return _secureflash.flash_info.flash_profile->architecture.secure_zone_total_size;
}

int SecureFBlockDevice::get_erase_value() const
{
    return 0xFF;
}

const char *SecureFBlockDevice::get_type() const
{
    return "SECUREF";
}

/***************************************************/
/*********** SPI Driver API Functions **************/
/***************************************************/
securef_bd_error SecureFBlockDevice::_spi_set_frequency(int freq)
{
    _spi->frequency(freq);
    return SECUREF_BD_ERROR_OK;
}

/*********************************************************/
/********** SFDP Parsing and Detection Functions *********/
/*********************************************************/
int SecureFBlockDevice::_reset_flash_mem()
{
    // Perform Soft Reset of the Device prior to initialization
    int status = 0;
    uint8_t status_value[2] = {0};
    tr_info("_reset_flash_mem:");
    // Read the Status Register from device
    if (0 == spi_nor_read_sr(status_value, 1)) {    
        // store received values in status_value
        tr_debug("Reading Status Register Success: value = 0x%x", (int)status_value[0]);
    } else {
        tr_error("Reading Status Register failed");
        status = -1;
    }

    if (0 == status) {
        // Send Reset Enable
        if (0 == spi_nor_reset_enable()) {        
            // store received values in status_value
            tr_debug("Sending RSTEN Success");
        } else {
            tr_error("Sending RSTEN failed");
            status = -1;
        }

        if (0 == status) {
            // Send Reset
            if (0 == spi_nor_reset()) {            
                // store received values in status_value
                tr_debug("Sending RST Success");
            } else {
                tr_error("Sending RST failed");
                status = -1;
            }
            if (0 != spi_nor_polling_for_wr_ready()) {
                tr_error("Device not ready, write failed");
                status = -1;
            }
        }
    }

    return status;
}

int SecureFBlockDevice::_handle_vendor_quirks()
{
    uint8_t vendor_device_ids[4];
    size_t data_length = 3;

    // Read Manufacturer ID (1byte), and Device ID (2bytes)
    if (0 != spi_nor_read_id(vendor_device_ids, data_length)) {
        tr_error("Read Vendor ID Failed");
            return -1;
    }

    tr_debug("Vendor device ID = 0x%x 0x%x 0x%x", vendor_device_ids[0], vendor_device_ids[1], vendor_device_ids[2]);

    return 0;
}