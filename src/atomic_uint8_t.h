#pragma once

#include <cstdint>

class atomic_uint8_t
{
private:
    uint8_t _int;

public:
    bool check_int(uint8_t pos);

};