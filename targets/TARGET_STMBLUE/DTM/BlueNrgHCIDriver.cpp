/**
  ******************************************************************************
  * @file    DTM_cmd_db.c
  * @author  Antonio O.
  * @date    21 Dec. 2018
  ******************************************************************************
  */
#include <mbed.h>
#include "CordioHCIDriver.h"
#include "CordioHCITransportDriver.h"
#include "hci_api.h"
#include "hci_cmd.h"
#include "hci_core.h"
#include "dm_api.h"
#include "bstream.h"
//
#include "DTM_boot.h"
#include "DTM_cmd_db.h"
#include "osal.h"
#include "bluenrg1_api.h"
#include "bluenrg1_stack.h"
#include "hci_defs.h"

//STACKTICK_CODE is used for emulating BTLE_StackTick on an HCI vendor specific command.
//This value is not used and can be assigned to this functionality. The wrapper for BTLE_StackTick
//is providen in DTM_cmd_db.h
#define STACKTICK_CODE 0xFCFF
#define TICK_MS 500

extern "C" void rcv_callback(uint8_t *data, uint16_t len){
	ble::vendor::cordio::CordioHCITransportDriver::on_data_received(data, len);
}

#define HCI_RESET_RAND_CNT        4
#define LL_WITHOUT_HOST_OFFSET 0x2C

namespace ble {
namespace vendor {
namespace bluenrg {

/**
 * BlueNRG HCI driver implementation.
 * @see cordio::CordioHCIDriver
 */

class HCIDriver : public cordio::CordioHCIDriver
{
public:
    /**
     * Construction of the BlueNRG HCIDriver.
     * @param transport: Transport of the HCI commands.
     */
    HCIDriver(cordio::CordioHCITransportDriver& transport_driver) :
    	cordio::CordioHCIDriver(transport_driver) { }

    virtual ~HCIDriver() {};
    /**
     * @see CordioHCIDriver::do_initialize
     */
    virtual void do_initialize() {

    }

    /**
     * @see CordioHCIDriver::start_reset_sequence
     */
    virtual void start_reset_sequence() {
        /* send an HCI Reset command to start the sequence */
        HciResetCmd();
    }

    /**
     * @see CordioHCIDriver::do_terminate
     */
    virtual void do_terminate() {

    }

    /**
     * @see CordioHCIDriver::handle_reset_sequence
     */

    virtual void handle_reset_sequence(uint8_t *pMsg) {
        // only accept command complete event:
        if (*pMsg != HCI_CMD_CMPL_EVT) {
            return;
        }

        uint8_t Value_LL = 0x01;
        uint16_t       opcode;
//        static uint8_t randCnt;

        /* parse parameters */
        pMsg += HCI_EVT_HDR_LEN;
        pMsg++;                   /* skip num packets */
        BSTREAM_TO_UINT16(opcode, pMsg); // copy opcode
        pMsg++;                   /* skip status */

        /* decode opcode */
        switch (opcode)
        {
            case HCI_OPCODE_RESET:
                /* initialize rand command count */
//                randCnt = 0;

                //manually initialization of random address (because there is no GAP init)
                uint8_t Random_Address[8];  //8 bytes allocated because of ST random generation
                hci_le_rand(Random_Address);
                Random_Address[5] = Random_Address[5] | 0xC0;  //bitwise or for 2 MSB (required by core 5.0 for Random Static Address)

            	//Set the controller in Link Layer Only mode
            	aci_hal_write_config_data(LL_WITHOUT_HOST_OFFSET, 1, &Value_LL);
            	hci_le_set_random_address(Random_Address);

            	//DO NOT SET ANY EVENT MASK, BY DEFAULT ALL EVENTS ENABLED!!

            	// Ask the Bluetooth address of the controller.
            	hci_read_bd_addr(hciCoreCb.bdAddr);

            	// Read the size of the buffer of the controller and store the buffer parameters in the stack Cordio runtime parameters.
            	hci_le_read_buffer_size(&hciCoreCb.bufSize, &hciCoreCb.numBufs);  ///////rivedere
            	/* initialize ACL buffer accounting */
            	hciCoreCb.availBufs = hciCoreCb.numBufs;

            	// Read the states and state combinations supported by the link layer of the controller and
            	// store supported state and combination in the runtime parameters of the stack Cordio.
            	hci_le_read_supported_states(hciCoreCb.leStates);

            	// Read the total of whitelist entries that can be stored in the controller and
            	// store the number of whitelist entries in the stack Cordio runtime parameters.
            	hci_le_read_white_list_size(&hciCoreCb.whiteListSize);

            	// Read the LE features supported by the controller and store the set of
            	// LE features supported by the controller in the Cordio Stack runtime parameters.
            	uint8_t features[2];
            	hci_le_read_local_supported_features(features);   //it is a 64 bit number, but only 16 MSB are significant (others are reserved for future use)
            	hciCoreCb.leSupFeat = features[1] << 8 | features[0];

            	hciCoreReadResolvingListSize();  //reset sequence could terminate here depending on controller configuration

                break;

            case HCI_OPCODE_LE_READ_RES_LIST_SIZE:
                // Store the number of address translation entries in the stack runtime parameter.
                BSTREAM_TO_UINT8(hciCoreCb.resListSize, pMsg);

                // Read the Controller’s maximum supported payload octets and packet duration times for transmission and reception.
                hciCoreReadMaxDataLen();
                break;

            case HCI_OPCODE_LE_READ_MAX_DATA_LEN:
                {
                    // Store payload definition in the runtime stack parameters.
                    uint16_t maxTxOctets;
                    uint16_t maxTxTime;

                    BSTREAM_TO_UINT16(maxTxOctets, pMsg);
                    BSTREAM_TO_UINT16(maxTxTime, pMsg);

                    /* use Controller's maximum supported payload octets and packet duration times
                    * for transmission as Host's suggested values for maximum transmission number
                    * of payload octets and maximum packet transmission time for new connections.
                    */
                    HciLeWriteDefDataLen(maxTxOctets, maxTxTime);
                }
                break;

            case HCI_OPCODE_LE_WRITE_DEF_DATA_LEN:
                if (hciCoreCb.extResetSeq)
                {
                    /* send first extended command */
                    (*hciCoreCb.extResetSeq)(pMsg, opcode);
                }
                else
                {
                    /* initialize extended parameters */
                    hciCoreCb.maxAdvDataLen = 0;
                    hciCoreCb.numSupAdvSets = 0;
                    hciCoreCb.perAdvListSize = 0;

                    /* send next command in sequence */
                    HciLeRandCmd();
                }
                break;

            case HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN:
            case HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS:
            case HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE:
                // handle extended command
                if (hciCoreCb.extResetSeq)
                {
                    /* send next extended command in sequence */
                    (*hciCoreCb.extResetSeq)(pMsg, opcode);
                }
                break;

            case HCI_OPCODE_LE_RAND:
                    /* last command in sequence; set resetting state and call callback */
                    signal_reset_sequence_done();
                break;

            default:
                break;
        }








    }

    virtual ble::vendor::cordio::buf_pool_desc_t get_buffer_pool_description() {
    	//return ble::vendor::cordio::CordioHCIDriver::get_default_buffer_pool_description();

    	//minimal configuration for HRM example
        uint8_t buffer[1430];
        static const wsfBufPoolDesc_t pool_desc[] = {
            {  16, 14 },
            {  32, 10 },
            {  64, 4 },
            { 128, 2 },
            { 272, 1 }
        };

        return ble::vendor::cordio::buf_pool_desc_t(buffer, pool_desc);
    }

private:

    void hciCoreReadMaxDataLen(void)
    {
        /* if LE Data Packet Length Extensions is supported by Controller and included */
        if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_DATA_LEN_EXT) &&
            (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_DATA_LEN_EXT))
        {
            /* send next command in sequence */
            HciLeReadMaxDataLen();
        }
        else
        {
            /* send next command in sequence */
            HciLeRandCmd();
        }
    }

    void hciCoreReadResolvingListSize(void)
    {
        /* if LL Privacy is supported by Controller and included */
        if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_PRIVACY) &&
            (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_PRIVACY))
        {
            /* send next command in sequence */
            HciLeReadResolvingListSize();
        }
        else
        {
            hciCoreCb.resListSize = 0;

            /* send next command in sequence */
            hciCoreReadMaxDataLen();
        }
    }
};

/**
 *  Virtual Transport driver, used to exchange packet between host and controller.
 */
class TransportDriver : public cordio::CordioHCITransportDriver {
public:

    TransportDriver() {}

    virtual ~TransportDriver(){ }

    /**
     * @see CordioHCITransportDriver::initialize
     */
    virtual void initialize() {
    	/* Stack Initialization */
    	//DTM_StackInit();
    	/* Periodic signal for BTLE_StackTick initialization */
    	tick.attach_us(&TransportDriver::StackTick, TICK_MS*1000);
    }

    /**
     * @see CordioHCITransportDriver::terminate
     */
    virtual void terminate() { }

    /**
     * @see CordioHCITransportDriver::write
     */
    virtual uint16_t write(uint8_t type, uint16_t len, uint8_t *pData) {
    	if(type== HCI_CMD_TYPE){
        	uint8_t resp_len;
        	resp_len = process_command(pData, len, buffer_out, 255);
        	rcv_callback(buffer_out, resp_len);
    	}
    	else if(type==HCI_ACL_TYPE){
    	      uint16_t connHandle;
    	      uint16_t dataLen;
    	      uint8_t* pduData;
    	      uint8_t  pb_flag;
    	      uint8_t  bc_flag;

    	      connHandle = ((pData[1] & 0x0F) << 8) + pData[0];
    	      dataLen = (pData[3] << 8) + pData[2];
    	      pduData = pData+4;
    	      pb_flag = (pData[1] >> 4) & 0x3;
    	      bc_flag = (pData[1] >> 6) & 0x3;
    	      hci_tx_acl_data(connHandle, pb_flag, bc_flag, dataLen, pduData);
    	}

    	return len;
    }

private:
    uint16_t process_command(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length){
    	  uint16_t ret_val, opCode;

    	  Osal_MemCpy(&opCode, buffer_in, 2);
    	  for (uint i = 0; i < (sizeof(hci_command_table)/sizeof(hci_command_table_type)); i++) {
    	    if (opCode == hci_command_table[i].opcode) {
    	      ret_val = hci_command_table[i].execute(buffer_in+2, buffer_in_length-2, buffer_out, buffer_out_max_length);
    	      return ret_val;
    	    }
    	  }
    	  // Unknown command length
    	  buffer_out[0] = 0x04;
    	  buffer_out[1] = 0x0F;
    	  buffer_out[2] = 0x04;
    	  buffer_out[3] = 0x01;      ///01 unknown command <-
    	  buffer_out[4] = 0x01;
    	  Osal_MemCpy(&buffer_out[5], &opCode, 2);
    	  return 7;
    }

    static void StackTick(){
    	uint8_t *pBuf = hciCmdAlloc(STACKTICK_CODE, 0);  //vendor specific command
    	hciCmdSend(pBuf);
    }

    uint8_t buffer_out[258]; //buffer to store hci event packets generated after an hci command
    Ticker tick;
};

} // namespace bluenrg
} // namespace vendor
} // namespace ble

/**
 * Cordio HCI driver factory
 */
ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver() {
    static ble::vendor::bluenrg::TransportDriver transport_driver;
    static ble::vendor::bluenrg::HCIDriver hci_driver(transport_driver);
    return hci_driver;
}
