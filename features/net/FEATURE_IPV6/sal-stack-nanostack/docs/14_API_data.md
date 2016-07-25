Data Structures, Types and Variables
====================================
This chapter describes data structure, types and variables used in the socket and RF layers. It contains the following sections:

- [_Basic data types_](#basic-data-types)
- [_Socket address type definition_](#socket-address-type-definition)
- [_Address type_](#address-type)

## API Headers

```
#include ns_types.h
#include ns_addres.h
```

## Basic data types

The basic data types used are:

Data type|Description
---------|-----------
`uint8_t`|An unsigned 8-bit integer.
`int8_t`|A signed 8-bit integer.
`uint16_t`|An unsigned 16-bit integer.
`int16_t`|A signed 16-bit integer.
`uint32_t`|An unsigned 32-bit integer.
`int32_t`|A signed 32-bit integer.

## Socket address type definition

This structure defines the socket address type and has the following members:

```
typedef struct ns_address_t
{
    address_type_t	type;
    uint8_t			address[16];
    uint16_t		identifier;
}ns_address_t;
```

Member|Description
------|-----------
`type`|The address type.
`address`|The address data in the format defined by type.
`identifier`|The port for TCP/UDP, message ID for ICMP.
</dl>

## Address type

This enumeration defines the address types:

```
typedef enum address_type_t
{
	ADDRESS_IPV6,
	ADDRESS_IPV4,
	ADDRESS_TUN_DRIVER_ID
}address_type_t;
```

Parameter|Description
---------|-----------
`ADDRESS_IPV6`|IPv6 address type. The address is in 128-bit binary form.
`ADDRESS_IPV4`|IPv4 address type. The address is in 32-bit binary form.
`ADDRESS_TUN_DRIVER_ID`|Local socket address type. The address is an 8-bit driver ID for packet source.

## Address type

This constant defines the IPv6 address INADDR_ANY:

```
extern const uint8_t ns_in6addr_any[16];
```

