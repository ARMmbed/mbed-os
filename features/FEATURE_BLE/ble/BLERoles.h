#ifndef MBED_BLE_ROLES_H__
#define MBED_BLE_ROLES_H__

#if !defined(BLE_ROLE_OBSERVER) && !defined(BLE_ROLE_BROADCASTER)
    #error "BLE requires at least one role 'BROADCASTER' or 'OBSERVER' to be enabled"
#endif

#ifdef BLE_ROLE_PERIPHERAL
    #if !defined(BLE_ROLE_OBSERVER)
        #error "BLE role 'PERIPHERAL' depends on role 'OBSERVER'"
    #endif
#endif // BLE_ROLE_PERIPHERAL

#ifdef BLE_ROLE_CENTRAL
    #if !defined(BLE_ROLE_BROADCASTER)
        #error "BLE role 'CENTRAL' depends on role 'BROADCASTER'"
    #endif
#endif // BLE_ROLE_CENTRAL

#ifdef BLE_FEATURE_SECURITY
    #if !defined(BLE_ROLE_PERIPHERAL) && !defined(BLE_ROLE_CENTRAL)
        #error "BLE feature 'SECURITY' requires 'PERIPHERAL' or 'CENTRAL' role"
    #endif
#endif // BLE_FEATURE_SECURITY

#ifdef BLE_FEATURE_SECURE_CONNECTIONS
    #if !defined(BLE_FEATURE_SECURITY)
        #error "BLE feature 'SECURE CONNECTIONS' requires the 'SECURITY' feature"
    #endif
#endif // BLE_FEATURE_SECURE_CONNECTIONS

#ifdef BLE_FEATURE_SIGNING
    #if !defined(BLE_FEATURE_SECURITY)
        #error "BLE feature 'SIGNING' requires the 'SECURITY' feature"
    #endif
#endif // BLE_FEATURE_SIGNING

#ifdef BLE_FEATURE_WHITELIST
    #if !defined(BLE_FEATURE_SECURITY)
        #error "BLE feature 'WHITELIST' requires the 'SECURITY' feature"
    #endif
#endif // BLE_FEATURE_WHITELIST

#ifdef BLE_FEATURE_PRIVACY
    #if !defined(BLE_FEATURE_SECURITY)
        #error "BLE feature 'PRIVACY' requires the 'SECURITY' feature"
    #endif
#endif // BLE_FEATURE_PRIVACY

#ifdef BLE_FEATURE_EXTENDED_ADVERTISING
    #if !defined(BLE_FEATURE_PHY_MANAGEMENT)
        #error "BLE feature 'EXTENDED ADVERTISING' requires the 'PHY MANAGEMENT' feature"
    #endif
#endif // BLE_FEATURE_EXTENDED_ADVERTISING

#ifdef BLE_FEATURE_PERIODIC_ADVERTISING
    #if !defined(BLE_FEATURE_EXTENDED_ADVERTISING)
        #error "BLE feature 'PERIODIC ADVERTISING' requires the 'EXTENDED ADVERTISING' feature"
    #endif
#endif // BLE_FEATURE_PERIODIC_ADVERTISING

#ifdef BLE_FEATURE_GATT_CLIENT
    #if !defined(BLE_ROLE_PERIPHERAL) && !defined(BLE_ROLE_CENTRAL)
        #error "BLE feature 'GATT CLIENT' requires 'PERIPHERAL' or 'CENTRAL' role"
    #endif
#endif // BLE_FEATURE_GATT_CLIENT

#ifdef BLE_FEATURE_GATT_SERVER
    #if !defined(BLE_ROLE_PERIPHERAL) && !defined(BLE_ROLE_CENTRAL)
        #error "BLE feature 'GATT SERVER' requires 'PERIPHERAL' or 'CENTRAL' role"
    #endif
#endif // BLE_FEATURE_GATT_SERVER

#endif // MBED_BLE_ROLES_H__
