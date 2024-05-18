#include "atomic_uint8_t.h"

bool atomic_uint8_t::check_int(uint8_t pos)
{
    if (_int & (1 << pos))
    {
        return true;
    }

    return false;
}