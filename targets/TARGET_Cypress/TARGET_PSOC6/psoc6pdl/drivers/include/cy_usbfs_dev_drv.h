/***************************************************************************//**
* \file cy_usbfs_dev_drv.h
* \version 1.10
*
* Provides API declarations of the USBFS driver.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


/**
* \addtogroup group_usbfs_dev_drv
* \{
* The USBFS driver provides API to interact with a fixed-function USB block.
*
* The functions and other declarations used in this driver are in cy_usbfs_dev_drv.h. 
* You can include cy_pdl.h (ModusToolbox only) to get access to all functions 
* and declarations in the PDL. 
* 
* The USB block supports Host and Device modes of operation. This version of the
* driver supports only Device mode.
*
* <b>Features:</b>
* * Complies with USB Specification 2.0
* * Supports full-speed peripheral device operation with a signaling bit 
*   rate of 12 Mbps.
* * Supports eight data endpoints and one control endpoint
* * Provides shared 512-byte buffer for data endpoints
* * Provides dedicated 8-byte memory for control endpoint (EP0)
* * Supports four types of transfers: bulk, interrupt, isochronous, and control
* * Supports bus- and self-powered configurations
* * Supports USB suspend, resume, and remove wakeup
* * Supports three types of logical transfer modes: 
*   * CPU (No DMA) mode (Mode 1).
*   * Manual DMA mode (Mode 2).
*   * Automatic DMA mode (Mode 3).
* * Supports maximum packet size: 
*   * 512 bytes using Mode 1 and Mode 2.
*   * 1023 bytes for isochronous transfer using Mode 3.
* * Provides integrated 22 Ohm USB termination resistors on D+ and D- lines, 
*   and 1.5 kOhm pull-up resistor on the D+ line
* * Supports USB 2.0 Link Power Management (LPM)
*
*
********************************************************************************
* \section group_usbfs_dev_drv_use_cases Common Use Cases
********************************************************************************
*
* <b>The primary usage model for the USBFS driver is to provide a defined API 
* interface to <a href="../../usbfs_dev_api_reference_manual.html" target="_blank">USB Device Middleware</a> 
* component that works on top of it.</b> \n
* The driver also provides an API interface for the application to implement required 
* functionality: 
*  * \ref group_usbfs_dev_drv_callbacks
*  * \ref group_usbfs_dev_drv_low_power
*  * \ref group_usbfs_dev_drv_lpm
*  * \ref group_usbfs_dev_drv_vbus
*
********************************************************************************
* \section group_usbfs_dev_drv_configuration Configuration Considerations
********************************************************************************
* 
* This section explains how to configure USBFS driver and system resources to 
* enable USB Device operation. The pointers to populated \ref cy_stc_usbfs_dev_drv_config_t configuration 
* structure and allocated context are passed in the middleware initialization 
* function Cy_USB_Dev_Init. After middleware initialization, it calls 
* \ref Cy_USBFS_Dev_Drv_Init to initialize the USBFS driver for Device operation.

********************************************************************************
* \subsection group_usbfs_dev_drv_config Configure Driver
********************************************************************************
* 
* To configure the USBFS driver in Device mode, the configuration structure 
* \ref cy_stc_usbfs_dev_drv_config_t parameters must be populated. 
* The configuration structure content significantly depends on the selected 
* endpoints management mode parameter:
*
* * \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU \n
*   The <i>epAccess, intrLevelSel and enableLpm</i> must be provided. All 
*   other parameters are do not cares for this mode. Refer to section 
*   \ref group_usbfs_dev_drv_intr to get information about <i>intrLevelSel</i> 
*   configuration.
*
*   \snippet usbfs/1.0/snippet/main.c snipped_Cy_USBFS_Dev_Drv_CfgCpu
*
* * \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA \n
*   To enable DMA operation the DMA channels must be assigned for each endpoint 
*   that will be used. Each DMA channel needs a single DMA descriptor to 
*   operate. The USBFS driver defines DMA configuration structure 
*   \ref cy_stc_usbfs_dev_drv_dma_config_t, which must be populated for each DMA 
*   channel.
*   The code example below provides initialized USBFS driver DMA configuration 
*   structure:
*   
*   \snippet usbfs/1.0/snippet/main.c snipped_Cy_USBFS_Dev_Drv_CfgDma_DmaInit
*
*   The pointers to the DMA configuration structure must provided into the 
*   \ref cy_stc_usbfs_dev_drv_config_t USBFS driver configuration structure
*   to allow USBFS driver use DMA channels for used endpoints. 
*   The <i>dmaConfig[0]</i> field expects a pointer to DMA configuration for 
*   data endpoint 1, the <i>dmaConfig[1]</i> field pointer to DMA configuration 
*   for data endpoint 2, and so on up to data endpoint 8.
*   The code example below provides initialized USBFS driver configuration 
*   structure which use endpoint 1:
*
*   \snippet usbfs/1.0/snippet/main.c snipped_Cy_USBFS_Dev_Drv_CfgDma
*   
* * \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO \n
*   DMA Automatic mode needs the similar DMA channels configuration as described 
*   above. But it also requires one more DMA descriptor for each DMA channel and 
*   DMA output trigger multiplexer. Find more information about trigger multiplexer 
*   in the \ref group_usbfs_dev_drv_dma section.
*   The code example below provides initialized USBFS driver DMA configuration 
*   structure:
*   
*   \snippet usbfs/1.0/snippet/main.c snipped_Cy_USBFS_Dev_Drv_CfgDmaAuto_DmaInit
*
*   The driver requires a buffer for data endpoints to operate. This buffer must be
*   allocated by the user. The buffer size is equal to the sum of all used 
*   endpoints maximum packet sizes. If an endpoint belongs to more than 
*   one alternate setting, select the greatest maximum packet size for this 
*   endpoint. The driver configuration structure \ref cy_stc_usbfs_dev_drv_config_t 
*   parameters have <i>epBuffer and epBufferSize</i> to pass the buffer to the driver.
*
*   The code example below provides initialized USBFS driver configuration 
*   structure that uses data endpoint 1 with a maximum packet size of 63 bytes and 
*   set 16-bit access:
*    
*   \snippet usbfs/1.0/snippet/main.c snipped_Cy_USBFS_Dev_Drv_CfgDmaAuto
*
*
* \note
* The endpoint buffer allocation depends on the access type used: 8-bit or 16-bit. 
* Refer to \ref group_usbfs_dev_drv_ep_management_buf_access for more information.
*
********************************************************************************
* \subsection group_usbfs_dev_drv_pins Assign and Configure Pins
********************************************************************************
*
* Only dedicated USB pins can be used for USB operation. Keep the default 
* USB pins configuration because after the USBFS driver initializes, the 
* USB block takes control over the pins and drives them properly.
*
********************************************************************************
* \subsection group_usbfs_dev_drv_clock Assign and Configure Clocks
********************************************************************************
*
* The USB hardware block requires two clock sources for operation:
* * Clk_HF3 (USB clock) must be 48 MHz. The accuracy of the USB clock must be 
*   within -/+ 0.25%. Note that Clk_HF3 has an integer divider so the input 
*   clock can be a multiple of 48. The valid options to get an internal USB 
*   clock are PLL or ECO.\n
*   <b>The typical configuration is:</b> the IMO output is used by the PLL to 
*   generate Clk_HF3 (USB clock). To meet USB clock accuracy requirements 
*   the IMO must be trimmed with USB SOF signal. Therefore, the driver 
*   \ref Cy_USBFS_Dev_Drv_Init function enables the IMO trim from USB.
*
* * Divided Clk_Peri clock (PCLK_USB_CLOCK_DEV_BRS) equal to 100 kHz, which is 
*   used to detect a Bus Reset event. Use one of the 8-bit or 16-bit dividers 
*   to provide required clock frequency.
*
* The code example below shows connection source path 1 
* (which expected provide 48 MHz -/+ 0.25% clock) to Clk_HF3 and Bus Reset clock 
* (Clk_Peri assumed to be 50 MHz):
*    
* \snippet usbfs/1.0/snippet/main.c snipped_Cy_USBFS_Dev_Drv_ClockInit
*
* Refer to \ref group_sysclk driver API for more detail about clock 
* configuration.
*
********************************************************************************
* \subsection group_usbfs_dev_drv_dma Assign and Route DMA Channels
********************************************************************************
*
* The USBFS driver requires the DMA controller to operate in DMA Manual and Automatic modes. 
* The USB hardware block supports the DMA request and feedback lines for each 
* data endpoint. Therefore, up to eight DMA channels are needed to service eight data 
* endpoints. 
* The connection between the USB block and the DMA channels is established using the trigger 
* muxes infrastructure. The USB block output DMA request line is connected to 
* the DMA channel trigger input. This allows the USB block to request a DMA transfer. 
* The DMA completion output is connected to the USB block burst end input. 
* This allows the USB block get notification that a DMA transfer has been completed 
* and next DMA request can be sent. The USBFS driver DMA configuration 
* structure requires the <i>outTrigMux</i> field to provide the trigger mux that 
* performs DMA completion and USB block burst end connection.
*
* Refer to \ref group_trigmux to get more information about routing capabilities.
* 
* The code examples below shows connection DMA channel and USB block and define 
* for <i>outTrigMux</i> field initialization for CY8C6xx6 or CY8C6xx7 devices.
*
* \snippet usbfs/1.0/snippet/main.c snipped_Cy_USBFS_Dev_Drv_DmaConnect
*
********************************************************************************
* \subsection group_usbfs_dev_drv_intr Configure Interrupts
********************************************************************************
*
* <b>The interrupts are mandatory for the USBFS driver operation.</b>
* The USBFS block provides multiple interrupt sources, which can be assigned to 
* trigger one of the three interrupts: Low, Medium, or High. This allows to 
* assign different priority for interrupt sources handling.
* The \ref cy_stc_usbfs_dev_drv_config_t structure provides field 
* <i>intrLevelSel</i> that initializes the INTR_LVL_SEL 
* register that configures to which interrupt is triggered by interrupt source. 
* 
* \note
* The interrupt name (Low, Medium, or High) does not specify the interrupt 
* priority. The interrupt priority is configured in the NVIC.
*
* The recommended/default configuration is: 
* * Interrupt Low: Bus Reset, Control Endpoint and SOF.
* * Interrupt Medium: Endpoint 1-8 Completion. 
* * Interrupt High: Arbiter and LPM. 
*
* However, the final configuration must be defined by the application.
*
* \snippet usbfs/1.0/snippet/main.c snipped_Cy_USBFS_Dev_Drv_UserLvlSel
*
* The \ref Cy_USBFS_Dev_Drv_Interrupt function must be called in the interrupt 
* handler for the selected USB block instance. Note that 
* the \ref Cy_USBFS_Dev_Drv_Interrupt has the parameter <i>intrCause</i> that 
* must be assigned by calling the appropriate interrupt cause function: 
* * \ref Cy_USBFS_Dev_Drv_GetInterruptCauseHi 
* * \ref Cy_USBFS_Dev_Drv_GetInterruptCauseMed 
* * \ref Cy_USBFS_Dev_Drv_GetInterruptCauseLo
*
* \snippet usbfs/1.0/snippet/main.c snipped_Cy_USBFS_Dev_Drv_IntrHandlers
*
* Finally, the interrupts must be configured and interrupt handler routines 
* hooked up to NVIC. The code below assigns interrupt priorities accordingly 
* to interrupt names: High interrupt has the greater priority than Low and 
* Medium interrupts, the Medium interrupt has the greater priority that 
* Low interrupt and Low interrupt has lowest priority among USBFS interrupts.
*
* \note
* For proper operation in Manual DMA mode (Mode 2) the Arbiter interrupt source 
* must be assigned to interrupt which priority is greater than interrupt 
* triggered by Data Endpoint 1-8 Completion interrupt sources. \n
* For Automatic DMA mode (Mode 3) the rule above is recommend to follow.
*
* \snippet usbfs/1.0/snippet/main.c snipped_Cy_USBFS_Dev_Drv_IntrCfg
* \snippet usbfs/1.0/snippet/main.c snipped_Cy_USBFS_Dev_Drv_IntrCfgHook
* 
********************************************************************************
* \section group_usbfs_dev_drv_ep_management Endpoint Buffer Management Modes
********************************************************************************
* 
* The USBFS hardware block supports three endpoint buffer management modes:
* CPU (No DMA) mode (Mode 1), Manual DMA mode (Mode 2) and Automatic DMA mode (Mode 3).
* These modes are listed using enum \ref cy_en_usbfs_dev_drv_ep_management_mode_t.
* The following sub-sections provide more information about endpoint buffer 
* management.
*
********************************************************************************
* \subsection group_usbfs_dev_drv_ep_management_buff Hardware buffers
********************************************************************************
*
* The USBFS block has a 512-byte hardware buffer that is divided between all 
* data endpoints used in the selected configuration. How the hardware buffer 
* is divided between endpoints depends on the selected endpoint buffer management
* modes:
*  
* * \ref group_usbfs_dev_drv_ep_management_mode1 and \ref  group_usbfs_dev_drv_ep_management_mode2
*   Each data endpoint consumes space (number of bytes) in the hardware buffer 
*   that is equal to the endpoint maximum packet size defined in the endpoint 
*   descriptor. The total space consumed by all endpoints is restricted
*   by the size of hardware buffer (512 bytes). When an endpoint appears in the 
*   different alternate settings and has a different maximum packet size, the greatest 
*   value is selected to the allocate space of the endpoint in the 
*   hardware buffer. This is to ensure correct USB Device operation when interface 
*   alternate settings are changed. Note that endpoint can consume extra byte in 
*   the hardware buffer when 16-bit access is used (See \ref 
*   group_usbfs_dev_drv_ep_management_buf_access for more information).
*
* * \ref group_usbfs_dev_drv_ep_management_mode3
*   Each data endpoint consumes 32 bytes in the hardware buffer (if all eight 
*   endpoints are used, consumed buffer space is 32 * 8 = 256 byte). 
*   This buffer is called the dedicated endpoint buffer. It acts as an endpoint 
*   FIFO. The remaining space (256 bytes, if all eight endpoints are 
*   used) in the hardware buffer is used by any endpoint that currently 
*   communicates. This part of the buffer is called the common area. This hardware 
*   buffer configuration gives sufficient dedicated buffer size for each used 
*   endpoint and common area for operation. The total space consumed by all 
*   endpoints is not restricted by the size of hardware buffer.
*
* To access the hardware buffer, the endpoint data register is read or written by 
* the CPU or DMA. On each read or write, the buffer pointers are updated to access
* the next data element.
*
********************************************************************************
* \subsection group_usbfs_dev_drv_ep_management_buf_access Hardware Buffer Access
********************************************************************************
*
* The USBFS block provides two sets of data registers: 8-bit and 16-bit. Either
* the 8-bit endpoint data register or the 16-bit endpoint data register can 
* be used to read/write to the endpoint buffer. The buffer access is controlled 
* by the <i>epAccess</i> field of the driver configuration structure 
* \ref cy_stc_usbfs_dev_drv_config_t. 
* The endpoint hardware buffer and SRAM buffer must be allocated using the 
* rules below when 16-bit access is used:
* * Buffer size must be even. If endpoint maximum packet size is odd 
*   the allocated buffer size must be equal to (maximum packet size + 1).
* * Buffer must be aligned to 2-byte boundary.
*
* The driver provides the \ref CY_USBFS_DEV_DRV_ALLOC_ENDPOINT_BUFFER macro that 
* applying rules above to allocate the SRAM buffer for endpoint. This macro 
* should be used by application to hide configuration differences. 
* <b>However, in this case the application must ignore extra bytes in the buffer.</b>
* Alternately, apply rules above only for 16-bits access type configuration.
*
* The driver firmware allocates endpoint hardware buffer (dividing hardware buffer 
* between utilized endpoints). Therefore, for \ref group_usbfs_dev_drv_ep_management_mode1 
* and \ref group_usbfs_dev_drv_ep_management_mode2, the endpoints whose 
* maximum packet size is odd consumes an extra byte in the hardware buffer 
* when 16-bit access is used. This is not applicable for \ref group_usbfs_dev_drv_ep_management_mode3 
* because endpoints dedicated buffer are even and aligned.
*
* In addition, to operate in \ref group_usbfs_dev_drv_ep_management_mode3, 
* the driver needs an internal SRAM buffer for endpoints. The buffer size is a 
* sum of all endpoint buffers. When 16-bit access is used, each endpoint buffer 
* must be allocated using the rules above. The driver configuration structure 
* \ref cy_stc_usbfs_dev_drv_config_t has <i>epBuffer and epBufferSize</i> fields 
* to pass the allocated buffer to the driver. \n
* For example: the USB Device uses three data endpoint whose max packets are
* 63 bytes, 63 bytes, and 8 bytes. The endpoints buffer for the driver must be 
* allocated as follows:
* *  8-bits: uint8_t endpointsBuffer[63 + 63 + 8];
* * 16-bits: uint8_t endpointsBuffer[(63+1) + (63+1) + 8] CY_ALLIGN(2); or 
*   CY_USBFS_DEV_DRV_ALLOC_ENDPOINT_BUFFER((63+1) + (63+1) + 8);
* 
********************************************************************************
* \subsection group_usbfs_dev_drv_ep_management_mode1 CPU mode (Mode 1)
********************************************************************************
*
* The CPU handles data transfers between the user provided SRAM endpoint buffer 
* and the USB block hardware buffer when \ref Cy_USBFS_Dev_Drv_ReadOutEndpoint 
* or \ref Cy_USBFS_Dev_Drv_LoadInEndpoint is called.
*
* \image html usbfs_ep_mngmnt_mode1.png
*
********************************************************************************
* \subsection group_usbfs_dev_drv_ep_management_mode2 Manual DMA mode (Mode 2)
********************************************************************************
*
* The DMA handles data transfers between the user-provided SRAM endpoint 
* buffer and the USB block hardware buffer. The DMA request is issued by the CPU 
* to execute a data transfer when \ref Cy_USBFS_Dev_Drv_ReadOutEndpoint or 
* \ref Cy_USBFS_Dev_Drv_LoadInEndpoint.
*
* \image html usbfs_ep_mngmnt_mode2.png
*
********************************************************************************
* \subsection group_usbfs_dev_drv_ep_management_mode3 Automatic DMA mode (Mode 3)
********************************************************************************
* 
* The DMA handles data transfers between the driver SRAM endpoints buffer and 
* the USB block hardware buffer. The USB block generates DMA requests  
* automatically. When USB transfer starts, the USB block triggers DMA 
* requests to transfer data between the driver endpoint buffer and the hardware 
* buffer until transfer completion. The common area acts as a FIFO to (and keeps 
* data that does not fit into) the endpoint dedicated buffer. For IN endpoints, 
* the dedicated buffer is pre-loaded before enabling USB Host access to the endpoint. 
* This gives time for the DMA to provide remaining data before underflow 
* occurs. The USB block hardware has a feedback connection with the DMA 
* and does not issue new DMA request until it receives notification that the 
* previous DMA transfer completed. 
* When the \ref Cy_USBFS_Dev_Drv_ReadOutEndpoint or \ref Cy_USBFS_Dev_Drv_LoadInEndpoint 
* function is called, the memcpy function is used to copy data from/into 
* driver endpoints buffer to the user-provided endpoint buffer.
* The driver provides the \ref Cy_USBFS_Dev_Drv_OverwriteMemcpy function to 
* replace memcpy function by one that has been custom implemented (the DMA can be used for data copy).
* 
* \image html usbfs_ep_mngmnt_mode3.png
*
* \warning 
* When the DMA is not capable of transferring data fast enough, the overflow or 
* underflow interrupt triggers for impacted endpoint. This must never happen 
* because this error condition indicates system failure with no recovery. 
* To fix this, give the DMA channel assigned to this endpoint greater priority or 
* increase the clock that the DMA operates.
*
********************************************************************************
* \section group_usbfs_dev_drv_callbacks Callbacks Usage
********************************************************************************
*
* The driver provides following callbacks that can be used by the application:
* 1. Data endpoint 1-8 completion. This callback is invoked when the USB Host 
* completed communication with the endpoint. For IN endpoints, it means that data has 
* been read by the USB Host. For OUT endpoints, it means that data has been written 
* by the USB Host. Call \ref Cy_USBFS_Dev_Drv_RegisterEndpointCallback to 
* register callback function.
* 
* 2. Start Of Frame packet received. This can be used in the application to 
* synchronize with SOF packets or for monitoring the bus activity. 
* Call \ref Cy_USBFS_Dev_Drv_RegisterSofCallback to register callback function. 
* 
* 3. LPM (Link Power Management) packet received. This must be used to implement 
* LPM power optimization. Call \ref Cy_USBFS_Dev_Drv_RegisterLpmCallback to 
* register callback function.
* 
* Also the driver provides callbacks for a Bus Reset event and Control Endpoint 0 
* communication events (setup packet, in packet, out packet). But these 
* callbacks are used by middleware and must not be used by the application directly. 
* The middleware provides appropriate hooks for these events.
*
********************************************************************************
* \section group_usbfs_dev_drv_vbus VBUS Detection
********************************************************************************
*
* The USB specification requires that no device supplies current on VBUS at its 
* upstream facing port at any time. To meet this requirement, the device must 
* monitor for the presence or absence of VBUS and remove power from the Dp/Dm 
* pull-up resistor if VBUS is absent. The USBFS driver does not provide any 
* support of VBUS monitoring or detection. The application firmware must implement 
* required functionality using a VDDUSB power pad or GPIO. Refer to the section 
* Universal Serial Bus (USB) Device Mode sub-section VBUS Detection in the technical 
* reference manual (TRM).
* 
* Connect the VBUS through the resistive network when the 
* regular GPIO is used for VBUS detection to save the pin from voltage picks on VBUS, 
* or use GPIO that is tolerant over voltage. An example schematic is shown below.
* 
* \image html usbfs_vbus_connect_schem.png
* 
* \note The power is removed when the USB cable is removed from the USB Host 
* for bus-powered USB Device. Therefore, such a USB Device complies with 
* specification requirement above.
*
********************************************************************************
* \section group_usbfs_dev_drv_low_power Low Power Support
********************************************************************************
*
* The USBFS driver supports USB Suspend, Resume, and Remote Wakeup functionality. 
* This functionality is tightly related with the user application. The USBFS 
* driver provides only the API interface, which helps the user achieve desired 
* low-power behavior. The additional processing is required from the user application. 
* The description of application processing is provided below.
*
* Normally, the USB Host sends SOF packet every 1 ms (at full speed), and this is
* what keeps the USB Device awake. The USB Host suspends the USB Device by not 
* sending anything to the USB Device for 3 ms. To recognize this condition, the bus 
* activity must be checked. This can be done using the \ref Cy_USBFS_Dev_Drv_CheckActivity 
* function or by monitoring the SOF interrupt. A suspended device may draw no 
* more than 0.5 mA from VBUS. Therefore, put device into low-power
* mode to consume less current. 
* The \ref Cy_USBFS_Dev_Drv_Suspend function must be called before entering 
* low-power mode. When the USB Host wants to wake the device after a suspend, 
* it does so by reversing the polarity of the signal on the data lines for at 
* least 20 ms. The resume signaling is completed with a low-speed end-of-packet 
* signal. The USB block is disabled during Deep Sleep or Hibernate low-power modes. 
* To exit a low-power mode when USB Host drives resume, a falling edge interrupt 
* on Dp must be configured before entering these modes. The \ref Cy_USBFS_Dev_Drv_Resume 
* function must be called after exiting the low-power mode. To resume communication with 
* the USB Host, the data endpoints must be managed: the OUT endpoints must be 
* enabled and IN endpoints must be loaded with data.
* 
* \note After entering low-power mode, the data which was left in the IN or OUT 
* endpoint buffers is not restored after device wakes up and is lost. Therefore, 
* it must be stored in the SRAM for OUT endpoint or read by the Host for IN 
* endpoint before entering low-power mode.
*
* If the USB Device supports remote wakeup functionality, the application has 
* to use middleware function Cy_USB_Dev_IsRemoteWakeupEnabled to determine whether 
* remote wakeup was enabled by the USB Host. When the device is suspended and 
* it determines the conditions to initiate a remote wakeup are met, 
* the application must call the \ref Cy_USBFS_Dev_Drv_Force
* function to force the appropriate J and K states onto the USB bus, signaling a 
* remote wakeup condition. Note that \ref Cy_USBFS_Dev_Drv_Resume must be called 
* first to restore the condition.
* 
********************************************************************************
* \section group_usbfs_dev_drv_lpm Link Power Management (LPM)
********************************************************************************
*
* Link Power Management is a USB low-power mode feature that provides more 
* flexibility in terms of features than the existing resume mode. This feature 
* is similar to the existing Suspend/Resume, but has transitional latencies of 
* tens of microseconds between power states (instead of 3 to greater than 20 
* millisecond latencies of the USB 2.0 Suspend/Resume).
* 
* USB2.0 Power states are re-arranged as below with the introduction of LPM. 
* The existing power states are re-named with LPM:
* * L0 (On) 
* * L1 (Sleep) -- Newly Introduced State in LPM 
* * L2 (Suspend) 
* * L3 (Powered-Off)
* 
* LPM state transitions between is shown below:
*
* \image html usbfs_lpm_state_transition.png
* 
* For example, a USB Host must transition a link from L1 (Sleep) to L0 before 
* transitioning it to L2 (Suspend), and similarly when transitioning from L2 to L1.
* 
* When a USB Host is ready to transition a USB Device from L0 to a deeper power 
* savings state, it issues an LPM transaction to the USB Device. The USB Device 
* function responds with an ACK if it is ready to make the transition or a NYET 
* (Not Yet) if it is not ready (usually because it is has data pending for the 
* USB Host). A USB Device will transmit a STALL handshake if it does not support 
* the requested link state. If the USB Device detects errors in either of the 
* token packets or does not understand the protocol extension transaction, 
* no handshake is returned.
*
* \image html usbfs_lpm_responses.png
* 
* After USB Device is initialized the LPM transaction are to acknowledged (ACKed) 
* meaning that device is ready to enter requested low-power mode. To override this 
* behavior, use \ref Cy_USBFS_Dev_Drv_Lpm_SetResponse. \n
* 
* The USB block provides an interrupt source to define that LPM transaction was 
* received and acknowledged (ACKed). Use the \ref Cy_USBFS_Dev_Drv_RegisterLpmCallback 
* function to register application level callback function to service the LPM 
* transaction. The callback function can notify the application about an LPM transaction
* and can use \ref Cy_USBFS_Dev_Drv_Lpm_GetBeslValue read to read Best Effort Service 
* Latency (BESL) values provided as part of an LPM transaction. The BESL value 
* indicates the  amount of time from the start of a resume to when the USB Host 
* attempts to begin issuing transactions to the USB Device. The 
* application must use the value BESL to decide which low-power mode is entered
* to meet wakeup timing. The LPM transaction also contains the field that allows 
* remote wake up. Use \ref Cy_USBFS_Dev_Drv_Lpm_RemoteWakeUpAllowed to get its 
* value.
* 
* LPM related USB 2.0 Extension Descriptor provides attributes fields named 
* baseline BESL and deep BESL to provide a range of values for different low-power
* optimization. The recommended use of these fields is that the 
* baseline BESL field will have a value less than the deep BESL field. The 
* expected use is the baseline BESL value communicates a nominal power savings 
* design point and the deep BESL value communicates a significant power saving 
* design point.
* For example when the received BESL is less than baseline BESL, leave the device in the 
* Active mode. When it is between baseline BESL and deep BESL, put the device into 
* the Deep Sleep mode. When it is greater than deep BESL, put the device into 
* Hibernate mode.
* 
* \note 
* The driver implements the USB Full-Speed device which does not support LPM 
* NYET response.
*
* \note 
* The device will restart after Hibernate mode and the USB Device must 
* be initialized at the application level. Call initialization functions 
* instead of \ref Cy_USBFS_Dev_Drv_Resume. The application must ensure that 
* the device will resume within the time defined in the BESL value of LPM request.  
*
********************************************************************************
* \section group_usbfs_drv_more_information More Information
********************************************************************************
* 
* For more information on the USB Full-Speed Device peripheral, refer to the 
* section Universal Serial Bus (USB) Device Mode in technical reference 
* manual (TRM).
*
********************************************************************************
* \section group_usbfs_drv_MISRA MISRA-C Compliance
********************************************************************************
*
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>11.4</td>
*     <td>A</td>
*     <td>A cast should not be performed between a pointer to object type and
*         a different pointer to object type.</td>
*     <td>The function \ref Cy_USBFS_Dev_Drv_LoadInEndpoint and 
*         \ref Cy_USBFS_Dev_Drv_ReadOutEndpoint cast buffer parameters from 
*         (uint8_t *) to (uint16_t *) when 16-bit access is enabled. 
*         To handle alignment issues the macro  
*         \ref CY_USBFS_DEV_DRV_ALLOC_ENDPOINT_BUFFER must be used to 
*        allocate the buffer for the endpoint.</td>
*   </tr>
*   <tr>
*     <td>11.5</td>
*     <td>R</td>
*     <td>A cast shall not be performed that removes any const or volatile
*         qualification from the type addressed by a pointer.</td>
*     <td>
*      1. The register access macros cast base pointers to USBFS peripheral 
*         registers losing const qualification. Despite the qualification being 
*         lost, the driver ensures proper registers access.
*      2. The volatile qualification is lost when a register address is passed 
*         as source or destination to the DMA channel. This does not cause any 
*         negative impact because the DMA does not optimize any memory access.
*     </td>
*   </tr>
*   <tr>
*     <td>14.7</td>
*     <td>R</td>
*     <td>A function shall have a single point of exit at the end of the
*         function.</td>
*     <td>The functions can return from several points. This is typically 
*         done to improve code clarity when returning error status code if 
*         input parameters validation fail.</td>
*   </tr>
*   <tr>
*     <td>16.7</td>
*     <td>A</td>
*     <td>A pointer parameter in a function prototype should be declared as
*         pointer to const if the pointer is not used to modify the addressed
*         object.</td>
*     <td>The middleware and USBFS driver defines general function 
*         prototypes and pointers to function types but the function's 
*         implementation depends on configuration. Therefore, 
*         some functions' implementations require parameters to be pointer to 
*         const but this is not met because of the generalized implementation 
*         approach.</td>
*   </tr>
* </table>
*
********************************************************************************
* \section group_usbfs_drv_changelog Changelog
********************************************************************************
*
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td rowspan="2"> 1.10</td>
*     <td>Fixed the \ref Cy_USBFS_Dev_Drv_Disable function to not disable DMA
*         in CPU mode.</td>
*     <td>Calling this function triggers assert because DMA for endpoints is not 
*         initialized/used in the CPU mode.</td>
*   </tr>
*   <tr>
*     <td>Updated the condition statement in the \ref CY_USBFS_DEV_DRV_ALLOC_ENDPOINT_BUFFER
*         macro to explicitly check against non-zero.</td>
*     <td>Fixed MISRA 13.2 violation in the macro.</td>
*   </tr>
*   <tr>
*     <td>1.0</td>
*     <td>The initial version.</td>
*     <td></td>
*   </tr>
* </table>
*
********************************************************************************
*
* \defgroup group_usbfs_dev_drv_macros Macros
* \{
      \defgroup group_usbfs_dev_drv_macros_intr_level Interrupt Level
*     \defgroup group_usbfs_dev_drv_macros_intr_cause Interrupt Cause
*     \defgroup group_usbfs_dev_drv_macros_ep_xfer_err Transfer Errors
* \}
* \defgroup group_usbfs_dev_drv_functions Functions
* \{
*     \defgroup group_usbfs_dev_hal_functions_common          Initialization Functions
*     \defgroup group_usbfs_dev_drv_functions_interrupts      Interrupt Functions
*     \defgroup group_usbfs_dev_hal_functions_ep0_service     Endpoint 0 Service Functions
*     \defgroup group_usbfs_dev_hal_functions_endpoint_config Data Endpoint Configuration Functions
*     \defgroup group_usbfs_dev_hal_functions_data_xfer       Data Endpoint Transfer Functions
*     \defgroup group_usbfs_dev_drv_functions_low_power       Low Power Functions
*     \defgroup group_usbfs_dev_drv_functions_lpm             LPM (Link Power Management) Functions
* \} 
* \defgroup group_usbfs_dev_drv_data_structures Data Structures
* \defgroup group_usbfs_dev_drv_enums           Enumerated Types
* \} 
*/


#if !defined(CY_USBFS_DEV_DRV_H)
#define CY_USBFS_DEV_DRV_H

#include "cy_dma.h"
#include "cy_trigmux.h"
#include "cy_usbfs_dev_drv_reg.h"

#ifdef CY_IP_MXUSBFS

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
*                           Driver version and ID
*******************************************************************************/

/**
* \addtogroup group_usbfs_dev_drv_macros
* \{
*/

/** USBFS Driver major version */
#define CY_USBFS_VERSION_MAJOR      (1)

/** USBFS Driver minor version */
#define CY_USBFS_VERSION_MINOR      (10)

/** USBFS Driver identifier */
#define CY_USBFS_ID                 CY_PDL_DRV_ID(0x3BU)

/** USBFS Driver mode position in STATUS CODE: 0 - Device, 1 - Host */
#define CY_USBFS_MODE_POS           (15UL)

/** USBFS Driver status code Device */
#define CY_USBFS_DEV_DRV_STATUS_CODE    (0U)
/** \} group_usbfs_dev_drv_macros */


/*******************************************************************************
*                           Enumerated Types
*******************************************************************************/

/**
* \addtogroup group_usbfs_dev_drv_enums
* \{
*/
/** USBFS Device Driver return codes */
typedef enum
{
    /** Operation completed successfully */
    CY_USBFS_DEV_DRV_SUCCESS = 0U,
    
    /** One or more input parameters are invalid */                                
    CY_USBFS_DEV_DRV_BAD_PARAM               = (CY_USBFS_ID | CY_PDL_STATUS_ERROR | CY_USBFS_DEV_DRV_STATUS_CODE | 1U), 
    
    /** There is not enough space in the buffer to be allocated for endpoint (hardware or RAM) */ 
    CY_USBFS_DEV_DRV_BUF_ALLOC_FAILED        = (CY_USBFS_ID | CY_PDL_STATUS_ERROR | CY_USBFS_DEV_DRV_STATUS_CODE | 2U),        
    
    /** Failure during DMA configuration */                                                     
    CY_USBFS_DEV_DRV_DMA_CFG_FAILED          = (CY_USBFS_ID | CY_PDL_STATUS_ERROR | CY_USBFS_DEV_DRV_STATUS_CODE | 3U),       
    
    /** Timeout during dynamic reconfiguration */                                            
    CY_USBFS_DEV_DRV_EP_DYN_RECONFIG_TIMEOUT = (CY_USBFS_ID | CY_PDL_STATUS_ERROR | CY_USBFS_DEV_DRV_STATUS_CODE | 4U), 
    
    /** Timeout during execution of the DMA read request for OUT endpoint 
    * (only applicable in \ref group_usbfs_dev_drv_ep_management_mode2) 
    */
    CY_USBFS_DEV_DRV_EP_DMA_READ_TIMEOUT     = (CY_USBFS_ID | CY_PDL_STATUS_ERROR | CY_USBFS_DEV_DRV_STATUS_CODE | 5U), 

    /** Timeout during execution of the DMA read request for OUT endpoint 
    * (only applicable in \ref group_usbfs_dev_drv_ep_management_mode2) 
    */
    CY_USBFS_DEV_DRV_EP_DMA_WRITE_TIMEOUT     = (CY_USBFS_ID | CY_PDL_STATUS_ERROR | CY_USBFS_DEV_DRV_STATUS_CODE | 6U),

} cy_en_usbfs_dev_drv_status_t; 


/** Data Endpoints Buffer Management Mode */
typedef enum
{
    /** The CPU manages data transfer between the hardware endpoints buffer 
    * and the user SRAM
    */
    CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU = 0,
    
    /** The DMA manages data transfer between the hardware endpoints buffer and
    * the user SRAM
    */
    CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA = 1,

    /** The DMA automatically manages data transfer between the hardware endpoints 
    * FIFO buffer and the user SRAM
    */
    CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO = 2,
} cy_en_usbfs_dev_drv_ep_management_mode_t;

/** Data Endpoint Register Access Type */
typedef enum
{
    CY_USBFS_DEV_DRV_USE_8_BITS_DR,     /**< Use 8-bits registers to access data endpoints */
    CY_USBFS_DEV_DRV_USE_16_BITS_DR,    /**< Use 16-bits registers to access data endpoints */
} cy_en_usbfs_dev_ep_access_t;

/** Service Callback Events (this enumerated type is used by middleware) */
typedef enum
{
    CY_USB_DEV_BUS_RESET = 0U,   /**< Callback hooked to bus reset interrupt */
    CY_USB_DEV_EP0_SETUP = 1U,   /**< Callback hooked to endpoint 0 SETUP packet interrupt */
    CY_USB_DEV_EP0_IN    = 2U,   /**< Callback hooked to endpoint 0 IN packet interrupt */
    CY_USB_DEV_EP0_OUT   = 3U,   /**< Callback hooked to endpoint 0 OUT packet interrupt */
} cy_en_usb_dev_service_cb_t;

/** Callback Sources */
typedef enum
{
    CY_USBFS_DEV_DRV_EP1 = 0U,   /**< Callback hooked to Data Endpoint 1 completion interrupt */
    CY_USBFS_DEV_DRV_EP2 = 1U,   /**< Callback hooked to Data Endpoint 2 completion interrupt */
    CY_USBFS_DEV_DRV_EP3 = 2U,   /**< Callback hooked to Data Endpoint 3 completion interrupt */
    CY_USBFS_DEV_DRV_EP4 = 3U,   /**< Callback hooked to Data Endpoint 4 completion interrupt */
    CY_USBFS_DEV_DRV_EP5 = 4U,   /**< Callback hooked to Data Endpoint 5 completion interrupt */
    CY_USBFS_DEV_DRV_EP6 = 5U,   /**< Callback hooked to Data Endpoint 6 completion interrupt */
    CY_USBFS_DEV_DRV_EP7 = 6U,   /**< Callback hooked to Data Endpoint 7 completion interrupt */
    CY_USBFS_DEV_DRV_EP8 = 7U,   /**< Callback hooked to Data Endpoint 8 completion interrupt */
    CY_USBFS_DEV_DRV_SOF = 8U,   /**< Callback hooked to SOF packet received interrupt */
    CY_USBFS_DEV_DRV_LPM = 9U,   /**< Callback hooked to LPM request received interrupt */
} cy_en_usbfs_dev_drv_cb_source_t;

/** Data Endpoint States (this enumerated type is used by middleware) */
typedef enum
{
    CY_USB_DEV_EP_IDLE,       /**< Endpoint is in idle state after configuration is set */
    CY_USB_DEV_EP_PENDING,    /**< Transfer targeted to an endpoint is in progress */
    CY_USB_DEV_EP_COMPLETED,  /**< Transfer targeted to an endpoint is completed */
    CY_USB_DEV_EP_STALLED,    /**< Endpoint is stalled */
    CY_USB_DEV_EP_DISABLED,   /**< Endpoint is disabled (not used in this configuration) */
    CY_USB_DEV_EP_INVALID,     /**< Endpoint does not supported by the hardware */
}  cy_en_usb_dev_ep_state_t;

/** USB Lines Control */
typedef enum
{
    CY_USBFS_DEV_DRV_FORCE_STATE_J    = 0xA0U,  /**< Force a J State onto the USB lines */
    CY_USBFS_DEV_DRV_FORCE_STATE_K    = 0x80U,  /**< Force a K State onto the USB lines */
    CY_USBFS_DEV_DRV_FORCE_STATE_SE0  = 0xC0U,  /**< Force a Single Ended 0 onto the USB lines */
    CY_USBFS_DEV_DRV_FORCE_STATE_NONE = 0x00U   /**< Return bus to SIE control */
} cy_en_usbfs_dev_drv_force_bus_state_t;

/** LPM (Link Power Management) Responses */
typedef enum
{
    /** The next LPM request will be responded with NACK */
    CY_USBFS_DEV_DRV_LPM_REQ_NACK = 0x0U,  

    /** The next LPM request will be responded with ACK */
    CY_USBFS_DEV_DRV_LPM_REQ_ACK  = 0x1U,
} cy_en_usbfs_dev_drv_lpm_req_t;
/** \} group_usbfs_dev_drv_enums */


/*******************************************************************************
*                                  Type Definitions
*******************************************************************************/

/**
* \addtogroup group_usbfs_dev_drv_data_structures
* \{
*/

/** Data Endpoint Configuration Structure */
typedef struct
{
    bool     enableEndpoint;    /**< Defines if endpoint becomes active after configuration */
    bool     allocBuffer;       /**< Defines if buffer allocation is need for endpoint */
    uint16_t maxPacketSize;     /**< Endpoint max packet size */
    uint16_t bufferSize;        /**< Endpoint buffer size (the biggest max packet size
                                     across all alternate for this endpoint) */
    uint8_t  endpointAddr;      /**< Endpoint address (number plus direction bit) */
    uint8_t  attributes;        /**< Endpoint attributes */ 
} cy_stc_usb_dev_ep_config_t;

/**
* Driver context structure prototype.
* The driver define this structure type \ref cy_stc_usbfs_dev_drv_context_t.
*/
struct cy_stc_usbfs_dev_drv_context;

/**
* Provides the typedef for the callback function called in the
* \ref Cy_USBFS_Dev_Drv_Interrupt to notify the user interrupt events.
*/
typedef void (* cy_cb_usbfs_dev_drv_callback_t)(USBFS_Type *base, 
                                                struct cy_stc_usbfs_dev_drv_context *context);

/**
* Provides the typedef for the callback function called in the
* \ref Cy_USBFS_Dev_Drv_Interrupt to notify the user about endpoint transfer 
* completion event.
*/
typedef void (* cy_cb_usbfs_dev_drv_ep_callback_t)(USBFS_Type *base, 
                                                   uint32_t errorType, 
                                                   struct cy_stc_usbfs_dev_drv_context *context);

/**
* Provides the typedef for the user defined function to replace library provided 
* memcpy function to copy data from endpoint buffer to the user buffer.
*/
typedef uint8_t * (* cy_fn_usbfs_dev_drv_memcpy_ptr_t)(uint8_t *dest, 
                                                       const uint8_t *src, 
                                                       uint32_t size);

/** \cond INTERNAL*/

/**
* Specifies the typedef for the pointer to the function that adds a data endpoint
*/
typedef cy_en_usbfs_dev_drv_status_t (* cy_fn_usbfs_dev_drv_add_ep_ptr_t)
                                        (USBFS_Type *base,
                                         cy_stc_usb_dev_ep_config_t const *config,
                                         struct cy_stc_usbfs_dev_drv_context     *context);

/**
* Specifies the typedef for the pointer to the function that loads data into
* the data endpoint.
*/
typedef cy_en_usbfs_dev_drv_status_t (* cy_fn_usbfs_dev_drv_load_ep_ptr_t)
                                        (USBFS_Type    *base,
                                         uint32_t       endpoint,
                                         const uint8_t *buffer,
                                         uint32_t       size,
                                         struct cy_stc_usbfs_dev_drv_context *context);

/**
* Specifies the typedef for the pointer to the function that reads data from
* the data endpoint.
*/
typedef cy_en_usbfs_dev_drv_status_t (* cy_fn_usbfs_dev_drv_read_ep_ptr_t)
                                        (USBFS_Type *base,
                                         uint32_t    endpoint,
                                         uint8_t    *buffer,
                                         uint32_t    size,
                                         uint32_t   *actSize,
                                         struct cy_stc_usbfs_dev_drv_context *context);
/** \endcond */

/** DMA Channel Configuration Structure */
typedef struct
{
    DW_Type *base;          /**< Pointer to the DMA base */
    uint32_t chNum;         /**< Channel number */
    uint32_t priority;      /**< Channel's priority */
    bool     preemptable;   /**< Specifies whether the channel is preempt-able by another higher-priority channel */

    /** DMA out trigger mux (applicable only when mode is 
    * \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO)
    */
    uint32_t outTrigMux;

    /** Pointer to the 1st allocated DMA descriptor (required for DMA operation) */
    cy_stc_dma_descriptor_t *descr0;

    /** Pointer to the 2nd allocated DMA descriptor (required when mode is 
    * \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO) 
    */
    cy_stc_dma_descriptor_t *descr1;

} cy_stc_usbfs_dev_drv_dma_config_t;

/** Driver Configuration Structure */
typedef struct cy_stc_usbfs_dev_drv_config
{
    /** Endpoints management mode */
    cy_en_usbfs_dev_drv_ep_management_mode_t mode;

    /** DMA channels configuration for the endpoints.
    * Only DMA channels for active endpoints must be configured. Provide NULL 
    * pointer if endpoint is not used. Applicable when \ref mode is 
    * \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA or \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO.
    */
    const cy_stc_usbfs_dev_drv_dma_config_t *dmaConfig[CY_USBFS_DEV_DRV_NUM_EPS_MAX];

    /** 
    * Pointer to the buffer allocated for OUT endpoints (applicable only when \ref mode 
    * is \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO)
    */
    uint8_t  *epBuffer;     
    
    /** 
    * The size of the buffer for OUT endpoints (applicable only when \ref mode 
    * is \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO)
    */
    uint16_t  epBufferSize;

    /** Mask that assigns interrupt sources to trigger: Low, Medium, or High interrupt.
    * Use macros provided in group_usbfs_dev_drv_macros_intr_level to initialize 
    * intrLevelSel mask.
    */
    uint32_t intrLevelSel;

    /** Enables LPM (Link Power Management) response */
    bool enableLpm;

    /** Data endpoints access type */
    cy_en_usbfs_dev_ep_access_t epAccess;

} cy_stc_usbfs_dev_drv_config_t;

/** \cond INTERNAL: Endpoint Structure */
typedef struct
{
    volatile uint8_t address; /**< Endpoint address (include direction bit) */
    volatile uint8_t toggle;  /**< Toggle bit in SIE_EP_CNT1 register */
    volatile uint8_t sieMode; /**< SIE mode to arm endpoint on the bus */

    uint8_t  *buffer;              /**< Pointer to the buffer */
    volatile uint16_t bufferSize;  /**< Endpoint buffer size */
    volatile uint16_t startBuf;    /**< Start of the buffer */

    volatile bool isPending;   /**< Save pending state before stall endpoint */
    volatile cy_en_usb_dev_ep_state_t state; /**< Endpoint state */

    /** Complete event notification callback */
    cy_cb_usbfs_dev_drv_ep_callback_t epComplete;

    DW_Type *base;          /**< Pointer to the DMA base */
    uint32_t chNum;         /**< DMA Channel number */
    uint32_t outTrigMux;    /**< Out trigger mux for DMA channel number */

    cy_stc_dma_descriptor_t* descr0;    /**< Pointer to the descriptor 0 */
    cy_stc_dma_descriptor_t* descr1;    /**< Pointer to the descriptor 1 */
    
    cy_fn_usbfs_dev_drv_memcpy_ptr_t copyData;  /**< Pointer to user memcpy function */

} cy_stc_usbfs_dev_drv_endpoint_data_t;
/** \endcond */

/** USBFS Device context structure.
* All fields for the context structure are internal. Firmware never reads or
* writes these values. Firmware allocates the structure and provides the
* address of the structure to the middleware in HID function calls. Firmware 
* must ensure that the defined instance of this structure remains in scope while
* the middleware is in use.
*/
typedef struct cy_stc_usbfs_dev_drv_context
{
    /** \cond INTERNAL */
    /** Endpoints management mode */
    cy_en_usbfs_dev_drv_ep_management_mode_t mode;

    /** Defines which endpoint registers to use: 8-bits or 16-bits */
    bool useReg16;

    /** Bus reset callback notification */
    cy_cb_usbfs_dev_drv_callback_t busReset;

    /** Endpoint 0: Setup packet has received callback notification */
    cy_cb_usbfs_dev_drv_callback_t ep0Setup;

    /** Endpoint 0: IN data packet has received callback notification */
    cy_cb_usbfs_dev_drv_callback_t ep0In;

    /** Endpoint 0: OUT data packet has received callback notification */
    cy_cb_usbfs_dev_drv_callback_t ep0Out;

    /** SOF frame has received callback notification */
    cy_cb_usbfs_dev_drv_callback_t cbSof;

    /** LPM request has received callback notification */
    cy_cb_usbfs_dev_drv_callback_t cbLpm;

    /** Pointer to addEndpoint function: depends on operation mode */
    cy_fn_usbfs_dev_drv_add_ep_ptr_t addEndpoint;

    /** Pointer to loadInEndpoint function: depends on operation mode */
    cy_fn_usbfs_dev_drv_load_ep_ptr_t loadInEndpoint;

    /** Pointer to readOutEndpoint function: depends on operation mode */
    cy_fn_usbfs_dev_drv_read_ep_ptr_t readOutEndpoint;

    uint32_t ep0ModeReg;  /**< Endpoint 0 Mode register */
    uint32_t ep0CntReg;   /**< Endpoint 0 Counter register */

    uint8_t  *epSharedBuf;     /**< Buffer for OUT endpoints */
    uint16_t  epSharedBufSize; /**< Buffer size */

    uint16_t  curBufAddr; /** Current position in endpoint buffer (HW or SRAM) */

    /** Stores endpoints information */
    cy_stc_usbfs_dev_drv_endpoint_data_t epPool[CY_USBFS_DEV_DRV_NUM_EPS_MAX];

    /** Pointer to the device context structure */
    void *devConext;
    /** \endcond */

} cy_stc_usbfs_dev_drv_context_t;
/** \} group_usbfs_dev_drv_data_structures */


/*******************************************************************************
*                              Function Prototypes
*******************************************************************************/

/**
* \addtogroup group_usbfs_dev_hal_functions_common
* \{
* The Initialization functions provide the API to begin USBFS driver operation 
* (configure and enable) and to stop operation (disable and de-initialize).
*/
cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_Init(USBFS_Type *base,
                                                   cy_stc_usbfs_dev_drv_config_t const *config,
                                                   cy_stc_usbfs_dev_drv_context_t      *context);

void Cy_USBFS_Dev_Drv_DeInit(USBFS_Type *base, 
                             cy_stc_usbfs_dev_drv_context_t *context);

void Cy_USBFS_Dev_Drv_Enable(USBFS_Type *base, 
                             cy_stc_usbfs_dev_drv_context_t const *context);

void Cy_USBFS_Dev_Drv_Disable(USBFS_Type *base, 
                              cy_stc_usbfs_dev_drv_context_t *context);

__STATIC_INLINE void     Cy_USBFS_Dev_Drv_SetDeviceAddress(USBFS_Type *base, uint8_t address);
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetDeviceAddress(USBFS_Type const *base);

__STATIC_INLINE void  Cy_USBFS_Dev_Drv_SetDevContext(USBFS_Type const *base,
                                                     void *devContext, 
                                                     cy_stc_usbfs_dev_drv_context_t *context);

__STATIC_INLINE void* Cy_USBFS_Dev_Drv_GetDevContext(USBFS_Type const *base,
                                                     cy_stc_usbfs_dev_drv_context_t *context);

void Cy_USBFS_Dev_Drv_ConfigDevice(USBFS_Type *base, 
                                     cy_stc_usbfs_dev_drv_context_t *context);

void Cy_USBFS_Dev_Drv_ConfigDeviceComplete(USBFS_Type *base,
                                           cy_stc_usbfs_dev_drv_context_t const *context);

void Cy_USBFS_Dev_Drv_UnConfigureDevice(USBFS_Type *base, 
                                        cy_stc_usbfs_dev_drv_context_t *context);
/** \} group_usbfs_dev_hal_functions_common */

/**
* \addtogroup group_usbfs_dev_hal_functions_ep0_service
* \{
* The Endpoint 0 Service functions provides API to establish communication with 
* the USB Host using control endpoint 0.
*/
void Cy_USBFS_Dev_Drv_Ep0GetSetup(USBFS_Type const *base,
                                  uint8_t    *buffer,
                                  cy_stc_usbfs_dev_drv_context_t const *context);

uint32_t Cy_USBFS_Dev_Drv_Ep0Write(USBFS_Type *base,
                                   uint8_t const *buffer,
                                   uint32_t    size,
                                   cy_stc_usbfs_dev_drv_context_t *context);

uint32_t Cy_USBFS_Dev_Drv_Ep0Read(USBFS_Type const *base,
                                  uint8_t    *buffer,
                                  uint32_t    size,
                                  cy_stc_usbfs_dev_drv_context_t *context);

__STATIC_INLINE void Cy_USBFS_Dev_Drv_Ep0Stall(USBFS_Type const *base, 
                                               cy_stc_usbfs_dev_drv_context_t *context);
/** \} group_usbfs_dev_hal_functions_ep0_service */


/**
* \addtogroup group_usbfs_dev_hal_functions_endpoint_config
* \{
* The Data Endpoint Configuration Functions provide the API to allocate and release 
* hardware resources and override the memcpy function for data endpoints.
*/
__STATIC_INLINE  cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_AddEndpoint(USBFS_Type *base,
                                                    cy_stc_usb_dev_ep_config_t const  *config,
                                                    cy_stc_usbfs_dev_drv_context_t    *context);

cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_RemoveEndpoint(USBFS_Type *base,
                                                             uint32_t    endpointAddr,
                                                             cy_stc_usbfs_dev_drv_context_t *context);

__STATIC_INLINE void Cy_USBFS_Dev_Drv_OverwriteMemcpy(USBFS_Type const *base, 
                                                      uint32_t    endpoint, 
                                                      cy_fn_usbfs_dev_drv_memcpy_ptr_t  memcpyFunc, 
                                                      cy_stc_usbfs_dev_drv_context_t *context);
/** \} group_usbfs_dev_hal_functions_endpoint_config */

/**
* \addtogroup group_usbfs_dev_hal_functions_data_xfer
* The Data Endpoint Transfer functions provide the API to establish 
* communication with the USB Host using data endpoint.
* \{
*/
__STATIC_INLINE cy_en_usb_dev_ep_state_t Cy_USBFS_Dev_Drv_GetEndpointState(USBFS_Type const *base,
                                                                           uint32_t    endpoint,
                                                                           cy_stc_usbfs_dev_drv_context_t const *context);

__STATIC_INLINE cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_LoadInEndpoint(USBFS_Type *base,
                                                             uint32_t      endpoint,
                                                             uint8_t const *buffer,
                                                             uint32_t      size,
                                                             cy_stc_usbfs_dev_drv_context_t *context);

void Cy_USBFS_Dev_Drv_EnableOutEndpoint(USBFS_Type *base,
                                        uint32_t    endpoint,
                                        cy_stc_usbfs_dev_drv_context_t *context);

__STATIC_INLINE cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_ReadOutEndpoint(USBFS_Type *base,
                                                              uint32_t endpoint,
                                                              uint8_t  *buffer,
                                                              uint32_t size,
                                                              uint32_t *actSize,
                                                              cy_stc_usbfs_dev_drv_context_t *context);

cy_en_usb_dev_ep_state_t Cy_USBFS_Dev_Drv_Abort(USBFS_Type *base,
                                                uint32_t   endpoint,
                                                cy_stc_usbfs_dev_drv_context_t const *context);

cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_AbortComplete(USBFS_Type *base,
                                                            uint32_t   endpoint,
                                                            cy_stc_usbfs_dev_drv_context_t *context);

__STATIC_INLINE bool     Cy_USBFS_Dev_Drv_GetEndpointAckState(USBFS_Type const *base, uint32_t endpoint);
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetEndpointCount   (USBFS_Type const *base, uint32_t endpoint);


__STATIC_INLINE cy_en_usb_dev_ep_state_t Cy_USBFS_Dev_Drv_GetEndpointStallState(USBFS_Type const *base,
                                                                               uint32_t    endpoint,
                                                                               cy_stc_usbfs_dev_drv_context_t const *context);

cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_StallEndpoint(USBFS_Type *base,
                                                            uint32_t    endpoint,
                                                            cy_stc_usbfs_dev_drv_context_t *context);

cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_UnStallEndpoint(USBFS_Type *base,
                                                              uint32_t    endpoint,
                                                              cy_stc_usbfs_dev_drv_context_t *context);

/** \} group_usbfs_dev_hal_functions_data_xfer */

/**
* \addtogroup group_usbfs_dev_drv_functions_interrupts
* The Functions Interrupt functions provide the API to register callbacks 
* for interrupt events provided by the USB block, interrupt handler, and configuration functions.
* \{
*/
void Cy_USBFS_Dev_Drv_Interrupt(USBFS_Type *base, uint32_t intrCause, cy_stc_usbfs_dev_drv_context_t *context);

__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetInterruptCauseHi (USBFS_Type const *base);
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetInterruptCauseMed(USBFS_Type const *base);
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetInterruptCauseLo (USBFS_Type const *base);

void Cy_USBFS_Dev_Drv_RegisterServiceCallback(USBFS_Type const *base,
                                              cy_en_usb_dev_service_cb_t     source,
                                              cy_cb_usbfs_dev_drv_callback_t callback,
                                              cy_stc_usbfs_dev_drv_context_t *context);

__STATIC_INLINE void Cy_USBFS_Dev_Drv_RegisterSofCallback(USBFS_Type *base, 
                                                          cy_cb_usbfs_dev_drv_callback_t  callback, 
                                                          cy_stc_usbfs_dev_drv_context_t *context);

__STATIC_INLINE void Cy_USBFS_Dev_Drv_RegisterLpmCallback(USBFS_Type *base, 
                                                          cy_cb_usbfs_dev_drv_callback_t  callback, 
                                                          cy_stc_usbfs_dev_drv_context_t *context);

__STATIC_INLINE void Cy_USBFS_Dev_Drv_RegisterEndpointCallback(USBFS_Type const *base, 
                                                               uint32_t endpoint, 
                                                               cy_cb_usbfs_dev_drv_ep_callback_t callback,
                                                               cy_stc_usbfs_dev_drv_context_t    *context);

__STATIC_INLINE     void Cy_USBFS_Dev_Drv_SetInterruptsLevel(USBFS_Type *base, uint32_t intrLevel);
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetInterruptsLevel(USBFS_Type const *base);

__STATIC_INLINE void Cy_USBFS_Dev_Drv_DisableEp0Interrupt(USBFS_Type *base);
__STATIC_INLINE void Cy_USBFS_Dev_Drv_EnableEp0Interrupt(USBFS_Type *base);
/** \} group_usbfs_dev_drv_functions_interrupts */

/**
* \addtogroup group_usbfs_dev_drv_functions_low_power 
* The Low Power functions provide the API to implement Low Power callback on application level.
* \{
*/
__STATIC_INLINE bool Cy_USBFS_Dev_Drv_CheckActivity(USBFS_Type *base);
__STATIC_INLINE void Cy_USBFS_Dev_Drv_Force        (USBFS_Type *base, cy_en_usbfs_dev_drv_force_bus_state_t state);
                void Cy_USBFS_Dev_Drv_Suspend(USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context);
                void Cy_USBFS_Dev_Drv_Resume (USBFS_Type *base, cy_stc_usbfs_dev_drv_context_t *context);
/** \} group_usbfs_dev_drv_functions_low_power */

/**
* \addtogroup group_usbfs_dev_drv_functions_lpm
* The LPM functions provide the API to use the LPM feature available in the USB block. 
* \{
*/
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_Lpm_GetBeslValue       (USBFS_Type const *base);
__STATIC_INLINE bool     Cy_USBFS_Dev_Drv_Lpm_RemoteWakeUpAllowed(USBFS_Type const *base);
__STATIC_INLINE void     Cy_USBFS_Dev_Drv_Lpm_SetResponse        (USBFS_Type *base, cy_en_usbfs_dev_drv_lpm_req_t response);
__STATIC_INLINE cy_en_usbfs_dev_drv_lpm_req_t Cy_USBFS_Dev_Drv_Lpm_GetResponse(USBFS_Type const *base);
/** \} group_usbfs_dev_drv_functions_lpm */


/*******************************************************************************
*                                 Driver Constants
*******************************************************************************/

/**
* \addtogroup group_usbfs_dev_drv_macros
* \{
*/
/** Allocates static buffer for data endpoint. The size parameter must be a constant. 
* The allocated buffer is aligned on a 2 byte boundary. <b>An odd buffer size is 
* converted to even consuming 1 extra byte. The application must discard this 
* extra byte</b>. This manipulation is needed to support different 8-bit and 16-bit 
* hardware buffer access types in the driver (See section 
* \ref group_usbfs_dev_drv_ep_management_buf_access for more information). 
*/
#define CY_USBFS_DEV_DRV_ALLOC_ENDPOINT_BUFFER(buf, size)  uint8_t buf[(0U != ((size) & 0x1U)) ? ((size) + 1U) : (size)] CY_ALIGN(2)
/** \} group_usbfs_dev_drv_macros */

/**
* \addtogroup group_usbfs_dev_drv_macros_intr_level
* \{
*/
/** Interrupt source is assigned to trigger High interrupt */
#define CY_USBFS_DEV_DRV_LVL_HIGH   (0U)
/** Interrupt source is assigned to trigger Medium interrupt */
#define CY_USBFS_DEV_DRV_LVL_MEDIUM (1U)
/** Interrupt source is assigned to trigger Low interrupt */
#define CY_USBFS_DEV_DRV_LVL_LOW    (2U)

/** Assigns SOF interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_SOF_LVL(level)       _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_SOF_LVL_SEL, level)
/** Assigns Bus Reset interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_BUS_RESET_LVL(level) _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_BUS_RESET_LVL_SEL, level)
/** Assigns Endpoint 0 interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_EP0_LVL(level)       _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_EP0_LVL_SEL, level)
/** Assigns LPM interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_LPM_LVL(level)       _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_LPM_LVL_SEL, level)
/** Assigns Resume interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_RESUME_LVL(level)    _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_RESUME_LVL_SEL, level)
/** Assigns Arbiter interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_ARB_EP_LVL(level)    _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_ARB_EP_LVL_SEL, level)
/** Assigns Endpoint 1 interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_EP1_LVL(level)       _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_EP1_LVL_SEL, level)
/** Assigns Endpoint 2 interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_EP2_LVL(level)       _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_EP2_LVL_SEL, level)
/** Assigns Endpoint 3 interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_EP3_LVL(level)       _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_EP3_LVL_SEL, level)
/** Assigns Endpoint 4 interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_EP4_LVL(level)       _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_EP4_LVL_SEL, level)
/** Assigns Endpoint 5 interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_EP5_LVL(level)       _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_EP5_LVL_SEL, level)
/** Assigns Endpoint 6 interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_EP6_LVL(level)       _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_EP6_LVL_SEL, level)
/** Assigns Endpoint 7 interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_EP7_LVL(level)       _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_EP7_LVL_SEL, level)
/** Assigns Endpoint 8 interrupt source to trigger interrupt Low, Medium, or High */
#define CY_USBFS_DEV_DRV_SET_EP8_LVL(level)       _VAL2FLD(USBFS_USBLPM_INTR_LVL_SEL_EP8_LVL_SEL, level)
/** \} group_usbfs_dev_drv_macros_intr_level */

/**
* \addtogroup group_usbfs_dev_drv_macros_intr_cause
* \{
*/
#define CY_USBFS_DEV_DRV_LPM_INTR       USBFS_USBLPM_INTR_CAUSE_HI_LPM_INTR_Msk         /**< Link Power Management request interrupt */
#define CY_USBFS_DEV_DRV_ARBITER_INTR   USBFS_USBLPM_INTR_CAUSE_HI_ARB_EP_INTR_Msk      /**< Arbiter interrupt */
#define CY_USBFS_DEV_DRV_EP0_INTR       USBFS_USBLPM_INTR_CAUSE_HI_EP0_INTR_Msk         /**< Endpoint 0 interrupt */
#define CY_USBFS_DEV_DRV_SOF_INTR       USBFS_USBLPM_INTR_CAUSE_HI_SOF_INTR_Msk         /**< SOF interrupt */
#define CY_USBFS_DEV_DRV_BUS_RESET_INTR USBFS_USBLPM_INTR_CAUSE_HI_BUS_RESET_INTR_Msk   /**< Bus Reset interrupt */
#define CY_USBFS_DEV_DRV_EP1_INTR       USBFS_USBLPM_INTR_CAUSE_HI_EP1_INTR_Msk         /**< Data endpoint 1 interrupt */
#define CY_USBFS_DEV_DRV_EP2_INTR       USBFS_USBLPM_INTR_CAUSE_HI_EP2_INTR_Msk         /**< Data endpoint 2 interrupt */
#define CY_USBFS_DEV_DRV_EP3_INTR       USBFS_USBLPM_INTR_CAUSE_HI_EP3_INTR_Msk         /**< Data endpoint 3 interrupt */
#define CY_USBFS_DEV_DRV_EP4_INTR       USBFS_USBLPM_INTR_CAUSE_HI_EP4_INTR_Msk         /**< Data endpoint 4 interrupt */
#define CY_USBFS_DEV_DRV_EP5_INTR       USBFS_USBLPM_INTR_CAUSE_HI_EP5_INTR_Msk         /**< Data endpoint 5 interrupt */
#define CY_USBFS_DEV_DRV_EP6_INTR       USBFS_USBLPM_INTR_CAUSE_HI_EP6_INTR_Msk         /**< Data endpoint 6 interrupt */
#define CY_USBFS_DEV_DRV_EP7_INTR       USBFS_USBLPM_INTR_CAUSE_HI_EP7_INTR_Msk         /**< Data endpoint 7 interrupt */
#define CY_USBFS_DEV_DRV_EP8_INTR       USBFS_USBLPM_INTR_CAUSE_HI_EP8_INTR_Msk         /**< Data endpoint 8 interrupt */
/** \} group_usbfs_dev_drv_macros_intr_cause */

/**
* \addtogroup group_usbfs_dev_drv_macros_ep_xfer_err
* \{
*/
/** 
* Error occurred during USB transfer. 
* For an IN transaction, this indicates a no response from HOST scenario. 
* For an OUT transaction, this represents a PID or CRC error or bit-stuff error 
* scenario.
*/
#define CY_USBFS_DEV_ENDPOINT_TRANSFER_ERROR    (0x1U)

/** 
* Data toggle bit remained the same.
* The received OUT packet has the same data toggle bit that the previous 
* packet had. This indicates that the Host retransmitted packet.
*/
#define CY_USBFS_DEV_ENDPOINT_SAME_DATA_TOGGLE  (0x2U)
/** \} group_usbfs_dev_drv_macros_ep_xfer_err */


/*******************************************************************************
*                              Internal Constants
*******************************************************************************/

/** \cond INTERNAL */
/* Start position of data endpoints SIE interrupt sources */
#define USBFS_USBLPM_INTR_CAUSE_LPM_INTR_Msk        USBFS_USBLPM_INTR_CAUSE_HI_LPM_INTR_Msk
#define USBFS_USBLPM_INTR_CAUSE_ARB_EP_INTR_Msk     USBFS_USBLPM_INTR_CAUSE_HI_ARB_EP_INTR_Msk
#define USBFS_USBLPM_INTR_CAUSE_EP0_INTR_Msk        USBFS_USBLPM_INTR_CAUSE_HI_EP0_INTR_Msk
#define USBFS_USBLPM_INTR_CAUSE_SOF_INTR_Msk        USBFS_USBLPM_INTR_CAUSE_HI_SOF_INTR_Msk
#define USBFS_USBLPM_INTR_CAUSE_BUS_RESET_INTR_Msk  USBFS_USBLPM_INTR_CAUSE_HI_BUS_RESET_INTR_Msk
#define USBFS_USBLPM_INTR_CAUSE_EP1_INTR_Pos        USBFS_USBLPM_INTR_CAUSE_HI_EP1_INTR_Pos

/* Validation macros */
#define CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint)  (((endpoint) > 0U) && ((endpoint) <= CY_USBFS_DEV_DRV_NUM_EPS_MAX))
#define CY_USBFS_DEV_DRV_EP2PHY(endpoint)       ((uint32_t) (endpoint) - 1U)
#define CY_USBFS_DEV_DRV_EP2MASK(endpont)       ((uint32_t) (0x1UL << endpoint))

#define CY_USBFS_DEV_DRV_EPADDR2EP(endpointAddr)        ((uint32_t) (endpointAddr) & 0x0FU)
#define CY_USBFS_DEV_DRV_IS_EP_DIR_IN(endpointAddr)     (0U != ((endpointAddr) & 0x80U))
#define CY_USBFS_DEV_DRV_IS_EP_DIR_OUT(endpointAddr)    (0U == ((endpointAddr) & 0x80U))
#define CY_USBFS_DEV_DRV_EPADDR2PHY(endpointAddr)       CY_USBFS_DEV_DRV_EP2PHY(CY_USBFS_DEV_DRV_EPADDR2EP(endpointAddr))

#define CY_USBFS_DEV_DRV_IS_MODE_VALID(mode)    (((mode) == CY_USBFS_DEV_DRV_EP_MANAGEMENT_CPU) || \
                                                 ((mode) == CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA) || \
                                                 ((mode) == CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO))
/** \endcond */
/** \} group_usbfs_drv_macros */


/*******************************************************************************
*                         In-line Function Implementation
*******************************************************************************/

/**
* \addtogroup group_usbfs_dev_hal_functions_common
* \{
*/
/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_SetDeviceAddress
****************************************************************************//**
*
* Sets the device address.
*
* \param base
* The pointer to the USBFS instance.
*
* \param address
* Device address.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_SetDeviceAddress(USBFS_Type *base, uint8_t address)
{
    base->USBDEV.CR0 = _CLR_SET_FLD32U(base->USBDEV.CR0, USBFS_USBDEV_CR0_DEVICE_ADDRESS, address);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_GetDeviceAddress
****************************************************************************//**
*
* Returns the device address.
*
* \param base
* The pointer to the USBFS instance.
*
* \return
* Device address.
* The device address is assigned by the Host during device enumeration. 
* Zero means that device address has not yet been assigned.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetDeviceAddress(USBFS_Type const *base)
{
    return _FLD2VAL(USBFS_USBDEV_CR0_DEVICE_ADDRESS, base->USBDEV.CR0);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_SetDevContext
****************************************************************************//**
*
* Stores pointer to the USB Device context in the driver context.
*
* \param base
* The pointer to the USBFS instance
*
* \param devContext
* The pointer to the USB Device context structure.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \note
* This function is intended to be used by USB Device middleware for operation.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_SetDevContext(USBFS_Type const *base,
                                                    void *devContext, 
                                                    cy_stc_usbfs_dev_drv_context_t *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    context->devConext = devContext;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_GetDevContext
****************************************************************************//**
*
* Returns pointer to the USB Device context.
*
* \param base
* The pointer to the USBFS instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* The pointer to the USB Device context.
*
* \note
* This function is intended to be used by USB Device middleware for operation.
*
*******************************************************************************/
__STATIC_INLINE void* Cy_USBFS_Dev_Drv_GetDevContext(USBFS_Type const *base,
                                                     cy_stc_usbfs_dev_drv_context_t *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    return (context->devConext);
}
/** \} group_usbfs_dev_hal_functions_common */


/**
* \addtogroup group_usbfs_dev_drv_functions_interrupts
* \{
*/
/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_RegisterSofCallback
****************************************************************************//**
*
* Registers a callback function to notify about an SOF event in 
* \ref Cy_USBFS_Dev_Drv_Interrupt. The SOF interrupt source is enabled after 
* registration. To remove callback function pass NULL as function pointer.
* When callback removed the interrupt source is disabled.
*
* \param base
* The pointer to the USBFS instance.
*
* \param callback
* The pointer to a callback function.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \note
* To remove the callback, pass NULL as the pointer to a callback function.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_RegisterSofCallback(USBFS_Type *base, 
                                                          cy_cb_usbfs_dev_drv_callback_t  callback, 
                                                          cy_stc_usbfs_dev_drv_context_t *context)
{
    uint32_t mask;
    
    context->cbSof = callback;
    
    /* Enable/Disable SOF interrupt */
    mask = Cy_USBFS_Dev_Drv_GetSieInterruptMask(base);
    
    if (NULL != callback)
    {
        mask |= CY_USBFS_DEV_DRV_INTR_SIE_SOF;
    }
    else
    {
        mask &= ~CY_USBFS_DEV_DRV_INTR_SIE_SOF;
    }
    
    Cy_USBFS_Dev_Drv_ClearSieInterrupt(base, CY_USBFS_DEV_DRV_INTR_SIE_SOF);
    Cy_USBFS_Dev_Drv_SetSieInterruptMask(base, mask);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_RegisterLpmCallback
****************************************************************************//**
*
* Registers a callback function to notify about an LPM event in 
* \ref Cy_USBFS_Dev_Drv_Interrupt. The LPM interrupt source is enabled after 
* registration. To remove the callback function, pass NULL as function pointer.
* When the callback has been removed, the interrupt source is disabled.
*
* \param base
* The pointer to the USBFS instance.
*
* \param callback
* The pointer to a callback function.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \note
* To remove the callback, pass NULL as the pointer to a callback function.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_RegisterLpmCallback(USBFS_Type *base, 
                                                          cy_cb_usbfs_dev_drv_callback_t  callback, 
                                                          cy_stc_usbfs_dev_drv_context_t *context)

{
    uint32_t mask;
    
    context->cbLpm = callback;
    
    /* Enable/Disable LPM interrupt source */
    mask = Cy_USBFS_Dev_Drv_GetSieInterruptMask(base);
    
    if (NULL != callback)
    {
        mask |= CY_USBFS_DEV_DRV_INTR_SIE_LPM;
    }
    else
    {
        mask &= ~CY_USBFS_DEV_DRV_INTR_SIE_LPM;
    }
    
    Cy_USBFS_Dev_Drv_ClearSieInterrupt(base, CY_USBFS_DEV_DRV_INTR_SIE_LPM);
    Cy_USBFS_Dev_Drv_SetSieInterruptMask(base, mask);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_RegisterEndpointCallback
****************************************************************************//**
*
* Registers a callback function to notify of an endpoint transfer completion 
* event in \ref Cy_USBFS_Dev_Drv_Interrupt. 
* * IN endpoint  - the Host read data from the endpoint and new data can be 
*                  loaded. 
* * OUT endpoint - the Host wrote data into the endpoint and the data is 
*                  ready to be read.
* To remove the callback function, pass NULL as function pointer.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The data endpoint number.
*
* \param callback
* The pointer to a callback function.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \note
* To remove the callback, pass NULL as the pointer to a callback function.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_RegisterEndpointCallback(USBFS_Type const *base, 
                                                               uint32_t endpoint, 
                                                               cy_cb_usbfs_dev_drv_ep_callback_t  callback, 
                                                               cy_stc_usbfs_dev_drv_context_t    *context)

{
    /* Suppress a compiler warning about unused variables */
    (void) base;
    
    CY_ASSERT_L1(CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint));
    
    endpoint = CY_USBFS_DEV_DRV_EP2PHY(endpoint);
    context->epPool[endpoint].epComplete = callback;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_GetInterruptCauseHi
****************************************************************************//**
*
* Returns the mask of bits showing the source of the current triggered
* interrupt. This is useful for modes of operation where an interrupt can
* be generated by conditions in multiple interrupt source registers.
*
* \param base
* The pointer to the USBFS instance.
*
* \return
* The mask with the OR of the following conditions that have been triggered.
* See \ref group_usbfs_dev_drv_macros_intr_cause for the set of constants.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetInterruptCauseHi(USBFS_Type const *base)
{
    return USBFS_DEV_LPM_INTR_CAUSE_HI(base);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_GetInterruptCauseMed
****************************************************************************//**
*
* Returns the mask of bits showing the source of the current triggered
* interrupt. This is useful for modes of operation where an interrupt can
* be generated by conditions in multiple interrupt source registers.
*
* \param base
* The pointer to the USBFS instance.
*
* \return
* The mask with the OR of the following conditions that have been triggered.
* See \ref group_usbfs_dev_drv_macros_intr_cause for the set of constants.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetInterruptCauseMed(USBFS_Type const *base)
{
    return USBFS_DEV_LPM_INTR_CAUSE_MED(base);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_GetInterruptCauseLo
****************************************************************************//**
*
* Returns the mask of bits showing the source of the current triggered
* interrupt. This is useful for modes of operation where an interrupt can
* be generated by conditions in multiple interrupt source registers.
*
* \param base
* The pointer to the USBFS instance.
*
* \return
* The mask with the OR of the following conditions that have been triggered.
* See \ref group_usbfs_dev_drv_macros_intr_cause for the set of constants.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetInterruptCauseLo(USBFS_Type const *base)
{
    return USBFS_DEV_LPM_INTR_CAUSE_LO(base);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_SetInterruptsLevel
****************************************************************************//**
*
* Writes INTR_LVL_SEL register which contains groups for all interrupt sources.
*
* \param base
* The pointer to the USBFS instance.
*
* \param intrLevel
* INTR_LVL_SEL register value.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_SetInterruptsLevel(USBFS_Type *base, uint32_t intrLevel)
{
    USBFS_DEV_LPM_INTR_LVL_SEL(base) = intrLevel;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_GetInterruptsLevel
****************************************************************************//**
*
* Returns INTR_LVL_SEL register that contains groups for all interrupt sources.
*
* \param base
* The pointer to the USBFS instance.
*
* \return
* Returns INTR_LVL_SEL register that contains groups for all interrupt sources.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetInterruptsLevel(USBFS_Type const *base)
{
    return base->USBLPM.INTR_LVL_SEL;
}



/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_DisableEp0Interrupt
****************************************************************************//**
*
* Enables Control Endpoint 0 interrupt source.
*
* \param base
* The pointer to the USBFS instance.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_DisableEp0Interrupt(USBFS_Type *base)
{
    uint32_t mask = (Cy_USBFS_Dev_Drv_GetSieInterruptMask(base) & ~CY_USBFS_DEV_DRV_INTR_SIE_EP0);
    Cy_USBFS_Dev_Drv_SetSieInterruptMask(base, mask);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_EnableEp0Interrupt
****************************************************************************//**
*
* Enables Control Endpoint 0 interrupt.
*
* \param base
* The pointer to the USBFS instance source.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_EnableEp0Interrupt(USBFS_Type *base)
{
    uint32_t mask = (Cy_USBFS_Dev_Drv_GetSieInterruptMask(base) | CY_USBFS_DEV_DRV_INTR_SIE_EP0);
    Cy_USBFS_Dev_Drv_SetSieInterruptMask(base, mask);
}
/** \} group_usbfs_dev_drv_functions_interrupts */

/**
* \addtogroup group_usbfs_dev_drv_functions_low_power
* \{
*/
/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_CheckActivity
****************************************************************************//**
*
* Returns the activity status of the bus. 
* It clears the hardware status to provide updated status on the next call of 
* this function. This function is useful to determine whether there is any USB bus 
* activity between function calls. The typical use case is to determine whether 
* the USB suspend conditions are met.
*
* \param base
* The pointer to the USBFS instance.
*
* \return
* Bus activity since last call.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_USBFS_Dev_Drv_CheckActivity(USBFS_Type *base)
{
    uint32_t tmpReg = base->USBDEV.CR1;

    /* Clear hardware status */
    base->USBDEV.CR1 &= (tmpReg & ~USBFS_USBDEV_CR1_BUS_ACTIVITY_Msk);
    (void) base->USBDEV.CR1;

    return (0U != (tmpReg & USBFS_USBDEV_CR1_BUS_ACTIVITY_Msk));
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Force
****************************************************************************//**
*
* Forces a USB J, K, or SE0 state on the USB lines. 
* The typical use case is to signal a Remote Wakeup condition on the USB bus.
*
* \param base
* The pointer to the USBFS instance.
*
* \param state
* Desired bus state.
* See \ref cy_en_usbfs_dev_drv_force_bus_state_t for the set of constants.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_Force(USBFS_Type *base, cy_en_usbfs_dev_drv_force_bus_state_t state)
{
    base->USBDEV.USBIO_CR0 = (uint32_t) state;
    (void) base->USBDEV.USBIO_CR0;
}
/** \} group_usbfs_dev_drv_functions_low_power */


/**
* \addtogroup group_usbfs_dev_drv_functions_lpm
* \{
*/
/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Lpm_GetBeslValue
****************************************************************************//**
*
* Returns the Best Effort Service Latency (BESL) value sent by the host as 
* part of the LPM token transaction.
*
* \param base
* The pointer to the USBFS instance.
*
* \return
* BESL value (4-bits)
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_Lpm_GetBeslValue(USBFS_Type const *base)
{
    return _FLD2VAL(USBFS_USBLPM_LPM_STAT_LPM_BESL, USBFS_DEV_LPM_LPM_STAT(base));
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Lpm_RemoteWakeUpAllowed
****************************************************************************//**
*
* Returns the remote wakeup permission set by the Host as part of the 
* LPM token transaction.
*
* \param base
* The pointer to the USBFS instance.
*
* \return
* Remote wakeup permission: true - allowed, false - not allowed.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_USBFS_Dev_Drv_Lpm_RemoteWakeUpAllowed(USBFS_Type const *base)
{
    return _FLD2BOOL(USBFS_USBLPM_LPM_STAT_LPM_REMOTEWAKE, USBFS_DEV_LPM_LPM_STAT(base));
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Lpm_SetResponse
****************************************************************************//**
*
* Configures the response in the handshake packet that the device sends when 
* an LPM token packet is received.
*
* \param base
* The pointer to the USBFS instance.
*
* \param response
* The response to return for an LPM token packet.
* See \ref cy_en_usbfs_dev_drv_lpm_req_t for the set of options.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_Lpm_SetResponse(USBFS_Type *base, cy_en_usbfs_dev_drv_lpm_req_t response)
{
    USBFS_DEV_LPM_LPM_CTL(base) = _CLR_SET_FLD32U(USBFS_DEV_LPM_LPM_CTL(base), 
                                                  USBFS_USBLPM_LPM_CTL_LPM_RESP, ((uint32_t) response));
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Lpm_GetResponse
****************************************************************************//**
*
* Returns the response value that the device sends as part of the handshake 
* packet when an LPM token packet is received.
*
* \param base
* The pointer to the USBFS instance.
*
* \return
* The response to return for an LPM token packet.
* See \ref cy_en_usbfs_dev_drv_lpm_req_t for the set of options.
*
*******************************************************************************/
__STATIC_INLINE cy_en_usbfs_dev_drv_lpm_req_t Cy_USBFS_Dev_Drv_Lpm_GetResponse(USBFS_Type const *base)
{
    uint32_t retValue = _FLD2VAL(USBFS_USBLPM_LPM_CTL_LPM_RESP, USBFS_DEV_LPM_LPM_CTL(base));
    return (cy_en_usbfs_dev_drv_lpm_req_t) retValue;
}
/** \} group_usbfs_dev_drv_functions_lpm */


/**
* \addtogroup group_usbfs_dev_hal_functions_endpoint_config
* \{
*/
/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_AddEndpoint
****************************************************************************//**
*
* Configures data endpoint for the following operation (allocates hardware 
* resources for data endpoint).
*
* \param base
* The pointer to the USBFS instance.
*
* \param config
* The pointer to data endpoint configuration \ref cy_stc_usb_dev_ep_config_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Status code of the function execution \ref cy_en_usbfs_dev_drv_status_t.
*
*******************************************************************************/
__STATIC_INLINE cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_AddEndpoint(USBFS_Type *base,
                                                      cy_stc_usb_dev_ep_config_t  const *config,
                                                      cy_stc_usbfs_dev_drv_context_t      *context)
{
    return context->addEndpoint(base, config, context);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_OverwriteMemcpy
****************************************************************************//**
*
* Overwrites the memory copy (memcpy) function used to copy data with the user 
* implemented:
* * \ref Cy_USBFS_Dev_Drv_ReadOutEndpoint copies data from from the internal 
*   buffer to the application buffer for OUT endpoint.
* * \ref Cy_USBFS_Dev_Drv_LoadInEndpoint copies data from the application buffer 
*   for IN endpoint to the the internal buffer.
* Only applicable when endpoint management mode is 
* \ref CY_USBFS_DEV_DRV_EP_MANAGEMENT_DMA_AUTO.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint 
* The data endpoint number.
*
* \param memcpyFunc
* The pointer to the function that copies data.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_OverwriteMemcpy(USBFS_Type const *base, 
                                                      uint32_t   endpoint, 
                                cy_fn_usbfs_dev_drv_memcpy_ptr_t memcpyFunc, 
                                cy_stc_usbfs_dev_drv_context_t   *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;
    
    CY_ASSERT_L1(CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint));
    
    endpoint = CY_USBFS_DEV_DRV_EP2PHY(endpoint);
    context->epPool[endpoint].copyData = memcpyFunc;
}
/** \} group_usbfs_dev_hal_functions_endpoint_config */


/**
* \addtogroup group_usbfs_dev_hal_functions_data_xfer
* \{
*/
/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_GetEndpointState
****************************************************************************//**
*
* Returns the state of the endpoint.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The data endpoint number.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Data endpoint state \ref cy_en_usb_dev_ep_state_t.
*
*******************************************************************************/
__STATIC_INLINE cy_en_usb_dev_ep_state_t Cy_USBFS_Dev_Drv_GetEndpointState(
                                                             USBFS_Type const *base,
                                                             uint32_t    endpoint,
                                                             cy_stc_usbfs_dev_drv_context_t const *context)

{
    /* Suppress a compiler warning about unused variables */
    (void) base;
    
    CY_ASSERT_L1(CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint));

    return context->epPool[CY_USBFS_DEV_DRV_EP2PHY(endpoint)].state;
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_LoadInEndpoint
****************************************************************************//**
*
* Loads data into the IN endpoint buffer. After data loads, the 
* endpoint is ready to be read by the host.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The IN data endpoint number.
*
* \param buffer
* The pointer to the buffer containing data bytes to load.
*
* \param size
* The number of bytes to load into the endpoint.
* This value must be less than or equal to endpoint maximum packet size.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Status code of the function execution \ref cy_en_usbfs_dev_drv_status_t.
*
*******************************************************************************/
__STATIC_INLINE cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_LoadInEndpoint(
                                                         USBFS_Type    *base,
                                                         uint32_t      endpoint,
                                                         uint8_t const *buffer,
                                                         uint32_t      size,
                                                         cy_stc_usbfs_dev_drv_context_t *context)
{
    CY_ASSERT_L1(CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint));
    CY_ASSERT_L1(CY_USBFS_DEV_DRV_IS_EP_DIR_IN(context->epPool[CY_USBFS_DEV_DRV_EP2PHY(endpoint)].address));

    return context->loadInEndpoint(base, CY_USBFS_DEV_DRV_EP2PHY(endpoint), buffer, size, context);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_ReadOutEndpoint
****************************************************************************//**
*
* Reads data from the OUT endpoint buffer. 
* Before executing the next read, the \ref Cy_USBFS_Dev_Drv_EnableOutEndpoint must be 
* called to allow the Host to write data into the endpoint.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The OUT data endpoint number.
*
* \param buffer
* Pointer to the buffer that stores data that was read.
*
* \param size
* The number of bytes to read from endpoint.
* This value must be less than or equal to endpoint maximum packet size.
*
* \param actSize
* The number of bytes which were actually read.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Status code of the function execution \ref cy_en_usbfs_dev_drv_status_t.
*
*******************************************************************************/
__STATIC_INLINE cy_en_usbfs_dev_drv_status_t Cy_USBFS_Dev_Drv_ReadOutEndpoint(
                                                          USBFS_Type *base,
                                                          uint32_t   endpoint,
                                                          uint8_t    *buffer,
                                                          uint32_t   size,
                                                          uint32_t   *actSize,
                                                          cy_stc_usbfs_dev_drv_context_t *context)
{
    CY_ASSERT_L1(CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint));
    CY_ASSERT_L1(CY_USBFS_DEV_DRV_IS_EP_DIR_OUT(context->epPool[CY_USBFS_DEV_DRV_EP2PHY(endpoint)].address));

    return context->readOutEndpoint(base, CY_USBFS_DEV_DRV_EP2PHY(endpoint), buffer, size, actSize, context);
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_GetEndpointAckState
****************************************************************************//**
*
* Returns whether the transaction completed with ACK for a certain endpoint.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The data endpoint number.
*
* \return
* ACK state: true - transaction completed with ACK, false - otherwise.
*
*******************************************************************************/
__STATIC_INLINE bool Cy_USBFS_Dev_Drv_GetEndpointAckState(USBFS_Type const *base, uint32_t endpoint)
{
    CY_ASSERT_L1(CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint));
    
    endpoint = CY_USBFS_DEV_DRV_EP2PHY(endpoint);
    return _FLD2BOOL(USBFS_USBDEV_SIE_EP1_CR0_ACKED_TXN, USBFS_DEV_SIE_EP_CR0(base, endpoint));
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_GetEndpointCount
****************************************************************************//**
*
* Return number of data bytes in the transaction for a certain endpoint. 
* Before calling this function ensure that the Host wrote data into the 
* endpoint. The returned value is updated after the Host access to the 
* endpoint but remains unchanged after data has been read from the endpoint 
* buffer.
* The typical use case is to read number of bytes that the Host wrote into the 
* OUT endpoint. 
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The data endpoint number.
*
* \return
* The number of data bytes in the transaction.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_USBFS_Dev_Drv_GetEndpointCount(USBFS_Type const *base, uint32_t endpoint)
{
    CY_ASSERT_L1(CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint));

    return Cy_USBFS_Dev_Drv_GetSieEpCount(base, CY_USBFS_DEV_DRV_EP2PHY(endpoint));
}


/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_GetEndpointStallState
****************************************************************************//**
*
* This function returns the state of a certain endpoint.
*
* \param base
* The pointer to the USBFS instance.
*
* \param endpoint
* The data endpoint number.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
* \return
* Endpoint state \ref cy_en_usb_dev_ep_state_t.
*
*******************************************************************************/
__STATIC_INLINE cy_en_usb_dev_ep_state_t Cy_USBFS_Dev_Drv_GetEndpointStallState(
                                                              USBFS_Type const *base,
                                                              uint32_t    endpoint,
                                                              cy_stc_usbfs_dev_drv_context_t const *context)
{    
    if (CY_USBFS_DEV_DRV_IS_EP_VALID(endpoint))
    {
        return Cy_USBFS_Dev_Drv_GetEndpointState(base, endpoint, context);   
    }
    else
    {
        return CY_USB_DEV_EP_INVALID;
    }
}
/** \} group_usbfs_dev_hal_functions_data_xfer */


/**
* \addtogroup group_usbfs_dev_hal_functions_ep0_service
* \{
*/
/*******************************************************************************
* Function Name: Cy_USBFS_Dev_Drv_Ep0Stall
****************************************************************************//**
*
* Stalls endpoint 0.
*
* \param base
* The pointer to the USBFS instance.
*
* \param context
* The pointer to the context structure \ref cy_stc_usbfs_dev_drv_context_t
* allocated by the user. The structure is used during the USBFS Device 
* operation for internal configuration and data retention. The user must not 
* modify anything in this structure.
*
*******************************************************************************/
__STATIC_INLINE void Cy_USBFS_Dev_Drv_Ep0Stall(USBFS_Type const *base, cy_stc_usbfs_dev_drv_context_t *context)
{
    /* Suppress a compiler warning about unused variables */
    (void) base;

    context->ep0ModeReg = CY_USBFS_DEV_DRV_EP_CR_STALL_INOUT;
}
/** \} group_usbfs_dev_hal_functions_ep0_service */

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXUSBFS */

#endif /* (CY_USBFS_DEV_DRV_H) */


/* [] END OF FILE */
