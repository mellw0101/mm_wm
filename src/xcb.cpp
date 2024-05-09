#include "xcb.hpp"
// #include "data.hpp"
#include "defenitions.hpp"
#include "tools.hpp"
#include "Log.hpp"
#include <cstdint>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include "prof.hpp"

namespace XCB
{
    uint32_t gen_Xid()
    {
        return xcb_generate_id(conn);
    }

    void flush()
    {
        xcb_flush(conn);
    }
    
    void change_window_attributes(uint32_t __window, uint32_t __mask, const void *__data)
    {
        xcb_change_window_attributes
        (
            conn,
            __window,
            __mask,
            __data
        );
        xcb_flush(conn);
    }

    void change_window_attributes_checked(uint32_t __window, uint32_t __mask, const void *__data)
    {
        xcb_void_cookie_t cookie = xcb_change_window_attributes
        (
            conn,
            __window,
            __mask,
            __data
        );
        xcb_generic_error_t *error = xcb_request_check(conn, cookie);
        if (error != nullptr)
        {
            loutE << WINDOW_ID_BY_INPUT(__window) << "error_code" << error->error_code << loutEND;
        }
        xcb_flush(conn);
    }

    void configure_window(uint32_t __window, uint32_t __mask, const void *__data)
    {
        xcb_configure_window
        (
            conn,
            __window,
            __mask,
            __data
        );
    }

    void configure_window_checked(uint32_t __window, uint32_t __mask, const void *__data)
    {
        xcb_void_cookie_t cookie = xcb_configure_window_checked
        (
            conn,
            __window,
            __mask,
            __data
        );
        xcb_generic_error_t *error = xcb_request_check(conn, cookie);
        if (error != nullptr)
        {
            loutE << WINDOW_ID_BY_INPUT(__window) << "error_code" << error->error_code << loutEND;
        }
    }

    void map_window(uint32_t __window)
    {
        xcb_map_window(conn, __window);
        xcb_flush(conn);
    }

    void unmap_window(uint32_t __window)
    {
        xcb_unmap_window(conn, __window);
        xcb_flush(conn);
    }

    void raise_window(uint32_t __window)
    {
        XCB::configure_window
        (
            __window,
            XCB_CONFIG_WINDOW_STACK_MODE,
            (const uint32_t[])
            {
                XCB_STACK_MODE_ABOVE
            }
        );
        xcb_flush(conn);
    }

    void reparent_window(uint32_t __window, uint32_t __new_parent, int16_t __x, int16_t __y)
    {
        xcb_reparent_window
        (
            conn,
            __window,
            __new_parent,
            __x,
            __y
        );
        xcb_flush(conn);
    }

    /**
     
      @brief avalible 'revert_to' options 
     
      @p XCB_INPUT_FOCUS_NONE 
      @p XCB_INPUT_FOCUS_PARENT 
      @p XCB_INPUT_FOCUS_POINTER_ROOT
     
      @p XCB_INPUT_FOCUS_FOLLOW_KEYBOARD NOTE: This is experemental
      
     */
    void set_focus_input(uint32_t __window, uint8_t __revert_to)
    {
        xcb_set_input_focus
        (
            conn,
            __revert_to,
            __window,
            XCB_CURRENT_TIME
        );
        xcb_flush(conn);
    }

    void clear_area(uint32_t __window, uint16_t __width, uint16_t __height, int16_t __x, int16_t __y)
    {
        xcb_clear_area
        (
            conn,
            0,
            __window,
            __x,
            __y,
            __width,
            __height
        );
        xcb_flush(conn);
    }

    void open_font(uint32_t __font_id, const char *__font_name)
    {
        xcb_open_font
        (
            conn,
            __font_id,
            slen(__font_name),
            __font_name
        );
        xcb_flush(conn);
    }

    uint32_t open_and_get_font_id(const char *__font_name)
    {
        uint32_t font_id = XCB::gen_Xid();
        XCB::open_font(font_id, __font_name);
        return font_id;
    }

    void change_back_pixel(uint32_t __window, uint32_t __pixel)
    {
        XCB::change_window_attributes(__window, XCB_CW_BACK_PIXEL, (const uint32_t[]){ __pixel });
    }

    void apply_ev_mask(uint32_t __window, const uint32_t *__mask)
    {
        XCB::change_window_attributes(__window, XCB_CW_EVENT_MASK, __mask);
        XCB::flush();
    }

    void create_window(
        uint32_t    __window,
        uint32_t    __parent,
        int16_t     __x,
        int16_t     __y,
        uint16_t    __width,
        uint16_t    __height,
        uint8_t     __depth,
        uint16_t    __border_width,
        uint16_t    ___class,
        uint32_t    __visual,
        uint32_t    __value_mask,
        const void *__value_list)
    {
        xcb_create_window
        (
            conn,
            __depth,
            __window,
            __parent,
            __x,
            __y,
            __width,
            __height,
            __border_width,
            ___class,
            __visual,
            __value_mask,
            __value_list
        );
        XCB::flush();
    }

    void create_gc(uint32_t __window, uint32_t __gc, uint32_t __mask, const void *__data)
    {
        xcb_create_gc
        (
            conn,
            __gc,
            __window,
            __mask,
            __data
        );
        XCB::flush();
    }

    uint32_t create_graphics_exposure_gc(uint32_t __window)
    {
        uint32_t gc = XCB::gen_Xid();
        XCB::create_gc
        (
            __window,
            gc,
            XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_GRAPHICS_EXPOSURES,
            (const uint32_t[])
            {
                screen->black_pixel,
                screen->white_pixel,
                0
            }
        );

        return gc;
    }

    xcb_get_geometry_cookie_t get_geometry(uint32_t __window)
    {
        return xcb_get_geometry(conn, __window);
    }

    xcb_get_geometry_reply_t *get_geometry_reply(xcb_get_geometry_cookie_t __cookie)
    {
        return xcb_get_geometry_reply(conn, __cookie, nullptr);
    }

    xcb_intern_atom_cookie_t intern_atom(bool __only_if_exists, const char *__name)
    {
        return xcb_intern_atom(conn, __only_if_exists, slen(__name), __name);
    }

    xcb_intern_atom_reply_t *intern_atom_reply(xcb_intern_atom_cookie_t __cookie)
    {
        return xcb_intern_atom_reply(conn, __cookie, nullptr);
    }
}

/**
*****************************************
*****************************************
**** @class @c VoidError
*****************************************
****************************************/

VoidError::VoidError(xcb_void_cookie_t cookie)
: err(nullptr)
{
    checkErr(cookie);

}

VoidError::~VoidError()
{
    if (err)
    {
        free(err);/* Free the error pointer if it's not NULL */
    }
}

void VoidError::checkErr(xcb_void_cookie_t cookie)
{
    err = xcb_request_check(conn, cookie);
    if (err)
    {
        loutE << "XCB Error occurred. Error code: " << err->error_code << loutEND;
    }
}

bool VoidError::hasErr() const
{
    return err != nullptr;
}

/**
*****************************************
*****************************************
**** @class @c xcb
*****************************************
****************************************/
xcb_intern_atom_cookie_t xcb::intern_atom_cookie(const char *__name)
{
    return xcb_intern_atom(
        conn,
        0,
        slen(__name),
        __name
    );
}

xcb_intern_atom_reply_t *xcb::intern_atom_reply(xcb_intern_atom_cookie_t __cookie)
{
    return xcb_intern_atom_reply(conn, __cookie, nullptr);
}

xcb_atom_t xcb::intern_atom(const char *__name)
{
    xcb_intern_atom_reply_t *rep = intern_atom_reply(intern_atom_cookie(__name));
    if (!rep)
    {
        loutE << "rep = nullptr could not get intern_atom_reply" << loutEND;
        free(rep);
        return XCB_ATOM_NONE;
    }
    xcb_atom_t atom(rep->atom);
    free(rep);
    return atom;
}

/* bool xcb::window_exists(uint32_t __w)
{
    xcb_generic_error_t *err;
    free(xcb_query_tree_reply(_conn, xcb_query_tree(_conn, __w), &err));
    if (err != NULL)
    {
        free(err);
        return false;
    }
    return true;
} */
/* bool xcb::window_exists(uint32_t __w)
{
    xcb_generic_error_t *err = NULL;
    auto *reply = xcb_query_tree_reply(conn, xcb_query_tree(conn, __w), &err);
    if (reply) free(reply); // Correctly free the reply if it's non-null

    if (!err) return true; // If there was no error, assume the window exists
    loutE << err->error_code << loutEND;
    free(err); // Free the error if it's non-null
    return false; // Return false if there was an error, assuming it indicates non-existence

} */

uint32_t xcb::gen_Xid()
{
    if (_flags & (1ULL << X_CONN_ERROR))
    {
        return -1;
    }
    uint32_t w = xcb_generate_id(conn);

    if (w == -1)
    {
        set_flag(X_ID_GEN_ERROR);
        loutE << "Failed to generate Xid" << loutEND;
    }
    else
    {
        clear_flag(X_ID_GEN_ERROR);
    }
    return w;
}

void xcb::window_stack(uint32_t __window1, uint32_t __window2, uint32_t __mode)
{
    if (__window2 == XCB_NONE) return;
    
    uint16_t mask = XCB_CONFIG_WINDOW_SIBLING | XCB_CONFIG_WINDOW_STACK_MODE;
    uint32_t values[] = {__window2, __mode};
    
    xcb_configure_window(conn, __window1, mask, values);   
}

bool xcb::is_flag_set(unsigned int __f)
{
    return ( _flags & ( 1ULL << __f )) != 0;
}

void xcb::set_flag(unsigned int __f)
{
    _flags |= 1ULL << __f;
}

void xcb::clear_flag(unsigned int __f)
{
    _flags &= ~(1ULL << __f);
}

void xcb::toggle_flag(unsigned int __f)
{
    _flags ^= 1ULL << __f;
}

xcb::xcb(xcb_screen_t *__s) : _s(__s)
{
    if (xcb_connection_has_error(conn))
    {
        loutE << "could not connect to the exisiting 'xcb_connection_t *'" << loutEND;
        _flags |= 1ULL << X_CONN_ERROR;
        /* _conn = nullptr; */
    }
    else
    {
        _flags &= ~(1ULL << X_CONN_ERROR);
        loutI << "success x is now connected to the server" << loutEND;    
    }

}

uint64_t &xcb::check_conn()
{
    return _flags;    
}

void xcb::create_w(uint32_t __pw, uint32_t __w, int16_t __x, int16_t __y, uint16_t __width, uint16_t __height)
{
    // Create the window
    _cookie = xcb_create_window(
        conn,
        24,
        __w,
        __pw,
        __x,
        __y,
        __width,
        __height,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        _s->root_visual,
        0,
        nullptr
    );
    check_error();
}

void xcb::check_error()
{
    _err = xcb_request_check(conn, _cookie);
    if (_err)
    {
        loutE << ERRNO_MSG("XCB Error occurred. Error code:") << _err->error_code << loutEND;
        _flags |= 1ULL << X_REQ_ERROR; /* Set bit to true */
    }
    else
    {
        _flags &= ~(1ULL << X_REQ_ERROR); /* Set bit to false */    
    }
}

void xcb::mapW(uint32_t __w)
{
    VoidC cookie = xcb_map_window(conn, __w);
    CheckVoidC(cookie, "window map failed");
    xcb_flush(conn);
}

xcb_font_t xcb::get_font(const char *font_name)
{
    xcb_font_t font = xcb_generate_id(conn);
    xcb_open_font(
        conn,
        font,
        slen(font_name),
        font_name
    );
    xcb_flush(conn);
    return font;
}

/* void atoms_t::fetch_atom_data(xcb_connection_t *conn, char *__name) {
    // atom_t *atom = new atom_t;
    atom_t *atom = Malloc<atom_t>().allocate();
    intern_atom_cok_t cookie(conn, 0, __name);
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, nullptr);
    if (!reply) {
        loutE << "reply = nullptr" << loutEND;
        
    }

    atom->response_type = reply->response_type;
    atom->pad0          = reply->pad0;
    atom->sequence      = reply->sequence;
    atom->length        = reply->length;
    atom->atom          = reply->atom;

    // _data.push_back(std::move(atom));

}
atoms_t::atoms_t(xcb_connection_t *conn, char **__atoms){
    for (int i = 0; __atoms[i]; ++i) {
        fetch_atom_data(conn, __atoms[i]);

    }

}
atoms_t::~atoms_t() {
    for (const auto &atom : _data) Malloc<atom_t>().deallocate(atom); // delete atom;
    
} */

uint32_t xcb::get_atom(uint8_t __only_if_exists, const char *__atom_name)
{
    iAtomR r(0, __atom_name);
    return (r.is_not_valid()) ? XCB_NONE : r.Atom();
}

uint32_t xcb::get_input_focus()
{
    AutoTimer t("xcb::get_input_focus");

    xcb_get_input_focus_cookie_t cookie = xcb_get_input_focus(conn);
    xcb_get_input_focus_reply_t *reply = xcb_get_input_focus_reply(conn, cookie, NULL); // NULL for no error handling

    if (reply == nullptr)
    {
        loutE << "Could not get input focus" << "\n";
        return UINT32_MAX;
    }
    uint32_t w = reply->focus;
    loutI << "The current focused win" << w << '\n';
    free(reply);
    return w;
}

uint32_t xcb::get_input_focus_unchecked()
{
    AutoTimer t("xcb::get_input_focus_unchecked");

    xcb_get_input_focus_cookie_t cookie = xcb_get_input_focus_unchecked(conn);
    xcb_get_input_focus_reply_t *reply = xcb_get_input_focus_reply(conn, cookie, NULL); // NULL for no error handling

    if (reply == nullptr)
    {
        loutE << "Could not get input focus" << "\n";
        return UINT32_MAX;
    }
    uint32_t w = reply->focus;
    loutI << "The current focused win" << w << '\n';
    free(reply);
    return w;
}

uint32_t xcb::window_exists(uint32_t w)
{
    // Attempt to retrieve the WM_NAME property of the window
    xcb_get_property_cookie_t cookie = xcb_get_property_unchecked(conn, 0, w, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, 0);

    xcb_get_property_reply_t *reply = xcb_get_property_reply(conn, cookie, NULL);

    if (reply == nullptr)
    {
        // The window does not exist
        return UINT32_MAX;
    }

    free(reply);
    return w;
}