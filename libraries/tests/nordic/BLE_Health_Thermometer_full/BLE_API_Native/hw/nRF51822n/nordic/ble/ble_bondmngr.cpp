/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "ble_bondmngr.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf_error.h"
#include "ble_gap.h"
#include "ble_srv_common.h"
#include "app_util.h"
#include "nrf_assert.h"
//#include "nrf.h"
#include "nrf51_bitfields.h"
#include "crc16.h"
#include "pstorage.h"
#include "ble_bondmngr_cfg.h"

#define CCCD_SIZE                    6                                                                   /**< Number of bytes needed for storing the state of one CCCD. */
#define CRC_SIZE                     2                                                                   /**< Size of CRC in sys_attribute data. */
#define SYS_ATTR_BUFFER_MAX_LEN      (((BLE_BONDMNGR_CCCD_COUNT + 1) * CCCD_SIZE) + CRC_SIZE)            /**< Size of sys_attribute data. */
#define MAX_NUM_CENTRAL_WHITE_LIST   MIN(BLE_BONDMNGR_MAX_BONDED_CENTRALS, 8)                            /**< Maximum number of whitelisted centrals supported.*/
#define MAX_BONDS_IN_FLASH           10                                                                  /**< Maximum number of bonds that can be stored in flash. */
#define BOND_MANAGER_DATA_SIGNATURE  0x53240000

/**@defgroup ble_bond_mngr_sec_access  Bond Manager Security Status Access Macros
 * @brief    The following group of macros abstract access to Security Status with a peer.
 * @{
 */

#define SEC_STATUS_INIT_VAL       0x00                                                                  /**< Initialization value for security status flags. */
#define ENC_STATUS_SET_VAL        0x01                                                                  /**< Bitmask for encryption status. */
#define BOND_IN_PROGRESS_SET_VAL  0x02                                                                  /**< Bitmask for 'bonding in progress'. */


/**@brief   Macro for setting the Encryption Status for current link.
 *
 * @details Macro for setting the Encryption Status for current link.
 */
#define ENCRYPTION_STATUS_SET()                                                                   \
        do                                                                                        \
        {                                                                                         \
             m_sec_con_status |= ENC_STATUS_SET_VAL;                                              \
        } while (0)

/**@brief   Macro for getting the Encryption Status for current link.
 *
 * @details Macro for getting the Encryption Status for current link.
 */
#define ENCRYPTION_STATUS_GET()                                                                   \
        (((m_sec_con_status & ENC_STATUS_SET_VAL) == 0) ? false : true)

/**@brief  Macro for resetting the Encryption Status for current link.
 *
 * @details Macro for resetting the Encryption Status for current link.
 */
#define ENCRYPTION_STATUS_RESET()                                                                 \
        do                                                                                        \
        {                                                                                         \
            m_sec_con_status &= (~ENC_STATUS_SET_VAL);                                            \
        } while (0)


/**@brief   Macro for resetting the Bonding In Progress status for current link.
 *
 * @details Macro for resetting the Bonding In Progress status for current link.
 */
#define BONDING_IN_PROGRESS_STATUS_SET()                                                          \
        do                                                                                        \
        {                                                                                         \
            m_sec_con_status |= BOND_IN_PROGRESS_SET_VAL;                                         \
        } while (0)

/**@brief   Macro for setting the Bonding In Progress status for current link.
 *
 * @details Macro for setting the Bonding In Progress status for current link.
 */
#define BONDING_IN_PROGRESS_STATUS_GET()                                                          \
         (((m_sec_con_status & BOND_IN_PROGRESS_SET_VAL) == 0) ? false: true)

/**@brief   Macro for resetting the Bonding In Progress status for current link.
 *
 * @details Macro for resetting the Bonding In Progress status for current link.
 */
#define BONDING_IN_PROGRESS_STATUS_RESET()                                                        \
        do                                                                                        \
        {                                                                                         \
            m_sec_con_status &= (~BOND_IN_PROGRESS_SET_VAL);                                      \
        } while (0)

/**@brief   Macro for resetting all security status flags for current link.
 *
 * @details Macro for resetting all security status flags for current link.
 */
#define SECURITY_STATUS_RESET()                                                                   \
        do                                                                                        \
        {                                                                                         \
            m_sec_con_status = SEC_STATUS_INIT_VAL;                                               \
        } while (0)

/** @} */

/**@brief    Verify module's initialization status.
 *
 * @details  Verify module's initialization status. Returns NRF_INVALID_STATE in case a module API
 *           is called without initializing the module.
 */
#define VERIFY_MODULE_INITIALIZED()                                                               \
        do                                                                                        \
        {                                                                                         \
            if (!m_is_bondmngr_initialized)                                                       \
            {                                                                                     \
                 return NRF_ERROR_INVALID_STATE;                                                  \
            }                                                                                     \
        } while(0)


/**@brief This structure contains the Bonding Information for one central.
 */
typedef struct
{
    int32_t                        central_handle;                           /**< Central's handle (NOTE: Size is 32 bits just to make struct size dividable by 4). */
    ble_gap_evt_auth_status_t      auth_status;                              /**< Central authentication data. */
    ble_gap_evt_sec_info_request_t central_id_info;                          /**< Central identification info. */
    ble_gap_addr_t                 central_addr;                             /**< Central's address. */
} central_bond_t;

STATIC_ASSERT(sizeof(central_bond_t) % 4 == 0);

/**@brief This structure contains the System Attributes information related to one central.
 */
typedef struct
{
    int32_t  central_handle;                                                 /**< Central's handle (NOTE: Size is 32 bits just to make struct size dividable by 4). */
    uint8_t  sys_attr[SYS_ATTR_BUFFER_MAX_LEN];                              /**< Central sys_attribute data. */
    uint32_t sys_attr_size;                                                  /**< Central sys_attribute data's size (NOTE: Size is 32 bits just to make struct size dividable by 4). */
} central_sys_attr_t;

STATIC_ASSERT(sizeof(central_sys_attr_t) % 4 == 0);

/**@brief This structure contains the Bonding Information and System Attributes related to one
 *        central.
 */
typedef struct
{
    central_bond_t     bond;                                                 /**< Bonding information. */
    central_sys_attr_t sys_attr;                                             /**< System attribute information. */
} central_t;

/**@brief This structure contains the whitelisted addresses.
 */
typedef struct
{
    int8_t           central_handle;                                        /**< Central's handle. */
    ble_gap_addr_t * p_addr;                                                /**< Pointer to the central's address if BLE_GAP_ADDR_TYPE_PUBLIC. */
} whitelist_addr_t;

/**@brief This structure contains the whitelisted IRKs.
 */
typedef struct
{
    int8_t           central_handle;                                        /**< Central's handle. */
    ble_gap_irk_t  * p_irk;                                                 /**< Pointer to the central's irk if available. */
} whitelist_irk_t;

static bool                m_is_bondmngr_initialized = false;               /**< Flag for checking if module has been initialized. */
static ble_bondmngr_init_t m_bondmngr_config;                               /**< Configuration as specified by the application. */
static uint16_t            m_conn_handle;                                   /**< Current connection handle. */
static central_t           m_central;                                       /**< Current central data. */
static central_t           m_centrals_db[BLE_BONDMNGR_MAX_BONDED_CENTRALS]; /**< Pointer to start of bonded centrals database. */
static uint8_t             m_centrals_in_db_count;                          /**< Number of bonded centrals. */
static whitelist_addr_t    m_whitelist_addr[MAX_NUM_CENTRAL_WHITE_LIST];    /**< List of central's addresses  for the whitelist. */
static whitelist_irk_t     m_whitelist_irk[MAX_NUM_CENTRAL_WHITE_LIST];     /**< List of central's IRKs  for the whitelist. */
static uint8_t             m_addr_count;                                    /**< Number of addresses in the whitelist. */
static uint8_t             m_irk_count;                                     /**< Number of IRKs in the whitelist. */
static uint16_t            m_crc_bond_info;                                 /**< Combined CRC for all Bonding Information currently stored in flash. */
static uint16_t            m_crc_sys_attr;                                  /**< Combined CRC for all System Attributes currently stored in flash. */
static pstorage_handle_t   mp_flash_bond_info;                              /**< Pointer to flash location to write next Bonding Information. */
static pstorage_handle_t   mp_flash_sys_attr;                               /**< Pointer to flash location to write next System Attribute information. */
static uint8_t             m_bond_info_in_flash_count;                      /**< Number of Bonding Information currently stored in flash. */
static uint8_t             m_sys_attr_in_flash_count;                       /**< Number of System Attributes currently stored in flash. */
static uint8_t             m_sec_con_status;                                /**< Variable to denote security status.*/
static bool                m_bond_loaded;                                   /**< Variable to indicate if the bonding information of the currently connected central is available in the RAM.*/
static bool                m_sys_attr_loaded;                               /**< Variable to indicate if the system attribute information of the currently connected central is loaded from the database and set in the S110 SoftDevice.*/
static uint32_t            m_bond_crc_array[BLE_BONDMNGR_MAX_BONDED_CENTRALS];
static uint32_t            m_sys_crc_array[BLE_BONDMNGR_MAX_BONDED_CENTRALS];

/**@brief      Function for extracting the CRC from an encoded 32 bit number that typical resides in
 *             the flash memory
 *
 * @param[in]  header   Header containing CRC and magic number.
 * @param[out] p_crc    Extracted CRC.
 *
 * @retval      NRF_SUCCESS              CRC successfully extracted.
 * @retval      NRF_ERROR_NOT_FOUND      Flash seems to be empty.
 * @retval      NRF_ERROR_INVALID_DATA   Header does not contain the magic number.
 */
static uint32_t crc_extract(uint32_t header, uint16_t * p_crc)
{
    if ((header & 0xFFFF0000U) == BOND_MANAGER_DATA_SIGNATURE)
    {
        *p_crc = (uint16_t)(header & 0x0000FFFFU);

        return NRF_SUCCESS;
    }
    else if (header == 0xFFFFFFFFU)
    {
        return NRF_ERROR_NOT_FOUND;
    }
    else
    {
        return NRF_ERROR_INVALID_DATA;
    }
}


/**@brief      Function for storing the Bonding Information of the specified central to the flash.
 *
 * @param[in]  p_bond   Bonding information to be stored.
 *
 * @return     NRF_SUCCESS on success, an error_code otherwise.
 */
static uint32_t bond_info_store(central_bond_t * p_bond)
{
    uint32_t err_code;
    pstorage_handle_t dest_block;

    // Check if flash is full
    if (m_bond_info_in_flash_count >= MAX_BONDS_IN_FLASH)
    {
        return NRF_ERROR_NO_MEM;
    }

    // Check if this is the first bond to be stored
    if (m_bond_info_in_flash_count == 0)
    {
        // Initialize CRC
        m_crc_bond_info = crc16_compute(NULL, 0, NULL);
    }

    // Get block pointer from base
    err_code = pstorage_block_identifier_get(&mp_flash_bond_info,m_bond_info_in_flash_count,&dest_block);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }    
   
    // Write Bonding Information
    err_code = pstorage_store(&dest_block,
                              (uint8_t *)p_bond,
                              sizeof(central_bond_t),
                              sizeof(uint32_t));

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    m_crc_bond_info = crc16_compute((uint8_t *)p_bond,
                                     sizeof(central_bond_t),
                                     &m_crc_bond_info);

    // Write header
    m_bond_crc_array[m_bond_info_in_flash_count] = (BOND_MANAGER_DATA_SIGNATURE | m_crc_bond_info);

    err_code = pstorage_store (&dest_block, (uint8_t *)&m_bond_crc_array[m_bond_info_in_flash_count],sizeof(uint32_t),0);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    m_bond_info_in_flash_count++;
    return NRF_SUCCESS;
}


/**@brief      Function for storing the System Attributes related to a specified central in flash.
 *
 * @param[in]  p_sys_attr   System Attributes to be stored.
 *
 * @return     NRF_SUCCESS on success, an error_code otherwise.
 */
static uint32_t sys_attr_store(central_sys_attr_t * p_sys_attr)
{
    uint32_t err_code;
    pstorage_handle_t dest_block;

    // Check if flash is full.
    if (m_sys_attr_in_flash_count >= MAX_BONDS_IN_FLASH)
    {
        return NRF_ERROR_NO_MEM;
    }

    // Check if this is the first time any System Attributes is stored.
    if (m_sys_attr_in_flash_count == 0)
    {
        // Initialize CRC
        m_crc_sys_attr = crc16_compute(NULL, 0, NULL);
    }


    // Get block pointer from base
    err_code = pstorage_block_identifier_get(&mp_flash_sys_attr,m_sys_attr_in_flash_count,&dest_block);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    // Write System Attributes in flash.
    err_code = pstorage_store(&dest_block,
                              (uint8_t *)p_sys_attr,
                              sizeof(central_sys_attr_t),
                              sizeof(uint32_t));

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    m_crc_sys_attr = crc16_compute((uint8_t *)p_sys_attr,
                                             sizeof(central_sys_attr_t),
                                             &m_crc_sys_attr);

    // Write header.
    m_sys_crc_array[m_sys_attr_in_flash_count] = (BOND_MANAGER_DATA_SIGNATURE | m_crc_sys_attr);

    err_code = pstorage_store (&dest_block,
                               (uint8_t *)&m_sys_crc_array[m_sys_attr_in_flash_count],
                               sizeof(uint32_t),
                               0);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    m_sys_attr_in_flash_count++;

    
    return NRF_SUCCESS;
}


/**@brief      Function for loading the Bonding Information of one central from flash.
 *
 * @param[out] p_bond   Loaded Bonding Information.
 *
 * @return     NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t bonding_info_load_from_flash(central_bond_t * p_bond)
{
    pstorage_handle_t source_block;
    uint32_t          err_code;
    uint32_t          crc;
    uint16_t          crc_header;
    
    // Check if this is the first bond to be loaded, in which case the
    // m_bond_info_in_flash_count variable would have the intial value 0.
    if (m_bond_info_in_flash_count == 0)
    {
        // Initialize CRC.
        m_crc_bond_info = crc16_compute(NULL, 0, NULL);
    }

    // Get block pointer from base
    err_code = pstorage_block_identifier_get(&mp_flash_bond_info,
                                             m_bond_info_in_flash_count,
                                             &source_block);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
        
    err_code = pstorage_load((uint8_t *)&crc, &source_block, sizeof(uint32_t), 0);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Extract CRC from header.
    err_code = crc_extract(crc, &crc_header);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Load central.
    err_code = pstorage_load((uint8_t *)p_bond,
                             &source_block,
                             sizeof(central_bond_t),
                             sizeof(uint32_t));
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Check CRC.
    m_crc_bond_info = crc16_compute((uint8_t *)p_bond,
                                    sizeof(central_bond_t),
                                    &m_crc_bond_info);
    if (m_crc_bond_info == crc_header)
    {
        m_bond_info_in_flash_count++;
        return NRF_SUCCESS;
    }
    else
    {
        return NRF_ERROR_INVALID_DATA;
    }
}



/**@brief      Function for loading the System Attributes related to one central from flash.
 *
 * @param[out] p_sys_attr   Loaded System Attributes.
 *
 * @return     NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t sys_attr_load_from_flash(central_sys_attr_t * p_sys_attr)
{
    pstorage_handle_t source_block;
    uint32_t err_code;
    uint32_t crc;
    uint16_t crc_header;

    // Check if this is the first time System Attributes is loaded from flash, in which case the
    // m_sys_attr_in_flash_count variable would have the initial value 0.
    if (m_sys_attr_in_flash_count == 0)
    {
        // Initialize CRC.
        m_crc_sys_attr = crc16_compute(NULL, 0, NULL);
    }

    // Get block pointer from base
    err_code = pstorage_block_identifier_get(&mp_flash_sys_attr,
                                             m_sys_attr_in_flash_count,
                                             &source_block);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    err_code = pstorage_load((uint8_t *)&crc, &source_block, sizeof(uint32_t), 0);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    // Extract CRC from header.
    err_code = crc_extract(crc, &crc_header);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    err_code = pstorage_load((uint8_t *)p_sys_attr,
                             &source_block,
                             sizeof(central_sys_attr_t),
                             sizeof(uint32_t));
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Check CRC.
    m_crc_sys_attr = crc16_compute((uint8_t *)p_sys_attr,
                                   sizeof(central_sys_attr_t),
                                   &m_crc_sys_attr);

    if (m_crc_sys_attr == crc_header)
    {
        m_sys_attr_in_flash_count++;
        return NRF_SUCCESS;
    }
    else
    {
        return NRF_ERROR_INVALID_DATA;
    }
}


/**@brief      Function for erasing the flash pages that contain Bonding Information and System
 *             Attributes.
 *
 * @return     NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t flash_pages_erase(void)
{
    uint32_t err_code;

    err_code = pstorage_clear(&mp_flash_bond_info, MAX_BONDS_IN_FLASH);

    if (err_code == NRF_SUCCESS)
    {
        err_code = pstorage_clear(&mp_flash_sys_attr, MAX_BONDS_IN_FLASH);
    }

    return err_code;
}


/**@brief      Function for checking if Bonding Information in RAM is different from that in
 *             flash.
 *
 * @return     TRUE if Bonding Information in flash and RAM are different, FALSE otherwise.
 */
static bool bond_info_changed(void)
{
    int      i;
    uint16_t crc = crc16_compute(NULL, 0, NULL);

    // Compute CRC for all bonds in database.
    for (i = 0; i < m_centrals_in_db_count; i++)
    {
        crc = crc16_compute((uint8_t *)&m_centrals_db[i].bond,
                                      sizeof(central_bond_t),
                                      &crc);
    }

    // Compare the computed CRS to CRC stored in flash.
    return (crc != m_crc_bond_info);
}


/**@brief      Function for checking if System Attributes in RAM is different from that in flash.
 *
 * @return     TRUE if System Attributes in flash and RAM are different, FALSE otherwise.
 */
static bool sys_attr_changed(void)
{
    int      i;
    uint16_t crc = crc16_compute(NULL, 0, NULL);

    // Compute CRC for all System Attributes in database.
    for (i = 0; i < m_centrals_in_db_count; i++)
    {
        crc = crc16_compute((uint8_t *)&m_centrals_db[i].sys_attr,
                                      sizeof(central_sys_attr_t),
                                      &crc);
    }

    // Compare the CRC of System Attributes in flash with that of the System Attributes in memory.
    return (crc != m_crc_sys_attr);
}


/**@brief      Function for setting the System Attributes for specified central to the SoftDevice, or
 *             clearing the System Attributes if central is a previously unknown.
 *
 * @param[in]  p_central   Central for which the System Attributes is to be set.
 *
 * @return     NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t central_sys_attr_set(central_t * p_central)
{
    uint8_t * p_sys_attr;

    if (m_central.sys_attr.sys_attr_size != 0)
    {
        if (m_central.sys_attr.sys_attr_size > SYS_ATTR_BUFFER_MAX_LEN)
        {
            return NRF_ERROR_INTERNAL;
        }

        p_sys_attr = m_central.sys_attr.sys_attr;
    }
    else
    {
        p_sys_attr = NULL;
    }

    return sd_ble_gatts_sys_attr_set(m_conn_handle, p_sys_attr, m_central.sys_attr.sys_attr_size);
}


/**@brief Function for updating the whitelist data structures.
 */
static void update_whitelist(void)
{
    int i;

    for (i = 0, m_addr_count = 0, m_irk_count = 0; i < m_centrals_in_db_count; i++)
    {
        central_bond_t * p_bond = &m_centrals_db[i].bond;

        if (p_bond->auth_status.central_kex.irk)
        {
            m_whitelist_irk[m_irk_count].central_handle = p_bond->central_handle;
            m_whitelist_irk[m_irk_count].p_irk          = &(p_bond->auth_status.central_keys.irk);

            m_irk_count++;
        }

        if (p_bond->central_addr.addr_type != BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE)
        {
            m_whitelist_addr[m_addr_count].central_handle = p_bond->central_handle;
            m_whitelist_addr[m_addr_count].p_addr         = &(p_bond->central_addr);

            m_addr_count++;
        }
    }
}


/**@brief      Function for handling the authentication status event related to a new central.
 *
 * @details    This function adds the new central to the database and stores the central's Bonding
 *             Information to flash. It also notifies the application when the new bond is created,
 *             and sets the System Attributes to prepare the stack for connection with the new
 *             central.
 *
 * @param[in]  p_auth_status   New authentication status.
 *
 * @return     NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t on_auth_status_from_new_central(ble_gap_evt_auth_status_t * p_auth_status)
{
    uint32_t err_code;

    if (m_centrals_in_db_count >= BLE_BONDMNGR_MAX_BONDED_CENTRALS)
    {
        return NRF_ERROR_NO_MEM;
    }

    // Update central.
    m_central.bond.auth_status         = *p_auth_status;
    m_central.bond.central_id_info.div = p_auth_status->periph_keys.enc_info.div;
    m_central.sys_attr.sys_attr_size   = 0;

    // Add new central to database.
    m_central.bond.central_handle           = m_centrals_in_db_count;
    m_centrals_db[m_centrals_in_db_count++] = m_central;

    update_whitelist();

    m_bond_loaded = true;

    // Clear System Attributes.
    err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Write new central's Bonding Information to flash.
    err_code = bond_info_store(&m_central.bond);
    if ((err_code == NRF_ERROR_NO_MEM) && (m_bondmngr_config.evt_handler != NULL))
    {
        ble_bondmngr_evt_t evt;

        evt.evt_type       = BLE_BONDMNGR_EVT_BOND_FLASH_FULL;
        evt.central_handle = m_central.bond.central_handle;
        evt.central_id     = m_central.bond.central_id_info.div;

        m_bondmngr_config.evt_handler(&evt);
    }
    else if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Pass the event to application.
    if (m_bondmngr_config.evt_handler != NULL)
    {
        ble_bondmngr_evt_t evt;

        evt.evt_type       = BLE_BONDMNGR_EVT_NEW_BOND;
        evt.central_handle = m_central.bond.central_handle;
        evt.central_id     = m_central.bond.central_id_info.div;

        m_bondmngr_config.evt_handler(&evt);
    }

    return NRF_SUCCESS;
}


/**@brief Function for updating the current central's entry in the database.
 */
static uint32_t central_update(void)
{
    uint32_t err_code;
    int32_t  central_handle = m_central.bond.central_handle;

    if ((central_handle >= 0) && (central_handle < m_centrals_in_db_count))
    {
        // Update the database based on whether the bond and system attributes have
        // been loaded or not to avoid overwriting information that was not used in the
        // connection session.
        if (m_bond_loaded)
        {
            m_centrals_db[central_handle].bond = m_central.bond;
        }

        if (m_sys_attr_loaded)
        {
            m_centrals_db[central_handle].sys_attr = m_central.sys_attr;
        }

        update_whitelist();

        err_code = NRF_SUCCESS;
    }
    else
    {
        err_code = NRF_ERROR_INTERNAL;
    }

    return err_code;
}


/**@brief      Function for searching for the central in the database of known centrals.
 *
 * @details    If the central is found, the variable m_central will be populated with all the
 *             information (Bonding Information and System Attributes) related to that central.
 *
 * @param[in]  central_id   Central Identifier.
 * @return     NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t central_find_in_db(uint16_t central_id)
{
    int i;

    for (i = 0; i < m_centrals_in_db_count; i++)
    {
        if (central_id == m_centrals_db[i].bond.central_id_info.div)
        {
            m_central = m_centrals_db[i];
            return NRF_SUCCESS;
        }
    }

    return NRF_ERROR_NOT_FOUND;
}


/**@brief      Function for loading all Bonding Information and System Attributes from flash.
 *
 * @return     NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t load_all_from_flash(void)
{
    uint32_t err_code;
    int      i;

    m_centrals_in_db_count = 0;

    while (m_centrals_in_db_count < BLE_BONDMNGR_MAX_BONDED_CENTRALS)
    {
        central_bond_t central_bond_info;
        int           central_handle;

        // Load Bonding Information.
        err_code = bonding_info_load_from_flash(&central_bond_info);
        if (err_code == NRF_ERROR_NOT_FOUND)
        {
            // No more bonds in flash.
            break;
        }
        else if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        central_handle = central_bond_info.central_handle;
        if (central_handle > m_centrals_in_db_count)
        {
            // Central handle value(s) missing in flash. This should never happen.
            return NRF_ERROR_INVALID_DATA;
        }
        else
        {
            // Add/update Bonding Information in central array.
            m_centrals_db[central_handle].bond = central_bond_info;
            if (central_handle == m_centrals_in_db_count)
            {
                // New central handle, clear System Attributes.
                m_centrals_db[central_handle].sys_attr.sys_attr_size  = 0;
                m_centrals_db[central_handle].sys_attr.central_handle = INVALID_CENTRAL_HANDLE;
                m_centrals_in_db_count++;
            }
            else
            {
                // Entry was updated, do nothing.
            }
        }
    }

    // Load System Attributes for all previously known centrals.
    for (i = 0; i < m_centrals_in_db_count; i++)
    {
        central_sys_attr_t central_sys_attr;

        // Load System Attributes.
        err_code = sys_attr_load_from_flash(&central_sys_attr);
        if (err_code == NRF_ERROR_NOT_FOUND)
        {
            // No more System Attributes in flash.
            break;
        }
        else if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        if (central_sys_attr.central_handle > m_centrals_in_db_count)
        {
            // Central handle value(s) missing in flash. This should never happen.
            return NRF_ERROR_INVALID_DATA;
        }
        else
        {
            // Add/update Bonding Information in central array.
            m_centrals_db[central_sys_attr.central_handle].sys_attr = central_sys_attr;
        }
    }

    // Initialize the remaining empty bond entries in the memory.
    for (i = m_centrals_in_db_count; i < BLE_BONDMNGR_MAX_BONDED_CENTRALS; i++)
    {
        m_centrals_db[i].bond.central_handle     = INVALID_CENTRAL_HANDLE;
        m_centrals_db[i].sys_attr.sys_attr_size  = 0;
        m_centrals_db[i].sys_attr.central_handle = INVALID_CENTRAL_HANDLE;
    }

    // Update whitelist data structures.
    update_whitelist();

    return NRF_SUCCESS;
}


/**@brief      Function for handling the connected event received from the BLE stack.
 *
 * @param[in]  p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_evt_t * p_ble_evt)
{
    m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

    m_central.bond.central_handle     = INVALID_CENTRAL_HANDLE;
    m_central.bond.central_addr       = p_ble_evt->evt.gap_evt.params.connected.peer_addr;
    m_central.sys_attr.sys_attr_size  = 0;

    if (p_ble_evt->evt.gap_evt.params.connected.irk_match)
    {
        uint8_t irk_idx  = p_ble_evt->evt.gap_evt.params.connected.irk_match_idx;

        if ((irk_idx >= MAX_NUM_CENTRAL_WHITE_LIST) ||
            (m_whitelist_irk[irk_idx].central_handle >= BLE_BONDMNGR_MAX_BONDED_CENTRALS))
        {
            m_bondmngr_config.error_handler(NRF_ERROR_INTERNAL);
        }
        else
        {
            m_central = m_centrals_db[m_whitelist_irk[irk_idx].central_handle];
        }
    }
    else
    {
        int i;

        for (i = 0; i < m_addr_count; i++)
        {
            ble_gap_addr_t * p_cur_addr = m_whitelist_addr[i].p_addr;

            if (memcmp(p_cur_addr->addr, m_central.bond.central_addr.addr, BLE_GAP_ADDR_LEN) == 0)
            {
                m_central = m_centrals_db[m_whitelist_addr[i].central_handle];
                break;
            }
        }
    }

    if (m_central.bond.central_handle != INVALID_CENTRAL_HANDLE)
    {
        // Reset bond and system attributes loaded variables.
        m_bond_loaded     = false;
        m_sys_attr_loaded = false;

        // Do not set the system attributes of the central on connection.
        if (m_bondmngr_config.evt_handler != NULL)
        {
            ble_bondmngr_evt_t evt;

            evt.evt_type       = BLE_BONDMNGR_EVT_CONN_TO_BONDED_CENTRAL;
            evt.central_handle = m_central.bond.central_handle;
            evt.central_id     = m_central.bond.central_id_info.div;

            m_bondmngr_config.evt_handler(&evt);
        }
    }
}


/**@brief      Function for handling the 'System Attributes Missing' event received from the
 *             SoftDevice.
 *
 * @param[in]  p_ble_evt   Event received from the BLE stack.
 */
static void on_sys_attr_missing(ble_evt_t * p_ble_evt)
{
    uint32_t err_code;

    if (
        (m_central.bond.central_handle == INVALID_CENTRAL_HANDLE) ||
        !ENCRYPTION_STATUS_GET()                                  ||
        BONDING_IN_PROGRESS_STATUS_GET()
       )
    {
        err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0);
    }
    else
    {
        // Current central is valid, use its data. Set the corresponding sys_attr.
        err_code = central_sys_attr_set(&m_central);
        if (err_code == NRF_SUCCESS)
        {
            // Set System Attributes loaded status variable.
            m_sys_attr_loaded = true;
        }
    }

    if (err_code != NRF_SUCCESS)
    {
        m_bondmngr_config.error_handler(err_code);
    }
}


/**@brief      Function for handling the new authentication status event, received from the
 *             SoftDevice, related to an already bonded central.
 *
 * @details    This function also writes the updated Bonding Information to flash and notifies the
 *             application.
 *
 * @param[in]  p_auth_status   Updated authentication status.
 */
static void auth_status_update(ble_gap_evt_auth_status_t * p_auth_status)
{
    uint32_t err_code;

    // Authentication status changed, update Bonding Information.
    m_central.bond.auth_status         = *p_auth_status;
    m_central.bond.central_id_info.div = p_auth_status->periph_keys.enc_info.div;

    memset(&(m_centrals_db[m_central.bond.central_handle]), 0, sizeof(central_t));
    m_centrals_db[m_central.bond.central_handle] = m_central;

    // Write updated Bonding Information to flash.
    err_code = bond_info_store(&m_central.bond);
    if ((err_code == NRF_ERROR_NO_MEM) && (m_bondmngr_config.evt_handler != NULL))
    {
        ble_bondmngr_evt_t evt;

        evt.evt_type       = BLE_BONDMNGR_EVT_BOND_FLASH_FULL;
        evt.central_handle = m_central.bond.central_handle;
        evt.central_id     = m_central.bond.central_id_info.div;

        m_bondmngr_config.evt_handler(&evt);
    }
    else if (err_code != NRF_SUCCESS)
    {
        m_bondmngr_config.error_handler(err_code);
    }

   // Pass the event to the application.
   if (m_bondmngr_config.evt_handler != NULL)
   {
       ble_bondmngr_evt_t evt;

        evt.evt_type       = BLE_BONDMNGR_EVT_AUTH_STATUS_UPDATED;
        evt.central_handle = m_central.bond.central_handle;
        evt.central_id     = m_central.bond.central_id_info.div;

       m_bondmngr_config.evt_handler(&evt);
   }
}


/**@brief      Function for handling the Authentication Status event received from the BLE stack.
 *
 * @param[in]  p_ble_evt   Event received from the BLE stack.
 */
static void on_auth_status(ble_gap_evt_auth_status_t * p_auth_status)
{
    if (p_auth_status->auth_status != BLE_GAP_SEC_STATUS_SUCCESS)
    {
        return;
    }

    // Verify if its pairing and not bonding
    if (!ENCRYPTION_STATUS_GET())
    {
        return;
    }

    if (m_central.bond.central_handle == INVALID_CENTRAL_HANDLE)
        {
        uint32_t err_code = central_find_in_db(p_auth_status->periph_keys.enc_info.div);

        if (err_code == NRF_SUCCESS)
        {
            // Possible DIV Collision indicate error to application,
            // not storing the new LTK
            err_code = NRF_ERROR_FORBIDDEN;
        }
        else
        {
            // Add the new device to data base
            err_code = on_auth_status_from_new_central(p_auth_status);
        }

        if (err_code != NRF_SUCCESS)
        {
            m_bondmngr_config.error_handler(err_code);
        }
    }
    else
    {
        m_bond_loaded = true;

        // Receiving a auth status again when already in have existing information!
        auth_status_update(p_auth_status);
    }
}


/**@brief      Function for handling the Security Info Request event received from the BLE stack.
 *
 * @param[in]  p_ble_evt   Event received from the BLE stack.
 */
static void on_sec_info_request(ble_evt_t * p_ble_evt)
{
    uint32_t err_code;

    err_code = central_find_in_db(p_ble_evt->evt.gap_evt.params.sec_info_request.div);
    if (err_code == NRF_SUCCESS)
    {
        // Bond information has been found and loaded for security procedures. Reflect this in the
        // status variable
        m_bond_loaded = true;

        // Central found in the list of bonded central. Use the encryption info for this central.
        err_code = sd_ble_gap_sec_info_reply(m_conn_handle,
                                             &m_central.bond.auth_status.periph_keys.enc_info,
                                             NULL);
        if (err_code != NRF_SUCCESS)
        {
            m_bondmngr_config.error_handler(err_code);
        }

        // Do not set the sys_attr yet, should be set only when sec_update is successful.
    }
    else if (err_code == NRF_ERROR_NOT_FOUND)
    {
        m_central.bond.central_id_info = p_ble_evt->evt.gap_evt.params.sec_info_request;

        // New central.
        err_code = sd_ble_gap_sec_info_reply(m_conn_handle, NULL, NULL);
        if (err_code != NRF_SUCCESS)
        {
            m_bondmngr_config.error_handler(err_code);
        }

        // Initialize the sys_attr.
        err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0);
    }

    if (err_code != NRF_SUCCESS)
    {
        m_bondmngr_config.error_handler(err_code);
    }
}


/**@brief      Function for handling the Connection Security Update event received from the BLE
 *             stack.
 *
 * @param[in]  p_ble_evt   Event received from the BLE stack.
 */
static void on_sec_update(ble_gap_evt_conn_sec_update_t * p_sec_update)
{
    uint8_t security_mode  = p_sec_update->conn_sec.sec_mode.sm;
    uint8_t security_level = p_sec_update->conn_sec.sec_mode.lv;

    if (((security_mode == 1) && (security_level > 1)) ||
        ((security_mode == 2) && (security_level != 0)))
    {
        ENCRYPTION_STATUS_SET();

        uint32_t err_code = central_sys_attr_set(&m_central);

        if (err_code != NRF_SUCCESS)
        {
            m_bondmngr_config.error_handler(err_code);
        }
        else
        {
            m_sys_attr_loaded = true;
        }

        if (m_bondmngr_config.evt_handler != NULL)
        {
            ble_bondmngr_evt_t evt;

            evt.evt_type       = BLE_BONDMNGR_EVT_ENCRYPTED;
            evt.central_handle = m_central.bond.central_handle;
            evt.central_id     = m_central.bond.central_id_info.div;

            m_bondmngr_config.evt_handler(&evt);
        }
    }
}


/**@brief      Function for handling the Connection Security Parameters Request event received from
 *             the BLE stack.
 *
 * @param[in]  p_ble_evt   Event received from the BLE stack.
 */
static void on_sec_param_request(ble_gap_evt_sec_params_request_t * p_sec_update)
{
    if (p_sec_update->peer_params.bond)
    {
        BONDING_IN_PROGRESS_STATUS_SET();

        if (m_central.bond.central_handle != INVALID_CENTRAL_HANDLE)
        {
            // Bonding request received from a bonded central, make all system attributes null
            m_central.sys_attr.sys_attr_size = 0;
            memset(m_central.sys_attr.sys_attr, 0, SYS_ATTR_BUFFER_MAX_LEN);
        }
    }
}


void ble_bondmngr_on_ble_evt(ble_evt_t * p_ble_evt)
{
    if (!m_is_bondmngr_initialized)
    {
        m_bondmngr_config.error_handler(NRF_ERROR_INVALID_STATE);
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_ble_evt);
            break;

        // NOTE: All actions to be taken on the Disconnected event are performed in
        //       ble_bondmngr_bonded_centrals_store(). This function must be called from the
        //       Disconnected handler of the application before advertising is restarted (to make
        //       sure the flash blocks are cleared while the radio is inactive).
        case BLE_GAP_EVT_DISCONNECTED:
            SECURITY_STATUS_RESET();
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            on_sys_attr_missing(p_ble_evt);
            break;

        case BLE_GAP_EVT_AUTH_STATUS:
            on_auth_status(&p_ble_evt->evt.gap_evt.params.auth_status);
            break;

        case BLE_GAP_EVT_SEC_INFO_REQUEST:
            on_sec_info_request(p_ble_evt);
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            on_sec_param_request(&p_ble_evt->evt.gap_evt.params.sec_params_request);
            break;

        case BLE_GAP_EVT_CONN_SEC_UPDATE:
            on_sec_update(&p_ble_evt->evt.gap_evt.params.conn_sec_update);
            break;

        default:
            // No implementation needed.
            break;
    }
}


uint32_t ble_bondmngr_bonded_centrals_store(void)
{
    uint32_t err_code;
    int      i;

    VERIFY_MODULE_INITIALIZED();

    if (m_central.bond.central_handle != INVALID_CENTRAL_HANDLE)
    {
        // Fetch System Attributes from stack.
        uint16_t sys_attr_size = SYS_ATTR_BUFFER_MAX_LEN;

        err_code = sd_ble_gatts_sys_attr_get(m_conn_handle,
                                             m_central.sys_attr.sys_attr,
                                             &sys_attr_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        m_central.sys_attr.central_handle = m_central.bond.central_handle;
        m_central.sys_attr.sys_attr_size  = (uint16_t)sys_attr_size;

        // Update the current central.
        err_code = central_update();
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Save Bonding Information if changed.
    if (bond_info_changed())
    {
        // Erase flash page.
        err_code = pstorage_clear(&mp_flash_bond_info,MAX_BONDS_IN_FLASH);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        // Store bond information for all centrals.
        m_bond_info_in_flash_count = 0;
        for (i = 0; i < m_centrals_in_db_count; i++)
        {
            err_code = bond_info_store(&m_centrals_db[i].bond);
            if (err_code != NRF_SUCCESS)
            {
                return err_code;
            }
        }
    }

    // Save System Attributes, if changed.
    if (sys_attr_changed())
    {
        // Erase flash page.
        err_code = pstorage_clear(&mp_flash_sys_attr, MAX_BONDS_IN_FLASH);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        // Store System Attributes for all centrals.
        m_sys_attr_in_flash_count = 0;
        for (i = 0; i < m_centrals_in_db_count; i++)
        {
            err_code = sys_attr_store(&m_centrals_db[i].sys_attr);
            if (err_code != NRF_SUCCESS)
            {
                return err_code;
            }
        }
    }

    m_conn_handle                     = BLE_CONN_HANDLE_INVALID;
    m_central.bond.central_handle     = INVALID_CENTRAL_HANDLE;
    m_central.sys_attr.central_handle = INVALID_CENTRAL_HANDLE;
    m_central.sys_attr.sys_attr_size  = 0;
    m_bond_loaded                     = false;
    m_sys_attr_loaded                 = false;

    return NRF_SUCCESS;
}


uint32_t ble_bondmngr_sys_attr_store(void)
{
    uint32_t err_code;

    if (m_central.sys_attr.sys_attr_size == 0)
    {
        // Connected to new central. So the flash block for System Attributes for this
        // central is empty. Hence no erase is needed.

        uint16_t sys_attr_size = SYS_ATTR_BUFFER_MAX_LEN;

        // Fetch System Attributes from stack.
        err_code = sd_ble_gatts_sys_attr_get(m_conn_handle,
                                             m_central.sys_attr.sys_attr,
                                             &sys_attr_size);

        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        m_central.sys_attr.central_handle = m_central.bond.central_handle;
        m_central.sys_attr.sys_attr_size  = (uint16_t)sys_attr_size;

        // Copy the System Attributes to database.
        m_centrals_db[m_central.bond.central_handle].sys_attr = m_central.sys_attr;

        // Write new central's System Attributes to flash.
        return (sys_attr_store(&m_central.sys_attr));
    }
    else
    {
        // Will not write to flash because System Attributes of an old central would already be
        // in flash and so this operation needs a flash erase operation.
        return NRF_ERROR_INVALID_STATE;
    }
}


uint32_t ble_bondmngr_bonded_centrals_delete(void)
{
    VERIFY_MODULE_INITIALIZED();

    m_centrals_in_db_count         = 0;
    m_bond_info_in_flash_count     = 0;
    m_sys_attr_in_flash_count      = 0;

    return flash_pages_erase();
}


uint32_t ble_bondmngr_central_addr_get(int8_t central_handle, ble_gap_addr_t * p_central_addr)
{
    if (
        (central_handle == INVALID_CENTRAL_HANDLE) ||
        (central_handle >= m_centrals_in_db_count) ||
        (p_central_addr == NULL)                   ||
        (
         m_centrals_db[central_handle].bond.central_addr.addr_type
         ==
         BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE
        )
        )
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    *p_central_addr = m_centrals_db[central_handle].bond.central_addr;
    return NRF_SUCCESS;
}


uint32_t ble_bondmngr_whitelist_get(ble_gap_whitelist_t * p_whitelist)
{
    static ble_gap_addr_t * s_addr[MAX_NUM_CENTRAL_WHITE_LIST];
    static ble_gap_irk_t  * s_irk[MAX_NUM_CENTRAL_WHITE_LIST];

    int i;

    for (i = 0; i < m_irk_count; i++)
    {
        s_irk[i] = m_whitelist_irk[i].p_irk;
    }
    for (i = 0; i < m_addr_count; i++)
    {
        s_addr[i] = m_whitelist_addr[i].p_addr;
    }

    p_whitelist->addr_count = m_addr_count;
    p_whitelist->pp_addrs   = (m_addr_count != 0) ? s_addr : NULL;
    p_whitelist->irk_count  = m_irk_count;
    p_whitelist->pp_irks    = (m_irk_count != 0) ? s_irk : NULL;

    return NRF_SUCCESS;
}


static void bm_pstorage_cb_handler(pstorage_handle_t * handle,
                                   uint8_t             op_code,
                                   uint32_t            result,
                                   uint8_t           * p_data,
                                   uint32_t            data_len)
{
    if (result != NRF_SUCCESS)
    {
        m_bondmngr_config.error_handler(result);
    }
}


uint32_t ble_bondmngr_init(ble_bondmngr_init_t * p_init)
{
    pstorage_module_param_t param;
    uint32_t err_code;

    if (p_init->error_handler == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    if (BLE_BONDMNGR_MAX_BONDED_CENTRALS > MAX_BONDS_IN_FLASH)
    {
        return NRF_ERROR_DATA_SIZE;
    }
          
    param.block_size  = sizeof (central_bond_t) + sizeof (uint32_t);
    param.block_count = MAX_BONDS_IN_FLASH;
    param.cb          = bm_pstorage_cb_handler;
    
    // Blocks are requested twice, once for bond information and once for system attributes.
    // The number of blocks requested has to be the maximum number of bonded devices that
    // need to be supported. However, the size of blocks can be different if the sizes of
    // system attributes and bonds are not too close.
    err_code = pstorage_register(&param, &mp_flash_bond_info);    
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    param.block_size = sizeof(central_sys_attr_t) + sizeof(uint32_t);
    
    err_code = pstorage_register(&param, &mp_flash_sys_attr);    
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }    
    
    m_bondmngr_config = *p_init;

    memset(&m_central, 0, sizeof(central_t));

    m_central.bond.central_handle  = INVALID_CENTRAL_HANDLE;
    m_conn_handle                  = BLE_CONN_HANDLE_INVALID;
    m_centrals_in_db_count         = 0;
    m_bond_info_in_flash_count     = 0;
    m_sys_attr_in_flash_count      = 0;

    SECURITY_STATUS_RESET();

    // Erase all stored centrals if specified.
    if (m_bondmngr_config.bonds_delete)
    {
        err_code = flash_pages_erase();
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        m_centrals_in_db_count = 0;
        int i;
        for (i = m_centrals_in_db_count; i < BLE_BONDMNGR_MAX_BONDED_CENTRALS; i++)
        {
            m_centrals_db[i].bond.central_handle     = INVALID_CENTRAL_HANDLE;
            m_centrals_db[i].sys_attr.sys_attr_size  = 0;
            m_centrals_db[i].sys_attr.central_handle = INVALID_CENTRAL_HANDLE;
        }
    }
    else
    {
        // Load bond manager data from flash.
        err_code = load_all_from_flash();
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    m_is_bondmngr_initialized = true;
    
    return NRF_SUCCESS;
}


uint32_t ble_bondmngr_central_ids_get(uint16_t * p_central_ids, uint16_t * p_length)
{
    VERIFY_MODULE_INITIALIZED();
    int i;
    if (p_length == NULL)
    {
        return NRF_ERROR_NULL;
    }

    if (*p_length < m_centrals_in_db_count)
    {
        // Length of the input array is not enough to fit all known central identifiers.
        return NRF_ERROR_DATA_SIZE;
    }

    *p_length = m_centrals_in_db_count;
    if (p_central_ids == NULL)
    {
        // Only the length field was required to be filled.
        return NRF_SUCCESS;
    }

    for (i = 0; i < m_centrals_in_db_count; i++)
    {
        p_central_ids[i] = m_centrals_db[i].bond.central_id_info.div;
    }

    return NRF_SUCCESS;
}


uint32_t ble_bondmngr_bonded_central_delete(uint16_t central_id)
{
    VERIFY_MODULE_INITIALIZED();

    int8_t  central_handle_to_be_deleted = INVALID_CENTRAL_HANDLE;
    uint8_t i;

    // Search for the handle of the central.
    for (i = 0; i < m_centrals_in_db_count; i++)
    {
        if (m_centrals_db[i].bond.central_id_info.div == central_id)
        {
            central_handle_to_be_deleted = i;
            break;
        }
    }

    if (central_handle_to_be_deleted == INVALID_CENTRAL_HANDLE)
    {
        // Central ID not found.
        return NRF_ERROR_NOT_FOUND;
    }

    // Delete the central in RAM.
    for (i = central_handle_to_be_deleted; i < (m_centrals_in_db_count - 1); i++)
    {
        // Overwrite the current central entry with the next one.
        m_centrals_db[i] = m_centrals_db[i + 1];

        // Decrement the value of handle.
        m_centrals_db[i].bond.central_handle--;
        if (INVALID_CENTRAL_HANDLE != m_centrals_db[i].sys_attr.central_handle)
        {
            m_centrals_db[i].sys_attr.central_handle--;
        }
    }

    // Clear the last database entry.
    memset(&(m_centrals_db[m_centrals_in_db_count - 1]), 0, sizeof(central_t));

    m_centrals_in_db_count--;

    uint32_t err_code;

    // Reinitialize the pointers to the memory where bonding info and System Attributes are stored
    // in flash.
    // Refresh the data in the flash memory (both Bonding Information and System Attributes).
    // Erase and rewrite bonding info and System Attributes.

    err_code = flash_pages_erase();
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    m_bond_info_in_flash_count    = 0;
    m_sys_attr_in_flash_count     = 0;

    for (i = 0; i < m_centrals_in_db_count; i++)
    {
        err_code = bond_info_store(&(m_centrals_db[i].bond));
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    for (i = 0; i < m_centrals_in_db_count; i++)
    {
        err_code = sys_attr_store(&(m_centrals_db[i].sys_attr));
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    update_whitelist();

    return NRF_SUCCESS;
}


uint32_t ble_bondmngr_is_link_encrypted (bool * status)
{
    VERIFY_MODULE_INITIALIZED();

    (*status) = ENCRYPTION_STATUS_GET();

    return NRF_SUCCESS;
}
