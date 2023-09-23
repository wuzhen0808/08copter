#pragma once
#include "a8/util/Attributes.h"
#include "a8/util/Buffer.h"
#include "a8/util/Context.h"
#include "a8/util/Runnable.h"
#include "a8/util/String.h"

using namespace a8::util;

namespace a8::util {

class Component {

protected:
    Attributes *attributes;

public:
    Component();
    ~Component();

    void log(a8::util::String msg);

    Attributes *getAttributes();

    virtual void populate(Context* context) {

    }

    virtual void setup(Context* context) {

    }
    
    virtual void start(Context* context) {

    }
};
} // namespace a8::util

