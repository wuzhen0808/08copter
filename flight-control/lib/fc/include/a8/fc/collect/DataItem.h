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
    virtual int setup(Collect *collect, Result &res) = 0;
    
    virtual double get(Collect * collect) = 0;
};

} // namespace a8::fc::collect