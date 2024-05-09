#ifndef SIGNAL_H
#define SIGNAL_H

#include <cstdint>
#include <functional>
#include <xcb/xcb.h>

#include "prof.hpp"
#include "globals.h"
#include "defenitions.hpp"

using namespace std;

/**
*****************************************
*****************************************
**** @class @c __window_signals__
*****************************************
****************************************/
class __window_signals__
{
    public:
        umap<uint32_t, umap<int, function<void(uint32_t)>>> _data;

        template<typename Callback>
        void connect(uint32_t __w, uint8_t __sig, Callback &&__cb);

        void emit(uint32_t __w, uint8_t __sig);

        void emit(uint32_t __w, uint8_t __sig, uint32_t __w2);

        void remove(uint32_t __w);
};


/**
*****************************************
*****************************************
**** @class @c __ev_sigs
*****************************************
****************************************/
class __ev_sigs
{
    /* Defines */
        #define ConnEvSig(__w, __sig, __cb) \
        do { \
            ev_sigs->connect(__w, __sig, [this](const vector<uint32_t> &ev) -> void { __cb }); \
        } while(false)

    public:
    /* Variabels */
        umap<uint32_t, umap<int, function<void(vector<uint32_t>)>>> _data;

    /* Methods */
        template<typename Callback>
        void connect(uint32_t __w, uint8_t __sig, Callback &&__cb);

        void emit(uint32_t __w, uint8_t __sig, const vector<uint32_t> &__event_vec);
        
        void remove(uint32_t __w);
};
static __ev_sigs *ev_sigs(nullptr);

#endif/* SIGNAL_H */