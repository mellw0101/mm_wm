#ifndef COLOR_HPP
#define COLOR_HPP

#include <vector>
#include <utility>
#include <cstdint>
#include "structs.hpp"
#include <xcb/xcb.h>

using namespace std;

class __color__
{
    private:
    /* Variabels */
        vector<pair<int, uint32_t>> colorVec;

    /* Methods   */
        rgb_color_code rgb_code__(int __color);

        uint32_t get_color__(int __color);

        void init_colors__(const vector<int> &__vec);

    public:
        __color__(const vector<int> &__vec);

        uint32_t get(int __color);
};
static __color__ *Color(nullptr);

#endif/* COLOR_HPP */