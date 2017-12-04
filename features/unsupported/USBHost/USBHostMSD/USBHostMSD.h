/* mbed USBHost Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifndef USBHOSTMSD_H
#define USBHOSTMSD_H

#include "USBHostConf.h"

#if USBHOST_MSD

#include "USBHost.h"
#include "FATFileSystem.h"
#include "BlockDevice.h"

/**
 * A class to communicate a USB flash disk
 */
class USBHostMSD : public IUSBEnumerator, public BlockDevice
{
public:
    /**
     * Constructor
     *
     * @param rootdir mount name
     */
    USBHostMSD();

    /**
     * Check if a MSD device is connected
     *
     * @return true if a MSD device is connected
     */
    bool connected();

    /**
     * Try to connect to a MSD device
     *
     * @return true if connection was successful
     */
    bool connect();
    virtual int init();
    virtual int deinit()
    {
        return BD_ERROR_OK;
    };
    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size);
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size);
    virtual int erase(bd_addr_t addr, bd_size_t size);
    virtual bd_size_t get_read_size() const;
    virtual bd_size_t get_program_size() const;
    virtual bd_size_t get_erase_size() const;
    virtual bd_size_t size() const;



protected:
    //From IUSBEnumerator
    virtual void setVidPid(uint16_t vid, uint16_t pid);
    virtual bool parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol); //Must return true if the interface should be parsed
    virtual bool useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir); //Must return true if the endpoint will be used


private:
    USBHost * host;
    USBDeviceConnected * dev;
    bool dev_connected;
    USBEndpoint * bulk_in;
    USBEndpoint * bulk_out;
    uint8_t nb_ep;

    // Bulk-only CBW
    typedef struct {
        uint32_t Signature;
        uint32_t Tag;
        uint32_t DataLength;
        uint8_t  Flags;
        uint8_t  LUN;
        uint8_t  CBLength;
        uint8_t  CB[16];
    } PACKED CBW;

    // Bulk-only CSW
    typedef struct {
        uint32_t Signature;
        uint32_t Tag;
        uint32_t DataResidue;
        uint8_t  Status;
    } PACKED CSW;

    CBW cbw;
    CSW csw;

    int SCSITransfer(uint8_t * cmd, uint8_t cmd_len, int flags, uint8_t * data, uint32_t transfer_len);
    int testUnitReady();
    int readCapacity();
    int inquiry(uint8_t lun, uint8_t page_code);
    int SCSIRequestSense();
    int dataTransfer(uint8_t * buf, uint32_t block, uint8_t nbBlock, int direction);
    int checkResult(uint8_t res, USBEndpoint * ep);
    int getMaxLun();

    int blockSize;
    uint32_t blockCount;

    int msd_intf;
    bool msd_device_found;
    bool disk_init;

    void init_usb();

};

#endif

#endif
