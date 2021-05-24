/*****************************************************************************
 * @file    ble_bufsize.h
 * @author  MCD
 * @brief   Definition of BLE stack buffers size
 *****************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
 *****************************************************************************
 */

#ifndef BLE_BUFSIZE_H__
#define BLE_BUFSIZE_H__


/*
 * BLE_DEFAULT_ATT_MTU: minimum MTU value that GATT must support.
 */
#define BLE_DEFAULT_ATT_MTU                  23

/*
 * BLE_DEFAULT_MAX_ATT_SIZE: maximum attribute size.
 */
#define BLE_DEFAULT_MAX_ATT_SIZE            512

/*
 * BLE_PREP_WRITE_X_ATT: compute how many Prepare Write Request are needed to
 * write a characteristic with size 'max_att' when the used ATT_MTU value is
 * equal to BLE_DEFAULT_ATT_MTU (23).
 */
#define BLE_PREP_WRITE_X_ATT(max_att) \
          (DIVC(max_att, BLE_DEFAULT_ATT_MTU - 5) * 2)

/*
 * BLE_DEFAULT_PREP_WRITE_LIST_SIZE: default minimum Prepare Write List size.
 */
#define BLE_DEFAULT_PREP_WRITE_LIST_SIZE \
          BLE_PREP_WRITE_X_ATT(BLE_DEFAULT_MAX_ATT_SIZE)

/*
 * BLE_MEM_BLOCK_X_MTU: compute how many memory blocks are needed to compose
 * an ATT packet with ATT_MTU=mtu.
 */
#define BLE_MEM_BLOCK_SIZE                   32

#define BLE_MEM_BLOCK_X_TX(mtu) \
          (DIVC((mtu) + 4U, BLE_MEM_BLOCK_SIZE) + 1U)

#define BLE_MEM_BLOCK_X_RX(mtu, n_link) \
          ((DIVC((mtu) + 4U, BLE_MEM_BLOCK_SIZE) + 2U) * (n_link) + 1)

#define BLE_MEM_BLOCK_X_MTU(mtu, n_link) \
          (BLE_MEM_BLOCK_X_TX(mtu) + BLE_MEM_BLOCK_X_RX(mtu, n_link))

/*
 * BLE_MBLOCKS_SECURE_CONNECTIONS: minimum number of blocks required for
 * secure connections
 */
#define BLE_MBLOCKS_SECURE_CONNECTIONS        4

/*
 * BLE_MBLOCKS_CALC: minimum number of buffers needed by the stack.
 * This is the minimum racomanded value and depends on:
 *  - pw: size of Prepare Write List
 *  - mtu: ATT_MTU size
 *  - n_link: maximum number of simultaneous connections
 */
#define BLE_MBLOCKS_CALC(pw, mtu, n_link) \
          ((pw) + MAX(BLE_MEM_BLOCK_X_MTU(mtu, n_link), \
                      BLE_MBLOCKS_SECURE_CONNECTIONS))

/*
 * BLE_FIXED_BUFFER_SIZE_BYTES:
 * A part of the RAM, is dinamically allocated by initilizing all the pointers 
 * defined in a global context variable "mem_alloc_ctx_p".
 * This initialization is made in the Dynamic_allocator functions, which 
 * assing a portion of RAM given by the external application to the above
 * mentioned "global pointers".
 *
 * The size of this Dynamic RAM is made of 2 main components: 
 * - a part that is parameters-dependent (num of links, GATT buffers, ...),
 *   and which value is explicited by the following macro;
 * - a part, that may be considered "fixed", i.e. independent from the above
 *   mentioned parameters.
*/
#if (BEACON_ONLY != 0)
#define BLE_FIXED_BUFFER_SIZE_BYTES  6212   /* Beacon only */
#elif (LL_ONLY != 0)
#define BLE_FIXED_BUFFER_SIZE_BYTES  6272   /* LL only */
#elif (SLAVE_ONLY != 0)
#define BLE_FIXED_BUFFER_SIZE_BYTES  6712   /* Peripheral only */
#elif (BASIC_FEATURES != 0)
#define BLE_FIXED_BUFFER_SIZE_BYTES  6972   /* Basic Features */
#else
#define BLE_FIXED_BUFFER_SIZE_BYTES  7240   /* Full stack */
#endif

/*
 * BLE_PER_LINK_SIZE_BYTES: additional memory size used per link
 */
#if (BEACON_ONLY != 0)
#define BLE_PER_LINK_SIZE_BYTES       148   /* Beacon only */
#elif (LL_ONLY != 0)
#define BLE_PER_LINK_SIZE_BYTES       196   /* LL only */
#elif (SLAVE_ONLY != 0)
#define BLE_PER_LINK_SIZE_BYTES       332   /* Peripheral only */
#elif (BASIC_FEATURES != 0)
#define BLE_PER_LINK_SIZE_BYTES       332   /* Basic Features */
#else
#define BLE_PER_LINK_SIZE_BYTES       384   /* Full stack */
#endif

/*
 * BLE_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes,
 * needed for the storage of data structures (except GATT database elements)
 * whose size depends on the number of supported connections.
 *
 * @param n_link: Maximum number of simultaneous connections that the device
 * will support. Valid values are from 1 to 8.
 *
 * @param mblocks_count: Number of memory blocks allocated for packets.
 */
#define BLE_TOTAL_BUFFER_SIZE(n_link, mblocks_count) \
          (BLE_FIXED_BUFFER_SIZE_BYTES + \
           (BLE_PER_LINK_SIZE_BYTES * (n_link)) + \
           ((BLE_MEM_BLOCK_SIZE + 12) * (mblocks_count)))

/*
 * BLE_TOTAL_BUFFER_SIZE_GATT: this macro returns the amount of memory,
 * in bytes, needed for the storage of GATT database elements.
 *
 * @param num_gatt_attributes: Maximum number of Attributes (i.e. the number
 * of characteristic + the number of characteristic values + the number of
 * descriptors, excluding the services) that can be stored in the GATT
 * database. Note that certain characteristics and relative descriptors are
 * added automatically during device initialization so this parameters should
 * be 9 plus the number of user Attributes
 *
 * @param num_gatt_services: Maximum number of Services that can be stored in
 * the GATT database. Note that the GAP and GATT services are automatically
 * added so this parameter should be 2 plus the number of user services
 *
 * @param att_value_array_size: Size of the storage area for Attribute values.
  */
#define BLE_TOTAL_BUFFER_SIZE_GATT(num_gatt_attributes, num_gatt_services, att_value_array_size) \
          (((((att_value_array_size) - 1) | 3) + 1) + \
           (40 * (num_gatt_attributes)) + (48 * (num_gatt_services)))


#endif /* BLE_BUFSIZE_H__ */
