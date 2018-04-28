/* mbed Example Program
 * Copyright (c) 2006-2014 ARM Limited
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

#ifndef USBSERIAL_H_
#define USBSERIAL_H_

#include "mbed.h"
#include "rtos.h"
#include "Stream.h"
#include "VirtualSerial.h"

class USBSerial : public Stream{
public:

    /** brief Constructor
     *
     */
    USBSerial();

    /** @brief Send a character. You can use puts, printf.
     *
     *  @param c character to be sent
     *  @returns true if there is no error, false otherwise
     *
     */
    virtual int _putc(int c);

    /** @brief Read a character: blocking
     *
     *  @returns character read
     *
     */
    virtual int _getc();

    /** @brief Determine if there is space available to write a character
     *
     *  @returns
     *    1 if there is space to write a character,
     *    0 otherwise
     */
    int writable(void);

    /** @brief Determine if there is a character available to read
     *
     *  @returns
     *    1 if there is a character available to read,
     *    0 otherwise
     */
    int readable(void);

    /** @ brief Write a block of data.
    *
    * For more efficiency, a block of size 64 (maximum size of a bulk endpoint) has to be written.
    *
    * @param buf pointer on data which will be written
    * @param size size of the buffer. The maximum size of a block is limited by the size of the endpoint (64 bytes)
    *
    * @returns true if successfull
    */
    bool writeBlock(uint8_t * buf, uint16_t size);

private:

    Thread usb_vcom_thread;

};

#endif /* USBSERIAL_H_ */

/*EOF*/
