#pragma once
#include "a8/util.h"
namespace a8::fc::collect {
using namespace a8::util;
class DataItem {
public:
    String name;
    const Format::Float *format;

    DataItem(String name, const Format::Float *format) {
        this->name = name;
        this->format = format;
    }

    String getName() {
        return name;
    }

    virtual double get() = 0;
};

template <typename T>
class BindDataItem : public DataItem {

public:
    T &bind_;
    BindDataItem(String name, T &bind, const Format::Float *format) : DataItem(name, format), bind_(bind) {
    }
    double get() override {        
        return double(this->bind_);
    }
};

template <typename C>
class FunctionalDataItem : public DataItem {
    using getter = double (*)(C);

public:
    getter getter_;
    C c2;
    FunctionalDataItem(String name, C c2, getter getter, const Format::Float *format) : DataItem(name, format), c2(c2) {
        this->getter_ = getter;
    }
    double get() override {
        return this->getter_(c2);
    }
};
} // namespace a8::fc::collect