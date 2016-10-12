/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include "device.h"
#include "PeripheralPins.h"
#include "ioman_regs.h"

/*
 * To select a peripheral function on Maxim microcontrollers, multiple
 * configurations must be made. The mbed PinMap structure only includes one
 * data member to hold this information. To extend the configuration storage,
 * the "function" data member is used as a pointer to a pin_function_t
 * structure. This structure is defined in objects.h. The definitions below
 * include the creation of the pin_function_t structures and the assignment of
 * the pointers to the "function" data members.
 */

#ifdef TOOLCHAIN_ARM_STD
#pragma diag_suppress 1296
#endif

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    { P1_6,  I2C_0, (int)&((pin_function_t){&MXC_IOMAN->i2cm0_req, &MXC_IOMAN->i2cm0_ack, MXC_F_IOMAN_I2CM_REQ_MAPPING_REQ, MXC_F_IOMAN_I2CM_ACK_MAPPING_ACK}) },
    { P3_4,  I2C_1, (int)&((pin_function_t){&MXC_IOMAN->i2cm1_req, &MXC_IOMAN->i2cm1_ack, MXC_F_IOMAN_I2CM_REQ_MAPPING_REQ, MXC_F_IOMAN_I2CM_ACK_MAPPING_ACK}) },
    { P5_7,  I2C_2, (int)&((pin_function_t){&MXC_IOMAN->i2cm2_req, &MXC_IOMAN->i2cm2_ack, MXC_F_IOMAN_I2CM_REQ_MAPPING_REQ, MXC_F_IOMAN_I2CM_ACK_MAPPING_ACK}) },
    { NC,    NC,    0 }
};

const PinMap PinMap_I2C_SCL[] = {
    { P1_7,  I2C_0, (int)&((pin_function_t){&MXC_IOMAN->i2cm0_req, &MXC_IOMAN->i2cm0_ack, MXC_F_IOMAN_I2CM_REQ_MAPPING_REQ, MXC_F_IOMAN_I2CM_ACK_MAPPING_ACK}) },
    { P3_5,  I2C_1, (int)&((pin_function_t){&MXC_IOMAN->i2cm1_req, &MXC_IOMAN->i2cm1_ack, MXC_F_IOMAN_I2CM_REQ_MAPPING_REQ, MXC_F_IOMAN_I2CM_ACK_MAPPING_ACK}) },
    { P6_0,  I2C_2, (int)&((pin_function_t){&MXC_IOMAN->i2cm2_req, &MXC_IOMAN->i2cm2_ack, MXC_F_IOMAN_I2CM_REQ_MAPPING_REQ, MXC_F_IOMAN_I2CM_ACK_MAPPING_ACK}) },
    { NC,    NC,    0 }
};

/************UART***************/
/*
*/
const PinMap PinMap_UART_TX[] = {
    { P0_1, UART_0, (int)&((pin_function_t){&MXC_IOMAN->uart0_req, &MXC_IOMAN->uart0_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P2_1, UART_1, (int)&((pin_function_t){&MXC_IOMAN->uart1_req, &MXC_IOMAN->uart1_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P3_1, UART_2, (int)&((pin_function_t){&MXC_IOMAN->uart2_req, &MXC_IOMAN->uart2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P5_4, UART_3, (int)&((pin_function_t){&MXC_IOMAN->uart3_req, &MXC_IOMAN->uart3_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P0_0, UART_0, (int)&((pin_function_t){&MXC_IOMAN->uart0_req, &MXC_IOMAN->uart0_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P2_0, UART_1, (int)&((pin_function_t){&MXC_IOMAN->uart1_req, &MXC_IOMAN->uart1_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P3_0, UART_2, (int)&((pin_function_t){&MXC_IOMAN->uart2_req, &MXC_IOMAN->uart2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P5_3, UART_3, (int)&((pin_function_t){&MXC_IOMAN->uart3_req, &MXC_IOMAN->uart3_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { NC,   NC,     0 }
};

const PinMap PinMap_UART_RX[] = {
    { P0_0, UART_0, (int)&((pin_function_t){&MXC_IOMAN->uart0_req, &MXC_IOMAN->uart0_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P2_0, UART_1, (int)&((pin_function_t){&MXC_IOMAN->uart1_req, &MXC_IOMAN->uart1_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P3_0, UART_2, (int)&((pin_function_t){&MXC_IOMAN->uart2_req, &MXC_IOMAN->uart2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P5_3, UART_3, (int)&((pin_function_t){&MXC_IOMAN->uart3_req, &MXC_IOMAN->uart3_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P0_1, UART_0, (int)&((pin_function_t){&MXC_IOMAN->uart0_req, &MXC_IOMAN->uart0_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P2_1, UART_1, (int)&((pin_function_t){&MXC_IOMAN->uart1_req, &MXC_IOMAN->uart1_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P3_1, UART_2, (int)&((pin_function_t){&MXC_IOMAN->uart2_req, &MXC_IOMAN->uart2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { P5_4, UART_3, (int)&((pin_function_t){&MXC_IOMAN->uart3_req, &MXC_IOMAN->uart3_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_IO_REQ), (MXC_F_IOMAN_UART_ACK_IO_MAP | MXC_F_IOMAN_UART_ACK_IO_ACK)}) },
    { NC,   NC,     0 }
};

const PinMap PinMap_UART_CTS[] = {
    { P0_2, UART_0, (int)&((pin_function_t){&MXC_IOMAN->uart0_req, &MXC_IOMAN->uart0_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_CTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_CTS_MAP | MXC_F_IOMAN_UART_ACK_CTS_IO_ACK)}) },
    { P2_2, UART_1, (int)&((pin_function_t){&MXC_IOMAN->uart1_req, &MXC_IOMAN->uart1_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_CTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_CTS_MAP | MXC_F_IOMAN_UART_ACK_CTS_IO_ACK)}) },
    { P3_2, UART_2, (int)&((pin_function_t){&MXC_IOMAN->uart2_req, &MXC_IOMAN->uart2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_CTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_CTS_MAP | MXC_F_IOMAN_UART_ACK_CTS_IO_ACK)}) },
    { P5_5, UART_3, (int)&((pin_function_t){&MXC_IOMAN->uart3_req, &MXC_IOMAN->uart3_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_CTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_CTS_MAP | MXC_F_IOMAN_UART_ACK_CTS_IO_ACK)}) },
    { P0_3, UART_0, (int)&((pin_function_t){&MXC_IOMAN->uart0_req, &MXC_IOMAN->uart0_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_CTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_CTS_MAP | MXC_F_IOMAN_UART_ACK_CTS_IO_ACK)}) },
    { P2_3, UART_1, (int)&((pin_function_t){&MXC_IOMAN->uart1_req, &MXC_IOMAN->uart1_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_CTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_CTS_MAP | MXC_F_IOMAN_UART_ACK_CTS_IO_ACK)}) },
    { P3_3, UART_2, (int)&((pin_function_t){&MXC_IOMAN->uart2_req, &MXC_IOMAN->uart2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_CTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_CTS_MAP | MXC_F_IOMAN_UART_ACK_CTS_IO_ACK)}) },
    { P5_6, UART_3, (int)&((pin_function_t){&MXC_IOMAN->uart3_req, &MXC_IOMAN->uart3_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_CTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_CTS_MAP | MXC_F_IOMAN_UART_ACK_CTS_IO_ACK)}) },
    { NC,   NC,     0 }
};

const PinMap PinMap_UART_RTS[] = {
    { P0_3, UART_0, (int)&((pin_function_t){&MXC_IOMAN->uart0_req, &MXC_IOMAN->uart0_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_RTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_RTS_MAP | MXC_F_IOMAN_UART_ACK_RTS_IO_ACK)}) },
    { P2_3, UART_1, (int)&((pin_function_t){&MXC_IOMAN->uart1_req, &MXC_IOMAN->uart1_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_RTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_RTS_MAP | MXC_F_IOMAN_UART_ACK_RTS_IO_ACK)}) },
    { P3_3, UART_2, (int)&((pin_function_t){&MXC_IOMAN->uart2_req, &MXC_IOMAN->uart2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_RTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_RTS_MAP | MXC_F_IOMAN_UART_ACK_RTS_IO_ACK)}) },
    { P5_6, UART_3, (int)&((pin_function_t){&MXC_IOMAN->uart3_req, &MXC_IOMAN->uart3_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_UART_REQ_RTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_RTS_MAP | MXC_F_IOMAN_UART_ACK_RTS_IO_ACK)}) },
    { P0_2, UART_0, (int)&((pin_function_t){&MXC_IOMAN->uart0_req, &MXC_IOMAN->uart0_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_RTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_RTS_MAP | MXC_F_IOMAN_UART_ACK_RTS_IO_ACK)}) },
    { P2_2, UART_1, (int)&((pin_function_t){&MXC_IOMAN->uart1_req, &MXC_IOMAN->uart1_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_RTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_RTS_MAP | MXC_F_IOMAN_UART_ACK_RTS_IO_ACK)}) },
    { P3_2, UART_2, (int)&((pin_function_t){&MXC_IOMAN->uart2_req, &MXC_IOMAN->uart2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_RTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_RTS_MAP | MXC_F_IOMAN_UART_ACK_RTS_IO_ACK)}) },
    { P5_5, UART_3, (int)&((pin_function_t){&MXC_IOMAN->uart3_req, &MXC_IOMAN->uart3_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_UART_REQ_RTS_IO_REQ), (MXC_F_IOMAN_UART_ACK_RTS_MAP | MXC_F_IOMAN_UART_ACK_RTS_IO_ACK)}) },
    { NC,   NC,     0 }
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    { P0_4, SPI_0, (int)&((pin_function_t){&MXC_IOMAN->spim0_req, &MXC_IOMAN->spim0_ack, MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ, MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK}) },
    { P1_0, SPI_1, (int)&((pin_function_t){&MXC_IOMAN->spim1_req, &MXC_IOMAN->spim1_ack, MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ, MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK}) },
    { P2_4, SPI_2, (int)&((pin_function_t){&MXC_IOMAN->spim2_req, &MXC_IOMAN->spim2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ), (MXC_F_IOMAN_SPIM_REQ_MAPPING_REQ | MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK)}) },
    { P5_0, SPI_2, (int)&((pin_function_t){&MXC_IOMAN->spim2_req, &MXC_IOMAN->spim2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ), (MXC_F_IOMAN_SPIM_REQ_MAPPING_REQ | MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK)}) },
    { NC,   NC,    0 }
};

const PinMap PinMap_SPI_MOSI[] = {
    { P0_5, SPI_0, (int)&((pin_function_t){&MXC_IOMAN->spim0_req, &MXC_IOMAN->spim0_ack, MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ, MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK}) },
    { P1_1, SPI_1, (int)&((pin_function_t){&MXC_IOMAN->spim1_req, &MXC_IOMAN->spim1_ack, MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ, MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK}) },
    { P2_5, SPI_2, (int)&((pin_function_t){&MXC_IOMAN->spim2_req, &MXC_IOMAN->spim2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ), (MXC_F_IOMAN_SPIM_REQ_MAPPING_REQ | MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK)}) },
    { P5_1, SPI_2, (int)&((pin_function_t){&MXC_IOMAN->spim2_req, &MXC_IOMAN->spim2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ), (MXC_F_IOMAN_SPIM_REQ_MAPPING_REQ | MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK)}) },
    { NC,   NC,    0 }
};

const PinMap PinMap_SPI_MISO[] = {
    { P0_6, SPI_0, (int)&((pin_function_t){&MXC_IOMAN->spim0_req, &MXC_IOMAN->spim0_ack, MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ, MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK}) },
    { P1_2, SPI_1, (int)&((pin_function_t){&MXC_IOMAN->spim1_req, &MXC_IOMAN->spim1_ack, MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ, MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK}) },
    { P2_6, SPI_2, (int)&((pin_function_t){&MXC_IOMAN->spim2_req, &MXC_IOMAN->spim2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ), (MXC_F_IOMAN_SPIM_REQ_MAPPING_REQ | MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK)}) },
    { P5_2, SPI_2, (int)&((pin_function_t){&MXC_IOMAN->spim2_req, &MXC_IOMAN->spim2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ), (MXC_F_IOMAN_SPIM_REQ_MAPPING_REQ | MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK)}) },
    { NC,   NC,    0 }
};

const PinMap PinMap_SPI_SSEL[] = {
    { P0_7, SPI_0, (int)&((pin_function_t){&MXC_IOMAN->spim0_req, &MXC_IOMAN->spim0_ack, MXC_F_IOMAN_SPIM_REQ_SS0_IO_REQ, MXC_F_IOMAN_SPIM_ACK_SS0_IO_ACK}) },
    { P1_3, SPI_1, (int)&((pin_function_t){&MXC_IOMAN->spim1_req, &MXC_IOMAN->spim1_ack, MXC_F_IOMAN_SPIM_REQ_SS0_IO_REQ, MXC_F_IOMAN_SPIM_ACK_SS0_IO_ACK}) },
    { P2_7, SPI_2, (int)&((pin_function_t){&MXC_IOMAN->spim2_req, &MXC_IOMAN->spim2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_A | MXC_F_IOMAN_SPIM_REQ_SS0_IO_REQ), (MXC_F_IOMAN_SPIM_REQ_MAPPING_REQ | MXC_F_IOMAN_SPIM_ACK_SS0_IO_ACK)}) },
    { P5_3, SPI_2, (int)&((pin_function_t){&MXC_IOMAN->spim2_req, &MXC_IOMAN->spim2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_SPIM_REQ_SS0_IO_REQ | MXC_F_IOMAN_SPIM_REQ_SR0_IO_REQ), (MXC_F_IOMAN_SPIM_REQ_MAPPING_REQ | MXC_F_IOMAN_SPIM_ACK_SS0_IO_ACK | MXC_F_IOMAN_SPIM_ACK_SR0_IO_ACK)}) },
    { NC,   NC,    0 }
};

const PinMap PinMap_SPI_QUAD[] = {
    { P0_4, SPI_0, (int)&((pin_function_t){&MXC_IOMAN->spim0_req, &MXC_IOMAN->spim0_ack, MXC_F_IOMAN_SPIM_REQ_QUAD_IO_REQ, MXC_F_IOMAN_SPIM_ACK_QUAD_IO_ACK}) },
    { P1_0, SPI_1, (int)&((pin_function_t){&MXC_IOMAN->spim1_req, &MXC_IOMAN->spim1_ack, MXC_F_IOMAN_SPIM_REQ_QUAD_IO_REQ, MXC_F_IOMAN_SPIM_ACK_QUAD_IO_ACK}) },
    { P5_0, SPI_2, (int)&((pin_function_t){&MXC_IOMAN->spim2_req, &MXC_IOMAN->spim2_ack, ((uint32_t)MXC_E_IOMAN_MAPPING_B | MXC_F_IOMAN_SPIM_REQ_CORE_IO_REQ | MXC_F_IOMAN_SPIM_REQ_QUAD_IO_REQ), (MXC_F_IOMAN_SPIM_REQ_MAPPING_REQ | MXC_F_IOMAN_SPIM_ACK_CORE_IO_ACK | MXC_F_IOMAN_SPIM_ACK_QUAD_IO_ACK)}) },
    { NC,   NC,    0 }
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    { P0_0, PWM_0,  1 }, { P2_0, PWM_0,  1 }, { P4_0, PWM_0,  1 }, {P6_0, PWM_0, 1},
    { P0_1, PWM_1,  1 }, { P2_1, PWM_1,  1 }, { P4_1, PWM_1,  1 },
    { P0_2, PWM_2,  1 }, { P2_2, PWM_2,  1 }, { P4_2, PWM_2,  1 },
    { P0_3, PWM_3,  1 }, { P2_3, PWM_3,  1 }, { P4_3, PWM_3,  1 },
    { P0_4, PWM_4,  1 }, { P2_4, PWM_4,  1 }, { P4_4, PWM_4,  1 },
    { P0_5, PWM_5,  1 }, { P2_5, PWM_5,  1 }, { P4_5, PWM_5,  1 },
    { P0_6, PWM_6,  1 }, { P2_6, PWM_6,  1 }, { P4_6, PWM_6,  1 },
    { P0_7, PWM_7,  1 }, { P2_7, PWM_7,  1 }, { P4_7, PWM_7,  1 },
    { P1_0, PWM_8,  1 }, { P3_0, PWM_8,  1 }, { P5_0, PWM_8,  1 },
    { P1_1, PWM_9,  1 }, { P3_1, PWM_9,  1 }, { P5_1, PWM_9,  1 },
    { P1_2, PWM_10, 1 }, { P3_2, PWM_10, 1 }, { P5_2, PWM_10, 1 },
    { P1_3, PWM_11, 1 }, { P3_3, PWM_11, 1 }, { P5_3, PWM_11, 1 },
    { P1_4, PWM_12, 1 }, { P3_4, PWM_12, 1 }, { P5_4, PWM_12, 1 },
    { P1_5, PWM_13, 1 }, { P3_5, PWM_13, 1 }, { P5_5, PWM_13, 1 },
    { P1_6, PWM_14, 1 }, { P3_6, PWM_14, 1 }, { P5_6, PWM_14, 1 },
    { P1_7, PWM_15, 1 }, { P3_7, PWM_15, 1 }, { P5_7, PWM_15, 1 },
    { NC,   NC,    0 }
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    { AIN_0, ADC, 0 },
    { AIN_1, ADC, 0 },
    { AIN_2, ADC, 0 },
    { AIN_3, ADC, 0 },
    { NC,    NC,  0 }
};
