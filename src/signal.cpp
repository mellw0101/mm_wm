#include "signal.h"

/**
*****************************************
*****************************************
**** @class @c __window_signals__
*****************************************
****************************************/
template<typename Callback>
void __window_signals__::connect(uint32_t __w, uint8_t __sig, Callback &&__cb)
{
    _data[__w][__sig] = std::forward<Callback>(__cb);
}

void __window_signals__::emit(uint32_t __w, uint8_t __sig)
{
    AutoTimer t("__window_signals__:" + string(__func__) + ":1");

    auto it = _data[__w].find(__sig);
    if (it == _data[__w].end()) return;
    it->second(__w);
}

void __window_signals__::emit(uint32_t __w, uint8_t __sig, uint32_t __w2)
{
    AutoTimer t("__window_signals__:" + string(__func__) + ":2");

    auto it = _data[__w].find(__sig);
    if (it == _data[__w].end()) return;
    it->second(__w2);
}

void __window_signals__::remove(uint32_t __w)
{
    auto it = _data.find(__w);
    if (it == _data.end()) return;
    _data.erase(it);
}

/**
*****************************************
*****************************************
**** @class @c __ev_sigs
*****************************************
****************************************/
template<typename Callback>
void __ev_sigs::connect(uint32_t __w, uint8_t __sig, Callback &&__cb)
{
    _data[__w][__sig] = std::forward<Callback>(__cb);
}

void __ev_sigs::emit(uint32_t __w, uint8_t __sig, const vector<uint32_t> &__event_vec)
{
    AutoTimer t("__ev_sigs::emit");

    auto it = _data[__w].find(__sig);
    if (it == _data[__w].end()) return;
    it->second(__event_vec);
}

void __ev_sigs::remove(uint32_t __w)
{
    auto it = _data.find(__w);
    if (it == _data.end()) return;
    _data.erase(it);
}