#ifndef MBED_INIT_MODE_FLAGS_H
#define MBED_INIT_MODE_FLAGS_H

#include <stdint.h>
#include "mbed_enum_flags.h"

namespace mbed {

/**
 * @brief A set of creation flags for the KVStore instance.
 * 
 * The Read, Write, and ReadWrite flags may be OR-ed to produce the correct initialization
 * sequence. This is similar to how a file is opened.
 * 
 * By default, the init mode opens in ReadWrite and Append mode as the default argument.
 * 
 * At least one of Read, Write, or ReadWrite must be specified. Additionally, at least one
 * of the following must be specified with write access: Append, Truncate, CreateNewOnly, or
 * ExclusiveCreation.
 * 
 */
MBED_SCOPED_ENUM_FLAGS(InitModeFlags) {
    Read                    = (1 << 0),                 //!< Enable read access from the KVStore
    Write                   = (1 << 1),                 //!< Enable write access to the KVStore
    ReadWrite               = ((1 << 0) | (1 << 1)),    //!< Enable read and write access to the KVSTore. This is the default.
    WriteOnlyAllowKeyRead   = (1 << 3),                 //!< Allow reading KVStore keys even in write only mode
    Append                  = (1 << 8),                 //!< Allow adding to the the KVStore and create from new if necessary. This is the default.
    Truncate                = (1 << 9),                 //!< Erase all key/value pairs before using.
    CreateNewOnly           = (1 << 10),                //!< Only open the KVStore if it does not already exist.
    ExclusiveCreation       = (1 << 11),                //!< Only open the KVStore if it already exists.

#if !defined(DOXYGEN_ONLY)
    // These are for interal use only
    WriteOpenFlags      = 0xf00,
    NoFlags             = 0,
    AllFlags            = 0xf07

#endif // DOXYGEN_ONLY
};

}

#endif // MBED_INIT_MODE_FLAGS_H