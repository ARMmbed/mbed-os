/*
 * Copyright (c) 2013-2017, Pelion and affiliates.
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
 */
#ifndef _NS_RPL_H
#define _NS_RPL_H

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file net_rpl.h
 * \brief ZigBeeIP Router and Border Router RPL API.
 *
 * This API is primarily used with a border router. You can also use it with a basic router.
 *
 * - arm_nwk_6lowpan_rpl_memory_limit_set(), Set RPL global memory limit sizes. Call this only at boot ones.
 * - arm_nwk_6lowpan_rpl_dodag_init(), Allocate and init RPL DODAG root.
 * - arm_nwk_6lowpan_rpl_dodag_remove(), Remove DDAG root, specifically.
 * - arm_nwk_6lowpan_rpl_dodag_start(), Activate RPL DODAG instance.
 * - arm_nwk_6lowpan_rpl_dodag_poison(), Trigger some poison advertisements for the current instance, and then become a leaf (so no more adverts are sent). If a DODAG root, this remains allocated so arm_nwk_6lowpan_rpl_dodag_remove() still needs to be called to free it, after delaying some seconds to allow the poison advertisements to be sent().
 * - arm_nwk_6lowpan_rpl_dodag_prefix_update(), Update prefix info to the root.
 * - arm_nwk_6lowpan_rpl_dodag_route_update(), Update route info to the root.
 * - arm_nwk_6lowpan_rpl_dodag_dao_trig(), Increment the DAO Trigger Sequence Number (DTSN), to cause downstream nodes to refresh their Destination Advertisement Objects (DAOs).
 * - arm_nwk_6lowpan_rpl_dodag_version_increment(), Increment the DODAG version to trigger a global DODAG repair.
 *
 * \section rpl-recommend RECOMMEND API for router and border router:
 * - rpl_instance_list_read(), Read active RPL instance list.
 * - rpl_read_dodag_info(), Read RPL DODAG information to rpl_dodag_info_t structure by selected RPL instance ID.
 *
 * \section rpl-dodag-init Steps to define a new RPL DODAG instance:
 *  1. Allocate RPL root base with arm_nwk_6lowpan_rpl_dodag_init().
 *  2. Set prefix 1 to root with arm_nwk_6lowpan_rpl_dodag_prefix_update().
 *  3. Set Route(s) to root with arm_nwk_6lowpan_rpl_dodag_route_update().
 *  4. Start Activate RPL DODAG:
 *   * arm_nwk_6lowpan_rpl_dodag_start() if RPL is generated after arm_nwk_interface().
 *
 */

/** \name DoDag Root setups
 * <B>DODAGPreference (Prf):</B> A 3-bit unsigned integer that defines how
         preferable the root of this DODAG is compared to other DODAG
         roots within the instance. DAGPreference ranges from 0x00
         (least preferred) to 0x07 (most preferred). The default is 0
         (least preferred).
 *
 * @{
 */
#define RPL_DODAG_PREF_MASK         0x07                          /**< Preference mask */
#define RPL_DODAG_PREF(n)           ((n) & RPL_DODAG_PREF_MASK)   /**< DODAG preference */
/** @} */


/** \name Mode of Operation (MOP)
 * The Mode of Operation (MOP) field identifies
   the mode of operation of the RPL instance as administratively
   provisioned at and distributed by the DODAG root. All nodes
   joining the DODAG must be able to honor the MOP to
   fully participate as a router. Otherwise, they must only join as a leaf.
 * @{
 */
#define RPL_MODE_MASK               0x38  /**< MOP mask */
#define RPL_MODE_SHIFT              3     /**< shift count */
#define RPL_MODE_NO_DOWNWARD        0x00  /**< No Downward routes maintained by RPL */
#define RPL_MODE_NON_STORING        0x08  /**< Non-Storing Mode of Operation */
#define RPL_MODE_STORING            0x10  /**< Storing Mode of Operation with no multicast support */
#define RPL_MODE_STORING_MULTICAST  0x18  /**< Storing Mode of Operation with multicast support */
#define RPL_MODE_P2P_DISCOVERY      0x20  /**< RFC 6997 */
/** @} */

/** Grounded (G): The Grounded 'G' flag indicates whether the DODAG
   advertised can satisfy the application-defined goal. If the
   flag is set, the DODAG is grounded. If the flag is cleared,
   the DODAG is floating.
*/
#define RPL_GROUNDED                0x80

/** \name FOR BACKWARDS COMPATIBILITY
 * @{
 */
#define BR_DODAG_PREF_0             RPL_DODAG_PREF(0)     /**< backward compatibility */
#define BR_DODAG_PREF_1             RPL_DODAG_PREF(1)     /**< backward compatibility */
#define BR_DODAG_PREF_2             RPL_DODAG_PREF(2)     /**< backward compatibility */
#define BR_DODAG_PREF_3             RPL_DODAG_PREF(3)     /**< backward compatibility */
#define BR_DODAG_PREF_4             RPL_DODAG_PREF(4)     /**< backward compatibility */
#define BR_DODAG_PREF_5             RPL_DODAG_PREF(5)     /**< backward compatibility */
#define BR_DODAG_PREF_6             RPL_DODAG_PREF(6)     /**< backward compatibility */
#define BR_DODAG_PREF_7             RPL_DODAG_PREF(7)     /**< backward compatibility */
#define BR_DODAG_MOP_NON_STORING    RPL_MODE_NON_STORING  /**< backward compatibility */
#define BR_DODAG_MOP_STORING        RPL_MODE_STORING      /**< backward compatibility */
#define BR_DODAG_FLOATING           0                     /**< backward compatibility */
#define BR_DODAG_GROUNDED           RPL_GROUNDED          /**< backward compatibility */
/** @} */

/** \name Compatibility for even older misspellings
 * @{
 */
#define BR_DODAG_MOP_NON_STRORING   BR_DODAG_MOP_NON_STORING    /**< backward compatibility */
#define BR_DODAG_MOP_STRORING       BR_DODAG_MOP_STORING        /**< backward compatibility */
#define BR_DODAG_FLOATIN            BR_DODAG_FLOATING           /**< backward compatibility */
/** @} */

/** RPL ROOT parent flag */
#define RPL_ROOT_PARENT             0
/** RPL primary active primary_parent primary_parent_rank information is valid. */
#define RPL_PRIMARY_PARENT_SET      1
/** RPL secondary active secondary_parent secondary_parent_rank information is valid. */
#define RPL_SECONDARY_PARENT_SET    2

/** RPL prefix update flags for A-flag AUTONOMOUS address generation. */
#define RPL_PREFIX_AUTONOMOUS_ADDRESS_FLAG 0x40
/** RPL prefix update flags for R-Flag. */
#define RPL_PREFIX_ROUTER_ADDRESS_FLAG 0x20

/** Flag in RPL instance ID to indicate that it is a local instance. */
#define RPL_INSTANCE_LOCAL          0x80

/*!
 * \struct rpl_dodag_info_t
 * \brief RPL Instance DODAG info structure for rpl_read_dodag_info. Read RFC 6550 for more information and to make sure you know what you are doing.
 */
typedef struct rpl_dodag_info_t {
    uint8_t dodag_id[16];           /**< RPL DODAG ID. */
    uint8_t instance_id;            /**< RPL instance ID. */
    uint8_t flags;                  /**< RPL DODAG Flags: (MOP,Grounded, Router Pref) */
    uint8_t version_num;            /**< RPL DODAG version number. */
    uint8_t DTSN;                   /**< RPL DODAG DAO trigger version number. */
    uint16_t curent_rank;           /**< RPL DODAG node current Rank. */
    uint8_t parent_flags;           /**< RPL DODAG parent Flags: RPL_ROOT_PARENT or RPL_PRIMARY_PARENT_SET, RPL_SECONDARY_PARENT_SET */
    uint8_t primary_parent[16];     /**< Primary Parent GP address if RPL_PRIMARY_PARENT_SET flag is active. */
    uint16_t primary_parent_rank;   /**< Primary Parent Rank if RPL_PRIMARY_PARENT_SET flag is active. */
    uint8_t secondary_parent[16];   /**< Secondary Parent GP address if RPL_SECONDARY_PARENT_SET flag is active. */
    uint16_t secondary_parent_rank; /**< Secondary Parent rank if RPL_SECONDARY_PARENT_SET flag is active. */
    uint16_t dag_min_hop_rank_inc;  /**< RPL DODAG conf DAG minimum rank increase. */
} rpl_dodag_info_t;

/*!
 * \struct dodag_config_t
 * \brief RPL DODAG config is used when allocating RPL base arm_nwk_6lowpan_rpl_dodag_init().
 */
typedef struct dodag_config_t {
    uint8_t DAG_SEC_PCS;        /**< Path Control Size limits number of DAO parents. Possible values are 0-7, meaning 1-8 parents.
(This also has an Authentication flag 0x10 – the “SEC”, but we don’t support that, and 4 reserved flags, so it is just the 3-bit PCS field). */
    uint8_t DAG_DIO_INT_DOUB;   /**< RPL Trickle DIOIntervalDoublings, recommended value 12. */
    uint8_t DAG_DIO_INT_MIN;    /**< RPL Trickle DIOIntervalMin, recommended value 9. */
    uint8_t DAG_DIO_REDU;       /**< RPL Trickle DIORedundancyConstant, recommended value 3. */
    uint16_t DAG_MAX_RANK_INC;  /**< RPL MaxRankIncrease, recommended value 2048. */
    uint16_t DAG_MIN_HOP_RANK_INC;  /**< RPL MinHopRankIncrease, recommended value 128. */
    uint16_t DAG_OCP;           /**< Objective code point, recommended value 1. */
    uint8_t LIFE_IN_SECONDS;    /**< Default lifetime for RPL routes, expressed in lifetime units. */
    uint16_t LIFETIME_UNIT;     /**< Defines the unit used to express route lifetimes, in seconds. */
} dodag_config_t;

/**
  * \brief RPL DODAG root base allocate.
  *
  * \param interface_id Interface ID
  * \param dodag_id A pointer to unique DODAGID. This must be the node's GP address in the ZigBeeIP network.
  * \param config A pointer to the DODAG configure structure.
  * \param instace_id Instance ID for RPL DODAG.
  * \param flags Defines RPL MOP and DODAG pref. ZigBeeIP should use (BR_DODAG_MOP_NON_STORING | BR_DODAG_PREF_7).
  *
  * \return 0, Allocate OK.
  * \return -1, Allocate fail (The node is already connected to the same instance or the memory allocate fails).
  * \return -2, DODAG configuration parameter failure.
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_init(int8_t interface_id, const uint8_t *dodag_id, const dodag_config_t *config, uint8_t instace_id, uint8_t flags);

/**
  * \brief RPL Global memory size limits.
  *
  * Calling this function you can update default memory limits. Soft default is 1024 and hard limit is 2048.
  * \param soft_limit When RPL reach this state at total allocation it start cleaning unused data.
  * \param hard_limit Total allocation limit. 0 means no limit and > 0 define hard limit. When hard limit > 0 soft_limit must be smaller than hard.
  *
  * \return 0, Set OK.
  * \return -1, Unsupported parameter
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_memory_limit_set(size_t soft_limit, size_t hard_limit);
/**
  * \brief RPL DODAG remove by given interface ID.
  *
  * \param interface_id Interface ID for removed DODAG.
  *
  * \return 0, Remove OK.
  * \return -1, Remove fail.
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_remove(int8_t interface_id);

/**
  * \brief Activate RPL DODAG by given Interface ID.
  *
  * \param interface_id Interface ID for start DODAG root.
  *
  * \return 0, Start OK.
  * \return <0, Start fail.
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_start(int8_t interface_id);

/**
  * \brief RPL prefix information update.
  *
  * \param interface_id Root interface ID
  * \param prefix_ptr A pointer to IPv6 prefix (16-bytes).
  * \param prefix_len Prefix length (should be 64).
  * \param flags  Define R-flag (RPL_PREFIX_ROUTER_ADDRESS_FLAG), A-flag (RPL_PREFIX_AUTONOMOUS_ADDRESS_FLAG).
  * \param lifetime Prefix lifetime.
  *
  *
  * \return 0, Update OK.
  * \return <0, Update fail.
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_prefix_update(int8_t interface_id, uint8_t *prefix_ptr, uint8_t prefix_len, uint8_t flags, uint32_t lifetime);
/**
  * \brief RPL route information update.
  *
  * \param interface_id Root interface ID.
  * \param route_ptr A pointer to IPv6 prefix (16-bytes).
  * \param prefix_len Prefix length (should be 64).
  * \param flags Define R-flag (RPL_PREFIX_ROUTER_ADDRESS_FLAG).
  * \param lifetime Route lifetime.
  *
  *
  * \return 0, Update OK.
  * \return <0, Update fail.
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_route_update(int8_t interface_id, uint8_t *route_ptr, uint8_t prefix_len, uint8_t flags, uint32_t lifetime);

/**
  * \brief RPL DODAG poison.
  *
  * This function poisons the current RPL instance. After a few seconds arm_nwk_6lowpan_rpl_dodag_remove() can also clean the root.
  *
  * \param interface_id Interface ID that defines the RPL instance to be poisoned.
  *
  *
  * \return 0, Poison OK.
  * \return <0, Poison fail.
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_poison(int8_t interface_id);
/**
  * \brief Trigger RPL DODAG DAO by DTSN increment.
  *
  *
  * \param interface_id Network interface ID.
  *
  *
  * \return 0, DAO trig OK
  * \return <0, DAO trig Fail
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_dao_trig(int8_t interface_id);
/**
  * \brief RPL DODAG version update.
  *
  * Network devices need to reset the current RPL instance and do unicast DIS/DIO and DAO/DAO ACK handshake.
  *
  * \param interface_id Root interface ID.
  *
  *
  * \return 0, Version update OK.
  * \return <0, Version update fail.
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_version_increment(int8_t interface_id);
/**
  * \brief Read RPL instance list of a node.
  *
  * Global instances are output as a single byte containing the instance ID;
  * local instances are output as the instance ID followed by the 16-byte DODAG ID.
  *
  * \param buffer_ptr A pointer to the location of the instance IDs.
  * \param buffer_size Instance list buffer size.
  *
  * \return RPL instance count (not necessarily number of bytes, if local instances).
  *
  */
extern uint8_t rpl_instance_list_read(uint8_t *buffer_ptr, uint8_t buffer_size);
/**
  * \brief Read DODAG information by given RPL instance ID.
  *
  * If it is a local instance ID, dodag_ptr must contain the DODAG ID on entry.
  *
  * \param dodag_ptr A pointer to DODAG information structure.
  * \param instance_id Read instance ID.
  *
  * \return 1, Read OK.
  * \return 0, Read fail.
  *
  */
extern uint8_t rpl_read_dodag_info(rpl_dodag_info_t *dodag_ptr, uint8_t instance_id);
/**
  * \brief RPL DODAG preference set.
  *
  * \param interface_id Interface ID in which the RPL instance updates the DODAG preference.
  * \param preference DODAG preference. 0 to 7. 0 is least preferred.
  *
  * \return 0, Update OK
  * \return <0, Update Fail
  *
  */
extern int8_t arm_nwk_6lowpan_rpl_dodag_pref_set(int8_t interface_id, uint8_t preference);

#ifdef __cplusplus
}
#endif
#endif /*_NS_RPL_H*/
