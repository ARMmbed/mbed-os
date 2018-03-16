/* mbed Microcontroller Library
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

#ifndef __BLE_UART_SERVICE_H__
#define __BLE_UART_SERVICE_H__

#ifdef YOTTA_CFG_MBED_OS
#include "mbed-drivers/mbed.h"
#include "mbed-drivers/Stream.h"
#else
#include "mbed.h"
#include "Stream.h"
#endif

#include "ble/UUID.h"
#include "ble/BLE.h"

extern const uint8_t  UARTServiceBaseUUID[UUID::LENGTH_OF_LONG_UUID];
extern const uint16_t UARTServiceShortUUID;
extern const uint16_t UARTServiceTXCharacteristicShortUUID;
extern const uint16_t UARTServiceRXCharacteristicShortUUID;

extern const uint8_t  UARTServiceUUID[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t  UARTServiceUUID_reversed[UUID::LENGTH_OF_LONG_UUID];

extern const uint8_t  UARTServiceTXCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID];
extern const uint8_t  UARTServiceRXCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID];

/**
* @class UARTService.
* @brief BLE Service to enable UART over BLE.
*/
class UARTService {
public:
    /** Maximum length of data (in bytes) that the UART service module can transmit to the peer. */
    static const unsigned BLE_UART_SERVICE_MAX_DATA_LEN = (BLE_GATT_MTU_SIZE_DEFAULT - 3);

public:

    /**
    * @param _ble
    *               BLE object for the underlying controller.
    */
    UARTService(BLE &_ble) :
        ble(_ble),
        receiveBuffer(),
        sendBuffer(),
        sendBufferIndex(0),
        numBytesReceived(0),
        receiveBufferIndex(0),
        txCharacteristic(UARTServiceTXCharacteristicUUID, receiveBuffer, 1, BLE_UART_SERVICE_MAX_DATA_LEN,
                         GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE),
        rxCharacteristic(UARTServiceRXCharacteristicUUID, sendBuffer, 1, BLE_UART_SERVICE_MAX_DATA_LEN, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY) {
        GattCharacteristic *charTable[] = {&txCharacteristic, &rxCharacteristic};
        GattService         uartService(UARTServiceUUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

        ble.addService(uartService);
        ble.onDataWritten(this, &UARTService::onDataWritten);
    }

    /**
     * Note: TX and RX characteristics are to be interpreted from the viewpoint of the GATT client using this service.
     */
    uint16_t getTXCharacteristicHandle() {
        return txCharacteristic.getValueAttribute().getHandle();
    }

    /**
     * Note: TX and RX characteristics are to be interpreted from the viewpoint of the GATT client using this service.
     */
    uint16_t getRXCharacteristicHandle() {
        return rxCharacteristic.getValueAttribute().getHandle();
    }

    /**
     * We attempt to collect bytes before pushing them to the UART RX
     * characteristic; writing to the RX characteristic then generates
     * notifications for the client. Updates made in quick succession to a
     * notification-generating characteristic result in data being buffered
     * in the Bluetooth stack as notifications are sent out. The stack has
     * its limits for this buffering - typically a small number under 10.
     * Collecting data into the sendBuffer buffer helps mitigate the rate of
     * updates. But we shouldn't buffer a large amount of data before updating
     * the characteristic, otherwise the client needs to turn around and make
     * a long read request; this is because notifications include only the first
     * 20 bytes of the updated data.
     *
     * @param  _buffer The received update.
     * @param  length Number of characters to be appended.
     * @return        Number of characters appended to the rxCharacteristic.
     */
    size_t write(const void *_buffer, size_t length) {
        size_t         origLength = length;
        const uint8_t *buffer     = static_cast<const uint8_t *>(_buffer);

        if (ble.getGapState().connected) {
            unsigned bufferIndex = 0;
            while (length) {
                unsigned bytesRemainingInSendBuffer = BLE_UART_SERVICE_MAX_DATA_LEN - sendBufferIndex;
                unsigned bytesToCopy                = (length < bytesRemainingInSendBuffer) ? length : bytesRemainingInSendBuffer;

                /* Copy bytes into sendBuffer. */
                memcpy(&sendBuffer[sendBufferIndex], &buffer[bufferIndex], bytesToCopy);
                length          -= bytesToCopy;
                sendBufferIndex += bytesToCopy;
                bufferIndex     += bytesToCopy;

                /* Have we collected enough? */
                if ((sendBufferIndex == BLE_UART_SERVICE_MAX_DATA_LEN) ||
                    // (sendBuffer[sendBufferIndex - 1] == '\r')          ||
                    (sendBuffer[sendBufferIndex - 1] == '\n')) {
                    ble.gattServer().write(getRXCharacteristicHandle(), static_cast<const uint8_t *>(sendBuffer), sendBufferIndex);
                    sendBufferIndex = 0;
                }
            }
        }

        return origLength;
    }

    /**
     * Helper function to write out strings.
     * @param  str The received string.
     * @return     Number of characters appended to the rxCharacteristic.
     */
    size_t writeString(const char *str) {
        return write(str, strlen(str));
    }

    /**
     * Flush sendBuffer, i.e., forcefully write its contents to the UART RX
     * characteristic even if the buffer is not full.
     */
    void flush() {
        if (ble.getGapState().connected) {
            if (sendBufferIndex != 0) {
                ble.gattServer().write(getRXCharacteristicHandle(), static_cast<const uint8_t *>(sendBuffer), sendBufferIndex);
                sendBufferIndex = 0;
            }
        }
    }

    /**
     * Override for Stream::_putc().
     * @param  c
     *         This function writes the character c, cast to an unsigned char, to stream.
     * @return
     *     The character written as an unsigned char cast to an int or EOF on error.
     */
    int _putc(int c) {
        return (write(&c, 1) == 1) ? 1 : EOF;
    }

    /**
     * Override for Stream::_getc().
     * @return
     *     The character read.
     */
    int _getc() {
        if (receiveBufferIndex == numBytesReceived) {
            return EOF;
        }

        return receiveBuffer[receiveBufferIndex++];
    }

protected:
    /**
     * This callback allows the UART service to receive updates to the
     * txCharacteristic. The application should forward the call to this
     * function from the global onDataWritten() callback handler; if that's
     * not used, this method can be used as a callback directly.
     */
    void onDataWritten(const GattWriteCallbackParams *params) {
        if (params->handle == getTXCharacteristicHandle()) {
            uint16_t bytesRead = params->len;
            if (bytesRead <= BLE_UART_SERVICE_MAX_DATA_LEN) {
                numBytesReceived   = bytesRead;
                receiveBufferIndex = 0;
                memcpy(receiveBuffer, params->data, numBytesReceived);
            }
        }
    }

protected:
    BLE                &ble;

    uint8_t             receiveBuffer[BLE_UART_SERVICE_MAX_DATA_LEN]; /**< The local buffer into which we receive
                                                                       *   inbound data before forwarding it to the
                                                                       *   application. */

    uint8_t             sendBuffer[BLE_UART_SERVICE_MAX_DATA_LEN];    /**< The local buffer into which outbound data is
                                                                       *   accumulated before being pushed to the
                                                                       *   rxCharacteristic. */
    uint8_t             sendBufferIndex;
    uint8_t             numBytesReceived;
    uint8_t             receiveBufferIndex;

    GattCharacteristic  txCharacteristic; /**< From the point of view of the external client, this is the characteristic
                                           *   they'd write into in order to communicate with this application. */
    GattCharacteristic  rxCharacteristic; /**< From the point of view of the external client, this is the characteristic
                                           *   they'd read from in order to receive the bytes transmitted by this
                                           *   application. */
};

#endif /* #ifndef __BLE_UART_SERVICE_H__*/
