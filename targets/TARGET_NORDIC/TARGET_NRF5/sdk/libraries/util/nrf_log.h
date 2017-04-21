#ifndef NRF_LOG_H_
#define NRF_LOG_H_

#ifndef DOXYGEN

#include <stdint.h>
#include <stdarg.h>
#include <app_util.h>

#ifndef NRF_LOG_USES_RTT
#define NRF_LOG_USES_RTT 0
#endif

#ifndef NRF_LOG_USES_UART
#define NRF_LOG_USES_UART 0
#endif

#ifndef NRF_LOG_USES_RAW_UART
#define NRF_LOG_USES_RAW_UART 0
#endif

#ifndef NRF_LOG_USES_COLORS
    #define NRF_LOG_USES_COLORS 1
#endif

#if NRF_LOG_USES_COLORS == 1
    #define NRF_LOG_COLOR_DEFAULT  "\x1B[0m"
    #define NRF_LOG_COLOR_BLACK    "\x1B[1;30m"
    #define NRF_LOG_COLOR_RED      "\x1B[1;31m"
    #define NRF_LOG_COLOR_GREEN    "\x1B[1;32m"
    #define NRF_LOG_COLOR_YELLOW   "\x1B[1;33m"
    #define NRF_LOG_COLOR_BLUE     "\x1B[1;34m"
    #define NRF_LOG_COLOR_MAGENTA  "\x1B[1;35m"
    #define NRF_LOG_COLOR_CYAN     "\x1B[1;36m"
    #define NRF_LOG_COLOR_WHITE    "\x1B[1;37m"
#else
    #define NRF_LOG_COLOR_DEFAULT
    #define NRF_LOG_COLOR_BLACK
    #define NRF_LOG_COLOR_RED
    #define NRF_LOG_COLOR_GREEN
    #define NRF_LOG_COLOR_YELLOW
    #define NRF_LOG_COLOR_BLUE
    #define NRF_LOG_COLOR_MAGENTA
    #define NRF_LOG_COLOR_CYAN
    #define NRF_LOG_COLOR_WHITE
#endif

#if defined(NRF_LOG_USES_RTT) && NRF_LOG_USES_RTT == 1

#define LOG_TERMINAL_NORMAL         (0)
#define LOG_TERMINAL_ERROR          (1)
#define LOG_TERMINAL_INPUT          (0)

/**@brief Function for initializing the SEGGER RTT logger.
 *
 * @details See <a href="https://www.segger.com/jlink-rtt.html" target="_blank">segger.com</a>
 *          for information about SEGGER Real Time Transfer (RTT).
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use the macro @ref NRF_LOG_INIT instead.
 *
 * @retval     NRF_SUCCESS     If initialization was successful.
 * @retval     NRF_ERROR       Otherwise.
 */
uint32_t log_rtt_init(void);

/**@brief Function for writing a printf string using RTT.
 *
 * @details The printf implementation in SEGGER's RTT is more efficient than
 * the standard implementation. However, printf requires more processor time
 * than other logging functions. Therefore, applications that require logging
 * but need it to interfere as little as possible with the execution, should
 * avoid using printf.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_PRINTF
 * - @ref NRF_LOG_PRINTF_DEBUG
 * - @ref NRF_LOG_PRINTF_ERROR
 *
 * @param   terminal_index  Segger RTT terminal index to use as output.
 * @param   format_msg      Printf format string.
 */
void log_rtt_printf(int terminal_index, char * format_msg, ...);

/**@brief Function for writing a string using RTT.
 *
 * @details The string to write must be null-terminated, but the null termination will not be stored
 *          in the ring buffer.
 *          The impact of running this function should be very low compared to writing to UART.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG
 * - @ref NRF_LOG_DEBUG
 * - @ref NRF_LOG_ERROR
 *
 * @param   terminal_index  Segger RTT terminal index to use as output.
 * @param   num_args        Number of arguments.
 */
void log_rtt_write_string(int terminal_index, int num_args, ...);

/**@brief Function for writing an integer as HEX using RTT.
 *
 * The output data is formatted as, for example, 0x89ABCDEF.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_HEX
 * - @ref NRF_LOG_HEX_DEBUG
 * - @ref NRF_LOG_HEX_ERROR
 *
 * @param   terminal_index  Segger RTT terminal index to use as output.
 * @param   value           Integer value to be printed as HEX.
 */
void log_rtt_write_hex(int terminal_index, uint32_t value);

/**@brief Function for writing a single character as HEX using RTT.
 *
 * The output string is formatted as, for example, AA.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_HEX_CHAR
 * - @ref NRF_LOG_HEX_CHAR_DEBUG
 * - @ref NRF_LOG_HEX_CHAR_ERROR
 *
 * @param   terminal_index  Segger RTT terminal index to use as output.
 * @param   value           Character to print as HEX.
 */
void log_rtt_write_hex_char(int terminal_index, uint8_t value);

/**@brief Function for checking if data is available in the input buffer.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use @ref NRF_LOG_HAS_INPUT instead.
 *
 * @retval      1 If characters are available to read.
 * @retval      0 If no characters are available.
 */
int log_rtt_has_input(void);

/**@brief Function for reading one character from the input buffer.
 *
 * @param[out] p_char  Pointer where to store the character.
 *
 * This function is available only when NRF_LOG_USES_RTT is defined as 1.
 *
 * @note Do not call this function directly. Use @ref NRF_LOG_READ_INPUT instead.
 *
 * @retval      NRF_SUCCESS If the character was read out.
 * @retval      NRF_ERROR_INVALID_DATA If no character could be read.
 */
uint32_t log_rtt_read_input(char* p_char);

#define NRF_LOG_INIT()                  log_rtt_init()                                                                          /*!< Initialize the module. */

#define NRF_LOG_PRINTF(...)             log_rtt_printf(LOG_TERMINAL_NORMAL, ##__VA_ARGS__)                                      /*!< Print a log message using printf. */
#define NRF_LOG_PRINTF_DEBUG(...)       log_rtt_printf(LOG_TERMINAL_NORMAL, ##__VA_ARGS__)                                      /*!< If DEBUG is set, print a log message using printf. */
#define NRF_LOG_PRINTF_ERROR(...)       log_rtt_printf(LOG_TERMINAL_ERROR, ##__VA_ARGS__)                                       /*!< Print a log message using printf to the error stream. */

#define NRF_LOG(...)                    log_rtt_write_string(LOG_TERMINAL_NORMAL, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)      /*!< Print a log message. The input string must be null-terminated. */
#define NRF_LOG_DEBUG(...)              log_rtt_write_string(LOG_TERMINAL_NORMAL, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)      /*!< If DEBUG is set, print a log message. The input string must be null-terminated. */
#define NRF_LOG_ERROR(...)              log_rtt_write_string(LOG_TERMINAL_ERROR, NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)       /*!< Print a log message to the error stream. The input string must be null-terminated. */

#define NRF_LOG_HEX(val)                log_rtt_write_hex(LOG_TERMINAL_NORMAL, val)                                             /*!< Log an integer as HEX value (example output: 0x89ABCDEF). */
#define NRF_LOG_HEX_DEBUG(val)          log_rtt_write_hex(LOG_TERMINAL_NORMAL, val)                                             /*!< If DEBUG is set, log an integer as HEX value (example output: 0x89ABCDEF). */
#define NRF_LOG_HEX_ERROR(val)          log_rtt_write_hex(LOG_TERMINAL_ERROR, val)                                              /*!< Log an integer as HEX value to the error stream (example output: 0x89ABCDEF). */

#define NRF_LOG_HEX_CHAR(val)           log_rtt_write_hex_char(LOG_TERMINAL_NORMAL, val)                                        /*!< Log a character as HEX value (example output: AA). */
#define NRF_LOG_HEX_CHAR_DEBUG(val)     log_rtt_write_hex_char(LOG_TERMINAL_NORMAL, val)                                        /*!< If DEBUG is set, log a character as HEX value (example output: AA). */
#define NRF_LOG_HEX_CHAR_ERROR(val)     log_rtt_write_hex_char(LOG_TERMINAL_ERROR, val)                                         /*!< Log a character as HEX value to the error stream (example output: AA). */

#define NRF_LOG_HAS_INPUT()             log_rtt_has_input()                                                                     /*!< Check if the input buffer has unconsumed characters. */
#define NRF_LOG_READ_INPUT(p_char)      log_rtt_read_input(p_char)                                                              /*!< Consume a character from the input buffer. */

#if !defined(DEBUG) && !defined(DOXYGEN)

#undef NRF_LOG_DEBUG
#define NRF_LOG_DEBUG(...)

#undef NRF_LOG_STR_DEBUG
#define NRF_LOG_STR_DEBUG(...)

#undef NRF_LOG_HEX_DEBUG
#define NRF_LOG_HEX_DEBUG(...)

#undef NRF_LOG_HEX_CHAR_DEBUG
#define NRF_LOG_HEX_CHAR_DEBUG(...)

#endif // !defined(DEBUG) && !defined(DOXYGEN)

#elif defined(NRF_LOG_USES_UART) && NRF_LOG_USES_UART == 1

/**@brief Function for initializing the UART logger.
 *
 * This function is available only when NRF_LOG_USES_UART is defined as 1.
 *
 * @note Do not call this function directly. Use the macro @ref NRF_LOG_INIT instead.
 *
 * @retval     NRF_SUCCESS     If initialization was successful.
 * @retval     NRF_ERROR       Otherwise.
 */
uint32_t log_uart_init(void);

/**@brief Function for logging a printf string to UART.
 *
 * @details  Printf requires more processor time
 * than other logging functions. Therefore, applications that require logging
 * but need it to interfere as little as possible with the execution, should
 * avoid using printf.
 *
 * This function is available only when NRF_LOG_USES_UART is defined as 1.
 *
 * @note This function is non-blocking. If too much data is sent to the UART,
 *          some characters might be skipped.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_PRINTF
 * - @ref NRF_LOG_PRINTF_DEBUG
 * - @ref NRF_LOG_PRINTF_ERROR
 *
 * @param  format_msg      Printf format string.
 */
void log_uart_printf(const char * format_msg, ...);

/**@brief Function for logging a single character to UART.
 *
 * This function is available only when NRF_LOG_USES_UART is defined as 1.
 *
 * @param  c     Character.
 */
void log_uart_write_char(const char c);

/**@brief Function for logging null-terminated strings to UART.
 *
 * @details This function is more efficient than using printf.
 *          The null termination will not be logged.
 *
 * This function is available only when NRF_LOG_USES_UART is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG
 * - @ref NRF_LOG_DEBUG
 * - @ref NRF_LOG_ERROR
 *
 * @param  num_args     Number of arguments.
 */
void log_uart_write_string_many(int num_args, ...);


/**@brief Function for logging a null-terminated string to UART.
 *
 * @details This function is more efficient than using printf.
 *          The null termination will not be logged.
 *
 * This function is available only when NRF_LOG_USES_UART is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG
 * - @ref NRF_LOG_DEBUG
 * - @ref NRF_LOG_ERROR
 *
 * @param  msg     Null-terminated string.
 */
void log_uart_write_string(const char* msg);


/**@brief Function for logging an integer value as HEX to UART.
 *
 * @details The output data is formatted as, for example, 0x89ABCDEF.
 * This function is more efficient than printf.
 *
 * This function is available only when NRF_LOG_USES_UART is defined as 1.
 *
 * @note This function is non-blocking. If too much data is sent to the UART,
 *          some characters might be skipped.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_HEX
 * - @ref NRF_LOG_HEX_DEBUG
 * - @ref NRF_LOG_HEX_ERROR
 *
 * @param   value   Integer value to be printed as HEX.
 */
void log_uart_write_hex(uint32_t value);

/**@brief Function for logging a single character as HEX to UART.
 *
 * @details The output string is formatted as, for example, AA.
 *
 * This function is available only when NRF_LOG_USES_UART is defined as 1.
 *
 * @note This function is non-blocking. If too much data is sent to the UART,
 *          some characters might be skipped.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_HEX_CHAR
 * - @ref NRF_LOG_HEX_CHAR_DEBUG
 * - @ref NRF_LOG_HEX_CHAR_ERROR
 *
 * @param c Character.
  */
void log_uart_write_hex_char(uint8_t c);

/**@brief Function for checking if data is available in the input buffer.
 *
 * This function is available only when NRF_LOG_USES_UART is defined as 1.
 *
 * @note Do not call this function directly. Use @ref NRF_LOG_HAS_INPUT instead.
 *
 * @retval      1 If characters are available to read.
 * @retval      0 If no characters are available.
 */
int log_uart_has_input(void);

/**@brief Function for reading one character from the input buffer.
 *
 * @param[out]   p_char  Pointer where to store the character.
 *
 * This function is available only when NRF_LOG_USES_UART is defined as 1.
 *
 * @note Do not call this function directly. Use NRF_LOG_READ_INPUT instead.
 *
 * @retval      NRF_SUCCESS If the character was read out.
 * @retval      NRF_ERROR_INVALID_DATA If no character could be read.
 */
uint32_t log_uart_read_input(char* p_char);


#define NRF_LOG_INIT()                  log_uart_init()                                                         /*!< Initialize the module. */

#define NRF_LOG_PRINTF(...)             log_uart_printf(__VA_ARGS__)                                            /*!< Print a log message using printf. */
#define NRF_LOG_PRINTF_DEBUG(...)       log_uart_printf(__VA_ARGS__)                                            /*!< If DEBUG is set, print a log message using printf. */
#define NRF_LOG_PRINTF_ERROR(...)       log_uart_printf(__VA_ARGS__)                                            /*!< Print a log message using printf to the error stream. */

#define NRF_LOG(...)                    log_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< Print a log message. The input string must be null-terminated. */
#define NRF_LOG_DEBUG(...)              log_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< If DEBUG is set, print a log message. The input string must be null-terminated. */
#define NRF_LOG_ERROR(...)              log_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< Print a log message to the error stream. The input string must be null-terminated. */

#define NRF_LOG_HEX(val)                log_uart_write_hex(val)                                                 /*!< Log an integer as HEX value (example output: 0x89ABCDEF). */
#define NRF_LOG_HEX_DEBUG(val)          log_uart_write_hex(val)                                                 /*!< If DEBUG is set, log an integer as HEX value (example output: 0x89ABCDEF). */
#define NRF_LOG_HEX_ERROR(val)          log_uart_write_hex(val)                                                 /*!< Log an integer as HEX value to the error stream (example output: 0x89ABCDEF). */

#define NRF_LOG_HEX_CHAR(val)           log_uart_write_hex_char(val)                                            /*!< Log a character as HEX value (example output: AA). */
#define NRF_LOG_HEX_CHAR_DEBUG(val)     log_uart_write_hex_char(val)                                            /*!< If DEBUG is set, log a character as HEX value (example output: AA). */
#define NRF_LOG_HEX_CHAR_ERROR(val)     log_uart_write_hex_char(val)                                            /*!< Log a character as HEX value to the error stream (example output: AA). */

#define NRF_LOG_HAS_INPUT()             log_uart_has_input()                                                    /*!< Check if the input buffer has unconsumed characters. */
#define NRF_LOG_READ_INPUT(p_char)      log_uart_read_input(p_char)                                             /*!< Consume a character from the input buffer. */

#if !defined(DEBUG) && !defined(DOXYGEN)

#undef NRF_LOG_DEBUG
#define NRF_LOG_DEBUG(...)

#undef NRF_LOG_PRINTF_DEBUG
#define NRF_LOG_PRINTF_DEBUG(...)

#undef NRF_LOG_STR_DEBUG
#define NRF_LOG_STR_DEBUG(...)

#undef NRF_LOG_HEX_DEBUG
#define NRF_LOG_HEX_DEBUG(...)

#undef NRF_LOG_HEX_CHAR_DEBUG
#define NRF_LOG_HEX_CHAR_DEBUG(...)

#endif // !defined(DEBUG) && !defined(DOXYGEN)

#elif defined(NRF_LOG_USES_RAW_UART) && NRF_LOG_USES_RAW_UART == 1

/**@brief Function for initializing the raw UART logger.
 *
 * This function is available only when NRF_LOG_USES_RAW_UART is defined as 1.
 *
 * @note Do not call this function directly. Use the macro @ref NRF_LOG_INIT instead.
 *
 * @retval     NRF_SUCCESS     If initialization was successful.
 * @retval     NRF_ERROR       Otherwise.
 */
uint32_t log_raw_uart_init(void);

/**@brief Function for logging a printf string to raw UART.
 *
 * @details  Printf requires more processor time
 * than other logging functions. Therefore, applications that require logging
 * but need it to interfere as little as possible with the execution, should
 * avoid using printf.
 *
 * This function is available only when NRF_LOG_USES_RAW_UART is defined as 1.
 *
 * @note This function is non-blocking. If too much data is sent to the UART,
 *          some characters might be skipped.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_PRINTF
 * - @ref NRF_LOG_PRINTF_DEBUG
 * - @ref NRF_LOG_PRINTF_ERROR
 *
 * @param  format_msg      Printf format string.
 */
void log_raw_uart_printf(const char * format_msg, ...);

/**@brief Function for logging a single character to raw UART.
 *
 * This function is available only when NRF_LOG_USES_RAW_UART is defined as 1.
 *
 * @param  c     Character.
 */
void log_raw_uart_write_char(const char c);

/**@brief Function for logging null-terminated strings to raw UART.
 *
 * @details This function is more efficient than using printf.
 *          The null termination will not be logged.
 *
 * This function is available only when NRF_LOG_USES_RAW_UART is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG
 * - @ref NRF_LOG_DEBUG
 * - @ref NRF_LOG_ERROR
 *
 * @param  num_args     Number of arguments.
 */
void log_raw_uart_write_string_many(int num_args, ...);


/**@brief Function for logging a null-terminated string to raw UART.
 *
 * @details This function is more efficient than using printf.
 *          The null termination will not be logged.
 *
 * This function is available only when NRF_LOG_USES_RAW_UART is defined as 1.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG
 * - @ref NRF_LOG_DEBUG
 * - @ref NRF_LOG_ERROR
 *
 * @param  str     Null-terminated string.
 */
void log_raw_uart_write_string(const char * str);

/**@brief Function for logging an integer value as HEX to raw UART.
 *
 * @details The output data is formatted as, for example, 0x89ABCDEF.
 * This function is more efficient than printf.
 *
 * This function is available only when NRF_LOG_USES_RAW_UART is defined as 1.
 *
 * @note This function is non-blocking. If too much data is sent to the UART,
 *          some characters might be skipped.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_HEX
 * - @ref NRF_LOG_HEX_DEBUG
 * - @ref NRF_LOG_HEX_ERROR
 *
 * @param   value   Integer value to be printed as HEX.
 */
void log_raw_uart_write_hex(uint32_t value);

/**@brief Function for logging a single character as HEX to raw UART.
 *
 * @details The output string is formatted as, for example, AA.
 *
 * This function is available only when NRF_LOG_USES_RAW_UART is defined as 1.
 *
 * @note This function is non-blocking. If too much data is sent to the UART,
 *          some characters might be skipped.
 *
 * @note Do not call this function directly. Use one of the following macros instead:
 * - @ref NRF_LOG_HEX_CHAR
 * - @ref NRF_LOG_HEX_CHAR_DEBUG
 * - @ref NRF_LOG_HEX_CHAR_ERROR
 *
 * @param c Character.
  */
void log_raw_uart_write_hex_char(uint8_t c);

/**@brief Function for checking if data is available in the input buffer.
 *
 * This function is available only when NRF_LOG_USES_RAW_UART is defined as 1.
 *
 * @note Do not call this function directly. Use @ref NRF_LOG_HAS_INPUT instead.
 *
 * @retval      1 If characters are available to read.
 * @retval      0 If no characters are available.
 */
int log_raw_uart_has_input(void);

/**@brief Function for reading one character from the input buffer.
 *
 * @param[out]   p_char  Pointer where to store the character.
 *
 * This function is available only when NRF_LOG_USES_RAW_UART is defined as 1.
 *
 * @note Do not call this function directly. Use NRF_LOG_READ_INPUT instead.
 *
 * @retval      NRF_SUCCESS If the character was read out.
 * @retval      NRF_ERROR_INVALID_DATA If no character could be read.
 */

uint32_t log_raw_uart_read_input(char* p_char);

#define NRF_LOG_INIT()                  log_raw_uart_init()                                                         /*!< nitialize the module. */

#define NRF_LOG_PRINTF(...)             log_raw_uart_printf(__VA_ARGS__)                                            /*!< Print a log message using printf. */
#define NRF_LOG_PRINTF_DEBUG(...)       log_raw_uart_printf(__VA_ARGS__)                                            /*!< If DEBUG is set, print a log message using printf. */
#define NRF_LOG_PRINTF_ERROR(...)       log_raw_uart_printf(__VA_ARGS__)                                            /*!< Print a log message using printf to the error stream. */

#define NRF_LOG(...)                    log_raw_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< Print a log message. The input string must be null-terminated. */
#define NRF_LOG_DEBUG(...)              log_raw_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< If DEBUG is set, print a log message. The input string must be null-terminated. */
#define NRF_LOG_ERROR(...)              log_raw_uart_write_string_many(NUM_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__)     /*!< Print a log message to the error stream. The input string must be null-terminated. */

#define NRF_LOG_HEX(val)                log_raw_uart_write_hex(val)                                                 /*!< Log an integer as HEX value (example output: 0x89ABCDEF). */
#define NRF_LOG_HEX_DEBUG(val)          log_raw_uart_write_hex(val)                                                 /*!< If DEBUG is set, log an integer as HEX value (example output: 0x89ABCDEF). */
#define NRF_LOG_HEX_ERROR(val)          log_raw_uart_write_hex(val)                                                 /*!< Log an integer as HEX value to the error stream (example output: 0x89ABCDEF). */

#define NRF_LOG_HEX_CHAR(val)           log_raw_uart_write_hex_char(val)                                            /*!< Log a character as HEX value (example output: AA). */
#define NRF_LOG_HEX_CHAR_DEBUG(val)     log_raw_uart_write_hex_char(val)                                            /*!< If DEBUG is set, log a character as HEX value (example output: AA). */
#define NRF_LOG_HEX_CHAR_ERROR(val)     log_raw_uart_write_hex_char(val)                                            /*!< Log a character as HEX value to the error stream (example output: AA). */

#define NRF_LOG_HAS_INPUT()             log_raw_uart_has_input()                                                    /*!< Check if the input buffer has unconsumed characters. */
#define NRF_LOG_READ_INPUT(p_char)      log_raw_uart_read_input(p_char)                                             /*!< Consume a character from the input buffer. */

#if !defined(DEBUG) && !defined(DOXYGEN)

#undef NRF_LOG_DEBUG
#define NRF_LOG_DEBUG(...)

#undef NRF_LOG_PRINTF_DEBUG
#define NRF_LOG_PRINTF_DEBUG(...)

#undef NRF_LOG_STR_DEBUG
#define NRF_LOG_STR_DEBUG(...)

#undef NRF_LOG_HEX_DEBUG
#define NRF_LOG_HEX_DEBUG(...)

#undef NRF_LOG_HEX_CHAR_DEBUG
#define NRF_LOG_HEX_CHAR_DEBUG(...)

#endif // !defined(DEBUG) && !defined(DOXYGEN)

#else

#include "nrf_error.h"
#include "nordic_common.h"

// Empty definitions

#define NRF_LOG_INIT()                 NRF_SUCCESS
#define NRF_LOG(...)
#define NRF_LOG_DEBUG(...)
#define NRF_LOG_ERROR(...)

#define NRF_LOG_PRINTF(...)
#define NRF_LOG_PRINTF_DEBUG(...)
#define NRF_LOG_PRINTF_ERROR(...)

#define NRF_LOG_HEX(val)
#define NRF_LOG_HEX_DEBUG(val)
#define NRF_LOG_HEX_ERROR(val)

#define NRF_LOG_HEX_CHAR(val)
#define NRF_LOG_HEX_CHAR_DEBUG(val)
#define NRF_LOG_HEX_CHAR_ERROR(val)

#define NRF_LOG_HAS_INPUT()              0
#define NRF_LOG_READ_INPUT(ignore)       NRF_SUCCESS

#endif

/**@brief Function for writing HEX values.
 *
 * @note This function not thread-safe. It is written for convenience.
 *          If you log from different application contexts, you might get different results.
 *
 * @retval NULL By default.
 */
const char* log_hex(uint32_t value);

/**@brief Function for writing HEX characters.
 *
 * @note This function not thread-safe. It is written for convenience.
 *          If you log from different application contexts, you might get different results.
 *
 * @retval NULL By default.
 */
const char* log_hex_char(const char value);




#else // DOXYGEN

/** @defgroup nrf_log UART/RTT logging
 * @{
 * @ingroup app_common
 *
 * @brief Library to output logging information over SEGGER's Real Time Transfer
 *       (RTT), UART, or raw UART.
 *
 * This library provides macros that call the respective functions depending on
 * which protocol is used. Define LOG_USES_RTT=1 to enable logging over RTT,
 * NRF_LOG_USES_UART=1 to enable logging over UART, or NRF_LOG_USES_RAW_UART=1
 * to enable logging over raw UART. One of these defines must be set for any of
 * the macros to have effect. If you choose to not output information, all
 * logging macros can be left in the code without any cost; they will just be
 * ignored.
 */



/**@brief Macro for initializing the logger.
 *
 * @retval     NRF_SUCCESS     If initialization was successful.
 * @retval     NRF_ERROR       Otherwise.
 */
uint32_t NRF_LOG_INIT(void);

/**@brief Macro for logging null-terminated strings.
 *
 * @details This function is more efficient than using printf.
 *          The null termination will not be logged.
 *
 * @param  msg  Null-terminated string.
 */
void NRF_LOG(const char* msg);

/**@brief Macro for logging a printf string.
 *
 * @details  Printf requires more processor time
 * than other logging functions. Therefore, applications that require logging
 * but need it to interfere as little as possible with the execution, should
 * avoid using printf.
 *
 * @note When NRF_LOG_USES_UART is set to 1, this macro is non-blocking.
 *       If too much data is sent, some characters might be skipped.
 *
 * @param  format_msg      Printf format string.
 * @param  ...             Additional arguments replacing format specifiers in format_msg.
 */
void NRF_LOG_PRINTF(const char * format_msg, ...);

/**@brief Macro for logging an integer value as HEX.
 *
 * @details The output data is formatted as, for example, 0x89ABCDEF.
 * This function is more efficient than printf.
 *
 * @note When NRF_LOG_USES_UART is set to 1, this macro is non-blocking.
 *       If too much data is sent, some characters might be skipped.
 *
 * @param   value   Integer value to be printed as HEX.
 */
void NRF_LOG_HEX(uint32_t value);

/**@brief Macro for logging a single character as HEX.
 *
 * @details The output string is formatted as, for example, AA.
 *
 * @note When NRF_LOG_USES_UART is set to 1, this macro is non-blocking.
 *       If too much data is sent, some characters might be skipped.
 *
 * @param c Character.
  */
void NRF_LOG_HEX_CHAR(uint8_t c);

/**@brief Macro for checking if data is available in the input buffer.
 *
 * @note When NRF_LOG_USES_UART is set to 1, this macro is non-blocking.
 *       If too much data is sent, some characters might be skipped.
 *
 * @retval      1 If characters are available to read.
 * @retval      0 If no characters are available.
 */
int NRF_LOG_HAS_INPUT(void);

/**@brief Macro for reading one character from the input buffer.
 *
 * @param[out]   p_char  Pointer where to store the character.
 *
 * @retval      NRF_SUCCESS If the character was read out.
 * @retval      NRF_ERROR_INVALID_DATA If no character could be read.
 */
uint32_t NRF_LOG_READ_INPUT(char* p_char);

/** @} */
#endif // DOXYGEN
#endif // NRF_LOG_H_
