# How to add the WICED SDK

This document summarizes how to download and integrate the WICED SDK into Mbed OS.

**NOTE:** This document is intended for when the WICED SDK is extracted from Mbed OS.

There are several files not provided by the WICED SDK. This is a non-exhaustive
list of files that are currently provided by Mbed OS:
- w_generated_mac_address.txt
- w_generated_security_dct.h
- w_resources.h
- mbed_lib.json

## Download the WICED SDK

You can find the full instructions to download the WICED SDK with Advantech's patches
on Advantech's Wiki:
http://ess-wiki.advantech.com.tw/view/WISE-1530_SDK

To get the WICED SDK, download WICED-Studio-5.2.0 for your platform:
https://community.cypress.com/community/wiced-wifi/wiced-wifi-documentation

Once downloaded, move the WICED SDK folder into `targets/TARGET_WICED`.

For the WISE1530, download the patch from Advantech:
`WM-BN-BM-22_SDK_5.1.x_platform_patch.zip`

Once downloaded, copy the contents into `targets/TARGET_WICED/WICED`.

## Modify the WICED SDK to support Mbed OS's build tools

1. Remove these files because they are unnecessary and large:
   - `targets/TARGET_WICED/WICED/tools`
   - `targets/TARGET_WICED/WICED/build`
   - `targets/TARGET_WICED/WICED/apps`
   - `targets/TARGET_WICED/WICED/factory`
   - `targets/TARGET_WICED/WICED/doc`
   - `targets/TARGET_WICED/WICED/RTOS/NoOS`
   - `targets/TARGET_WICED/WICED/RTOS/NuttX`
   - `targets/TARGET_WICED/WICED/RTOS/ThreadX`
   - `targets/TARGET_WICED/WICED/network/NetX`
   - `targets/TARGET_WICED/WICED/network/NetX_Duo`
   - `targets/TARGET_WICED/WICED/network/NoNS`
   - `targets/TARGET_WICED/WICED/network/NuttX_NS`

1. Add the `w_` prefix to all c/h files in WICED.

   Edit all c/h files and change the relevant `#include` statments to use the
   updated file names.

   This is required to avoid naming conflicts between Mbed OS and the WICED SDK.

1. Prevent Mbed OS from compiling the include-only c files. These are
   any include statments that refer to a file ending in `.c`.

   For example:
   
   ``` diff
   - libraries/crypto/micro-ecc/w_asm_arm.c
   + libraries/crypto/micro-ecc/w_asm_arm.inc
   ```

   Edit all c/h files, and change the relevant `#include` statements to use the
   updated file names.

1. Remove any references to the WICED file system.

   For example:
   
   ``` diff
   - #include "wicedfs.h"
   ```

1. Create a C wrapper for the firmware image.

   In `targets/TARGET_WICED/WICED/resources/firmware/4343W`:
   
   ``` bash
   xxd -i 4343WA1.bin > 4343WA1.c
   sed -i 's/__4343WA1_bin/wifi_firmware_image_data/' 4343WA1.c
   sed -i 's/unsigned/const unsigned/' 4343WA1.c
   ```

   Then, modify the firmware image in the C wrapper to match WICED's link-time
   resource API.

   In `targets/TARGET_WICED/WICED/resources/firmware/4343W/4343WA1.c`:
   
   ``` c
   #include "w_resources.h"

   extern const resource_hnd_t wifi_firmware_image;
   extern const char wifi_firmware_image_data[383156];

   const resource_hnd_t wifi_firmware_image = {
       .location = RESOURCE_IN_MEMORY,
       .size = sizeof(wifi_firmware_image_data),
       .val.mem.data = wifi_firmware_image_data,
   };

   const char wifi_firmware_image_data[383156] = {
       ...
   ```

   Then, create a `w_resources.h` file to reference the above firmware image.

   ``` c
   #ifndef INCLUDED_RESOURCES_H_
   #define INCLUDED_RESOURCES_H_
   #include "wiced_resource.h"
   extern const resource_hnd_t wifi_firmware_image;
   extern const char wifi_firmware_image_data[383156];
   #endif
   ```

1. Create the `w_generated_security_dct.h` file that
   the WICED SDK tools would normally generate.

   ``` c
   #define CERTIFICATE_STRING "\0"
   #define PRIVATE_KEY_STRING "\0"
   ```

1. Create the `w_generated_mac_address.txt` file that
   the WICED SDK tools would normally generate.

   ``` c
   #define NVRAM_GENERATED_MAC_ADDRESS        "macaddr=02:0A:F7:11:b6:19"
   #define DCT_GENERATED_MAC_ADDRESS          "\x02\x0A\xF7\x19\x11\xb6"
   #define DCT_GENERATED_ETHERNET_MAC_ADDRESS "\x02\x0A\xF7\x19\x11\xb7"
   ```

1. Change any instances of `_irq` functions to match Mbed OS's linker `_IRQHandler`

   For example:
   
   ``` diff
   - WWD_RTOS_DEFINE_ISR( NoOS_systick_irq )
   + WWD_RTOS_DEFINE_ISR( NoOS_systick_IRQHandler )
   ```

1. Comment out the bad `wwd_bus_packet_available_to_read` call while polling for
   bus packet status.

   In `targets/TARGET_WICED/WICED/WWD/internal/wwd_thread.c` line 336:
   
   ``` diff
     wwd_bus_interrupt = WICED_FALSE;

     /* Check if the interrupt indicated there is a packet to read */
   - if ( wwd_bus_packet_available_to_read( ) != 0)
   + // TODO: packet check causes device to lock up^M
   + if ( /*wwd_bus_packet_available_to_read( )*/ 1 != 0)^M
     {
         /* Receive all available packets */
         do
   ```

   Note this is required for the WISE1530 but may not be needed for other
   platforms.

1. Remove the includes of `w_core_cm4.h` and `w_core_cmFunc.h`, and replace the
   includes with references to Mbed OS's `core_cm4.h`.

   For example:
   
   ``` diff
   - #include "w_core_cmFunc.h"
   + #include "core_cm4.h"
   ```

1. Remove the padding of SDPCM data packets. This requires modifications in two
   places.

   In `targets/TARGET_WICED/WICED/WWD/internal/w_wwd_sdpcm.c` line 210:
   
   ``` diff
    typedef struct
    {
        sdpcm_common_header_t  common;
   -    uint8_t                _padding[2];
   +    //uint8_t                _padding[2];
        sdpcm_bdc_header_t     bdc_header;
    } sdpcm_data_header_t;
   ```

   In `targets/TARGET_WICED/WICED/WWD/internal/w_wwd_sdpcm.c` line 1282:
   
   ``` diff
        /* Prepare the SDPCM header */
     memset( (uint8_t*) &packet->sdpcm_header, 0, sizeof(sdpcm_header_t) );
     packet->sdpcm_header.sw_header.channel_and_flags = (uint8_t) header_type;
   - packet->sdpcm_header.sw_header.header_length = ( header_type == DATA_HEADER ) ? sizeof(sdpcm_header_t) + 2 : sizeof(sdpcm_header_t);
   + packet->sdpcm_header.sw_header.header_length = ( header_type == DATA_HEADER ) ? sizeof(sdpcm_header_t) /*+ 2*/ : sizeof(sdpcm_header_t);^M
     packet->sdpcm_header.sw_header.sequence = 0; /* Note: The real sequence will be written later */
     packet->sdpcm_header.frametag[0] = size;
     packet->sdpcm_header.frametag[1] = (uint16_t) ~size;
   ```

   Note this is required for the WISE1530 but may not be needed for other
   platforms.

1. Set up SDIO_ENUMERATION_TIMEOUT_MS to be configurable. Mbed OS will override this,
   depending on the current platform.

   In `targets/TARGET_WICED/WICED/WICED/platform/MCU/STM32F4xx/WWD/wwd_SDIO.c` line 66:
   
   ``` diff
     #define SDIO_IRQ_CHANNEL                     ((u8)0x31)
     #define DMA2_3_IRQ_CHANNEL                   ((u8)DMA2_Stream3_IRQn)
     #define BUS_LEVEL_MAX_RETRIES                (5)
   +
   + #ifndef SDIO_ENUMERATION_TIMEOUT_MS
     #define SDIO_ENUMERATION_TIMEOUT_MS          (500)
   + #endif
   ```

## Running the Mbed OS socket demo

For the WISE1530, you now have access to the Mbed OS WISE1530Interface class.

You can try this class with the Mbed OS socket example:
https://github.com/armmbed/mbed-os-example-sockets
 
 
 
 
 
 
