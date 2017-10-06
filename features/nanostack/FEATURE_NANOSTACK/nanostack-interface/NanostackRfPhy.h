/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */

#ifndef NANOSTACK_RF_PHY_H_
#define NANOSTACK_RF_PHY_H_

#include "NanostackPhy.h"

class NanostackRfPhy : public NanostackPhy {
public:

    /** Register this physical interface with Nanostack
     *
     *  @return         Device driver ID or a negative error
     *                  code on failure
     */
    virtual int8_t rf_register() = 0;

    /** Unregister this physical interface
     *
     */
    virtual void rf_unregister() = 0;

    /** Register this physical interface with Nanostack
     *
     *  @return         Device driver ID or a negative error
     *                  code on failure
     */
    virtual int8_t phy_register() { return rf_register();}

    /** Unregister this physical interface
     *
     */
    virtual void unregister() { rf_unregister(); }
};

#endif /* NANOSTACK_RF_PHY_H_ */
