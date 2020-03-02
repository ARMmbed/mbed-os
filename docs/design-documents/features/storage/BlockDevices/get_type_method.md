# Get type method addon to block devices class
 
### Revision history
| Revision  | Date            | Authors                                                     | Mbed OS version        | Comments         |
|---------- |-----------------|-------------------------------------------------------------|------------------------|------------------|
| 1.0       | 04/12/2018      | Yossi Levy ([@yossi2le](https://github.com/yossi2le/))      | 5.11+                  | Initial revision |
 
## Introduction
 
Most storage solutions use block devices, whether it is Filesystems, KVStore or any other solution 
most of them will have some BlockDevice class underneath. However, sometimes a storage type or an application needs to know the physical 
BlockDevice type in order to work smoothly or the BlockDevice type in order to decide what is
the best storage configuration to use.
To address this an add-on method of getting type is proposed for BlockDevice interface class.
 
## The Motivation 
 
Below there is a list of some examples to explain the motivation and the need for the adding of get_type to BlockDevice interface.
 
examples:
- TDBStore needs to know if there are flash characteristics for the block device and if there aren�t it should use
  FlashSimBlockDevice to simulate a flash BlockDevice.
- When creating a file system you would prefer working with FAT on top of SD while LITTLEFS on top of any flash block device. 
  Those preference in favor of better performance.

To summarize the above, it may be very useful when using block device to know the type of the instance and especially, but not only, 
when using get_default_instace. Sometimes applications and tests would like to behave differently depending on the instance that has been created
or provided to them.
 
In fact it might be worth to consider adding the get_type to any interface with get_default_instance at mbed-os.   
  
## Dive into details
we should add the following method to BlockDevice interface class.
 
```virtual const char * get_type() const = 0;```
 
then every physical BlockDevice class which implements the interface should also implement this method and return a string 
representing its type. Furthermore, a nonphysical BlockDevice like SlicingBlockDevice should return the underlying physical 
BlockDevice type.
 
### Physical BlockDevice:
```
const char * HeapBlockDevice::get_type() const
{
    return "HEAP";
}
```
 
### Logical BlockDevice:
```
const char * SlicingBlockDevice::get_type() const
{
    if (_bd != NULL) {
     return _bd->get_type();
    }
 
    return NULL;
}
```

### Open issue
The ChainingBlockDevice which chains different type of physical block devices into one block device is unable
to return the underneath physical as it contains two or more types. Therefore it will return CHAINING as its 
identity and its left for the user to decide how the application will treat this information.


The below table describes physical BlockDevice and its tyep names


| BlockDevice class           | String description | 
|-----------------------------|--------------------|
| HeapBlockDevice             | "HEAP"             |
| SPIFBlockDevice             | "SPIF"             |
| QSPIFBlockDevice            | "QSPIF"            |
| SDBlockDevice               | "SD"               |
| FlashIAPBlockDevice         | "FLASHIAP"         |
| DataFlashBlockDevice        | "DATAFLASH"        |
| ChainingBlockDevice         | "CHAINING"         |
