# Configuration Store C++ API 
Author: Simon Hughes
Revision: v1.0.0
Date: 21 Sep 2016
DOCID: CFSTORE_HLD_0001_v1_0_0


## Configuration Store C++ API Design Goals


### Summary of Design Goals

The design goals can be summarised as follows:
- Present thread-safe (reentrant), synchronous programming model that
  integrates with RTOS RTX.
- Allows asynchronous programming model for derived classes, if desired


### Details of Design Goals

- The muxing/demuxing of multiple c++ layer client callbacks from a single
  underlying c-layer callback for asynchronous completion notifications.
  Considerations include:
	- allowing the registration of multiple asynchronous
      notification callbacks at the c++ layer (currently not supported)
    - Per client object type callback granularity (currently not supported)
    - avoiding performance bottlenecks e.g. serialising transactions to the
      c-layer using a transaction queue with mutex.
 - Support of both synchronous and asynchronous programming models in a simple
   way.
 - Flexibility, i.e. a design that provides a strong basis for developer
   customisation for specific application needs.
 - Scalability: A design that can evolve gracefully to support multiple
   CFSTORE instances in a system.
 - A design supporting re-entrant, concurrent access from multiple clients.
 - A design supporting the definition of custom types for storage i.e. a
   richer interface capable of storing developer defined types in addition
   to binary blobs.


## Configuration Store Software Stack


```
                              Fig 20160817_01. CFSTORE Software Stack
                              =======================================

     +-+-+-+-+-+-+-+-+-+-+-++-+  +-+-+-+-+-+-+-+-+  +-+-+-+-+-+-+-+-+-+  ++-+-+-+-+-+-+-+
     |  Provisioning Client   |  |  mbed Client  |  |  Update Service |  | Thread Stack |
     +-+-+-+-+-+-+-+-+-+-+-++-+  +-+-+-+-+-+-+-+-+  +-+-+-+-+-+-+-+-+-+  ++-+-+-+-+-+-+-+

     +-+-+-+-+-+-+-+-+-+-+-++-+
     |  mbedTLS Seed Store    |
     +-+-+-+-+-+-+-+-+-+-+-++-+

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+  +-+-+-+-+-+-+
     |             Configuration Store CPP Interface Wrapper             |  |  uvisor   |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+  |           |
                                                                            |           |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+  |           |
     |                      Configuration Store  C-HAL                   |  |           |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+  |           |
                                                                            |           |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+  +-+-+-+-+-+-+-+-+-+  +-+-+-+-+-+-+-+-+  |           |
     |  Data Journaling Layer    |  |  FAT32 library  |  | Linux FS      |  |           |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+  +-+-+-+-+-+-+-+-+++  +-+-+-+-+-+-+-+-+  |           |
                                                                            |           |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++  |           |
     |  Flash Driver Layer       |  |  Flash Driver                      |  |           |
     |  e.g. CMSIS-Driver        |  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++  +-+-+-+-+-+-+
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+

     SW
     ------------------------------------------------------------------------------------
     HW

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     |  NV Storage Media e.g. Flash  |
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

```

The following sections provide notes on the elements in the above figure.

##### Configuration Store Clients

The top row of entities shows the CFSTORE Clients:
- Provisioning Client
- mbed Client
- Update Service
- Thread Stack (Nanostack) Reference Application
- mbedTLS seed store.

These clients:
- Use the CFSTORE C++ interface to interact with the store.
- When security is enabled, the CFSTORE clients:
	- Run in the uvisor main box, or
    - Run in their own private security context uvisor box.


##### Configuration Store CPP Interface Wrapper

This is the C++ interface used by the CFSTORE clients. The interface presented by
this entity is the subject of this document.


###### Configuration Store C-HAL 

This is the underlying implementation of the configuration store, which is written in C.


##### Data Journaling Layer

The data journalling layer consists of the following elements:
- Flash Journal. This provides a specification of the flash journal interface.
- Flash Journal Strategy Sequential. This provides an implementation of the 
  flash journal internal which provides:
    - Atomicity of transactions. A client knows that once a call to the flash 
      journal API function has returned, the client can assume the operation
      has performed an atomic change to the stored data. 
    - Wear leveling. 
- Storage Volume manager.

##### Flash Driver Layer and Flash Driver

This is the implementation of the HW platform specific flash driver e.g.
storage_driver.c for the K64F.


##### FAT32 Library

Conceptually this is the implementation of the FAT32 file system.


#####Linux FS*

Conceptually this is the implementation of the Linux file system.


##### uvisor

This is the mbedos security subsystem. 


## Configuration Store C++ API is Synchronous

From the mbedos design perspective, the configuration store async callback 
model is not a good design fit for the following reasons:
- Threaded systems i.e. mbedOS RTOS RTX can adopt a synchronous programming 
  model and let thread scheduling deal the progression of other tasks
  while a block thread is waiting for service to be completed.
- A component integrating with uvisor (sync/async RPC gateways for
  inter-box communication).

This is to due to the following reasons:
- CFSTORE box has the storage driver stack running in the CFSTORE
  security context.
- The underlying storage driver calls the CFSTORE registered
  callback at interrupt context.
- CFSTORE invokes the client registered callback at the same interrupt
  context.
- If CFSTORE is running in its own uvisor secure context, its not
  possible to get back into the security context of the originating caller.
  Hence, uvisor with async callbacks is not a good fit
- As the primary behaviour for the wrapper is sync, then wrapper should
  use sync uvisor rpc gateways. CFSTORE box target function may be async,
  but then there needs to be a sync object that that this function
  can block on, and the CFSTORE callback signal when the transaction has
  been completed.
- At the present time, only the following functions are require async
  completion internally:
    - Initialize(). this happens once for all clients
    - Flush(). this needs to be sequentialised, and other outstanding operations completed, then
      blocked i.e. operations that modify anything e.g. write, create, delete. other operations
      can occur.
- If the CFSTORE wants to have an async api, then it needs to use async uvisor rpc gateways
  this means re-designing the interface not to use callbacks.

As a consequence of the above:
- there is no Cfstore static member function for async call back notifications
- Cfstore doesnt have a Notify() method, which is used to give async notifications
  to Cfstore derived classes, for example.
- CfstoreKey class also does not have a Notify() method for async notification.

