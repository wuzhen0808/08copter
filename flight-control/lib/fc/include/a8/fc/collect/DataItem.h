#pragma once
#include "a8/util.h"

namespace a8::fc::collect {
using namespace a8::util;
class DataItem {
public:
    String name;
    DataItem(String name) {
        this->name = name;
    }
    String getName(){
        return name;
    }
    virtual double get() = 0;
};
template <typename C>
class FunctionalDataItem : public DataItem {
    using getter = double (*)(C);

public:
    C context;
    getter getter_;
    FunctionalDataItem(String name, C context, getter getter) : DataItem(name) {
        this->context = context;
        this->getter_ = getter;
    }
    double get() override {
        return this->getter_(this->context);
    }
};
} // namespace a8::fc::collector