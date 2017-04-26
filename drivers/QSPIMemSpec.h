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
#ifndef QSPIMEMSPEC_H
#define QSPIMEMSPEC_H

#include "qspi_api.h"

/** Type representing a quantity of 8-bit bytes
 */
typedef uint64_t memspec_size_t;

namespace mbed {

/** \addtogroup drivers */
/** @{*/

/** QSPI driver helper. Storing memory config data. Could be inherit to implement for example
 * filling this class with data received from SFDP function.
 *
 */
class QSPIMemSpec
{
public:
    /** Options to get into quad mode
      */
    typedef enum
    {
        MEMSPEC_QUADMODREQ_NO_REQ,
        MEMSPEC_QUADMODREQ_NOT_SUPPORTED,
        MEMSPEC_QUADMODREQ_1BIT_2SREG_CLR2B,
        MEMSPEC_QUADMODREQ_6BIT_1SREG,
        MEMSPEC_QUADMODREQ_7BIT_2SREG_W3EH_R3FH,
        MEMSPEC_QUADMODREQ_1BIT_2SREG_CLR1B,
        MEMSPEC_QUADMODREQ_1BIT_2SREG_R35H_W01H_W2B,
        MEMSPEC_QUADMODREQ_NOT_DEFINED,
    } MemSpec_QuadModeReq_t;

    /** Options to get into 32bit addressing mode
      */
    typedef enum
    {
        MEMSPEC_32BITMODREQ_NOT_SUPPORTED,
        MEMSPEC_32BITMODREQ_INST_B7H,
        MEMSPEC_32BITMODREQ_WREN_INST_B7H,
        MEMSPEC_32BITMODREQ_DEDICATED,
        MEMSPEC_32BITMODREQ_ALWAYS_32BIT,
        MEMSPEC_32BITMODREQ_OTHER,
        MEMSPEC_32BITMODREQ_NOT_DEFINED,
    } MemSpec_32bitAddrModeReq_t;

    /** Create a Memory Specification object
     *
     * @param cmd_32bit   Command used to enter to 32bit addressing mode
     * @param flash_siz   Flash size of the memory device
     * @param page_siz    Page size supported by the memory device
     * @param sector_siz  Erasing sector size supported by the memory device
     * @param addr_mod    The best addressing mode supported by the memory device
     * @param read_mod    The fastest read pin configuration supported by the memory device
     * @param write_mod   The fastest write pin configuration supported by the memory device
     * @param quad_mode   hint how to enter in quad mode
     * @param enter_32bit hint how to enter in 32bit addressing mode
     */
    QSPIMemSpec(uint8_t cmd_32bit,
                memspec_size_t flash_siz,
                memspec_size_t page_siz,
                memspec_size_t sector_siz,
                qspi_addrmode_t addr_mod,
                qspi_readmode_t read_mod,
                qspi_writemode_t write_mod,
                MemSpec_QuadModeReq_t quad_mode,
                MemSpec_32bitAddrModeReq_t enter_32bit);

    /** Flash size getter
     * @return flash size
     */
    memspec_size_t flash_size_get(void) const;

    /** Page size getter
     * @return Page size
     */
    memspec_size_t page_size_get(void) const;

    /** Erase sector size getter
     * @return Erase sector size
     */
    memspec_size_t sector_size_get(void) const;

    /** Address mode getter
     * @return The best addressing mode
     */
    qspi_addrmode_t addr_mode_get(void) const;

    /** Read mode getter
     * @return The fastest pin configuration
     */
    qspi_readmode_t read_mode_get(void) const;

    /** Write mode getter
     * @return The fastest pin configuration
     */
    qspi_writemode_t write_mode_get(void) const;

    /** 32bit addressing mode way to obtain getter
     * @return 32bit addressing mode hint
     */
    MemSpec_32bitAddrModeReq_t enter_32bit_mode_req_get(void) const;

    /** Quad mode way to obtain getter
     * @return Quad mode hint
     */
    MemSpec_QuadModeReq_t quad_mode_req_get(void) const;

protected:
    uint8_t cmd_enter_32bit;
    memspec_size_t flash_size;
    memspec_size_t page_size;
    memspec_size_t sector_size;
    qspi_addrmode_t addr_mode;
    qspi_readmode_t read_mode;
    qspi_writemode_t write_mode;
    MemSpec_QuadModeReq_t quad_mode_req;
    MemSpec_32bitAddrModeReq_t enter_32bit_mode_req;
};

}
#endif // QSPIMEMSPEC_H
