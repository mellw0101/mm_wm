#ifndef EVENT__HPP
#define EVENT__HPP

#include <cstdint>
#include <functional>
#include <utility>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <vector>

#include "globals.h"
#include "defenitions.hpp"

using namespace std;

class __event_signals__
{
    private:
        umap<uint8_t, function<void(const vector<uint32_t> &ev)>> _event_map;
    
    public:
        template<typename Cb>
        void connect(uint8_t __sig, Cb &&__cb)
        {
            _event_map[__sig] = std::forward<Cb>(__cb);
        }

        void emit(uint32_t __window, uint8_t __sig, const vector<uint32_t> &__ev)
        {
            auto it = _event_map.find(__sig);
            if (it == _event_map.end()) return;
            it->second(__ev);
        }

        __event_signals__() {}
};

template<uint8_t EventType> static constexpr void handle_event(xcb_generic_event_t *ev) { return; };

template<> constexpr void handle_event<XCB_EXPOSE>(xcb_generic_event_t *ev) {  }

class __event__handler
{
    private:
        unordered_map<int, function<void(xcb_generic_event_t *)>> map;
        xcb_generic_event_t *ev;

        void makeMap_()
        {
            map[XCB_EXPOSE] = [this](xcb_generic_event_t *ev) -> void
            {
            
            };
        }

    public:
        template<typename Func>
        void addCb(int __s, Func &&func)
        {
            map[__s] = std::forward<Func>(func);
        }
            
        void run()
        {
            while ((ev = xcb_wait_for_event(conn)) != nullptr)
            {
                uint8_t responseType = ev->response_type & ~0x80;
                if (map.find(responseType) != map.end())
                {
                    map[responseType](ev);
                }
                free(ev);
            }
        }
            
        __event__handler(xcb_connection_t *conn) : ev(new xcb_generic_event_t) {}
};

#endif