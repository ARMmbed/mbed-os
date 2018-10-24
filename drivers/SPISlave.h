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
#ifndef MBED_SPISLAVE_H
#define MBED_SPISLAVE_H

#include "platform/platform.h"
#include "platform/NonCopyable.h"

#if defined (DEVICE_SPISLAVE) || defined(DOXYGEN_ONLY)

#include "hal/spi_api.h"

namespace mbed {
/** \addtogroup drivers */

/** A SPI slave, used for communicating with a SPI master device.
 *
 * The default format is set to 8 bits, mode 0 and a clock frequency of 1MHz.
 *
 * @note Synchronization level: Not protected
 *
 * Example of how to reply to a SPI master as slave:
 * @code
 *
 * #include "mbed.h"
 *
 * SPISlave device(SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_CS);
 *
 * int main() {
 *     device.reply(0x00);              // Prime SPI with first reply
 *     while(1) {
 *         if(device.receive()) {
 *             int v = device.read();   // Read byte from master
 *             v = (v + 1) % 0x100;     // Add one to it, modulo 256
 *             device.reply(v);         // Make this the next reply
 *         }
 *     }
 * }
 * @endcode
 * @ingroup drivers
 */
class SPISlave : private NonCopyable<SPISlave> {

public:

    /** Create a SPI slave connected to the specified pins.
     *
     *  @note Either mosi or miso can be specified as NC if not used.
     *
     *  @param mosi SPI Master Out, Slave In pin.
     *  @param miso SPI Master In, Slave Out pin.
     *  @param sclk SPI Clock pin.
     *  @param ssel SPI Chip Select pin.
     */
    SPISlave(PinName mosi, PinName miso, PinName sclk, PinName ssel);

    /** Configure the data transmission format.
     *
     *  @param bits Number of bits per SPI frame (4 - 16).
     *  @param mode Clock polarity and phase mode (0 - 3).
     *
     * @code
     * mode | POL PHA
     * -----+--------
     *   0  |  0   0
     *   1  |  0   1
     *   2  |  1   0
     *   3  |  1   1
     * @endcode
     */
    void format(int bits, int mode = 0);

    /** Set the SPI bus clock frequency.
     *
     *  @param hz Clock frequency in hz (default = 1MHz).
     */
    void frequency(int hz = 1000000);

    /** Polls the SPI to see if data has been received.
     *
     *  @return Presence of received data.
     *  @retval 0 No data waiting.
     *  @retval 1 Data waiting.
     */
    int receive(void);

    /** Retrieve data from receive buffer as slave.
     *
     *  @return The data in the receive buffer.
     */
    int read(void);

    /** Fill the transmission buffer with the value to be written out
     *  as slave on the next received message from the master.
     *
     *  @param value The data to be transmitted next.
     */
    void reply(int value);

#if !defined(DOXYGEN_ONLY)

protected:
    /* Internal SPI object identifying the resources */
    spi_t _spi;

    /* How many bits in an SPI frame */
    int _bits;
    /* Clock phase and polarity */
    int _mode;
    /* Clock frequency */
    int _hz;

#endif //!defined(DOXYGEN_ONLY)
};

} // namespace mbed

#endif

#endif
