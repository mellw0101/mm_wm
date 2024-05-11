
/** NOTE: currently DEPRECATED */
/* class __system_settings__ {
    // Defines.
        #define MENU_WINDOW_WIDTH 120
        #define MENU_ENTRY_HEIGHT 20
        #define MENU_ENTRY_TEXT_Y 14
        #define MENU_ENTRY_TEXT_X 4
    
    private:
    // Structs.
        typedef struct {
            window   _window;
            string   _device_name;
            uint16_t _device_id;
        } pointer_device_info_t;

    // Variabels.
        vector<pointer_device_info_t> pointer_vec;

    // Methods.
        void query_input_devices__()
        {
            xcb_input_xi_query_device_cookie_t cookie = xcb_input_xi_query_device(conn, XCB_INPUT_DEVICE_ALL);
            xcb_input_xi_query_device_reply_t* reply = xcb_input_xi_query_device_reply(conn, cookie, NULL);

            if (reply == nullptr)
            {
                loutE << "xcb_input_xi_query_device_reply_t == nullptr" << loutEND;
                return;
            }

            xcb_input_xi_device_info_iterator_t iter;
            for (iter = xcb_input_xi_query_device_infos_iterator(reply); iter.rem; xcb_input_xi_device_info_next(&iter))
            {
                xcb_input_xi_device_info_t* device = iter.data;

                char* device_name = (char*)(device + 1); // Device name is stored immediately after the device info structure.
                if (device->type == XCB_INPUT_DEVICE_TYPE_SLAVE_POINTER || device->type == XCB_INPUT_DEVICE_TYPE_FLOATING_SLAVE)
                {
                    pointer_device_info_t pointer_device;
                    pointer_device._device_name = device_name;
                    pointer_device._device_id   = device->deviceid;
                    pointer_vec.push_back(pointer_device);

                    loutI << "Found pointing device: " << device_name << loutEND;
                    loutI << "Device ID" << device->deviceid << loutEND;
                }
            }

            free(reply);
        }

        void make_windows__()
        {
            uint32_t mask;
            int width = (screen->width_in_pixels / 2), height = (screen->height_in_pixels / 2);
            int x = ((screen->width_in_pixels / 2) - (width / 2)), y = ((screen->height_in_pixels / 2) - (height / 2));

            _main_window.create_window(
                screen->root,
                x,
                y,
                width,
                height,
                DARK_GREY,
                NONE,
                MAP | DEFAULT_KEYS
            );
            
            _menu_window.create_window(
                _main_window,
                0,
                0,
                MENU_WINDOW_WIDTH,
                _main_window.height(),
                RED,
                XCB_EVENT_MASK_BUTTON_PRESS,
                MAP
            );
            
            _default_settings_window.create_window(
                _main_window,
                MENU_WINDOW_WIDTH,
                0,
                (_main_window.width() - MENU_WINDOW_WIDTH),
                _main_window.height(),
                ORANGE,
                XCB_EVENT_MASK_BUTTON_PRESS,
                MAP
            );

            _screen_menu_entry_window.create_window(
                _menu_window,
                0,
                0,
                MENU_WINDOW_WIDTH,
                MENU_ENTRY_HEIGHT,
                BLUE,
                XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_EXPOSURE,
                MAP,
                (int[]){DOWN | RIGHT, 2, BLACK}
            );

            _screen_settings_window.create_window(
                _main_window,
                MENU_WINDOW_WIDTH,
                0,
                (_main_window.width() - MENU_WINDOW_WIDTH),
                _main_window.height(),
                WHITE,
                XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS
            );

            _screen_resolution_window.create_window(
                _screen_settings_window,
                100,
                20,
                160,
                20,
                DARK_GREY,
                XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS,
                NONE,
                (int[]){ALL, 2, BLACK}
            );

            _screen_resolution_button_window.create_window(
                _screen_settings_window,
                260,
                20,
                20,
                20,
                DARK_GREY,
                XCB_EVENT_MASK_BUTTON_PRESS,
                NONE,
                (int[]){RIGHT | DOWN | UP, 2, BLACK}
            );

            _audio_menu_entry_window.create_window(
                _menu_window,
                0,
                20,
                MENU_WINDOW_WIDTH,
                MENU_ENTRY_HEIGHT,
                BLUE,
                XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_EXPOSURE,
                MAP,
                (int[]){DOWN | RIGHT, 2, BLACK}
            );

            _audio_settings_window.create_window(
                _main_window,
                MENU_WINDOW_WIDTH,
                0,
                (_main_window.width() - MENU_WINDOW_WIDTH),
                _main_window.height(),
                PURPLE,
                XCB_EVENT_MASK_BUTTON_PRESS
            );

            _network_menu_entry_window.create_window(
                _menu_window,
                0,
                40,
                MENU_WINDOW_WIDTH,
                MENU_ENTRY_HEIGHT,
                BLUE,
                XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_EXPOSURE,
                MAP,
                (int[]){DOWN | RIGHT, 2, BLACK}
            );

            _network_settings_window.create_window(
                _main_window,
                MENU_WINDOW_WIDTH,
                0,
                (_main_window.width() - MENU_WINDOW_WIDTH),
                _main_window.height(),
                MAGENTA
            );

            _input_menu_entry_window.create_window(
                _menu_window,
                0,
                60,
                MENU_WINDOW_WIDTH,
                MENU_ENTRY_HEIGHT,
                BLUE,
                XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_EXPOSURE,
                MAP,
                (int[]){DOWN | RIGHT, 2, BLACK}
            );

            _input_settings_window.create_window(
                _main_window,
                MENU_WINDOW_WIDTH,
                0,
                _main_window.width() - MENU_WINDOW_WIDTH,
                _main_window.height()
            );

            pointer_vec.clear();
            query_input_devices__();
            _input_device_window.create_window(
                _input_settings_window,
                100,
                20,
                160,
                20,
                WHITE,
                XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_EXPOSURE
            );

            _input_device_button_window.create_window(
                _input_settings_window,
                (_input_device_window.x() + _input_device_window.width()),
                _input_device_window.y(),
                20,
                20,
                WHITE,
                XCB_EVENT_MASK_BUTTON_PRESS
            );
        }

        void make_internal_client__()
        {
            c         = new client;
            c->win    = _main_window;
            c->x      = _main_window.x();
            c->y      = _main_window.y();
            c->width  = _main_window.width();
            c->height = _main_window.height();
            c->make_decorations();
            c->frame.set_event_mask(FRAME_EVENT_MASK);
            c->win.set_event_mask(CLIENT_EVENT_MASK);
            wm->client_list.push_back(c);
            wm->cur_d->current_clients.push_back(c);
            c->focus();
            wm->focused_client = c;
        }

        void adjust_and_map_subwindow__(window &__window)
        {
            if (__window.is_mapped()) return;

            if (_default_settings_window.is_mapped()) _default_settings_window.unmap();
            if (_screen_settings_window.is_mapped() ) _screen_settings_window.unmap();
            if (_audio_settings_window.is_mapped()  ) _audio_settings_window.unmap();
            if (_network_settings_window.is_mapped()) _network_settings_window.unmap();
            if (_input_settings_window.is_mapped()  ) _input_settings_window.unmap();

            __window.width_height((c->win.width() - MENU_WINDOW_WIDTH), c->win.height());
            xcb_flush(conn);
            __window.map();
            __window.raise();

            if (__window == _screen_settings_window)
            {
                expose(__window);
                _screen_resolution_window.map();
                expose(_screen_resolution_window);
                _screen_resolution_button_window.map();
            }

            if (__window == _input_settings_window)
            {
                expose(__window);
                uint32_t window_width = 0;
                for (int i = 0; i < pointer_vec.size(); ++i)
                {
                    uint32_t len = pointer_vec[i]._device_name.length() * DEFAULT_FONT_WIDTH;
                    if (len > window_width) window_width = len;
                }

                _input_device_window.width(window_width);
                _input_device_button_window.x(_input_device_window.x() + _input_device_window.width());

                _input_device_window.make_borders(ALL, 2, BLACK);
                _input_device_window.map();
                _input_device_button_window.make_borders(RIGHT | DOWN | UP, 2, BLACK);
                _input_device_button_window.map();
            }
        }

        void show__(uint32_t __window)
        {
            if (__window == _screen_resolution_dropdown_window)
            {
                _screen_resolution_dropdown_window.create_window(
                    _screen_settings_window,
                    100,
                    40,
                    180,
                    (screen_settings->_avalible_resolutions.size() * MENU_ENTRY_HEIGHT),
                    DARK_GREY,
                    NONE,
                    MAP
                );
                _screen_resolution_options_vector.clear();

                for (int i = 0; i < screen_settings->_avalible_resolutions.size(); ++i)
                {
                    window option;
                    option.create_window(
                        _screen_resolution_dropdown_window,
                        0,
                        (_screen_resolution_options_vector.size() * MENU_ENTRY_HEIGHT),
                        _screen_resolution_dropdown_window.width(),
                        MENU_ENTRY_HEIGHT,
                        DARK_GREY,
                        XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_EXPOSURE,
                        MAP,
                        (int[]){RIGHT | LEFT | DOWN, 2, BLACK}
                    );
                    option.draw_text_auto_color(
                        screen_settings->_avalible_resolutions[i].second.c_str(),
                        4,
                        14
                    );
                    _screen_resolution_options_vector.push_back(option);
                }
            }

            if (__window == _input_device_dropdown_window)
            {
                _input_device_dropdown_window.create_window(
                    _input_settings_window,
                    _input_device_window.x(),
                    (_input_device_window.y() + _input_device_window.height()),
                    (_input_device_window.width() + _input_device_button_window.width()),
                    pointer_vec.size() * 20,
                    WHITE,
                    NONE,
                    MAP
                );

                for (int i = 0; i < pointer_vec.size(); ++i)
                {
                    pointer_vec[i]._window.create_window(
                        _input_device_dropdown_window,
                        0,
                        (i * 20),
                        _input_device_dropdown_window.width(),
                        20,
                        WHITE,
                        XCB_EVENT_MASK_EXPOSURE,
                        NONE,
                        (int[]){LEFT | RIGHT | DOWN, 2, BLACK}
                    );
                    pointer_vec[i]._window.map();
                    pointer_vec[i]._window.draw_text_auto_color(pointer_vec[i]._device_name.c_str(), 4, 14, BLACK);
                }
            }
        }

        void hide__(uint32_t __window)
        {
            if (__window == _screen_resolution_dropdown_window)
            {
                _screen_resolution_dropdown_window.unmap();
                _screen_resolution_dropdown_window.kill();
            }

            if (__window == _input_device_dropdown_window)
            {
                _input_device_dropdown_window.unmap();
                _input_device_dropdown_window.kill();
            }
        }

        void setup_events__()
        {
            event_handler->setEventCallback(XCB_BUTTON_PRESS,     [this](Ev ev)-> void
            {
                RE_CAST_EV(xcb_button_press_event_t);
                if (e->detail == L_MOUSE_BUTTON)
                {
                    if (wm->focused_client != this->c)
                    {
                        if (e->event == _main_window
                        ||  e->event == _menu_window
                        ||  e->event == _default_settings_window
                        ||  e->event == _screen_menu_entry_window
                        ||  e->event == _screen_settings_window
                        ||  e->event == _audio_menu_entry_window
                        ||  e->event == _audio_settings_window
                        ||  e->event == _network_menu_entry_window
                        ||  e->event == _network_settings_window)
                        {
                            this->c->focus();
                            wm->focused_client = this->c;
                        }
                    }

                    if (e->event == _menu_window)
                    {
                        adjust_and_map_subwindow__(_default_settings_window);
                        return;
                    }

                    if (e->event == _screen_menu_entry_window)
                    {
                        adjust_and_map_subwindow__(_screen_settings_window);
                        return;
                    }

                    if (e->event == _screen_resolution_button_window)
                    {
                        if (!_screen_resolution_dropdown_window.is_mapped())
                        {
                            show__(_screen_resolution_dropdown_window);
                            return;
                        }

                        hide__(_screen_resolution_dropdown_window);
                        return;
                    }

                    if (e->event == _audio_menu_entry_window)
                    {
                        adjust_and_map_subwindow__(_audio_settings_window);
                        return;
                    }

                    if (e->event == _network_menu_entry_window)
                    {
                        adjust_and_map_subwindow__(_network_settings_window);
                        return;
                    }

                    if (e->event == _input_menu_entry_window)
                    {
                        adjust_and_map_subwindow__(_input_settings_window);
                    }

                    if (e->event == _input_device_button_window)
                    {
                        if (!_input_device_dropdown_window.is_mapped())
                        {
                            show__(_input_device_dropdown_window);
                            return;
                        }

                        hide__(_input_device_dropdown_window);
                        return;
                    }

                    for (int i(0); i < _screen_resolution_options_vector.size(); ++i)
                    {
                        if (e->event == _screen_resolution_options_vector[i])
                        {
                            screen_settings->set_resolution(screen_settings->_avalible_resolutions[i].first);
                            log_info("screen->width_in_pixels: " + to_string(screen->width_in_pixels));
                            log_info("screen->height_in_pixels: " + to_string(screen->height_in_pixels));
                        }
                    }
                }
            });

            event_handler->set_key_press_callback(SUPER, wm->key_codes.s, [this]()-> void { launch(); });

            event_handler->setEventCallback(XCB_CONFIGURE_NOTIFY, [this](Ev ev)->void
            {
                RE_CAST_EV(xcb_configure_notify_event_t); 
                configure(e->window, e->width, e->height);
            });

            event_handler->setEventCallback(XCB_EXPOSE,           [this](Ev ev)->void
            {
                RE_CAST_EV(xcb_expose_event_t);
                expose(e->window);
            });

            SET_INTR_CLI_KILL_CALLBACK();
        }

    public:
    // Variabels.
        window(_main_window),
            (_menu_window), (_default_settings_window),
            (_screen_menu_entry_window), (_screen_settings_window), (_screen_resolution_window), (_screen_resolution_button_window), (_screen_resolution_dropdown_window),
            (_audio_menu_entry_window), (_audio_settings_window),
            (_network_menu_entry_window), (_network_settings_window),
            (_input_menu_entry_window), (_input_settings_window), (_input_device_window), (_input_device_button_window), (_input_device_dropdown_window);
        
        vector<window> _screen_resolution_options_vector;
        client *c = nullptr;

    // Methods.
        void launch()
        {
            make_windows__();
            make_internal_client__();
        }

        void expose(uint32_t __window)
        {
            if (__window == _screen_menu_entry_window ) _screen_menu_entry_window.draw_text_auto_color("Screen", 4, 14);
            if (__window == _screen_settings_window   ) _screen_settings_window.draw_text_auto_color("Resolution", (_screen_resolution_window.x() - (size("Resolution") * DEFAULT_FONT_WIDTH)), 35, BLACK);
            if (__window == _audio_menu_entry_window  ) _audio_menu_entry_window.draw_text_auto_color("Audio", 4, 14);
            if (__window == _network_menu_entry_window) _network_menu_entry_window.draw_text_auto_color("Network", 4, 14);
            if (__window == _input_menu_entry_window  ) _input_menu_entry_window.draw_text_auto_color("Input", 4, 14);
            if (__window == _input_settings_window    ) _input_settings_window.draw_text_auto_color("Device", (_input_device_window.x() - (size("Device") * DEFAULT_FONT_WIDTH)), 35);
            if (__window == _input_device_window      ) _input_device_window.draw_text_auto_color("Select Input Device.", 4, 15, BLACK);
            if (__window == _screen_resolution_window )
            {
                string resolution(screen_settings->_current_resoluton_string);
                if (resolution.empty()) return;
                _screen_resolution_window.draw_text_auto_color(resolution.c_str(), 4, 15);
            }

            for (int i(0); i < _screen_resolution_options_vector.size(); ++i)
            {
                if (__window == _screen_resolution_options_vector[i])
                {
                    _screen_resolution_options_vector[i].draw_text_auto_color(
                        screen_settings->_avalible_resolutions[i].second.c_str(),
                        4,
                        14
                    );
                }
            }

            for (int i = 0; i < pointer_vec.size(); ++i)
            {
                if (__window == pointer_vec[i]._window)
                {
                    pointer_vec[i]._window.draw_text_auto_color(
                        pointer_vec[i]._device_name.c_str(),
                        4,
                        14,
                        BLACK
                    );
                }
            }
        }

        void configure(uint32_t __window, uint32_t __width, uint32_t __height)
        {
            if (__window == _main_window)
            {
                _menu_window.height(__height);
                if (_default_settings_window.is_mapped()) _default_settings_window.width_height((__width - MENU_WINDOW_WIDTH), __height);
                if (_screen_settings_window.is_mapped() ) _screen_settings_window.width_height((__width - MENU_WINDOW_WIDTH), __height);
                if (_audio_settings_window.is_mapped()  ) _audio_settings_window.width_height((__width - MENU_WINDOW_WIDTH), __height);
                if (_network_settings_window.is_mapped()) _network_settings_window.width_height((__width - MENU_WINDOW_WIDTH), __height);
                if (_input_settings_window.is_mapped()  ) _input_settings_window.width_height((__width - MENU_WINDOW_WIDTH), __height);
                xcb_flush(conn);
            }
        }

        void init()
        {
            setup_events__();
        }

    // Constructor.
        __system_settings__() {}

}; static __system_settings__ *system_settings(nullptr); */

/** NOTE: DEPRECATED */
/* class Mwm_Runner {
    public:
    // Variabels.
        window main_window;
        search_window search_window;
        uint32_t BORDER = 2;
        Launcher launcher;
        
    // Methods.
        void init()
        {
            main_window.create_default(
                screen->root,
                (screen->width_in_pixels / 2) - ((140 + (BORDER * 2)) / 2),
                0,
                140 + (BORDER * 2),
                400 + (BORDER * 2)
            );
            uint32_t mask = XCB_EVENT_MASK_STRUCTURE_NOTIFY;
            main_window.apply_event_mask(& mask);
            main_window.set_backround_color(DARK_GREY);
            main_window.grab_button({
                { L_MOUSE_BUTTON, NULL }
            });

            setup_events();
            search_window.create(
                main_window,
                2,
                2,
                main_window.width() - (BORDER * 2),
                main_window.height() - (BORDER * 2)
            );

            search_window.init();
            search_window.add_enter_action([this]()-> void
            {
                launcher.program((char *) search_window.search_string.c_str());
                hide();
            });
        }

        void show()
        {
            main_window.raise();
            main_window.map();
            search_window.main_window.focus_input();
        }

    private:
    // Methods.
        void hide()
        {
            main_window.unmap();
            search_window.search_string.clear();
        }

        void setup_events()
        {
            event_handler->setEventCallback(XCB_KEY_PRESS,    [&](Ev ev)-> void
            {
                RE_CAST_EV(xcb_button_press_event_t);
                if (e->detail == wm->key_codes.r)
                {
                    if (e->state == SUPER)
                    {
                        show();
                    }
                }
            });

            event_handler->setEventCallback(XCB_BUTTON_PRESS, [&](Ev ev)-> void
            {
                RE_CAST_EV(xcb_button_press_event_t);
                if (!main_window.is_mapped()) return;

                if (e->event != main_window && e->event != search_window.main_window)
                {
                    hide();
                }
            });
        }
}; static Mwm_Runner * mwm_runner(nullptr); */


/** FROM: @class @c DropDownTerm -> */

/* for (int i = 0; i < ((screen->height_in_pixels / 2) / 20); ++i)
{
    window window;
    window.create_window(
        w,
        0,
        ((( screen->height_in_pixels / 2 ) - 20 ) - ( i * 20 )),
        screen->width_in_pixels,
        20,
        BLACK,
        NONE,
        MAP
    );
    xcb_flush(conn);
    w_vec.push_back( window );
} */

/* event_handler->setEventCallback(
XCB_KEY_PRESS,
[ this ]( Ev ev ) -> void
{
    RE_CAST_EV( xcb_key_press_event_t );
    if ( e->detail == wm->key_codes.f12 )
    {
        toggle__();
    }
}); */

/** FROM: class window  */

/* bool should_be_decorated()
{
    // Atom for the _MOTIF_WM_HINTS property, used to control window decorations
    const char* MOTIF_WM_HINTS = "_MOTIF_WM_HINTS";
    
    // Get the atom for the _MOTIF_WM_HINTS property
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, 0, strlen(MOTIF_WM_HINTS), MOTIF_WM_HINTS);
    xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(conn, cookie, NULL);
    if (!reply)
    {
        loutE << "reply = nullptr" << loutEND;
        return true; // Default to decorating if we can't check
    }

    xcb_atom_t motif_wm_hints_atom = reply->atom; free(reply);

    // Try to get the _MOTIF_WM_HINTS property from the window
    xcb_get_property_cookie_t prop_cookie = xcb_get_property(conn, 0, _window, motif_wm_hints_atom, XCB_ATOM_ANY, 0, sizeof(uint32_t) * 5);
    xcb_get_property_reply_t* prop_reply = xcb_get_property_reply(conn, prop_cookie, NULL);

    if (prop_reply && xcb_get_property_value_length(prop_reply) >= sizeof(uint32_t) * 5)
    {
        uint32_t* hints = (uint32_t*)xcb_get_property_value(prop_reply);
        
        // Check if decorations are disabled
        bool decorate = !(hints[1] & (1 << 1));

        free(prop_reply);
        return decorate;
    }

    if (prop_reply)
    {
        free(prop_reply);
    }

    // Default to decorating if we can't find or interpret the hints
    return true;
} */

/* void print_window_states()
{
    xcb_intern_atom_reply_t *atom_r = XCB::atom_r(XCB::atom_cok("_NET_WM_STATE"));
    if (atom_r == nullptr)
    {
        loutEWin << "Failed to get _NET_WM_STATE atom." << loutEND;
        return;
    }

    xcb_atom_t atom = atom_r->atom;
    free(atom_r);

    xcb_get_property_cookie_t property_cookie = xcb_get_property(conn, 0, _window, atom, XCB_ATOM_ATOM, 0, 1024);
    xcb_get_property_reply_t* property_reply = xcb_get_property_reply(conn, property_cookie, NULL);
    if ( !property_reply )
    {
        loutEWin << "Failed to get property." << loutEND;
        return;
    }
    
    xcb_atom_t* atoms = static_cast<xcb_atom_t*>(xcb_get_property_value(property_reply));
    int atom_count = xcb_get_property_value_length(property_reply) / sizeof(xcb_atom_t);

    for (int i = 0; i < atom_count; ++i)
    {
        xcb_get_atom_name_cookie_t atom_name_cookie = xcb_get_atom_name(conn, atoms[i]);
        xcb_get_atom_name_reply_t* atom_name_reply = xcb_get_atom_name_reply(conn, atom_name_cookie, NULL);

        if (atom_name_reply)
        {
            int name_len = xcb_get_atom_name_name_length(atom_name_reply);
            char* name = xcb_get_atom_name_name(atom_name_reply);
            loutIWin << "Window   State: " << string(name, name_len) << loutEND;
            free(atom_name_reply);
        }
    }

    free(property_reply);
} */

/** <- END: */
