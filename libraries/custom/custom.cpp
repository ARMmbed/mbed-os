#include "custom.h"

custom::custom() : _val(0) {}

custom::~custom() {}

int custom::get_val() {
    return _val++;
}
