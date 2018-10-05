# NVStore

NVStore is a lightweight module that stores data by keys in the internal flash for security purposes. 

## Description

NVStore provides the ability to store a minimal set of system critical items in the internal flash.
For each item key, the NVStore module provides the ability to set the item data or get it.
Newly added values are added to the end of the existing data, superseding the previous value that was there for the same key. 
The NVStore module ensures that power failures don't harm existing data during any operation.
The full interface can be found under `nvstore.h`.

### Flash structure
NVStore uses two Flash areas, active and nonactive. Each area must consist of at least one erasable unit (sector).
Data is written to the active area until it becomes full. When it does, garbage collection is invoked.
This compacts items from the active area to the nonactive one and switches activity between areas.
Each item is kept in an entry containing a header and data, where the header holds the item key, size and CRC.

### APIs
- init: Initialize NVStore (also lazily called by get, set, set_once and remove APIs).
- deinit: Deinitialize NVStore.
- get: Get the value of an item, given key.
- set: Set the value of an item, given key and value.
- set_once: Like set, but allows only a one time setting of this item (and disables deleting of this item).
- alloc_key: Allocates a free key (from the keys that are not predefined) to an owner (an owning feature).
- free_all_keys_by_owner: Free all allocated keys, given an owner.
- remove: Remove an item, given key.
- get_item_size: Get the item value size (in bytes).
- set_max_keys: Set maximal value of unique keys. Overriding the default of NVSTORE_MAX_KEYS. This affects RAM consumption,
  as NVStore consumes 4 bytes per unique key. Reinitializes the module.


## Usage

### Enabling NVStore and configuring it for your board
NVStore is enabled by default for all devices with the internal flash driver (have "FLASH" in the device_has attribute).
One can disable it by setting its "enabled" attribute to false.
Unless specifically configured by the user, NVStore selects the last two flash sectors as its areas, with the minimum size of 4KBs,
meaning that if the sectors are smaller, few continuous ones will be used for each area.
The user can override this by setting the addresses and sizes of both areas in` mbed_lib.json` on a per board basis.
In this case, all following four attributes need to be set:
- area_1_address
- area_1_size
- area_2_address
- area_2_size

In addition, the `num_keys` value should be modified to change the default number of different keys.

### Using NVStore
NVStore is a singleton class, meaning that the system can have only a single instance of it.
To instantiate NVStore, one needs to call its get_instance member function as following:
``` c++
    NVStore &nvstore = NVStore::get_instance();
```
After the NVStore instantiation, one can call the init API, but it is not necessary because all
NVStore APIs (get, set and so on) perform a "lazy initialization".

### Testing NVStore
Run the NVStore functionality test with the `mbed` command as following:
```mbed test -n features-nvstore-tests-nvstore-functionality```
