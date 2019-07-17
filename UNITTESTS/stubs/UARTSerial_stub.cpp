/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#include "UARTSerial.h"

namespace mbed {


UARTSerial::UARTSerial(PinName tx, PinName rx, int baud) : SerialBase(tx, rx, baud)
{
}

UARTSerial::~UARTSerial()
{
}

ssize_t UARTSerial::read(void *buffer, size_t length)
{
    return 0;
}

ssize_t UARTSerial::write(const void *buffer, size_t length)
{
    return 0;
}

off_t UARTSerial::seek(off_t offset, int whence)
{
    return -ESPIPE;
}

int UARTSerial::close()
{
    return 0;
}

void UARTSerial::dcd_irq()
{
}

void UARTSerial::set_baud(int baud)
{
}

void UARTSerial::set_data_carrier_detect(PinName dcd_pin, bool active_high)
{
}

void UARTSerial::set_format(int bits, Parity parity, int stop_bits)
{
}

int UARTSerial::isatty()
{
    return 1;
}

int UARTSerial::sync()
{
    return 0;
}

void UARTSerial::sigio(Callback<void()> func)
{
}

ssize_t UARTSerial::write_unbuffered(const char *buf_ptr, size_t length)
{
    return 0;
}

bool UARTSerial::hup() const
{
}

void UARTSerial::wake()
{
}

short UARTSerial::poll(short events) const
{
    return 0;
}

void UARTSerial::lock()
{
}

void UARTSerial::unlock()
{
}

void UARTSerial::api_lock(void)
{
}

void UARTSerial::api_unlock(void)
{
}

void UARTSerial::rx_irq(void)
{
}

void UARTSerial::tx_irq(void)
{
}

int UARTSerial::enable_input(bool enabled)
{
    return 0;
}

int UARTSerial::enable_output(bool enabled)
{
    return 0;
}

void UARTSerial::set_flow_control(mbed::SerialBase::Flow, PinName, PinName)
{

}

}
