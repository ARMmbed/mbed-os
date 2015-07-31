#ifndef _CUSTOM_H_
#define _CUSTOM_H_

#include "mbed.h"

class custom {
    public:
        custom();
        ~custom();

        int get_val();

    private:
        int _val;
};

#endif
