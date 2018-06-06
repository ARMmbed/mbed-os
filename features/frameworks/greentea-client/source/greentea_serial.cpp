#include "greentea-client/greentea_serial.h"

/**
 * Macros for setting console flow control.
 */
#define CONSOLE_FLOWCONTROL_RTS     1
#define CONSOLE_FLOWCONTROL_CTS     2
#define CONSOLE_FLOWCONTROL_RTSCTS  3
#define mbed_console_concat_(x) CONSOLE_FLOWCONTROL_##x
#define mbed_console_concat(x) mbed_console_concat_(x)
#define CONSOLE_FLOWCONTROL mbed_console_concat(MBED_CONF_TARGET_CONSOLE_UART_FLOW_CONTROL)

SingletonPtr<GreenteaSerial> greentea_serial;

GreenteaSerial::GreenteaSerial() : mbed::RawSerial(USBTX, USBRX, MBED_CONF_PLATFORM_STDIO_BAUD_RATE) {
#if   CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_RTS
    set_flow_control(SerialBase::RTS, STDIO_UART_RTS, NC);
#elif CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_CTS
    set_flow_control(SerialBase::CTS, NC, STDIO_UART_CTS);
#elif CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_RTSCTS
    set_flow_control(SerialBase::RTSCTS, STDIO_UART_RTS, STDIO_UART_CTS);
#endif
}
