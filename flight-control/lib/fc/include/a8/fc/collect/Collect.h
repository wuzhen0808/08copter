#pragma once
#include "a8/util.h"
namespace a8::fc::collect {
using namespace a8::util;

const Format::AutoOffsetFloat doubleFormat(15, 3);
const Format::AutoOffsetFloat floatFormat(8, 3); // only for faster format than double.
const Format::AutoOffsetFloat noTailDoubleFormat(15, 0);
const Format::AutoOffsetFloat longFormat(15, 0);
const Format::AutoOffsetFloat noTailFloatFormat(8, 0); // only  for faster format than double.
const Format::AutoOffsetFloat intFormat(8, 0); // only  for faster format than double.
const Format::AutoOffsetFloat boolFormat(1, 0); // only  for faster format than double.
class DataItem;
class Collect {
private:
    Collect(const Collect &);
    Collect &operator=(const Collect &);

protected:
    // first row num is 1, not 0;
    long rowNum = 0;

public:
    Collect() {
    }
    long getRowNum() {
        return this->rowNum;
    }

    virtual DataItem *get(String name) = 0;
};
} // namespace a8::fc::collect