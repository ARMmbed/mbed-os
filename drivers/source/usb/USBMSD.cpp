/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "USBMSD.h"
#include "EndpointResolver.h"
#include "usb_phy_api.h"

#define DISK_OK         0x00
#define NO_INIT         0x01
#define NO_DISK         0x02
#define WRITE_PROTECT   0x04

#define CBW_Signature   0x43425355
#define CSW_Signature   0x53425355

// SCSI Commands
#define TEST_UNIT_READY            0x00
#define REQUEST_SENSE              0x03
#define FORMAT_UNIT                0x04
#define INQUIRY                    0x12
#define MODE_SELECT6               0x15
#define MODE_SENSE6                0x1A
#define START_STOP_UNIT            0x1B
#define MEDIA_REMOVAL              0x1E
#define READ_FORMAT_CAPACITIES     0x23
#define READ_CAPACITY              0x25
#define READ10                     0x28
#define WRITE10                    0x2A
#define VERIFY10                   0x2F
#define READ12                     0xA8
#define WRITE12                    0xAA
#define MODE_SELECT10              0x55
#define MODE_SENSE10               0x5A

// MSC class specific requests
#define MSC_REQUEST_RESET          0xFF
#define MSC_REQUEST_GET_MAX_LUN    0xFE

#define DEFAULT_CONFIGURATION (1)

// max packet size
#define MAX_PACKET  64

// CSW Status
enum Status {
    CSW_PASSED,
    CSW_FAILED,
    CSW_ERROR,
};

USBMSD::USBMSD(BlockDevice *bd, bool connect_blocking, uint16_t vendor_id, uint16_t product_id, uint16_t product_release)
    : USBDevice(get_usb_phy(), vendor_id, product_id, product_release),
      _initialized(false), _media_removed(false),
      _addr(0), _length(0), _mem_ok(false), _block_size(0), _memory_size(0), _block_count(0),
      _out_ready(false), _in_ready(false), _bulk_out_size(0),
      _in_task(&_queue), _out_task(&_queue), _reset_task(&_queue), _control_task(&_queue),
      _configure_task(&_queue), _bd(bd)
{
    _init();
    if (connect_blocking) {
        connect();
    } else {
        init();
    }
}

USBMSD::USBMSD(USBPhy *phy, BlockDevice *bd, uint16_t vendor_id, uint16_t product_id, uint16_t product_release)
    : USBDevice(phy, vendor_id, product_id, product_release),
      _initialized(false), _media_removed(false),
      _addr(0), _length(0), _mem_ok(false), _block_size(0), _memory_size(0), _block_count(0),
      _out_ready(false), _in_ready(false), _bulk_out_size(0),
      _in_task(&_queue), _out_task(&_queue), _reset_task(&_queue), _control_task(&_queue),
      _configure_task(&_queue), _bd(bd)
{
    _init();
}


void USBMSD::_init()
{
    _bd->init();

    _in_task = mbed::callback(this, &USBMSD::_in);
    _out_task = mbed::callback(this, &USBMSD::_out);
    _reset_task = mbed::callback(this, &USBMSD::_reset);
    _control_task = mbed::callback(this, &USBMSD::_control);
    _configure_task = mbed::callback(this, &USBMSD::_configure);

    EndpointResolver resolver(endpoint_table());

    resolver.endpoint_ctrl(64);
    _bulk_in = resolver.endpoint_in(USB_EP_TYPE_BULK, MAX_PACKET);
    _bulk_out = resolver.endpoint_out(USB_EP_TYPE_BULK, MAX_PACKET);
    MBED_ASSERT(resolver.valid());

    _stage = READ_CBW;
    memset((void *)&_cbw, 0, sizeof(CBW));
    memset((void *)&_csw, 0, sizeof(CSW));
    _page = NULL;
}

USBMSD::~USBMSD()
{
    disconnect();
    _bd->deinit();
    deinit();
}

bool USBMSD::connect()
{
    _mutex_init.lock();
    _mutex.lock();

    // already initialized
    if (_initialized) {
        _mutex.unlock();
        _mutex_init.unlock();
        return false;
    }

    //disk initialization
    if (disk_status() & NO_INIT) {
        if (disk_initialize()) {
            _mutex.unlock();
            _mutex_init.unlock();
            return false;
        }
    }

    // get number of blocks
    _block_count = disk_sectors();

    // get memory size
    _memory_size = disk_size();

    if (_block_count > 0) {
        _block_size = _memory_size / _block_count;
        if (_block_size != 0) {
            free(_page);
            _page = (uint8_t *)malloc(_block_size * sizeof(uint8_t));
            if (_page == NULL) {
                _mutex.unlock();
                _mutex_init.unlock();
                return false;
            }
        }
    } else {
        _mutex.unlock();
        _mutex_init.unlock();
        return false;
    }

    //connect the device
    USBDevice::connect();
    _initialized = true;
    _media_removed = false;
    _mutex.unlock();
    _mutex_init.unlock();
    return true;
}

void USBMSD::disconnect()
{
    _mutex_init.lock();
    _mutex.lock();

    USBDevice::disconnect();
    _initialized = false;

    _in_task.cancel();
    _out_task.cancel();
    _reset_task.cancel();
    _control_task.cancel();
    _configure_task.cancel();

    _mutex.unlock();

    // object mutex must be unlocked for waiting
    _in_task.wait();
    _out_task.wait();
    _reset_task.wait();
    _control_task.wait();
    _configure_task.wait();

    _mutex.lock();

    //De-allocate MSD page size:
    free(_page);
    _page = NULL;

    _mutex.unlock();
    _mutex_init.unlock();
}

void USBMSD::process()
{
    _queue.dispatch();
}

void USBMSD::attach(mbed::Callback<void()> cb)
{
    lock();

    _queue.attach(cb);

    unlock();
}

bool USBMSD::media_removed()
{
    return _media_removed;
}

int USBMSD::disk_read(uint8_t *data, uint64_t block, uint8_t count)
{
    bd_addr_t addr =  block * _bd->get_erase_size();
    bd_size_t size = count * _bd->get_erase_size();
    return _bd->read(data, addr, size);
}

int USBMSD::disk_write(const uint8_t *data, uint64_t block, uint8_t count)
{
    bd_addr_t addr =  block * _bd->get_erase_size();
    bd_size_t size = count * _bd->get_erase_size();
    int ret = _bd->erase(addr, size);
    if (ret != 0) {
        return ret;
    }

    return _bd->program(data, addr, size);
}

int USBMSD::disk_initialize()
{
    return 0;
}

uint64_t USBMSD::disk_sectors()
{
    return _bd->size() / _bd->get_erase_size();
}

uint64_t USBMSD::disk_size()
{
    return _bd->size();
}


int USBMSD::disk_status()
{
    return 0;
}

void USBMSD::_isr_out()
{
    _out_task.call();
}

void USBMSD::_isr_in()
{
    _in_task.call();
}

void USBMSD::callback_state_change(DeviceState new_state)
{
    // called in ISR context

    if (new_state != Configured) {
        _reset_task.cancel();
        _reset_task.call();
    }
}

void USBMSD::callback_request(const setup_packet_t *setup)
{
    // called in ISR context

    if (setup->bmRequestType.Type == CLASS_TYPE) {
        _control_task.call(setup);
    } else {
        complete_request(PassThrough, NULL, 0);
    }
}

void USBMSD::callback_request_xfer_done(const setup_packet_t *setup, bool aborted)
{
    // called in ISR context

    bool success = setup->bRequest == MSC_REQUEST_GET_MAX_LUN;
    complete_request_xfer_done(success);
}

void USBMSD::callback_set_configuration(uint8_t configuration)
{
    // called in ISR context

    if (configuration != DEFAULT_CONFIGURATION) {
        complete_set_configuration(false);
        return;
    }
    _configure_task.call();
}

void USBMSD::callback_set_interface(uint16_t interface, uint8_t alternate)
{
    // called in ISR context

    bool success = (interface == 0) && (alternate == 0);
    complete_set_interface(success);
}


const uint8_t *USBMSD::string_iinterface_desc()
{
    static const uint8_t string_iinterface_descriptor[] = {
        0x08,                           //bLength
        STRING_DESCRIPTOR,              //bDescriptorType 0x03
        'M', 0, 'S', 0, 'D', 0          //bString iInterface - MSD
    };
    return string_iinterface_descriptor;
}

const uint8_t *USBMSD::string_iproduct_desc()
{
    static const uint8_t string_iproduct_descriptor[] = {
        0x12,                                           //bLength
        STRING_DESCRIPTOR,                              //bDescriptorType 0x03
        'M', 0, 'b', 0, 'e', 0, 'd', 0, ' ', 0, 'M', 0, 'S', 0, 'D', 0 //bString iProduct - Mbed Audio
    };
    return string_iproduct_descriptor;
}


const uint8_t *USBMSD::configuration_desc(uint8_t index)
{
    if (index != 0) {
        return NULL;
    }

    uint8_t config_descriptor_temp[] = {

        // Configuration 1
        9,      // bLength
        2,      // bDescriptorType
        LSB(9 + 9 + 7 + 7), // wTotalLength
        MSB(9 + 9 + 7 + 7),
        0x01,   // bNumInterfaces
        0x01,   // bConfigurationValue: 0x01 is used to select this configuration
        0x00,   // iConfiguration: no string to describe this configuration
        0xC0,   // bmAttributes
        100,    // bMaxPower, device power consumption is 100 mA

        // Interface 0, Alternate Setting 0, MSC Class
        9,      // bLength
        4,      // bDescriptorType
        0x00,   // bInterfaceNumber
        0x00,   // bAlternateSetting
        0x02,   // bNumEndpoints
        0x08,   // bInterfaceClass
        0x06,   // bInterfaceSubClass
        0x50,   // bInterfaceProtocol
        0x04,   // iInterface

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                          // bLength
        5,                          // bDescriptorType
        _bulk_in,                   // bEndpointAddress
        0x02,                       // bmAttributes (0x02=bulk)
        LSB(MAX_PACKET),            // wMaxPacketSize (LSB)
        MSB(MAX_PACKET),            // wMaxPacketSize (MSB)
        0,                          // bInterval

        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                          // bLength
        5,                          // bDescriptorType
        _bulk_out,                  // bEndpointAddress
        0x02,                       // bmAttributes (0x02=bulk)
        LSB(MAX_PACKET),            // wMaxPacketSize (LSB)
        MSB(MAX_PACKET),            // wMaxPacketSize (MSB)
        0                           // bInterval
    };
    MBED_ASSERT(sizeof(config_descriptor_temp) == sizeof(_configuration_descriptor));
    memcpy(_configuration_descriptor, config_descriptor_temp, sizeof(_configuration_descriptor));
    return _configuration_descriptor;
}

void USBMSD::_out()
{
    _mutex.lock();

    _bulk_out_size = read_finish(_bulk_out);
    _out_ready = true;
    _process();

    _mutex.unlock();
}

void USBMSD::_in()
{
    _mutex.lock();

    write_finish(_bulk_in);
    _in_ready = true;
    _process();

    _mutex.unlock();
}

void USBMSD::_reset()
{
    _mutex.lock();

    msd_reset();

    _mutex.unlock();
}

void USBMSD::_control(const setup_packet_t *setup)
{
    _mutex.lock();

    static const uint8_t maxLUN[1] = {0};

    RequestResult result = PassThrough;
    uint8_t *data = NULL;
    uint32_t size = 0;

    if (setup->bmRequestType.Type == CLASS_TYPE) {
        switch (setup->bRequest) {
            case MSC_REQUEST_RESET:
                result = Success;
                msd_reset();
                break;
            case MSC_REQUEST_GET_MAX_LUN:
                result = Send;
                data = (uint8_t *)maxLUN;
                size = 1;
                break;
            default:
                break;
        }
    }

    complete_request(result, data, size);

    _mutex.unlock();
}

void USBMSD::_configure()
{
    _mutex.lock();

    // Configure endpoints > 0
    endpoint_add(_bulk_in, MAX_PACKET, USB_EP_TYPE_BULK, &USBMSD::_isr_in);
    endpoint_add(_bulk_out, MAX_PACKET, USB_EP_TYPE_BULK, &USBMSD::_isr_out);
    MBED_ASSERT(sizeof(_bulk_out_buf) == MAX_PACKET);
    MBED_ASSERT(sizeof(_bulk_in_buf) == MAX_PACKET);

    _out_ready = false;
    _in_ready = true;

    //activate readings
    read_start(_bulk_out, _bulk_out_buf, sizeof(_bulk_out_buf));
    complete_set_configuration(true);

    _mutex.unlock();
}

void USBMSD::_process()
{
    // Mutex must be locked by caller

    switch (_stage) {
        // the device has to decode the CBW received
        case READ_CBW:
            if (!_out_ready) {
                break;
            }
            CBWDecode(_bulk_out_buf, _bulk_out_size);
            _read_next();
            break;


        case PROCESS_CBW:
            switch (_cbw.CB[0]) {
                // the device has to receive data from the host
                case WRITE10:
                case WRITE12:
                    if (!_out_ready) {
                        break;
                    }
                    memoryWrite(_bulk_out_buf, _bulk_out_size);
                    _read_next();
                    break;
                case VERIFY10:
                    if (!_out_ready) {
                        break;
                    }
                    memoryVerify(_bulk_out_buf, _bulk_out_size);
                    _read_next();
                    break;
                // the device has to send data to the host
                case READ10:
                case READ12:
                    if (!_in_ready) {
                        break;
                    }
                    memoryRead();
                    break;
            }
            break;

        //the device has to send a CSW
        case SEND_CSW:
            if (!_in_ready) {
                break;
            }
            sendCSW();
            break;

        // an error has occurred: stall endpoint and send CSW
        default:
            endpoint_stall(_bulk_out);
            endpoint_stall(_bulk_in);
            _csw.Status = CSW_ERROR;
            sendCSW();
            break;
    }
}

void USBMSD::_write_next(uint8_t *data, uint32_t size)
{
    lock();

    MBED_ASSERT(size <= MAX_PACKET);
    MBED_ASSERT(_in_ready);
    uint32_t send_size = MAX_PACKET > size ? size : MAX_PACKET;
    memcpy(_bulk_in_buf, data, send_size);
    write_start(_bulk_in, _bulk_in_buf, send_size);
    _in_ready = false;

    unlock();
}

void USBMSD::_read_next()
{
    lock();

    MBED_ASSERT(_out_ready);
    read_start(_bulk_out, _bulk_out_buf, sizeof(_bulk_out_buf));
    _out_ready = false;

    unlock();
}

void USBMSD::memoryWrite(uint8_t *buf, uint16_t size)
{
    // Max sized packets are required to be sent until the transfer is complete
    MBED_ASSERT(_block_size % MAX_PACKET == 0);
    if ((size != MAX_PACKET) && (size != 0)) {
        _stage = ERROR;
        endpoint_stall(_bulk_out);
        return;
    }

    if ((_addr + size) > _memory_size) {
        size = _memory_size - _addr;
        _stage = ERROR;
        endpoint_stall(_bulk_out);
    }

    // we fill an array in RAM of 1 block before writing it in memory
    for (int i = 0; i < size; i++) {
        _page[_addr % _block_size + i] = buf[i];
    }

    // if the array is filled, write it in memory
    if (!((_addr + size) % _block_size)) {
        if (!(disk_status() & WRITE_PROTECT)) {
            disk_write(_page, _addr / _block_size, 1);
        }
    }

    _addr += size;
    _length -= size;
    _csw.DataResidue -= size;

    if ((!_length) || (_stage != PROCESS_CBW)) {
        _csw.Status = (_stage == ERROR) ? CSW_FAILED : CSW_PASSED;
        sendCSW();
    }
}

void USBMSD::memoryVerify(uint8_t *buf, uint16_t size)
{
    uint32_t n;

    if ((_addr + size) > _memory_size) {
        size = _memory_size - _addr;
        _stage = ERROR;
        endpoint_stall(_bulk_out);
    }

    // beginning of a new block -> load a whole block in RAM
    if (!(_addr % _block_size)) {
        disk_read(_page, _addr / _block_size, 1);
    }

    // info are in RAM -> no need to re-read memory
    for (n = 0; n < size; n++) {
        if (_page[_addr % _block_size + n] != buf[n]) {
            _mem_ok = false;
            break;
        }
    }

    _addr += size;
    _length -= size;
    _csw.DataResidue -= size;

    if (!_length || (_stage != PROCESS_CBW)) {
        _csw.Status = (_mem_ok && (_stage == PROCESS_CBW)) ? CSW_PASSED : CSW_FAILED;
        sendCSW();
    }
}


bool USBMSD::inquiryRequest(void)
{
    uint8_t inquiry[] = { 0x00, 0x80, 0x00, 0x01,
                          36 - 4, 0x80, 0x00, 0x00,
                          'M', 'B', 'E', 'D', '.', 'O', 'R', 'G',
                          'M', 'B', 'E', 'D', ' ', 'U', 'S', 'B', ' ', 'D', 'I', 'S', 'K', ' ', ' ', ' ',
                          '1', '.', '0', ' ',
                        };
    if (!write(inquiry, sizeof(inquiry))) {
        return false;
    }
    return true;
}


bool USBMSD::readFormatCapacity()
{
    uint8_t capacity[] = { 0x00, 0x00, 0x00, 0x08,
                           (uint8_t)((_block_count >> 24) & 0xff),
                           (uint8_t)((_block_count >> 16) & 0xff),
                           (uint8_t)((_block_count >> 8) & 0xff),
                           (uint8_t)((_block_count >> 0) & 0xff),

                           0x02,
                           (uint8_t)((_block_size >> 16) & 0xff),
                           (uint8_t)((_block_size >> 8) & 0xff),
                           (uint8_t)((_block_size >> 0) & 0xff),
                         };
    if (!write(capacity, sizeof(capacity))) {
        return false;
    }
    return true;
}


bool USBMSD::readCapacity(void)
{
    uint8_t capacity[] = {
        (uint8_t)(((_block_count - 1) >> 24) & 0xff),
        (uint8_t)(((_block_count - 1) >> 16) & 0xff),
        (uint8_t)(((_block_count - 1) >> 8) & 0xff),
        (uint8_t)(((_block_count - 1) >> 0) & 0xff),

        (uint8_t)((_block_size >> 24) & 0xff),
        (uint8_t)((_block_size >> 16) & 0xff),
        (uint8_t)((_block_size >> 8) & 0xff),
        (uint8_t)((_block_size >> 0) & 0xff),
    };
    if (!write(capacity, sizeof(capacity))) {
        return false;
    }
    return true;
}

bool USBMSD::write(uint8_t *buf, uint16_t size)
{

    if (size >= _cbw.DataLength) {
        size = _cbw.DataLength;
    }
    _stage = SEND_CSW;

    _write_next(buf, size);

    _csw.DataResidue -= size;
    _csw.Status = CSW_PASSED;
    return true;
}


bool USBMSD::modeSense6(void)
{
    uint8_t sense6[] = { 0x03, 0x00, 0x00, 0x00 };
    if (!write(sense6, sizeof(sense6))) {
        return false;
    }
    return true;
}

bool USBMSD::modeSense10(void)
{
    uint8_t sense10[] = { 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    if (!write(sense10, sizeof(sense10))) {
        return false;
    }
    return true;
}

void USBMSD::sendCSW()
{
    _csw.Signature = CSW_Signature;
    _write_next((uint8_t *)&_csw, sizeof(CSW));
    _stage = READ_CBW;
}

bool USBMSD::requestSense(void)
{
    uint8_t request_sense[] = {
        0x70,
        0x00,
        0x05,   // Sense Key: illegal request
        0x00,
        0x00,
        0x00,
        0x00,
        0x0A,
        0x00,
        0x00,
        0x00,
        0x00,
        0x30,
        0x01,
        0x00,
        0x00,
        0x00,
        0x00,
    };

    if (!write(request_sense, sizeof(request_sense))) {
        return false;
    }

    return true;
}

void USBMSD::fail()
{
    _csw.Status = CSW_FAILED;
    sendCSW();
}


void USBMSD::CBWDecode(uint8_t *buf, uint16_t size)
{
    if (size == sizeof(_cbw)) {
        memcpy((uint8_t *)&_cbw, buf, size);
        if (_cbw.Signature == CBW_Signature) {
            _csw.Tag = _cbw.Tag;
            _csw.DataResidue = _cbw.DataLength;
            if ((_cbw.CBLength <  1) || (_cbw.CBLength > 16)) {
                fail();
            } else {
                switch (_cbw.CB[0]) {
                    case TEST_UNIT_READY:
                        testUnitReady();
                        break;
                    case REQUEST_SENSE:
                        requestSense();
                        break;
                    case INQUIRY:
                        inquiryRequest();
                        break;
                    case MODE_SENSE6:
                        modeSense6();
                        break;
                    case READ_FORMAT_CAPACITIES:
                        readFormatCapacity();
                        break;
                    case READ_CAPACITY:
                        readCapacity();
                        break;
                    case READ10:
                    case READ12:
                        if (infoTransfer()) {
                            if ((_cbw.Flags & 0x80)) {
                                _stage = PROCESS_CBW;
                                memoryRead();
                            } else {
                                endpoint_stall(_bulk_out);
                                _csw.Status = CSW_ERROR;
                                sendCSW();
                            }
                        }
                        break;
                    case WRITE10:
                    case WRITE12:
                        if (infoTransfer()) {
                            if (!(_cbw.Flags & 0x80)) {
                                _stage = PROCESS_CBW;
                            } else {
                                endpoint_stall(_bulk_in);
                                _csw.Status = CSW_ERROR;
                                sendCSW();
                            }
                        }
                        break;
                    case VERIFY10:
                        if (!(_cbw.CB[1] & 0x02)) {
                            _csw.Status = CSW_PASSED;
                            sendCSW();
                            break;
                        }
                        if (infoTransfer()) {
                            if (!(_cbw.Flags & 0x80)) {
                                _stage = PROCESS_CBW;
                                _mem_ok = true;
                            } else {
                                endpoint_stall(_bulk_in);
                                _csw.Status = CSW_ERROR;
                                sendCSW();
                            }
                        }
                        break;
                    case MEDIA_REMOVAL:
                        _csw.Status = CSW_PASSED;
                        sendCSW();
                        _media_removed = true;
                        break;
                    case MODE_SENSE10:
                        modeSense10();
                        break;
                    default:
                        fail();
                        break;
                }
            }
        }
    }
}

void USBMSD::testUnitReady(void)
{

    if (_cbw.DataLength != 0) {
        if ((_cbw.Flags & 0x80) != 0) {
            endpoint_stall(_bulk_in);
        } else {
            endpoint_stall(_bulk_out);
        }
    }

    _csw.Status = CSW_PASSED;
    sendCSW();
}


void USBMSD::memoryRead(void)
{
    uint32_t n;

    n = (_length > MAX_PACKET) ? MAX_PACKET : _length;

    if (_addr > (_memory_size - n)) {
        n = _addr < _memory_size ? _memory_size - _addr : 0;
        _stage = ERROR;
    }

    if (n > 0) {
        // we read an entire block
        if (!(_addr % _block_size)) {
            disk_read(_page, _addr / _block_size, 1);
        }

        // write data which are in RAM
        _write_next(&_page[_addr % _block_size], MAX_PACKET);

        _addr += n;
        _length -= n;

        _csw.DataResidue -= n;
    }

    if (!_length || (_stage != PROCESS_CBW)) {
        _csw.Status = (_stage == PROCESS_CBW) ? CSW_PASSED : CSW_FAILED;
        _stage = (_stage == PROCESS_CBW) ? SEND_CSW : _stage;
    }
}


bool USBMSD::infoTransfer(void)
{
    uint32_t addr_block;

    // Logical Block Address of First Block
    addr_block = (_cbw.CB[2] << 24) | (_cbw.CB[3] << 16) | (_cbw.CB[4] <<  8) | (_cbw.CB[5] <<  0);

    _addr = addr_block * _block_size;

    if ((addr_block >= _block_count) || (_addr >= _memory_size)) {
        _csw.Status = CSW_FAILED;
        sendCSW();
        return false;
    }

    uint32_t length_blocks = 0;
    // Number of Blocks to transfer
    switch (_cbw.CB[0]) {
        case READ10:
        case WRITE10:
        case VERIFY10:
            length_blocks = (_cbw.CB[7] <<  8) | (_cbw.CB[8] <<  0);
            break;

        case READ12:
        case WRITE12:
            length_blocks = (_cbw.CB[6] << 24) | (_cbw.CB[7] << 16) | (_cbw.CB[8] <<  8) | (_cbw.CB[9] <<  0);
            break;
    }

    _length = length_blocks * _block_size;

    if (!_cbw.DataLength || !length_blocks || (length_blocks > _block_count - addr_block) || (_length > _memory_size - _addr)) { // host requests no data or wrong length
        _csw.Status = CSW_FAILED;
        sendCSW();
        return false;
    }

    if (_cbw.DataLength != _length) {
        if ((_cbw.Flags & 0x80) != 0) {
            endpoint_stall(_bulk_in);
        } else {
            endpoint_stall(_bulk_out);
        }

        _csw.Status = CSW_FAILED;
        sendCSW();
        return false;
    }

    return true;
}

void USBMSD::msd_reset()
{
    _stage = READ_CBW;
}
