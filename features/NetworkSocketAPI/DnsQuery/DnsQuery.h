/* 
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
#ifndef __DNSQUERY_H__
#define __DNSQUERY_H__

#include "NetworkStack.h"
#include "UDPSocket.h"


/** Function dnsQuery implements the functionality to query a domain name 
  * server for an IP-Address of a given hostname.
  * @param iface : Network interface to use for DNS resolution.
  * @param sock : Previously opened socket to use for DNS resolution.
  * @param hostname : The hostname of interest as a string.
  *     Format must be without http:// or www. IE google.com, mbed.org, etc.
  *     If a standard IP Address is passed, it will be copied into ip unmodified.
  * @param ipaddress : A reference to a IPADDRESS_t object which will receive 
  *     the resolved IP Address of the host in question.
  * @returns 0 on succes, NS_DNS_FAILURE if host is not found,
  *     or a negative value for other errors.
  */
int32_t dnsQuery(NetworkStack *iface, const char *host, char *ip);
int32_t dnsQuery(UDPSocket *sock, const char *host, char *ip);


#endif // __DNSQUERY_H__
