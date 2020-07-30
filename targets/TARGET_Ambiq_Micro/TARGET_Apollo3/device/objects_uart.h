/*
Copyright (c) 2019 SparkFun Electronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MBED_OBJECTS_UART_H
#define MBED_OBJECTS_UART_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint32_t ap3_uart_pad_t;
typedef uint32_t ap3_uart_inst_t;

typedef struct _ap3_uart_pad_map_elem_t {
	ap3_uart_pad_t pad;
	uint8_t funcsel;
} ap3_uart_pad_map_elem_t;

typedef struct _ap3_uart_control_t
{
	ap3_uart_inst_t inst;		// UART module instance
	void *handle;				// UART handle
	am_hal_uart_config_t cfg;	// UART configuration
	uint32_t serial_irq_id;
} ap3_uart_control_t;

#if DEVICE_SERIAL_ASYNCH
struct serial_s {
	ap3_uart_control_t *uart_control;
};
#else
struct serial_u {
	ap3_uart_control_t *uart_control;
};
struct serial_s {
	struct serial_u serial;
};
#endif // DEVICE_SERIAL_ASYNCH

#ifdef __cplusplus
}
#endif

#endif // MBED_OBJECTS_UART_H
