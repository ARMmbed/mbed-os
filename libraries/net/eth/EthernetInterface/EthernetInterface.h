/* EthernetInterface.h */
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
 
#ifndef ETHERNETINTERFACE_H_
#define ETHERNETINTERFACE_H_

#if !defined(TARGET_LPC1768) && !defined(TARGET_LPC4088) && !defined(TARGET_LPC4088_DM) && !defined(TARGET_K64F) && !defined(TARGET_RZ_A1H) && !defined(TARGET_STM32F4)
#error The Ethernet Interface library is not supported on this target
#endif

#include "rtos.h"
#include "lwip/netif.h"

/** \brief DP83848 PHY status definitions */
#define DP8_REMOTEFAULT    (1 << 6)   /**< Remote fault */
#define DP8_FULLDUPLEX     (1 << 2)   /**< 1=full duplex */
#define DP8_SPEED10MBPS    (1 << 1)   /**< 1=10MBps speed */
#define DP8_VALID_LINK     (1 << 0)   /**< 1=Link active */

 /** Interface using Ethernet to connect to an IP-based network
 *
 */
class EthernetInterface {
public:
  /** Initialize the interface with DHCP.
  * Initialize the interface and configure it to use DHCP (no connection at this point).
  * \return 0 on success, a negative number on failure
  */
  static int init(); //With DHCP

  /** Initialize the interface with a static IP address.
  * Initialize the interface and configure it with the following static configuration (no connection at this point).
  * \param ip the IP address to use
  * \param mask the IP address mask
  * \param gateway the gateway to use
  * \return 0 on success, a negative number on failure
  */
  static int init(const char* ip, const char* mask, const char* gateway);

  /** Connect
  * Bring the interface up, start DHCP if needed.
  * \param   timeout_ms  timeout in ms (default: (15)s).
  * \return 0 on success, a negative number on failure
  */
  static int connect(unsigned int timeout_ms=15000);
  
  /** Disconnect
  * Bring the interface down
  * \return 0 on success, a negative number on failure
  */
  static int disconnect();
  
  /** Get the MAC address of your Ethernet interface
   * \return a pointer to a string containing the MAC address
   */
  static char* getMACAddress();
  
  /** Get the IP address of your Ethernet interface
   * \return a pointer to a string containing the IP address
   */
  static char* getIPAddress();

  /** Get the Gateway address of your Ethernet interface
   * \return a pointer to a string containing the Gateway address
   */
  static char* getGateway();

  /** Get the Network mask of your Ethernet interface
   * \return a pointer to a string containing the Network mask
   */
  static char* getNetworkMask();
  
  /** Set the network name for this device. 
  *
  * Apply this before calling 'connect'.
  *
  * \example
  * EthernetInterface eth;
  * ...
  *     if (0 == eth.init()) {
  *         eth.setName("Sensor 3");
  *         if (0 == eth.connect()) {
  *             ...
  *
  * \param[in] myname is the name to assign for this node. 
  *        Only the first 32 characters will be used if the 
  *        name is longer.
  *        Only '0'-'9', 'A'-'Z', 'a'-'z' are accepted,
  *        any others are converted to '-'.
  * \return 0 on success, a negative number on failure.
  */
  static int setName(const char * myname);

};

#include "TCPSocketConnection.h"
#include "TCPSocketServer.h"

#include "Endpoint.h"
#include "UDPSocket.h"

#endif /* ETHERNETINTERFACE_H_ */
