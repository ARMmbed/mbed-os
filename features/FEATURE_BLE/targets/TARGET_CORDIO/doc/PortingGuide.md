# Porting guide 

Enabling the mbed BLE Cordio port for a given target is a two step process: 
* The target shall be configured to include Cordio BLE port and Cordio libraries
during the build process. 
* An implementation of the `CordioHCIDriver` class targeting the bluetooth 
controller used shall be provided.

## Target configuration

This step happens in the file defining all mbed os targets. The file is located 
in `targets/target.json`. 

### Add BLE feature 

First of all support for BLE has to be added to the target. This is achieved by 
adding the string `BLE` in the list of `features` of the target. Once this 
value has been added the sources of BLE api are added to the list of sources 
compiled for the target.

```json
    "TARGET_NAME": {
        "features": ["target features ...", "BLE"]
    }
```

### Include Cordio BLE implementation 

The target should also compile the sources of the BLE Cordio port. It is 
achieved by adding the string `CORDIO` in the list of the `extra_labels` 
property of the target. 

```json
    "TARGET_NAME": {
        "extra_labels": ["target extra labels ...", "CORDIO"],
        "features": ["target features ...", "BLE"]
    }
```

## CordioHCIDriver implementation: 

A port shall include an HCI driver for the BLE module used by the target and 
a factory function which create the BLE instance used by the user of BLE API. 

### Create source folder 

The port shall live in the folder of BLE API which host targets port : 
`features/FEATURE_BLE/targets`. 

To isolate the port code from other code a folder containing the port code has 
to be created. The name of this folder shall start with `TARGET_` and end with 
the target name in capital.

### Create the HCI driver

The HCI driver is split in two entities: one which handle HCI communication with 
the Bluetooth module and the other handling the initialization, reset sequence 
and memory dedicated for the Bluetooth controller.

More information about the architecture can be found in the 
[HCI abstraction architecture](HCIAbstraction.md) document.

#### HCITransport 

> **Note:** If the Bluetooth controller uses an H4 communication interface and
the host exposes serial flow control in mbed then this step can be skipped and
the class `ble::vendor::cordio::H4TransportDriver` can be used as the transport
driver.

An empty transport driver can be coded as: 

```c++
#include "CordioHCITransportDriver.h"

namespace ble { 
namespace vendor {
namespace target_name { 

class TransportDriver : public cordio::CordioHCITransportDriver { 
public: 
    TransportDriver(/* specific constructor arguments*/);

    virtual ~TransportDriver();

    virtual void initialize();

    virtual void terminate(); 
    
    virtual uint16_t write(uint8_t packet_type, uint16_t len, uint8_t *data);

private:
    // private driver declarations
};

} // namespace target_name
} // namespace vendor
} // namespace ble
```

It shall inherits publicly from the base class `CordioHCITransportDriver`. 

* **Initialization/termination:** The functions `initialize` and `terminate` are 
responsible for initializing and terminating the transport driver. It is not 
necessary to initialize the transport in the constructor.

* **Sending data** The function `write` shall sends data in input to the 
Bluetooth controller and return the number of bytes in the `data` buffer sent. 
Depending on the type of transport being implemented, the packet `type` might 
have to be sent to the controller before the packet data.

* **Receiving data**: HCI data from the Bluetooth controller shall be injected 
in the system by invoking the function `on_data_received`. This function is 
a static one and is provided by the base class. Its prototype is: 

```c++
void on_data_received(uint8_t* data_received, uint16_t length_of_data_received);
```

#### HCIDriver 

The skeleton of driver is: 

```c++
#include "CordioHCIDriver.h"

namespace ble { 
namespace vendor {
namespace target_name { 

class HCIDriver : public cordio::CordioHCIDriver { 
public: 
    HCIDriver(
        cordio::CordioHCITransportDriver& transport_driver, 
        /* specific constructor arguments*/
    );

    virtual ~HCIDriver();

    virtual void do_initialize();

    virtual void do_terminate();

    virtual buf_pool_desc_t get_buffer_pool_description();

    virtual void start_reset_sequence();

    virtual void handle_reset_sequence(uint8_t *msg);
private:
    // private driver declarations
};

} // namespace target_name
} // namespace vendor
} // namespace ble
```

##### Initialization process 

The initialization/termination process is achieved by the couple of functions 
`do_initialize` and `do_terminate`. Those function manages the state of the 
bluetooth controller. 

> **Important:** It is unnecessary to initialize or terminate the hci transport 
in those function because it is handled by the base class. The HCI transport is 
initialized right before the call to `do_initialize` and is terminated right 
after the call to `do_terminate`.

##### Memory pool 

The implementation of the function `get_buffer_pool_description` in the base 
class will return a buffer of 1040 bytes divided in different memory pools: 

| Chunk size (bytes) | Number of chunks |
|--------------------|------------------|
| 16                 | 8                |
| 32                 | 4                |
| 64                 | 2                |
| 128                | 2                |
| 272                | 1                |

A port shall override this function if the memory provided by the base class 
doesn't match what is required by the Bluetooth controller driven.

```c++
buf_pool_desc_t CordioHCIDriver::get_buffer_pool_description()  { 
    static uint8_t buffer[/* buffer size */];
    static const wsfBufPoolDesc_t pool_desc[] = {
        { /* chunk size */, /* number of chunks */ },
        { /* chunk size */, /* number of chunks */ },
        { /* chunk size */, /* number of chunks */ },
        { /* chunk size */, /* number of chunks */ },
        { /* chunk size */, /* number of chunks */ },
        { /* chunk size */, /* number of chunks */ },
        { /* chunk size */, /* number of chunks */ },
    };

    return buf_pool_desc_t(buffer, pool_desc);
} 
```

##### Reset sequence 

The reset sequence process is handled by three functions: 

* `start_reset_sequence`: This function start the process. The basic 
implementation sends an HCI reset command to the Bluetooth controller. The 
function shall be overridden in the driver if the bluetooth controller requires
more than just sending the standard reset command. 

* `handle_reset_sequence`: Entry point to the state machine handling the reset
process. Every time an HCI packet is received during the reset sequence, this 
function is called with the HCI packet received. It's purpose is to prepare the 
Bluetooth controller and set the parameters the stack needs to operate properly.
This function can be overridden if necessary.

* `signal_reset_sequence_done`: This function shall be called once the reset 
sequence is achieved. It cannot be overridden.

###### Controller parameters to set 

This instruct the controller which events are relevant for the stack.

The following parameters should be set in the controller (if supported): 
* event mask: The reset sequence should issue the call 
`HciSetEventMaskCmd((uint8_t *) hciEventMask)`
* LE event mask: The call `HciLeSetEventMaskCmd((uint8_t *) hciLeEventMask)` 
should be issued.
* 2nd page of events mask: Can be achieved by invoking 
`HciSetEventMaskPage2Cmd((uint8_t *) hciEventMaskPage2)`.


###### Stack runtime parameters

Some stack parameters shall be acquired at runtime from the controller: 

* Bluetooth address: Can be queried with `HciReadBdAddrCmd`. Response shall be 
copied into `hciCoreCb.bdAddr` with `BdaCpy`.
* Buffer size of the controller: Can be obtained by `HciLeReadBufSizeCmd`. The 
return parameter *HC_ACL_Data_Packet_Length* shall be copied into 
`hciCoreCb.bufSize` and the response parameter 
`HC_Synchronous_Data_Packet_Length`shall be copied into `hciCoreCb.numBufs`. 
The value of `hciCoreCb.availBufs` shall be initialized with `hciCoreCb.numBufs`.
* Supported state: Queried with `HciLeReadSupStatesCmd`, the response shall go 
into `hciCoreCb.leStates`.
* Whitelist size: Queried with `HciLeReadWhiteListSizeCmd` the response shall go
into `hciCoreCb.whiteListSize`.
* LE features supported: Obtained with `HciLeReadLocalSupFeatCmd`. The response 
shall be stored into `hciCoreCb.leSupFeat`.
* Resolving list size: Obtained with `hciCoreReadResolvingListSize`. The response
shall go into `hciCoreCb.resListSize`.
* Max data length: Acquired with `hciCoreReadMaxDataLen`. The response parameter
`supportedMaxTxOctets` and `supportedMaxTxTime` shall be pass to the function 
`HciLeWriteDefDataLen`.


The default implementation is: 

```c++
void HCIDriver::handle_reset_sequence(uint8_t *pMsg) { 
    // only accept command complete event: 
    if (*pMsg != HCI_CMD_CMPL_EVT) { 
        return;
    }

    uint16_t       opcode;
    static uint8_t randCnt;

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
            randCnt = 0;
            // set the event mask to control which events are generated by the 
            // controller for the host
            HciSetEventMaskCmd((uint8_t *) hciEventMask);
            break;

        case HCI_OPCODE_SET_EVENT_MASK:
            // set the event mask to control which LE events are generated by 
            // the controller for the host
            HciLeSetEventMaskCmd((uint8_t *) hciLeEventMask);
            break;

        case HCI_OPCODE_LE_SET_EVENT_MASK:
            // set the event mask to control which events are generated by the 
            // controller for the host (2nd page of flags )
            HciSetEventMaskPage2Cmd((uint8_t *) hciEventMaskPage2);
            break;

        case HCI_OPCODE_SET_EVENT_MASK_PAGE2:
            // Ask the Bluetooth address of the controller 
            HciReadBdAddrCmd();
            break;

        case HCI_OPCODE_READ_BD_ADDR:
            // Store the Bluetooth address in the stack runtime parameter 
            BdaCpy(hciCoreCb.bdAddr, pMsg);

            // Read the size of the buffer of the controller
            HciLeReadBufSizeCmd();
            break;

        case HCI_OPCODE_LE_READ_BUF_SIZE:
            // Store the buffer parameters in the stack runtime parameters
            BSTREAM_TO_UINT16(hciCoreCb.bufSize, pMsg);
            BSTREAM_TO_UINT8(hciCoreCb.numBufs, pMsg);

            /* initialize ACL buffer accounting */
            hciCoreCb.availBufs = hciCoreCb.numBufs;

            // read the states and state combinations supported by the link 
            // layer of the controller
            HciLeReadSupStatesCmd();
            break;

        case HCI_OPCODE_LE_READ_SUP_STATES:
            // store supported state and combination in the runtime parameters 
            // of the stack
            memcpy(hciCoreCb.leStates, pMsg, HCI_LE_STATES_LEN);

            // read the total of whitelist entries that can be stored in the 
            // controller.
            HciLeReadWhiteListSizeCmd();
            break;

        case HCI_OPCODE_LE_READ_WHITE_LIST_SIZE:
            // store the number of whitelist entries in the stack runtime 
            // parameters
            BSTREAM_TO_UINT8(hciCoreCb.whiteListSize, pMsg);

            // Read the LE features supported by the controller
            HciLeReadLocalSupFeatCmd();
            break;

        case HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT:
            // Store the set of LE features supported by the controller
            BSTREAM_TO_UINT16(hciCoreCb.leSupFeat, pMsg);

            // read the total number of address translation entries which can be 
            // stored in the controller resolving list.
            hciCoreReadResolvingListSize();
            break;

        case HCI_OPCODE_LE_READ_RES_LIST_SIZE:
            // store the number of address translation entries in the stack 
            // runtime parameter
            BSTREAM_TO_UINT8(hciCoreCb.resListSize, pMsg);

            // read the Controller’s maximum supported payload octets and packet 
            // duration times for transmission and reception
            hciCoreReadMaxDataLen();
            break;

        case HCI_OPCODE_LE_READ_MAX_DATA_LEN:
            {
                // store payload definition in the runtime stack parameters.
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
            /* check if need to send second rand command */
            if (randCnt < (HCI_RESET_RAND_CNT-1))
            {
                randCnt++;
                HciLeRandCmd();
            }
            else
            {
                /* last command in sequence; set resetting state and call callback */
                signal_reset_sequence_done();
            }
            break;

        default:
            break;
    }
}

static void hciCoreReadMaxDataLen(void)
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

static void hciCoreReadResolvingListSize(void)
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
```



### HCI accessor function

The HCI driver is injected in the `CordioBLE` class at construction site. 
Given that the CordioBLE class doesn't know what class shall be used to 
construct the driver nor it knows how to construct it, the port shall provide a 
function returning a reference to the HCI driver. 

This function lives in the global namespace and its signature is: 

```c++
ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver();
```

A common implementation might be: 

```c++
ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver() { 
    static ble::vendor::target_name::TransportDriver transport_driver(
        /* transport parameters */
    );

    static ble::vendor::target_name::HCIDriver hci_driver(
        transport_driver, /* other hci driver parameters */
    );

    return hci_driver;
}
```
















