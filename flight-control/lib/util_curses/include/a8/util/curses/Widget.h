#pragma once

#include "a8/util.h"
#include "a8/util/curses/defines.h"
#include <cdk.h>

namespace a8::util::curses {

template <typename T>
class Widget {
protected:
    T *widget;

public:
    Widget(T *widget) {
        this->widget = widget;
    }

    virtual void draw() = 0;
    
    virtual ~Widget() {}
};
} // namespace a8::util::curses