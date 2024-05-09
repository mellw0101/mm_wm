#include "color.hpp"
#include "globals.h"
#include "prof.hpp"

rgb_color_code __color__::rgb_code__(int __color)
{
    AutoTimer timer("__color__:rgb_code");

    rgb_color_code color;
    uint8_t r, g, b;
    
    switch (__color)
    {
        case WHITE:
        {
            r = 255; g = 255; b = 255;
            break;
        }

        case BLACK:
        {
            r = 0; g = 0; b = 0;
            break;
        }
        
        case RED:
        {
            r = 255; g = 0; b = 0;
            break;
        }
        
        case GREEN:
        {
            r = 0; g = 255; b = 0;
            break;
        }
        
        case BLUE:
        {
            r = 0; g = 0; b = 255;
            break;
        }
        
        case BLUE_2:
        {
            r = 0; g = 0; b = 230;
            break;
        }
        
        case BLUE_3:
        {
            r = 0; g = 0; b = 204;
            break;
        }
        
        case BLUE_4:
        {
            r = 0; g = 0; b = 178;
            break;
        }
        
        case BLUE_5:
        {
            r = 0; g = 0; b = 153;
            break;
        }
        
        case BLUE_6:
        {
            r = 0; g = 0; b = 128;
            break;
        }
        
        case BLUE_7:
        {
            r = 0; g = 0; b = 102;
            break;
        }
        
        case BLUE_8:
        {
            r = 0; g = 0; b = 76;
            break;
        }
        
        case BLUE_9:
        {
            r = 0; g = 0; b = 51;
            break;
        }
        
        case BLUE_10:
        {
            r = 0; g = 0; b = 26;
            break;
        }
        
        case YELLOW:
        {
            r = 255; g = 255; b = 0;
            break;
        }
        
        case CYAN:
        {
            r = 0; g = 255; b = 255;
            break;    
        }
        
        case MAGENTA:
        {
            r = 255; g = 0; b = 255;
            break;    
        }
        
        case GREY:
        {
            r = 128; g = 128; b = 128;
            break;
        }
        
        case LIGHT_GREY:
        {
            r = 192; g = 192; b = 192;
            break;
        }
        
        case DARK_GREY:
        {
            r = 64; g = 64; b = 64;
            break;
        }
        
        case DARK_GREY_2: {
            r = 70; g = 70; b = 70;
            break;
        }
        
        case DARK_GREY_3:
        {
            r = 76; g = 76; b = 76;
            break;
        }
        
        case DARK_GREY_4:
        {
            r = 82; g = 82; b = 82;
            break;
        }
        
        case ORANGE:
        {
            r = 255; g = 165; b = 0;
            break;
        }
        
        case PURPLE:
        {
            r = 128; g = 0; b = 128;
            break;
        }
        
        case BROWN:
        {
            r = 165; g = 42; b = 42;
            break;
        }
        
        case PINK:
        {
            r = 255; g = 192; b = 203;
            break;
        }
        
        default:
        {
            r = 0; g = 0; b = 0; 
            break;
        }
    }

    color.r = r;
    color.g = g;
    color.b = b;
    
    return color;
}

uint32_t __color__::get_color__(int __color)
{
    AutoTimer timer("__color__:get_color__");

    uint32_t pi          = 0;
    xcb_colormap_t cmap  = screen->default_colormap;
    rgb_color_code ccode = rgb_code__(__color);
    xcb_alloc_color_reply_t *r = xcb_alloc_color_reply
    (
        conn,
        xcb_alloc_color
        (
            conn,
            cmap,
            (ccode.r << 8) | ccode.r,
            (ccode.g << 8) | ccode.g,
            (ccode.b << 8) | ccode.b
        ),
        nullptr
    );
    pi = r->pixel;
    free(r);    
    return pi;
}

void __color__::init_colors__(const vector<int> &__vec)
{
    for (const int &i : __vec)
    {
        colorVec.push_back({i, get_color__(i)});
    }
}

__color__::__color__(const vector<int> &__vec)
{
    init_colors__(__vec);
}

uint32_t __color__::get(int __color)
{
    AutoTimer t("__color__:get");

    for (const pair<int, uint32_t> &pair : colorVec)
    {
        if (pair.first == __color) return pair.second;
    }

    return 0;
}

/* uint32_t get_color(const uint16_t &red_value, const uint16_t &green_value, const uint16_t &blue_value)
{
    uint32_t                pi = 0;
    xcb_colormap_t        cmap = screen->default_colormap;
    xcb_alloc_color_reply_t *r = xcb_alloc_color_reply(
        conn,
        xcb_alloc_color(
            conn,
            cmap,
            red_value,
            green_value,
            blue_value
        ),
        nullptr
    );
    pi = r->pixel;
    free(r);
    return pi;
}

uint32_t get_color(const uint8_t & red_value, const uint8_t & green_value, const uint8_t & blue_value)
{
    uint32_t                pi = 0;
    xcb_colormap_t        cmap = screen->default_colormap;
    xcb_alloc_color_reply_t *r = xcb_alloc_color_reply(
        conn,
        xcb_alloc_color(
            conn,
            cmap,
            _scale::from_8_to_16_bit(red_value), 
            _scale::from_8_to_16_bit(green_value),
            _scale::from_8_to_16_bit(blue_value)
        ),
        NULL
    );
    pi = r->pixel;
    free(r);
    return pi;
} */