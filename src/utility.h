/* utility.h */
#pragma once

#include "globals.h"
#include <chrono>

using namespace std;

namespace mwm
{
    bool isTimeToRender(chrono::high_resolution_clock::time_point tp);
}