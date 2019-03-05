#ifndef MBED_BLE_ROLES_H__
#define MBED_BLE_ROLES_H__

#if !(BLE_ROLE_OBSERVER) && !(BLE_ROLE_BROADCASTER)
    #error "BLE requires at least one role 'BROADCASTER' or 'OBSERVER' to be enabled"
#endif

#if (BLE_ROLE_PERIPHERAL) || (BLE_ROLE_CENTRAL)
    #define BLE_FEATURE_CONNECTABLE 1
#endif

#if (BLE_FEATURE_GATT_CLIENT) || (BLE_FEATURE_GATT_SERVER)
    #define BLE_FEATURE_ATT 1
#endif

#if BLE_ROLE_PERIPHERAL
    #if !(BLE_ROLE_BROADCASTER)
        #error "BLE role 'PERIPHERAL' depends on role 'BROADCASTER'"
    #endif
#endif // BLE_ROLE_PERIPHERAL

#if BLE_ROLE_CENTRAL
    #if !(BLE_ROLE_OBSERVER)
        #error "BLE role 'CENTRAL' depends on role 'OBSERVER'"
    #endif
#endif // BLE_ROLE_CENTRAL

#if BLE_FEATURE_SECURITY
    #if !(BLE_ROLE_PERIPHERAL) && !(BLE_ROLE_CENTRAL)
        #error "BLE feature 'SECURITY' requires 'PERIPHERAL' or 'CENTRAL' role"
    #endif
#endif // BLE_FEATURE_SECURITY

#if BLE_FEATURE_SECURE_CONNECTIONS
    #if !(BLE_FEATURE_SECURITY)
        #error "BLE feature 'SECURE CONNECTIONS' requires the 'SECURITY' feature"
    #endif
#endif // BLE_FEATURE_SECURE_CONNECTIONS

#if BLE_FEATURE_SIGNING
    #if !(BLE_FEATURE_SECURITY)
        #error "BLE feature 'SIGNING' requires the 'SECURITY' feature"
    #endif
#endif // BLE_FEATURE_SIGNING

#if BLE_FEATURE_WHITELIST
    #if !(BLE_FEATURE_SECURITY)
        #error "BLE feature 'WHITELIST' requires the 'SECURITY' feature"
    #endif
#endif // BLE_FEATURE_WHITELIST

#if BLE_FEATURE_PRIVACY
    #if !(BLE_FEATURE_SECURITY)
        #error "BLE feature 'PRIVACY' requires the 'SECURITY' feature"
    #endif
#endif // BLE_FEATURE_PRIVACY

#if BLE_FEATURE_PERIODIC_ADVERTISING
    #if !(BLE_FEATURE_EXTENDED_ADVERTISING)
        #error "BLE feature 'PERIODIC ADVERTISING' requires the 'EXTENDED ADVERTISING' feature"
    #endif
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#if BLE_FEATURE_GATT_CLIENT
    #if !(BLE_FEATURE_CONNECTABLE)
        #error "BLE feature 'GATT CLIENT' requires 'PERIPHERAL' or 'CENTRAL' role"
    #endif
#endif // BLE_FEATURE_GATT_CLIENT

#if BLE_FEATURE_GATT_SERVER
    #if !(BLE_FEATURE_CONNECTABLE)
        #error "BLE feature 'GATT SERVER' requires 'PERIPHERAL' or 'CENTRAL' role"
    #endif
#endif // BLE_FEATURE_GATT_SERVER

#endif // MBED_BLE_ROLES_H__
