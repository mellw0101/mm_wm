#include <cstdint>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include "globals.h"
#include "xcb.hpp"

/*
    #include "xcb.hpp"
    #include "structs.hpp"
*/

class window_t
{
    private:
        uint32_t _window = 0;

    public:
        operator uint32_t()
        {
            return _window;
        }

        operator const uint32_t&() const
        {
            return _window;
        }

        void map()
        {
            XCB::map_window(_window);
        }

        void unmap()
        {
            XCB::unmap_window(_window);
        }

        void raise()
        {
            XCB::raise_window(_window);
        }

        void change_attribute(uint32_t __mask, const void *__data)
        {
            XCB::change_window_attributes(_window, __mask, __data);
        }

        void change_attribute_checked(uint32_t __mask, const void *__data)
        {
            XCB::change_window_attributes_checked(_window, __mask, __data);
        }
};