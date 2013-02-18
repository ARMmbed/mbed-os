/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef BSD_SOCKET_H_
#define BSD_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "socket/sys/socket.h" //Must conform to <http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html>
#include "socket/netinet/in.h" //Must conform to <http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/netinet_in.h.html>
#include "socket/netdb.h"      //Must conform to <http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/netdb.h.html>

#ifdef __cplusplus
}
#endif

#endif /* BSD_SOCKET_H_ */
