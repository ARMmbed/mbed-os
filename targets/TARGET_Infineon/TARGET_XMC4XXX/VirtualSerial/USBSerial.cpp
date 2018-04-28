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

/***************************************************************** Includes **/
#include "USBSerial.h"
#include "mbed.h"
#include "VirtualSerial.h"

/****************************************************************** Globals **/
static CircularBuffer<int16_t, 1024> ringBuf;

/**************************************************************** Functions **/

/** @brief USB TASK
 *
 */
static void usb_vcom_task() {
    while (true) {
        wait(0.1);
        /* Call USB Task periodically */
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);

        /* Check if new message is available */
        while(CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface)){
            ringBuf.push(CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface));
        }
    }
}

/** @brief Constructor
 *
 */
USBSerial::USBSerial(void){


    /* initialize Virtlal USB port */
    USB_VCOM_Init();

    /* start USB task */
    this->usb_vcom_thread.start(usb_vcom_task);
}

/** @brief Send a character. You can use puts, printf.
 *
 *  @param c character to be sent
 *  @returns true if there is no error, false otherwise
 *
 */
int USBSerial::_putc(int c) {

    if(CDC_Device_SendByte(&VirtualSerial_CDC_Interface, c) != 0){
        return 0;;
    }
    return 1;
}

/** @brief Read a character: blocking
 *
 *  @returns character read
 *
 */
int USBSerial::_getc() {

    int16_t val = 0;

    // wait until character is available
    while(ringBuf.empty());
    // get & remove character from ring buffer
    ringBuf.pop(val);
    return val;
}

/** @brief Determine if there is space available to write a character
 *
 *  @returns
 *    1 if there is space to write a character,
 *    0 otherwise
 */
int USBSerial::writable(void){

    //TODO: Not supported yet
    return 1;
}

/** @brief Determine if there is a character available to read
 *
 *  @returns
 *    1 if there is a character available to read,
 *    0 otherwise
 */
int USBSerial::readable(void){

    if(!ringBuf.empty()){
        return 1;
    }
    return 0;
}

/** @ brief Write a block of data.
 *
 * For more efficiency, a block of size 64 (maximum size of a bulk endpoint) has to be written.
 *
 * @param buf pointer on data which will be written
 * @param size size of the buffer. The maximum size of a block is limited by the size of the endpoint (64 bytes)
 *
 * @returns true if successfull
 */
bool USBSerial::writeBlock(uint8_t * buf, uint16_t size){

    for(int i = 0; i < size; i++){
        if(CDC_Device_SendByte(&VirtualSerial_CDC_Interface, *(buf+i)) != 0){
            return false;
        }
    }
    return true;
}

/*EOF*/

