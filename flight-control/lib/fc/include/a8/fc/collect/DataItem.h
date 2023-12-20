#pragma once
#include "a8/util.h"

namespace a8::fc::collector {
using namespace a8::util;
class DataItem {
public:
    String name;
    DataItem(String name) {
        this->name = name;
    }
    virtual float get();
};
template <typename C>
class FunctionalDataItem : public DataItem {
    using getter = float (*)(C);

public:
    C context;
    getter getter_;
    FunctionalDataItem(String name, C context, getter getter) : DataItem(name) {
        this->context = context;
        this->getter_ = getter;
    }
    float get() override {
        return this->getter_(this->context);
    }
};
} // namespace a8::fc::collector