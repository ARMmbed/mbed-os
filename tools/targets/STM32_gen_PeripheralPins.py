"""
* mbed Microcontroller Library
* Copyright (c) 2006-2018 ARM Limited
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
"""

import argparse
import datetime
import fnmatch
import json
import os
import re
import sys
import textwrap
from xml.dom.minidom import parse, Node
from argparse import RawTextHelpFormatter

GENPINMAP_VERSION = "1.3"

ADD_DEVICE_IFDEF = 0
ADD_QSPI_FEATURE = 1

mcu_file=""
mcu_list = []       #'name'
io_list = []        #'PIN','name'
adclist = []        #'PIN','name','ADCSignal'
daclist = []        #'PIN','name','DACSignal'
i2cscl_list = []    #'PIN','name','I2CSCLSignal'
i2csda_list = []    #'PIN','name','I2CSDASignal'
pwm_list = []       #'PIN','name','PWM'
uarttx_list = []    #'PIN','name','UARTtx'
uartrx_list = []    #'PIN','name','UARTrx'
uartcts_list = []   #'PIN','name','UARTcts'
uartrts_list = []   #'PIN','name','UARTrts'
spimosi_list = []   #'PIN','name','SPIMOSI'
spimiso_list = []   #'PIN','name','SPIMISO'
spissel_list = []   #'PIN','name','SPISSEL'
spisclk_list = []   #'PIN','name','SPISCLK'
cantd_list = []     #'PIN','name','CANTD'
canrd_list = []     #'PIN','name','CANRD'
eth_list = []       #'PIN','name','ETH'
quadspidata_list = []      #'PIN','name','QUADSPIDATA'
quadspisclk_list = []      #'PIN','name','QUADSPISCLK'
quadspissel_list = []      #'PIN','name','QUADSPISSEL'
usb_list = []      #'PIN','name','USB'
osc_list = []      #'PIN','name','OSC'
sys_list = []      #'PIN','name','SYS'

TIM_MST_LIST = { # Timer used for us ticker is hardcoded in this script
"NUCLEO_F030R8":"TIM1",
"NUCLEO_F072RB":"TIM2",
"NUCLEO_F091RC":"TIM2",
"NUCLEO_F070RB":"TIM1",
"NUCLEO_F042K6":"TIM2",
"NUCLEO_F031K6":"TIM2",
"NUCLEO_F103RB":"TIM4",
"NUCLEO_F207ZG":"TIM5",
"NUCLEO_F302R8":"TIM2",
"NUCLEO_F334R8":"TIM2",
"NUCLEO_F303RE":"TIM2",
"NUCLEO_F303K8":"TIM2",
"NUCLEO_F303ZE":"TIM2",
"NUCLEO_F401RE":"TIM5",
"NUCLEO_F411RE":"TIM5",
"NUCLEO_F446RE":"TIM5",
"NUCLEO_F410RB":"TIM5",
"NUCLEO_F429ZI":"TIM5",
"NUCLEO_F446ZE":"TIM5",
"NUCLEO_F412ZG":"TIM5",
"NUCLEO_F413ZH":"TIM5",
"NUCLEO_F746ZG":"TIM5",
"NUCLEO_F767ZI":"TIM5",
"NUCLEO_F722ZE":"TIM5",
"NUCLEO_H743ZI":"TIM5",
"NUCLEO_L053R8":"TIM21",
"NUCLEO_L073RZ":"TIM21",
"NUCLEO_L031K6":"TIM21",
"NUCLEO_L011K4":"TIM21",
"NUCLEO_L152RE":"TIM5",
"NUCLEO_L476RG":"TIM5",
"NUCLEO_L432KC":"TIM2",
"NUCLEO_L496ZG":"TIM5",
"NUCLEO_L496ZG_P":"TIM5",
"NUCLEO_L433RC_P":"TIM2",

"DISCO_F051R8":"TIM1",
"DISCO_F100RB":"TIM4",
"DISCO_F303VC":"TIM2",
"DISCO_F334C8":"TIM2",
"DISCO_F401VC":"TIM5",
"DISCO_F407VG":"TIM5",
"DISCO_F413ZH":"TIM5",
"DISCO_F429ZI":"TIM5",
"DISCO_F469NI":"TIM5",
"DISCO_F769NI":"TIM5",
"DISCO_F746NG":"TIM5",
"DISCO_L053C8":"TIM21",
"DISCO_L072CZ_LRWAN1":"TIM21",
"DISCO_L475VG_IOT01A":"TIM5",
"DISCO_L476VG":"TIM5",
"DISCO_L496AG":"TIM5"
}


def find_gpio_file():
    res = 'ERROR'
    itemlist = xml_mcu.getElementsByTagName('IP')
    for s in itemlist:
        a = s.attributes['Name'].value
        if "GPIO" in a:
            res = s.attributes['Version'].value
    return res

def get_gpio_af_num(pintofind, iptofind):
    if 'STM32F10' in mcu_file:
        return get_gpio_af_numF1(pintofind, iptofind)
    #DBG print ('pin to find ' + pintofind)
    i=0
    mygpioaf = 'NOTFOUND'
    for n in  xml_gpio.documentElement.childNodes:
        i += 1
        j = 0
        if n.nodeType == Node.ELEMENT_NODE:
            for firstlevel in n.attributes.items():
#                if 'PB7' in firstlevel:
                if pintofind ==  firstlevel[1]:
                    #DBG print (i , firstlevel)
                    #n = pin node found
                    for m in n.childNodes:
                        j += 1
                        k = 0
                        if m.nodeType == Node.ELEMENT_NODE:
                            for secondlevel in  m.attributes.items():
                                k += 1
#                                if 'I2C1_SDA' in secondlevel:
                                if iptofind in secondlevel:
                                    #DBG print (i, j,  m.attributes.items())
                                    # m = IP node found
                                    for p in m.childNodes:
                                        if p.nodeType == Node.ELEMENT_NODE:
                                            #p node of 'Specific parameter'
                                            #DBG print (i,j,k,p.attributes.items())
                                            for myc in p.childNodes:
                                                #DBG print (myc)
                                                if myc.nodeType == Node.ELEMENT_NODE:
                                                    #myc = node of ALTERNATE
                                                    for mygpioaflist in myc.childNodes:
                                                        mygpioaf += ' ' + mygpioaflist.data
                                                        #print (mygpioaf)
    if mygpioaf == 'NOTFOUND':
        print ('GPIO AF not found in ' + gpiofile + ' for ' + pintofind + ' and the IP ' + iptofind)
        #quit()
    return mygpioaf.replace('NOTFOUND ', '')

def get_gpio_af_numF1(pintofind, iptofind):
    #print ('pin to find ' + pintofind + ' ip to find ' + iptofind)
    i=0
    mygpioaf = 'NOTFOUND'
    for n in  xml_gpio.documentElement.childNodes:
        i += 1
        j = 0
        if n.nodeType == Node.ELEMENT_NODE:
            for firstlevel in n.attributes.items():
                #print ('firstlevel ' , firstlevel)
#                if 'PB7' in firstlevel:
                if pintofind ==  firstlevel[1]:
                    #print ('firstlevel ' , i , firstlevel)
                    #n = pin node found
                    for m in n.childNodes:
                        j += 1
                        k = 0
                        if m.nodeType == Node.ELEMENT_NODE:
                            for secondlevel in  m.attributes.items():
                                #print ('secondlevel ' , i, j, k , secondlevel)
                                k += 1
#                                if 'I2C1_SDA' in secondlevel:
                                if iptofind in secondlevel:
                                    # m = IP node found
                                    #print (i, j,  m.attributes.items())
                                    for p in m.childNodes:
                                        #p node 'RemapBlock'
                                        if p.nodeType == Node.ELEMENT_NODE and p.hasChildNodes() == False:
                                            mygpioaf += ' AFIO_NONE'
                                        else:
                                            for s in p.childNodes:
                                                if s.nodeType == Node.ELEMENT_NODE:
                                                    #s node 'Specific parameter'
                                                    #DBG print (i,j,k,p.attributes.items())
                                                    for myc in s.childNodes:
                                                        #DBG print (myc)
                                                        if myc.nodeType == Node.ELEMENT_NODE:
                                                            #myc = AF value
                                                            for mygpioaflist in myc.childNodes:
                                                                mygpioaf += ' ' + mygpioaflist.data.replace("__HAL_", "").replace("_REMAP", "")
                                                                #print mygpioaf
    if mygpioaf == 'NOTFOUND':
        print ('GPIO AF not found in ' + gpiofile + ' for ' + pintofind + ' and the IP ' + iptofind + ' set as AFIO_NONE')
        mygpioaf = 'AFIO_NONE'
    return mygpioaf.replace('NOTFOUND ', '')\
        .replace("AFIO_NONE", "0")\
        .replace("AFIO_SPI1_ENABLE", "1")\
        .replace("AFIO_I2C1_ENABLE", "2")\
        .replace("AFIO_USART1_ENABLE", "3")\
        .replace("AFIO_USART3_PARTIAL", "5")\
        .replace("AFIO_TIM1_PARTIAL", "6")\
        .replace("AFIO_TIM3_PARTIAL", "7")\
        .replace("AFIO_TIM2_ENABLE", "8")\
        .replace("AFIO_TIM3_ENABLE", "9")\
        .replace("AFIO_CAN1_2", "10")

#function to store I/O pin
def store_pin (pin, name):
    p = [pin, name]
    io_list.append(p)

#function to store ADC list
def store_adc (pin, name, signal):
    adclist.append([pin,name,signal])

#function to store DAC list
def store_dac (pin, name, signal):
    daclist.append([pin,name,signal])

#function to store I2C list
def store_i2c (pin, name, signal):
    #is it SDA or SCL ?
    if "_SCL" in signal:
        i2cscl_list.append([pin,name,signal])
    if "_SDA" in signal:
        i2csda_list.append([pin,name,signal])

#function to store timers
def store_pwm(pin, name, signal):
    if "_CH" in signal:
        pwm_list.append([pin,name,signal])

#function to store Uart pins
def store_uart(pin, name, signal):
    if "_TX" in signal:
        uarttx_list.append([pin,name,signal])
    if "_RX" in signal:
        uartrx_list.append([pin,name,signal])
    if "_CTS" in signal:
        uartcts_list.append([pin,name,signal])
    if "_RTS" in signal:
        uartrts_list.append([pin,name,signal])

#function to store SPI pins
def store_spi(pin, name, signal):
    if "_MISO" in signal:
        spimiso_list.append([pin,name,signal])
    if "_MOSI" in signal:
        spimosi_list.append([pin,name,signal])
    if "_SCK" in signal:
        spisclk_list.append([pin,name,signal])
    if "_NSS" in signal:
        spissel_list.append([pin,name,signal])

#function to store CAN pins
def store_can(pin, name, signal):
    if "_RX" in signal:
        canrd_list.append([pin,name,signal])
    if "_TX" in signal:
        cantd_list.append([pin,name,signal])

#function to store ETH list
def store_eth (pin, name, signal):
    eth_list.append([pin,name,signal])

#function to store QSPI pins
def store_qspi (pin, name, signal):
    if "_BK" in signal:
        quadspidata_list.append([pin,name,signal])
    if "_CLK" in signal:
        quadspisclk_list.append([pin,name,signal])
    if "_NCS" in signal:
        quadspissel_list.append([pin,name,signal])

#function to store USB pins
def store_usb (pin, name, signal):
    usb_list.append([pin,name,signal])

#function to store OSC pins
def store_osc (pin, name, signal):
    osc_list.append([pin,name,signal])

#function to store SYS pins
def store_sys (pin, name, signal):
    sys_list.append([pin,name,signal])

def print_header():
    s =  ("""/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) %i, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 *
 * Automatically generated from %s
 */

#include "PeripheralPins.h"
#include "mbed_toolchain.h"

//==============================================================================
// Notes
//
// - The pins mentioned Px_y_ALTz are alternative possibilities which use other
//   HW peripheral instances. You can use them the same way as any other "normal"
//   pin (i.e. PwmOut pwm(PA_7_ALT0);). These pins are not displayed on the board
//   pinout image on mbed.org.
//
// - The pins which are connected to other components present on the board have
//   the comment "Connected to xxx". The pin function may not work properly in this
//   case. These pins may not be displayed on the board pinout image on mbed.org.
//   Please read the board reference manual and schematic for more information.
//
// - Warning: pins connected to the default STDIO_UART_TX and STDIO_UART_RX pins are commented
//   See https://os.mbed.com/teams/ST/wiki/STDIO for more information.
//
//==============================================================================

""" % (datetime.datetime.now().year, os.path.basename(input_file_name)))
    out_c_file.write( s )

    s =  ("""/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) %i, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 *
 * Automatically generated from %s
 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ALT0  = 0x100,
    ALT1  = 0x200,
    ALT2  = 0x300,
    ALT3  = 0x400
} ALTx;

typedef enum {

""" % (datetime.datetime.now().year, os.path.basename(input_file_name)))
    out_h_file.write( s )


def print_footer():
    s = ("""
    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
""")
    out_h_file.write(s)


def print_all_lists():
    if print_list_header("ADC", "ADC", adclist, "ANALOGIN"):
        print_adc()
    if print_list_header("DAC", "DAC", daclist, "ANALOGOUT"):
        print_dac()
    if print_list_header("I2C", "I2C_SDA", i2csda_list, "I2C"):
        print_i2c(i2csda_list)
    if print_list_header("", "I2C_SCL", i2cscl_list, "I2C"):
        print_i2c(i2cscl_list)
    if print_list_header("PWM", "PWM", pwm_list, "PWMOUT"):
        print_pwm()
    if print_list_header("SERIAL", "UART_TX", uarttx_list, "SERIAL"):
        print_uart(uarttx_list)
    if print_list_header("", "UART_RX", uartrx_list, "SERIAL"):
        print_uart(uartrx_list)
    if print_list_header("", "UART_RTS", uartrts_list, "SERIAL"):
        print_uart(uartrts_list)
    if print_list_header("", "UART_CTS", uartcts_list, "SERIAL"):
        print_uart(uartcts_list)
    if print_list_header("SPI", "SPI_MOSI", spimosi_list, "SPI"):
        print_spi(spimosi_list)
    if print_list_header("", "SPI_MISO", spimiso_list, "SPI"):
        print_spi(spimiso_list)
    if print_list_header("", "SPI_SCLK", spisclk_list, "SPI"):
        print_spi(spisclk_list)
    if print_list_header("", "SPI_SSEL", spissel_list, "SPI"):
        print_spi(spissel_list)
    if print_list_header("CAN", "CAN_RD", canrd_list, "CAN"):
        print_can(canrd_list)
    if print_list_header("", "CAN_TD", cantd_list, "CAN"):
        print_can(cantd_list)
    if ADD_QSPI_FEATURE:
        if print_list_header("QUADSPI", "QSPI_DATA", quadspidata_list, "QSPI"):
            print_qspi(quadspidata_list)
        if print_list_header("", "QSPI_SCLK", quadspisclk_list, "QSPI"):
            print_qspi(quadspisclk_list)
        if print_list_header("", "QSPI_SSEL", quadspissel_list, "QSPI"):
            print_qspi(quadspissel_list)
    print_h_file(usb_list, "USB")
    print_h_file(eth_list, "ETHERNET")
    print_h_file(osc_list, "OSCILLATOR")
    print_h_file(sys_list, "DEBUG")

def print_list_header(comment, name, l, switch):
    s = ""
    if len(l)>0:
        if comment:
            s += "\n//*** %s ***\n" % comment

        s += "\n"

        if name == "PWM":
            if TargetName in TIM_MST_LIST.keys():
                s += "// %s cannot be used because already used by the us_ticker\n" % TIM_MST_LIST[TargetName]
            else:
                s += "// TIM<x> cannot be used because already used by the us_ticker\n"
                s += "// You have to comment all PWM using TIM_MST defined in hal_tick.h file\n"
                s += "//  or update python script (check TIM_MST_LIST) and re-run it\n"

        if ADD_DEVICE_IFDEF:
            s += "#ifdef DEVICE_%s\n" % switch

        s += "MBED_WEAK const PinMap PinMap_%s[] = {\n" % name

    # else:
    #     if comment:
    #         s += "\n//*** No %s ***\n" % comment

    out_c_file.write(s)
    return len(l)

def print_adc():
    s_pin_data = 'STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, '
    prev_p = ''
    alt_index = 0
    for p in adclist:
        if "IN" in p[2]:
            CommentedLine = "  "
            if p[1] in PinLabel.keys():
                if "STDIO_UART" in PinLabel[p[1]]:
                    CommentedLine = "//"
                if "RCC_OSC" in PinLabel[p[1]]:
                    CommentedLine = "//"
            if CommentedLine != "//":
                if p[0] == prev_p:
                    prev_p = p[0]
                    p[0] += '_ALT%d' % alt_index
                    alt_index += 1
                else:
                    prev_p = p[0]
                    alt_index = 0
            s1 = "%-17s" % (CommentedLine + "  {" + p[0] + ',')
            a = p[2].split('_')
            inst = a[0].replace("ADC", "")
            if len(inst) == 0:
                inst = '1' #single ADC for this product
            s1 += "%-7s" % ('ADC_' + inst + ',')
            chan = re.sub('IN[N|P]?', '', a[1])
            s1 += s_pin_data + chan
            s1 += ', 0)}, // ' + p[2]
            if p[1] in PinLabel.keys():
                s1 += ' // Connected to ' + PinLabel[p[1]]
            s1 += '\n'
            out_c_file.write(s1)
    out_c_file.write( """    {NC, NC, 0}
};

// !!! SECTION TO BE CHECKED WITH DEVICE REFERENCE MANUAL
MBED_WEAK const PinMap PinMap_ADC_Internal[] = {
    {ADC_TEMP,   ADC_1,    STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 16, 0)},
    {ADC_VREF,   ADC_1,    STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 17, 0)},
    {ADC_VBAT,   ADC_1,    STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 18, 0)},
    {NC, NC, 0}
};
""")
    if ADD_DEVICE_IFDEF:
        out_c_file.write( "#endif\n" )

def print_dac():
    for p in daclist:
        CommentedLine = "  "
        if p[1] in PinLabel.keys():
            if "STDIO_UART" in PinLabel[p[1]]:
                CommentedLine = "//"
            if "RCC_OSC" in PinLabel[p[1]]:
                CommentedLine = "//"
        s1 = "%-17s" % (CommentedLine + "  {" + p[0] + ',')
        #p[2] : DAC_OUT1 / DAC1_OUT1
        a = p[2].split('_')
        inst = a[0].replace("DAC", "")
        b = a[1].replace("OUT", "")
        if len(inst) == 0:
            inst = '1'  # single DAC for this product
        s1 += "%-7s" % ('DAC_' + inst + ',')
        s1 += 'STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, ' + b + ', 0)}, // ' + p[2]
        if p[1] in PinLabel.keys():
            s1 += ' // Connected to ' + PinLabel[p[1]]
        s1 += '\n'
        out_c_file.write(s1)
    out_c_file.write( """    {NC, NC, 0}
};
""")
    if ADD_DEVICE_IFDEF:
        out_c_file.write( "#endif\n" )

def print_i2c(l):
    prev_p = ''
    alt_index = 0
    for p in l:
        result = get_gpio_af_num(p[1], p[2])
        if result != 'NOTFOUND':
            CommentedLine = "  "
            if p[1] in PinLabel.keys():
                if "STDIO_UART" in PinLabel[p[1]]:
                    CommentedLine = "//"
                if "RCC_OSC" in PinLabel[p[1]]:
                    CommentedLine = "//"
            if CommentedLine != "//":
                if p[0] == prev_p:
                    prev_p = p[0]
                    p[0] += '_ALT%d' % alt_index
                    alt_index += 1
                else:
                    prev_p = p[0]
                    alt_index = 0
            s1 = "%-17s" % (CommentedLine + "  {" + p[0] + ',')
            # p[2] : I2C1_SDA / FMPI2C1_SDA
            if "FMP" in p[2]:
                inst = p[2].split('_')[0].replace("FMPI2C", "")
                s1 += "%-10s" % ('FMPI2C_' + inst + ',')
            else:
                inst = p[2].split('_')[0].replace("I2C", "")
                s1 += "%-7s" % ('I2C_' + inst + ',')
            s1 += 'STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, '
            r = result.split(' ')
            for af in r:
                s2 = s1 + af  + ')},'
                if p[1] in PinLabel.keys():
                    s2 += ' // Connected to ' + PinLabel[p[1]]
                s2 += '\n'
                out_c_file.write(s2)
    out_c_file.write( """    {NC, NC, 0}
};
""")
    if ADD_DEVICE_IFDEF:
        out_c_file.write( "#endif\n" )

def print_pwm():
    prev_p = ''
    alt_index = 0
    TIM_MST = "NOT_KNOWN"
    if TargetName in TIM_MST_LIST.keys():
        TIM_MST = TIM_MST_LIST[TargetName]
    for p in pwm_list:
        result = get_gpio_af_num(p[1], p[2])
        if result != 'NOTFOUND':
            CommentedLine = "  "
            if p[1] in PinLabel.keys():
                if "STDIO_UART" in PinLabel[p[1]]:
                    CommentedLine = "//"
                if "RCC_OSC" in PinLabel[p[1]]:
                    CommentedLine = "//"
            if "%s_" % TIM_MST in p[2]:
                CommentedLine = "//"
            if CommentedLine != "//":
                if p[0] == prev_p:
                    prev_p = p[0]
                    p[0] += '_ALT%d' % alt_index
                    alt_index += 1
                else:
                    prev_p = p[0]
                    alt_index = 0
            s1 = "%-17s" % (CommentedLine + "  {" + p[0] + ',')
            # p[2] : TIM2_CH1 / TIM15_CH1N
            a = p[2].split('_')
            inst = a[0].replace("TIM", "PWM_")
            # if len(inst) == 3:
            #     inst += '1'
            s1 += "%-8s" % (inst + ',')
            chan = a[1].replace("CH", "")
            if chan.endswith('N'):
                neg = ', 1'
                chan = chan.strip('N')
            else:
                neg = ', 0'
            s1 += 'STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, '
            r = result.split(' ')
            for af in r:
                s2 = s1 + af + ', ' + chan + neg + ')}, // ' + p[2]
                if p[1] in PinLabel.keys():
                    s2 += ' // Connected to ' + PinLabel[p[1]]
                s2 += '\n'
                out_c_file.write(s2)
    out_c_file.write( """    {NC, NC, 0}
};
""")
    if ADD_DEVICE_IFDEF:
        out_c_file.write( "#endif\n" )

def print_uart(l):
    prev_p = ''
    alt_index = 0
    for p in l:
        result = get_gpio_af_num(p[1], p[2])
        if result != 'NOTFOUND':
            CommentedLine = "  "
            if p[1] in PinLabel.keys():
                if "RCC_OSC" in PinLabel[p[1]]:
                    CommentedLine = "//"
            if CommentedLine != "//":
                if p[0] == prev_p:
                    prev_p = p[0]
                    p[0] += '_ALT%d' % alt_index
                    alt_index += 1
                else:
                    prev_p = p[0]
                    alt_index = 0
            s1 = "%-17s" % (CommentedLine + "  {" + p[0] + ',')
            # p[2] : USART2_RX
            b=p[2].split('_')[0]
            b = b.replace("UART", "UART_")
            b = b.replace("USART", "UART_")
            s1 += "%-9s" % (b[:len(b)-1] +  b[len(b)-1:] + ',')
            if 'STM32F10' in mcu_file and l == uartrx_list:
                s1 += 'STM_PIN_DATA(STM_MODE_INPUT, GPIO_PULLUP, '
            else:
                s1 += 'STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, '
            r = result.split(' ')
            for af in r:
                s2 = s1 + af  + ')},'
                if p[1] in PinLabel.keys():
                    s2 += ' // Connected to ' + PinLabel[p[1]]
                s2 += '\n'
                out_c_file.write(s2)
    out_c_file.write( """    {NC, NC, 0}
};
""")
    if ADD_DEVICE_IFDEF:
        out_c_file.write( "#endif\n" )

def print_spi(l):
    prev_p = ''
    alt_index = 0
    for p in l:
        result = get_gpio_af_num(p[1], p[2])
        if result != 'NOTFOUND':
            CommentedLine = "  "
            if p[1] in PinLabel.keys():
                if "STDIO_UART" in PinLabel[p[1]]:
                    CommentedLine = "//"
                if "RCC_OSC" in PinLabel[p[1]]:
                    CommentedLine = "//"
            if CommentedLine != "//":
                if p[0] == prev_p:
                    prev_p = p[0]
                    p[0] += '_ALT%d' % alt_index
                    alt_index += 1
                else:
                    prev_p = p[0]
                    alt_index = 0
            s1 = "%-17s" % (CommentedLine + "  {" + p[0] + ',')
            # p[2] : SPI1_MISO
            instance=p[2].split('_')[0].replace("SPI", "")
            s1 += "%-7s" % ('SPI_' + instance + ',')
            s1 += 'STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, '
            r = result.split(' ')
            for af in r:
                s2 = s1 + af  + ')},'
                if p[1] in PinLabel.keys():
                    s2 += ' // Connected to ' + PinLabel[p[1]]
                s2 += '\n'
                out_c_file.write(s2)
    out_c_file.write( """    {NC, NC, 0}
};
""")
    if ADD_DEVICE_IFDEF:
        out_c_file.write( "#endif\n" )

def print_can(l):
    for p in l:
        result = get_gpio_af_num(p[1], p[2])
        if result != 'NOTFOUND':
            CommentedLine = "  "
            if p[1] in PinLabel.keys():
                if "STDIO_UART" in PinLabel[p[1]]:
                    CommentedLine = "//"
                if "RCC_OSC" in PinLabel[p[1]]:
                    CommentedLine = "//"
            s1 = "%-17s" % (CommentedLine + "  {" + p[0] + ',')
            # p[2] : CAN_RX / CAN1_RX
            p[2] = p[2].replace("FD", "")
            instance = p[2].split('_')[0].replace("CAN", "")
            if len(instance) == 0:
                instance = '1'
            s1 += "%-7s" % ('CAN_' + instance + ',')
            if 'STM32F10' in mcu_file and l == canrd_list:
                s1 += 'STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, '
            else:
                s1 += 'STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, '
            r = result.split(' ')
            for af in r:
                s2 = s1 + af  + ')},'
                if p[1] in PinLabel.keys():
                    s2 += ' // Connected to ' + PinLabel[p[1]]
                s2 += '\n'
                out_c_file.write(s2)
    out_c_file.write( """    {NC, NC, 0}
};
""")
    if ADD_DEVICE_IFDEF:
        out_c_file.write( "#endif\n" )

def print_qspi(l):
    for p in l:
        result = get_gpio_af_num(p[1], p[2])
        if result != 'NOTFOUND':
            CommentedLine = "  "
            if p[1] in PinLabel.keys():
                if "STDIO_UART" in PinLabel[p[1]]:
                    CommentedLine = "//"
                if "RCC_OSC" in PinLabel[p[1]]:
                    CommentedLine = "//"
            s1 = "%-16s" % (CommentedLine + "  {" + p[0] + ',')
            # p[2] : QUADSPI_BK1_IO3 / QUADSPI_CLK / QUADSPI_NCS
            s1 += "%-8s" % ('QSPI_1,')
            result = result.replace("GPIO_AF10_OTG_FS", "GPIO_AF10_QSPI")
            s1 += 'STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, ' + result +')},'
            s1 += '  // ' + p[2]
            if p[1] in PinLabel.keys():
                s1 += ' // Connected to ' + PinLabel[p[1]]
            s1 += '\n'
            out_c_file.write(s1)
    out_c_file.write( """    {NC, NC, 0}
};
""")
    if ADD_DEVICE_IFDEF:
        out_c_file.write( "#endif\n" )

def print_h_file(l, comment):
    if len(l) > 0:
        s = ("\n/**** %s pins ****/\n" % comment)
        out_h_file.write(s)

        prev_s = ''
        alt_index = 0
        for p in l:
            if p[2] == prev_s:
                prev_s = p[2]
                p[2] += '_ALT%d' % alt_index
                alt_index += 1
            else:
                prev_s = p[2]
                alt_index = 0
            s1 = "    %s = %s,\n" % (p[2].replace("-", "_"), p[0])
            out_h_file.write(s1)
    # else:
    #     s = ("\n/**** No %s pins ***/\n" % comment)
    #     out_h_file.write(s)


tokenize = re.compile(r'(\d+)|(\D+)').findall

def natural_sortkey(list_2_elem):
    return tuple(int(num) if num else alpha for num, alpha in tokenize(list_2_elem[0]))

def natural_sortkey2(list_2_elem):
    return tuple(int(num) if num else alpha for num, alpha in tokenize(list_2_elem[2]))

def natural_sortkey_uart(list_2_elem):
    return tuple(int(num) if num else alpha for num, alpha in tokenize(list_2_elem[2].replace("USART", "UART").replace("LPUART", "ZUART")))

def natural_sortkey_i2c(list_2_elem):
    return tuple(int(num) if num else alpha for num, alpha in tokenize(list_2_elem[2].replace("FMPI2C", "ZFMPI2C")))

def sort_my_lists():
    adclist.sort(key=natural_sortkey)
    daclist.sort(key=natural_sortkey)
    i2cscl_list.sort(key=natural_sortkey_i2c) # first sort on name column
    i2csda_list.sort(key=natural_sortkey_i2c) # first sort on name column
    i2cscl_list.sort(key=natural_sortkey)
    i2csda_list.sort(key=natural_sortkey)
    pwm_list.sort(key=natural_sortkey2) # first sort on name column
    pwm_list.sort(key=natural_sortkey)
    uarttx_list.sort(key=natural_sortkey_uart) # first sort on name column
    uartrx_list.sort(key=natural_sortkey_uart) # first sort on name column
    uartcts_list.sort(key=natural_sortkey_uart) # first sort on name column
    uartrts_list.sort(key=natural_sortkey_uart) # first sort on name column
    uarttx_list.sort(key=natural_sortkey)
    uartrx_list.sort(key=natural_sortkey)
    uartcts_list.sort(key=natural_sortkey)
    uartrts_list.sort(key=natural_sortkey)
    spimosi_list.sort(key=natural_sortkey)
    spimiso_list.sort(key=natural_sortkey)
    spissel_list.sort(key=natural_sortkey)
    spisclk_list.sort(key=natural_sortkey)
    cantd_list.sort(key=natural_sortkey)
    canrd_list.sort(key=natural_sortkey)
    eth_list.sort(key=natural_sortkey2)
    quadspidata_list.sort(key=natural_sortkey)
    quadspisclk_list.sort(key=natural_sortkey)
    quadspissel_list.sort(key=natural_sortkey)
    usb_list.sort(key=natural_sortkey2)
    osc_list.sort(key=natural_sortkey2)
    sys_list.sort(key=natural_sortkey2)

def clean_all_lists():
    del io_list[:]
    del adclist[:]
    del daclist[:]
    del i2cscl_list[:]
    del i2csda_list[:]
    del pwm_list[:]
    del uarttx_list[:]
    del uartrx_list[:]
    del uartcts_list[:]
    del uartrts_list[:]
    del spimosi_list[:]
    del spimiso_list[:]
    del spissel_list[:]
    del spisclk_list[:]
    del cantd_list[:]
    del canrd_list[:]
    del eth_list[:]
    del quadspidata_list[:]
    del quadspisclk_list[:]
    del quadspissel_list[:]
    del usb_list[:]
    del osc_list[:]
    del sys_list[:]

def parse_pins():
    # print (" * Getting pins per Ips...")
    pinregex=r'^(P[A-Z][0-9][0-5]?)'
    itemlist = xml_mcu.getElementsByTagName('Pin')
    for s in itemlist:
        m = re.match(pinregex, s.attributes['Name'].value)
        if m:
            pin = m.group(0)[:2] + '_' + m.group(0)[2:] # pin formatted P<port>_<number>: PF_O
            name = s.attributes['Name'].value.strip()   # full name: "PF0 / OSC_IN"
            if s.attributes['Type'].value == "I/O":
                store_pin(pin, name)
            else:
                continue
            siglist = s.getElementsByTagName('Signal')
            for a in siglist:
                sig = a.attributes['Name'].value.strip()
                if "ADC" in sig:
                    store_adc(pin, name, sig)
                if all(["DAC" in sig, "_OUT" in sig]):
                    store_dac(pin, name, sig)
                if "I2C" in sig:
                    store_i2c(pin, name, sig)
                if re.match('^TIM', sig) is not None: #ignore HRTIM
                    store_pwm(pin, name, sig)
                if re.match('^(LPU|US|U)ART', sig) is not None:
                    store_uart(pin, name, sig)
                if "SPI" in sig:
                    store_spi(pin, name, sig)
                if "CAN" in sig:
                    store_can(pin, name, sig)
                if "ETH" in sig:
                    store_eth(pin, name, sig)
                if "QUADSPI" in sig:
                    store_qspi(pin, name, sig)
                if "USB" in sig:
                    store_usb(pin, name, sig)
                if "RCC_OSC" in sig:
                    store_osc(pin, name, sig)
                if "SYS_" in sig:
                    store_sys(pin, name, sig)

PinData = {}
PinLabel = {}

def parse_BoardFile(fileName):
    print(" * Board file: '%s'" % (fileName))
    board_file = open(board_file_name, "r")
    # IOC_PIN_pattern = re.compile(r'(P[A-I][\d]*).*\.([\w]*)=(.*)')
    IOC_PIN_pattern = re.compile(r'(.*)\.([\w]*)=(.*)')
    for line in board_file.readlines():
        IOC_PIN = re.match(IOC_PIN_pattern, line)
        if IOC_PIN:
            if IOC_PIN.groups()[0] in PinData.keys():
                PinData[IOC_PIN.groups()[0]][IOC_PIN.groups()[1]] = IOC_PIN.groups()[2]
            else:
                PinData[IOC_PIN.groups()[0]] = {}
                PinData[IOC_PIN.groups()[0]][IOC_PIN.groups()[1]] = IOC_PIN.groups()[2]
        # IOC_MCU = re.match(r'Mcu\.UserName=(.*)', line)
        IOC_MCU = re.match(r'Mcu\.Name=(.*)', line)
        if IOC_MCU:
            mcu_list.append("%s.xml" % IOC_MCU.groups()[0])

    board_file.close()

    for EachPin in PinData:
        try:
            PinLabel[EachPin] = PinData[EachPin]["Signal"]
        except:
            pass

        try:
            PinLabel[EachPin] = PinData[EachPin]["GPIO_Label"]

            if "STLK_RX" in PinLabel[EachPin] or "STLK_TX" in PinLabel[EachPin]:
                # Patch waiting for CubeMX correction
                if "RX" in PinData[EachPin]["Signal"]:
                    PinLabel[EachPin] = "STDIO_UART_RX"
                else:
                    PinLabel[EachPin] = "STDIO_UART_TX"
            elif "USART_RX" in PinLabel[EachPin]:
                PinLabel[EachPin] = "STDIO_UART_RX"
            elif "USART_TX" in PinLabel[EachPin]:
                PinLabel[EachPin] = "STDIO_UART_TX"
            elif "VCP_RX" in PinLabel[EachPin]:
                PinLabel[EachPin] = "STDIO_UART_RX"
            elif "VCP_TX" in PinLabel[EachPin]:
                PinLabel[EachPin] = "STDIO_UART_TX"
            elif "ST_LINK_UART1_RX" in PinLabel[EachPin]:
                PinLabel[EachPin] = "STDIO_UART_RX"
            elif "ST_LINK_UART1_TX" in PinLabel[EachPin]:
                PinLabel[EachPin] = "STDIO_UART_TX"
            elif "USART2_RX" in PinLabel[EachPin]:
                PinLabel[EachPin] = "STDIO_UART_RX"
            elif "USART2_TX" in PinLabel[EachPin]:
                PinLabel[EachPin] = "STDIO_UART_TX"
            elif "STLINK_RX" in PinLabel[EachPin] or "STLINK_TX" in PinLabel[EachPin]:
                # Patch waiting for CubeMX correction
                if "RX" in PinData[EachPin]["Signal"]:
                    PinLabel[EachPin] = "STDIO_UART_RX"
                else:
                    PinLabel[EachPin] = "STDIO_UART_TX"
        except:
            pass

# main
print ("\nScript version %s" % GENPINMAP_VERSION)
cur_dir = os.getcwd()
PeripheralPins_c_filename = 'PeripheralPins.c'
PinNames_h_filename = 'PinNames.h'
config_filename = 'cube_path.json'

try:
    config_file = open(config_filename, "r")
except IOError:
    print("Please set your configuration in '%s' file" % config_filename)
    config_file = open(config_filename, "w")
    if sys.platform.startswith('win32'):
        print("Platform is Windows")
        cubemxdir = 'C:\\Program Files (x86)\\STMicroelectronics\\STM32Cube\\STM32CubeMX'
    elif sys.platform.startswith('linux'):
        print("Platform is Linux")
        cubemxdir = os.getenv("HOME")+'/STM32CubeMX'
    elif sys.platform.startswith('darwin'):
        print("Platform is Mac OSX")
        cubemxdir = '/Applications/STMicroelectronics/STM32CubeMX.app/Contents/Resources'
    else:
        print("Platform unknown")
        cubemxdir = '<Set CubeMX install directory>'
    config_file.write(json.dumps({"CUBEMX_DIRECTORY":cubemxdir}))
    config_file.close()
    exit(1)

config = json.load(config_file)
config_file.close()
cubemxdir = config["CUBEMX_DIRECTORY"]

parser = argparse.ArgumentParser(
    description=textwrap.dedent('''\
Script will generate %s thanks to the xml files description available in
STM32CubeMX directory defined in '%s':
\t%s''' % (PeripheralPins_c_filename, config_filename, cubemxdir)),
    epilog=textwrap.dedent('''\
Once generated, you have to check and comment pins that can not be used (specific HW, internal ADC channels, remove PWM using us ticker timer, ...)
'''),
    formatter_class=RawTextHelpFormatter)
group = parser.add_mutually_exclusive_group()

group.add_argument("-l", "--list", help="list available mcu xml files description in STM32CubeMX", action="store_true")

group.add_argument("-b", "--boards", help="list available boards description in STM32CubeMX", action="store_true")

group.add_argument("-m", "--mcu", metavar='xml', help=textwrap.dedent('''\
specify the mcu xml file description in STM32CubeMX to use (use double quotes).
   Parameter can be a filter like L496 if you want to parse all L496 chips (-m STM32 to parse all).
'''))

group.add_argument("-t", "--target", metavar='HW', help=textwrap.dedent('''\
specify the board file description in STM32CubeMX to use (use double quotes).
   Parameter can be a filter like L496 (only the first file found will be parsed).
'''))

args = parser.parse_args()

if not(os.path.isdir(cubemxdir)):
    print ("\n ! ! ! Cube Mx seems not to be installed or not at the requested location")
    print ("\n ! ! ! please check the value you set for 'CUBEMX_DIRECTORY' in '%s' file" % config_filename)
    quit()

cubemxdirMCU = os.path.join(cubemxdir, 'db', 'mcu')
cubemxdirIP = os.path.join(cubemxdir, 'db', 'mcu', 'IP')
cubemxdirBOARDS = os.path.join(cubemxdir, 'db', 'plugins', 'boardmanager', 'boards')

version_file = os.path.join(cubemxdir, 'db', 'package.xml')
try:
    xml_file = parse(version_file)
    PackDescription_item = xml_file.getElementsByTagName('PackDescription')
    for item in PackDescription_item:
        CUBEMX_DB_VERSION = item.attributes['Release'].value
except:
    CUBEMX_DB_VERSION = "NOT_FOUND"
print ("CubeMX DB version %s\n" % CUBEMX_DB_VERSION)

if args.list:
    FileCount = 0
    for f in fnmatch.filter(os.listdir(cubemxdirMCU), 'STM32*.xml'):
        print(f)
        FileCount += 1
    print
    print("%i available xml files description" % FileCount)
    quit()

if args.boards:
    NucleoFileCount = 0
    DiscoFileCount = 0
    for f in fnmatch.filter(os.listdir(cubemxdirBOARDS), '*AllConfig.ioc'):
        print(f)
        if "Nucleo" in f:
            NucleoFileCount += 1
        elif "Discovery" in f:
            DiscoFileCount += 1
    print
    print("%2i available Nucleo files description" % NucleoFileCount)
    print("%2i available Disco  files description" % DiscoFileCount)
    quit()

if args.mcu:
    #check input file exists
    if os.path.isfile(os.path.join(cubemxdirMCU, args.mcu)):
        mcu_list.append(args.mcu)
    else:
        mcu_list = fnmatch.filter(os.listdir(cubemxdirMCU), '*%s*' % args.mcu)
        if len(mcu_list) == 0:
            print (" ! ! ! " + args.mcu + " file not found")
            print (" ! ! ! Check in " + cubemxdirMCU + " the correct name of this file")
            print (" ! ! ! You may use double quotes for this file if it contains special characters")
            quit()

if args.target:
    board_file_name = os.path.join(cubemxdirBOARDS, args.target)
    if not(os.path.isfile(board_file_name)):
        board_list = fnmatch.filter(os.listdir(cubemxdirBOARDS), '*%s*AllConfig.ioc' % args.target)
        if len(board_list) == 0:
            print (" ! ! ! No file contains " + args.target)
            print (" ! ! ! Check in " + cubemxdirBOARDS + " the correct filter to apply")
            quit()
        elif len(board_list) > 1:
            print (" ! ! ! Multiple files contains " + args.target)
            for board_elem in board_list: print (board_elem)
            print (" ! ! ! Only the first one will be parsed\n")
        board_file_name = os.path.join(cubemxdirBOARDS,board_list[0])
        if not (os.path.isfile(board_file_name)):
            print (" ! ! ! " + args.target + " file not found")
            print (" ! ! ! Check in " + cubemxdirBOARDS + " the correct name of this file")
            print (" ! ! ! You may use double quotes for this file if it contains special characters")
            quit()
    parse_BoardFile(board_file_name)
    TargetName = ""
    if "Nucleo" in board_file_name:
        TargetName += "NUCLEO_"
    elif "Discovery" in board_file_name:
        TargetName += "DISCO_"
    elif "Evaluation" in board_file_name:
        TargetName += "EVAL_"
    m = re.search(r'STM32([\w][\dR]{3}[\w]{0,2})[\w]*_Board', board_file_name)
    if m:
        TargetName += "%s" % m.group(1)
        # specific case
        if "-P" in args.target:
            TargetName += "_P"
        if TargetName == "DISCO_L072":
            TargetName += "CZ_LRWAN1"
        if TargetName == "DISCO_L475V":
            TargetName += "G_IOT01A"
    else:
        quit()

for mcu_file in mcu_list:
    if args.mcu:
        TargetName = os.path.splitext(mcu_file)[0]
    out_path = os.path.join(cur_dir, '%s' % TargetName)
    print(" * Output directory: %s" % (out_path))
    print(" * Generating %s and %s with '%s'" % (PeripheralPins_c_filename, PinNames_h_filename, mcu_file))
    input_file_name = os.path.join(cubemxdirMCU, mcu_file)
    output_cfilename = os.path.join(out_path, PeripheralPins_c_filename)
    output_hfilename = os.path.join(out_path, PinNames_h_filename)
    if not(os.path.isdir(out_path)):
        os.makedirs(out_path)

    if (os.path.isfile(output_cfilename)):
        # print (" * Requested %s file already exists and will be overwritten" % PeripheralPins_c_filename)
        os.remove(output_cfilename)
    out_c_file = open(output_cfilename, 'w')
    out_h_file = open(output_hfilename, 'w')

    #open input file
    try:
        xml_mcu = parse(input_file_name)
    except:
        # Patch waiting for CubeMX correction
        if "STM32F042K6Tx" in input_file_name:
            input_file_name = os.path.join(cubemxdirMCU, "STM32F042K(4-6)Tx.xml")
            xml_mcu = parse(input_file_name)
        elif "STM32F429Z" in input_file_name:
            input_file_name = os.path.join(cubemxdirMCU, "STM32F429ZITx.xml")
            xml_mcu = parse(input_file_name)
        elif "STM32F746Z" in input_file_name:
            input_file_name = os.path.join(cubemxdirMCU, "STM32F746ZGTx.xml")
            xml_mcu = parse(input_file_name)
        elif "STM32F767Z" in input_file_name:
            input_file_name = os.path.join(cubemxdirMCU, "STM32F767ZGTx.xml")
            xml_mcu = parse(input_file_name)

        elif "STM32L011K4Tx" in input_file_name:
            input_file_name = os.path.join(cubemxdirMCU, "STM32L011K(3-4)Tx.xml")
            xml_mcu = parse(input_file_name)
        elif "STM32L432KCUx" in input_file_name:
            input_file_name = os.path.join(cubemxdirMCU, "STM32L432K(B-C)Ux.xml")
            xml_mcu = parse(input_file_name)
        elif "STM32F746N" in input_file_name:
            input_file_name = os.path.join(cubemxdirMCU, "STM32F746NGHx.xml")
            xml_mcu = parse(input_file_name)
        else:
            print ("\n ! ! ! Error in CubeMX file. File " + input_file_name + " doesn't exist")
            print (" ! ! ! Check in " + cubemxdirMCU)
            quit()
    gpiofile = find_gpio_file()
    if gpiofile == 'ERROR':
        print("Could not find GPIO file")
        quit()
    xml_gpio = parse(os.path.join(cubemxdirIP, 'GPIO-' + gpiofile + '_Modes.xml'))
    print (" * GPIO file: " + os.path.join(cubemxdirIP, 'GPIO-' + gpiofile + '_Modes.xml'))

    parse_pins()
    sort_my_lists()
    print_header()
    print_all_lists()
    print_footer()

    nb_pin = (len(io_list))
    nb_connected_pin = len(PinLabel)
    print (" * I/O pins found: %i connected: %i\n" % (nb_pin, nb_connected_pin))
    clean_all_lists()

    out_c_file.close()
    out_h_file.close()
