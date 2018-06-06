# BLE API Cordio Implementation 

The BLE API Cordio implementation allows Cordio licensee to easily deliver a 
complete and up to date implementation of mbed BLE to their customers using 
mbed OS. 

To deliver a BLE port, vendors simply have to provide an HCI driver tailored 
for the BLE module present on the board they want to support. 

## Source Organization 

The root contains the public header of the classes implementing BLE API public
interface.

* `doc`: Documentation such as the HCI extension guide and the 
porting guide.
* `driver`: HCI driver and transport interfaces. Also contains standard driver
implementation.
* `sources`: BLE API Implementation files 
* `stack`: Cordio stack 
    * `cordio_stack`: Headerd of the cordio stack 
    * `lib`: precompiled libraries of the cordio stack 
    * `mbed_os_adaptation`: Headers of the interface responsible for the bridge
    between BLE API and the Cordio stack.

## Documentation

* [HCI architecture](doc/HCIAbstraction.md)
* [Porting guide](doc/PortingGuide.md)
