/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */

/**
 * NanoStack NVM helper functions to read, write and delete key-value pairs to platform NVM.
 *
 * Client can use following methods:
 * -ns_nvm_data_write to write data to a key in platform NVM
 * -ns_nvm_data_read to read data from key in platform NVM
 * -ns_nvm_key_delete to delete a key from platform NVM
 *
 * If a API call returns NS_NVM_OK then a provided callback function will be called
 * and status argument indicates success or failure of the operation. If API call
 * returns error then callback will not be called.
 *
 * When client writes data this module will:
 * -initialize the NVM if not already initialized
 * -(re)create the key with a given size
 * -write data to the key
 * -flush data to the NVM
 *
 * When client reads data this module will:
 * -initialize the NVM if not initialized
 * -read data from the key
 *
 * When client deletes a key this module will:
 * -initialize the NVM if not initialized
 * -delete the key from NVM
 */

/*
 * API function and callback function return statuses
 */
#define NS_NVM_OK               0
#define NS_NVM_DATA_NOT_FOUND   -1
#define NS_NVM_ERROR            -2
#define NS_NVM_MEMORY           -3

/**
 *  callback type for NanoStack NVM
 */
typedef void (ns_nvm_callback)(int status, void *context);

/**
 * \brief Delete key from NVM
 *
 * \param callback function to be called when key deletion is ready
 * \param key_name Name of the key to be deleted from NVM
 * \param context argument will be provided as an argument when callback is called
 *
 * \return NS_NVM_OK if key deletion is in progress and callback will be called
 * \return NS_NVM_ERROR in error case, callback will not be called
 * \return provided callback function will be called with status indicating success or failure.
 */
int ns_nvm_key_delete(ns_nvm_callback *callback, const char *key_name, void *context);

/**
 * \brief Read data from NVM
 *
 * \param callback function to be called when data is read
 * \param key_name Name of the key whose data will be read
 * \param buf buffer where data will be stored
 * \param buf_len address of variable containing provided buffer length
 * \param context argument will be provided as an argument when callback is called
 *
 * \return NS_NVM_OK if read is in progress and callback will be called
 * \return NS_NVM_ERROR in error case, callback will not be called
 * \return provided callback function will be called with status indicating success or failure.
 */
int ns_nvm_data_read(ns_nvm_callback *callback, const char *key_name, uint8_t *buf, uint16_t *buf_len, void *context);

/**
 * \brief Write data to NVM
 *
 * \param callback function to be called when data writing is completed
 * \param key_name Name of the key whose data will be read
 * \param buf buffer where data will be stored
 * \param buf_len address of variable containing provided buffer length
 * \param context argument will be provided as an argument when callback is called
 *
 * \return NS_NVM_OK if read is in progress and callback will be called
 * \return NS_NVM_ERROR in error case, callback will not be called
 * \return provided callback function will be called with status indicating success or failure.
 */
int ns_nvm_data_write(ns_nvm_callback *callback, const char *key_name, uint8_t *buf, uint16_t *buf_len, void *context);
