# Multihoming in MbedOS

## Table of contents

1. [Revision history](#revision-history).
1. [Introduction](#introduction).
    1. [Overview and background](#overview-and-background).
    1. [Requirements and assumptions](#requirements-and-assumptions).
1. [System architecture and high-level design](#system-architecture-and-high-level-design).
    1. [Architecture](#architecture).
	1. [Component interaction](#component-interaction).
1. [Detailed design](#detailed-design).
    1. [MbedOS networking class diagram extension proposal](#mbedos-networking-class-diagram-extension-proposal).
    2. [Simplified LWIP and EMAC](#simplified-lwip-and-emac).
    1. [LWIPInterface changes](#lwipinterface-changes).
    1. [EMACInterface and EMAC driver](#emacinterface-and-emac-driver).
    1. [L3IPInterface and L3IP driver](#l3ipinterface-and-l3ip-driver).
    1. [Memory manager](#memory-manager).
    1. [Network default interface construction](#network-default-interface-construction).
    1. [Adding interface](#adding-interface).
    1. [Removing interface](#removing-interface).
    1. [Connecting Ethernet or Wifi](#connecting-ethernet-or-Wifi).
    1. [Disconnecting Ethernet or Wifi](#disconnecting-ethernet-or-Wifi).
    1. [Connecting Cellular](#connecting-cellular).
    1. [Disconnecting Cellular](#disconnecting-cellular).
    1. [Addressing](#addressing).
    1. [LWIP IP core](#lwip-ip-core).
    	1. [EMAC based outgoing traffic for ethernet and wifi](#emac-based-outgoing-traffic-for-ethernet-and-wifi).
    	1. [PPP based outgoing traffic for cellular](#ppp-based-outgoing-traffic-for-cellular).
    	1. [EMAC based incoming traffic for ethernet and wifi](#emac-based-incoming-traffic-for-ethernet-and-wifi).
    	1. [PPP based incoming traffic for cellular](#ppp-based-incoming-traffic-for-cellular).
    	1. [IP Route changes](#IP-Route-changes).
    1. [Network stack interface](#network-stack-interface).
    1. [Sockets changes](#sockets-changes).
    1. [DNS changes](#dns-changes).
1. [Usage scenario](#usage-scenario).


## Revision history

| Revision 	| Date           	|  Authors           | Mbed OS version 	| Comments         	|

| 1.0      	| 25 September 2018 	| Tymoteusz Bloch  	| 5.11           	| Initial revision 	|

#   Introduction

##  Overview and background

This document refers to mutlihoming for MbedOS  ARM CortexM based embedded devices.
Multihoming is the way of connecting device to more than one network interface. This can be done in order to increase reliability or performance. 

Under normal operating condition embedded ip stack  is connected to  only one network driver. In many situations , it can be useful to connect an embedded   device to multiple networks,  to increase reliability (if a one network link fails, data can still be routed through the remaining networks) and to improve performance (depending on the destination, it is more efficient to route through multiple networks ).

## Requirements and assumptions
To use multihoming embedded device board must be equipped with more than one network interface, driver and physical  medium device. It can be a combination of ethernet, wifi, cellular modules as well as the same type multiple devices  eg. two ethernet physical devices. 

# System architecture and high-level design


## Architecture

Mbed OS network functionality  is implemented  inside  **features** module.

High level view is on picture below

![high_level_architecture]

For connectivity MbedOS uses  Netsocket module. It contains :

- sockets 
- DNS
- TCP/UDP 
- network interfaces
- IP stacks
- glue logic between IP stacks and network interfaces

MbedOS support following IP stacks :

- LWIP
- Nanostack

This design refers to only LWIP multihoming support so Nanostack will be omitted.

## Component interaction



![components]

 
# Detailed design

## MbedOS networking class diagram extension proposal
![class_diagram]

## Simplified LWIP and EMAC

![simply_emac]

![simply_lwip]

	NetworkInterface *net;
	net = NetworkInterface::get_default_instance();
	net->connect();

**EMACInterface::connect** will call:

	- Stack -> add interface.
	- Stack::Interface ->bringup.

## LWIPInterface changes

LWIP and is a top class responsible for LWIP network stack implementation in Mbed OS.
**LWIP/inner Interface**  class view is shown below. 
For better readability  view is simplified.  

![lwip_class]

It's existing **EMAC only** version.

For clear view all  members from  **LWIP/inner Interface** are removed except **EMAC** related ones which  are important from multi interface point of view. 

![lwip_changes_old]

Those members are defined in  **LWIPInterfaceEMAC.cpp** module.
 
To use **non EMAC** drivers for cellular  new members must be add. **Lwip/Interface** after extension is shown below.


![lwip_changes_new]

New **L3IP** related  members will be defined  in  **LWIPInterfaceL3IP.cpp** module.

## EMACInterface and EMAC driver
![EMACInterface]

**EMAC  driver** class should be used to abstract low level access to networking hardware
All operations receive a `void *` hw pointer which an EMAC device provides when
it is registered with a stack. 

Existing **EMAC** class pure virtual definitions are shown below and must be overrided by target HW dedicated class driver derived from it. 

![EMAC]

## L3IPInterface and L3IP driver
**L3IPInterface**  is new helper class dedicated  to cellular connection. It based on existing 
**EMACInterface** and looks  similar to it. However class methods have different implementation eg connect  calls **lwip::add\_l3ip\_interface(**) to add the interface instead of  **lwip::add\_ethernet\_interface**.
Currently the existing interface does not support destruct and remove  drivers. 
So **remove/destruct** functionality is proposed to implement.
 

![L3IPInterface]

**L3IP**  driver abstract class ia also based on **EMAC** class it also looks similar but is bound to cellular driver. 
![L3IP]

## Memory manager
Currently there is memory  manager dedicated to ethernet. 
It does not  require changes however  should be renamed  from **EMACMemoryManager** to non confusing generic name like **NetStackMemoryManager**. Also  memory buffer **emac\_mem\_buf\_t** should folow this change to just **net\_stack\_mem\_buf\_t**.
 
## Network default interface construction



Mbed OS support  automated factories for default interface.

It depends on JSON configuration customized for particular target HW.

To instantiate it  **::get\_default\_instance** must be invoked on user desired interface type.

The top-level  NetworkInterface use the following parameter 


**MBED\_CONF\_TARGET\_NETWORK\_DEFAULT\_INTERFACE\_TYPE**  for creating proper  one : 

- (ETHERNET ) 	EthInterface ::get\_default\_instance()
- (CELLULAR )  	CellularBase ::get\_default\_instance()
- (WIFI     )		WiFiInterface::get\_default\_instance()
- (MESH     )		MeshInterface::get\_default\_instance()


Code for automated construction is in **NetwokInterfaceDefaults.cpp** module.



## Adding interface

Since automated factory suport creation  only  one **default interface**, others  must be constructed and added to LWIP manually.

For  **ethernet/wifi**  first new  interface, **EMACInterface** helper and **EMAC** driver must be instantiated. For this connection types constructors are already implemented

	EthernetInterface(EMAC &emac = EMAC::get_default_instance(),
                      OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance()) : EMACInterface(emac, stack) { }

or

 	OdinWiFiInterface(OdinWiFiEMAC &emac = OdinWiFiEMAC::get_instance(), OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance());


For cellular the situation is a little bit different due no support any ethernet like interfaces (ethernet,wlan). 
In this case constructor looks like below and is not yet implemented. This is new code proposal for new clases L3IPInterface  and L3IP.  


	L3IPInterface(L3IP &l3ip = L3IP::get_default_instance(),OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance());
 

Regardless of  interface type LWIP must be informed about new low level interface.
This can be done by calling **connect()** method on proper helper class( **EMACInterface** or **L3IPInterface**). Connect adds new network interface and   then sets up a connection on  LWIP stack with **bringup()** method. 

First it registers a network interface with the IP stack. Connects **EMAC/L3IP** layer with the IP stack and initializes all the required infrastructure. This function should be called only once for each available interface. 

For **ethernet/wifi** its already implemented as:

	LWIP::add_ethernet_interface(EMAC &emac, bool default_if, OnboardNetworkStack::Interface **interface_out)

Cellular connection doesn't support it yet.  This is  new feature. Proposal is shown below

	LWIP::add_l3ip_interface(L3IP &l3ip, bool default_if, OnboardNetworkStack::Interface **interface_out)


Both methods have the same purpose but must differ in implementation due to cellular handles layer 2 differently and doesn't  support any ethernet like interfaces (ethernet,wlan) and ARP.


Important part of addition new network interface is configuring LWIP  struct  called **netif**.
This is the core part of LWIP.
Simplifed more readable form (removed comments and preprocesors directives)  is shown below.

**netif** is a generic data structure used for all lwIP network interfaces.

	struct netif {
 	struct netif *next;
  	ip_addr_t ip_addr;
  	ip_addr_t netmask;
  	ip_addr_t gw;
  	ip_addr_t ip6_addr[LWIP_IPV6_NUM_ADDRESSES];
 	u8_t ip6_addr_state[LWIP_IPV6_NUM_ADDRESSES]; 
  	u32_t ip6_addr_valid_life[LWIP_IPV6_NUM_ADDRESSES];
  	u32_t ip6_addr_pref_life[LWIP_IPV6_NUM_ADDRESSES];
  	netif_input_fn input;
  	netif_output_fn output;
  	netif_linkoutput_fn linkoutput;
    netif_output_ip6_fn output_ip6;
    netif_status_callback_fn status_callback;
  	netif_status_callback_fn link_callback;
    netif_status_callback_fn remove_callback;
 	void *state;	
  	u8_t ip6_autoconfig_enabled;	
  
	const char*  hostname;
  	u16_t chksum_flags;
  	u16_t mtu;	
    u8_t hwaddr_len;	
    u8_t hwaddr[NETIF_MAX_HWADDR_LEN];
    u8_t flags;
    char name[2];
    u8_t num;
    netif_igmp_mac_filter_fn igmp_mac_filter;
    netif_mld_mac_filter_fn mld_mac_filter;
    u8_t *addr_hint;
    u16_t loop_cnt_current;
    };


Existing member **name[2]** can be used to bind **netif** to socket.
It is get  with **EMAC** class member **get_ifname** from hw driver and have unique 2 char in length name.

Currently following names exists:

	"en"  ethernet	
	"wl"  wifi
	"l6"  mesh
	"lo"  loopback
	"pp"  ppp
etc.
For cellular it can be "cl"

Two character name string is concatenated with 8 bit value containing index which is incremented on each netif addition eg **"wl0"**.
If no multiple interfaces of one type **"en0", "en1","en2"** ... exists, **name[2]** member is sufficient to distinguish between different interface types like** wifi, ethernet** and **cellular**.

This  **netif** struct uses handlers passed by 
 
	-	(emac/l3ip)_low_level_output
	-	(emac/l3ip)_input
	-	(emac/l3ip)_state_change 
	-	(emac/l3ip)_igmp_mac_filter  
	-   (emac/l3ip)_mld_mac_filter 
	-   (emac/l3ip)_if_init 

for bounding 

	-received data from driver  to LWIP input handler
	-LWIP outgoing data to driver for tramsmiting
	-status callback from device driver
	-filters settings

 
From our point of view  important function pointers  are:
 
	-netif_output_fn output  
	-netif_linkoutput_fn linkoutput 

From LWIP documentation we can read.

**"output"**   is called by the IP module when it wants to send a packet on the interface.  It firts  resolves the hardware address, then sends the packet. For ethernet physical layer, this is usually **etharp_output()**. 


**"linkoutput"**  is called by **ethernet_output()** when it wants  to send a packet on the interface. This function outputs the pbuf as-is on the link medium. 


ARP in used in  ethernet and wifi but for Cellular modem  **output** must be bound to low level write handler. In case of cellular this is done by PPP over serial module (**PPPOS**) and currently is bound to **pppos\_netif\_output** with proper  IP4/IP6 flag. This writes data to cellular modem UART based file handler. Due to different implementation of **PPPOS** **"linkoutput"** is not used for cellular. 

To  add all params to **new netif** following function must be called. This is the part of LWIP stack.
	 
 
	 	struct netif * netif_add(struct netif *netif, const ip4_addr_t *ipaddr, const ip4_addr_t *netmask, const ip4_addr_t *gw, void *state, netif_init_fn init, netif_input_fn input)
 


## Removing interface

To remove interface proper method must be called


    LWIP::remove_L3IP_interface(OnboardNetworkStack::Interface *interface)

Currently the **EMACInterface** does not support remove and **EMAC** drivers do not support destruct however implementation of  remove/destruct functionality  is considered for EMAC interface/drivers.


## Connecting Ethernet or Wifi


**EMACInterface** is constructed with **EMAC** driver and (default) onboard network stack.

 

When **EMACInterface::connect()** is called it calls the LWIP::**add\_ethernet\_interface()** from lwip and after that lwip **bringup()** which  activates the interface on lwip.

## Disconnecting Ethernet or Wifi

EMACInterface::**disconnect()** will bring the interface down.

## Connecting Cellular


**L3IPInterface** is constructed with **L3IP** driver and (default) onboard network stack.

 

When **L3IPInterface::connect()** is called it will call the LWIP::**add\_l3ip\_interface()** from lwip and after that lwip **bringup()** which  activates the interface on lwip as in EMACInterface.

## Disconnecting Cellular

**L3IPInterface::disconnect()** will bring the interface down.

 Its destructor will call the LWIP::**L3IP\_remove\_interface()** and remove itself from lwip/lwip **netif*. 

## Addressing

IP address can  provided as **LWIP::Interface::bringup()** parameter as a static IP or DHCP. If DHCP flag is true than address is set ansynchronously as negotiation  finish. Currently static address change after **LWIP::Interface::bringup()** is not used but there is proper setting member implemented for DHCP  so it can be used for static IP change. This is open issue.

For  **L3IPInterface** IPv6 unique identifier for construction link local address can be used similarly as in PPP. Both SLAAC and DHCP can be used. This is also open issue. 


##  LWIP IP core

All outgoing packets  regardles of TCP,UDP or RAW  are procesed by  
**ip(4\6)\_output\_if\_opt\_src** in LWIP IP module.
  

### EMAC based outgoing traffic for ethernet and wifi
For this  interface  **\ip_output\_if\_opt\_src**   calls  **netif->output** bound for low level  ARP (ethernet) and proceed  **netif->linkoutput** bound for ethernet output driver function.

![outgoing_eth]

### PPP based outgoing traffic for cellular
For cellular  **\ip_output\_if\_opt\_src**   calls  **netif->output** bound for low level  ARP (ethernet) or PPP (cellular).
![outgoing_ppp]

### EMAC based incoming traffic for ethernet and wifi
![incoming_emac]

### PPP based incoming traffic for cellular
![incoming_ppp]


### IP Route changes
This LWIP IP netif selection must be extended for  routing to a specific **netif** instead **default one**.


Original **ip4\_route**  finds the appropriate network interface for a given IP address.
It searches the list of network interfaces linearly. A match is found
if the masked IP address of the network interface equals the masked
IP address given to the function.

Passed param dest is the destination IP address for which to find the route
New paremeter **interface\_name** will be add to origin  **ip4\_route**  for improved selection of desired **netif**.

Function returns the proper **netif** responsible for sending data  to reach dest
 
	struct netif * ip4_route(const ip4_addr_t *dest)
	{
 	 struct netif *netif;

	#if LWIP_MULTICAST_TX_OPTIONS
  	/* Use administratively selected interface for multicast by default */
  	if (ip4_addr_ismulticast(dest) && ip4_default_multicast_netif) {
   	 return ip4_default_multicast_netif;
 	 }
	#endif /* LWIP_MULTICAST_TX_OPTIONS */

  	/* iterate through netifs */
  	for (netif = netif_list; netif != NULL; netif = netif->next) {
    /* is the netif up, does it have a link and a valid address? */
    if (netif_is_up(netif) && netif_is_link_up(netif) && !ip4_addr_isany_val(*netif_ip4_addr(netif))) {
      /* network mask matches? */
      if (ip4_addr_netcmp(dest, netif_ip4_addr(netif), netif_ip4_netmask(netif))) {
        /* return netif on which to forward IP packet */
        return netif;
      }
      /* gateway matches on a non broadcast interface? (i.e. peer in a point to point interface) */
      if (((netif->flags & NETIF_FLAG_BROADCAST) == 0) && ip4_addr_cmp(dest, netif_ip4_gw(netif))) {
        /* return netif on which to forward IP packet */
        return netif;
      }
    }
  	}

	#if LWIP_NETIF_LOOPBACK && !LWIP_HAVE_LOOPIF
 	 /* loopif is disabled, looopback traffic is passed through any netif */
  	if (ip4_addr_isloopback(dest)) {
    	/* don't check for link on loopback traffic */
    	if (netif_default != NULL && netif_is_up(netif_default)) {
    	  return netif_default;
   	 }
    	/* default netif is not up, just use any netif for loopback traffic */
    	for (netif = netif_list; netif != NULL; netif = netif->next) {
     	 if (netif_is_up(netif)) {
       	 return netif;
      	}
   	 }
   	 return NULL;
 	 }
	#endif /* LWIP_NETIF_LOOPBACK && !LWIP_HAVE_LOOPIF */

	#ifdef LWIP_HOOK_IP4_ROUTE_SRC
 	 netif = LWIP_HOOK_IP4_ROUTE_SRC(dest, NULL);
  	if (netif != NULL) {
    	return netif;
  	}
	#elif defined(LWIP_HOOK_IP4_ROUTE)
 	 netif = LWIP_HOOK_IP4_ROUTE(dest);
  	if (netif != NULL) {
   	 return netif;
 	 }
	#endif

  	if ((netif_default == NULL) || !netif_is_up(netif_default) || !netif_is_link_up(netif_default) ||
      ip4_addr_isany_val(*netif_ip4_addr(netif_default))) {
    /* No matching netif found and default netif is not usable.
       If this is not good enough for you, use LWIP_HOOK_IP4_ROUTE() */
    LWIP_DEBUGF(IP_DEBUG | LWIP_DBG_LEVEL_SERIOUS, ("ip4_route: No route to %"U16_F".%"U16_F".%"U16_F".%"U16_F"\n",
      ip4_addr1_16(dest), ip4_addr2_16(dest), ip4_addr3_16(dest), ip4_addr4_16(dest)));
    IP_STATS_INC(ip.rterr);
    MIB2_STATS_INC(mib2.ipoutnoroutes);
    return NULL;
  	}

  	return netif_default;
	}


##  Network stack interface

For multiple **netif's** there is need  to get DNS and IP addresses for an **interface\_name**

Currently LWIP class members  has no such parameter

	get_ip_address()
	get_dns_server(int index, SocketAddress *address)
	
Also there is only pointer to one **netif**.

    const struct netif *netif

So  single netif should be replaced with array of netif pointers  and members above must support to  get IP and DNS from proper netif instance.
Therefore new parameter  "interface_name" should be added to **get\_dns\_server** and new member **get\_ip\_address\_if** must be implemented to the **NetworkInterface** class.


 		get_ip_address_if(const char *interface_name)
		get_dns_server(int index, SocketAddress *address, const char *interface_name)

From multihoming point of view it is desirable  to get interface name and select any interface to default one at  runtime. Therefore following new members need to be implemented

		get_interface_name(char *interface_name)
		set_as_default()

Netif **interface\_name** should be added also to  

	   dns_setserver(u8_t numdns, const ip_addr_t *dnsserver,const char *interface_name);



# Sockets changes

For multihoming its needed to bind socket to desired network interface. To perform this new socket option must be add for socket class.

Member for LWIP stack specific socket option settings already exist in LWIP class.
New option  for socket binding to netif   must be implemented inside:


		nsapi_error_t LWIP::setsockopt(nsapi_socket_t handle, int level, int optname, const void *optval, unsigned optlen)


New socket option enum  **NSAPI\_BIND\_NETIF** must be added.

Socket options after changes.

	typedef enum nsapi_socket_option {
    NSAPI_REUSEADDR,         /*!< Allow bind to reuse local addresses */
    NSAPI_KEEPALIVE,         /*!< Enables sending of keepalive messages */
    NSAPI_KEEPIDLE,          /*!< Sets timeout value to initiate keepalive */
    NSAPI_KEEPINTVL,         /*!< Sets timeout value for keepalive */
    NSAPI_LINGER,            /*!< Keeps close from returning until queues empty */
    NSAPI_SNDBUF,            /*!< Sets send buffer size */
    NSAPI_RCVBUF,            /*!< Sets recv buffer size */
    NSAPI_ADD_MEMBERSHIP,    /*!< Add membership to multicast address */
    NSAPI_DROP_MEMBERSHIP,   /*!< Drop membership to multicast address */
    NSAPI_BIND_NETIF,  		 /*!< Biid nefit to socket */
	} nsapi_socket_option_t;

LWIP specific socket  is shown below 

	struct mbed_lwip_socket {
        bool in_use;

        struct netconn *conn;
        struct netbuf *buf;
        u16_t offset;

        void (*cb)(void *);
        void *data;

        // Track multicast addresses subscribed to by this socket
        nsapi_ip_mreq_t *multicast_memberships;
        uint32_t         multicast_memberships_count;
        uint32_t         multicast_memberships_registry;
    };

New binding option should be placed into **netconn** struct.
Descriptor for **netconn** is shown below.


		struct netconn {
  						enum netconn_type type;
  						enum netconn_state state;
  						union {
   	 							struct ip_pcb  *ip;
    							struct tcp_pcb *tcp;
    							struct udp_pcb *udp;
    							struct raw_pcb *raw;
  							  } pcb; 
  						err_t last_err;
  						sys_sem_t op_completed;
  						sys_mbox_t recvmbox;
  						sys_mbox_t acceptmbox;
  						int socket;
  						s32_t send_timeout; 
  						int recv_timeout;
  						int recv_bufsize;
  						int recv_avail;
  						s16_t linger;
 						u8_t flags;
  						size_t write_offset;
  						struct api_msg *current_msg;
  						netconn_callback callback;
						};



LWIP selects the proper netif on IP layer using **ip\_route**.

Currently it uses only ip adress as input parameter so choice is based on ip adress only. To extend choice ctiteria also to interface index **ip\_route** must be modified and  must take desired **interface\_name** as second argument. 
Therefore new member with  information about **netif interface\_name**  bound to current socket  should be placed in the common part of all PCB types.



		#define IP_PCB \
  		ip_addr_t local_ip; \
  		ip_addr_t remote_ip; \
 		u8_t so_options;     \
  		u8_t tos;            \
  		u8_t ttl             \
  		IP_PCB_ADDRHINT		 \
		const char* interface_name -new member     

New   member **char* interface\_name** should be add for binding socket to netif.
     
 
##  DNS changes 

Currently in Nsapi_dns module there is only one array for 5 DNS server adresses.

	 static nsapi_addr_t dns_servers[DNS_SERVERS_SIZE] = {
	    {NSAPI_IPv4, {8, 8, 8, 8}},                             // Google
	    {NSAPI_IPv4, {209, 244, 0, 3}},                         // Level 3
	    {NSAPI_IPv4, {84, 200, 69, 80}},                        // DNS.WATCH
	    {NSAPI_IPv6, {0x20,0x01, 0x48,0x60, 0x48,0x60, 0,0,     // Google
	                  0,0, 0,0, 0,0, 0x88,0x88}},
	    {NSAPI_IPv6, {0x20,0x01, 0x16,0x08, 0,0x10, 0,0x25,     // DNS.WATCH
	                  0,0, 0,0, 0x1c,0x04, 0xb1,0x2f}},
	};
It would be desirable to add storage for interface specific DNS server addresses.

It can be done as below.

 	static nsapi_addr_t dns_servers[NETIF_INDEX][DNS_SERVERS_SIZE]

Add interface name as option to DNS query

To resolve IP adress Netsocket module uses following functions 

 	gethostbyname(const char *name, SocketAddress *address, nsapi_version_t version)
	nsapi_dns_query(NetworkStack *stack, const char *host, SocketAddress *address, nsapi_version_t version)

Query above uses only single dns_servers  array described before. So it is also desired to add interface_name.

	gethostbyname(const char *name, SocketAddress *address, nsapi_version_t version)
	nsapi_dns_query(NetworkStack *stack, const char *host, SocketAddress *address, nsapi_version_t version)


Second one **nsapi\_dns\_query** finally calls  **get\_dns\_server** descirbed before. It has new parameter **interface\_name** for proper **netif** selecting. So it would be logical to add  also **interface\_name** also to:


	DNS::gethostbyname(const char *name, SocketAddress *address, nsapi_version_t version,const char* interface_name)
	nsapi_dns_query(NetworkStack *stack, const char *host, SocketAddress *address, nsapi_version_t version, const char* interface_name)



If  async DNS query  is used  therefore  UDP socket created for this purpose must be assigned for proper interface.  

To perform this following  UDPSocket base class member 
 
	InternetSocket::setsockopt(int level, int optname, const void *optval, unsigned optlen) 
 
is used with parameters:
	
	- optname  NSAPI_BIND_NETIF
	- optval   netif name

It finally calls stack specific implementation for socket option setting.	


# Usage scenario


Currently  Mbed OS can construct  only one **default** interface.

Its type depends on configuration file and **MBED\_CONF\_TARGET_NETWORK\_DEFAULT\_INTERFACE\_TYPE**  value. 

Example of default network init code is shown below

	NetworkInterface *net;
	net = NetworkInterface::get_default_instance();
	net->connect();

If it's set to ETHERNET following code  constructs **EthInterface** for ethernet EMAC.

If is set to  WIFI   **WiFiInterface**  instance  is constructed.

Similarly  if type is CELLULAR  **EasyCellularConnection** instance is created.

Second or third interface can be added manually

	NetworkInterface *wifi_net;
	wifi_net = WiFiInterface::get_target_default_instance();
	wifi_net->connect();

or

	NetworkInterface *cellular_net;
	cellular_net = EasyCellularConnection::get_target_default_instance();
	cellular_net->connect();

To construct network instance both Ethernet and WiFi uses EMAC and  EMACInterface internally. 

 IP layer 3 interface can be created following way:  

   	NetworkInterface *l3interface;
  	l3interface =new L3IPInterface(L3IP::get_default_instance(), OnboardNetworkStack::get_default_instance());
    l3interface->connect();

[high_level_architecture]: hi_level.png
[components]: components.png
[class_diagram]:class_diagram.png
[lwip_class]: lwip_class.png
[lwip_changes_old]:lwip_changes_old.png
[lwip_changes_new]:lwip_changes_new.png
[EMACInterface]:EMACInterface.png
[EMAC]:EMAC.png
[L3IPInterface]:L3IPInterface.png
[L3IP]:L3IP.png
[outgoing_eth]:outgoing_eth.png
[outgoing_ppp]:outgoing_ppp.png
[incoming_emac]:incoming_emac.png
[incoming_ppp]:incoming_ppp.png
[simply_emac]:EmacInterface_simple.png
[simply_lwip]:lwip_simply.png