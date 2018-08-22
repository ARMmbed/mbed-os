/*
 * Copyright (c) 2013-2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file pn512_registers.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 */

#ifndef PN512_REGISTERS_H_
#define PN512_REGISTERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stack/nfc_common.h"
#include "pn512.h"

//Page 0 - Command and Status
#define PN512_REG_PAGE 0x00 //Selects the register page
#define PN512_REG_COMMAND 0x01 //Starts and stops command execution
#define PN512_REG_COMIEN 0x02 //Controls bits to enable and disable the passing of Interrupt Requests
#define PN512_REG_DIVIEN 0x03 //Controls bits to enable and disable the passing of Interrupt Requests
#define PN512_REG_COMIRQ 0x04 //Contains Interrupt Request bits
#define PN512_REG_DIVIRQ 0x05 //Contains Interrupt Request bits
#define PN512_REG_ERROR 0x06 //Error bits showing the error status of the last command executed
#define PN512_REG_STATUS1 0x07 //Contains status bits for communication
#define PN512_REG_STATUS2 0x08 //Contains status bits of the receiver and transmitter
#define PN512_REG_FIFODATA 0x09 //In- and output of 64 byte FIFO-buffer
#define PN512_REG_FIFOLEVEL 0x0A //Indicates the number of bytes stored in the FIFO
#define PN512_REG_WATERLEVEL 0x0B //Defines the level for FIFO under- and overflow warning
#define PN512_REG_CONTROL 0x0C //Contains miscellaneous Control Registers
#define PN512_REG_BITFRAMING 0x0D //Adjustments for bit oriented frames
#define PN512_REG_COLL 0x0E //Bit position of the first bit collision detected on the RF-interface

//Page 1 - Command
//#define PN512_REG_PAGE 0x10 //Selects the register page
#define PN512_REG_MODE 0x11 //Defines general modes for transmitting and receiving
#define PN512_REG_TXMODE 0x12 //Defines the data rate and framing during transmission
#define PN512_REG_RXMODE 0x13 //Defines the data rate and framing during receiving
#define PN512_REG_TXCONTROL 0x14 //Controls the logical behavior of the antenna driver pins TX1 and TX2
#define PN512_REG_TXAUTO 0x15 //Controls the setting of the antenna drivers
#define PN512_REG_TXSEL 0x16 //Selects the internal sources for the antenna driver
#define PN512_REG_RXSEL 0x17 //Selects internal receiver settings
#define PN512_REG_RXTHRESHOLD 0x18 //Selects thresholds for the bit decoder
#define PN512_REG_DEMOD 0x19 //Defines demodulator settings
#define PN512_REG_FELNFC1 0x1A //Defines the length of the valid range for the receive package
#define PN512_REG_FELNFC2 0x1B //Defines the length of the valid range for the receive package
#define PN512_REG_MIFNFC 0x1C //Controls the communication in ISO/IEC 14443/MIFARE and NFC target mode at 106 kbit
#define PN512_REG_MANUALRCV 0x1D //Allows manual fine tuning of the internal receiver
#define PN512_REG_TYPEB 0x1E //Configure the ISO/IEC 14443 type B
#define PN512_REG_SERIALSPEED 0x1F //Selects the speed of the serial UART interface

//Page 2 - CFG
//#define PN512_REG_PAGE 0x20 //Selects the register page
#define PN512_REG_CRCRESULT_MSB 0x21 //Shows the actual MSB and LSB values of the CRC calculation
#define PN512_REG_CRCRESULT_LSB 0x22 //Shows the actual MSB and LSB values of the CRC calculation
#define PN512_REG_GSNOFF 0x23 //Selects the conductance of the antenna driver pins TX1 and TX2 for modulation, when the driver is switched off
#define PN512_REG_MODWIDTH 0x24 //Controls the setting of the ModWidth
#define PN512_REG_TXBITPHASE 0x25 //Adjust the TX bit phase at 106 kbit
#define PN512_REG_RFCFG 0x26 //Configures the receiver gain and RF level
#define PN512_REG_GSNON 0x27 //Selects the conductance of the antenna driver pins TX1 and TX2 for modulation when the drivers are switched on
#define PN512_REG_CWGSP 0x28 //Selects the conductance of the antenna driver pins TX1 and TX2 for modulation during times of no modulation
#define PN512_REG_MODGSP 0x29 //Selects the conductance of the antenna driver pins TX1 and TX2 for modulation during modulation
#define PN512_REG_TMODE_TPRESCALERHIGH 0x2A //Defines settings for the internal timer
#define PN512_REG_TPRESCALERLOW 0x2B //Defines settings for the internal timer
#define PN512_REG_TRELOADHIGH 0x2C //Describes the 16-bit timer reload value
#define PN512_REG_TRELOADLOW 0x2D //Describes the 16-bit timer reload value
#define PN512_REG_TCOUNTERVALHIGH 0x2E //Shows the 16-bit actual timer value
#define PN512_REG_TCOUNTERVALLOW 0x2F //Shows the 16-bit actual timer value

//Page 3 - TestRegister
//#define PN512_REG_PAGE 0x30 //Selects the register page
#define PN512_REG_TESTSEL1 0x31 //General test signal configuration
#define PN512_REG_TESTSEL2 0x32 //General test signal configuration and PRBS control
#define PN512_REG_TESTPINEN 0x33 //Enables pin output driver on 8-bit parallel bus (Note: For serial interfaces only)
#define PN512_REG_TESTPINVALUE 0x34 //Defines the values for the 8-bit parallel bus when it is used as I/O bus
#define PN512_REG_TESTBUS 0x35 //Shows the status of the internal testbus
#define PN512_REG_AUTOTEST 0x36 //Controls the digital selftest
#define PN512_REG_VERSION 0x37 //Shows the version
#define PN512_REG_ANALOGTEST 0x38 //Controls the pins AUX1 and AUX2
#define PN512_REG_TESTDAC1 0x39 //Defines the test value for the TestDAC1
#define PN512_REG_TESTDAC2 0x3A //Defines the test value for the TestDAC2
#define PN512_REG_TESTADC 0x3B //Shows the actual value of ADC I and Q


void pn512_registers_init(pn512_t *pPN512);
void pn512_registers_reset(pn512_t *pPN512);

void pn512_register_write(pn512_t *pPN512, uint8_t address, uint8_t data);
uint8_t pn512_register_read(pn512_t *pPN512, uint8_t address);

void pn512_register_switch_page(pn512_t *pPN512, uint8_t address);

#ifdef __cplusplus
}
#endif

#endif /* PN512_REGISTERS_H_ */
