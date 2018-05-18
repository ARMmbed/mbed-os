/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#ifndef MBED_SAI_H
#define MBED_SAI_H

#include "platform/platform.h"

#if defined (DEVICE_SAI) || defined(DOXYGEN_ONLY)

#include "hal/sai_api.h"
#include "platform/NonCopyable.h"
#include "platform/PlatformMutex.h"

namespace mbed {
/** \addtogroup drivers */

/** A SAI Master, used for communicating with SAI slave devices
 * @code
 * // Send and read back a data packet between a SAI transmitter/receiver
 *
 * #include "mbed.h"
 *
 * // SAI A/B channel pins defined in targets PinNames.h file
 * SAI sai_transmitter(SAI_A_MCLK, SAI_A_BCLK, SAI_A_WCLK, SAI_A_SD, false);
 * SAI sai_receiver(SAI_B_MCLK, SAI_B_BLCK, SAI_B_WCLK, SAI_B_SD, true);
 *
 * int main() {
 *     uint32_t sample = 0x12345678;
 *
 *     // Check SAI object's ctor status
 *     if (sai_transmitter.status() != SAI_RESULT_OK || sai_receiver.status() != SAI_RESULT_OK) {
 *         error("Transmitter status: %x, Receiver status: %x\r\n", sai_transmitter.status(), sai_receiver.status());
 *     }
 *
 *     if (sai_transmitter.xfer(&sample)) {
 *         printf("Transmitter error\r\n");
 *     }
 *
 *     sample = 0;   // Clobber sample to ensure received data is valid
 *
 *     if (sai_receiver.xfer(&sample)) {
 *         printf("Receiver error\r\n");
 *     }
 *
 *     printf("Received sample: %d\r\n", sample);
 * }
 * @endcode
 *
 * SAI Transmitter/Receiver Class Example:
 * @code
 * // Send and read back a data packet between a SAI transmitter/receiver subclass
 *
 * #include "mbed.h"
 *
 * // SAI A/B channel pins defined in targets PinNames.h file
 * SAITransmitter sai_transmitter(SAI_A_MCLK, SAI_A_BCLK, SAI_A_WCLK, SAI_A_SD);
 * SAIReceiver    sai_receiver(SAI_B_MCLK, SAI_B_BLCK, SAI_B_WCLK, SAI_B_SD);
 *
 * int main() {
 *     uint32_t sample = 0x12345678;
 *
 *     // Check SAI object's ctor status
 *     if (sai_transmitter.status() != SAI_RESULT_OK || sai_receiver.status() != SAI_RESULT_OK) {
 *         error("Transmitter status: %x, Receiver status: %x\r\n", sai_transmitter.status(), sai_receiver.status());
 *     }
 *
 *     if (sai_transmitter.send(sample)) {
 *         printf("Transmitter error\r\n");
 *     }
 *
 *     sample = 0;   // Clobber sample to ensure received data is valid
 *
 *     if (sai_receiver.receive(&sample)) {
 *         printf("Receiver error\r\n");
 *     }
 *
 *     printf("Received sample: %d\r\n", sample);
 * }
 * @endcode
 * @ingroup drivers
 */
class SAI : private NonCopyable<SAI> {

public:

    /** Create a SAI
     */
    SAI(PinName mclk, PinName bclk, PinName wclk, PinName sd,
        const sai_format_t *fmt = &sai_mode_i2s32, bool is_input = false,
        uint32_t master_clock = 0, bool internal_mclk = false);

    /** Retreive the object status. Allows the ctor to
     * funnel init errors/warnings back up to the application.
     */
    virtual sai_result_t status(void);

    /** Push a sample to the FIFO or try to read back a new
     *  sample depending on the direction of communication
     *  configured in the constructor.
     *
     *   @param  value  Integer pointer to a sample to transmit or
     *                  place received data into.
     *   @return        Returns 0 if no sample was available.
     */
    virtual bool xfer(uint32_t *value);

    virtual ~SAI() {}

protected:
    /** Acquire exclusive access to this SAI bus
     */
    virtual void lock(void);

    /** Release exclusive access to this SAI bus
     */
    virtual void unlock(void);

    sai_t           _sai;
    PlatformMutex   _mutex;
    bool            _is_input;
    sai_result_t    _status;
};

/** A SAI Tramsmitter, used for communicating with SAI slave devices
 *
 *  Inherits from the SAI class
 */
class SAITransmitter : public SAI {
    public:
        /** Create a SAITransmitter
        */
        SAITransmitter(PinName mclk, PinName bclk, PinName wclk, PinName sd,
                       const sai_format_t *fmt = &sai_mode_i2s32)
           : SAI(mclk, bclk, wclk, sd, fmt, false) { }

        /** Push a sample to the FIFO.
         *   @param  sample Integer sample to transmit
         *   @return        Returns 0 if no sample was available.
         */
        bool send(uint32_t sample) {
            return this->xfer(&sample);
        }
};

/** A SAI Receiver, used for reading samples from a SAI transmitter
 *
 *  Inherits from the SAI class
 */
class SAIReceiver : public SAI {
    public:
        /** Create a SAIReceiver
        */
        SAIReceiver(PinName mclk, PinName bclk, PinName wclk, PinName sd,
                    const sai_format_t *fmt = &sai_mode_i2s32)
            : SAI(mclk, bclk, wclk, sd, fmt, true) { }

        /** Read a sample from the FIFO.
         *   @param  sample Integer pointer to buffer to fill with the
         *                  received sample.
         *   @return        Returns 0 if no sample was available.
         */
        bool receive(uint32_t *sample) {
            return this->xfer(sample);
        }
};

} // namespace mbed

#endif

#endif
