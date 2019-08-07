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

#ifndef USBMSD_H
#define USBMSD_H

/* These headers are included for child class. */
#include "USBDescriptor.h"
#include "USBDevice_Types.h"
#include "platform/Callback.h"
#include "drivers/internal/PolledQueue.h"
#include "drivers/internal/Task.h"
#include "BlockDevice.h"
#include "Mutex.h"

#include "USBDevice.h"

/**
 * \defgroup drivers_USBMSD USBMSD class
 * \ingroup drivers-public-api-usb
 * @{
 */

/**
 * USBMSD class: generic class in order to use all kinds of blocks storage chip
 *
 * Introduction
 *
 * USBMSD implements the MSD protocol. It permits to access a block device (flash, SD Card,...)
 * from a computer over USB.
 *
 * @code
 * #include "mbed.h"
 * #include "SDBlockDevice.h"
 * #include "USBMSD.h"
 *
 * SDBlockDevice sd(PTE3, PTE1, PTE2, PTE4);
 * USBMSD usb(&sd);
 *
 * int main() {
 *
 *     while(true) {
 *         usb.process();
 *     }
 *
 *     return 0;
 * }
 * @endcode
 */
class USBMSD: public USBDevice {
public:

    /**
    * Constructor
    *
    * This creates a new USBMSD object with the given block device. Connect must be called
    * for the block device to connect.
    *
    * @param bd BlockDevice to mount as a USB drive
    * @param connect_blocking true to perform a blocking connect, false to start in a disconnected state
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    */
    USBMSD(BlockDevice *bd, bool connect_blocking = true, uint16_t vendor_id = 0x0703, uint16_t product_id = 0x0104, uint16_t product_release = 0x0001);

    /**
    * Fully featured constructor
    *
    * Construct this object with the supplied USBPhy and parameters. The user
    * this object is responsible for calling connect() or init().
    *
    * @note Derived classes must use this constructor and call init() or
    * connect() themselves. Derived classes should also call deinit() in
    * their destructor. This ensures that no interrupts can occur when the
    * object is partially constructed or destroyed.
    *
    * @param phy USB phy to use
    * @param bd BlockDevice to mount as a USB drive
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    */
    USBMSD(USBPhy *phy, BlockDevice *bd, uint16_t vendor_id, uint16_t product_id, uint16_t product_release);

    /**
     * Destroy this object
     *
     * Any classes which inherit from this class must call disconnect
     * before this destructor runs.
     */
    virtual ~USBMSD();

    /**
    * Connect the USB MSD device.
    *
    * @returns true if successful
    */
    bool connect();

    /**
    * Disconnect the USB MSD device.
    */
    void disconnect();

    /**
    * Perform USB processing
    */
    void process();

    /**
     * Called when USBMSD needs to perform processing
     *
     * @param cb Callback called when USBMSD needs process() to be called
     */
    void attach(mbed::Callback<void()> cb);

    /**
    * Check if MSD device was removed/unmounted on the host side.
    *
    * @returns true if device was removed/unmounted on the host side
    */
    bool media_removed();

protected:

    /*
    * read one or more blocks on a storage chip
    *
    * @param data pointer where will be stored read data
    * @param block starting block number
    * @param count number of blocks to read
    * @returns 0 if successful
    */
    virtual int disk_read(uint8_t *data, uint64_t block, uint8_t count);

    /*
    * write one or more blocks on a storage chip
    *
    * @param data data to write
    * @param block starting block number
    * @param count number of blocks to write
    * @returns 0 if successful
    */
    virtual int disk_write(const uint8_t *data, uint64_t block, uint8_t count);

    /*
    * Disk initilization
    */
    virtual int disk_initialize();

    /*
    * Return the number of blocks
    *
    * @returns number of blocks
    */
    virtual uint64_t disk_sectors();

    /*
    * Return memory size
    *
    * @returns memory size
    */
    virtual uint64_t disk_size();

    /*
    * To check the status of the storage chip
    *
    * @returns status: 0: OK, 1: disk not initialized, 2: no medium in the drive, 4: write protected
    */
    virtual int disk_status();

private:

    // MSC Bulk-only Stage
    enum Stage {
        READ_CBW,     // wait a CBW
        ERROR,        // error
        PROCESS_CBW,  // process a CBW request
        SEND_CSW,     // send a CSW
    };

    // Bulk-only CBW
    typedef MBED_PACKED(struct)
    {
        uint32_t Signature;
        uint32_t Tag;
        uint32_t DataLength;
        uint8_t  Flags;
        uint8_t  LUN;
        uint8_t  CBLength;
        uint8_t  CB[16];
    } CBW;

    // Bulk-only CSW
    typedef MBED_PACKED(struct)
    {
        uint32_t Signature;
        uint32_t Tag;
        uint32_t DataResidue;
        uint8_t  Status;
    } CSW;

    // If this class has been initialized
    bool _initialized;

    // If msd device has been unmounted by host
    volatile bool _media_removed;

    //state of the bulk-only state machine
    Stage _stage;

    // current CBW
    CBW _cbw;

    // CSW which will be sent
    CSW _csw;

    // addr where will be read or written data
    uint32_t _addr;

    // length of a reading or writing
    uint32_t _length;

    // memory OK (after a memoryVerify)
    bool _mem_ok;

    // cache in RAM before writing in memory. Useful also to read a block.
    uint8_t *_page;

    int _block_size;
    uint64_t _memory_size;
    uint64_t _block_count;

    // endpoints
    usb_ep_t _bulk_in;
    usb_ep_t _bulk_out;
    uint8_t _bulk_in_buf[64];
    uint8_t _bulk_out_buf[64];
    bool _out_ready;
    bool _in_ready;
    uint32_t _bulk_out_size;

    // Interrupt to thread deferral
    events::PolledQueue _queue;
    events::Task<void()> _in_task;
    events::Task<void()> _out_task;
    events::Task<void()> _reset_task;
    events::Task<void(const setup_packet_t *)> _control_task;
    events::Task<void()> _configure_task;

    BlockDevice *_bd;
    rtos::Mutex _mutex_init;
    rtos::Mutex _mutex;

    // space for config descriptor
    uint8_t _configuration_descriptor[32];

    virtual const uint8_t *string_iproduct_desc();
    virtual const uint8_t *string_iinterface_desc();
    virtual const uint8_t *configuration_desc(uint8_t index);
    virtual void callback_set_configuration(uint8_t configuration);
    virtual void callback_set_interface(uint16_t interface, uint8_t alternate);
    virtual void callback_state_change(DeviceState new_state);
    virtual void callback_request(const setup_packet_t *setup);
    virtual void callback_request_xfer_done(const setup_packet_t *setup, bool aborted);

    void _isr_out();
    void _isr_in();

    void _out();
    void _in();
    void _reset();
    void _control(const setup_packet_t *request);
    void _configure();

    void _init();
    void _process();
    void _write_next(uint8_t *data, uint32_t size);
    void _read_next();

    void CBWDecode(uint8_t *buf, uint16_t size);
    void sendCSW(void);
    bool inquiryRequest(void);
    bool write(uint8_t *buf, uint16_t size);
    bool readFormatCapacity();
    bool readCapacity(void);
    bool infoTransfer(void);
    void memoryRead(void);
    bool modeSense6(void);
    void testUnitReady(void);
    bool requestSense(void);
    void memoryVerify(uint8_t *buf, uint16_t size);
    void memoryWrite(uint8_t *buf, uint16_t size);
    void msd_reset();
    void fail();
};

/** @}*/

#endif
