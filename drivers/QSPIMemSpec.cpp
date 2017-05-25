/* mbed Microcontroller Library
 * Copyright (c) 2017 Nordic Semiconductor ASA
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
#include "QSPIMemSpec.h"


#ifdef DEVICE_QSPI

namespace mbed {

QSPIMemSpec::QSPIMemSpec(uint8_t cmd_32bit,
                         memspec_size_t flash_siz,
                         memspec_size_t page_siz,
                         memspec_size_t sector_siz,
                         qspi_addrmode_t addr_mod,
                         qspi_readmode_t read_mod,
                         qspi_writemode_t write_mod,
                         MemSpec_QuadModeReq_t quad_mode,
                         MemSpec_32bitAddrModeReq_t enter_32bit)
{
    cmd_enter_32bit      = cmd_32bit;
    flash_size           = flash_siz;
    page_size            = page_siz;
    sector_size          = sector_siz;
    addr_mode            = addr_mod;
    read_mode            = read_mod;
    write_mode           = write_mod;
    quad_mode_req        = quad_mode;
    enter_32bit_mode_req = enter_32bit;
}

qspi_addrmode_t QSPIMemSpec::addr_mode_get() const
{
    return addr_mode;
}

qspi_readmode_t QSPIMemSpec::read_mode_get() const
{
    return read_mode;
}

qspi_writemode_t QSPIMemSpec::write_mode_get() const
{
    return write_mode;
}

memspec_size_t QSPIMemSpec::flash_size_get() const
{
    return flash_size;
}

memspec_size_t QSPIMemSpec::page_size_get() const
{
    return page_size;
}

memspec_size_t QSPIMemSpec::sector_size_get() const
{
    return sector_size;
}

QSPIMemSpec::MemSpec_32bitAddrModeReq_t QSPIMemSpec::enter_32bit_mode_req_get() const
{
    return enter_32bit_mode_req;
}

QSPIMemSpec::MemSpec_QuadModeReq_t QSPIMemSpec::quad_mode_req_get() const
{
    return quad_mode_req;
}

}

#endif
