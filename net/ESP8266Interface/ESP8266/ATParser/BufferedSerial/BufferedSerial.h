
/**
 * @file    BufferedSerial.h
 * @brief   Software Buffer - Extends mbed Serial functionallity adding irq driven TX and RX
 * @author  sam grove
 * @version 1.0
 * @see     
 *
 * Copyright (c) 2013
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

#ifndef BUFFEREDSERIAL_H
#define BUFFEREDSERIAL_H
 
#include "mbed.h"
#include "MyBuffer.h"

/** A serial port (UART) for communication with other serial devices
 *
 * Can be used for Full Duplex communication, or Simplex by specifying
 * one pin as NC (Not Connected)
 *
 * Example:
 * @code
 *  #include "mbed.h"
 *  #include "BufferedSerial.h"
 *
 *  BufferedSerial pc(USBTX, USBRX);
 *
 *  int main()
 *  { 
 *      while(1)
 *      {
 *          Timer s;
 *        
 *          s.start();
 *          pc.printf("Hello World - buffered\n");
 *          int buffered_time = s.read_us();
 *          wait(0.1f); // give time for the buffer to empty
 *        
 *          s.reset();
 *          printf("Hello World - blocking\n");
 *          int polled_time = s.read_us();
 *          s.stop();
 *          wait(0.1f); // give time for the buffer to empty
 *        
 *          pc.printf("printf buffered took %d us\n", buffered_time);
 *          pc.printf("printf blocking took %d us\n", polled_time);
 *          wait(0.5f);
 *      }
 *  }
 * @endcode
 */

/**
 *  @class BufferedSerial
 *  @brief Software buffers and interrupt driven tx and rx for Serial
 */  
class BufferedSerial : public RawSerial 
{
private:
    MyBuffer <char> _rxbuf;
    MyBuffer <char> _txbuf;
    uint32_t      _buf_size;
    uint32_t      _tx_multiple;
 
    void rxIrq(void);
    void txIrq(void);
    void prime(void);

    Callback<void()> _cbs[2];
    
public:
    /** Create a BufferedSerial port, connected to the specified transmit and receive pins
     *  @param tx Transmit pin
     *  @param rx Receive pin
     *  @param buf_size printf() buffer size
     *  @param tx_multiple amount of max printf() present in the internal ring buffer at one time
     *  @param name optional name
     *  @note Either tx or rx may be specified as NC if unused
     */
    BufferedSerial(PinName tx, PinName rx, uint32_t buf_size = 256, uint32_t tx_multiple = 4,const char* name=NULL);
    
    /** Destroy a BufferedSerial port
     */
    virtual ~BufferedSerial(void);
    
    /** Check on how many bytes are in the rx buffer
     *  @return 1 if something exists, 0 otherwise
     */
    virtual int readable(void);
    
    /** Check to see if the tx buffer has room
     *  @return 1 always has room and can overwrite previous content if too small / slow
     */
    virtual int writeable(void);
    
    /** Get a single byte from the BufferedSerial Port.
     *  Should check readable() before calling this.
     *  @return A byte that came in on the Serial Port
     */
    virtual int getc(void);
    
    /** Write a single byte to the BufferedSerial Port.
     *  @param c The byte to write to the Serial Port
     *  @return The byte that was written to the Serial Port Buffer
     */
    virtual int putc(int c);
    
    /** Write a string to the BufferedSerial Port. Must be NULL terminated
     *  @param s The string to write to the Serial Port
     *  @return The number of bytes written to the Serial Port Buffer
     */
    virtual int puts(const char *s);
    
    /** Write a formatted string to the BufferedSerial Port.
     *  @param format The string + format specifiers to write to the Serial Port
     *  @return The number of bytes written to the Serial Port Buffer
     */
    virtual int printf(const char* format, ...);
    
    /** Write data to the Buffered Serial Port
     *  @param s A pointer to data to send
     *  @param length The amount of data being pointed to
     *  @return The number of bytes written to the Serial Port Buffer
     */
    virtual ssize_t write(const void *s, std::size_t length);

    /** Attach a function to call whenever a serial interrupt is generated
     *  @param func A pointer to a void function, or 0 to set as none
     *  @param type Which serial interrupt to attach the member function to (Serial::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    virtual void attach(Callback<void()> func, IrqType type=RxIrq);

    /** Attach a member function to call whenever a serial interrupt is generated
     *  @param obj pointer to the object to call the member function on
     *  @param method pointer to the member function to call
     *  @param type Which serial interrupt to attach the member function to (Serial::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    template <typename T>
    void attach(T *obj, void (T::*method)(), IrqType type=RxIrq) {
        attach(Callback<void()>(obj, method), type);
    }

    /** Attach a member function to call whenever a serial interrupt is generated
     *  @param obj pointer to the object to call the member function on
     *  @param method pointer to the member function to call
     *  @param type Which serial interrupt to attach the member function to (Serial::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    template <typename T>
    void attach(T *obj, void (*method)(T*), IrqType type=RxIrq) {
        attach(Callback<void()>(obj, method), type);
    }
};

#endif
