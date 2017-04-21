Introduction to Thread
============

The Thread stack is an open standard for reliable, cost-effective, low-power, wireless device-to-device (D2D) communication. It is designed specifically for Connected Home applications where IP-based networking is desired and a variety of application layers can be used on the stack.

These are the general characteristics of the Thread stack and network:

* Simple network installation, start-up and operation: The simple protocols for forming, joining, and maintaining Thread networks allow systems to self-configure and fix routing problems as they occur.

* Secure: Devices do not join the Thread network unless authorized and all communications are encrypted and secure.
	
* Small and large networks: Home networks vary from several devices to hundreds of devices communicating seamlessly. The network layer is designed to optimize the network operation based on the expected use.

* Range: Typical devices in conjunction with mesh networking provide sufficient range to cover a normal home. Spread spectrum technology is used at the physical layer to provide good immunity to interference.

* No single point of failure: The stack is designed to provide secure and reliable operations even with the failure or loss of individual devices.
	
* Low power: Host devices can typically operate for several years on AA type batteries using suitable duty cycles.

The Thread stack runs on the top of 6LoWPAN stack and hence, compliments the ARM mbed IPv6/6LoWPAN stack functionality providing added security, control, reliability, and robustness. 

**Figure 2-1 Thread network protocol architecture**

![nw-arc](img/Thread_Prot_Arch_Comb.png)




