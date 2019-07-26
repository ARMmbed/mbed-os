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

#include "stdint.h"
#include "USBAudio.h"
#include "USBAudio_Types.h"
#include "EndpointResolver.h"
#include "usb_phy_api.h"

#define SAMPLE_SIZE                 2
#define XFER_FREQUENCY_HZ           1000
#define WRITE_READY_UNBLOCK         (1 << 0)
#define READ_READY_UNBLOCK          (1 << 1)

class USBAudio::AsyncWrite: public AsyncOp {
public:
    AsyncWrite(USBAudio *audio, uint8_t *buf, uint32_t size):
        audio(audio), tx_buf(buf), tx_size(size), result(false)
    {

    }

    virtual ~AsyncWrite()
    {

    }

    virtual bool process()
    {
        if (audio->_tx_state != Opened) {
            result = false;
            return true;
        }

        uint32_t actual_size = 0;
        audio->write_nb(tx_buf, tx_size, &actual_size);
        tx_size -= actual_size;
        tx_buf += actual_size;
        if (tx_size == 0) {
            result = true;
            return true;
        }

        return false;
    }

    USBAudio *audio;
    uint8_t *tx_buf;
    uint32_t tx_size;
    bool result;
};

class USBAudio::AsyncRead: public AsyncOp {
public:
    AsyncRead(USBAudio *audio, uint8_t *buf, uint32_t size, uint32_t *size_read, bool read_all)
        :   audio(audio), rx_buf(buf), rx_size(size), rx_actual(size_read), all(read_all), result(false)
    {

    }

    virtual ~AsyncRead()
    {

    }

    virtual bool process()
    {
        if (audio->_rx_state != Opened) {
            result = false;
            return true;
        }

        uint32_t actual_size = 0;
        audio->read_nb(rx_buf, rx_size, &actual_size);
        rx_buf += actual_size;
        *rx_actual += actual_size;
        rx_size -= actual_size;
        if ((!all && *rx_actual > 0) || (rx_size == 0)) {
            // Wake thread if request is done
            result = true;
            return true;
        }

        return false;
    }

    USBAudio *audio;
    uint8_t *rx_buf;
    uint32_t rx_size;
    uint32_t *rx_actual;
    bool all;
    bool result;
};

static void stub_volume()
{

}

static void stub_handler(USBAudio::AudioEvent event)
{
    (void)event;
}

USBAudio::USBAudio(bool connect, uint32_t frequency_rx, uint8_t channel_count_rx, uint32_t frequency_tx, uint8_t channel_count_tx, uint32_t buffer_ms, uint16_t vendor_id, uint16_t product_id, uint16_t product_release):
    USBDevice(get_usb_phy(), vendor_id, product_id, product_release)
{
    _init(frequency_rx, channel_count_rx, frequency_tx, channel_count_tx, buffer_ms);

    // connect or init device
    if (connect) {
        USBDevice::connect();
    } else {
        USBDevice::init();
    }
}

USBAudio::USBAudio(USBPhy *phy, uint32_t frequency_rx, uint8_t channel_count_rx, uint32_t frequency_tx, uint8_t channel_count_tx, uint32_t buffer_ms, uint16_t vendor_id, uint16_t product_id, uint16_t product_release):
    USBDevice(phy, vendor_id, product_id, product_release)
{
    _init(frequency_rx, channel_count_rx, frequency_tx, channel_count_tx, buffer_ms);
}

void USBAudio::_init(uint32_t frequency_rx, uint8_t channel_count_rx, uint32_t frequency_tx, uint8_t channel_count_tx, uint32_t buffer_ms)
{
    _connected = false;

    _volume = 0;
    _mute = 0;
    _vol_cur = 0x0080;
    _vol_min = 0x0000;
    _vol_max = 0x0100;
    _vol_res = 0x0004;

    _update_vol = mbed::callback(stub_volume);
    _tx_done = mbed::callback(stub_handler);
    _rx_done = mbed::callback(stub_handler);

    _rx_overflow = 0;
    _tx_underflow = 0;

    _tx_freq = frequency_tx;
    _rx_freq = frequency_rx;

    _tx_channel_count = channel_count_tx;
    _rx_channel_count = channel_count_rx;

    _tx_idle = true;
    _tx_frame_fract = 0;
    _tx_whole_frames_per_xfer = _tx_freq / XFER_FREQUENCY_HZ;
    _tx_fract_frames_per_xfer = _tx_freq % XFER_FREQUENCY_HZ;

    uint32_t max_frames = _tx_whole_frames_per_xfer + (_tx_fract_frames_per_xfer ? 1 : 0);
    _tx_packet_size_max = max_frames * SAMPLE_SIZE * _tx_channel_count;
    _rx_packet_size_max = (_rx_freq + 1000 - 1) / 1000 * _rx_channel_count * 2;

    _tx_packet_buf = new uint8_t[_tx_packet_size_max]();
    _rx_packet_buf = new uint8_t[_rx_packet_size_max]();

    _tx_queue.resize(buffer_ms * _tx_channel_count * SAMPLE_SIZE * _tx_freq / XFER_FREQUENCY_HZ);
    _rx_queue.resize(buffer_ms * _rx_channel_count * SAMPLE_SIZE * _rx_freq / XFER_FREQUENCY_HZ);

    _tx_state = Closed;
    _rx_state = Closed;

    EndpointResolver resolver(endpoint_table());
    resolver.endpoint_ctrl(64);
    _episo_out = resolver.endpoint_out(USB_EP_TYPE_ISO, _tx_packet_size_max);
    _episo_in = resolver.endpoint_in(USB_EP_TYPE_ISO, _rx_packet_size_max);
    MBED_ASSERT(resolver.valid());

    _channel_config_rx = (_rx_channel_count == 1) ? CHANNEL_M : CHANNEL_L + CHANNEL_R;
    _channel_config_tx = (_tx_channel_count == 1) ? CHANNEL_M : CHANNEL_L + CHANNEL_R;

    _build_configuration_desc();
}

USBAudio::~USBAudio()
{
    disconnect();
    deinit();

    delete[] _tx_packet_buf;
    delete[] _rx_packet_buf;
}

void USBAudio::connect()
{
    lock();

    if (!_connected) {
        USBDevice::connect();
        _connected = true;
        _receive_change(Closed);
        _send_change(Closed);
    }

    unlock();
}

void USBAudio::disconnect()
{
    lock();

    if (_connected) {
        _connected = false;
        USBDevice::disconnect();
        _receive_change(Powerdown);
        _send_change(Powerdown);
    }

    unlock();
}

bool USBAudio::read(uint8_t *buf, uint32_t size)
{
    lock();

    uint32_t actual;
    AsyncRead read(this, buf, size, &actual, true);
    _read_list.add(&read);

    unlock();

    read.wait(NULL);
    return read.result;
}

void USBAudio::read_nb(uint8_t *buf, uint32_t size, uint32_t *actual)
{

    lock();

    uint32_t available = _rx_queue.size();
    uint32_t copy_size = available > size ? size : available;
    _rx_queue.read(buf, copy_size);
    *actual = copy_size;

    unlock();
}

uint32_t USBAudio::read_overflows(bool clear)
{
    lock();

    uint32_t overflows = _rx_overflow;
    if (clear) {
        _rx_overflow = 0;
    }

    unlock();
    return overflows;
}


bool USBAudio::read_ready()
{
    lock();

    bool ready = _rx_state == Opened;

    unlock();

    return ready;
}

void USBAudio::read_wait_ready()
{
    _flags.wait_any(READ_READY_UNBLOCK, osWaitForever, false);
}

bool USBAudio::write(uint8_t *buf, uint32_t size)
{
    lock();

    AsyncWrite write(this, buf, size);
    _write_list.add(&write);

    unlock();

    write.wait(NULL);
    return write.result;
}

void USBAudio::write_nb(uint8_t *buf, uint32_t size, uint32_t *actual)
{
    lock();

    uint32_t available = _tx_queue.free();
    uint32_t copy_size = available > size ? size : available;
    _tx_queue.write(buf, copy_size);
    *actual = copy_size;
    _send_isr_start();

    unlock();
}

uint32_t USBAudio::write_underflows(bool clear)
{
    lock();

    uint32_t underflows = _tx_underflow;
    if (clear) {
        _tx_underflow = 0;
    }

    unlock();
    return underflows;
}

bool USBAudio::write_ready()
{
    lock();

    bool ready = _tx_state == Opened;

    unlock();

    return ready;
}

void USBAudio::write_wait_ready()
{
    _flags.wait_any(WRITE_READY_UNBLOCK, osWaitForever, false);
}


float USBAudio::get_volume()
{
    lock();

    float ret = _mute ? 0.0f : _volume;

    unlock();
    return ret;
}

void USBAudio::attach(mbed::Callback<void()> &cb)
{
    lock();

    _update_vol = cb;
    if (!_update_vol) {
        _update_vol = stub_volume;
    }

    unlock();
}

void USBAudio::attach_tx(mbed::Callback<void(AudioEvent)> &cb)
{
    lock();

    _tx_done = cb;
    if (!_tx_done) {
        _tx_done = mbed::callback(stub_handler);
    }

    unlock();
}

void USBAudio::attach_rx(mbed::Callback<void(AudioEvent)> &cb)
{
    lock();

    _rx_done = cb;
    if (!_rx_done) {
        _rx_done = mbed::callback(stub_handler);
    }

    unlock();
}

void USBAudio::callback_state_change(DeviceState new_state)
{
    assert_locked();

    if (_connected && (new_state != Configured)) {
        _receive_change(Closed);
        _send_change(Closed);
    }
}

void USBAudio::callback_request(const setup_packet_t *setup)
{
    assert_locked();

    RequestResult result = PassThrough;
    uint8_t *data = NULL;
    uint32_t size = 0;

    // Process class-specific requests
    if (setup->bmRequestType.Type == CLASS_TYPE) {

        // Feature Unit: Interface = 0, ID = 2
        if (setup->wIndex == 0x0200) {

            // Master Channel
            if ((setup->wValue & 0xff) == 0) {

                switch (setup->wValue >> 8) {
                    case MUTE_CONTROL:
                        switch (setup->bRequest) {
                            case REQUEST_GET_CUR:
                                size = 1;
                                data = &_mute;
                                result = Send;
                                break;

                            case REQUEST_SET_CUR:
                                size = 1;
                                data = _control_receive;
                                result = Receive;
                                break;
                            default:
                                break;
                        }
                        break;
                    case VOLUME_CONTROL:
                        switch (setup->bRequest) {
                            case REQUEST_GET_CUR:
                                size = 2;
                                data = (uint8_t *)&_vol_cur;
                                result = Send;
                                break;
                            case REQUEST_GET_MIN:
                                size = 2;
                                data = (uint8_t *)&_vol_min;
                                result = Send;
                                break;
                            case REQUEST_GET_MAX:
                                size = 2;
                                data = (uint8_t *)&_vol_max;
                                result = Send;
                                break;
                            case REQUEST_GET_RES:
                                size = 2;
                                data = (uint8_t *)&_vol_res;
                                result = Send;
                                break;

                            case REQUEST_SET_CUR:
                                size = 2;
                                data = _control_receive;
                                result = Receive;
                                break;
                            case REQUEST_SET_MIN:
                                size = 2;
                                data = _control_receive;
                                result = Receive;
                                break;
                            case REQUEST_SET_MAX:
                                size = 2;
                                data = _control_receive;
                                result = Receive;
                                break;
                            case REQUEST_SET_RES:
                                size = 2;
                                data = _control_receive;
                                result = Receive;
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    complete_request(result, data, size);
}

void USBAudio::callback_request_xfer_done(const setup_packet_t *setup, bool aborted)
{
    assert_locked();

    if (aborted) {
        complete_request_xfer_done(false);
        return;
    }

    if (setup->bmRequestType.dataTransferDirection == DEVICE_TO_HOST) {
        complete_request_xfer_done(true);
        return;
    }

    if ((setup->wLength == 1) || (setup->wLength == 2)) {
        uint16_t data = (_control_receive[0] << 0) | (_control_receive[1] << 8);
        data &= ((setup->wLength == 1) ? 0xFF : 0xFFFF);
        switch (setup->wValue >> 8) {
            case MUTE_CONTROL:
                switch (setup->bRequest) {
                    case REQUEST_SET_CUR:
                        _mute = data & 0xff;
                        _update_vol.call();
                        break;
                    default:
                        break;
                }
                break;
            case VOLUME_CONTROL:
                switch (setup->bRequest) {
                    case REQUEST_SET_CUR:
                        _vol_cur = data;
                        _volume = (float)_vol_cur / (float)_vol_max;
                        _update_vol.call();
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        complete_request_xfer_done(true);
        return;
    }

    complete_request_xfer_done(false);
}

void USBAudio::callback_set_configuration(uint8_t configuration)
{
    assert_locked();

    bool ret = false;
    if (configuration == DEFAULT_CONFIGURATION) {
        endpoint_remove_all();

        // Configure isochronous endpoint
        endpoint_add(_episo_out, _rx_packet_size_max, USB_EP_TYPE_ISO,  static_cast<ep_cb_t>(&USBAudio::_receive_isr));
        endpoint_add(_episo_in, _tx_packet_size_max, USB_EP_TYPE_ISO,  static_cast<ep_cb_t>(&USBAudio::_send_isr));

        // activate readings on this endpoint
        read_start(_episo_out, _rx_packet_buf, _rx_packet_size_max);
        ret = true;
    }
    complete_set_configuration(ret);
}

void USBAudio::callback_set_interface(uint16_t interface, uint8_t alternate)
{
    assert_locked();

    bool ret = false;
    if (interface == 0 && alternate == 0) {
        ret = true;
    }
    if (interface == 1 && (alternate == 0 || alternate == 1)) {
        _receive_change(alternate == 1 ? Opened : Closed);
        ret = true;
    }
    if (interface == 2 && (alternate == 0 || alternate == 1)) {
        _send_change(alternate == 1 ? Opened : Closed);
        ret = true;
    }
    complete_set_interface(ret);
}

const uint8_t *USBAudio::configuration_desc(uint8_t index)
{
    if (index != 0) {
        return NULL;
    }
    return _config_descriptor;
}

const uint8_t *USBAudio::string_iinterface_desc()
{
    static const uint8_t stringIinterfaceDescriptor[] = {
        0x0c,                           //bLength
        STRING_DESCRIPTOR,              //bDescriptorType 0x03
        'A', 0, 'u', 0, 'd', 0, 'i', 0, 'o', 0 //bString iInterface - Audio
    };
    return stringIinterfaceDescriptor;
}

const uint8_t *USBAudio::string_iproduct_desc()
{
    static const uint8_t stringIproductDescriptor[] = {
        0x16,                                                       //bLength
        STRING_DESCRIPTOR,                                          //bDescriptorType 0x03
        'M', 0, 'b', 0, 'e', 0, 'd', 0, ' ', 0, 'A', 0, 'u', 0, 'd', 0, 'i', 0, 'o', 0 //bString iProduct - Mbed Audio
    };
    return stringIproductDescriptor;
}

#define TOTAL_DESCRIPTOR_LENGTH ((1 * CONFIGURATION_DESCRIPTOR_LENGTH) \
                               + (5 * INTERFACE_DESCRIPTOR_LENGTH) \
                               + (1 * CONTROL_INTERFACE_DESCRIPTOR_LENGTH + 1) \
                               + (2 * INPUT_TERMINAL_DESCRIPTOR_LENGTH) \
                               + (1 * FEATURE_UNIT_DESCRIPTOR_LENGTH) \
                               + (2 * OUTPUT_TERMINAL_DESCRIPTOR_LENGTH) \
                               + (2 * STREAMING_INTERFACE_DESCRIPTOR_LENGTH) \
                               + (2 * FORMAT_TYPE_I_DESCRIPTOR_LENGTH) \
                               + (2 * (ENDPOINT_DESCRIPTOR_LENGTH + 2)) \
                               + (2 * STREAMING_ENDPOINT_DESCRIPTOR_LENGTH) )

#define TOTAL_CONTROL_INTF_LENGTH    (CONTROL_INTERFACE_DESCRIPTOR_LENGTH + 1 + \
                                      2*INPUT_TERMINAL_DESCRIPTOR_LENGTH     + \
                                      FEATURE_UNIT_DESCRIPTOR_LENGTH    + \
                                      2*OUTPUT_TERMINAL_DESCRIPTOR_LENGTH)

void USBAudio::_build_configuration_desc()
{
    uint8_t config_descriptor_temp[] = {
        // Configuration 1
        CONFIGURATION_DESCRIPTOR_LENGTH,        // bLength
        CONFIGURATION_DESCRIPTOR,               // bDescriptorType
        LSB(TOTAL_DESCRIPTOR_LENGTH),           // wTotalLength (LSB)
        MSB(TOTAL_DESCRIPTOR_LENGTH),           // wTotalLength (MSB)
        0x03,                                   // bNumInterfaces
        DEFAULT_CONFIGURATION,                  // bConfigurationValue
        0x00,                                   // iConfiguration
        0x80,                                   // bmAttributes
        50,                                     // bMaxPower

        // Interface 0, Alternate Setting 0, Audio Control
        INTERFACE_DESCRIPTOR_LENGTH,            // bLength
        INTERFACE_DESCRIPTOR,                   // bDescriptorType
        0x00,                                   // bInterfaceNumber
        0x00,                                   // bAlternateSetting
        0x00,                                   // bNumEndpoints
        AUDIO_CLASS,                            // bInterfaceClass
        SUBCLASS_AUDIOCONTROL,                  // bInterfaceSubClass
        0x00,                                   // bInterfaceProtocol
        0x00,                                   // iInterface


        // Audio Control Interface
        CONTROL_INTERFACE_DESCRIPTOR_LENGTH + 1,// bLength
        INTERFACE_DESCRIPTOR_TYPE,              // bDescriptorType
        CONTROL_HEADER,                         // bDescriptorSubtype
        LSB(0x0100),                            // bcdADC (LSB)
        MSB(0x0100),                            // bcdADC (MSB)
        LSB(TOTAL_CONTROL_INTF_LENGTH),         // wTotalLength
        MSB(TOTAL_CONTROL_INTF_LENGTH),         // wTotalLength
        0x02,                                   // bInCollection
        0x01,                                   // baInterfaceNr
        0x02,                                   // baInterfaceNr

        // Audio Input Terminal (Speaker)
        INPUT_TERMINAL_DESCRIPTOR_LENGTH,       // bLength
        INTERFACE_DESCRIPTOR_TYPE,              // bDescriptorType
        CONTROL_INPUT_TERMINAL,                 // bDescriptorSubtype
        0x01,                                   // bTerminalID
        LSB(TERMINAL_USB_STREAMING),            // wTerminalType
        MSB(TERMINAL_USB_STREAMING),            // wTerminalType
        0x00,                                   // bAssocTerminal
        _rx_channel_count,                      // bNrChannels
        (uint8_t)(LSB(_channel_config_rx)),             // wChannelConfig
        (uint8_t)(MSB(_channel_config_rx)),             // wChannelConfig
        0x00,                                   // iChannelNames
        0x00,                                   // iTerminal

        // Audio Feature Unit (Speaker)
        FEATURE_UNIT_DESCRIPTOR_LENGTH,         // bLength
        INTERFACE_DESCRIPTOR_TYPE,              // bDescriptorType
        CONTROL_FEATURE_UNIT,                   // bDescriptorSubtype
        0x02,                                   // bUnitID
        0x01,                                   // bSourceID
        0x01,                                   // bControlSize
        CONTROL_MUTE |
        CONTROL_VOLUME,                         // bmaControls(0)
        0x00,                                   // bmaControls(1)
        0x00,                                   // iTerminal

        // Audio Output Terminal (Speaker)
        OUTPUT_TERMINAL_DESCRIPTOR_LENGTH,      // bLength
        INTERFACE_DESCRIPTOR_TYPE,              // bDescriptorType
        CONTROL_OUTPUT_TERMINAL,                // bDescriptorSubtype
        0x03,                                   // bTerminalID
        LSB(TERMINAL_SPEAKER),                  // wTerminalType
        MSB(TERMINAL_SPEAKER),                  // wTerminalType
        0x00,                                   // bAssocTerminal
        0x02,                                   // bSourceID
        0x00,                                   // iTerminal


        // Audio Input Terminal (Microphone)
        INPUT_TERMINAL_DESCRIPTOR_LENGTH,       // bLength
        INTERFACE_DESCRIPTOR_TYPE,              // bDescriptorType
        CONTROL_INPUT_TERMINAL,                 // bDescriptorSubtype
        0x04,                                   // bTerminalID
        LSB(TERMINAL_MICROPHONE),               // wTerminalType
        MSB(TERMINAL_MICROPHONE),               // wTerminalType
        0x00,                                   // bAssocTerminal
        _tx_channel_count,                      // bNrChannels
        (uint8_t)(LSB(_channel_config_tx)),             // wChannelConfig
        (uint8_t)(MSB(_channel_config_tx)),             // wChannelConfig
        0x00,                                   // iChannelNames
        0x00,                                   // iTerminal

        // Audio Output Terminal (Microphone)
        OUTPUT_TERMINAL_DESCRIPTOR_LENGTH,      // bLength
        INTERFACE_DESCRIPTOR_TYPE,              // bDescriptorType
        CONTROL_OUTPUT_TERMINAL,                // bDescriptorSubtype
        0x05,                                   // bTerminalID
        LSB(TERMINAL_USB_STREAMING),            // wTerminalType
        MSB(TERMINAL_USB_STREAMING),            // wTerminalType
        0x00,                                   // bAssocTerminal
        0x04,                                   // bSourceID
        0x00,                                   // iTerminal






        // Interface 1, Alternate Setting 0, Audio Streaming - Zero Bandwith
        INTERFACE_DESCRIPTOR_LENGTH,            // bLength
        INTERFACE_DESCRIPTOR,                   // bDescriptorType
        0x01,                                   // bInterfaceNumber
        0x00,                                   // bAlternateSetting
        0x00,                                   // bNumEndpoints
        AUDIO_CLASS,                            // bInterfaceClass
        SUBCLASS_AUDIOSTREAMING,                // bInterfaceSubClass
        0x00,                                   // bInterfaceProtocol
        0x00,                                   // iInterface

        // Interface 1, Alternate Setting 1, Audio Streaming - Operational
        INTERFACE_DESCRIPTOR_LENGTH,            // bLength
        INTERFACE_DESCRIPTOR,                   // bDescriptorType
        0x01,                                   // bInterfaceNumber
        0x01,                                   // bAlternateSetting
        0x01,                                   // bNumEndpoints
        AUDIO_CLASS,                            // bInterfaceClass
        SUBCLASS_AUDIOSTREAMING,                // bInterfaceSubClass
        0x00,                                   // bInterfaceProtocol
        0x00,                                   // iInterface

        // Audio Streaming Interface
        STREAMING_INTERFACE_DESCRIPTOR_LENGTH,  // bLength
        INTERFACE_DESCRIPTOR_TYPE,              // bDescriptorType
        STREAMING_GENERAL,                      // bDescriptorSubtype
        0x01,                                   // bTerminalLink
        0x00,                                   // bDelay
        LSB(FORMAT_PCM),                        // wFormatTag
        MSB(FORMAT_PCM),                        // wFormatTag

        // Audio Type I Format
        FORMAT_TYPE_I_DESCRIPTOR_LENGTH,        // bLength
        INTERFACE_DESCRIPTOR_TYPE,              // bDescriptorType
        STREAMING_FORMAT_TYPE,                  // bDescriptorSubtype
        FORMAT_TYPE_I,                          // bFormatType
        _rx_channel_count,                      // bNrChannels
        0x02,                                   // bSubFrameSize
        16,                                     // bBitResolution
        0x01,                                   // bSamFreqType
        (uint8_t)(LSB(_rx_freq)),               // tSamFreq
        (uint8_t)((_rx_freq >> 8) & 0xff),      // tSamFreq
        (uint8_t)((_rx_freq >> 16) & 0xff),     // tSamFreq

        // Endpoint - Standard Descriptor
        ENDPOINT_DESCRIPTOR_LENGTH + 2,         // bLength
        ENDPOINT_DESCRIPTOR,                    // bDescriptorType
        _episo_out,                             // bEndpointAddress
        E_ISOCHRONOUS,                          // bmAttributes
        (uint8_t)(LSB(_rx_packet_size_max)),    // wMaxPacketSize
        (uint8_t)(MSB(_rx_packet_size_max)),    // wMaxPacketSize
        0x01,                                   // bInterval
        0x00,                                   // bRefresh
        0x00,                                   // bSynchAddress

        // Endpoint - Audio Streaming
        STREAMING_ENDPOINT_DESCRIPTOR_LENGTH,   // bLength
        ENDPOINT_DESCRIPTOR_TYPE,               // bDescriptorType
        ENDPOINT_GENERAL,                       // bDescriptor
        0x00,                                   // bmAttributes
        0x00,                                   // bLockDelayUnits
        LSB(0x0000),                            // wLockDelay
        MSB(0x0000),                            // wLockDelay


        // Interface 1, Alternate Setting 0, Audio Streaming - Zero Bandwith
        INTERFACE_DESCRIPTOR_LENGTH,            // bLength
        INTERFACE_DESCRIPTOR,                   // bDescriptorType
        0x02,                                   // bInterfaceNumber
        0x00,                                   // bAlternateSetting
        0x00,                                   // bNumEndpoints
        AUDIO_CLASS,                            // bInterfaceClass
        SUBCLASS_AUDIOSTREAMING,                // bInterfaceSubClass
        0x00,                                   // bInterfaceProtocol
        0x00,                                   // iInterface

        // Interface 1, Alternate Setting 1, Audio Streaming - Operational
        INTERFACE_DESCRIPTOR_LENGTH,            // bLength
        INTERFACE_DESCRIPTOR,                   // bDescriptorType
        0x02,                                   // bInterfaceNumber
        0x01,                                   // bAlternateSetting
        0x01,                                   // bNumEndpoints
        AUDIO_CLASS,                            // bInterfaceClass
        SUBCLASS_AUDIOSTREAMING,                // bInterfaceSubClass
        0x00,                                   // bInterfaceProtocol
        0x00,                                   // iInterface

        // Audio Streaming Interface
        STREAMING_INTERFACE_DESCRIPTOR_LENGTH,  // bLength
        INTERFACE_DESCRIPTOR_TYPE,              // bDescriptorType
        SUBCLASS_AUDIOCONTROL,                  // bDescriptorSubtype
        0x05,                                   // bTerminalLink (output terminal microphone)
        0x01,                                   // bDelay
        0x01,                                   // wFormatTag
        0x00,                                   // wFormatTag

        // Audio Type I Format
        FORMAT_TYPE_I_DESCRIPTOR_LENGTH,        // bLength
        INTERFACE_DESCRIPTOR_TYPE,              // bDescriptorType
        SUBCLASS_AUDIOSTREAMING,                // bDescriptorSubtype
        FORMAT_TYPE_I,                          // bFormatType
        _tx_channel_count,                      // bNrChannels
        0x02,                                   // bSubFrameSize
        0x10,                                   // bBitResolution
        0x01,                                   // bSamFreqType
        (uint8_t)(LSB(_tx_freq)),               // tSamFreq
        (uint8_t)((_tx_freq >> 8) & 0xff),      // tSamFreq
        (uint8_t)((_tx_freq >> 16) & 0xff),     // tSamFreq

        // Endpoint - Standard Descriptor
        ENDPOINT_DESCRIPTOR_LENGTH + 2,         // bLength
        ENDPOINT_DESCRIPTOR,                    // bDescriptorType
        _episo_in,                              // bEndpointAddress
        E_ISOCHRONOUS,                          // bmAttributes
        (uint8_t)(LSB(_tx_packet_size_max)),    // wMaxPacketSize
        (uint8_t)(MSB(_tx_packet_size_max)),    // wMaxPacketSize
        0x01,                                   // bInterval
        0x00,                                   // bRefresh
        0x00,                                   // bSynchAddress

        // Endpoint - Audio Streaming
        STREAMING_ENDPOINT_DESCRIPTOR_LENGTH,   // bLength
        ENDPOINT_DESCRIPTOR_TYPE,               // bDescriptorType
        ENDPOINT_GENERAL,                       // bDescriptor
        0x00,                                   // bmAttributes
        0x00,                                   // bLockDelayUnits
        LSB(0x0000),                            // wLockDelay
        MSB(0x0000),                            // wLockDelay
    };

    MBED_ASSERT(sizeof(config_descriptor_temp) == sizeof(_config_descriptor));
    memcpy(_config_descriptor, config_descriptor_temp, sizeof(_config_descriptor));
}

void USBAudio::_receive_change(ChannelState new_state)
{
    assert_locked();

    ChannelState prev_state = _rx_state;
    _rx_state = new_state;
    if (prev_state == new_state) {
        // no change
        return;
    }

    if (prev_state == Opened) {
        // Leaving the opened state
        _read_list.process();
        _rx_done.call(End);
    }
    if (new_state == Opened) {
        // Entering the opened state
        _read_list.process();
        _rx_done.call(Start);
    }
    if (new_state == Closed) {
        // Only block if the channel is closed
        _flags.clear(READ_READY_UNBLOCK);
    } else {
        _flags.set(READ_READY_UNBLOCK);
    }
}

void USBAudio::_receive_isr()
{
    assert_locked();

    uint32_t size = read_finish(_episo_out);

    if (size > _rx_queue.free()) {
        _rx_overflow++;
    } else {

        // Copy data over
        _rx_queue.write(_rx_packet_buf, size);

        // Signal that there is more data available
        _read_list.process();
        if (_rx_done) {
            _rx_done.call(Transfer);
        }
    }

    read_start(_episo_out, _rx_packet_buf, _rx_packet_size_max);
}

void USBAudio::_send_change(ChannelState new_state)
{
    assert_locked();

    ChannelState prev_state = _tx_state;
    _tx_state = new_state;
    if (prev_state == new_state) {
        // no change
        return;
    }

    if (prev_state == Opened) {
        // Leaving the opened state
        _write_list.process();
        _tx_done.call(End);
    }
    if (new_state == Opened) {
        // Entering the opened state
        _write_list.process();
        _tx_done.call(Start);
    }
    if (new_state == Closed) {
        // Only block if the channel is closed
        _flags.clear(WRITE_READY_UNBLOCK);
    } else {
        _flags.set(WRITE_READY_UNBLOCK);
    }
}
void USBAudio::_send_isr_start()
{
    assert_locked();

    if (!_tx_idle) {
        return;
    }

    _send_isr_next_sync();
}

void USBAudio::_send_isr_next_sync()
{
    // Compute size to send
    uint32_t fames = _tx_whole_frames_per_xfer;
    if (_tx_frame_fract >= XFER_FREQUENCY_HZ) {
        _tx_frame_fract -= XFER_FREQUENCY_HZ;
        fames += 1;
    }
    uint32_t send_size = fames * _tx_channel_count * 2;

    // Check if this is the initial TX packet
    if (_tx_idle && !_tx_queue.full()) {
        // Don't start until the TX buffer is full
        return;
    }

    // Check if this stream was closed
    if (_tx_state != Opened) {
        _tx_idle = true;
        return;
    }

    // Check for enough data to send
    if (_tx_queue.size() < send_size) {
        _tx_underflow++;
        _tx_idle = true;
        return;
    }

    // Copy data over
    _tx_queue.read(_tx_packet_buf, send_size);

    // Start the write
    write_start(_episo_in, _tx_packet_buf, send_size);
    _tx_idle = false;
    _tx_frame_fract += _tx_fract_frames_per_xfer;
}

void USBAudio::_send_isr()
{
    assert_locked();

    write_finish(_episo_in);

    _send_isr_next_sync();

    // Signal that there is space for more data
    _write_list.process();
    if (_tx_done) {
        _tx_done.call(Transfer);
    }
}
