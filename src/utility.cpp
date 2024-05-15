#include "utility.h"

bool mwm::isTimeToRender(chrono::high_resolution_clock::time_point tp)
{
    const auto &currentTime = chrono::high_resolution_clock::now();
    const chrono::duration<double, milli> & elapsedTime = currentTime - tp;
    if (elapsedTime.count() >= FRAMETIME)
    {
        tp = currentTime;
        return true;
    }

    return false;
}