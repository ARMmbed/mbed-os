#ifndef MBED_BLE_ROLES_H__
#define MBED_BLE_ROLES_H__

#ifdef BLE_ROLE_PERIPHERAL
    #ifndef BLE_ROLE_OBSERVER
        #error "BLE role 'PERIPHERAL' depends on role 'OBSERVER'"
    #endif // BLE_ROLE_OBSERVER
#endif // BLE_ROLE_PERIPHERAL

#ifdef BLE_ROLE_CENTRAL
    #ifndef BLE_ROLE_BROADCASTER
        #error "BLE role 'CENTRAL' depends on role 'BROADCASTER'"
    #endif // BLE_ROLE_BROADCASTER
#endif // BLE_ROLE_CENTRAL

#ifdef BLE_ROLE_SECURITY
    #ifndef BLE_ROLE_PERIPHERAL
        #error "BLE role 'SECURITY' depends on role 'PERIPHERAL'"
    #endif // BLE_ROLE_PERIPHERAL
#endif // BLE_ROLE_SECURITY

#ifdef BLE_ROLE_PRIVACY
    #ifndef BLE_ROLE_SECURITY
        #error "BLE role 'PRIVACY' depends on role 'SECURITY'"
    #endif // BLE_ROLE_SECURITY
#endif // BLE_ROLE_PRIVACY

#ifdef BLE_ROLE_GATT_CLIENT
    #ifndef BLE_ROLE_PERIPHERAL
        #error "BLE role 'GATT CLIENT' depends on role 'PERIPHERAL'"
    #endif // BLE_ROLE_PERIPHERAL

    #ifndef BLE_ROLE_BROADCASTER
        #error "BLE role 'GATT CLIENT' depends on role 'BROADCASTER'"
    #endif // BLE_ROLE_BROADCASTER
#endif // BLE_ROLE_GATT_CLIENT

#ifdef BLE_ROLE_GATT_SERVER
    #ifndef BLE_ROLE_SECURITY
        #error "BLE role 'GATT SERVER' depends on role 'SECURITY'"
    #endif // BLE_ROLE_SECURITY
#endif // BLE_ROLE_GATT_SERVER

#endif // MBED_BLE_ROLES_H__
