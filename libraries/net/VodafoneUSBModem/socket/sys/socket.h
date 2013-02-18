/* socket.h */
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


#ifndef SYS_SOCKET_H_
#define SYS_SOCKET_H_

#include "lwip/sockets.h"

//Sockets

inline int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
  return lwip_accept(s, addr, addrlen);
}

inline int bind(int s, const struct sockaddr *name, socklen_t namelen)
{
  return lwip_bind(s, name, namelen);
}

inline int shutdown(int s, int how)
{
  return lwip_shutdown(s, how);
}

inline int getsockname (int s, struct sockaddr *name, socklen_t *namelen)
{
  return lwip_getsockname(s, name, namelen);
}

inline int getpeername (int s, struct sockaddr *name, socklen_t *namelen)
{
  return lwip_getpeername(s, name, namelen);
}

inline int getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen)
{
  return lwip_getsockopt(s, level, optname, optval, optlen);
}

inline int setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen)
{
  return lwip_setsockopt(s, level, optname, optval, optlen);
}

inline int connect(int s, const struct sockaddr *name, socklen_t namelen)
{
  return lwip_connect(s, name, namelen);
}

inline int listen(int s, int backlog)
{
  return lwip_listen(s, backlog);
}

inline int recv(int s, void *mem, size_t len, int flags)
{
  return lwip_recv(s, mem, len, flags);
}

inline int recvfrom(int s, void *mem, size_t len, int flags,
      struct sockaddr *from, socklen_t *fromlen)
{
  return lwip_recvfrom(s, mem, len, flags, from, fromlen);
}

inline int send(int s, const void *dataptr, size_t size, int flags)
{
  return lwip_send(s, dataptr, size, flags);
}

inline int sendto(int s, const void *dataptr, size_t size, int flags,
    const struct sockaddr *to, socklen_t tolen)
{
  return lwip_sendto(s, dataptr, size, flags, to, tolen);
}

inline int socket(int domain, int type, int protocol)
{
  return lwip_socket(domain, type, protocol);
}

inline int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
                struct timeval *timeout)
{
  return lwip_select(maxfdp1, readset, writeset, exceptset, timeout);
}

inline int ioctlsocket(int s, long cmd, void *argp)
{
  return lwip_ioctl(s, cmd, argp);
}

inline int read(int s, void *mem, size_t len)
{
  return lwip_read(s, mem, len);
}

inline int write(int s, const void *dataptr, size_t size)
{
  return lwip_write(s, dataptr, size);
}

inline int close(int s)
{
  return lwip_close(s);
}

#endif /* SYS_SOCKET_H_ */
