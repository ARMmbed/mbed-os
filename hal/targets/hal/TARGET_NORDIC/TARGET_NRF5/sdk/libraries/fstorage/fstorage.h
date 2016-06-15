/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
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

#ifndef FSTORAGE_H__
#define FSTORAGE_H__

/**
 * @defgroup fstorage fstorage
 * @ingroup app_common
 * @{
 *
 * @brief   Module which provides functionality to store data to flash and erase flash pages.
 */

#include <stdint.h>
#include "section_vars.h"


/**@brief   fstorage return values. */
typedef enum
{
    FS_SUCCESS,
    FS_ERR_NOT_INITIALIZED,     //!< Error. The module is not initialized.
    FS_ERR_INVALID_CFG,         //!< Error. Invalid fstorage configuration.
    FS_ERR_NULL_ARG,            //!< Error. Argument is NULL.
    FS_ERR_INVALID_ARG,         //!< Error. Argument contains invalid data.
    FS_ERR_INVALID_ADDR,        //!< Error. Address out of bounds.
    FS_ERR_UNALIGNED_ADDR,      //!< Error. Unaligned address.
    FS_ERR_QUEUE_FULL,          //!< Error. Queue is full.
    FS_ERR_OPERATION_TIMEOUT,   //!< Error. The operation has timed out.
    FS_ERR_INTERNAL,            //!< Error. Internal error.
} fs_ret_t;


/**@brief   fstorage event IDs. */
typedef enum
{
    FS_EVT_STORE,   //!< Event for @ref fs_store.
    FS_EVT_ERASE    //!< Event for @ref fs_erase.
} fs_evt_id_t;


#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#endif

/**@brief   An fstorage event. */
typedef struct
{
    fs_evt_id_t id;                         //!< The event ID.
    union
    {
        struct
        {
            uint32_t const * p_data;        //!< Pointer to the data stored in flash.
            uint16_t         length_words;  //!< Length of the data, in 4-byte words.
        } store;
        struct
        {
            uint16_t first_page;            //!< First page erased.
            uint16_t last_page;             //!< Last page erased.
        } erase;
    };
} fs_evt_t;

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
  /* leave anonymous unions enabled */
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#endif


/**@brief   fstorage event handler function prototype.
 *
 * @param[in]   evt     The event.
 * @param[in]   result  The result of the operation.
 */
typedef void (*fs_cb_t)(fs_evt_t const * const evt, fs_ret_t result);


/**@brief   fstorage application-specific configuration.
 *
 * @details Specifies the callback to invoke when an operation completes, the number of flash pages
 *          requested by the application and the priority with which these are to be assigned, with
 *          respect to other applications. Additionally, the configuration specifies the boundaries
 *          of the flash space assigned to an application. The configuration must be provided as an
 *          argument when invoking @ref fs_store and @ref fs_erase.
 *
 * @note    The fields @p p_start_addr and @p p_end_address are set by @ref fs_init, based on the
 *          value of the field @p priority.
 */
typedef struct
{
    /**@brief   The beginning of the flash space assigned to the application which registered this
     *          configuration. This field is set by @ref fs_init.
     */
    uint32_t const * p_start_addr;

    /**@brief   The end of the flash space assigned to the application which registered this
     *          configuration. This field is set by @ref fs_init.
     */
    uint32_t const * p_end_addr;

    fs_cb_t  const   callback;    //!< Callback to run when a flash operation has completed.
    uint8_t  const   num_pages;   //!< The number of flash pages requested.

    /**@brief   The priority with which fstorage should assign flash pages to this application,
     *          with respect to other applications. Applications with higher priority will be
     *          assigned flash pages with a higher memory address. The highest priority is
     *          reserved. Must be unique among configurations.
     */
    uint8_t  const   priority;
} fs_config_t;


/**@brief   Macro for registering with an fstorage configuration.
 *          Applications which use fstorage must register with the module using this macro.
 *          Registering involves defining a variable which holds the configuration of fstorage
 *          specific to the application which invokes the macro.
 *
 * @details This macro places the configuration variable in a section named "fs_data" that
 *          fstorage uses during initialization and regular operation.
 *
 * @param[in]   cfg_var     A @e definition of a @ref fs_config_t variable.
 */
#define FS_REGISTER_CFG(cfg_var) NRF_SECTION_VARS_ADD(fs_data, cfg_var)


/**@brief   Function for initializing the module.
 *
 * @details This functions assigns pages in flash according to all registered configurations.
 *
 * @retval  FS_SUCCESS    If the module was successfully initialized.
 */
fs_ret_t fs_init(void);


/**@brief   Function for storing data in flash.
 *
 * @details Copies @p length_words words from @p p_src to the location pointed by @p p_dest.
 *          If the length of the data exceeds @ref FS_MAX_WRITE_SIZE_WORDS, the data will be
 *          written down in several chunks, as necessary. Only one event will be sent to the
 *          application upon completion. Both the source and the destination of the data must be
 *          word aligned. This function is asynchronous, completion is reported via an event sent
 *          the the callback function specified in the supplied configuration.
 *
 * @warning The data to be written to flash has to be kept in memory until the operation has
 *          terminated, i.e., an event is received.
 *
 * @param[in]   p_config        fstorage configuration registered by the application.
 * @param[in]   p_dest          The address in flash memory where to store the data.
 * @param[in]   p_src           Pointer to the data to store in flash.
 * @param[in]   length_words    Length of the data to store, in words.
 *
 * @retval  FS_SUCCESS              If the operation was queued successfully.
 * @retval  FS_ERR_NOT_INITIALIZED  If the module is not initialized.
 * @retval  FS_ERR_INVALID_CFG      If @p p_config is NULL or contains invalid data.
 * @retval  FS_ERR_NULL_ARG         If @p p_dest or @p p_src are NULL.
 * @retval  FS_ERR_INVALID_ARG      If @p length_words is zero.
 * @retval  FS_ERR_INVALID_ADDR     If @p p_dest or @p p_src are outside of the flash memory
 *                                  boundaries specified in @p p_config.
 * @retval  FS_ERR_UNALIGNED_ADDR   If @p p_dest or @p p_src are not aligned to a word boundary.
 * @retval  FS_ERR_QUEUE_FULL       If the internal operation queue is full.
 */
fs_ret_t fs_store(fs_config_t const * const p_config,
                  uint32_t    const * const p_dest,
                  uint32_t    const * const p_src,
                  uint16_t                  length_words);


/**@brief   Function for erasing flash pages.
 *
 * @details Starting from the page at @p p_page_addr, erases @p num_pages flash pages.
 *          @p p_page_addr must be aligned to a page boundary. All pages to be erased must be
 *          within the bounds specified in the supplied fstorage configuration.
 *          This function is asynchronous. Completion is reported via an event.
 *
 * @param[in]   p_config        fstorage configuration registered by the application.
 * @param[in]   p_page_addr     Address of the page to erase. Must be aligned to a page boundary.
 * @param[in]   num_pages       Number of pages to erase. May not be zero.
 *
 * @retval  FS_SUCCESS              If the operation was queued successfully.
 * @retval  FS_ERR_NOT_INITIALIZED  If the module is not initialized.
 * @retval  FS_ERR_INVALID_CFG      If @p p_config is NULL or contains invalid data.
 * @retval  FS_ERR_NULL_ARG         If @p p_page_addr is NULL.
 * @retval  FS_ERR_INVALID_ARG      If @p num_pages is zero.
 * @retval  FS_ERR_INVALID_ADDR     If the operation would go beyond the flash memory boundaries
 *                                  specified in @p p_config.
 * @retval  FS_ERR_UNALIGNED_ADDR   If @p p_page_addr is not aligned to a page boundary.
 * @retval  FS_ERR_QUEUE_FULL       If the internal operation queue is full.
 */
fs_ret_t fs_erase(fs_config_t const * const p_config,
                  uint32_t    const * const p_page_addr,
                  uint16_t                  num_pages);


/**@brief Function for retrieving the number of queued flash operations.
 *
 * @param[out]  p_op_count    The number of queued flash operations.
 *
 * @retval  FS_SUCCESS          If the number of queued operations was retrieved successfully.
 * @retval  FS_ERR_NULL_ARG     If @p p_op_count is NULL.
 */
fs_ret_t fs_queued_op_count_get(uint32_t * const p_op_count);


/**@brief   Function for handling system events from the SoftDevice.
 *
 * @details If any of the modules used by the application rely on fstorage, the application should
 *          dispatch system events to fstorage using this function.
 *
 * @param[in]   sys_evt     System event from the SoftDevice.
 */
void fs_sys_event_handler(uint32_t sys_evt);


/** @} */

#endif // FSTORAGE_H__
