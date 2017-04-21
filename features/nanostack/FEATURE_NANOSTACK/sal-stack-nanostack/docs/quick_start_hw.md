Configuring hardware
====================

## Selecting your radio

6LoWPAN network uses IEEE 802.15.4 radios and therefore, operates on one of the following unlicensed
frequency bands:

* 868.0–868.6 MHz: Europe, allows one communication channel (2003, 2006, 2011[4])
* 902–928 MHz: North America, up to ten channels (2003), extended to thirty (2006)
* 2400–2483.5 MHz: worldwide use, up to sixteen channels (2003, 2006)

The data rate varies from 20 kbit/s to 250 kbit/s. The data rate available per node should be considered
when designing the application. Basically, the data rate is divided between all nodes in the network.
Roughly half of the channel capacity should be allocated for signalling purposes, and each hop requires
retransmisson of the packet.

![Datarate](img/bw.png)

<span style="background-color:#E6E6E6;border:1px solid #000;display:block; height:100%; padding:10px">**Rule of thumb:** The bandwidth per node is divided by the number of nodes in the network and the number of hops.</span>

### FHSS

Frequency Hopping Spread Spectrum (FHSS) is a method of transmitting radio signals by rapidly switching the carrier among many frequency channels, using a pseudorandom sequence known to both the transmitter and receiver. It makes the network more tolerant for narrow band interference and is sometimes required by the FCC rules.

In Nanostack, the FHSS implementation utilizes the standard 802.15.4 radios and switches RX/TX channels in a calculated sequence.
It does not modify the 802.15.4 radio frames in any way, and thus, does not require any special support from the RF chip.


