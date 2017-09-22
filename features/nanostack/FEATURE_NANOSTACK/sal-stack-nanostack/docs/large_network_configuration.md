# Nanomesh 2.1. configuration for GE Lighting network

This document describes the adjustable network features in Nanomesh 2.1. It also lists the default values for each feature. These values have been tested in a simulator (up to 600 nodes/gateway) and a test network with 32 nodes and 3 gateways. The test network is located indoors (almost all nodes and gateways in radio range = all nodes and routers within 1 hop radio range). Networks up to 3 hops.

Testing with a large network (100-600 nodes) is essential to find out the best working configuration in a network. The reference setup is a starting point for testing. The only value that needs to be set in the beginning is `LB_EXPECTED_NODE_COUNT`. That should be the maximum number of nodes per one router in a balanced network. See [Load Balancing](#load-balancing) for further details.

Formula: `LB_EXPECTED_NODE_COUNT` = The total number of nodes in one network (networkID) divided by the number of gateways.
This is a good value for a network where the nodes and gateways are quite evenly distributed. 

The **Mesh Link Establishment (MLE)** parameters should be adjusted if a node runs out of memory, because it can hear too many other nodes and try to store too many neighbours. The default value should be good because the test network has 32 nodes and the neighbour limit is 20 nodes, all nodes hearing each other.

The **IPv6 Routing Protocol for Low-Power and Lossy Networks (RPL)** and **Neighbour Discovery (ND)** parameters should be adjusted if there are too many network maintenance messages when testing in a large (100-600 nodes) network.

## Load Balancing

Load Balancing balances nodes between border routers in the same network (networkID). The border routers send periodically a DIx message with the DODAG preference. It informs the nodes of the load on the router. The DODAG preference contains a value (with eight steps) which describes the number of nodes compared to `LB_EXPECTED_NODE_COUNT` - how many nodes are connected compared to the maximum expected number of nodes. A node has adjustable min and max thresholds for the DODAG preference and a max probability for network change. The minimum threshold gives 0% probablity for the network change and and maximum threshold gives highest probability. The threshold values are compared against the received DODAG preference minus the current preference. If the probability is too high, the nodes change routers too agressively causing oscillation. The default value is 10%. It can be more if the network routers are located so that only a few nodes are within 1 hop range from them.

**Router** has two configuration values in the `Nanorouter3.cfg` file:

```
* DEVICE_HARD_LIMIT=0  // default hard limit off = 0, hard limit number of nodes ON when > 0 and this value is the max number of connected nodes
* LB_EXPECTED_NODE_COUNT=304  // this number of nodes gives the worst DODAG preference, default 304 nodes
```

`LB_EXPECTED_NODE_COUNT` = Total number of nodes in the network/routers in the network. This can be smaller in a router that has nodes that are very far from other routers. If you use the hard limit, it should be high enough. If some of routers in the network are down the remaining routers can then handle all nodes. 

**Node** has the following adjustable parameters in the `static_config.h` file:

    {"LB_MAX_PROBABILITY",              NULL,               10U },  // 1 - 100,  10% default
    {"LB_THRESHOLD_MIN",                NULL,               32U },  // don't change
    {"LB_THRESHOLD_MAX",                NULL,               64U },  // don't change
    
You should not change the min and max values because it is difficult to predict the behaviour of the load balance if they are changed. These values have been tested in the simulator. The probability is the best way to adjust how agressively the nodes change the routers.


## Frequency Hopping (FHSS)

Nanomesh 2.x adjustable settings:

* The total number of channels.
* The number of broadcast channels (max 255 channels).
* Channel change interval.
* Channel mask -> individual channels can be removed/added to FHSS.
* The number of TX/RX slots in one channel.
* Full (source code) access to radio settings (radio packet size, base frequency, baudrate, channel width, modulation).

The total number of channels affects the probablity of changing the load balancing gateway. The more channels there are available the smaller is the probability of receiving advertisements from another gateway. The number of broadcast channels should be 1/5 of the total number of channels (50 channels require 10 broadcast channels). If this ratio is changed, some changes may be necesssary to RPL and multicast OTA.

The router and the node must have the same channel mask and and the same driver side configuration (radio packet size, base frequency, baudrate, channel width, modulation). 

The router sets most of the FHSS parameters:

* The total number of channels (max 255 channels). Default 25 channels.
* The number of broadcast channels (max 255 channels). Default number of channels/5 = 5
* Channel mask (which channels out of the 255 possible are used). Default 1-25.
* Channel change interval. Default 0.4 sec.
* Number of TX/RX slots in one channel. Default 2.

**Node**

You need to set only the FHSS/SINGLE CHANNEL mode and the channels used in the node. Verify that the values match with router's values.
You can select the FHSS/SINGLE_CHANNEL in the `static_config.h` file:

```
// define APP_CHANNEL_PAGE    CHANNEL_PAGE_10
// Single channel by default uses 0, FHSS channels 0-49
static uint32_t channel_mask_10_sc[] = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001};
static uint32_t channel_mask_10_fhss[] = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0007ffff, 0xffffffff};
static conf_t static_config[] = {
	  	{"CHANNEL_MODE", "FHSS", 0},  // replase FHSS with SINGLE_CHANNEL if single channel is selected
```
		

The RF driver files set the radio packet size, base frequency, baudrate, channel width, modulation. See the `rf_configuration.h` file.

**Router**

The `Nanorouter3.cfg` file sets the following:

* The total number of channels.
* The number of broadcast channels.
* Channel change interval.
* Channel mask -> individual channels can be removed/added to FHSS.
* The number of TX/RX slots in one channel.

The RF driver files (MSP project) set the radio packet size, base frequency, baudrate, channel width, modulation.

The `rf_configuration.h`file:

Network channel used, max 255 channels bit mask (example for 50 channels starting from channel 0).

```
BR_CHANNEL_PAGE = 10
BR_CHANNEL_MASK_7 = 0x00000000
BR_CHANNEL_MASK_6 = 0x00000000
BR_CHANNEL_MASK_5 = 0x00000000
BR_CHANNEL_MASK_4 = 0x00000000
BR_CHANNEL_MASK_3 = 0x00000000
BR_CHANNEL_MASK_2 = 0x00000000
BR_CHANNEL_MASK_1 = 0x0007ffff
BR_CHANNEL_MASK_0 = 0xffffffff
```

- The number of set bits gives the number of channels in use, 1 = channel is used, 0 = channel is not used
- Each channel mask represents 24 channels (for example, BR_CHANNEL_MASK_0 is channels 0 - 23, BR_CHANNEL_MASK_1 is channels 24 - 47)
- lsb of BR_CHANNEL_MASK_0 is channel 0
- each bit is one channel

_An example of the frequency setup:_

Channels 2 - 23 excluding channels 4 and 23: `BR_CHANNEL_MASK_0 = 0x7ffffffe`, all other BR_CHANNEL_MASK_Xs are 0x00000000.

```
CHANNEL_MODE = "FHSS"   \# "SINGLE_CHANNEL"
FHSS_NUMBER_OF_BC_CHANNELS = 10
\# number of broadcast channels, number of channels = N * number of broadcast channels
FHSS_NUMBER_OF_TX_SLOTS = 2
\# number of TX slots in one channel = num of RX slots
FHSS_SUPERFRAME_LENGTH = 50000
\# unit us -> 50 ms, shortest lenght of one TX slot,  64 us is MSP's accuracy
FHSS_NUMBER_OF_SUPERFRAMES = 8
\# number of superframes in one channel -> 400 ms, minimum value 2* FHSS_NUMBER_OF_TX_SLOTS 
```

## Radio packet length

The Nanostack library sets the max length of a 6LoWPAN packet (< 1280 bytes). The radio packet must be shorter. 

- Default radio packet size: 600 bytes.
- In FHSS, the length of the TX slot sets the limit.
- Default 100 ms TX slots (CCA), 250 kbaud -> max 800 bytes.

**Node**

The radio drivers set the radio packet size.

**Router**

The router's radio MCU drivers set the radio packet size. You also must set the router's (Linux) TUN SPI packet size to radio packet size. TUN SPI is the connnection between router's radio and router's Linux core.

**Router configuration (Linux)**

`driverTunSpi.h`:

```
\#define RF_PHY_MTU  255
\#define TUN_SPI_BLOCK_SIZE  265 // must be > PHY_MTU_SIZE+9
```

**Router configuration (MSP430)**

```
driverRFPhy.c: static uint8_t rf_rx_buffer[PHY_MTU_SIZE];
driverRFPhy.h: 
\#define PHY_MTU_SIZE     255
```

`driverTunSpi.h`:

```
\#define TUN_SPI_BLOCK_SIZE  265  // must be > PHY_MTU_SIZE+9
```

`driverTunSpi.c_`:

```
static uint8_t tun_spi_tx_buffer[TUN_SPI_BLOCK_SIZE];
static uint8_t tun_spi_rx_buffer[TUN_SPI_BLOCK_SIZE];
```

**Node configuration**

`driverRFPhy.h_`:

```
\#define PHY_MTU_SIZE     255
driverRFPhy.c: 
static uint8_t rf_rx_buffer[PHY_MTU_SIZE];
```


## MLE

Security counters are synced using MLE three-way handshake. This provides protection against replay attacks.
MLE maintains the neighbour link quality using ETX (estimated packet send success probability).

* Sharing ETX data between neighbours to get two-way link quality.
* Using ETX to measure the link quality to neighbors and to select parents in RPL.
* Selecting a new RPL parent if the MLE link to a parent is lost.

Both the router and the node have a setting for the time the neighbours are held in memory. This value must be the same for both.

**Router** 

In the `nanorouter3.cfg` file:

`LOWPAN_ROUTER_DEFAULT_LIFETIME=1200`

**Node** 

In the `static_config.h` file:

`{"LOWPAN_ROUTER_DEFAULT_LIFETIME",  NULL, 1200},`

The node also limits the number of neighbours (it has a limited amount of memory to store neighbours). This is set in the `node_tasklet.c`file:

```
//SET MLE neighbour limits
arm_nwk_6lowpan_mle_neighbor_limits_set(net_rf_id, 8, 15, 20);
```

To set the MLE neighbor list limits:

```
int8_t arm_nwk_6lowpan_mle_neighbor_limits_set(int8_t interface_id, uint16_t lower_threshold, uint16_t upper_threshold, uint16_t max);
```

where

* `interface_id` = Interface ID.
* `lower_threshold` = Lower threshold. 5 to 499. 0 limits not used.
* `upper_threshold` = Upper threshold. 5 to 500. 0 limits not used.
* `max`=  The maximum number of neighbours. 5 to 500. 0 limits not used.
* return 0 when the limits update is OK, < 0 when the update fails.

The MLE neighbor limits settings limit the number of neighbours added to the MLE neighbour list.

- If the number of neighbours reaches the lower threshold, MLE starts to limit the addition of new neighbors. Multicast MLE messages from unknown neighbours are ignored (the probability for ignore is randomized). The value must be smaller than the upper threshold and maximum value. 

- If the number of neighbours reaches the upper threshold, MLE stops adding new neighbours based on the multicast MLE messages. Only the nodes that select this node for a parent during the bootstrap will be accepted. The value must be smaller than or the same as the maximum value. 

- If the number of neighbours reaches the maximum value, no new neighbours are added. If the MLE neighbor list limits are not used, all values must be set to 0.


## RPL

This affects the load balance because it sets the interval of the DODAG preference messages.

**Router** 

In the `nanorouter3.cfg` file:

```
BR_DAG_MAX_RANK_INC = 2048  // RPL MaxRankIncrease
BR_DAG_SEC_PCS = 2 // Path Control Size limits number of DAO parents.
BR_DAG_DIO_INT_DOUB = 8  // RPL Trickle DIOIntervalDoublings
BR_DAG_DIO_INT_MIN = 11  // RPL Trickle DIOIntervalMin
BR_LIFE_IN_SECONDS = 68  // Default lifetime for RPL routes, expressed in lifetime units
```

## ND

ND improves the network healing. The node uses a ND probe if it has lost the link to its parent before starting the network search. 

**Node** 

In the `main.c` file:

```
    nd_parameters_s nd_params;
    net_6lowpan_nd_parameter_read(&nd_params);
    nd_params.send_nud_probes = true;
    nd_params.ra_reachable_time = 300000; // Defines the value of reachable time placed in Router Advertisements (in milliseconds).
    nd_params.ra_retrans_timer = 5000; // Defines the value of retransfer timer placed in Router Advertisements (in milliseconds).
    net_6lowpan_nd_parameter_set(&nd_params);
```
