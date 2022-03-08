/* ===================================================================================
 * Copyright (c) <2009> Synopsys, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software annotated with this license and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including without
 * limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * =================================================================================== */


/** \file
 * This is the network dependent layer to handle network related functionality.
 * This file is tightly coupled to neworking frame work of linux 2.6.xx kernel.
 * The functionality carried out in this file should be treated as an example only
 * if the underlying operating system is not Linux.
 *
 * \note Many of the functions other than the device specific functions
 *  changes for operating system other than Linux 2.6.xx
 * \internal
 *-----------------------------REVISION HISTORY-----------------------------------
 * Synopsys			01/Aug/2007				Created
 */

#include "string.h"
#include "synopGMAC_network_interface.h"


synopGMACdevice GMACdev[GMAC_CNT];
static DmaDesc tx_desc[GMAC_CNT][TRANSMIT_DESC_SIZE] __attribute__ ((aligned (64)));
static DmaDesc rx_desc[GMAC_CNT][RECEIVE_DESC_SIZE] __attribute__ ((aligned (64)));

//static struct sk_buff tx_buf[GMAC_CNT][TRANSMIT_DESC_SIZE] __attribute__ ((aligned (64)));
//static struct sk_buff rx_buf[GMAC_CNT][RECEIVE_DESC_SIZE] __attribute__ ((aligned (64)));
struct sk_buff tx_buf[GMAC_CNT][TRANSMIT_DESC_SIZE] __attribute__ ((aligned (64)));
struct sk_buff rx_buf[GMAC_CNT][RECEIVE_DESC_SIZE] __attribute__ ((aligned (64)));

// These 2 are accessable from application
struct sk_buff txbuf[GMAC_CNT] __attribute__ ((aligned (64))); // set align to separate cacheable and non-cacheable data to different cache line.
struct sk_buff rxbuf[GMAC_CNT] __attribute__ ((aligned (64)));

u8 mac_addr0[6] = DEFAULT_MAC0_ADDRESS;
u8 mac_addr1[6] = DEFAULT_MAC1_ADDRESS;

//static struct timer_list synopGMAC_cable_unplug_timer;
//static u32 GMAC_Power_down; // This global variable is used to indicate the ISR whether the interrupts occured in the process of powering down the mac or not


/**
 * Function used to detect the cable plugging and unplugging.
 * This function gets scheduled once in every second and polls
 * the PHY register for network cable plug/unplug. Once the
 * connection is back the GMAC device is configured as per
 * new Duplex mode and Speed of the connection.
 * @param[in] u32 type but is not used currently.
 * \return returns void.
 * \note This function is tightly coupled with Linux 2.6.xx.
 * \callgraph
 */



void synopGMAC_powerdown_mac(synopGMACdevice *gmacdev)
{
    TR0("Put the GMAC to power down mode..\n");
    // Disable the Dma engines in tx path
    gmacdev->GMAC_Power_down = 1;	// Let ISR know that Mac is going to be in the power down mode
    synopGMAC_disable_dma_tx(gmacdev);
    plat_delay(10000);		//allow any pending transmission to complete
    // Disable the Mac for both tx and rx
    synopGMAC_tx_disable(gmacdev);
    synopGMAC_rx_disable(gmacdev);
    plat_delay(10000); 		//Allow any pending buffer to be read by host
    //Disable the Dma in rx path
    synopGMAC_disable_dma_rx(gmacdev);

    //enable the power down mode
    //synopGMAC_pmt_unicast_enable(gmacdev);

    //prepare the gmac for magic packet reception and wake up frame reception
    synopGMAC_magic_packet_enable(gmacdev);

    //gate the application and transmit clock inputs to the code. This is not done in this driver :).

    //enable the Mac for reception
    synopGMAC_rx_enable(gmacdev);

    //Enable the assertion of PMT interrupt
    synopGMAC_pmt_int_enable(gmacdev);
    //enter the power down mode
    synopGMAC_power_down_enable(gmacdev);
    return;
}

void synopGMAC_powerup_mac(synopGMACdevice *gmacdev)
{
    gmacdev->GMAC_Power_down = 0;	// Let ISR know that MAC is out of power down now
    if( synopGMAC_is_magic_packet_received(gmacdev))
        TR("GMAC wokeup due to Magic Pkt Received\n");
    if(synopGMAC_is_wakeup_frame_received(gmacdev))
        TR("GMAC wokeup due to Wakeup Frame Received\n");
    //Disable the assertion of PMT interrupt
    synopGMAC_pmt_int_disable(gmacdev);
    //Enable the mac and Dma rx and tx paths
    synopGMAC_rx_enable(gmacdev);
    synopGMAC_enable_dma_rx(gmacdev);

    synopGMAC_tx_enable(gmacdev);
    synopGMAC_enable_dma_tx(gmacdev);
    return;
}

/**
  * This sets up the transmit Descriptor queue in ring or chain mode.
  * This function is tightly coupled to the platform and operating system
  * Device is interested only after the descriptors are setup. Therefore this function
  * is not included in the device driver API. This function should be treated as an
  * example code to design the descriptor structures for ring mode or chain mode.
  * This function depends on the pcidev structure for allocation consistent dma-able memory in case of linux.
  * This limitation is due to the fact that linux uses pci structure to allocate a dmable memory
  *	- Allocates the memory for the descriptors.
  *	- Initialize the Busy and Next descriptors indices to 0(Indicating first descriptor).
  *	- Initialize the Busy and Next descriptors to first descriptor address.
  * 	- Initialize the last descriptor with the endof ring in case of ring mode.
  *	- Initialize the descriptors in chain mode.
  * @param[in] pointer to synopGMACdevice.
  * @param[in] pointer to pci_device structure.
  * @param[in] number of descriptor expected in tx descriptor queue.
  * @param[in] whether descriptors to be created in RING mode or CHAIN mode.
  * \return 0 upon success. Error code upon failure.
  * \note This function fails if allocation fails for required number of descriptors in Ring mode, but in chain mode
  *  function returns -ESYNOPGMACNOMEM in the process of descriptor chain creation. once returned from this function
  *  user should for gmacdev->TxDescCount to see how many descriptors are there in the chain. Should continue further
  *  only if the number of descriptors in the chain meets the requirements
  */

s32 synopGMAC_setup_tx_desc_queue(synopGMACdevice * gmacdev,u32 no_of_desc, u32 desc_mode)
{
    s32 i;

    DmaDesc *first_desc = &tx_desc[gmacdev->Intf][0];
    dma_addr_t dma_addr;
    gmacdev->TxDescCount = 0;

	TR("Total size of memory required for Tx Descriptors in Ring Mode = 0x%08x\n",((sizeof(DmaDesc) * no_of_desc)));

	gmacdev->TxDescCount = no_of_desc;
	gmacdev->TxDesc      = first_desc;
#ifdef CACHE_ON
	gmacdev->TxDescDma   = (DmaDesc *)((u64)first_desc | 0x100000000);
#else
	gmacdev->TxDescDma   = (DmaDesc *)((u64)first_desc);
#endif
	for(i =0; i < gmacdev -> TxDescCount; i++) {
		synopGMAC_tx_desc_init_ring(gmacdev->TxDescDma + i, i == gmacdev->TxDescCount-1);
		TR("%02d %08x \n",i, (unsigned int)(gmacdev->TxDesc + i) );
	}


    gmacdev->TxNext = 0;
    gmacdev->TxBusy = 0;
    gmacdev->TxNextDesc = gmacdev->TxDesc;
    gmacdev->TxBusyDesc = gmacdev->TxDesc;
    gmacdev->BusyTxDesc  = 0;

    return 0;
}


/**
  * This sets up the receive Descriptor queue in ring or chain mode.
  * This function is tightly coupled to the platform and operating system
  * Device is interested only after the descriptors are setup. Therefore this function
  * is not included in the device driver API. This function should be treated as an
  * example code to design the descriptor structures in ring mode or chain mode.
  * This function depends on the pcidev structure for allocation of consistent dma-able memory in case of linux.
  * This limitation is due to the fact that linux uses pci structure to allocate a dmable memory
  *	- Allocates the memory for the descriptors.
  *	- Initialize the Busy and Next descriptors indices to 0(Indicating first descriptor).
  *	- Initialize the Busy and Next descriptors to first descriptor address.
  * 	- Initialize the last descriptor with the endof ring in case of ring mode.
  *	- Initialize the descriptors in chain mode.
  * @param[in] pointer to synopGMACdevice.
  * @param[in] pointer to pci_device structure.
  * @param[in] number of descriptor expected in rx descriptor queue.
  * @param[in] whether descriptors to be created in RING mode or CHAIN mode.
  * \return 0 upon success. Error code upon failure.
  * \note This function fails if allocation fails for required number of descriptors in Ring mode, but in chain mode
  *  function returns -ESYNOPGMACNOMEM in the process of descriptor chain creation. once returned from this function
  *  user should for gmacdev->RxDescCount to see how many descriptors are there in the chain. Should continue further
  *  only if the number of descriptors in the chain meets the requirements
  */
s32 synopGMAC_setup_rx_desc_queue(synopGMACdevice * gmacdev,u32 no_of_desc, u32 desc_mode)
{
    s32 i;

    DmaDesc *first_desc = &rx_desc[gmacdev->Intf][0];
    dma_addr_t dma_addr;
    gmacdev->RxDescCount = 0;


	TR("total size of memory required for Rx Descriptors in Ring Mode = 0x%08x\n",((sizeof(DmaDesc) * no_of_desc)));

	gmacdev->RxDescCount = no_of_desc;
	gmacdev->RxDesc      = first_desc;
#ifdef CACHE_ON
	gmacdev->RxDescDma   = (DmaDesc *)((u64)first_desc | 0x100000000);
#else
	gmacdev->RxDescDma   = (DmaDesc *)((u64)first_desc);
#endif
	for(i =0; i < gmacdev -> RxDescCount; i++) {
		synopGMAC_rx_desc_init_ring(gmacdev->RxDescDma + i, i == gmacdev->RxDescCount-1);
		TR("%02d %08x \n",i, (unsigned int)(gmacdev->RxDesc + i));
	}


    gmacdev->RxNext = 0;
    gmacdev->RxBusy = 0;
    gmacdev->RxNextDesc = gmacdev->RxDesc;
    gmacdev->RxBusyDesc = gmacdev->RxDesc;

    gmacdev->BusyRxDesc   = 0;

    return 0;
}


/**
  * This gives up the receive Descriptor queue in ring or chain mode.
  * This function is tightly coupled to the platform and operating system
  * Once device's Dma is stopped the memory descriptor memory and the buffer memory deallocation,
  * is completely handled by the operating system, this call is kept outside the device driver Api.
  * This function should be treated as an example code to de-allocate the descriptor structures in ring mode or chain mode
  * and network buffer deallocation.
  * This function depends on the pcidev structure for dma-able memory deallocation for both descriptor memory and the
  * network buffer memory under linux.
  * The responsibility of this function is to
  *     - Free the network buffer memory if any.
  *	- Fee the memory allocated for the descriptors.
  * @param[in] pointer to synopGMACdevice.
  * @param[in] pointer to pci_device structure.
  * @param[in] number of descriptor expected in rx descriptor queue.
  * @param[in] whether descriptors to be created in RING mode or CHAIN mode.
  * \return 0 upon success. Error code upon failure.
  * \note No referece should be made to descriptors once this function is called. This function is invoked when the device is closed.
  */
void synopGMAC_giveup_rx_desc_queue(synopGMACdevice * gmacdev, u32 desc_mode)
{
	gmacdev->RxDesc    = NULL;
	gmacdev->RxDescDma = 0;
	return;
}

/**
  * This gives up the transmit Descriptor queue in ring or chain mode.
  * This function is tightly coupled to the platform and operating system
  * Once device's Dma is stopped the memory descriptor memory and the buffer memory deallocation,
  * is completely handled by the operating system, this call is kept outside the device driver Api.
  * This function should be treated as an example code to de-allocate the descriptor structures in ring mode or chain mode
  * and network buffer deallocation.
  * This function depends on the pcidev structure for dma-able memory deallocation for both descriptor memory and the
  * network buffer memory under linux.
  * The responsibility of this function is to
  *     - Free the network buffer memory if any.
  *	- Fee the memory allocated for the descriptors.
  * @param[in] pointer to synopGMACdevice.
  * @param[in] pointer to pci_device structure.
  * @param[in] number of descriptor expected in tx descriptor queue.
  * @param[in] whether descriptors to be created in RING mode or CHAIN mode.
  * \return 0 upon success. Error code upon failure.
  * \note No reference should be made to descriptors once this function is called. This function is invoked when the device is closed.
  */
void synopGMAC_giveup_tx_desc_queue(synopGMACdevice * gmacdev, u32 desc_mode)
{


	gmacdev->TxDesc    = NULL;
	gmacdev->TxDescDma = 0;
	return;
}

/**
 * Function to handle housekeeping after a packet is transmitted over the wire.
 * After the transmission of a packet DMA generates corresponding interrupt
 * (if it is enabled). It takes care of returning the sk_buff to the linux
 * kernel, updating the networking statistics and tracking the descriptors.
 * @param[in] pointer to net_device structure.
 * \return void.
 * \note This function runs in interrupt context
 */
void synop_handle_transmit_over(int intf)
{
    synopGMACdevice * gmacdev;
    s32 desc_index;
    u32 data1;
    u32 status;
    u32 length1;
    u32 dma_addr1;

    u32 ext_status;
    //u16 time_stamp_higher;
    u32 time_stamp_high;
    u32 time_stamp_low;


    gmacdev = &GMACdev[intf];

    /*Handle the transmit Descriptors*/
    do {

        desc_index = synopGMAC_get_tx_qptr(gmacdev, &status, &dma_addr1, &length1, &data1,&ext_status,&time_stamp_high,&time_stamp_low);
        //synopGMAC_TS_read_timestamp_higher_val(gmacdev, &time_stamp_higher);

        if(desc_index >= 0 /*&& data1 != 0*/) {
            TR("Finished Transmit at Tx Descriptor %d for skb 0x%08x and buffer = %08x whose status is %08x \n", desc_index,data1,dma_addr1,status);

            if(synopGMAC_is_tx_ipv4header_checksum_error(gmacdev, status)) {
                TR("Harware Failed to Insert IPV4 Header Checksum\n");
                gmacdev->synopGMACNetStats.tx_ip_header_errors++;
            }
            if(synopGMAC_is_tx_payload_checksum_error(gmacdev, status)) {
                TR("Harware Failed to Insert Payload Checksum\n");
                gmacdev->synopGMACNetStats.tx_ip_payload_errors++;
            }



            if(synopGMAC_is_desc_valid(status)) {
            	gmacdev->synopGMACNetStats.tx_bytes += length1;
            	gmacdev->synopGMACNetStats.tx_packets++;
            	if(status & DescTxTSStatus) {
            		gmacdev->tx_sec = time_stamp_high;
            		gmacdev->tx_subsec = time_stamp_low;
            	} else {
            		gmacdev->tx_sec = 0;
            		gmacdev->tx_subsec = 0;
            	}
            } else {
                TR("Error in Status %08x\n",status);
                gmacdev->synopGMACNetStats.tx_errors++;
                gmacdev->synopGMACNetStats.tx_aborted_errors += synopGMAC_is_tx_aborted(status);
                gmacdev->synopGMACNetStats.tx_carrier_errors += synopGMAC_is_tx_carrier_error(status);
            }
        }
        gmacdev->synopGMACNetStats.collisions += synopGMAC_get_tx_collision_count(status);
    } while(desc_index >= 0);

}




/**
 * Function to Receive a packet from the interface.
 * After Receiving a packet, DMA transfers the received packet to the system memory
 * and generates corresponding interrupt (if it is enabled). This function prepares
 * the sk_buff for received packet after removing the ethernet CRC, and hands it over
 * to linux networking stack.
 * 	- Updataes the networking interface statistics
 *	- Keeps track of the rx descriptors
 * @param[in] pointer to net_device structure.
 * \return void.
 * \note This function runs in interrupt context.
 */
extern DmaDesc * prevtx;  // for CRC test
s32 synop_handle_received_data(int intf, u8 **buf)      // Chris, to get RX buffer pointer
{

    synopGMACdevice * gmacdev;
    s32 desc_index;

    u32 data1;
    u32 len = 0;
    u32 status;
    u32 dma_addr1;

    u32 ext_status;
    //u16 time_stamp_higher;
    u32 time_stamp_high;
    u32 time_stamp_low;
    struct sk_buff *rb = &rxbuf[intf];

    //struct sk_buff *skb; //This is the pointer to hold the received data

    TR("%s\n",__FUNCTION__);



    gmacdev = &GMACdev[intf];

    /*Handle the Receive Descriptors*/
    do {

        desc_index = synopGMAC_get_rx_qptr(gmacdev, &status,
        		                           &dma_addr1, NULL, &data1,
										   &ext_status, &time_stamp_high, &time_stamp_low);
        if(desc_index >0) {
            //synopGMAC_TS_read_timestamp_higher_val(gmacdev, &time_stamp_higher);
            //TR("S:%08x ES:%08x DA1:%08x d1:%08x TSH:%08x TSL:%08x TSHW:%08x \n",status,ext_status,dma_addr1, data1,time_stamp_high,time_stamp_low,time_stamp_higher);
        	TR("S:%08x ES:%08x DA1:%08x d1:%08x TSH:%08x TSL:%08x\n",status,ext_status,dma_addr1, data1,time_stamp_high,time_stamp_low);
        }

        if(desc_index >= 0 /*&& data1 != 0*/) {
            TR("Received Data at Rx Descriptor %d for skb 0x%08x whose status is %08x\n",desc_index,data1,status);


            //skb = (struct sk_buff *)((u64)data1);
            if(1/*synopGMAC_is_rx_desc_valid(status)*/) {
            	// Always enter this loop. synopGMAC_is_rx_desc_valid() also report invalid descriptor
            	// if there's packet error generated by test code and drop it. But we need to execute ext_status
            	// check code to tell what's going on.                                          --ya

                len =  synopGMAC_get_rx_desc_frame_length(status) - 4; //Not interested in Ethernet CRC bytes


                // Now lets check for the IPC offloading
                /*  Since we have enabled the checksum offloading in hardware, lets inform the kernel
                    not to perform the checksum computation on the incoming packet. Note that ip header
                    checksum will be computed by the kernel immaterial of what we inform. Similary TCP/UDP/ICMP
                    pseudo header checksum will be computed by the stack. What we can inform is not to perform
                    payload checksum.
                    When CHECKSUM_UNNECESSARY is set kernel bypasses the checksum computation.
                */

                TR("Checksum Offloading will be done now\n");

                if(synopGMAC_is_ext_status(gmacdev, status)) { // extended status present indicates that the RDES4 need to be probed
                    TR("Extended Status present\n");
                    if(synopGMAC_ES_is_IP_header_error(gmacdev,ext_status)) {      // IP header (IPV4) checksum error
                        //Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
                        TR("(EXTSTS)Error in IP header error\n");
                        gmacdev->synopGMACNetStats.rx_ip_header_errors++;
                    }
                    if(synopGMAC_ES_is_rx_checksum_bypassed(gmacdev,ext_status)) {  // Hardware engine bypassed the checksum computation/checking
                        TR("(EXTSTS)Hardware bypassed checksum computation\n");
                    }
                    if(synopGMAC_ES_is_IP_payload_error(gmacdev,ext_status)) {      // IP payload checksum is in error (UDP/TCP/ICMP checksum error)
                        TR("(EXTSTS) Error in EP payload\n");
                        gmacdev->synopGMACNetStats.rx_ip_payload_errors++;
                    }
                } else { // No extended status. So relevant information is available in the status itself
                    if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxNoChkError ) {
                        TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 4>  \n");
                    }
                    if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrChkError ) {
                        //Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
                        TR(" Error in 16bit IPV4 Header Checksum <Chk Status = 6>  \n");
                        gmacdev->synopGMACNetStats.rx_ip_header_errors++;
                    }
                    if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxLenLT600 ) {
                        TR("IEEE 802.3 type frame with Length field Lesss than 0x0600 <Chk Status = 0> \n");
                    }
                    if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrPayLoadChkBypass ) {
                        TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 1>\n");
                    }
                    if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxChkBypass ) {
                        TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 3>  \n");
                    }
                    if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxPayLoadChkError ) {
                        TR(" TCP/UDP payload checksum Error <Chk Status = 5>  \n");
                        gmacdev->synopGMACNetStats.rx_ip_payload_errors++;
                    }
                    if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrPayLoadChkError ) {
                        //Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
                        TR(" Both IP header and Payload Checksum Error <Chk Status = 7>  \n");
                        gmacdev->synopGMACNetStats.rx_ip_header_errors++;
                        gmacdev->synopGMACNetStats.rx_ip_payload_errors++;
                    }
                }
#if 0
#ifdef CACHE_ON
                memcpy((void *)rb->data, (void *)((u64)dma_addr1 | 0x100000000), len);
#else
                memcpy((void *)rb->data, (void *)((u64)dma_addr1), len);
#endif
                if(prevtx != NULL) {
#ifdef CACHE_ON
                memcpy((void *)(rb->data + len), (void *)((u64)(dma_addr1 | 0x100000000) + len), 4);
#else
                memcpy((void *)(rb->data + len), (void *)((u64)dma_addr1 + len), 4);
#endif
                }
#else
                *buf = (u8 *)(u32)((u64)dma_addr1);
#endif
                rb->rdy = 1;
                rb->len = len;
                gmacdev->synopGMACNetStats.rx_packets++;
                gmacdev->synopGMACNetStats.rx_bytes += len;
                if(status & DescRxTSAvailable) {
                	gmacdev->rx_sec = time_stamp_high;
                	gmacdev->rx_subsec = time_stamp_low;
                } else {
            		gmacdev->rx_sec = 0;
            		gmacdev->rx_subsec = 0;
                }
            } else {
                /*Now the present skb should be set free*/
            	TR("s: %08x\n",status);
            	gmacdev->synopGMACNetStats.rx_errors++;
            	gmacdev->synopGMACNetStats.collisions       += synopGMAC_is_rx_frame_collision(status);
            	gmacdev->synopGMACNetStats.rx_crc_errors    += synopGMAC_is_rx_crc(status);
            	gmacdev->synopGMACNetStats.rx_frame_errors  += synopGMAC_is_frame_dribbling_errors(status);
            	gmacdev->synopGMACNetStats.rx_length_errors += synopGMAC_is_rx_frame_length_errors(status);
            }
        }
    } while(0); //while(desc_index >= 0);
    return len;
}

u32 volatile LPIStsChange = 0;
u32 volatile LPIReg = 0;
/**
 * Interrupt service routing.
 * This is the function registered as ISR for device interrupts.
 * @param[in] interrupt number.
 * @param[in] void pointer to device unique structure (Required for shared interrupts in Linux).
 * @param[in] pointer to pt_regs (not used).
 * \return Returns IRQ_NONE if not device interrupts IRQ_HANDLED for device interrupts.
 * \note This function runs in interrupt context
 *
 */
void synopGMAC0_intr_handler(void)
{
    synopGMACdevice * gmacdev = &GMACdev[0];
    u32 interrupt,dma_status_reg, mac_status_reg;
    s32 status;
    u32 dma_addr;

    // Check GMAC interrupt
    mac_status_reg = synopGMACReadReg((u32 *)gmacdev->MacBase, GmacInterruptStatus);
    if(mac_status_reg & GmacTSIntSts) {
    	gmacdev->synopGMACNetStats.ts_int = 1;
    	status = synopGMACReadReg((u32 *)gmacdev->MacBase, GmacTSStatus);
    	if(!(status & (1 << 1)))
    		printf("TS alarm flag not set??\n");
    	else
    		printf("TS alarm!!!!!!!!!!!!!!!!\n");

    }
    if(mac_status_reg & GmacLPIIntSts) {
    	//printf("LPI\n");
    	LPIStsChange = 1;
    	LPIReg = synopGMACReadReg((u32 *)gmacdev->MacBase, GmacLPICtrlSts);

    }
    if(mac_status_reg & GmacRgmiiIntSts) {
    	u32 volatile reg;
    	reg = synopGMACReadReg((u32 *)gmacdev->MacBase, GmacRgmiiCtrlSts);

    }
    synopGMACWriteReg((u32 *)gmacdev->MacBase, GmacInterruptStatus ,mac_status_reg);

    /*Read the Dma interrupt status to know whether the interrupt got generated by our device or not*/
    dma_status_reg = synopGMACReadReg((u32 *)gmacdev->DmaBase, DmaStatus);
    //printf("i");
    //printf("i %08x %08x\n", mac_status_reg, dma_status_reg);

    if(dma_status_reg == 0)
        return;

    synopGMAC_disable_interrupt_all(gmacdev);

    TR("%s:Dma Status Reg: 0x%08x\n",__FUNCTION__,dma_status_reg);

    if(dma_status_reg & GmacPmtIntr) {
        TR("%s:: Interrupt due to PMT module\n",__FUNCTION__);
        synopGMAC_powerup_mac(gmacdev);
    }

    if(dma_status_reg & GmacLineIntfIntr) {
        TR("%s:: Interrupt due to GMAC LINE module\n",__FUNCTION__);
    }

    /*Now lets handle the DMA interrupts*/
    interrupt = synopGMAC_get_interrupt_type(gmacdev);
    TR("%s:Interrupts to be handled: 0x%08x\n",__FUNCTION__,interrupt);


    if(interrupt & synopGMACDmaError) {

        TR("%s::Fatal Bus Error Inetrrupt Seen\n",__FUNCTION__);
        synopGMAC_disable_dma_tx(gmacdev);
        synopGMAC_disable_dma_rx(gmacdev);

        synopGMAC_take_desc_ownership_tx(gmacdev);
        synopGMAC_take_desc_ownership_rx(gmacdev);

        synopGMAC_init_tx_rx_desc_queue(gmacdev);

        synopGMAC_reset(gmacdev);//reset the DMA engine and the GMAC ip

        synopGMAC_set_mac_addr(gmacdev,GmacAddr0High,GmacAddr0Low, gmacdev->Intf == 0 ? mac_addr0 : mac_addr1);
        synopGMAC_dma_bus_mode_init(gmacdev,DmaFixedBurstEnable| DmaBurstLength8 | DmaDescriptorSkip0/*DmaDescriptorSkip2*/ );
        synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward);
        synopGMAC_init_rx_desc_base(gmacdev);
        synopGMAC_init_tx_desc_base(gmacdev);
        synopGMAC_mac_init(gmacdev);
        synopGMAC_enable_dma_rx(gmacdev);
        synopGMAC_enable_dma_tx(gmacdev);

    }


    if(interrupt & synopGMACDmaRxNormal) {
        u8 **buf
    	//printf("rx\n");
        TR("%s:: Rx Normal \n", __FUNCTION__);
        synop_handle_received_data(0, buf);     // Chris, to get RX buffer pointer
    }

    if(interrupt & synopGMACDmaRxAbnormal) {
        TR("%s::Abnormal Rx Interrupt Seen\n",__FUNCTION__);
    	gmacdev->synopGMACNetStats.rx_over_errors++;
#if 1

        if(gmacdev->GMAC_Power_down == 0) {	// If Mac is not in powerdown
            synopGMAC_resume_dma_rx(gmacdev);//To handle GBPS with 12 descriptors
        }
#endif
    }

    if(interrupt & synopGMACDmaRxStopped) {
        TR("%s::Receiver stopped seeing Rx interrupts\n",__FUNCTION__); //Receiver gone in to stopped state
#if 1
        if(gmacdev->GMAC_Power_down == 0) {	// If Mac is not in powerdown
        	gmacdev->synopGMACNetStats.rx_over_errors++;
            synopGMAC_enable_dma_rx(gmacdev);
        }
#endif
    }

    if(interrupt & synopGMACDmaTxNormal) {
    	//printf("tx\n");
        //xmit function has done its job
        TR("%s::Finished Normal Transmission \n",__FUNCTION__);
        synop_handle_transmit_over(0);//Do whatever you want after the transmission is over


    }

    if(interrupt & synopGMACDmaTxAbnormal) {
        TR("%s::Abnormal Tx Interrupt Seen\n",__FUNCTION__);
#if 1
        if(gmacdev->GMAC_Power_down == 0) {	// If Mac is not in powerdown
            synop_handle_transmit_over(0);
        }
#endif
    }



    if(interrupt & synopGMACDmaTxStopped) {
        TR("%s::Transmitter stopped sending the packets\n",__FUNCTION__);
#if 1
        if(gmacdev->GMAC_Power_down == 0) {	// If Mac is not in powerdown
            synopGMAC_disable_dma_tx(gmacdev);
            synopGMAC_take_desc_ownership_tx(gmacdev);

            synopGMAC_enable_dma_tx(gmacdev);
            TR("%s::Transmission Resumed\n",__FUNCTION__);
        }
#endif
    }

    /* Enable the interrupt before returning from ISR*/
    synopGMAC_enable_interrupt(gmacdev,DmaIntEnable);
    return;
}

void synopGMAC_set_speed(int intf)
{

    synopGMACdevice *gmacdev = &GMACdev[intf];

	switch (gmacdev->Speed) {
	case SPEED1000:
		synopGMACClearBits((u32 *)gmacdev->MacBase,GmacConfig,GmacMiiGmii);
		break;
	case SPEED100:
		synopGMACSetBits((u32 *)gmacdev->MacBase,GmacConfig,GmacMiiGmii);
		synopGMACSetBits((u32 *)gmacdev->MacBase,GmacConfig,GmacFESpeed100);
		break;
	case SPEED10:
		synopGMACSetBits((u32 *)gmacdev->MacBase,GmacConfig,GmacMiiGmii);
		synopGMACClearBits((u32 *)gmacdev->MacBase,GmacConfig,GmacFESpeed100);
	default:
		break;
	}

}

/**
 * Function used when the interface is opened for use.
 * We register synopGMAC_linux_open function to linux open(). Basically this
 * function prepares the the device for operation . This function is called whenever ifconfig (in Linux)
 * activates the device (for example "ifconfig eth0 up"). This function registers
 * system resources needed
 * 	- Attaches device to device specific structure
 * 	- Programs the MDC clock for PHY configuration
 * 	- Check and initialize the PHY interface
 *	- ISR registration
 * 	- Setup and initialize Tx and Rx descriptors
 *	- Initialize MAC and DMA
 *	- Allocate Memory for RX descriptors (The should be DMAable)
 * 	- Initialize one second timer to detect cable plug/unplug
 *	- Configure and Enable Interrupts
 *	- Enable Tx and Rx
 *	- start the Linux network queue interface
 * @param[in] pointer to net_device structure.
 * \return Returns 0 on success and error status upon failure.
 * \callgraph
 */

s32 synopGMAC_open(int intf)
{
    //s32 status = 0;
    s32 retval = 0;
    s32 i;
    //s32 reserve_len=2;
    u32 dma_addr;
    struct sk_buff *skb;
    synopGMACdevice * gmacdev = &GMACdev[intf];

    /*Attach the device to MAC struct This will configure all the required base addresses
      such as Mac base, configuration base, phy base address(out of 32 possible phys )*/
    if(intf == 0)
    	synopGMAC_attach(gmacdev, GMAC0MappedAddr + MACBASE, GMAC0MappedAddr + DMABASE, DEFAULT_PHY_BASE);
    else
    	synopGMAC_attach(gmacdev, GMAC1MappedAddr + MACBASE, GMAC1MappedAddr + DMABASE, DEFAULT_PHY_BASE);

    synopGMAC_reset(gmacdev); // Reset to make RGMII/RMII setting take affect --ya
    gmacdev->Intf = intf;
    /*Lets read the version of ip in to device structure*/
    synopGMAC_read_version(gmacdev);

    /*Check for Phy initialization*/
    synopGMAC_set_mdc_clk_div(gmacdev,GmiiCsrClk5);
    //synopGMAC_set_mdc_clk_div(gmacdev, (GmiiCsrClk2|BIT5));
    gmacdev->ClockDivMdc = synopGMAC_get_mdc_clk_div(gmacdev);
    //status = synopGMAC_check_phy_init(gmacdev);
    synopGMAC_check_phy_init(gmacdev);

    /*Set up the tx and rx descriptor queue/ring*/

    synopGMAC_setup_tx_desc_queue(gmacdev,TRANSMIT_DESC_SIZE, RINGMODE);
    synopGMAC_init_tx_desc_base(gmacdev);	//Program the transmit descriptor base address in to DmaTxBase addr

    synopGMAC_setup_rx_desc_queue(gmacdev,RECEIVE_DESC_SIZE, RINGMODE);
    synopGMAC_init_rx_desc_base(gmacdev);	//Program the transmit descriptor base address in to DmaTxBase addr


    synopGMAC_dma_bus_mode_init(gmacdev, DmaBurstLength32 | DmaDescriptorSkip0/*DmaDescriptorSkip2*/ | DmaDescriptor8Words ); //pbl32 incr with rxthreshold 128 and Desc is 8 Words

    synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward |DmaTxSecondFrame|DmaRxThreshCtrl128);

    /*Initialize the mac interface*/

    synopGMAC_mac_init(gmacdev);
    synopGMAC_promisc_enable(gmacdev);

    synopGMAC_pause_control(gmacdev); // This enables the pause control in Full duplex mode of operation

#if 0
    /*IPC Checksum offloading is enabled for this driver. Should only be used if Full Ip checksumm offload engine is configured in the hardware*/
    synopGMAC_enable_rx_chksum_offload(gmacdev);  	//Enable the offload engine in the receive path
    synopGMAC_rx_tcpip_chksum_drop_enable(gmacdev); // This is default configuration, DMA drops the packets if error in encapsulated ethernet payload
#endif

    for(i = 0; i < RECEIVE_DESC_SIZE; i ++) {
        skb = &rx_buf[intf][i];
        synopGMAC_set_rx_qptr(gmacdev, (u32)((u64)(skb->data) & 0xFFFFFFFF), sizeof(skb->data), (u32)((u64)skb & 0xFFFFFFFF));
    }

    synopGMAC_clear_interrupt(gmacdev);

    synopGMAC_enable_interrupt(gmacdev,DmaIntEnable);
    synopGMAC_enable_dma_rx(gmacdev);
    synopGMAC_enable_dma_tx(gmacdev);


    synopGMAC_set_mac_address(intf, intf == 0 ? mac_addr0 : mac_addr1);


    return 0;

}

/**
 * Function used when the interface is closed.
 *
 * This function is registered to linux stop() function. This function is
 * called whenever ifconfig (in Linux) closes the device (for example "ifconfig eth0 down").
 * This releases all the system resources allocated during open call.
 * system resources int needs
 * 	- Disable the device interrupts
 * 	- Stop the receiver and get back all the rx descriptors from the DMA
 * 	- Stop the transmitter and get back all the tx descriptors from the DMA
 * 	- Stop the Linux network queue interface
 *	- Free the irq (ISR registered is removed from the kernel)
 * 	- Release the TX and RX descripor memory
 *	- De-initialize one second timer rgistered for cable plug/unplug tracking
 * @param[in] pointer to net_device structure.
 * \return Returns 0 on success and error status upon failure.
 * \callgraph
 */

s32 synopGMAC_close(int intf)
{

    synopGMACdevice * gmacdev = &GMACdev[intf];;

	//TR0("%s\n",__FUNCTION__);

	/*Disable all the interrupts*/
	synopGMAC_disable_interrupt_all(gmacdev);


	TR("the synopGMAC interrupt has been disabled\n");

	/*Disable the reception*/
	synopGMAC_disable_dma_rx(gmacdev);
    synopGMAC_take_desc_ownership_rx(gmacdev);
	TR("the synopGMAC Reception has been disabled\n");

	/*Disable the transmission*/
	synopGMAC_disable_dma_tx(gmacdev);
    synopGMAC_take_desc_ownership_tx(gmacdev);
    
	TR("the synopGMAC interrupt handler has been removed\n");

	/*Free the Rx Descriptor contents*/
	TR("Now calling synopGMAC_giveup_rx_desc_queue \n");
	synopGMAC_giveup_rx_desc_queue(gmacdev, RINGMODE);

	TR("Now calling synopGMAC_giveup_tx_desc_queue \n");
	synopGMAC_giveup_tx_desc_queue(gmacdev, RINGMODE);

	return 0;

}


/**
 * Function to transmit a given packet on the wire.
 * Whenever Linux Kernel has a packet ready to be transmitted, this function is called.
 * The function prepares a packet and prepares the descriptor and
 * enables/resumes the transmission.
 * @param[in] pointer to sk_buff structure.
 * @param[in] pointer to net_device structure.
 * \return Returns 0 on success and Error code on failure.
 * \note structure sk_buff is used to hold packet in Linux networking stacks.
 */
s32 synopGMAC_xmit_frames(struct sk_buff *skb, int intf, u32 offload_needed, u32 ts)
{
    s32 status = 0;

    u32 dma_addr = (u32)((u64)skb->data & 0xFFFFFFFF);

    synopGMACdevice * gmacdev;


    gmacdev = &GMACdev[intf];

    /*Now we have skb ready and OS invoked this function. Lets make our DMA know about this*/
    status = synopGMAC_set_tx_qptr(gmacdev, dma_addr, skb->len, (u32)((u64)skb & 0xFFFFFFFF), offload_needed, ts);
    if(status < 0) {
        TR0("%s No More Free Tx Descriptors\n",__FUNCTION__);
        return -1;
    }

    /*Now force the DMA to start transmission*/
    synopGMAC_resume_dma_tx(gmacdev);
    

    return 0;
}


/**
 * Function to set ethernet address of the NIC.
 * @param[in] pointer to net_device structure.
 * @param[in] pointer to an address structure.
 * \return Returns 0 on success Errorcode on failure.
 */
s32 synopGMAC_set_mac_address(int intf,  u8* macaddr)
{


    synopGMACdevice * gmacdev = NULL;

    gmacdev = &GMACdev[intf];


    synopGMAC_set_mac_addr(gmacdev, GmacAddr0High, GmacAddr0Low, macaddr);

    return 0;
}

// mode 0: 1000Mbps, 1: 100Mbps, 2: 10Mbps

void synopGMAC_set_mode(int intf, int mode)
{

    synopGMACdevice *gmacdev = &GMACdev[intf];

    // Must stop Tx/Rx before change speed/mode
    synopGMAC_tx_disable(gmacdev);
    synopGMAC_rx_disable(gmacdev);
	switch (mode) {
	case 0:
		synopGMACClearBits((u32 *)gmacdev->MacBase,GmacConfig,GmacMiiGmii);
		gmacdev->Speed = SPEED1000;
		break;
	case 1:
		synopGMACSetBits((u32 *)gmacdev->MacBase,GmacConfig,GmacMiiGmii);
		synopGMACSetBits((u32 *)gmacdev->MacBase,GmacConfig,GmacFESpeed100);
		gmacdev->Speed = SPEED100;
		break;
	case 2:
		synopGMACSetBits((u32 *)gmacdev->MacBase,GmacConfig,GmacMiiGmii);
		synopGMACClearBits((u32 *)gmacdev->MacBase,GmacConfig,GmacFESpeed100);
		gmacdev->Speed = SPEED10;
	default:
		break;
	}
    synopGMAC_tx_enable(gmacdev);
    synopGMAC_rx_enable(gmacdev);
}

