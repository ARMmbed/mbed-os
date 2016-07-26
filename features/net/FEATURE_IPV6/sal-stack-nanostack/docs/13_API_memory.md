Dynamic Memory API
==================
This chapter introduces the dynamic memory API of the 6LoWPAN stack. It contains the following sections:

- [_About dynamic memory API_](#about-dynamic-memory-api)
- [_Initialization, allocation and freeing_](#initialization-allocation-and-freeing)
- [_Failure callback and reporting_](#failure-callback-and-reporting)

## API Header

```
#include nsdynmemLIB.h
```

## About dynamic memory API

This section introduces the dynamic memory API of the 6LoWPAN stack. Using this API, the application can specify the size of the 6LoWPAN stack heap to minimize the _Random Access Memory_ (RAM) usage.

It is not recommended to use heap size less than 2.5KB since the reliability of transmitting full-size packets will become compromised.

Depending on the network configuration and topology, a higher heap size is recommended.

The application can also allocate memory from the 6LoWPAN stack heap and receive failure reports in case of usage errors.

## Initialization, allocation and freeing

This section describes several APIs that allow you to initialize the 6LoWPAN stack memory block and allocate memory from the initialized block.

### Dynamic memory initialization

To initialize the 6LoWPAN stack dynamic memory, use the following function call:

```
void ns_dyn_mem_init
(
	uint8_t		*heap_ptr,
	uint16_t	heap_size
)
```

Parameter|Description
---------|-----------
`heap_ptr`|A pointer to data memory block that is reserved for the 6LoWPAN stack.
`heap_size`|The size of the 6LoWPAN stack dynamic memory.

### Dynamic memory temporary allocation

To allocate memory temporarily from the 6LoWPAN stack heap, use the following function call:

```
void *ns_dyn_mem_temporary_alloc
(
	int16_t		alloc_size
)
```

Parameter|Description
---------|-----------
`alloc_size`|Size of the allocation.

<dl>
<dt>Return value</dt>
<dd>0 Allocation fail.</dd>
<dd>>0 Pointer to allocated memory.</dd>
</dl>

### Dynamic memory long period allocation

To allocate memory from the 6LoWPAN stack heap for longer-term use, use the following function call:

```
void *ns_dyn_mem_alloc
(
	int16_t		alloc_size
)
```

Parameter|Description
---------|-----------
`alloc_size`|Size of the allocation.

<dl>
<dt>Return value</dt>
<dd>0 Allocation fail.</dd>
<dd>>0 Pointer to allocated memory.</dd>
</dl>

### Dynamic memory freeing

To free allocated memory from the 6LoWPAN stack heap, use the following function call:

```
void ns_dyn_mem_free
(
	void	*heap_ptr
)
```

Parameter|Description
---------|-----------
`heap_ptr`|A pointer to the start of the allocated memory.

## Failure callback and reporting 

With the failure callback and delivered failure report, a developer can debug possible usage errors with the 6LoWPAN stack dynamic memory.

### Dynamic memory failure callback

To set a callback function for the heap failure report of the 6LoWPAN stack, use the following function:

```
void ns_dyn_mem_heap_fail_callback
(
	void (*passed_fptr)(heap_fail_t)
)
```

Parameter|Description
---------|-----------
`passed_fptr`|A pointer to the callback function of the 6LoWPAN stack memory failure on application.

### Dynamic memory failure report events

_Table 3-26_ lists the possible events of the dynamic memory failure report.

**Table 3-26 Possible events of the memory failure report**

Event Type|Description
----------|-----------
`NS_DYN_MEM_NULL_FREE`|Null pointer free detected.
`NS_DYN_MEM_DOUBLE_FREE`|Double free detected.
`NS_DYN_MEM_ALLOCATE_SIZE_NOT_VALID`|Allocation size not valid.
`NS_DYN_MEM_POINTER_NOT_VALID`|Invalid pointer detected while freeing memory.
`NS_DYN_MEM_HEAP_SECTOR_CORRUPTED`|Heap sector corruption detected.
`NS_DYN_MEM_HEAP_SECTOR_UNITIALIZED`|Detected allocation attempt before dynamic memory is initialized.

