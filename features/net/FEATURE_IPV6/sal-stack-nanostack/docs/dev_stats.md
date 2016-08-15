# Device statistics

## dev_stat_internal_init

To start collecting device statistics within the device statistics structure:

```
int8_t dev_stat_internal_init
(
	dev_stat_t	*info_ptr
)
```

Parameter|Description
---------|-----------
`info_ptr`|A pointer to the device statistics structure on application.

<dl>
<dt>Return value</dt>
<dd>0 Success.</dd>

<dd>-1 Fail.</dd>
</dl>


### dev_stat_get_longest_heap_sector

The following function call returns the longest available free sector size in the 6LoWPAN stack dynamic memory:

```
int16_t dev_stat_get_longest_heap_sector
(
	void
)
```

<dl>
<dt>Return value</dt>
<dd>The size of the longest free sector.</dd>
</dl>


### dev_stat_get_runtime_seconds

The following function call returns the runtime since last reset:

```
uint32_t dev_stat_get_runtime_seconds
(
	void
)
```

<dl>
<dt>Return value</dt>
<dd>The runtime since last reset.</dd>
</dl> 
