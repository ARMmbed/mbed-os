/***************************************************************************//**
* \file cy_ble_ds.h
* \version 1.0
*
* \brief
*  Contains the documentation data.
*
********************************************************************************
* \copyright
* Copyright 2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
 * \mainpage Cypress BLE Middleware Library
 *
 * The Bluetooth Low Energy (BLE) middleware contains a comprehensive API list to allow you to configure the BLE stack and the
 * underlying chip hardware.
 *
 * The BLE middleware incorporates a Bluetooth Core Specification v5.0 compliant protocol stack.
 * You may access the GAP, GATT and L2CAP layers of the stack using provided API.
 *
 * The API are broadly categorized as follows:
 * - \ref group_ble_common_api
 * - \ref group_ble_service_api
 * - \ref group_ble_clock_api_section
 *
 * <b>Features:</b>
 *  - Multi-link support up to four simultaneous connections in any combination of roles
 *  - Bluetooth v5.0 compliant protocol stack
 *      - LE 2 Mbps
 *  - Generic Access Profile (GAP) Features
 *      - Broadcaster, Observer, Peripheral and Central roles
 *      - User-defined advertising data
 *      - Bonding support for up to four devices
 *      - Security modes 1 and 2
 *  - Generic Attribute Profile (GATT) Features
 *      - GATT Client and Server
 *      - 16-, 32-, and 128-bit UUIDs
 *  - Special Interest Group (SIG) adopted GATT-based Profiles and Services, and quick prototype of new profile design
 *    through intuitive GUI Custom Profile development; Support of Bluetooth Developer Studio Profile format
 *  - Security Manager features
 *      - Pairing methods: Just works, Passkey Entry, Out of Band, Numeric Comparison
 *      - Authenticated man-in-the-middle (MITM) protection and data signing
 *  - Logical Link Adaption Protocol (L2CAP) Connection Oriented Channel
 *  - Link Layer (LL) Features
 *      - Master and Slave role
 *      - 128-bit AES encryption
 *      - Low Duty Cycle Advertising
 *      - LE Ping
 * 
 *
 * \section group_ble_section_more_information More Information
 * Refer to the BLE_PDL component datasheet.
 *
 *
 * \section group_ble_MISRA MISRA-C Compliance
 * <table class="doxtable">
 * <tr>
 *   <th>MISRA rule</th>
 *   <th>Rule Class (Required/ Advisory)</th>
 *   <th>Rule Description</th>
 *   <th>Description of Deviation(s)</th>
 * </tr>
 * <tr>
 *   <td>1.2</td>
 *   <td>R</td>
 *   <td>No reliance shall be placed on undefined or unspecified behaviour.</td>
 *   <td>This specific behavior is explicitly covered in rule 5.1.</td>
 * </tr>
 * <tr>
 *   <td>5.1</td>
 *   <td>R</td>
 *   <td>Identifiers (internal and external) shall not rely on the significance of more than 31 characters.</td>
 *   <td>This rule applies to ISO:C90 standard. PDL conforms to ISO:C99 that does not require this limitation.</td>
 * </tr>
 * <tr>
 *   <td>9.3</td>
 *   <td>R</td>
 *   <td>In an enumerator list, the '=' construct shall not be used to explicitly initialise members other than the
 *       first, unless all items are explicitly initialised.</td>
 *   <td>There are enumerator lists which depend on configurations (e.g. cy_en_ble_srvi_t) and require to 
 *       use the '=' construct for calculating the instance count for the multi-instances services, 
 *       such as HIDS, BAS or CUSTOM</td>
 * </tr>
 * <tr>
 *   <td>10.1</td>
 *   <td>R</td>
 *   <td>The value of an expression of integer type shall not be implicitly converted to a different underlying type
 *       under some circumstances.</td>
 *   <td>An operand of essentially enum type is being converted to unsigned type as a result of an arithmetic or
 *       conditional operation. The conversion does not have any unintended effect.</td>
 * </tr>
 * <tr>
 *   <td>11.4</td>
 *   <td>A</td>
 *   <td>A cast should not be performed between a pointer to object type and a different pointer to object type.</td>
 *   <td>A cast involving pointers is conducted with caution that the pointers are correctly aligned for the type of
 *       object being pointed to.</td>
 * </tr>
 * <tr>
 *   <td>11.5</td>
 *   <td>A</td>
 *   <td>A cast shall not be performed that removes any const or volatile qualification from the type addressed by a
 *       pointer.</td>
 *   <td>The const or volatile qualification is lost during pointer cast before passing to the stack functions.</td>
 * </tr>
 * <tr>
 *   <td>13.7</td>
 *   <td>R</td>
 *   <td>Boolean operations whose results are invariant shall not be permitted.</td>
 *   <td>A Boolean operator can yield a result that can be proven to be always "true" or always "false" in some specific
 *       configurations because of generalized implementation approach.</td>
 * </tr>
 * <tr>
 *   <td>16.7</td>
 *   <td>A</td>
 *   <td>The object addressed by the pointer parameter is not modified and so the pointer could be of type 'pointer to const'.</td>
 *   <td>The 'base' and 'content' parameters in Cy_BLE_DeepSleepCallback function are not used by BLE but callback type is universal for all drivers.</td>
 * </tr>
 * <tr>
 *   <td>18.4</td>
 *   <td>R</td>
 *   <td>Unions shall not be used.</td>
 *   <td>The use of deviations is acceptable for packing and unpacking of data, for example when sending and 
 *       receiving messages, and implementing variant records provided that the variants are differentiated 
 *       by a common field. </td>
 * </tr>
 * </table>
 *
 *
 * \section group_ble_changelog Changelog
 * <table class="doxtable">
 * <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
 * <tr>
 *  <td>1.0</td>
 *  <td>Initial version.</td>
 *  <td></td>
 * </tr>
 * </table>
 *
 *
 *
 *
 *
 *
 * \defgroup group_ble_common_api BLE Common API
 * \ingroup group_ble
 *
 * \brief
 * The common API act as a general interface between the BLE application
 * and the BLE stack module. The application may use these API to control
 * the underlying hardware such as radio power, data encryption and device
 * bonding via the stack. It may also access the GAP, GATT and L2CAP layers
 * of the stack. These are divided into the following categories:
 * - \ref group_ble_common_api_functions
 * - \ref group_ble_common_api_gap_functions_section
 * - \ref group_ble_common_api_gatt_functions_section
 * - \ref group_ble_common_api_l2cap_functions
 *
 * These API also use API specific definitions and data structures. These are classified
 * in the following subset:
 * - \ref group_ble_common_api_definitions
 *
 *
 * \defgroup group_ble_common_api_functions BLE Common Core Functions
 * \ingroup group_ble_common_api
 * \brief
 * The common core API are used for general BLE component configuration.
 * These include initialization, power management, and utilities.
 *
 *
 * \defgroup group_ble_common_api_gap_functions_section GAP Functions
 * \ingroup group_ble_common_api
 * \brief
 * The GAP APIs allow access to the Generic Access Profile (GAP) layer 
 * of the BLE stack. Depending on the chosen GAP role in the GUI, you may
 * use a subset of the supported APIs.
 * 
 * The GAP API names begin with Cy_BLE_GAP_. In addition to this, the APIs
 * also append the GAP role initial letter in the API name.
 * 
 * \defgroup group_ble_common_api_gap_functions GAP Central and Peripheral Functions
 * \ingroup group_ble_common_api_gap_functions_section
 * \brief
 * These are APIs common to both GAP Central role and GAP Peripheral role. 
 * You may use them in either roles.
 * 
 * No letter is appended to the API name: Cy_BLE_GAP_
 * 
 * \defgroup group_ble_common_api_gap_central_functions GAP Central Functions
 * \ingroup group_ble_common_api_gap_functions_section
 * \brief
 * APIs unique to designs configured as a GAP Central role. 
 * 
 * A letter 'C' is appended to the API name: Cy_BLE_GAPC_
 * 
 * \defgroup group_ble_common_api_gap_peripheral_functions GAP Peripheral Functions
 * \ingroup group_ble_common_api_gap_functions_section
 * \brief
 * APIs unique to designs configured as a GAP Peripheral role. 
 * 
 * A letter 'P' is appended to the API name: Cy_BLE_GAPP_
 * 
 * 
 * \defgroup group_ble_common_api_gatt_functions_section GATT Functions
 * \ingroup group_ble_common_api
 * \brief
 * The GATT APIs allow access to the Generic Attribute Profile (GATT) layer 
 * of the BLE stack. Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported APIs.
 * 
 * The GATT API names begin with CyBle_Gatt. In addition to this, the APIs
 * also append the GATT role initial letter in the API name.
 * 
 * \defgroup group_ble_common_api_gatt_functions GATT Client and Server Functions
 * \ingroup group_ble_common_api_gatt_functions_section
 * \brief
 * These are APIs common to both GATT Client role and GATT Server role. 
 * You may use them in either roles.
 * 
 * No letter is appended to the API name: Cy_BLE_GATT_
 * 
 * \defgroup group_ble_common_api_gatt_client_functions GATT Client Functions
 * \ingroup group_ble_common_api_gatt_functions_section
 * \brief
 * APIs unique to designs configured as a GATT Client role. 
 * 
 * A letter 'C' is appended to the API name: Cy_BLE_GATTC_
 * 
 * \defgroup group_ble_common_api_gatt_server_functions GATT Server Functions
 * \ingroup group_ble_common_api_gatt_functions_section
 * \brief
 * APIs unique to designs configured as a GATT Server role. 
 * 
 * A letter 'S' is appended to the API name: Cy_BLE_GATTS_
 * 
 * 
 * \defgroup group_ble_common_api_l2cap_functions L2CAP Functions
 * \ingroup group_ble_common_api
 * \brief
 * The L2CAP APIs allow access to the Logical link control and adaptation
 * protocol (L2CAP) layer of the BLE stack.
 * 
 * The L2CAP API names begin with Cy_BLE_L2CAP.
 * 
 * 
 *  
 * \defgroup group_ble_common_api_events BLE Common Events
 * \ingroup group_ble_common_api
 * \brief
 * The BLE stack generates events to notify the application on various status
 * alerts concerning the stack. These can be generic stack events or can be 
 * specific to GAP, GATT or L2CAP layers. The service specific events are
 * handled separately in \ref group_ble_service_api_events.
 *  
 *
 * \defgroup group_ble_common_api_definitions_section BLE Common Definitions and Data Structures
 * \ingroup group_ble_common_api
 * \brief
 * Contains definitions and structures that are common to all BLE common API.
 * Note that some of these are also used in Service-specific API.
 *
 * \defgroup group_ble_common_macros_error BLE Error Code
 * \ingroup group_ble_common_api_definitions_section
 * \brief
 * Contains definitions for all the spec defined error code in Core Spec 5.0, Vol2, Part D
 * 
 * Related Document:
 *  BLE Standard Spec - CoreV5.0
 *
 * \defgroup group_ble_common_api_macros_section Macros
 * \ingroup group_ble_common_api_definitions_section
 *
 * \defgroup group_ble_common_api_macros Common
 * \ingroup group_ble_common_api_macros_section
 *  BLE Common macros
 *
 * \defgroup group_ble_common_api_macros_gatt_db BLE GATT Database
 * \ingroup group_ble_common_api_macros_section
 * \brief
 *  BLE GATT Database macros
 *
 * \defgroup group_ble_common_api_macros_gatt_uuid_services BLE Services UUID 
 * \ingroup group_ble_common_api_macros_section
 * \brief
 *   BLE Services Universal Unique Identifier (UUID) macros
 *
 * \defgroup group_ble_common_api_macros_gatt_uuid_char_gatt_type BLE GATT Attribute Types UUID 
 * \ingroup group_ble_common_api_macros_section
 * \brief
 *   BLE GATT Attribute Types defined by GATT Profile UUID macros
 *
 * \defgroup group_ble_common_api_macros_gatt_uuid_char_desc BLE GATT Characteristic Descriptors UUID 
 * \ingroup group_ble_common_api_macros_section
 * \brief
 *   BLE GATT Attribute Types defined by GATT Profile UUID macros
 *
 * \defgroup group_ble_common_api_macros_gatt_uuid_char_type BLE GATT Characteristic Types UUID 
 * \ingroup group_ble_common_api_macros_section
 * \brief
 *   BLE GATT Characteristic Types UUID macros
 *
 * \defgroup group_ble_common_api_macros_appearance_values BLE Appearance values
 * \ingroup group_ble_common_api_macros_section
 * \brief
 *   BLE Appearance values macros
 *
 * \defgroup group_ble_common_api_data_stuct_section Data Structures
 * \ingroup group_ble_common_api_definitions_section
 *
 * \defgroup group_ble_common_api_definitions Common 
 * \ingroup group_ble_common_api_data_stuct_section
 * \brief
 * Contains the common definitions and data structures used in the BLE.
 *
 * \defgroup group_ble_common_api_gap_definitions GAP 
 * \ingroup group_ble_common_api_data_stuct_section
 * \brief
 * Contains the GAP specific definitions and data structures used in the
 * GAP APIs.
 *
 * \defgroup group_ble_common_api_gatt_definitions GATT
 * \ingroup group_ble_common_api_data_stuct_section
 * \brief
 * Contains the GATT specific definitions and data structures used in the
 * GATT APIs.
 *
 * \defgroup group_ble_common_api_l2cap_definitions L2CAP 
 * \ingroup group_ble_common_api_data_stuct_section
 * \brief
 * Contains the L2CAP specific definitions and data structures used in the
 * L2CAP APIs.
 *
 *
 * \defgroup group_ble_service_api BLE Service-Specific API
 * \ingroup group_ble
 * \brief
 * This section describes BLE Service-specific API. The Service
 * API are only included in the design if the Service is
 * added to the selected Profile in the component GUI.
 * These are interfaces for the BLE application to use during BLE
 * connectivity. The service-specific API internally use the BLE
 * Stack API to achieve the Service use case.
 *
 * Refer to the <a href="https://www.bluetooth.org/en-us/specification/adopted-specifications">Special Interest Group
 * Web Site</a>
 * for links to the latest specifications and other documentation.
 *
 * Many of the API will generate Service-specific events. The events
 * are also used in the Service-specific callback functions. These are
 * documented in:
 * - \ref group_ble_service_api_events
 *
 *
 * \defgroup group_ble_service_api_events BLE Service-Specific Events
 * \ingroup group_ble_service_api
 * \brief
 * The BLE stack generates service-specific events to notify the application
 * that a service-specific status change needs attention.
 *
 *
 *
 * \defgroup group_ble_service_api_ANCS Apple Notification Center Service (ANCS)
 * \ingroup group_ble_service_api
 * \brief
 * The Apple Notification Center Service provides iOS notifications from Apple devices for accessories.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The ANCS API names begin with Cy_BLE_ANCS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_ANCS_server_client ANCS Server and Client Function
 * \ingroup group_ble_service_api_ANCS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_ANCS_
 *
 * \defgroup group_ble_service_api_ANCS_server ANCS Server Functions
 * \ingroup group_ble_service_api_ANCS
 * \brief
 * API unique to ANCS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_ANCSS_
 *
 * \defgroup group_ble_service_api_ANCS_client ANCS Client Functions
 * \ingroup group_ble_service_api_ANCS
 * \brief
 * API unique to ANCS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_ANCSC_
 *
 * \defgroup group_ble_service_api_ANCS_definitions ANCS Definitions and Data Structures
 * \ingroup group_ble_service_api_ANCS
 * \brief
 * Contains the ANCS specific definitions and data structures used
 * in the ANCS API.
 *
 *
 *
 * \defgroup group_ble_service_api_ANS Alert Notification Service (ANS)
 * \ingroup group_ble_service_api
 * \brief
 * The Alert Notification Service exposes alert information in a device.
 *
 * This information includes:
 * - Type of alert occurring in a device
 * - Additional text information such as the caller's ID or sender's ID
 * - Count of new alerts
 * - Count of unread alert items
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The ANS API names begin with Cy_BLE_ANS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_ANS_server_client ANS Server and Client Function
 * \ingroup group_ble_service_api_ANS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_ANS_
 *
 * \defgroup group_ble_service_api_ANS_server ANS Server Functions
 * \ingroup group_ble_service_api_ANS
 * \brief
 * API unique to ANS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_ANSS_
 *
 * \defgroup group_ble_service_api_ANS_client ANS Client Functions
 * \ingroup group_ble_service_api_ANS
 * \brief
 * API unique to ANS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_ANSC_
 *
 * \defgroup group_ble_service_api_ANS_definitions ANS Definitions and Data Structures
 * \ingroup group_ble_service_api_ANS
 * \brief
 * Contains the ANS specific definitions and data structures used
 * in the ANS API.
 *
 *
 *
 * \defgroup group_ble_service_api_AIOS Automation IO Service (AIOS)
 * \ingroup group_ble_service_api
 * \brief
 * The Automation IO Service enables a device to connect and interact with an Automation IO Module (IOM) in order to access digital and analog signals.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The AIOS API names begin with Cy_BLE_AIOS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_AIOS_server_client AIOS Server and Client Function
 * \ingroup group_ble_service_api_AIOS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_AIOS_
 *
 * \defgroup group_ble_service_api_AIOS_server AIOS Server Functions
 * \ingroup group_ble_service_api_AIOS
 * \brief
 * API unique to AIOS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_AIOSS_
 *
 * \defgroup group_ble_service_api_AIOS_client AIOS Client Functions
 * \ingroup group_ble_service_api_AIOS
 * \brief
 * API unique to AIOS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_AIOSC_
 *
 * \defgroup group_ble_service_api_AIOS_definitions AIOS Definitions and Data Structures
 * \ingroup group_ble_service_api_AIOS
 * \brief
 * Contains the AIOS specific definitions and data structures used
 * in the AIOS API.
 *
 *
 * \defgroup group_ble_service_api_BAS Battery Service (BAS)
 * \ingroup group_ble_service_api
 * \brief
 * The Battery Service exposes the battery level of a single battery
 * or set of batteries in a device.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The BAS API names begin with Cy_BLE_BAS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_BAS_server_client BAS Server and Client Function
 * \ingroup group_ble_service_api_BAS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_BAS_
 *
 * \defgroup group_ble_service_api_BAS_server BAS Server Functions
 * \ingroup group_ble_service_api_BAS
 * \brief
 * API unique to BAS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_BASS_
 *
 * \defgroup group_ble_service_api_BAS_client BAS Client Functions
 * \ingroup group_ble_service_api_BAS
 * \brief
 * API unique to BAS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_BASC_
 *
 * \defgroup group_ble_service_api_BAS_definitions BAS Definitions and Data Structures
 * \ingroup group_ble_service_api_BAS
 * \brief
 * Contains the BAS specific definitions and data structures used
 * in the BAS API.
 *
 *
 *
 * \defgroup group_ble_service_api_BCS Body Composition Service (BCS)
 * \ingroup group_ble_service_api
 * \brief
 * The Body Composition Service exposes data related to body composition from a body composition analyzer (Server)
 * intended for consumer healthcare as well as sports/fitness applications.
 *
 * Depending on the chosen GATT role in the GUI, you may use a subset of the supported API.
 *
 * The BCS API names begin with Cy_BLE_BCS_. In addition to this, the API also append the GATT role initial letter in the
 * API name.
 *
 * \defgroup group_ble_service_api_BCS_server_client BCS Server and Client Function
 * \ingroup group_ble_service_api_BCS
 * \brief
 * These are API common to both GATT Client role and GATT Server role. You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_BCS_
 *
 * \defgroup group_ble_service_api_BCS_server BCS Server Functions
 * \ingroup group_ble_service_api_BCS
 * \brief
 * API unique to BCS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_BCSS_
 *
 * \defgroup group_ble_service_api_BCS_client BCS Client Functions
 * \ingroup group_ble_service_api_BCS
 * \brief
 * API unique to BCS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_BCSC_
 *
 * \defgroup group_ble_service_api_BCS_definitions BCS Definitions and Data Structures
 * \ingroup group_ble_service_api_BCS
 * \brief
 * Contains the BCS specific definitions and data structures used in the BCS API.
 *
 *
 *
 * \defgroup group_ble_service_api_BLS Blood Pressure Service (BLS)
 * \ingroup group_ble_service_api
 * \brief
 * The Blood Pressure Service exposes blood pressure and other data related
 * to a non-invasive blood pressure monitor for consumer and professional
 * healthcare applications.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The BLS API names begin with Cy_BLE_BLS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_BLS_server_client BLS Server and Client Function
 * \ingroup group_ble_service_api_BLS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_BLS_
 *
 * \defgroup group_ble_service_api_BLS_server BLS Server Functions
 * \ingroup group_ble_service_api_BLS
 * \brief
 * API unique to BLS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_BLSS_
 *
 * \defgroup group_ble_service_api_BLS_client BLS Client Functions
 * \ingroup group_ble_service_api_BLS
 * \brief
 * API unique to BLS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_BLSC_
 *
 * \defgroup group_ble_service_api_BLS_definitions BLS Definitions and Data Structures
 * \ingroup group_ble_service_api_BLS
 * \brief
 * Contains the BLS specific definitions and data structures
 * used in the BLS API.
 *
 *
 *
 * \defgroup group_ble_service_api_BMS Bond Management Service (BMS)
 * \ingroup group_ble_service_api
 * \brief
 * The Bond Management Service defines how a peer Bluetooth device can manage the storage of bond information, especially
 * the deletion of it, on the Bluetooth device supporting this service.
 *
 * Depending on the chosen GATT role in the GUI, you may use a subset of the supported API.
 *
 * The BMS API names begin with Cy_BLE_BMS_. In addition to this, the API also append the GATT role initial letter in the
 * API name.
 *
 * \defgroup group_ble_service_api_BMS_server_client BMS Server and Client Function
 * \ingroup group_ble_service_api_BMS
 * \brief
 * These are API common to both GATT Client role and GATT Server role. You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_BMS_
 *
 * \defgroup group_ble_service_api_BMS_server BMS Server Functions
 * \ingroup group_ble_service_api_BMS
 * \brief
 * API unique to BMS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_BMSS_
 *
 * \defgroup group_ble_service_api_BMS_client BMS Client Functions
 * \ingroup group_ble_service_api_BMS
 * \brief
 * API unique to BMS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_BMSC_
 *
 * \defgroup group_ble_service_api_BMS_definitions BMS Definitions and Data Structures
 * \ingroup group_ble_service_api_BMS
 * \brief
 * Contains the BMS specific definitions and data structures used in the BMS API.
 *
 *
 *
 * \defgroup group_ble_service_api_CGMS Continuous Glucose Monitoring Service (CGMS)
 * \ingroup group_ble_service_api
 * \brief
 * The Continuous Glucose Monitoring Service exposes glucose measurement and other data related to a personal CGM sensor
 * for healthcare applications.
 *
 * Depending on the chosen GATT role in the GUI, you may use a subset of the supported API.
 *
 * The CGMS API names begin with Cy_BLE_CGMS_. In addition to this, the API also append the GATT role initial letter in the
 * API name.
 *
 * \defgroup group_ble_service_api_CGMS_server_client CGMS Server and Client Function
 * \ingroup group_ble_service_api_CGMS
 * \brief
 * These are API common to both GATT Client role and GATT Server role. You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_CGMS_
 *
 * \defgroup group_ble_service_api_CGMS_server CGMS Server Functions
 * \ingroup group_ble_service_api_CGMS
 * \brief
 * API unique to CGMS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_CGMSS_
 *
 * \defgroup group_ble_service_api_CGMS_client CGMS Client Functions
 * \ingroup group_ble_service_api_CGMS
 * \brief
 * API unique to CGMS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_CGMSC_
 *
 * \defgroup group_ble_service_api_CGMS_definitions CGMS Definitions and Data Structures
 * \ingroup group_ble_service_api_CGMS
 * \brief
 * Contains the CGMS specific definitions and data structures used in the CGMS API.
 *
 *
 *
 * \defgroup group_ble_service_api_CPS Cycling Power Service (CPS)
 * \ingroup group_ble_service_api
 * \brief
 * The Cycling Power Service (CPS) exposes power- and force-related
 * data and optionally speed- and cadence-related data from a Cycling
 * Power sensor (GATT Server) intended for sports and fitness applications.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The CPS API names begin with Cy_BLE_CPS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 *
 * \defgroup group_ble_service_api_CPS_server_client CPS Server and Client Function
 * \ingroup group_ble_service_api_CPS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_CPS_
 *
 * \defgroup group_ble_service_api_CPS_server CPS Server Functions
 * \ingroup group_ble_service_api_CPS
 * \brief
 * API unique to CPS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_CPSS_
 *
 * \defgroup group_ble_service_api_CPS_client CPS Client Functions
 * \ingroup group_ble_service_api_CPS
 * \brief
 * API unique to CPS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_CPSC_
 *
 * \defgroup group_ble_service_api_CPS_definitions CPS Definitions and Data Structures
 * \ingroup group_ble_service_api_CPS
 * \brief
 * Contains the CPS specific definitions and data structures
 * used in the CPS API.
 *
 *
 * \defgroup group_ble_service_api_CSCS Cycling Speed and Cadence Service (CSCS)
 * \ingroup group_ble_service_api
 * \brief
 * The Cycling Speed and Cadence (CSC) Service exposes speed-related
 * data and/or cadence-related data while using the Cycling Speed
 * and Cadence sensor (Server).
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The CSCS API names begin with Cy_BLE_CSCS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 *
 * \defgroup group_ble_service_api_CSCS_server_client CSCS Server and Client Function
 * \ingroup group_ble_service_api_CSCS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_CSCS_
 *
 * \defgroup group_ble_service_api_CSCS_server CSCS Server Functions
 * \ingroup group_ble_service_api_CSCS
 * \brief
 * API unique to CSCS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_CSCSS_
 *
 * \defgroup group_ble_service_api_CSCS_client CSCS Client Functions
 * \ingroup group_ble_service_api_CSCS
 * \brief
 * API unique to CSCS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_CSCSC_
 *
 * \defgroup group_ble_service_api_CSCS_definitions CSCS Definitions and Data Structures
 * \ingroup group_ble_service_api_CSCS
 * \brief
 * Contains the CSCS specific definitions and data structures
 * used in the CSCS API.
 *
 *
 * \defgroup group_ble_service_api_CTS Current Time Service (CTS)
 * \ingroup group_ble_service_api
 * \brief
 * The Current Time Service defines how a Bluetooth device can expose time information
 * to other Bluetooth devices.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The CTS API names begin with Cy_BLE_CTS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_CTS_server_client CTS Server and Client Function
 * \ingroup group_ble_service_api_CTS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_CTS_
 *
 * \defgroup group_ble_service_api_CTS_server CTS Server Functions
 * \ingroup group_ble_service_api_CTS
 * \brief
 * API unique to CTS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_CTSS_
 *
 * \defgroup group_ble_service_api_CTS_client CTS Client Functions
 * \ingroup group_ble_service_api_CTS
 * \brief
 * API unique to CTS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_CTSC_
 *
 * \defgroup group_ble_service_api_CTS_definitions CTS Definitions and Data Structures
 * \ingroup group_ble_service_api_CTS
 * \brief
 * Contains the CTS specific definitions and data structures
 * used in the CTS API.
 *
 *
 * \defgroup group_ble_service_api_DIS Device Information Service (DIS)
 * \ingroup group_ble_service_api
 * \brief
 * The Device Information Service exposes manufacturer and/or
 * vendor information about a device.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The DIS API names begin with Cy_BLE_DIS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_DIS_server_client DIS Server and Client Function
 * \ingroup group_ble_service_api_DIS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_DIS_
 *
 * \defgroup group_ble_service_api_DIS_server DIS Server Functions
 * \ingroup group_ble_service_api_DIS
 * \brief
 * API unique to DIS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_DISS_
 *
 * \defgroup group_ble_service_api_DIS_client DIS Client Functions
 * \ingroup group_ble_service_api_DIS
 * \brief
 * API unique to DIS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_DISC_
 *
 * \defgroup group_ble_service_api_DIS_definitions DIS Definitions and Data Structures
 * \ingroup group_ble_service_api_DIS
 * \brief
 * Contains the DIS specific definitions and data structures
 * used in the DIS API.
 *
 *
 *
 * \defgroup group_ble_service_api_ESS Environmental Sensing Service (ESS)
 * \ingroup group_ble_service_api
 * \brief
 * The Environmental Sensing Service exposes measurement data from an environmental sensor intended for sports and fitness
 * applications.
 *
 * Depending on the chosen GATT role in the GUI, you may use a subset of the supported API.
 *
 * The ESS API names begin with Cy_BLE_ESS_. In addition to this, the API also append the GATT role initial letter in the
 * API name.
 *
 * \defgroup group_ble_service_api_ESS_server_client ESS Server and Client Function
 * \ingroup group_ble_service_api_ESS
 * \brief
 * These are API common to both GATT Client role and GATT Server role. You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_ESS_
 *
 * \defgroup group_ble_service_api_ESS_server ESS Server Functions
 * \ingroup group_ble_service_api_ESS
 * \brief
 * API unique to ESS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_ESSS_
 *
 * \defgroup group_ble_service_api_ESS_client ESS Client Functions
 * \ingroup group_ble_service_api_ESS
 * \brief
 * API unique to ESS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_ESSC_
 *
 * \defgroup group_ble_service_api_ESS_definitions ESS Definitions and Data Structures
 * \ingroup group_ble_service_api_ESS
 * \brief
 * Contains the ESS specific definitions and data structures used in the ESS API.
 *
 *
 *
 * \defgroup group_ble_service_api_GLS Glucose Service (GLS)
 * \ingroup group_ble_service_api
 * \brief
 * The Glucose Service exposes glucose and other data related to
 * a personal glucose sensor for consumer healthcare applications
 * and is not designed for clinical use.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The GLS API names begin with Cy_BLE_GLS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_GLS_server_client GLS Server and Client Function
 * \ingroup group_ble_service_api_GLS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_GLS_
 *
 * \defgroup group_ble_service_api_GLS_server GLS Server Functions
 * \ingroup group_ble_service_api_GLS
 * \brief
 * API unique to GLS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_GLSS_
 *
 * \defgroup group_ble_service_api_GLS_client GLS Client Functions
 * \ingroup group_ble_service_api_GLS
 * \brief
 * API unique to GLS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_GLSC_
 *
 * \defgroup group_ble_service_api_GLS_definitions GLS Definitions and Data Structures
 * \ingroup group_ble_service_api_GLS
 * \brief
 * Contains the GLS specific definitions and data structures
 * used in the GLS API.
 *
 *
 *
 * \defgroup group_ble_service_api_HIDS HID Service (HIDS)
 * \ingroup group_ble_service_api
 * \brief
 * The HID Service exposes data and associated formatting for
 * HID Devices and HID Hosts.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The HID API names begin with Cy_BLE_HID. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_HIDS_server_client HIDS Server and Client Functions
 * \ingroup group_ble_service_api_HIDS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_HIDS_
 *
 * \defgroup group_ble_service_api_HIDS_server HIDS Server Functions
 * \ingroup group_ble_service_api_HIDS
 * \brief
 * API unique to HID designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_HIDSS_
 *
 * \defgroup group_ble_service_api_HIDS_client HIDS Client Functions
 * \ingroup group_ble_service_api_HIDS
 * \brief
 * API unique to HID designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_HIDSC_
 *
 * \defgroup group_ble_service_api_HIDS_definitions HIDS Definitions and Data Structures
 * \ingroup group_ble_service_api_HIDS
 * \brief
 * Contains the HID specific definitions and data structures
 * used in the HID API.
 *
 *
 *
 * \defgroup group_ble_service_api_HRS Heart Rate Service (HRS)
 * \ingroup group_ble_service_api
 * \brief
 * The Heart Rate Service exposes heart rate and other data
 * related to a heart rate sensor intended for fitness applications.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The HRS API names begin with Cy_BLE_HRS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_HRS_server_client HRS Server and Client Function
 * \ingroup group_ble_service_api_HRS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_HRS_
 *
 * \defgroup group_ble_service_api_HRS_server HRS Server Functions
 * \ingroup group_ble_service_api_HRS
 * \brief
 * API unique to HRS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_HRSS_
 *
 * \defgroup group_ble_service_api_HRS_client HRS Client Functions
 * \ingroup group_ble_service_api_HRS
 * \brief
 * API unique to HRS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_HRSC_
 *
 * \defgroup group_ble_service_api_HRS_definitions HRS Definitions and Data Structures
 * \ingroup group_ble_service_api_HRS
 * \brief
 * Contains the HRS specific definitions and data structures
 * used in the HRS API.
 *
 *
 *
 * \defgroup group_ble_service_api_HPS HTTP Proxy Service (HPS)
 * \ingroup group_ble_service_api
 * \brief
 * The HTTP Proxy Service allows a Client device, typically a sensor,
 * to communicate with a Web Server through a gateway device.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The HPS API names begin with Cy_BLE_HPS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_HPS_server_client HPS Server and Client Function
 * \ingroup group_ble_service_api_HPS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_HPS_
 *
 * \defgroup group_ble_service_api_HPS_server HPS Server Functions
 * \ingroup group_ble_service_api_HPS
 * \brief
 * API unique to HPS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_HPSS_
 *
 * \defgroup group_ble_service_api_HPS_client HPS Client Functions
 * \ingroup group_ble_service_api_HPS
 * \brief
 * API unique to HPS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_HPSC_
 *
 * \defgroup group_ble_service_api_HPS_definitions HPS Definitions and Data Structures
 * \ingroup group_ble_service_api_HPS
 * \brief
 * Contains the HPS specific definitions and data structures
 * used in the HPS API.
 *
 *
 *
 * \defgroup group_ble_service_api_HTS Health Thermometer Service (HTS)
 * \ingroup group_ble_service_api
 * \brief
 * The Health Thermometer Service exposes temperature and other
 * data related to a thermometer used for healthcare applications.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The HTS API names begin with Cy_BLE_HTS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_HTS_server_client HTS Server and Client Function
 * \ingroup group_ble_service_api_HTS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_HTS_
 *
 * \defgroup group_ble_service_api_HTS_server HTS Server Functions
 * \ingroup group_ble_service_api_HTS
 * \brief
 * API unique to HTS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_HTSS_
 *
 * \defgroup group_ble_service_api_HTS_client HTS Client Functions
 * \ingroup group_ble_service_api_HTS
 * \brief
 * API unique to HTS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_HTSC_
 *
 * \defgroup group_ble_service_api_HTS_definitions HTS Definitions and Data Structures
 * \ingroup group_ble_service_api_HTS
 * \brief
 * Contains the HTS specific definitions and data structures
 * used in the HTS API.
 *
 *
 *
 * \defgroup group_ble_service_api_IAS Immediate Alert Service (IAS)
 * \ingroup group_ble_service_api
 * \brief
 * The Immediate Alert Service exposes a control point to allow a peer device to cause the device to immediately alert.
 *
 * The Immediate Alert Service uses the Alert Level Characteristic
 * to cause an alert when it is written with a value other than "No Alert".
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The IAS API names begin with Cy_BLE_IAS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_IAS_server_client IAS Server and Client Function
 * \ingroup group_ble_service_api_IAS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_IAS_
 *
 * \defgroup group_ble_service_api_IAS_server IAS Server Functions
 * \ingroup group_ble_service_api_IAS
 * \brief
 * API unique to IAS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_IASS_
 *
 * \defgroup group_ble_service_api_IAS_client IAS Client Functions
 * \ingroup group_ble_service_api_IAS
 * \brief
 * API unique to IAS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_IASC_
 *
 * \defgroup group_ble_service_api_IAS_definitions IAS Definitions and Data Structures
 * \ingroup group_ble_service_api_IAS
 * \brief
 * Contains the IAS specific definitions and data structures
 * used in the IAS API.
 *
 *
 *
 * \defgroup group_ble_service_api_IPS Indoor Positioning Service (IPS)
 * \ingroup group_ble_service_api
 * \brief
 * The Indoor Positioning exposes coordinates and other location related information via
 * an advertisement or indicates that the device address can be used for location look-up,
 * enabling mobile devices to find their position.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The IPS API names begin with Cy_BLE_IPS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_IPS_server_client IPS Server and Client Function
 * \ingroup group_ble_service_api_IPS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_IPS_
 *
 * \defgroup group_ble_service_api_IPS_server IPS Server Functions
 * \ingroup group_ble_service_api_IPS
 * \brief
 * API unique to IPS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_IPSS_
 *
 * \defgroup group_ble_service_api_IPS_client IPS Client Functions
 * \ingroup group_ble_service_api_IPS
 * \brief
 * API unique to IPS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_IPSC_
 *
 * \defgroup group_ble_service_api_IPS_definitions IPS Definitions and Data Structures
 * \ingroup group_ble_service_api_IPS
 * \brief
 * Contains the IPS specific definitions and data structures
 * used in the IPS API.
 *
 *
 *
 * \defgroup group_ble_service_api_LLS Link Loss Service (LLS)
 * \ingroup group_ble_service_api
 * \brief
 * The Link Loss Service uses the Alert Level Characteristic to
 * cause an alert in the device when the link is lost.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The LLS API names begin with Cy_BLE_LLS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_LLS_server_client LLS Server and Client Function
 * \ingroup group_ble_service_api_LLS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_LLS_
 *
 * \defgroup group_ble_service_api_LLS_server LLS Server Functions
 * \ingroup group_ble_service_api_LLS
 * \brief
 * API unique to LLS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_LLSS_
 *
 * \defgroup group_ble_service_api_LLS_client LLS Client Functions
 * \ingroup group_ble_service_api_LLS
 * \brief
 * API unique to LLS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_LLSC_
 *
 * \defgroup group_ble_service_api_LLS_definitions LLS Definitions and Data Structures
 * \ingroup group_ble_service_api_LLS
 * \brief
 * Contains the LLS specific definitions and data structures
 * used in the LLS API.
 *
 *
 *
 * \defgroup group_ble_service_api_LNS Location and Navigation Service (LNS)
 * \ingroup group_ble_service_api
 * \brief
 * The Location and Navigation Service exposes location and
 * navigation-related data from a Location and Navigation sensor
 * (Server) intended for outdoor activity applications.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The LNS API names begin with Cy_BLE_LNS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_LNS_server_client LNS Server and Client Function
 * \ingroup group_ble_service_api_LNS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_LNS_
 *
 * \defgroup group_ble_service_api_LNS_server LNS Server Functions
 * \ingroup group_ble_service_api_LNS
 * \brief
 * API unique to LNS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_LNSS_
 *
 * \defgroup group_ble_service_api_LNS_client LNS Client Functions
 * \ingroup group_ble_service_api_LNS
 * \brief
 * API unique to LNS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_LNSC_
 *
 * \defgroup group_ble_service_api_LNS_definitions LNS Definitions and Data Structures
 * \ingroup group_ble_service_api_LNS
 * \brief
 * Contains the LNS specific definitions and data structures
 * used in the LNS API.
 *
 *
 *
 * \defgroup group_ble_service_api_NDCS Next DST Change Service (NDCS)
 * \ingroup group_ble_service_api
 * \brief
 * The Next DST Change Service enables a BLE device that has knowledge about the
 * next occurrence of a DST change to expose this information to
 * another Bluetooth device. The Service uses the "Time with DST"
 * Characteristic and the functions exposed in this Service are
 * used to interact with that Characteristic.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The NDCS API names begin with Cy_BLE_NDCS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_NDCS_server_client NDCS Server and Client Functions
 * \ingroup group_ble_service_api_NDCS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_NDCS_
 *
 * \defgroup group_ble_service_api_NDCS_server NDCS Server Functions
 * \ingroup group_ble_service_api_NDCS
 * \brief
 * API unique to NDCS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_NDCSS_
 *
 * \defgroup group_ble_service_api_NDCS_client NDCS Client Functions
 * \ingroup group_ble_service_api_NDCS
 * \brief
 * API unique to NDCS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_NDCSC_
 *
 * \defgroup group_ble_service_api_NDCS_definitions NDCS Definitions and Data Structures
 * \ingroup group_ble_service_api_NDCS
 * \brief
 * Contains the NDCS specific definitions and data structures
 * used in the NDCS API.
 *
 *
 *
 * \defgroup group_ble_service_api_PASS Phone Alert Status Service (PASS)
 * \ingroup group_ble_service_api
 * \brief
 * The Phone Alert Status Service uses the Alert Status Characteristic
 * and Ringer Setting Characteristic to expose the phone alert status
 * and uses the Ringer Control Point Characteristic to control the
 * phone's ringer into mute or enable.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The PASS API names begin with Cy_BLE_PASS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_PASS_server_client PASS Server and Client Function
 * \ingroup group_ble_service_api_PASS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_PASS_
 *
 * \defgroup group_ble_service_api_PASS_server PASS Server Functions
 * \ingroup group_ble_service_api_PASS
 * \brief
 * API unique to PASS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_PASSS_
 *
 * \defgroup group_ble_service_api_PASS_client PASS Client Functions
 * \ingroup group_ble_service_api_PASS
 * \brief
 * API unique to PASS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_PASSC_
 *
 * \defgroup group_ble_service_api_PASS_definitions PASS Definitions and Data Structures
 * \ingroup group_ble_service_api_PASS
 * \brief
 * Contains the PASS specific definitions and data structures
 * used in the PASS API.
 *
 *
 *
 * \defgroup group_ble_service_api_PLXS Pulse Oximeter Service (PLXS)
 * \ingroup group_ble_service_api
 * \brief
 * The Pulse Oximeter Service enables a Collector device to connect and interact with a pulse oximeter intended for healthcare applications.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The PLXS API names begin with Cy_BLE_PLXS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_PLXS_server_client PLXS Server and Client Function
 * \ingroup group_ble_service_api_PLXS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_PLXS_
 *
 * \defgroup group_ble_service_api_PLXS_server PLXS Server Functions
 * \ingroup group_ble_service_api_PLXS
 * \brief
 * API unique to PLXS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_PLXSS_
 *
 * \defgroup group_ble_service_api_PLXS_client PLXS Client Functions
 * \ingroup group_ble_service_api_PLXS
 * \brief
 * API unique to PLXS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_PLXSC_
 *
 * \defgroup group_ble_service_api_PLXS_definitions PLXS Definitions and Data Structures
 * \ingroup group_ble_service_api_PLXS
 * \brief
 * Contains the PLXS specific definitions and data structures used
 * in the PLXS API.
 *
 *
 *
 * \defgroup group_ble_service_api_RSCS Running Speed and Cadence Service (RSCS)
 * \ingroup group_ble_service_api
 * \brief
 * The Running Speed and Cadence (RSC) Service exposes speed,
 * cadence and other data related to fitness applications such
 * as the stride length and the total distance the user has
 * travelled while using the Running Speed and Cadence sensor (Server).
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The RSCS API names begin with Cy_BLE_RSCS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_RSCS_server_client RSCS Server and Client Functions
 * \ingroup group_ble_service_api_RSCS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_RSCS_
 *
 * \defgroup group_ble_service_api_RSCS_server RSCS Server Functions
 * \ingroup group_ble_service_api_RSCS
 * \brief
 * API unique to RSCS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_RSCSS_
 *
 * \defgroup group_ble_service_api_RSCS_client RSCS Client Functions
 * \ingroup group_ble_service_api_RSCS
 * \brief
 * API unique to RSCS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_RSCSC_
 *
 * \defgroup group_ble_service_api_RSCS_definitions RSCS Definitions and Data Structures
 * \ingroup group_ble_service_api_RSCS
 * \brief
 * Contains the RSCS specific definitions and data structures
 * used in the RSCS API.
 *
 *
 *
 * \defgroup group_ble_service_api_RTUS Reference Time Update Service (RTUS)
 * \ingroup group_ble_service_api
 * \brief
 * The Reference Time Update Service enables a Bluetooth device that can update the
 * system time using the reference time such as a GPS receiver
 * to expose a control point and expose the accuracy (drift) of
 * the local system time compared to the reference time source.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The RTUS API names begin with Cy_BLE_RTUS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_RTUS_server_client RTUS Server and Client Function
 * \ingroup group_ble_service_api_RTUS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_RTUS_
 *
 * \defgroup group_ble_service_api_RTUS_server RTUS Server Functions
 * \ingroup group_ble_service_api_RTUS
 * \brief
 * API unique to RTUS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_RTUSS_
 *
 * \defgroup group_ble_service_api_RTUS_client RTUS Client Functions
 * \ingroup group_ble_service_api_RTUS
 * \brief
 * API unique to RTUS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_RTUSC_
 *
 * \defgroup group_ble_service_api_RTUS_definitions RTUS Definitions and Data Structures
 * \ingroup group_ble_service_api_RTUS
 * \brief
 * Contains the RTUS specific definitions and data structures
 * used in the RTUS API.
 *
 *
 *
 * \defgroup group_ble_service_api_SCPS Scan Parameters Service (ScPS)
 * \ingroup group_ble_service_api
 * \brief
 * The Scan Parameters Service enables a Server device to expose
 * a Characteristic for the GATT Client to write its scan interval
 * and scan window on the Server device, and enables a Server to
 * request a refresh of the GATT Client scan interval and scan window.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The ScPS API names begin with Cy_BLE_SCPS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_SCPS_server_client ScPS Server and Client Functions
 * \ingroup group_ble_service_api_SCPS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_SCPS_
 *
 * \defgroup group_ble_service_api_SCPS_server ScPS Server Functions
 * \ingroup group_ble_service_api_SCPS
 * \brief
 * API unique to ScPS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_SCPSS_
 *
 * \defgroup group_ble_service_api_SCPS_client ScPS Client Functions
 * \ingroup group_ble_service_api_SCPS
 * \brief
 * API unique to ScPS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_SCPSC_
 *
 * \defgroup group_ble_service_api_SCPS_definitions ScPS Definitions and Data Structures
 * \ingroup group_ble_service_api_SCPS
 * \brief
 * Contains the ScPS specific definitions and data structures
 * used in the ScPS API.
 *
 *
 *
 * \defgroup group_ble_service_api_TPS TX Power Service (TPS)
 * \ingroup group_ble_service_api
 * \brief
 * The Tx Power Service uses the Tx Power Level Characteristic
 * to expose the current transmit power level of a device when
 * in a connection.
 *
 * Depending on the chosen GATT role in the GUI, you may
 * use a subset of the supported API.
 *
 * The TPS API names begin with Cy_BLE_TPS_. In addition to this, the API
 * also append the GATT role initial letter in the API name.
 *
 * \defgroup group_ble_service_api_TPS_server_client TPS Server and Client Function
 * \ingroup group_ble_service_api_TPS
 * \brief
 * These are API common to both GATT Client role and GATT Server role.
 * You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_TPS_
 *
 * \defgroup group_ble_service_api_TPS_server TPS Server Functions
 * \ingroup group_ble_service_api_TPS
 * \brief
 * API unique to TPS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_TPSS_
 *
 * \defgroup group_ble_service_api_TPS_client TPS Client Functions
 * \ingroup group_ble_service_api_TPS
 * \brief
 * API unique to TPS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_TPSC_
 *
 * \defgroup group_ble_service_api_TPS_definitions TPS Definitions and Data Structures
 * \ingroup group_ble_service_api_TPS
 * \brief
 * Contains the TPS specific definitions and data structures
 * used in the TPS API.
 *
 *
 *
 * \defgroup group_ble_service_api_UDS User Data Service (UDS)
 * \ingroup group_ble_service_api
 * \brief
 * The User Data Service exposes user-related data in the sports and fitness environment. This allows remote access and
 * update of user data by a Client as well as the synchronization of user data between a Server and a Client.
 *
 * Depending on the chosen GATT role in the GUI, you may use a subset of the supported API.
 *
 * The UDS API names begin with Cy_BLE_UDS_. In addition to this, the API also append the GATT role initial letter in the
 * API name.
 *
 * \defgroup group_ble_service_api_UDS_server_client UDS Server and Client Function
 * \ingroup group_ble_service_api_UDS
 * \brief
 * These are API common to both GATT Client role and GATT Server role. You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_UDS_
 *
 * \defgroup group_ble_service_api_UDS_server UDS Server Functions
 * \ingroup group_ble_service_api_UDS
 * \brief
 * API unique to UDS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_UDSS_
 *
 * \defgroup group_ble_service_api_UDS_client UDS Client Functions
 * \ingroup group_ble_service_api_UDS
 * \brief
 * API unique to UDS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_UDSC_
 *
 * \defgroup group_ble_service_api_UDS_definitions UDS Definitions and Data Structures
 * \ingroup group_ble_service_api_UDS
 * \brief
 * Contains the UDS specific definitions and data structures used in the UDS API.
 *
 *
 *
 *
 * \defgroup group_ble_service_api_WPTS Wireless Power Transfer Service (WPTS)
 * \ingroup group_ble_service_api
 * \brief
 * The Wireless Power Transfer Service enables communication between Power Receiver Unit and Power Transmitter Unit
 * in the Wireless Power Transfer systems.
 *
 * Depending on the chosen GATT role in the GUI, you may use a subset of the supported API.
 *
 * The WPTS API names begin with Cy_BLE_WPTS_. In addition to this, the API also append the GATT role initial letter in the
 * API name.
 *
 * \defgroup group_ble_service_api_WPTS_server_client WPTS Server and Client Function
 * \ingroup group_ble_service_api_WPTS
 * \brief
 * These are API common to both GATT Client role and GATT Server role. You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_WPTS_
 *
 * \defgroup group_ble_service_api_WPTS_server WPTS Server Functions
 * \ingroup group_ble_service_api_WPTS
 * \brief
 * API unique to WPTS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_WPTSS_
 *
 * \defgroup group_ble_service_api_WPTS_client WPTS Client Functions
 * \ingroup group_ble_service_api_WPTS
 * \brief
 * API unique to WPTS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_WPTSC_
 *
 * \defgroup group_ble_service_api_WPTS_definitions WPTS Definitions and Data Structures
 * \ingroup group_ble_service_api_WPTS
 * \brief
 * Contains the WPTS specific definitions and data structures used in the WPTS API.
 *
 *
 *
 * \defgroup group_ble_service_api_WSS Weight Scale Service (WSS)
 * \ingroup group_ble_service_api
 * \brief
 * The Weight Scale Service exposes weight and related data from a weight scale (Server) intended for consumer healthcare
 * as well as sports/fitness applications.
 *
 * Depending on the chosen GATT role in the GUI, you may use a subset of the supported API.
 *
 * The WSS API names begin with Cy_BLE_WSS_. In addition to this, the API also append the GATT role initial letter in the
 * API name.
 *
 * \defgroup group_ble_service_api_WSS_server_client WSS Server and Client Function
 * \ingroup group_ble_service_api_WSS
 * \brief
 * These are API common to both GATT Client role and GATT Server role. You may use them in either roles.
 *
 * No letter is appended to the API name: Cy_BLE_WSS_
 *
 * \defgroup group_ble_service_api_WSS_server WSS Server Functions
 * \ingroup group_ble_service_api_WSS
 * \brief
 * API unique to WSS designs configured as a GATT Server role.
 *
 * A letter 's' is appended to the API name: Cy_BLE_WSSS_
 *
 * \defgroup group_ble_service_api_WSS_client WSS Client Functions
 * \ingroup group_ble_service_api_WSS
 * \brief
 * API unique to WSS designs configured as a GATT Client role.
 *
 * A letter 'c' is appended to the API name: Cy_BLE_WSSC_
 *
 * \defgroup group_ble_service_api_WSS_definitions WSS Definitions and Data Structures
 * \ingroup group_ble_service_api_WSS
 * \brief
 * Contains the WSS specific definitions and data structures used in the WSS API.
 *
 *
 * \defgroup group_ble_service_api_custom Custom Service
 * \ingroup group_ble_service_api
 * \brief
 * This section contains the the data structures used for Custom Services.
 *
 *
 *
 * \defgroup group_ble_clock_api_section BLE Clock API
 * \ingroup group_ble
 * \brief
 *  This section describes the BLE ECO clock.
 *
 *
 * \defgroup group_ble_clock_api Functions
 * \ingroup group_ble_clock_api_section
 *
 * \defgroup group_ble_clock_api_data_types  Definitions and Data Structures
 * \ingroup group_ble_clock_api_section
 *
 */
