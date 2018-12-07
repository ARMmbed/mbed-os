## Porting guide

There are two main steps to enable the Mbed BLE Cordio port:

1. Configure your target to include Cordio BLE port and Cordio libraries during the build process.

2. Implement the `CordioHCIDriver` class targeting the Bluetooth controller.

### Configure the target

1. Define all Mbed OS targets in the `targets/target.json` file:

1. Add BLE support to the target:

  * Add the string `BLE` to the target's list of `features`. This adds the BLE API sources to the list of sources compiled for the target:

```
    "TARGET_NAME": {
        "features": ["target features ...", "BLE"]
    }
```

### Include Cordio BLE implementation

Compile the BLE Cordio port sources:

* Add the string `CORDIO` to the `extra_labels` property of the JSON file:

```
    "TARGET_NAME": {
        "extra_labels": ["target extra labels ...", "CORDIO"],
        "features": ["target features ...", "BLE"]
    }
```

### Implement CordioHCIDriver:

Include an HCI driver for the BLE module used by the target, and a factory function that creates the BLE instance you use.

#### Create source folder

1. Navigate to the folder of the BLE API that hosts the target port `features/FEATURE_BLE/targets`.

1. Create a folder containing the port code to isolate it from other code.
  
1. Begin this folder's name with `TARGET_` and the rest of the name in capital letters.

#### Create the HCI driver

The HCI driver is split into two entities. One handles HCI communication with the Bluetooth module. The other handles the initialization, reset sequence, and memory dedicated to the Bluetooth controller.

More information about the architecture can be found in [HCI abstraction architecture](HCIAbstraction.md).

#### HCITransport

<span class="notes">**Note:** If the Bluetooth controller uses an H4 communication interface and the host exposes serial flow control in Mbed, you can skip this step. Use the class `ble::vendor::cordio::H4TransportDriver` as the transport driver.</span>

The transport driver shall inherit publicly from the base class `CordioHCITransportDriver`. 

To code a basic transport driver:

```
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

##### Functions

* **Initialization/termination**: The functions `initialize` and `terminate` are responsible for initializing and terminating the transport driver. It is not necessary to initialize the transport in the constructor.

* **Sending data**: The function `write` sends data as input to the Bluetooth controller and returns the number of bytes in the `data` buffer sent. Depending on the type of transport you implement, you may need to send the packet `type` to the controller before the packet data.

* **Receiving data**: Inject HCI data from the Bluetooth controller to the system by calling `on_data_received`. This is a static function provided by the base class.

**Example:**

```
void on_data_received(uint8_t* data_received, uint16_t length_of_data_received);
```

#### HCIDriver

The HCI Driver shall inherit publicly from the base class `CordioHCIDriver`.

The driver template is:

```
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

The functions `do_initialize` and `do_terminate` handle initialization and termination processes. These functions manage the state of the Bluetooth controller.

<span class="notes">**Note:** It is unnecessary to initialize or terminate the HCI transport in these functions, because that is handled by the base class. The HCI transport is initialized right before the call to `do_initialize` and is terminated right after the call to `do_terminate`.</span>

##### Memory pool

Porters must override the `get_buffer_pool_description` function, which should return a buffer pool for the Cordio stack to use.

The function `get_default_buffer_pool_description` in the base class returns a buffer of 2250 bytes divided into different memory pools, and most implementations can use it:

| Chunk size (bytes) | Number of chunks |
|--------------------|------------------|
| 16                 | 16               |
| 32                 | 16               |
| 64                 | 8                |
| 128                | 4                |
| 272                | 1                |

**Example:**
```
buf_pool_desc_t CordioHCIDriver::get_buffer_pool_description()  {
    return get_default_buffer_pool_description();
}
```

If the memory the base class provides doesn't match what the Bluetooth controller driver requires, a custom pool can be returned.

**Example:**

```
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

Three functions handle the reset sequence process:

* `start_reset_sequence`: This function starts the process. It sends an HCI reset command to the Bluetooth controller. You can override this function *if* the Bluetooth controller requires more than just sending the standard reset command.

* `handle_reset_sequence`: Entry point to the state machine handling the reset process. Every time an HCI packet is received during the reset sequence, this function is called with the HCI packet received. Its purpose is to prepare the Bluetooth controller and set the parameters the stack needs to operate properly. You can override this function if necessary.

* `signal_reset_sequence_done`: Once the reset sequence is completed, you can call this function. You cannot override it.

###### Controller parameters to set

This step tells the controller which events are relevant to the stack.

You should set the following parameters in the controller (if supported):

* **Event mask:** Call `HciSetEventMaskCmd((uint8_t *) hciEventMask)`.

* **LE event mask:** Call `HciLeSetEventMaskCmd((uint8_t *) hciLeEventMask)`.

* **2nd page of events mask:** Call `HciSetEventMaskPage2Cmd((uint8_t *) hciEventMaskPage2)`.

###### Stack runtime parameters

At runtime, you can get stack parameters from the controller:

* **Bluetooth address:** Query this with `HciReadBdAddrCmd`. Copy the response into `hciCoreCb.bdAddr` with `BdaCpy`.

* **Buffer size of the controller:** Query this with `HciLeReadBufSizeCmd`.
  * The return parameter `HC_ACL_Data_Packet_Length` is copied to `hciCoreCb.bufSize`. Copy the response parameter `HC_Synchronous_Data_Packet_Length` into `hciCoreCb.numBufs`. `hciCoreCb.numBufs` initializes the value of `hciCoreCb.availBufs`.

* **Supported state:** Query this with `HciLeReadSupStatesCmd`, and copy the response into `hciCoreCb.leStates`.

* **Whitelist size:** Query this with `HciLeReadWhiteListSizeCmd`, and copy the response into `hciCoreCb.whiteListSize`.

* **LE features supported:** Query this with `HciLeReadLocalSupFeatCmd`, and copy the response into `hciCoreCb.leSupFeat`.

* **Resolving list size:** Query this with `hciCoreReadResolvingListSize`, and copy the response into `hciCoreCb.resListSize`.

* **Max data length:** Query this with `hciCoreReadMaxDataLen`, and pass the response parameters `supportedMaxTxOctets` and `supportedMaxTxTime` to the function `HciLeWriteDefDataLen`.

**Example:**

```
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
            // Set the event mask to control which events are generated by the controller for the host.
            HciSetEventMaskCmd((uint8_t *) hciEventMask);
            break;

        case HCI_OPCODE_SET_EVENT_MASK:
            // Set the event mask to control which LE events are generated by the controller for the host.
            HciLeSetEventMaskCmd((uint8_t *) hciLeEventMask);
            break;

        case HCI_OPCODE_LE_SET_EVENT_MASK:
            // Set the event mask to control which events are generated by the controller for the host (2nd page of flags).
            HciSetEventMaskPage2Cmd((uint8_t *) hciEventMaskPage2);
            break;

        case HCI_OPCODE_SET_EVENT_MASK_PAGE2:
            // Ask the Bluetooth address of the controller.
            HciReadBdAddrCmd();
            break;

        case HCI_OPCODE_READ_BD_ADDR:
            // Store the Bluetooth address in the stack runtime parameter.
            BdaCpy(hciCoreCb.bdAddr, pMsg);

            // Read the size of the buffer of the controller.
            HciLeReadBufSizeCmd();
            break;

        case HCI_OPCODE_LE_READ_BUF_SIZE:
            // Store the buffer parameters in the stack runtime parameters.
            BSTREAM_TO_UINT16(hciCoreCb.bufSize, pMsg);
            BSTREAM_TO_UINT8(hciCoreCb.numBufs, pMsg);

            /* initialize ACL buffer accounting */
            hciCoreCb.availBufs = hciCoreCb.numBufs;

            // Read the states and state combinations supported by the link layer of the controller.
            HciLeReadSupStatesCmd();
            break;

        case HCI_OPCODE_LE_READ_SUP_STATES:
            // Store supported state and combination in the runtime parameters of the stack.
            memcpy(hciCoreCb.leStates, pMsg, HCI_LE_STATES_LEN);

            // Read the total of whitelist entries that can be stored in the controller.
            HciLeReadWhiteListSizeCmd();
            break;

        case HCI_OPCODE_LE_READ_WHITE_LIST_SIZE:
            // Store the number of whitelist entries in the stack runtime parameters.
            BSTREAM_TO_UINT8(hciCoreCb.whiteListSize, pMsg);

            // Read the LE features supported by the controller.
            HciLeReadLocalSupFeatCmd();
            break;

        case HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT:
            // Store the set of LE features supported by the controller.
            BSTREAM_TO_UINT16(hciCoreCb.leSupFeat, pMsg);

            // Read the total number of address translation entries which can be stored in the controller resolving list.
            hciCoreReadResolvingListSize();
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

The HCI driver is injected to the `CordioBLE` class at manufacture.

Given that the `CordioBLE` class doesn't know which class constructs the driver nor how to construct it, the port provides a function returning a reference to the HCI driver.

This function is in the global namespace, and its signature is:

```
ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver();
```

**Example:**

```
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

### Tests

We bundle Greentea tests with the Cordio port of the BLE API, so the transport driver and validation of Cordio stack initialization both work. You can run these tests with the following command:

```
mbed test -t <toolchain> -m <target> -n mbed-os-features-feature_ble-targets-target_cordio-tests-cordio_hci-driver,mbed-os-features-feature_ble-targets-target_cordio-tests-cordio_hci-transport
```

* `mbed-os-features-feature_ble-targets-target_cordio-tests-cordio_hci-transport`: Ensures that the transport is able to send an HCI reset command and receive the corresponding HCI status event.
* `mbed-os-features-feature_ble-targets-target_cordio-tests-cordio_hci-driver`: Runs the whole initialization process, then ensures the HCI driver has properly initialized the Cordio stack.


### Tools

You can use the application [mbed-os-cordio-hci-passthrough](https://github.com/ARMmbed/mbed-os-cordio-hci-passthrough) to proxify a Bluetooth controller connected to an Mbed board.

The host sent bytes over the board serial, which the `HCITransport Driver` forwards. Bytes sent by the controller go back to the host through the board serial.

You can use this application to validate the transport driver and debug the initialization process.
