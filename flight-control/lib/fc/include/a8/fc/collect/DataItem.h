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
    String getName() {
        return name;
    }

    virtual double get() = 0;
};

template <typename C>
class FunctionalDataItem : public DataItem {
    using getter = double (*)(C);

public:
    getter getter_;
    C c2;
    FunctionalDataItem(String name, C c2, getter getter) : DataItem(name), c2(c2) {
        this->getter_ = getter;
    }
    double get() override {
        return this->getter_(c2);
    }
};
} // namespace a8::fc::collect