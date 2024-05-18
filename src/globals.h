#ifndef GLOBALS_H
#define GLOBALS_H

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <vector>

#define U32_MAX 0xFFFFFFFF

#define FRAMERATE 120

#define CLEAR_UNCLOSED_CLIENTS 233

#define FRAMETIME (double)1000 / FRAMERATE

extern xcb_connection_t *conn;
extern xcb_screen_t *screen;

static xcb_font_t default_font;

typedef xcb_void_cookie_t VoidC;

#endif/* GLOBALS_H */