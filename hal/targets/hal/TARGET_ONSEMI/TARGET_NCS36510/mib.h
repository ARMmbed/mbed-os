/**
 ******************************************************************************
 * @file mib.h
 * @brief Defines the structure of a Management Information Base (MIB)
 * @internal
 * @author ON Semiconductor
 * $Rev: 2284 $
 * $Date: 2013-09-12 15:08:22 +0530 (Thu, 12 Sep 2013) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @details
 * This file defines the structure, as seen from the outside, of any Management
 * Information Base (MIB) implementation. It does not define the contents of the
 * MIB.
 *
 * A MIB is implemented as a map of parameters, each identified by a unique Object ID.
 * The contents of the map have to be filled in by the actual MIB implementation, by
 * assigning to the GlobMibMap variable. The mib_param_t data type defines the format
 * of each parameter in the map. *NOTE:* The length of the last entry in the map must
 * be 0.
 *
 * One parameter must always be present in the MIB map: the system revision. It has
 * to be set by assigning a value to the systemRevision variable.
 *
 * Parameters can be of any data type, and read / write or read only. To resemble
 * the hierarchical structure of an SNMP MIB, each Object ID consists of four nibbles
 * (uint16_t) where the more significant nibbles correspond to a higher level in the
 * hierarchy.
 *
 * The contents of the map can be accessed with the fMibSetBytes() and fMibGetBytes()
 * functions. The implementation of the MIB may also make its parameters accessible
 * in a more "direct" way; that is left to the implementation. The fMibSetBytes() and
 * fMibGetBytes() functions are made available to the user interface (see ui.h). In
 * order to do so, fMibUiInit() must be called during device initialization.
 *
 * Additionally, for parameters that have an array as their value, the individual
 * elements of the array can be accessed using the fMibIndexedSetBytes() and
 * fMibIndexedGetBytes() functions. The functions are similar to their non-indexed
 * variants (fMibSetBytes() and fMibGetBytes()), except that they expect the index of
 * the element as an argument.
 *
 * fMibGetBytes() and fMibSetBytes() will normally copy bytes from the MIB parameter
 * into a given place in memory, or from memory to the MIB parameter. This behaviour
 * can be overruled by assigning a function to the setAction and / or getAction
 * fields of the MIB parameter.
 *
 * For parameters that have an array as their value, the length field has a slightly
 * different meaning: the most significant 16 bits contain the length of the array;
 * the least significant 16 bits contain the width of the array (i.e. the size in
 * bytes of the array's elements). This implies that the normal fMibGetBytes() and
 * fMibSetBytes() cannot be used on these parameters; instead, a get and set action
 * needs to be provided to interpret the length field in case of a non-indexed get
 * or set.
 *
 * To access the MIB fields over the user interface, the module ID must be equal to
 * MIB_MODULE_ID or 0x01. The data in the packet must have the following structure:
 * <table>
 *  <tr>
 *   <th>Get (0x00) / Set (0x01) code</th>
 *   <th>Object ID</th>
 *   <th>Value (only for set)</th>
 *  </tr>
 *  <tr>
 *   <td>1 Byte</td>
 *   <td>2 Bytes</td>
 *   <td>X bytes</td>
 *  </tr>
 * </table>
 *
 * <table>
 *  <tr>
 *   <th>Indexed Get (0x02) / Set (0x03) code</th>
 *   <th>Object ID</th>
 *   <th>Index</th>
 *   <th>Value (only for set)</th>
 *  </tr>
 *  <tr>
 *   <td>1 Byte</td>
 *   <td>2 Bytes</td>
 *   <td>2 Bytes</td>
 *   <td>X bytes</td>
 *  </tr>
 * </table>
 *
 * The reply to this request will have the same structure, with the value always
 * being present (in case of a Set as well as a Get request).
 *
 * In case an error occurs during a Set or Get request, an error is returned by
 * fMibGetBytes(), fMibSetBytes() or the UI. See the UI Module's documentation for
 * the format of an error reply. The applicable error codes are:
 * <table>
 *  <tr><th>0x01</th><td>Trying to set a read-only parameter.</td></tr>
 *  <tr><th>0x02</th><td>Requested value out of range.</td></tr>
 *  <tr><th>0x03</th><td>Object ID is unknown.</td></tr>
 *  <tr><th>0x04</th><td>Provided index is incorrect.</td></tr>
 * </table>
 *
 * @ingroup mib
 */

#ifndef MIB_H_
#define MIB_H_


#include <stdint.h>

#include "types.h"
#include "error.h"

/** A structure defining the format of the system revision parameter. */
typedef struct mib_systemRevision {
    uint8_t hardwareRevision;
    uint8_t patchLevel;
    uint8_t bugFix;
    uint8_t featureSet;
    uint8_t generation;
    uint8_t release;
} mib_systemRevision_t, *mib_systemRevision_pt;

/** The system revision. */
extern const mib_systemRevision_t systemRevision;


#endif /* MIB_H_ */
