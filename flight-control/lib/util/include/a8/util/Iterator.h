#pragma once
namespace a8::util {
template <typename T>
class Iterator {
    virtual bool hasNext() = 0;
    virtual T next() = 0;
};
} // namespace a8::util