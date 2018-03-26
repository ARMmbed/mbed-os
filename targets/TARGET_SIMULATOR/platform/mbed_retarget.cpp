/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include <time.h>
#include "platform/platform.h"
#include "platform/FilePath.h"
#if DEVICE_SERIAL
#include "hal/serial_api.h"
#endif
#include "hal/us_ticker_api.h"
#include "platform/mbed_toolchain.h"
#ifndef TARGET_SIMULATOR
#include "platform/mbed_semihost_api.h"
#endif
#include "platform/mbed_interface.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"
#include "platform/mbed_error.h"
#ifndef TARGET_SIMULATOR
#include "platform/mbed_stats.h"
#endif
#include "platform/mbed_critical.h"
#include "platform/mbed_poll.h"
#include "platform/PlatformMutex.h"
#include "drivers/UARTSerial.h"
#include "us_ticker_api.h"
#include "lp_ticker_api.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#ifndef SSIZE_MAX
#define SSIZE_MAX INT_MAX
#endif
#include <stdio.h>
#include <errno.h>
#include "platform/mbed_retarget.h"

static SingletonPtr<PlatformMutex> _mutex;

#if defined(__ARMCC_VERSION)
#   if __ARMCC_VERSION >= 6010050
#      include <arm_compat.h>
#   endif
#   include <rt_sys.h>
#   include <rt_misc.h>
#   include <stdint.h>
#   define PREFIX(x)    _sys##x
#   define OPEN_MAX     _SYS_OPEN
#   ifdef __MICROLIB
#       pragma import(__use_full_stdio)
#   endif

#elif defined(__ICCARM__)
#   include <yfuns.h>
#   define PREFIX(x)        _##x
#   define OPEN_MAX         16

#   define STDIN_FILENO     0
#   define STDOUT_FILENO    1
#   define STDERR_FILENO    2

#else
#   include <sys/stat.h>
#   define PREFIX(x)    x
#   define OPEN_MAX         16
#endif

#define FILE_HANDLE_RESERVED    ((FileHandle*)0xFFFFFFFF)

using namespace mbed;

#if defined(__MICROLIB) && (__ARMCC_VERSION>5030000)
// Before version 5.03, we were using a patched version of microlib with proper names
extern const char __stdin_name[]  = ":tt";
extern const char __stdout_name[] = ":tt";
extern const char __stderr_name[] = ":tt";

#else
extern const char __stdin_name[]  = "/stdin";
extern const char __stdout_name[] = "/stdout";
extern const char __stderr_name[] = "/stderr";
#endif

unsigned char *mbed_heap_start = 0;
uint32_t mbed_heap_size = 0;

/* newlib has the filehandle field in the FILE struct as a short, so
 * we can't just return a Filehandle* from _open and instead have to
 * put it in a filehandles array and return the index into that array
 */
static FileHandle *filehandles[OPEN_MAX] = { FILE_HANDLE_RESERVED, FILE_HANDLE_RESERVED, FILE_HANDLE_RESERVED };
static char stdio_in_prev[OPEN_MAX];
static char stdio_out_prev[OPEN_MAX];
static SingletonPtr<PlatformMutex> filehandle_mutex;

namespace mbed {
void mbed_set_unbuffered_stream(std::FILE *_file);

void remove_filehandle(FileHandle *file) {
    filehandle_mutex->lock();
    /* Remove all open filehandles for this */
    for (unsigned int fh_i = 0; fh_i < sizeof(filehandles)/sizeof(*filehandles); fh_i++) {
        if (filehandles[fh_i] == file) {
            filehandles[fh_i] = NULL;
        }
    }
    filehandle_mutex->unlock();
}
}

#if DEVICE_SERIAL
extern int stdio_uart_inited;
extern serial_t stdio_uart;

/* Private FileHandle to implement backwards-compatible functionality of
 * direct HAL serial access for default stdin/stdout/stderr.
 * This is not a particularly well-behaved FileHandle for a stream, which
 * is why it's not public. People should be using UARTSerial.
 */
class DirectSerial : public FileHandle {
public:
    DirectSerial(PinName tx, PinName rx, int baud);
    virtual ssize_t write(const void *buffer, size_t size);
    virtual ssize_t read(void *buffer, size_t size);
    virtual off_t seek(off_t offset, int whence = SEEK_SET) {
        return -ESPIPE;
    }
    virtual off_t size() {
        return -EINVAL;
    }
    virtual int isatty() {
        return true;
    }
    virtual int close() {
        return 0;
    }
    virtual short poll(short events) const;
};

DirectSerial::DirectSerial(PinName tx, PinName rx, int baud) {
    if (stdio_uart_inited) return;
    serial_init(&stdio_uart, tx, rx);
    serial_baud(&stdio_uart, baud);
}

ssize_t DirectSerial::write(const void *buffer, size_t size) {
    const unsigned char *buf = static_cast<const unsigned char *>(buffer);
    for (size_t i = 0; i < size; i++) {
        serial_putc(&stdio_uart, buf[i]);
    }
    return size;
}

ssize_t DirectSerial::read(void *buffer, size_t size) {
    unsigned char *buf = static_cast<unsigned char *>(buffer);
    if (size == 0) {
        return 0;
    }
    buf[0] = serial_getc(&stdio_uart);
    return 1;
}

short DirectSerial::poll(short events) const {
    short revents = 0;
    if ((events & POLLIN) && serial_readable(&stdio_uart)) {
        revents |= POLLIN;
    }
    if ((events & POLLOUT) && serial_writable(&stdio_uart)) {
        revents |= POLLOUT;
    }
    return revents;
}

class Sink : public FileHandle {
public:
    virtual ssize_t write(const void *buffer, size_t size);
    virtual ssize_t read(void *buffer, size_t size);
    virtual off_t seek(off_t offset, int whence = SEEK_SET) { return ESPIPE; }
    virtual off_t size() { return -EINVAL; }
    virtual int isatty() { return true; }
    virtual int close() { return 0; }
};

ssize_t Sink::write(const void *buffer, size_t size) {
    // Just swallow the data - this is historical non-DEVICE_SERIAL behaviour
    return size;
}

ssize_t Sink::read(void *buffer, size_t size) {
    // Produce 1 zero byte - historical behaviour returned 1 without touching
    // the buffer
    unsigned char *buf = static_cast<unsigned char *>(buffer);
    buf[0] = 0;
    return 1;
}
#endif

MBED_WEAK FileHandle* mbed::mbed_target_override_console(int fd)
{
    return NULL;
}

MBED_WEAK FileHandle* mbed::mbed_override_console(int fd)
{
    return NULL;
}

static int reserve_filehandle() {
    // find the first empty slot in filehandles, after the slots reserved for stdin/stdout/stderr
    filehandle_mutex->lock();
    int fh_i;
    for (fh_i = 3; fh_i < OPEN_MAX; fh_i++) {
        /* Take a next free filehandle slot available. */
        if (filehandles[fh_i] == NULL) break;
    }
    if (fh_i >= OPEN_MAX) {
        /* Too many file handles have been opened */
        errno = EMFILE;
        filehandle_mutex->unlock();
        return -1;
    }
    filehandles[fh_i] = FILE_HANDLE_RESERVED;
    filehandle_mutex->unlock();

    return fh_i;
}

int mbed::bind_to_fd(FileHandle *fh) {
    int fh_i = reserve_filehandle();
    if (fh_i < 0) {
        return fh_i;
    }

    filehandles[fh_i] = fh;
    stdio_in_prev[fh_i] = 0;
    stdio_out_prev[fh_i] = 0;

    return fh_i;
}

#ifdef __ARMCC_VERSION
extern "C" char *_sys_command_string(char *cmd, int len) {
    return NULL;
}
#endif


#if defined(TOOLCHAIN_GCC)
/* prevents the exception handling name demangling code getting pulled in */
#include "mbed_error.h"
namespace __gnu_cxx {
    void __verbose_terminate_handler() {
        error("Exception");
    }
}
extern "C" WEAK void __cxa_pure_virtual(void);
extern "C" WEAK void __cxa_pure_virtual(void) {
    exit(1);
}

#endif

// Provide implementation of _sbrk (low-level dynamic memory allocation
// routine) for GCC_ARM which compares new heap pointer with MSP instead of
// SP.  This make it compatible with RTX RTOS thread stacks.
#if defined(TOOLCHAIN_GCC_ARM) || defined(TOOLCHAIN_GCC_CR)

#if defined(TARGET_CORTEX_A)
extern "C" uint32_t  __HeapLimit;
#endif

// Turn off the errno macro and use actual global variable instead.
#undef errno
extern "C" int errno;

// Dynamic memory allocation related syscall.
#if (defined(TARGET_NUVOTON) || defined(TWO_RAM_REGIONS))

// Overwrite _sbrk() to support two region model (heap and stack are two distinct regions).
// __wrap__sbrk() is implemented in:
// TARGET_NUMAKER_PFM_NUC472    targets/TARGET_NUVOTON/TARGET_NUC472/TARGET_NUMAKER_PFM_NUC472/TOOLCHAIN_GCC_ARM/nuc472_retarget.c
// TARGET_NUMAKER_PFM_M453      targets/TARGET_NUVOTON/TARGET_M451/TARGET_NUMAKER_PFM_M453/TOOLCHAIN_GCC_ARM/m451_retarget.c
// TARGET_STM32L4               targets/TARGET_STM/TARGET_STM32L4/TARGET_STM32L4/l4_retarget.c
extern "C" void *__wrap__sbrk(int incr);
extern "C" caddr_t _sbrk(int incr) {
    return (caddr_t) __wrap__sbrk(incr);
}
#else
// Linker defined symbol used by _sbrk to indicate where heap should start.
extern "C" uint32_t __end__;
// Weak attribute allows user to override, e.g. to use external RAM for dynamic memory.
extern "C" WEAK caddr_t _sbrk(int incr) {
    static unsigned char* heap = (unsigned char*)&__end__;
    unsigned char*        prev_heap = heap;
    unsigned char*        new_heap = heap + incr;

#if defined(TARGET_CORTEX_A)
    if (new_heap >= (unsigned char*)&__HeapLimit) {     /* __HeapLimit is end of heap section */
#else
    if (new_heap >= (unsigned char*)__get_MSP()) {
#endif
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    // Additional heap checking if set
    if (mbed_heap_size && (new_heap >= mbed_heap_start + mbed_heap_size)) {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    heap = new_heap;
    return (caddr_t) prev_heap;
}
#endif
#endif

#if defined(TOOLCHAIN_GCC_ARM) || defined(TOOLCHAIN_GCC_CR)
extern "C" void _exit(int return_code) {
#else
namespace std {
extern "C" void exit(int return_code) {
#endif

#if DEVICE_STDIO_MESSAGES
#if MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT
    fflush(stdout);
    fflush(stderr);
#endif
#endif

#if DEVICE_SEMIHOST
    if (mbed_interface_connected()) {
        semihost_exit();
    }
#endif
    if (return_code) {
        mbed_die();
    }

    while (1);
}

#if !defined(TOOLCHAIN_GCC_ARM) && !defined(TOOLCHAIN_GCC_CR)
} //namespace std
#endif

#if defined(TOOLCHAIN_ARM) || defined(TOOLCHAIN_GCC)

// This series of function disable the registration of global destructors
// in a dynamic table which will be called when the application exit.
// In mbed, program never exit properly, it dies.
// More informations about this topic for ARMCC here:
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/6449.html
extern "C" {
int __aeabi_atexit(void *object, void (*dtor)(void* /*this*/), void *handle) {
    return 1;
}

int __cxa_atexit(void (*dtor)(void* /*this*/), void *object, void *handle) {
    return 1;
}

void __cxa_finalize(void *handle) {
}

} // end of extern "C"

#endif


#if defined(TOOLCHAIN_GCC)

/*
 * Depending on how newlib is  configured, it is often not enough to define
 * __aeabi_atexit, __cxa_atexit and __cxa_finalize in order to override the
 * behavior regarding the registration of handlers with atexit.
 *
 * To overcome this limitation, exit and atexit are overriden here.
 */
extern "C"{

/**
 * @brief Retarget of exit for GCC.
 * @details Unlike the standard version, this function doesn't call any function
 * registered with atexit before calling _exit.
 */
void __wrap_exit(int return_code) {
    _exit(return_code);
}

/**
 * @brief Retarget atexit from GCC.
 * @details This function will always fail and never register any handler to be
 * called at exit.
 */
int __wrap_atexit(void (*func)()) {
    return 1;
}

}

#endif



namespace mbed {

void mbed_set_unbuffered_stream(std::FILE *_file) {
#if defined (__ICCARM__)
    char buf[2];
    std::setvbuf(_file,buf,_IONBF,NULL);
#else
    setbuf(_file, NULL);
#endif
}

int mbed_getc(std::FILE *_file){
#if defined(__IAR_SYSTEMS_ICC__ ) && (__VER__ < 8000000)
    /*This is only valid for unbuffered streams*/
    int res = std::fgetc(_file);
    if (res>=0){
        _file->_Mode = (unsigned short)(_file->_Mode & ~ 0x1000);/* Unset read mode */
        _file->_Rend = _file->_Wend;
        _file->_Next = _file->_Wend;
    }
    return res;
#else
    return std::fgetc(_file);
#endif
}

char* mbed_gets(char*s, int size, std::FILE *_file){
#if defined(__IAR_SYSTEMS_ICC__ ) && (__VER__ < 8000000)
    /*This is only valid for unbuffered streams*/
    char *str = fgets(s,size,_file);
    if (str!=NULL){
        _file->_Mode = (unsigned short)(_file->_Mode & ~ 0x1000);/* Unset read mode */
        _file->_Rend = _file->_Wend;
        _file->_Next = _file->_Wend;
    }
    return str;
#else
    return std::fgets(s,size,_file);
#endif
}

} // namespace mbed

#if defined (__ICCARM__)
// Stub out locks when an rtos is not present
extern "C" WEAK void __iar_system_Mtxinit(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_system_Mtxdst(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_system_Mtxlock(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_system_Mtxunlock(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_file_Mtxinit(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_file_Mtxdst(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_file_Mtxlock(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_file_Mtxunlock(__iar_Rmtx *mutex) {}
#if defined(__IAR_SYSTEMS_ICC__ ) && (__VER__ >= 8000000)
#pragma section="__iar_tls$$DATA"
extern "C" WEAK void *__aeabi_read_tp (void) {
  // Thread Local storage is not supported, using main thread memory for errno
  return __section_begin("__iar_tls$$DATA");
}
#endif
#elif defined(__CC_ARM)
// Do nothing
#elif defined (__GNUC__)
struct _reent;
// Stub out locks when an rtos is not present
extern "C" WEAK void __rtos_malloc_lock( struct _reent *_r ) {}
extern "C" WEAK void __rtos_malloc_unlock( struct _reent *_r ) {}
extern "C" WEAK void __rtos_env_lock( struct _reent *_r ) {}
extern "C" WEAK void __rtos_env_unlock( struct _reent *_r ) {}

extern "C" void __malloc_lock( struct _reent *_r )
{
    __rtos_malloc_lock(_r);
}

extern "C" void __malloc_unlock( struct _reent *_r )
{
    __rtos_malloc_unlock(_r);
}

extern "C" void __env_lock( struct _reent *_r )
{
    __rtos_env_lock(_r);
}

extern "C" void __env_unlock( struct _reent *_r )
{
    __rtos_env_unlock(_r);
}

#endif


/* @brief   standard c library clock() function.
 *
 * This function returns the number of clock ticks elapsed since the start of the program.
 *
 * @note Synchronization level: Thread safe
 *
 * @return
 *  the number of clock ticks elapsed since the start of the program.
 *
 * */
extern "C" clock_t clock()
{
    _mutex->lock();
    clock_t t = ticker_read(get_us_ticker_data());
    t /= 1000000 / CLOCKS_PER_SEC; // convert to processor time
    _mutex->unlock();
    return t;
}

// temporary - Default to 1MHz at 32 bits if target does not have us_ticker_get_info
MBED_WEAK const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        1000000,
        32
    };
    return &info;
}

// temporary - Default to 1MHz at 32 bits if target does not have lp_ticker_get_info
MBED_WEAK const ticker_info_t* lp_ticker_get_info()
{
    static const ticker_info_t info = {
        1000000,
        32
    };
    return &info;
}
